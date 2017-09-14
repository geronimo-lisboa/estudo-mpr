#version 400
in vec3 outColor;
in vec3 textureCoords;
 
out vec4 frag_colour;

uniform sampler3D myTextureSampler;
uniform bool useTexture;

void main() {
  if(useTexture) 
  {
     frag_colour = texture( myTextureSampler, textureCoords );
  }
  else
  {
     frag_colour = vec4(outColor, 1.0);
  }
 
}