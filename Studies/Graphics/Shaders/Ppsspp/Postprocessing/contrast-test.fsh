// Color correction

#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D sampler0;
varying vec2 v_texcoord0;

uniform vec4 u_setting;

void main()
{
  vec3 rgb = texture2D(sampler0, v_texcoord0).xyz;
  rgb = vec3(mix(vec3(dot(rgb, vec3(0.299, 0.587, 0.114))), rgb, u_setting.y));

  if (v_texcoord0.x < 1.0/3.0) {
    rgb.r = 0.5 * ((rgb.r < 0.5 ? -1.0 : 1.0)*pow((rgb.r < 0.5 ? -1.0 : 1.0)*(2.0*rgb.r - 1.0), 1.0 / u_setting.z) + 1.0);
    rgb.g = 0.5 * ((rgb.g < 0.5 ? -1.0 : 1.0)*pow((rgb.g < 0.5 ? -1.0 : 1.0)*(2.0*rgb.g - 1.0), 1.0 / u_setting.z) + 1.0);
    rgb.b = 0.5 * ((rgb.b < 0.5 ? -1.0 : 1.0)*pow((rgb.b < 0.5 ? -1.0 : 1.0)*(2.0*rgb.b - 1.0), 1.0 / u_setting.z) + 1.0);
  } else if (v_texcoord0.x > 2.0/3.0) {
    rgb = (rgb-0.5)*u_setting.z+0.5;
  }

  rgb = rgb + u_setting.x-1.0;
  gl_FragColor.rgb = pow(rgb, vec3(1.0/u_setting.w));
  gl_FragColor.a = 1.0;
}
