// MHFU status based effect v2

/*
_C1 Status based shader
_L 0xA3000100 0x090B3724
_L 0xA3000101 0x090B399A

[MHFUHP]
Name=MHFU Status
Fragment=mhfu_hp.fsh
Vertex=fxaa.vsh
SettingName3=HP Saturation
SettingDefaultValue3=1.0
SettingMaxValue3=1.0
SettingMinValue3=0.0
SettingStep3=1.0
SettingName4=Stamina Shake
SettingDefaultValue4=1.0
SettingMaxValue4=1.0
SettingMinValue4=0.0
SettingStep4=1.0
*/

#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D sampler0;
uniform vec2 u_pixelDelta;
uniform vec4 u_time;
uniform vec4 u_setting;
varying vec2 v_texcoord0;

void main()
{
  float shake_fac = 0.0;
  if (u_setting.y < 200.0 && u_setting.w > 0.5)
    shake_fac = (1.0-u_setting.y/150.0)*0.1;
  float sat_fac = 1.0;
  if (u_setting.z > 0.5)
    sat_fac = u_setting.x/100.0;

  vec2 uv = v_texcoord0.xy;
  float t = clamp(mod(u_time.x, 10.0), 0.0, 10.0)*0.1;
  float shake_magnitude = shake_fac * sin(31.4159*t) * (t+0.5)*t*t*(t-1.0)*(t-1.0)*15.5;
  uv.y += shake_magnitude;
  float trans_magnitude = shake_fac * sin(31.4159*t) * -t*(t-1.);
  uv -= trans_magnitude;

  vec3 col = texture2D(sampler0, uv).rgb;

  gl_FragColor.rgb = mix(vec3(dot(col, vec3(0.299, 0.587, 0.114))), col, sat_fac);
  gl_FragColor.a = 1.0;
}
