#version 430 compatibility

#RGL MATH

layout(location = 0) out vec4 outColor;

in vec2 texCoord;

uniform sampler2D inDiffuse;
uniform sampler2D inSpecular;

uniform sampler2D inAmbient;
uniform sampler2D inNormal;
uniform sampler2D inDepth;

uniform mat4 camMatrix;
uniform mat4 projMatrix;

vec3 ambientOcclusion(){
	return vec3(1,1,1);
}


void main(){
	vec3 color = vec3(0,0,0),ambient = vec3(0,0,0);
	vec3 N = texture2D(inNormal,texCoord).xyz  * 2.0 - 1.0;
	vec3 screenPos = vec3(texCoord,texture2D(inDepth,texCoord).x);
	if(texture2D(inDepth,texCoord).x == 1) discard;
	
	vec3 diff = texture2D(inDiffuse,texCoord).xyz;
	vec3 spec = texture2D(inSpecular,texCoord).xyz;
	
	ambient = ambientOcclusion();

	color = ambient*diff + spec;

	outColor = vec4(color,1);
}