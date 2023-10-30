#version 330 core

#extension GL_ARB_texture_query_levels : enable

const float EPSILON = 0.005;
const float      PI = 3.1415;

in vec2 v_TexCoord;
in vec3 v_Position;
in vec3 v_Normal;
in vec4 v_FragPosLightSpace;

uniform sampler2D u_Albedo;

uniform sampler2D u_ShadowMap;

uniform float u_Time;

int u_PCFSamples = 32;

float lightSize = 10;
float NEAR = 0.1;

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
    float cosLi = max(0.0, dot(_normal, _lightDir));
    float cosLh = max(0.0, dot(_normal, _halfVec));
    float cosLo = max(0.0, dot(_normal, _viewDir));

    vec3 F0 = mix(vec3(0.04), _albedo, _metallic);

    vec3 fresnel = Fresnel(F0, max(0.0, dot(_halfVec, _viewDir)));

    vec3 diffuse = Irradiance(fresnel, _metallic);

    vec3 specular = (fresnel * Distrib(cosLh, _roughness) * Geom(cosLi, cosLo, _roughness)) /
        max(EPSILON, 4.0 * cosLi * cosLo);

    return (diffuse + specular) * cosLi;
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
    return lightSize * (_viewerDepth - NEAR) / v_Position.z;
}

float calcOccluderDistance(vec4 fragPosLightSpace, float _viewerDepth, float bias) {

    float sumBlockerDistances = 0.0;
      int numBlockerDistances = 0;

    // perform perspective divide
    vec2 projCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w).xy;

    // transform to [0,1] range
    projCoords = (projCoords * 0.5) + 0.5;

    float texelSize = 1.0 / textureSize(u_ShadowMap, 0).r;

    int sw = int(calcSearchWidth(_viewerDepth));
    for (int i = 0; i < u_PCFSamples; ++i) {

        vec2 randomDir = PoissonDisk(projCoords.xy + vec2(i + 1), 0.1);

        vec2 sampleDir = randomDir * (i + 1) * texelSize;

        float pcfDepth = texture(u_ShadowMap, projCoords.xy + sampleDir).r;

        if (pcfDepth < _viewerDepth - bias) {
            ++numBlockerDistances;
            sumBlockerDistances += pcfDepth;
        }
    }

    if (numBlockerDistances > 0) {
        return sumBlockerDistances / float(numBlockerDistances);
    }
    else {
        return -1;
    }
}

float calcPCFKernelSize(vec4 fragPosLightSpace, float viewerDepth, float bias) {

    float occluderDepth = calcOccluderDistance(fragPosLightSpace, viewerDepth, bias);

    if (occluderDepth == -1) {
        return 1;
    }

    float penumbraWidth = abs(viewerDepth - occluderDepth) / occluderDepth;

    return penumbraWidth * lightSize * (NEAR / viewerDepth);
}

float ShadowCalculationPCF(vec4 fragPosLightSpace, vec3 _normal, vec3 _lightDir, float _bias, float _normalBias) {

    float result = 0.0;

    // Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // Transform to [0,1] range
    projCoords = (projCoords * 0.5) + 0.5;

    // Get depth of current fragment from light's perspective
    float fragDepth = projCoords.z;

    float totalBias = max(_normalBias * (1.0 - dot(_normal, _lightDir)), _bias);

    vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);

    int kSize = 3; // Size of kernel. Must be odd number above 1.

    for (int x = -(kSize - 1) / 2; x <= (kSize - 1) / 2; x++) {
    for (int y = -(kSize - 1) / 2; y <= (kSize - 1) / 2; y++) {

        vec2 dir = vec2(x, y);

        dir *= texelSize;

        float shadowDepth = texture(u_ShadowMap, projCoords.xy + dir).r;

        result += (fragDepth <= 1.0) && (fragDepth - totalBias > shadowDepth) ?
            1.0 : 0.0;
    }}

    return result / (kSize * kSize);
}

float ShadowCalculationPCSS(vec4 fragPosLightSpace, vec3 _normal, vec3 _lightDir, float _bias, float _normalBias) {

    float result = 0.0;

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // transform to [0,1] range
    projCoords = (projCoords * 0.5) + 0.5;

    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float occluderDepth = texture(u_ShadowMap, projCoords.xy).r;

    // Get depth of current fragment from light's perspective
    float fragDepth = projCoords.z;

    float NdL = dot(_normal, _lightDir);

    float totalBias = max(_normalBias * (1.0 - NdL), _bias);

    vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);

    float radius = calcPCFKernelSize(fragPosLightSpace, fragDepth, totalBias);

    for (int i = 0; i < u_PCFSamples; i++) {

        vec2 dir = PoissonDisk(projCoords.xy + vec2(i + 1), 0.1);

        dir *= radius * (i + 1) * texelSize;

        float shadowDepth = texture(u_ShadowMap, projCoords.xy + dir).r;

        result += (fragDepth <= 1.0) && (fragDepth - totalBias > shadowDepth) ?
            1.0 : 0.0;
    }

    float l = clamp(smoothstep(0.0, 0.2f, NdL), 0.0, 1.0);
    float t = smoothstep(gold_noise(projCoords.xy * 1000.0, fract(u_Time) + 0.3) * 0.5f, 1.0, l);

    return clamp(result / (u_PCFSamples * t), 0.0, 1.0);
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 _normal, vec3 _lightDir, float _bias, float _normalBias) {

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // transform to [0,1] range
    projCoords = (projCoords * 0.5) + 0.5;

    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_ShadowMap, projCoords.xy).r;

    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    float totalBias = max(_normalBias * (1.0 - dot(_normal, _lightDir)), _bias);

    // check whether current frag pos is in shadow
    return (currentDepth <= 1.0) && (currentDepth - totalBias > closestDepth) ?
        1.0 : 0.0;
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