
#version 120

uniform sampler2D texBack;
uniform sampler3D texVol;
uniform vec3 texSize;
uniform vec2 winSize;
uniform float iso;

varying vec3 inCoords;

vec3 getGradient(in vec3 pos){
	vec3 g;
	float dx = 1.0 / texSize.x;
	g.x = (texture3D(texVol,pos+vec3(dx,0,0)).r - texture3D(texVol,pos-vec3(dx,0,0)).r ) / (2*dx);
	g.y = (texture3D(texVol,pos+vec3(0,dx,0)).r - texture3D(texVol,pos-vec3(0,dx,0)).r ) / (2*dx);
	g.z = (texture3D(texVol,pos+vec3(0,0,dx)).r - texture3D(texVol,pos-vec3(0,0,dx)).r ) / (2*dx);
	return g;
}

vec4 raytrace(in vec3 entryPoint,in vec3 exitPoint){
	vec3 dir = exitPoint - entryPoint;
	vec3 pos = entryPoint;
	vec4 color = vec4(0.0,0.0,0.0,0.0);
	int steps = int(2.0*length(texSize));
	dir = dir * (1.0/steps);
	vec3 n;
	float v,m=0.0,avg=0.0,avg2=0.0;
	for(int i = 0;i<steps && i < 2500;i++){
		v = texture3D(texVol,pos).r;
		avg += v;
		m = max(v,m);
		if(v<iso-0.01){
			vec3 n = normalize(getGradient(pos));
			float ii = abs(dot(n,normalize(dir)));
			color = vec4(ii,ii,ii,1);
			break;
		}

		pos += dir;
	}
	return color;
	
}

void main()
{
	vec2 texCoord = gl_FragCoord.xy/winSize;
	vec3 exitPoint = texture2D(texBack,texCoord).xyz;
	vec4 color = raytrace(inCoords,exitPoint);
	if(color.a < 0.1)
		discard;
	gl_FragColor = color;
}













