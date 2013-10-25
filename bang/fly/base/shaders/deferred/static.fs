#version 120

varying vec4 position;
varying mat4 TBN;

uniform mat4 world_matrix;

uniform float glossiness;
uniform float bumpiness;

uniform float specular_level;
uniform float alpha_test;

uniform int mat_id;

uniform sampler2D diffuse_map;
uniform sampler2D bump_map;
uniform sampler2D spec_map;

uniform float near;
uniform float far;

/* Headers */

float to_gamma(float color);
float from_gamma(float color);
vec3 to_gamma(vec3 color);
vec3 from_gamma(vec3 color);
vec3 swap_red_green_inv(vec3 color);

/* End */

float linear_depth(float depth, float near, float far){
  return (2.0 * near) / (far + near - depth * (far - near));
}

void main( void ) {
  
	vec2 uvs = vec2(gl_TexCoord[0].x, -gl_TexCoord[0].y);

	float spec = texture2D(spec_map,uvs).r * specular_level;
	
	vec4 normal = texture2D(bump_map, uvs);
	normal.rgb = swap_red_green_inv(normal.rgb);
  
  normal = mix(normal, vec4( 0.5, 0.5, 1.0, 1.0 ), bumpiness);
	normal = (normal * 2.0 - vec4(1.0,1.0,1.0,0.0)) * TBN;
	
  vec4 diffuse = texture2D(diffuse_map, uvs);
  if (diffuse.a < alpha_test) {
    discard;
  }
  
	gl_FragData[0].rgb = from_gamma(diffuse.rgb);
	gl_FragData[0].a = spec;
	
	gl_FragData[1].rgb = position.xyz;
	gl_FragData[1].a = float(mat_id);
	
	gl_FragData[2] = normal;
	gl_FragData[2].a = glossiness;
  
  gl_FragDepth = linear_depth(gl_FragCoord.z, near, far);
  //gl_FragDepth = gl_FragCoord.z;
}