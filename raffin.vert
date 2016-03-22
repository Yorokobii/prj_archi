uniform vec2 VecDeplac;

void main(void){
    vec4 point = gl_Vertex;

    point.xy += VecDeplac.xy;

    gl_Position = gl_ModelViewProjectionMatrix * point;

    gl_TexCoord[0] = gl_MultiTexCoord0;
}
