// Sobel edge detect

#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D sampler0;
uniform vec2 u_texelDelta;
varying vec2 v_texcoord0;

// hsv2rgb snippet
vec3 hsv2rgb(vec3 c) {
  vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
  vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
  return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {

  // Get luminance of addiacent pixels
  vec3 lum = vec3(0.299, 0.587, 0.114);
  float col0 = dot(texture2D(sampler0, v_texcoord0 + vec2(-1.0, -1.0)*u_texelDelta).rgb, lum);
  float col1 = dot(texture2D(sampler0, v_texcoord0 + vec2(0.0, -1.0)*u_texelDelta).rgb, lum);
  float col2 = dot(texture2D(sampler0, v_texcoord0 + vec2(1.0, -1.0)*u_texelDelta).rgb, lum);
  float col3 = dot(texture2D(sampler0, v_texcoord0 + vec2(-1.0, 0.0)*u_texelDelta).rgb, lum);
  float col5 = dot(texture2D(sampler0, v_texcoord0 + vec2(1.0, 0.0)*u_texelDelta).rgb, lum);
  float col6 = dot(texture2D(sampler0, v_texcoord0 + vec2(-1.0, 1.0)*u_texelDelta).rgb, lum);
  float col7 = dot(texture2D(sampler0, v_texcoord0 + vec2(0.0, 1.0)*u_texelDelta).rgb, lum);
  float col8 = dot(texture2D(sampler0, v_texcoord0 + vec2(1.0, 1.0)*u_texelDelta).rgb, lum);

  // Compute the vertical and horizzontal component
  float colh = col0+2.0*col1+col2-col6-2.0*col7-col8;
  float colv = col2+2.0*col5+col8-col0-2.0*col3-col6;

  // Output color
  if (sqrt(colh*colh+colv*colv) > 0.8)
    gl_FragColor = texture2D(sampler0, v_texcoord0);
  else
    gl_FragColor = vec4(0.0);

}
