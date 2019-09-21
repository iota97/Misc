// GodotEngine 3.1.1 vertex lighting with normal

shader_type spatial;
render_mode unshaded;

uniform vec3 albedo;
uniform vec3 light_pos0;
uniform vec3 light_col0;
uniform float light_rad0;
uniform vec3 light_pos1;
uniform vec3 light_col1;
uniform float light_rad1;

varying vec3 light;

float len(vec3 v)
{
	return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}

void vertex()
{
	vec3 norm = normalize((WORLD_MATRIX * vec4(NORMAL, 0.0)).rgb);
	light = vec3(0.0);
	
	vec3 light_dir0 = (light_pos0 - (WORLD_MATRIX*vec4(VERTEX, 1.0)).xyz);
	vec3 light_dir1 = (light_pos1 - (WORLD_MATRIX*vec4(VERTEX, 1.0)).xyz);
	
	float light_fall0 = max((light_rad0 - len(light_dir0))/light_rad0, 0);
	float light_fall1 = max((light_rad1 - len(light_dir1))/light_rad1, 0);
	
	light += light_col0*max(dot(normalize(light_dir0), norm), 0)*light_fall0;
	light += light_col1*max(dot(normalize(light_dir1), norm), 0)*light_fall1;

	clamp(light, 0, 1);
}

void fragment()
{
	ALBEDO = albedo*light;
}
