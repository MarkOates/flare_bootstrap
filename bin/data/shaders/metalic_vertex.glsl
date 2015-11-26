

attribute vec4 al_pos;
attribute vec3 al_user_attr_0;
attribute vec2 al_texcoord;

uniform mat4 al_projview_matrix;
uniform mat4 position_transform; // provided by the user, this is the model's ALLEGRO_TRANSFORM

varying vec3 surface_normal;
varying vec3 world_position;
varying vec2 texture_coords;

void main()
{
	surface_normal = (position_transform * vec4(al_user_attr_0, 0.0)).xyz;
	world_position = (position_transform * al_pos).xyz;
	texture_coords = al_texcoord;
	gl_Position = al_projview_matrix * position_transform * al_pos;
}


