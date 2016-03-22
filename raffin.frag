uniform sampler2D colorMap;

varying float VChange;

void main (void)
{
	vec4 gris = vec4(0.1, 0.1, 0.1, 1.0);

	if(VChange == 1.0)
		gl_FragColor = gris;
	else
		gl_FragColor = texture2D(colorMap, gl_TexCoord[0].xy);
}
