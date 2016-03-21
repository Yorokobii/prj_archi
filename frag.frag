varying vec4 colorForGPU;

void main (void)
{
	vec4 modif = vec4(0.1, 0.1, 0.1, 1.0);
	gl_FragColor = gl_Color;// * modif;
}
