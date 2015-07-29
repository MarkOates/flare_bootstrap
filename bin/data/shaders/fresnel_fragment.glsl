
uniform vec3 camera_position;
uniform sampler2D al_tex;

varying vec3 surface_normal;
varying vec3 world_position;
varying vec2 texture_coords;
#define _sharpness 1 

void main()
{
	vec3 V = normalize(camera_position - world_position); 
	vec3 N = normalize(surface_normal); 
	float fresnel = pow(1.0 - dot(N, V), _sharpness);
	gl_FragColor = vec4(fresnel, fresnel, fresnel, 1.0);
}


