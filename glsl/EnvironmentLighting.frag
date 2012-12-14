varying vec3 N;
varying vec3 V;

#RGL ENV_LIGHT

void main(){
	gl_FragColor = vec4(0.6,0.324,0.167,1)*vec4(sample_environment_map(normalize(N),normalize(V)),1);
	//gl_FragColor = vec4(sample_environment_map(normalize(N),normalize(V)),1);
}