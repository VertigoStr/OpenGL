varying vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    gl_FragColor = textureCube(skybox, TexCoords);// * vec4(1.0 , 0.0, 0.0, 0.0);
}
  