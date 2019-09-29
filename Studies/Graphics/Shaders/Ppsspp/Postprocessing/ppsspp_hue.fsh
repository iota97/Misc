// HSV hue loop

#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D sampler0;
uniform vec4 u_time;
varying vec2 v_texcoord0;

vec3 rgb2hsv(vec3 c) {
  vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
  vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
  vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
  float d = q.x - min(q.w, q.y);
  float e = 1.0e-10;
  return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

void main() {
  vec3 col;
  vec3 rgb = texture2D(sampler0, v_texcoord0.xy).xyz;
  vec3 hsv = rgb2hsv(rgb);

  float color_hue = fract(u_time.x*0.25);

  float color_range = 0.1;
  float hsv_min0 = color_hue - color_range;
  float hsv_max0 = color_hue + color_range;
  float hsv_min1 = 1.0;
  float hsv_max1 = 0.0;	

  if (hsv_min0 < 0.0)
  {
    hsv_min1 = 1.0 + hsv_min0;
    hsv_max1 = 1.0;
    hsv_min0 = 0.0;
  }

  if (hsv_max0 > 1.0)
  {
    hsv_max1 = hsv_max0 - 1.0;
    hsv_min1 = 0.0;
    hsv_max0 = 1.0;
  }

  if ((hsv.x > hsv_min0 && hsv.x < hsv_max0) || (hsv.x > hsv_min1 && hsv.x < hsv_max1))
  {
    col = rgb;
  }

  else
  {
    float lum = dot(vec3(0.299, 0.587, 0.114), rgb);
    col = vec3(lum);
  }
 
  gl_FragColor.rgb = col;
  gl_FragColor.a = 1.0;
}

