//uniform sampler2D sceneBuffer;
//uniform sampler2D noiseTex;
//uniform sampler2D maskTex;
//uniform float elapsedTime; // seconds
float luminanceThreshold = 0.2; // 0.2
float colorAmplification = 0.4; // 4.0
float effectCoverage = 0.5; // 0.5

/*
void main ()
{
  vec4 finalColor;
  // Set effectCoverage to 1.0 for normal use.
  if (gl_TexCoord[0].x < effectCoverage)
  {
    vec2 uv;
    uv.x = 0.4*sin(elapsedTime*50.0);
    uv.y = 0.4*cos(elapsedTime*50.0);
    float m = texture2D(maskTex, gl_TexCoord[0].st).r;
    vec3 n = texture2D(noiseTex, (gl_TexCoord[0].st*3.5) + uv).rgb;
    vec3 c = texture2D(sceneBuffer, gl_TexCoord[0].st);

    float lum = dot(vec3(0.30, 0.59, 0.11), c);
    if (lum < luminanceThreshold)
      c *= colorAmplification;

    vec4 visionColor = vec4(0.1, 0.95, 0.2, 0.0);
    finalColor.rgb = c + visionColor;
   }
   else
   {
    finalColor = texture2D(sceneBuffer, gl_TexCoord[0].st);
   }
  gl_FragColor.rgb = finalColor.rgb;
  gl_FragColor.a = 1.0;
}
*/

uniform sampler2D colorMap;

void main(void)
{
	vec4 tmp = texture2D(colorMap, gl_TexCoord[0].st);
    vec4 c = tmp;

    float lum = 0.299 * tmp.r + 0.587 * tmp.g + 0.114 * tmp.b;
    if(lum < luminanceThreshold)
        c *= colorAmplification;

    tmp = c + vec4(0.1,0.95,0.2,0.0);


	//tmp.rgb = vec3(lum);
	gl_FragColor = tmp;
}
