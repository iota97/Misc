// Sobel edge detect

#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D sampler0;
uniform vec2 u_texelDelta;
varying vec2 v_texcoord0;

void main() {

  // Get color of addiacent pixels
  vec3 col0 = texture2D(sampler0, v_texcoord0 + vec2(-1.0, -1.0)*u_texelDelta).rgb;
  vec3 col1 = texture2D(sampler0, v_texcoord0 + vec2(0.0, -1.0)*u_texelDelta).rgb;
  vec3 col2 = texture2D(sampler0, v_texcoord0 + vec2(1.0, -1.0)*u_texelDelta).rgb;
  vec3 col3 = texture2D(sampler0, v_texcoord0 + vec2(-1.0, 0.0)*u_texelDelta).rgb;
  vec3 col5 = texture2D(sampler0, v_texcoord0 + vec2(1.0, 0.0)*u_texelDelta).rgb;
  vec3 col6 = texture2D(sampler0, v_texcoord0 + vec2(-1.0, 1.0)*u_texelDelta).rgb;
  vec3 col7 = texture2D(sampler0, v_texcoord0 + vec2(0.0, 1.0)*u_texelDelta).rgb;
  vec3 col8 = texture2D(sampler0, v_texcoord0 + vec2(1.0, 1.0)*u_texelDelta).rgb;

  // Compute the vertical and horizzontal component
  vec3 colh = col0+2.0*col1+col2-col6-2.0*col7-col8;
  vec3 colv = col2+2.0*col5+col8-col0-2.0*col3-col6;

  // Output
  vec3 col = sqrt(colh*colh+colv*colv);
  gl_FragColor.rgb = 1.0 - vec3(col.r+col.g+col.b)/3.0;
  gl_FragColor.a = 1.0;

}
