
#version 120

uniform sampler2D texBack;
uniform sampler3D texVol;
uniform vec3 texSize;
uniform vec2 winSize;
uniform float iso;

varying vec3 inCoords;

vec4 raytrace(in vec3 entryPoint,in vec3 exitPoint){
	vec3 dir = exitPoint - entryPoint;
	vec3 pos = entryPoint;
	vec4 color = vec4(0.0,0.0,0.0,0.0);
	int steps = int(2.0*length(texSize));
	dir = dir * (1.0/steps);
	vec3 n;
	float v,m=0.0,avg=0.0,avg2=0.0;
	for(int i = 0;i<steps || i < 2500;i++){
		v = texture3D(texVol,pos).r;
		m = max(v,m);
		avg += v;
		pos += dir;
	}
	return vec4(avg/steps,m,0,1);
	
}

void main()
{
	vec2 texCoord = gl_FragCoord.xy/winSize;
	vec3 exitPoint = texture2D(texBack,texCoord).xyz;
	gl_FragColor = raytrace(inCoords,exitPoint);
}













