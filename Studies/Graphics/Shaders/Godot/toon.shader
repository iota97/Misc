// GodotEngine 3.1.1 toon shader

shader_type spatial;
render_mode unshaded;

uniform vec3 albedo;
uniform vec3 sun_pos;
uniform vec3 sun_col;

varying float light;

float len(vec3 v)
{
	return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}

void vertex()
{
	vec3 norm = normalize((WORLD_MATRIX * vec4(NORMAL, 0.0)).rgb);
	light = dot(normalize(sun_pos), norm);
}

void fragment()
{
	float fac = 1.0;
	
	if (light > 0.98)
		fac = 1.2;
	else if (light < -0.2)
		fac = 0.7;
	
	ALBEDO = albedo*sun_col*fac;
}
