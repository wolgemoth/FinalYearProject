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

/* PARAMETERS */
uniform  vec3 u_CameraPosition;   // Camera position. Mainly used for lighting calculations.
uniform float u_Metallic;         // How metallic the surface is.
uniform float u_Roughness;        // How rough the surface is.

/* LIGHTING */
uniform samplerCube u_Ambient;
uniform float u_AmbientExposure = 1.6;

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

float ShadowCalculation(vec4 fragPosLightSpace) {

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_ShadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main() {

    vec4 albedo = texture2D(u_Albedo, v_TexCoord);

    vec3  viewDir = normalize(u_CameraPosition - v_Position);
    vec3 lightDir = normalize(u_PointLightPosition - v_Position);
    vec3  halfVec = normalize(lightDir + viewDir);
    vec3   normal = normalize(v_Normal);

    /* DIRECT */

    float attenuation = Attenuation(u_PointLightPosition, v_Position, u_PointLightRange);

    vec3 lighting = BRDF(
        albedo.rgb,
        normal,
        lightDir,
        viewDir,
        halfVec,
        u_Metallic,
        u_Roughness
    ) * u_PointLightBrightness * attenuation;

    vec3 directLighting = albedo.rgb * lighting * u_PointLightColor;

    /* INDIRECT */

    vec3 indirectLighting;

    {
        vec3 ambientNormal = reflect(-viewDir, normal);

        int indirectMipLevels = textureQueryLevels(u_Ambient);

        vec3 diffuse  = textureLod(u_Ambient, ambientNormal, indirectMipLevels).rgb * u_AmbientExposure;
        vec3 specular = textureLod(u_Ambient, ambientNormal, int(float(indirectMipLevels) * u_Roughness)).rgb * u_AmbientExposure;

        vec3 F0 = mix(vec3(0.04), vec3(1.0), u_Metallic);

        vec3 fresnel = Fresnel(F0, max(0.0, dot(normal, ambientNormal)));

        indirectLighting = ((diffuse * (1.0 - u_Metallic)) * albedo.rgb) + (fresnel * specular);
    }

    float shadows = ShadowCalculation(v_FragPosLightSpace);

    gl_FragColor = vec4((directLighting + indirectLighting), 1.0) - shadows;
}