#pragma vertex

    #version 330 core

    #extension GL_ARB_explicit_uniform_location : enable

    in mediump vec3 a_Position;
    in mediump vec2 a_TexCoord;
    in mediump vec3 a_Normal;

    out mediump vec3 v_Position;
    out mediump vec2 v_TexCoord;
    out mediump vec3 v_Normal;

    /* PARAMETERS */
    layout (location = 3) uniform mediump mat4 u_Projection;
    layout (location = 4) uniform mediump mat4 u_Model;
    layout (location = 5) uniform mediump mat4 u_View;

    void main() {

        gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);

        v_Position = vec3(u_Model * vec4(a_Position, 1.0));
        v_TexCoord = a_TexCoord;
        v_Normal   = transpose(inverse(mat3((u_Model)))) * a_Normal;
    }

#pragma fragment

    #version 330 core

    in mediump vec2 v_TexCoord;
    in mediump vec3 v_Position;
    in mediump vec3 v_Normal;

    uniform sampler2D u_Texture;

    /* PARAMETERS */
    uniform mediump  vec3 u_CameraPosition; // Camera position. Mainly used for lighting calculations.
    uniform mediump float u_Roughness;		// Roughness of the surface (inverse of smoothness / shininess).

    /* LIGHTING */
    uniform mediump vec4 u_AmbientLighting; // Color of ambient lighting. Accepts HDR values.

    uniform mediump  vec3 u_PointLightPosition;   // Position of light.
    uniform mediump float u_PointLightRange;      // Range of light.
    uniform mediump float u_PointLightBrightness; // Brightness of light.
    uniform mediump  vec4 u_PointLightColor;      // Color of light.

    uniform mediump  vec3 u_DirectionalLightNormal;     // Direction of light.
    uniform mediump float u_DirectionalLightBrightness; // Brightness of light.
    uniform mediump  vec4 u_DirectionalLightColor;      // Color of light.

    /* FOG */
    uniform mediump  vec4 u_FogColor;	 // Color of fog effect. Accepts HDR values.
    uniform mediump float u_FogDensity;  // Density of fog effect.

    // Couldn't find a square distance function so made my own. Does GLSL not have one?
    mediump float SqrDistance(in vec3 _A, in vec3 _B) {

        vec3 delta = _B - _A;

        return dot(delta, delta);
    }

    mediump vec4 Fog(in mediump vec3 _cameraPosition, in mediump vec3 _fragPosition, in mediump vec4 _color, in mediump float _density) {
        return SqrDistance(_cameraPosition, _fragPosition) * _color * _density;
    }

    // Light falloff with inverse square law.
    mediump float LightAttenuation(in mediump vec3 _lightPosition, in mediump vec3 _fragPosition, in mediump float _range) {
        return _range / SqrDistance(_lightPosition, _fragPosition);
    }

    // Inspired by a modification to the lambert diffuse by John Edwards (https://youtu.be/wt2yYnBRD3U?t=892)
    mediump float EdwardsLambertIrradiance(const vec3 _normal, const vec3 _lightDirection) {
        return max(4.0 * dot(_normal, _lightDirection) * 0.2, 0.0);
    }

    mediump float BlinnPhongSpecular(const mediump vec3 _normal, const mediump vec3 _lightDirection, const mediump vec3 _viewDirection, in mediump float _roughness) {

        mediump vec3  halfDir   = normalize(_lightDirection + _viewDirection);
        mediump float specAngle = max(dot(halfDir, _normal), 0.0);

        mediump float specularity = clamp(u_Roughness, 0.0, 0.998);
        specularity *= specularity;

        return pow(specAngle, 1.0 / specularity);
    }

    void main() {

        mediump vec4 color = texture2D(u_Texture, v_TexCoord);
        mediump vec4 result;

        mediump float lighting = 0;

        mediump vec3  viewDirection = normalize(u_CameraPosition - v_Position);
        mediump vec3         normal = normalize(v_Normal);

        /* POINT LIGHT */
        mediump vec3 lightDirection = normalize( u_PointLightPosition - v_Position);

        // Compute Diffuse Irradiance
        mediump float diffuse = EdwardsLambertIrradiance(normal, lightDirection);

        // Compute Specular Reflection
        mediump float specular = BlinnPhongSpecular(
            lightDirection,
            viewDirection,
            normal,
            u_Roughness
        );

        mediump float attenuation = LightAttenuation(u_PointLightPosition, v_Position, u_PointLightRange);

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
        mediump float depth = SqrDistance(u_CameraPosition, v_Position);

        gl_FragColor = mix(result, u_FogColor, clamp(depth * u_FogDensity, 0.0, 1.0));
    }