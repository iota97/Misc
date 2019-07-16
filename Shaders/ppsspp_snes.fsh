// SNES

#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D sampler0;
varying vec2 v_texcoord0;

void main() {

  // Pixelize
  vec2 coord = vec2(floor(v_texcoord0.x*160.)/160., floor(v_texcoord0.y*90.)/90.);

  // Reduce color depth
  vec3 col = floor(texture2D(sampler0, coord).xyz*5.)/5.;

  // Increase contrast
  col = 0.5 + 1.35*(col-0.5);

  // Output
  gl_FragColor.rgb = col;
  gl_FragColor.a = 1.0;

}
