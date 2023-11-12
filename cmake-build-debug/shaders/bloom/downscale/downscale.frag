#version 330 core

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main() {

	vec2 texelSize = vec2(1.0) / textureSize(u_Texture, 0);
    float x = texelSize.x;
    float y = texelSize.y;

    // Take 13 samples around current texel:
    // a - b - c
    // - j - k -
    // d - e - f
    // - l - m -
    // g - h - i
    // === ('e' is the current texel) ===
    vec3 a = texture(u_Texture, v_TexCoord + vec2(-2*x,  2*y)).rgb;
    vec3 b = texture(u_Texture, v_TexCoord + vec2(   0,  2*y)).rgb;
    vec3 c = texture(u_Texture, v_TexCoord + vec2( 2*x,  2*y)).rgb;

    vec3 d = texture(u_Texture, v_TexCoord + vec2(-2*x,    0)).rgb;
    vec3 e = texture(u_Texture, v_TexCoord + vec2(   0,    0)).rgb;
    vec3 f = texture(u_Texture, v_TexCoord + vec2( 2*x,    0)).rgb;

    vec3 g = texture(u_Texture, v_TexCoord + vec2(-2*x, -2*y)).rgb;
    vec3 h = texture(u_Texture, v_TexCoord + vec2(   0, -2*y)).rgb;
    vec3 i = texture(u_Texture, v_TexCoord + vec2( 2*x, -2*y)).rgb;

    vec3 j = texture(u_Texture, v_TexCoord + vec2(-x,  y)).rgb;
    vec3 k = texture(u_Texture, v_TexCoord + vec2( x,  y)).rgb;
    vec3 l = texture(u_Texture, v_TexCoord + vec2(-x, -y)).rgb;
    vec3 m = texture(u_Texture, v_TexCoord + vec2( x, -y)).rgb;

    // Apply weighted distribution:
    // 0.5 + 0.125 + 0.125 + 0.125 + 0.125 = 1
    // a,b,d,e * 0.125
    // b,c,e,f * 0.125
    // d,e,g,h * 0.125
    // e,f,h,i * 0.125
    // j,k,l,m * 0.5
    // This shows 5 square areas that are being sampled. But some of them overlap,
    // so to have an energy preserving downsample we need to make some adjustments.
    // The weights are the distributed, so that the sum of j,k,l,m (e.g.)
    // contribute 0.5 to the final color output. The code below is written
    // to effectively yield this sum. We get:
    // 0.125*5 + 0.03125*4 + 0.0625*4 = 1
    vec3 col = e*0.125;
    col += (a+c+g+i)*0.03125;
    col += (b+d+f+h)*0.0625;
    col += (j+k+l+m)*0.125;

	gl_FragColor = vec4(col, 1);
}