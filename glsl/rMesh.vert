#RGL RVERT

in vec3 Pos;
in vec3 Normal;
in vec2 TexCoord;


out vec3 oPos;
out vec3 oNormal;
out vec2 oTexCoord;

void main(){
	vec4 p = fTrans(vec4(Pos,1.0));
	gl_Position = p;
	oPos = p.xyz;
	oNormal = Normal;
	oTexCoord = TexCoord;
}