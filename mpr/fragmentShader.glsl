#version 400
in vec3 outColor;
in vec2 textureCoords;
 
out vec4 frag_colour;

uniform sampler2D myTextureSampler;
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