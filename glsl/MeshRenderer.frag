

varying vec3 N;
varying vec3 V;
varying vec3 P;

varying mat4 M;



#RGL LIGHTING

void main(){
	LitFrag lightFrag = calcLight(P,normalize(N),normalize(V));
	vec3 c = lightFrag.ambient;
	c += lightFrag.diffuse;
	c += lightFrag.specular;
	//c = normalize(N) + 1.0 / 2.0;
	gl_FragColor = vec4(c,material.diffuse.a);
}