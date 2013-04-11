varying vec3 N;
varying vec3 V;
varying vec3 P;


uniform float lum;

#RGL LIGHTING

void main(){
	float s = 1.0/(lum);
	vec3 col = vec3(s,s,s)*calcLight(P,normalize(N),normalize(V));

	gl_FragColor = vec4(col,1.0);
}