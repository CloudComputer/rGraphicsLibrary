
uniform sampler2D texBack;
uniform sampler2D texFront;
uniform sampler3D texVol;
uniform vec3 texSize;
uniform vec2 winSize;


varying vec2 texCoord;

vec4 isoSurace(vec3 entryPoint,vec3 exitPoint){
	
	vec3 dir = exitPoint - entryPoint;
	vec3 pos = entryPoint;
	vec4 color = vec4(0,0,0,0);
	int steps = 2*length(texSize);
	dir /= steps;
	vec3 n;
	float v,m=0,avg=0,avg2=0;
	for(int i = 0;i<steps;i++){
		v = texture3D(texVol,pos).r;
		if(v>0.5){
			n.x = (texture3D(texVol,pos+glm::vec3(1.0/texSize.x,0,0)).r-texture3D(texVol,pos-glm::vec3(1.0/texSize.x,0,0)).r);
			n.y = (texture3D(texVol,pos+glm::vec3(0,1.0/texSize.x,0)).r-texture3D(texVol,pos-glm::vec3(0,1.0/texSize.y,0)).r);
			n.z = (texture3D(texVol,pos+glm::vec3(0,0,1.0/texSize.x)).r-texture3D(texVol,pos-glm::vec3(0,0,1.0/texSize.z)).r);
			n = normalize(n);
			float ii = abs(dot(normalize(dir),n));
			ii = clamp(ii,0.2,1.0);
			ii = pow(ii,1);
			color = vec4(ii,ii,ii,1);
			break;
		}
		pos += dir;
	}
	return color;
}

void main()
{
	vec2 texCoord = abs(gl_FragCoord.xy/winSize);
	vec3 entryPoint = texture2D(texFront,texCoord).xyz;
	vec3 exitPoint = texture2D(texBack,texCoord).xyz;
	gl_FragColor = isoSurace(entryPoint,exitPoint);
}













