#version 330 core

in vec2 v_TexCoord;
in vec3 v_Position;
in vec3 v_Normal;

uniform sampler2D u_Texture;

/* PARAMETERS */
uniform  vec3 u_CameraPosition; // Camera position. Mainly used for lighting calculations.
uniform float u_Roughness;		// Roughness of the surface (inverse of smoothness / shininess).

/* LIGHTING */
uniform  vec4 u_AmbientLighting; // Color of ambient lighting. Accepts HDR values.

uniform  vec3 u_PointLightPosition;   // Position of light.
uniform float u_PointLightRange;      // Range of light.
uniform float u_PointLightBrightness; // Brightness of light.
uniform  vec4 u_PointLightColor;      // Color of light.

uniform  vec3 u_DirectionalLightNormal;     // Direction of light.
uniform float u_DirectionalLightBrightness; // Brightness of light.
uniform  vec4 u_DirectionalLightColor;      // Color of light.

/* FOG */
uniform  vec4 u_FogColor;	 // Color of fog effect. Accepts HDR values.
uniform float u_FogDensity;  // Density of fog effect.

// Couldn't find a square distance function so made my own. Does GLSL not have one?
float SqrDistance(vec3 _A, vec3 _B) {
	
	vec3 delta = _B - _A;
	
	return dot(delta, delta); 
}

vec4 Fog(vec3 _cameraPosition, vec3 _fragPosition, vec4 _color, float _density) {
	return SqrDistance(_cameraPosition, _fragPosition) * _color * _density;
}

// Light falloff with inverse square law.
float LightAttenuation(vec3 _lightPosition, vec3 _fragPosition, float _range) {
	return _range / SqrDistance(_lightPosition, _fragPosition);
}

// Inspired by a modification to the lambert diffuse by John Edwards (https://youtu.be/wt2yYnBRD3U?t=892)
float EdwardsLambertIrradiance(const vec3 _normal, const vec3 _lightDirection) {
	return max(4.0 * dot(_normal, _lightDirection) * 0.2, 0.0);
}

float BlinnPhongSpecular(const vec3 _normal, const vec3 _lightDirection, const vec3 _viewDirection, float _roughness) {

	vec3  halfDir   = normalize(_lightDirection + _viewDirection);
    float specAngle = max(dot(halfDir, _normal), 0.0);

	float specularity = clamp(u_Roughness, 0.0, 0.998);
	specularity *= specularity;

	return pow(specAngle, 1.0 / specularity);
}

void main() {

	vec4 color = texture2D(u_Texture, v_TexCoord);
	vec4 result;

	float lighting = 0;
	
    vec3  viewDirection = normalize(u_CameraPosition - v_Position);
	vec3         normal = normalize(v_Normal);

	/* POINT LIGHT */
	vec3 lightDirection = normalize( u_PointLightPosition - v_Position);

	// Compute Diffuse Irradiance
	float diffuse = EdwardsLambertIrradiance(normal, lightDirection);

	// Compute Specular Reflection
	float specular = BlinnPhongSpecular(
		lightDirection, 
		viewDirection, 
		normal, 
		u_Roughness
	);

	float attenuation = LightAttenuation(u_PointLightPosition, v_Position, u_PointLightRange);

	lighting += ((diffuse + specular) * u_PointLightBrightness) * attenuation;

	result += color * (u_PointLightColor * lighting);

	/* DIRECTIONAL LIGHT */
	lightDirection = u_DirectionalLightNormal;

	// Compute Diffuse Irradiance
	diffuse = EdwardsLambertIrradiance(normal, lightDirection);

	// Compute Specular Reflection
	specular = BlinnPhongSpecular(
		lightDirection, 
		viewDirection, 
		normal, 
		u_Roughness
	);
	
	lighting = ((diffuse + specular) * u_DirectionalLightBrightness);

	result += color * (u_DirectionalLightColor * lighting);

	/* AMBIENT LIGHTING */
	result += color * u_AmbientLighting;

	// This type of fog is of the "Exponential Squared" variety, which is slightly more performant as it skips a square root.
	float depth = SqrDistance(u_CameraPosition, v_Position);

	gl_FragColor = mix(result, u_FogColor, clamp(depth * u_FogDensity, 0.0, 1.0));
}