varying vec3 N;
varying vec3 V;

void main()
{
	N   =  (gl_NormalMatrix * gl_Normal).xyz;
	vec4 pos   =  gl_ModelViewMatrix * gl_Vertex;
	V = -pos.xyz;
	gl_Position =  gl_ProjectionMatrix * pos;
}









