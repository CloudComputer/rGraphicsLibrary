varying vec3 N;
varying vec3 V;
varying vec3 P;

varying mat4 M;

void main()
{
	N   =  (gl_NormalMatrix * gl_Normal).xyz;
	M = gl_ModelViewMatrix;
	vec4 pos   =  gl_ModelViewMatrix * gl_Vertex;
	P = pos.xyz;
	V = -P;
	gl_Position =  gl_ProjectionMatrix * pos;
}









