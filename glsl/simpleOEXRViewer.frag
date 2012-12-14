
//#version 140

#extension GL_ARB_texture_rectangle : enable

#RGL ENV_LIGHT


varying vec2 texCoord;

uniform sampler2DRect tex;
uniform float gamma;
uniform float minExp;
uniform float maxExp;

void main()
{
	vec4 color = vec4(exposure(texture2DRect(tex,texCoord),gamma,minExp,maxExp),1.0);
	gl_FragColor = color;
}

