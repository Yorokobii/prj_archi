uniform vec3 vecCDeform;
uniform vec3 vecVDeform;
uniform float rayonDeform;

uniform float BoolVert;

varying float BoolFrag;

void main(void){
        vec4 point = gl_Vertex;

    	//on calcule la distance du centre de la déformation au vertex (point) courant
    	float d = distance(vec4(vecCDeform, 1.0), point);	//distance est une fonction de GLSL (cf. mémo sur le site)

    	//si la distance ci-dessus est inférieure au rayon d'action de la contrainte
    	//on déforme de manière linéaire (max=centre, min=rayon)

        BoolFrag = BoolVert;

    	if ((d < rayonDeform) && (BoolVert == 1.0)) {
    		point.xyz +=
            ls   vecVDeform.xyz;
    	}

    	//et on projette ce point
    	//gl_Position = gl_ModelViewProjectionMatrix /** point*/;
        gl_Position = gl_ModelViewProjectionMatrix * /*gl_Vertex*/ point;


        gl_FrontColor = gl_Color;
}
