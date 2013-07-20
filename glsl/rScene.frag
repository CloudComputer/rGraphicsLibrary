#version 430 compatibility

#RGL MATH

layout(location = 0) out vec4 outDiffuse;
layout(location = 1) out vec4 outSpecular;

in vec2 texCoord;

uniform sampler2D inDiffuse;
uniform sampler2D inSpecular;
uniform sampler2D inNormal;
uniform sampler2D inDepth;

uniform mat4 camMatrix;
uniform mat4 projMatrix;

uniform vec3 lightPos;
uniform vec3 lightColor;

void main(){
	vec3 color = vec3(0,0,0);
	vec3 N = normalize(texture2D(inNormal,texCoord).xyz  * 2.0 - 1.0);
	vec3 screenPos = vec3(texCoord,texture2D(inDepth,texCoord).x);
	//if(screenPos.z == 1)discard;
	vec3 P = screenSpace2ViewSpace(screenPos,projMatrix,camMatrix);
	
	vec3 diff = texture2D(inDiffuse,texCoord).xyz;
	vec4 spec = texture2D(inSpecular,texCoord);

	vec3 L,V,R;
	L = normalize(lightPos-P);
	V = normalize(-P);
	R = reflect(-V,N);

	
	outDiffuse  = vec4(lightColor * diff *       clamp((dot(L,N)),0.0,1.0),1.0);
	if(spec.w!=0)
		outSpecular = vec4(lightColor * spec.xyz*pow(clamp((dot(L,R)),0.0,1.0),1.0/spec.w),1.0);
}