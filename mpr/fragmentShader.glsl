#version 400
in vec3 outColor;
in vec3 textureCoords;
 
out vec4 frag_colour;

uniform sampler3D myTextureSampler;
uniform bool useTexture;

uniform float windowLevel;
uniform float windowWidth;

void main() {
  if(useTexture) 
  {
	 float scalar = texture( myTextureSampler, textureCoords )[0];
	 float _lowerVal = windowLevel - (windowWidth/2);
	 float _upperVal = windowLevel + (windowWidth/2);
	 scalar = scalar - _lowerVal;
	 scalar = scalar / (_upperVal - _lowerVal);
     frag_colour = vec4(scalar, scalar, scalar, scalar);
  }
  else
  {
     frag_colour = vec4(outColor, 1.0);
  }
 
}