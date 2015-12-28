

varying vec3 normal;
varying vec3 eye_dir;
uniform samplerCube cube_map;
//uniform sampler2D al_tex;
uniform bool reflecting;

void main()
{
	vec3 reflected_dir = normalize(reflect(eye_dir, normalize(normal)));

	vec3 incoming_angle = reflecting ? reflected_dir : eye_dir;
	//vec3 incoming_angle = eye_dir;

	incoming_angle.y = -incoming_angle.y;
	incoming_angle.x = -incoming_angle.x;
	//incoming_angle.z = -incoming_angle.z;

	vec4 color = textureCube(cube_map, incoming_angle);
	gl_FragColor = color;
}
