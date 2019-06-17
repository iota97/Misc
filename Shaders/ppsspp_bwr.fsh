// Black and White with Red

#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D sampler0;
varying vec2 v_texcoord0;

vec3 rgb2hsv(vec3 c) {

  // rgb to hsv convert function, totally not stackoverflow copypaste...

  vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
  vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
  vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

  float d = q.x - min(q.w, q.y);
  float e = 1.0e-10;
  return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);

}

void main() {

  // get the "pixel" rgb value
  vec3 rgb = texture2D(sampler0, v_texcoord0.xy).xyz;

  // convert it to hsv (hue, saturation, value)
  vec3 hsv = rgb2hsv(rgb);

  // check the hue for "red" range, and for minimum saturation and "lightness"
  if ((hsv.x > 0.95 || hsv.x < 0.05) && hsv.y > 0.3 && hsv.z > 0.4) {

    // keep the red, set green and blue to 0
    gl_FragColor.rgb = vec3(rgb.r, 0, 0);

  }	

  else {

    // calculate the luminance of the pixel, different weight per channel (full blue doesn't look at bright as full green)
    float luma = dot(rgb, vec3(0.299, 0.587, 0.114));

    // increase the contrast and use it for all the channels
    luma = 1.2 * (luma - 0.5) + 0.5;
    gl_FragColor.rgb = vec3(luma, luma, luma);

  }

  // set the alpha of the fragment, was in other shaders, work fine without it, may be needed on different GPU?
  gl_FragColor.a = 1.0;

}
