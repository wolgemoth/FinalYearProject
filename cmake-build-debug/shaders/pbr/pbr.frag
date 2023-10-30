#version 330 core

#extension GL_ARB_texture_query_levels : enable

const float EPSILON = 0.005;
const float      PI = 3.141593;

in vec2 v_TexCoord;
in vec3 v_Position;
in vec3 v_Normal;
in vec4 v_FragPosLightSpace;

uniform sampler2D u_Albedo;

uniform sampler2D u_ShadowMap;

uniform float u_Time;

int u_ShadowSamples = 32;

float u_LightSize = 1000;
float u_NearPlane = 0.1;

/* PARAMETERS */
uniform  vec3 u_CameraPosition;   // Camera position. Mainly used for lighting calculations.
uniform float u_Metallic;         // How metallic the surface is.
uniform float u_Roughness;        // How rough the surface is.

/* LIGHTING */
uniform samplerCube u_Ambient;
uniform float u_AmbientExposure = 1.6;

uniform float u_ShadowBias = 0.005;
uniform float u_ShadowNormalBias = 0.1;

uniform  vec3 u_PointLightPosition;   // Position of light.
uniform float u_PointLightRange;      // Range of light.
uniform float u_PointLightBrightness; // Brightness of light.
uniform  vec3 u_PointLightColor;      // Color of light.

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
    float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.

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

    vec3 specular = (fresnel * Distrib(NdH, _roughness) * Geom(NdL, NdV, _roughness)) /
        max(EPSILON, 4.0 * NdL * NdV);

    return (diffuse + specular) * NdL;
}

// Gold Noise ©2015 dcerisano@standard3d.com
// - based on the Golden Ratio
// - uniform normalized distribution
// - fastest static noise generator function (also runs at low precision)
// - use with indicated fractional seeding method.

float PHI = 1.61803398874989484820459;  // Φ = Golden Ratio

float gold_noise(in vec2 xy, in float seed){
    return fract(tan(distance(xy*PHI, xy)*seed)*xy.x);
}

vec2 PoissonDisk(in vec2 _xy, float _offset) {

    return vec2(
        (gold_noise(_xy * 1000.0, fract(u_Time) + _offset + 0.1) - 0.5) * 2.0,
        (gold_noise(_xy * 1000.0, fract(u_Time) + _offset + 0.2) - 0.5) * 2.0
    ) / 1.414;
}

float calcSearchWidth(float _viewerDepth) {
    return u_LightSize * (_viewerDepth - u_NearPlane) / _viewerDepth;
}

float calcOccluderDistance(vec4 fragPosLightSpace, float _viewerDepth, float bias) {

    float result = 0.0;

    // perform perspective divide
    vec3 projCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w);

    // transform to [0,1] range
    projCoords = (projCoords * 0.5) + 0.5;

    float texelSize = 1.0 / textureSize(u_ShadowMap, 0).r;

    int numBlockerDistances = 0;

    int sw = u_ShadowSamples;

    for (int i = 0; i < sw; i++) {

        vec2 dir = PoissonDisk(projCoords.xy + vec2(i + 1), 0.1);

        dir *= (i + 1) * texelSize;

        float occluderDepth = texture(u_ShadowMap, projCoords.xy + dir).r;

        if (occluderDepth < _viewerDepth - bias) {
            ++numBlockerDistances;
            result += occluderDepth;
        }
    }

    if (numBlockerDistances > 0) {
        return result / float(numBlockerDistances);
    }
    else {
        return -1;
    }
}

float PCSSWidth(vec4 _fragPosLightSpace, float _viewerDepth, float _bias) {

    float occluderDepth = calcOccluderDistance(_fragPosLightSpace, _viewerDepth, _bias);

    if (occluderDepth == -1) {
        return 1;
    }

    float penumbraWidth = abs(_viewerDepth - occluderDepth) / occluderDepth;

    return penumbraWidth * u_LightSize * (u_NearPlane / _viewerDepth);
}

float ShadowCalculationHard(vec4 _fragPosLightSpace, vec3 _normal, vec3 _lightDir, float _bias, float _normalBias) {

    // perform perspective divide
    vec3 projCoords = _fragPosLightSpace.xyz / _fragPosLightSpace.w;

    // transform to [0,1] range
    projCoords = (projCoords * 0.5) + 0.5;

    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float occluderDepth = texture(u_ShadowMap, projCoords.xy).r;

    // Get depth of current fragment from light's perspective
    float viewerDepth = projCoords.z;

    float totalBias = max(_normalBias * (1.0 - dot(_normal, _lightDir)), _bias);

    // check whether current frag pos is in shadow
    return (viewerDepth <= 1.0) && (viewerDepth - totalBias > occluderDepth) ?
        1.0 : 0.0;
}

float ShadowCalculationPCF(vec4 _fragPosLightSpace, vec3 _normal, vec3 _lightDir, float _bias, float _normalBias, float _size) {

    float result = 0.0;

    // Perform perspective divide
    vec3 projCoords = _fragPosLightSpace.xyz / _fragPosLightSpace.w;

    // Transform to [0,1] range
    projCoords = (projCoords * 0.5) + 0.5;

    // Get depth of current fragment from light's perspective
    float visibleDepth = projCoords.z;

    float totalBias = max(_normalBias * (1.0 - dot(_normal, _lightDir)), _bias);

    vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);

    int kSize = int(ceil(_size));

    for (int x = kSize; x <= kSize; x++) {
    for (int y = kSize; y <= kSize; y++) {

        vec2 dir = vec2(x, y);

        dir *= texelSize * _size;

        float occluderDepth = texture(u_ShadowMap, projCoords.xy + dir).r;

        result += (visibleDepth <= 1.0) && (visibleDepth - totalBias > occluderDepth) ?
            1.0 : 0.0;
    }}

    return result / (kSize * kSize);
}

float ShadowCalculationDisk(vec4 _fragPosLightSpace, vec3 _normal, vec3 _lightDir, float _bias, float _normalBias, float _size) {

    float result = 0.0;

    // Perform perspective divide
    vec3 projCoords = _fragPosLightSpace.xyz / _fragPosLightSpace.w;

    // Transform to [0,1] range
    projCoords = (projCoords * 0.5) + 0.5;

    // Get depth of current fragment from light's perspective
    float visibleDepth = projCoords.z;

    float totalBias = max(_normalBias * (1.0 - dot(_normal, _lightDir)), _bias);

    vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);

    for (int i = 0; i < u_ShadowSamples; i++) {

        vec2 dir = normalize(PoissonDisk(projCoords.xy + vec2(i + 1), 0.1));

        dir *= texelSize * _size * ((i + 1) / float(u_ShadowSamples));

        float occluderDepth = texture(u_ShadowMap, projCoords.xy + dir).r;

        result += (visibleDepth <= 1.0) && (visibleDepth - totalBias > occluderDepth) ?
            1.0 : 0.0;
    }

    return result / u_ShadowSamples;
}

float ShadowCalculationPCSS(vec4 _fragPosLightSpace, vec3 _normal, vec3 _lightDir, float _bias, float _normalBias) {

    float result = 0.0;

    // perform perspective divide
    vec3 projCoords = _fragPosLightSpace.xyz / _fragPosLightSpace.w;

    // transform to [0,1] range
    projCoords = (projCoords * 0.5) + 0.5;

    // Get depth of current fragment from light's perspective
    float viewerDepth = projCoords.z;

    float NdL = dot(_normal, _lightDir);

    float totalBias = max(_normalBias * (1.0 - NdL), _bias);

    vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);

    float radius = PCSSWidth(_fragPosLightSpace, viewerDepth, totalBias);

    result = ShadowCalculationDisk(_fragPosLightSpace, _normal, _lightDir, _bias, _normalBias, radius);

    return result;
}

void main() {

    vec4 albedo = texture2D(u_Albedo, v_TexCoord);

    vec3  viewDir = normalize(u_CameraPosition - v_Position);
    vec3 lightDir = normalize(u_PointLightPosition - v_Position);
    vec3  halfVec = normalize(lightDir + viewDir);
    vec3   normal = normalize(v_Normal);

    /* DIRECT */

    vec3 directLighting;
    {
        float attenuation = Attenuation(u_PointLightPosition, v_Position, u_PointLightRange);

        float visibility = 1.0 - ShadowCalculationPCSS(v_FragPosLightSpace, normal, lightDir, u_ShadowBias, u_ShadowNormalBias);

        vec3 lighting = BRDF(
            albedo.rgb,
            normal,
            lightDir,
            viewDir,
            halfVec,
            u_Metallic,
            u_Roughness
        ) * u_PointLightBrightness * attenuation;

        directLighting += (visibility * lighting) * u_PointLightColor;
    }

    directLighting *= albedo.rgb;

    /* INDIRECT */

    vec3 indirectLighting;

    {
        // Figure out the direction of the ambient light
        vec3 ambientDir = reflect(-viewDir, normal);

        // Get the number of mip levels for the ambient lighting.
        int indirectMipLevels = textureQueryLevels(u_Ambient);

        // Sample for diffuse at max mip level.
        vec3 diffuse =
            textureLod(u_Ambient, ambientDir, indirectMipLevels).rgb * u_AmbientExposure;

        // Sample for specular at mip level determined by roughness.
        vec3 specular =
            textureLod(u_Ambient, ambientDir, int(float(indirectMipLevels) * u_Roughness)).rgb * u_AmbientExposure;

        vec3 F0 = mix(vec3(0.04), vec3(1.0), u_Metallic);

        vec3 fresnel = Fresnel(F0, max(0.0, dot(normal, ambientDir)));

        indirectLighting = ((diffuse * (1.0 - u_Metallic)) * albedo.rgb) + (fresnel * specular);
    }

    gl_FragColor = vec4((directLighting + indirectLighting), 1.0);
}