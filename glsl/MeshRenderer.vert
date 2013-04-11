varying vec3 N;
varying vec3 V;
varying vec3 P;

varying mat4 M;

attribute vec4 Pos;
attribute vec3 Normal;

void main()
{
	N = (gl_NormalMatrix * Normal).xyz;
	M = gl_ModelViewMatrix;
	vec4 pos   =  M * Pos;
	P = pos.xyz;
	V = -P;
	gl_Position =  gl_ProjectionMatrix * pos;
}









