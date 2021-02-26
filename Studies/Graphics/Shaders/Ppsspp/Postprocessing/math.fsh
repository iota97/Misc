// Math shader based on UV, no texture

#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

varying vec2 v_texcoord0;

float rand(vec2 co){
    return fract(cos(co.x*20.)*sin(co.y*40.)+sin(co.x*70.)+cos(co.y*40.));
}

void main() {
  gl_FragColor.rgb = vec3(rand(v_texcoord0));
  gl_FragColor.a = 1.0;
}