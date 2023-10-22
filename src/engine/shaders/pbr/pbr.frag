#version 330 core

in vec2 v_TexCoord;
in vec3 v_Position;
in vec3 v_Normal;

uniform sampler2D u_Texture;

/* PARAMETERS */
uniform  vec3 u_CameraPosition;   // Camera position. Mainly used for lighting calculations.
uniform float u_Metallic;         // How metallic the surface is.
uniform float u_Roughness;        // How rough the surface is.

/* LIGHTING */
uniform vec4 u_AmbientLighting; // Color of ambient lighting. Accepts HDR values.

uniform  vec3 u_PointLightPosition;   // Position of light.
uniform float u_PointLightRange;      // Range of light.
uniform float u_PointLightBrightness; // Brightness of light.
uniform  vec4 u_PointLightColor;      // Color of light.

uniform  vec3 u_DirectionalLightNormal;     // Direction of light.
uniform float u_DirectionalLightBrightness; // Brightness of light.
uniform  vec4 u_DirectionalLightColor;      // Color of light.

/* FOG */
uniform  vec4 u_FogColor;    // Color of fog effect. Accepts HDR values.
uniform float u_FogDensity;  // Density of fog effect.

// Couldn't find a square distance function so made my own. Does GLSL not have one?
float sqrLength(vec3 _A, vec3 _B) {

    vec3 delta = _B - _A;

    return dot(delta, delta);
}

// Light falloff with inverse square law.
float attenuation(vec3 _lightPosition, vec3 _fragPosition, float _range) {
    return _range / sqrLength(_lightPosition, _fragPosition);
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
    return alphaSq / (3.1415 * denom * denom);
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
        max(0.00001, 4.0 * cosLi * cosLo);

    return (diffuse + specular) * cosLi;
}

void main() {

    vec4 albedo = texture2D(u_Texture, v_TexCoord);

    vec3  viewDir = normalize(u_CameraPosition - v_Position);
    vec3 lightDir = normalize(u_PointLightPosition - v_Position);
    vec3  halfVec = normalize(lightDir + viewDir);
    vec3   normal = normalize(v_Normal);

    /* POINT LIGHT */

    float attenuation = attenuation(u_PointLightPosition, v_Position, u_PointLightRange);

    vec3 lighting = BRDF(
        albedo.rgb,
        normal,
        lightDir,
        viewDir,
        halfVec,
        u_Metallic,
        u_Roughness
    ) * u_PointLightBrightness * attenuation;

    gl_FragColor = albedo * (u_PointLightColor * vec4(lighting, 1.0));
}