
attribute vec2 tex;

varying vec2 pos;

void main()
{
	pos = tex;
	gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;
}









