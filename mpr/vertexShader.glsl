#version 400
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vc;
layout(location = 2) in vec2 uv;

uniform bool useTexture;

out vec3 outColor;
out vec2 textureCoords;

void main() {
  gl_Position = vec4(vp, 1.0);
  outColor = vc;
  textureCoords = uv;
}