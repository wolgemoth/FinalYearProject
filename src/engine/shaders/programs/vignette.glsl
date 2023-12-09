#pragma vertex

	#version 330 core

	in mediump vec2 a_Position;
	in mediump vec2 a_TexCoord;

	out mediump vec2 v_TexCoord;

	void main() {

		v_TexCoord = a_TexCoord;

		gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
	}

#pragma fragment

	#version 330 core

	#extension GL_ARB_explicit_uniform_location : enable

	in mediump vec2 v_TexCoord;

	uniform sampler2D u_Texture;

	/* PARAMETERS */
	uniform mediump float u_Amount;

	void main() {

	    mediump vec2 uv = (v_TexCoord - 0.5) * 2.0;

	    gl_FragColor = vec4(texture2D(u_Texture, v_TexCoord)) * (1.0 - distance(uv * u_Amount, vec2(0, 0)));
	}