#version 400
in vec3 outColor;
in vec3 textureCoords;
 
out vec4 frag_colour;

uniform sampler3D myTextureSampler;
uniform bool useTexture;

void main() {
  if(useTexture) 
  {
	 float scalar = texture( myTextureSampler, textureCoords )[0];
	 if (scalar < 1200) scalar = 0;
     frag_colour = vec4(scalar, 0, 0, 1);
  }
  else
  {
     frag_colour = vec4(outColor, 1.0);
  }
 
}