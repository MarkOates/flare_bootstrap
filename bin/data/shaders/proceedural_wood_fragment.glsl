

uniform vec3 camera_position;
uniform sampler2D al_tex;

varying vec3 surface_normal;
varying vec3 world_position;
varying vec3 local_position;
varying vec2 texture_coords;
#define _sharpness 1.0

uniform vec3 light_position;


/*
// this technique for getting random numbers is very good,
// however, it is per-pixel random and not per-position random, so it
// introduces complexities
*/
float rand_(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec3 get_proceedural_wood_color(in vec3 position)
{
  //float solid = 0.5;
  //if (mod(world_position.x, 0.1) < 0.05) solid += 0.5;
  //if (mod(world_position.y, 0.1) < 0.05) solid += 0.5;
  //if (mod(world_position.z, 0.1) < 0.05) solid += 0.5;

  float seed = 1.0;//rand(vec2(24567, 1243567));
  float r1 = rand_(vec2(seed, floor(position.x*100.0)/100.0));
  float r2 = rand_(vec2(seed, floor(position.z*100.0)/100.0));
   //float r1 = 1;
   //float r2 = 1;

  vec2 center_slope = vec2(0.21, 0.15) * position.y;

   vec2 circle_center = vec2(0.0, 0.0);
   float solid = distance(circle_center,
      vec2(
         (center_slope.x + position.x) * 0.9 + r1 * 0.01,
         (center_slope.y + position.z) * 0.9 + r2 * 0.01)) * 15.0;

  solid = mod(solid, 1.0);

//254, 196, 112
//194, 121, 45
  vec3 color_1 = vec3(254.0/255.0, 196.0/255.0, 112.0/255.0);
  vec3 color_2 = vec3(194.0/255.0, 121.0/255.0, 45.0/255.0);


	return vec3(
    color_1.r * solid + color_2.r * (1.0-solid),
    color_1.g * solid + color_2.g * (1.0-solid),
    color_1.b * solid + color_2.b * (1.0-solid));
}


void main()
{
	vec3 V = normalize(camera_position - world_position);
	vec3 L = normalize(light_position - world_position);
	vec3 N = normalize(surface_normal);
	float reflectance = max(dot(L, N), 0.0);
	float fresnel = pow(1.0 - dot(N, V), _sharpness);

	// gl_FragColor = vec4(fresnel, fresnel, fresnel, 1.0);

   vec3 wood_texture_color = get_proceedural_wood_color(local_position.xzy);

	gl_FragColor = vec4(wood_texture_color * (0.7+reflectance*0.3), 1.0);
}


