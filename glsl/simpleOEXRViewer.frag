
//#version 140

varying vec2 texCoord;

uniform sampler2DRect tex;

void main()
{
	vec4 color = texture2DRect(tex,texCoord);
	gl_FragColor = color;
}

