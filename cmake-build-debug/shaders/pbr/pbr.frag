#version 330 core

#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_texture_query_levels      : enable

const float EPSILON = 0.005;
const float      PI = 3.141593;

in vec2 v_TexCoord;
in vec3 v_Position;
in vec3 v_Normal;

in mat3 v_TBN;

in vec4 v_Position_LightSpace;

uniform vec3 u_CameraPosition;   // Camera position. Mainly used for lighting calculations.

/* PBR */
layout (location = 0) uniform sampler2D u_Albedo;
layout (location = 1) uniform sampler2D u_Roughness;
layout (location = 2) uniform sampler2D u_Metallic;
layout (location = 3) uniform sampler2D u_Normals;
layout (location = 4) uniform sampler2D u_Height;
layout (location = 5) uniform sampler2D u_Detail;
layout (location = 6) uniform sampler2D u_AO;
layout (location = 7) uniform sampler2D u_Emission;

/* AMBIENT LIGHTING */

//#define SAMPLER_CUBE

// Ambient lighting texture.
#ifdef SAMPLER_CUBE
layout (location = 98) uniform samplerCube u_Ambient;
#else
layout (location = 98) uniform sampler2D u_Ambient;
#endif

uniform float u_AmbientExposure = 1.0; // Brightness of ambient texture.

/* SHADOWS */
layout (location =  99) uniform sampler2D   u_ShadowMap2D;
layout (location = 100) uniform samplerCube u_ShadowMap3D;

const float PCSS_SCENE_SCALE = 0.015625; // Scale of shadow blur (PCSS only).

uniform float u_ShadowBias       = 0.005; // Shadow bias.
uniform float u_ShadowNormalBias = 0.1;   // Shadow normal bias.
uniform float u_NearPlane        = 0.1;   // Light's shadow near plane.

uniform int u_ShadowSamples = 10; // Number of shadow samples. Please choose a sane value.

uniform float  u_Metallic_Amount = 0.0; // How metallic the surface is.
uniform float u_Roughness_Amount = 0.0; // How rough the surface is.
uniform float  u_Emission_Amount = 1.0; // How emissive the surface is.
uniform float    u_Normal_Amount = 1.0; // Contribution of normal map.

uniform float u_Time;

/* TEXTURE PARAMETERS */
uniform vec2 u_Tiling = vec2(1.0); // Texture tiling scale (U, V).
uniform vec2 u_Offset = vec2(0.0); // Texture offset       (U, V)

/* DIRECT LIGHTING */

uniform  vec3 u_LightPosition;  // Position of light in world-space.
uniform  vec3 u_LightDirection; // Direction of the light in world-space.
uniform float u_LightRange;     // Range of light.
uniform float u_LightIntensity; // Brightness of light.
uniform  vec3 u_LightColor;     // Color of light.
uniform float u_LightSize       =  0.2; // Size of the light (PCSS only).
uniform float u_LightAngle      = -1.0; // Cos of light's FOV (for spot lights).

/* THIRD-PARTY */

// Gold Noise ©2015 dcerisano@standard3d.com
// - based on the Golden Ratio
// - uniform normalized distribution
// - fastest static noise generator function (also runs at low precision)
// - use with indicated fractional seeding method.

float PHI = 1.61803398874989484820459;  // Φ = Golden Ratio

float gold_noise(in vec2 xy, in float seed){
    return fract(tan(distance(xy*PHI, xy)*seed)*xy.x);
}

/* NOT THIRD-PARTY */

float Random1(in vec2 _xy, float _offset) {
    return (gold_noise(_xy * 1000.0, fract(u_Time) + _offset) - 0.5) * 2.0;
}

vec2 Random2(in vec2 _xy, float _offset) {

    return vec2(
        Random1(_xy, _offset + 0.1),
        Random1(_xy, _offset + 0.2)
    );
}

vec3 Random3(in vec3 _xyz, float _offset) {

    return vec3(
        Random1(_xyz.xy, _offset + 0.1),
        Random1(_xyz.yz, _offset + 0.2),
        Random1(_xyz.xz, _offset + 0.3)
    );
}

// Couldn't find a square distance function so made my own. Does GLSL not have one?
float length2(vec3 _A, vec3 _B) {

    vec3 delta = _B - _A;

    return dot(delta, delta);
}

// Light falloff with inverse square law.
float Attenuation(vec3 _lightPosition, vec3 _fragPosition, float _range) {
    return _range / length2(_lightPosition, _fragPosition);
}

vec3 Irradiance(vec3 _fresnel, float _metallic) {
    return mix(vec3(1.0) - _fresnel, vec3(0.0), _metallic);
}

float GeomG1(float _theta, float _k) {
    return _theta / (_theta * (1.0 - _k) + _k);
}

float Geom(float _cosLi, float _cosLo, float _roughness) {

    float r = _roughness + 1.0;
    float k = (r * r) / 8.0;

    return GeomG1(_cosLi, k) * GeomG1(_cosLo, k);
}

float Distrib(float _cosLh, float _roughness) {

    float alpha = _roughness * _roughness;
    float alphaSq = alpha * alpha;

    float denom = (_cosLh * _cosLh) * (alphaSq - 1.0) + 1.0;
    return alphaSq / (PI * denom * denom);
}

vec3 Fresnel(vec3 _metallic, float _theta) {
    return _metallic + (vec3(1.0) - _metallic) * pow(1.0 - _theta, 5.0);
}

vec3 BRDF(vec3 _albedo, vec3 _normal, vec3 _lightDir, vec3 _viewDir, vec3 _halfVec, float _metallic, float _roughness) {

    // Calculate angles between surface normal and various light vectors.
    float NdL = max(0.0, dot(_normal, _lightDir));
    float NdH = max(0.0, dot(_normal, _halfVec));
    float NdV = max(0.0, dot(_normal, _viewDir));

    vec3 F0 = mix(vec3(0.04), _albedo, _metallic);

    vec3 fresnel = Fresnel(F0, max(0.0, dot(_halfVec, _viewDir)));

    vec3 diffuse = Irradiance(fresnel, _metallic);

    vec3 specular =
        (fresnel * Distrib(NdH, _roughness) *
        Geom(NdL, NdV, _roughness)) /
        max(EPSILON, 4.0 * NdL * NdV);

    return (diffuse + specular) * NdL;
}

float PCSS_GetOccluderDepth3D(vec3 _dir, float _texelSize, float _bias) {

    float result = 0.0;

    int hits = 0;

    for (int i = 0; i < u_ShadowSamples; i++) {

        vec3 offset = normalize(Random3(_dir + vec3(i + 1), 0.1));

        offset *= Random1(_dir.xy + vec2(i + 1), 0.4);

        float occluderDepth = texture(u_ShadowMap3D, _dir + offset).r;

        if (occluderDepth < length(_dir) - _bias) {
            result += occluderDepth;

            hits++;
        }
    }

    if (hits == 0) {
        result = -1;
    }

    return result / float(max(hits, 1));
}

float PCSS_Radius3D(vec3 _dir, float _texelSize, float _bias) {

    float occluderDepth = PCSS_GetOccluderDepth3D(_dir, _texelSize, _bias);

    if (occluderDepth == -1) {
        return 1;
    }

    float len = length(_dir);

    float penumbraWidth = (len - occluderDepth) / occluderDepth;

    return penumbraWidth * u_LightSize;
}

float ShadowCalculationHard3D(vec3 _dir, vec3 _offset, float _bias) {

    // Get depth from light to closest surface.
    float shadowDepth = texture(u_ShadowMap3D, _dir + _offset).r * u_LightRange;

    // Compare the depth of the light against the distance from the light to the current fragment.
    return length(_dir) - _bias > shadowDepth ? 1.0 : 0.0;
}

float ShadowCalculationPCF3D(vec3 _dir, float _texelSize, float _bias, float _radius) {

    float result = 0.0;

    float hits = 0;

    float PCF_SAMPLES = 8;

    float multiplier = 50.0; // Compensation for 3D.

    float axis = pow(PCF_SAMPLES, 1.0 / 3.0);
    for (float x = -_radius; x < _radius; x += _radius / axis) {
    for (float y = -_radius; y < _radius; y += _radius / axis) {
    for (float z = -_radius; z < _radius; z += _radius / axis) {

        vec3 offset = vec3(x, y, z);

        offset *= _texelSize * multiplier;

        result += ShadowCalculationHard3D(_dir, offset, _bias);

        hits += 1.0;
    }}}

    return result / max(hits, 1);
}

float ShadowCalculationDisk3D(vec3 _dir, float _texelSize, float _bias, float _radius) {

    float result = 0.0;

    for (int i = 0; i < u_ShadowSamples; i++) {

        vec3 offset = normalize(Random3(_dir + vec3(i + 1), 0.1));

        offset *= _texelSize * _radius * Random1(_dir.xy + vec2(i + 1), 0.4);

        result += ShadowCalculationHard3D(_dir, offset, _bias);
    }

    return result / u_ShadowSamples;
}

float ShadowCalculationPCSS3D(vec3 _dir, float _texelSize, float _bias) {

    float result = 0.0;

    float radius = PCSS_Radius3D(_dir, _texelSize, _bias);

    result = ShadowCalculationDisk3D(_dir, _texelSize, _bias, radius);

    return result;
}

float TransferShadow3D(vec3 _normal, vec3 _lightDir, float _bias, float _normalBias) {

    float texelSize = 1.0 /
        max(textureSize(u_ShadowMap3D, 0).x, 1);

    vec3 fragToLight = v_Position - u_LightPosition;

    float perspective_multiplier = 32.0;

    float adjustedBias =
        texelSize * perspective_multiplier * max(_normalBias * (1.0 - dot(_normal, _lightDir)), _bias);

    //return ShadowCalculationHard3D(fragToLight, vec3(0), adjustedBias);
    //return ShadowCalculationPCF3D(fragToLight, texelSize, adjustedBias, 1.0f);
    //return ShadowCalculationDisk3D(fragToLight, texelSize, adjustedBias, 1.0f);
    return ShadowCalculationPCSS3D(fragToLight, PCSS_SCENE_SCALE, adjustedBias);
}

float PCSS_GetOccluderDepth2D(vec3 _fragPos, float _texelSize, float _bias) {

    float result = 0.0;

    int hits = 0;

    for (int i = 0; i < u_ShadowSamples; i++) {

        vec2 dir = Random2(_fragPos.xy + vec2(i + 1), 0.1);

        dir *= float(i + 1) * _texelSize;

        float occluderDepth = texture(u_ShadowMap2D, _fragPos.xy + dir).r;

        if (occluderDepth < _fragPos.z - _bias) {
            result += occluderDepth;

            hits++;
        }
    }

    if (hits == 0) {
        result = -1;
    }

    return result / float(max(hits, 1));
}

float PCSS_Radius2D(vec3 _fragPos, float _texelSize, float _bias) {

    float occluderDepth = PCSS_GetOccluderDepth2D(_fragPos, _texelSize, _bias);

    if (occluderDepth == -1) {
        return 1;
    }

    float penumbraWidth = (_fragPos.z - occluderDepth) / occluderDepth;

    return penumbraWidth * u_LightSize * (u_NearPlane / _fragPos.z);
}

float ShadowCalculationHard2D(vec3 _fragPos, vec2 _offset, float _bias) {

    // Get depth from light to closest surface.
    float shadowDepth = texture(u_ShadowMap2D, _fragPos.xy + _offset).r;

    // Compare the depth of the light against the distance from the light to the current fragment (converted to light space).
    return (_fragPos.z <= 1.0) && (_fragPos.z - _bias > shadowDepth) ?
        1.0 : 0.0;
}

float ShadowCalculationPCF2D(vec3 _fragPos, float _texelSize, float _bias, float _radius) {

    float result = 0.0;

    float hits = 0;

    float PCF_SAMPLES = 4;

    float axis = pow(PCF_SAMPLES, 1.0 / 2.0);
    for (float x = -_radius; x < _radius; x += _radius / axis) {
    for (float y = -_radius; y < _radius; y += _radius / axis) {

        vec2 dir = vec2(x, y);

        dir *= _texelSize;

        result += ShadowCalculationHard2D(_fragPos, dir, _bias);

        hits += 1.0;
    }}

    return result / max(hits, 1);
}

float ShadowCalculationDisk2D(vec3 _fragPos, float _texelSize, float _bias, float _radius) {

    float result = 0.0;

    for (int i = 0; i < u_ShadowSamples; i++) {

        vec2 dir = normalize(Random2(_fragPos.xy + vec2(i + 1), 0.1));

        dir *= _texelSize * _radius * (float(i + 1) / float(u_ShadowSamples));

        result += ShadowCalculationHard2D(_fragPos, dir, _bias);
    }

    return result / u_ShadowSamples;
}

float ShadowCalculationPCSS2D(vec3 _fragPos, float _texelSize, float _bias) {

    float result = 0.0;

    float perspective_multiplier = u_LightAngle == -1.0 ? 1.0 : 32.0;

    float radius = PCSS_Radius2D(_fragPos, _texelSize, _bias) / _texelSize;

    result = ShadowCalculationDisk2D(_fragPos, _texelSize, _bias, radius * perspective_multiplier);

    return result;
}

float TransferShadow2D(vec4 _fragPosLightSpace, vec3 _normal, vec3 _lightDir, float _bias, float _normalBias) {

    float texelSize = 1.0 /
        max(textureSize(u_ShadowMap2D, 0).x, 1);

    vec3 projCoords =
        ((_fragPosLightSpace.xyz / _fragPosLightSpace.w) * 0.5) + 0.5;

    float perspective_multiplier = u_LightAngle == -1.0 ? 1.0 : 32.0;

    float adjustedBias =
        texelSize * perspective_multiplier * max(_normalBias * (1.0 - dot(_normal, _lightDir)), _bias);

    //return ShadowCalculationHard2D(projCoords, vec2(0), adjustedBias);
    //return ShadowCalculationPCF2D(projCoords, texelSize, adjustedBias, 1.0f);
    //return ShadowCalculationDisk2D(projCoords, texelSize, adjustedBias, 1.0f);
    return ShadowCalculationPCSS2D(projCoords, pow(PCSS_SCENE_SCALE, 2.0) * 2.0, adjustedBias);
}

vec2 ScaleTexCoord(sampler2D _texture, in vec2 _coord) {
    return (_coord + fract(u_Offset)) * u_Tiling;
}

vec3 SampleAmbient(in vec3 _dir, float _blur) {

    vec3 result;

    float s = textureSize(u_Ambient, 0).x;

    float levels = log2(s);

    int b = int(pow(_blur, levels) * levels);

    #ifdef SAMPLER_CUBE

        // Sample the cubemap the direction directly.
        result = texture(u_Ambient, _dir, b).rgb;
    #else

        // Sample the texture2d by converting the direction to a uv coordinate.
        // See the example given on: https://en.wikipedia.org/wiki/UV_mapping

        vec2 uv = vec2(
            0.5 + ((atan(_dir.z, _dir.x) / PI) / 2.0),
            0.5 - (asin(_dir.y) / PI)
        );

        // Fix seam at wrap-around point.
        float threshold = 4.0 / textureSize(u_Ambient, b).x;

        float check = step(fract(uv.x - threshold), 1.0 - (threshold * 2.0));

        //uv.x *= check;
        //result = texture(u_Texture, uv, b).rgb;

        // Seemingly no way to do this without a branch.
        // Compiler seems to need an explicit branch to 'get it'.
        //
        // See for yourself by commenting out the following code
        // and uncommenting the previous code.

        if (check == 0.0) {
            result = texture(u_Ambient, vec2(0.0, uv.y), b).rgb;
        }
        else {
            result = texture(u_Ambient, uv, b).rgb;
        }

    #endif

    return result * u_AmbientExposure;
}

void main() {

    vec4     albedo = texture(u_Albedo,    ScaleTexCoord(u_Albedo,    v_TexCoord));
    float roughness = texture(u_Roughness, ScaleTexCoord(u_Roughness, v_TexCoord)).r * u_Roughness_Amount;
    float  metallic = texture(u_Metallic,  ScaleTexCoord(u_Metallic,  v_TexCoord)).r *  u_Metallic_Amount;

    vec3 n = texture(u_Normals, ScaleTexCoord(u_Normals, v_TexCoord)).rgb;
    n = normalize((n * 2.0) - 1.0);

    vec3 normal = mix(normalize(v_Normal), normalize(v_TBN * n), u_Normal_Amount);

    float height = texture(u_Height,  ScaleTexCoord(u_Height, v_TexCoord)).r;
    vec3  detail = texture(u_Detail,  ScaleTexCoord(u_Detail, v_TexCoord), 0).rgb;
    float     ao = texture(u_AO,      ScaleTexCoord(u_AO,     v_TexCoord)).r;

    vec3 emission = texture(u_Emission, ScaleTexCoord(u_Emission, v_TexCoord)).rgb * log2(u_Emission_Amount + 1.0);

    vec3  viewDir = normalize(u_CameraPosition - v_Position);
    vec3 lightDir = normalize(u_LightPosition - v_Position);
    vec3  halfVec = normalize(lightDir + viewDir);

    /* DIRECT */

    vec3 directLighting;
    {
        float attenuation = Attenuation(u_LightPosition, v_Position, u_LightRange);

        float visibility =
            (dot(u_LightDirection, lightDir) > u_LightAngle ? 1 : 0) *
            //(1.0 - TransferShadow2D(v_Position_LightSpace, normal, lightDir, u_ShadowBias, u_ShadowNormalBias));
            (1.0 - TransferShadow3D(normal, lightDir, u_ShadowBias, u_ShadowNormalBias));

        vec3 lighting = BRDF(
            albedo.rgb + detail,
            normal,
            lightDir,
            viewDir,
            halfVec,
            metallic,
            roughness
        ) * u_LightIntensity * attenuation;

        directLighting += (visibility * lighting) * u_LightColor;
    }

    directLighting *= albedo.rgb;

    /* INDIRECT */

    vec3 indirectLighting;

    {
        // Figure out the direction of the ambient light
        vec3 ambientDir = reflect(-viewDir, normal);

        // Sample at max mip level for the diffuse.
        vec3 diffuse = SampleAmbient(normal, 0.95);

        // Sample at variable mip level (determined by roughness) for specular.
        vec3 specular = SampleAmbient(ambientDir, roughness);

        vec3 F0 = mix(vec3(0.04), vec3(1.0), metallic);

        vec3 fresnel = Fresnel(F0, max(0.0, dot(normal, ambientDir)));

        //diffuse;//
        indirectLighting = (((diffuse * (1.0 - metallic)) * albedo.rgb) + (fresnel * specular)) * ao;
    }

    //vec4(indirectLighting, 1);//
    gl_FragColor = vec4((directLighting + indirectLighting) + emission, 1.0);
}