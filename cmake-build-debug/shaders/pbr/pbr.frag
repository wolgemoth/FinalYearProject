#version 330 core

#extension GL_ARB_texture_query_levels : enable

const float EPSILON = 0.005;
const float      PI = 3.141593;

in vec2 v_TexCoord;
in vec3 v_Position;
in vec3 v_Normal;
in vec4 v_Position_LightSpace;

uniform sampler2D u_Albedo;
uniform sampler2D u_Normals;

uniform sampler2D   u_ShadowMap2D;
uniform samplerCube u_ShadowMap3D;

uniform float u_Time;

// By default is set to 1 to deliberately look horrible.
// Please assign a value between 10 and 100 when implementing.
uniform int u_ShadowSamples = 1;

const float PCSS_SCENE_SCALE = 0.015625;

uniform float u_LightSize  = 0.2;
uniform float u_LightAngle = 0.0;
uniform float u_NearPlane  = 0.1;

/* PARAMETERS */
uniform  vec3 u_CameraPosition;   // Camera position. Mainly used for lighting calculations.
uniform float u_Metallic;         // How metallic the surface is.
uniform float u_Roughness;        // How rough the surface is.

/* LIGHTING */
uniform samplerCube u_Ambient;
uniform float u_AmbientExposure = 1.6;

uniform float u_ShadowBias = 0.005;
uniform float u_ShadowNormalBias = 0.1;

uniform  vec3 u_LightPosition;  // Position of light in world-space.
uniform vec3  u_LightDirection; // Direction of the light in world-space.
uniform float u_LightRange;     // Range of light.
uniform float u_LightIntensity; // Brightness of light.
uniform  vec3 u_LightColor;     // Color of light.

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

    float adjustedBias =
        texelSize * u_LightRange * max(_normalBias * (1.0 - dot(_normal, _lightDir)), _bias) * 2;

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
        texelSize * u_LightRange / perspective_multiplier * max(_normalBias * (1.0 - dot(_normal, _lightDir)), _bias);

    //return ShadowCalculationHard2D(projCoords, vec2(0), adjustedBias);
    //return ShadowCalculationPCF2D(projCoords, texelSize, adjustedBias, 1.0f);
    //return ShadowCalculationDisk2D(projCoords, texelSize, adjustedBias, 1.0f);
    return ShadowCalculationPCSS2D(projCoords, pow(PCSS_SCENE_SCALE, 2.0) * 2.0, adjustedBias);
}

void main() {

    vec4 albedo = texture2D(u_Albedo, v_TexCoord);

    vec3  viewDir = normalize(u_CameraPosition - v_Position);
    vec3 lightDir = normalize(u_LightPosition - v_Position);
    vec3  halfVec = normalize(lightDir + viewDir);

    vec3 normal = normalize(
        v_Normal +
        ((texture(u_Normals, v_TexCoord).rgb * 2.0) - 1.0) * 0.08
    );

    /* DIRECT */

    vec3 directLighting;
    {
        float attenuation = Attenuation(u_LightPosition, v_Position, u_LightRange);

        float visibility =
            (dot(u_LightDirection, lightDir) > u_LightAngle ? 1 : 0) *
            //(1.0 - TransferShadow2D(v_Position_LightSpace, normal, lightDir, u_ShadowBias, u_ShadowNormalBias));
            (1.0 - TransferShadow3D(normal, lightDir, u_ShadowBias, u_ShadowNormalBias));

        vec3 lighting = BRDF(
            albedo.rgb,
            normal,
            lightDir,
            viewDir,
            halfVec,
            u_Metallic,
            u_Roughness
        ) * u_LightIntensity * attenuation;

        directLighting += (visibility * lighting) * u_LightColor;
    }

    directLighting *= albedo.rgb;

    /* INDIRECT */

    vec3 indirectLighting;

    {
        // Figure out the direction of the ambient light
        vec3 ambientDir = reflect(-viewDir, normal);

        // Get the number of mip levels for the ambient lighting.
        int indirectMipLevels = textureQueryLevels(u_Ambient);

        // Sample at max mip level for the diffuse.
        vec3 diffuse =
            textureLod(u_Ambient, ambientDir, indirectMipLevels).rgb * u_AmbientExposure;

        // Sample at variable mip level (determined by roughness) for specular.
        vec3 specular =
            textureLod(u_Ambient, ambientDir, int(float(indirectMipLevels) * u_Roughness)).rgb * u_AmbientExposure;

        vec3 F0 = mix(vec3(0.04), vec3(1.0), u_Metallic);

        vec3 fresnel = Fresnel(F0, max(0.0, dot(normal, ambientDir)));

        indirectLighting = ((diffuse * (1.0 - u_Metallic)) * albedo.rgb) + (fresnel * specular);
    }

    gl_FragColor = vec4((directLighting + indirectLighting), 1.0);
}