// MHFU status based effect

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
SettingName4=Stamina Blur
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
  float blur_fac = 0.0;
  if (u_setting.y < 150.0 && u_setting.w > 0.5)
    blur_fac = (1.0-u_setting.y/150.0)*10.0*sin(u_time.x);
  float sat_fac = 1.0;
  if (u_setting.z > 0.5)
    sat_fac = u_setting.x/100.0;

  vec2 offset = u_pixelDelta*blur_fac;
  
  vec3 rgbSimple0 =  1.0 * texture2D(sampler0, v_texcoord0.xy + offset * vec2( 0.0,-2.0)).xyz;
  vec3 rgbSimple1 =  3.0 * texture2D(sampler0, v_texcoord0.xy + offset * vec2(-1.0,-1.0)).xyz;
  vec3 rgbSimple2 =  8.0 * texture2D(sampler0, v_texcoord0.xy + offset * vec2( 0.0,-1.0)).xyz;
  vec3 rgbSimple3 =  3.0 * texture2D(sampler0, v_texcoord0.xy + offset * vec2( 1.0,-1.0)).xyz;
  vec3 rgbSimple4 =  1.0 * texture2D(sampler0, v_texcoord0.xy + offset * vec2(-2.0, 0.0)).xyz;
  vec3 rgbSimple5 =  8.0 * texture2D(sampler0, v_texcoord0.xy + offset * vec2(-1.0, 0.0)).xyz;
  vec3 rgbSimple6 = 10.0 * texture2D(sampler0, v_texcoord0.xy + offset * vec2( 0.0, 0.0)).xyz;
  vec3 rgbSimple7 =  8.0 * texture2D(sampler0, v_texcoord0.xy + offset * vec2( 1.0, 0.0)).xyz;
  vec3 rgbSimple8 =  1.0 * texture2D(sampler0, v_texcoord0.xy + offset * vec2( 2.0, 0.0)).xyz;
  vec3 rgbSimple9 =  3.0 * texture2D(sampler0, v_texcoord0.xy + offset * vec2(-1.0, 1.0)).xyz;
  vec3 rgbSimple10=  8.0 * texture2D(sampler0, v_texcoord0.xy + offset * vec2( 0.0, 1.0)).xyz;
  vec3 rgbSimple11=  3.0 * texture2D(sampler0, v_texcoord0.xy + offset * vec2( 1.0, 1.0)).xyz;
  vec3 rgbSimple12=  1.0 * texture2D(sampler0, v_texcoord0.xy + offset * vec2( 0.0, 2.0)).xyz;
  
  vec3 rgb =  rgbSimple0 + 
              rgbSimple1 +
              rgbSimple2 +
              rgbSimple3 +
              rgbSimple4 +
              rgbSimple5 +
              rgbSimple6 +
              rgbSimple7 +
              rgbSimple8 +
              rgbSimple9 +
              rgbSimple10 +
              rgbSimple11 +
              rgbSimple12;
  rgb = rgb / 58.0;

  gl_FragColor.rgb = mix(vec3(dot(rgb, vec3(0.299, 0.587, 0.114))), rgb, sat_fac);
  gl_FragColor.a = 1.0;
}
