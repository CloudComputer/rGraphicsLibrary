 
#version 120

varying vec3 inCoords;

void main()
{
	inCoords = gl_Color.xyz;
	gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;
}









