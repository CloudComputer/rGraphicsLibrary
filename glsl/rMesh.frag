#RGL RFRAG


in vec3 oPos;
in vec3 oNormal;
in vec2 oTexCoord;

#ifdef USETEXTURE
	uniform sampler2D tex;	
#else
	uniform vec4 matDiffuse;
#endif

uniform vec3 matAmbient;
uniform vec3 matSpecular;
uniform float matSpecularity;

void main(){
	
	vec3 a = matAmbient;
	vec3 s = matSpecular;
	vec3 normal = oNormal;
	float depth = oPos.z;
#ifdef USETEXTURE
	vec4 d = texture2D(tex,oTexCoord);
#else
	vec4 d = matDiffuse;
	a *= d.xyz;
#endif
	setOutput(d,s,a,normal,depth,matSpecularity);
}