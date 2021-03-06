uniform vec2 VecDeplac;

uniform vec3 vecCDeform;
uniform float rayonDeform;
uniform vec3 vecVDeform;

uniform vec2 VecVib;

varying float VChange;

void main(void){
    vec4 point = gl_Vertex;

    point.xy += VecDeplac.xy;

    float dx = distance(vecCDeform.x, point.x);	//distance est une fonction de GLSL (cf. mémo sur le site)
    float dy = distance(vecCDeform.y, point.y);	//distance est une fonction de GLSL (cf. mémo sur le site)

	if (dx <= rayonDeform && dy <= rayonDeform) {
		point.xyz = point.xyz + vecVDeform.xyz;
		VChange = 1.0;
	}
	else
		VChange = 0.0;

    point.xy += VecVib;

    gl_Position = gl_ModelViewProjectionMatrix * point;

    gl_TexCoord[0] = gl_MultiTexCoord0;
}
