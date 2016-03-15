uniform vec3 vecCDeform;
uniform vec3 vecVDeform;
uniform float rayonDeform;

varying vec4 colorForGPU;

void main(void){
    if(rayonDeform == 1.0)
        colorForGPU = vec4(0.0,0.0,1.0,1.0);



    //gl_Position = gl_ModelViewProjectionMatrix * vecCDeform;
}
