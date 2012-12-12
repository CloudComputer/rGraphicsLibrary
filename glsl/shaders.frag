
varying vec2 pos;

uniform float curTime;

void main()
{
	float t = abs(sin(curTime+50.0*distance(pos,vec2(0.5,0.5))));

	gl_FragColor = vec4(t,t,t,1);
}

