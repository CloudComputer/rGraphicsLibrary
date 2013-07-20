#version 430 compatibility
out vec2 texCoord;

void main(){
	texCoord = gl_MultiTexCoord0.xy;
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}