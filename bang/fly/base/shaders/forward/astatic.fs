#version 120

#define MAX_LIGHTS 32

struct static_light {
    float power;
    float falloff;
    vec3  position;
    vec3  target;
    vec3  diffuse;
    vec3  ambient;
    vec3  specular;
};

struct static_map {
    sampler2D diffuse;
    sampler2D bump;
    sampler2D spec;
};

struct static_attr {
    float glossiness;
    float specular_level;
};

/*
 * all input variable UPPERCASE except varying
 */
uniform vec3         CAMERA_POS;
uniform int          NUM_LIGHTS;
uniform static_light LIGHT[MAX_LIGHTS];
uniform static_map   MAP;
uniform static_attr  ATTR;

/*
 * all varaying variable V_
 */
varying vec2 V_UVS;
varying vec4 V_POS;
varying mat4 V_TBN;

void main()
{
    vec3 albedo = texture2D(MAP.diffuse, V_UVS).rgb;
    vec3 spec = texture2D(MAP.spec, V_UVS).rgb;
    vec3 bump = texture2D(MAP.bump, V_UVS).rgb;

    vec4 world_bump = V_TBN * vec4(bump, 1.0);
    vec3 normal = world_bump.xyz / world_bump.w;
    vec3 position = V_POS.xyz / V_POS.w;
    vec3 camera_vector = normalize(CAMERA_POS - position);

    vec3 diffuse = vec3(0.0,0.0,0.0);
    vec3 ambient = vec3(0.0,0.0,0.0);
    vec3 specular = vec3(0.0,0.0,0.0);

    for(int i = 0; i < NUM_LIGHTS; i++) {
        float power = LIGHT[i].power /
                      pow(length(LIGHT[i].position - position), LIGHT[i].falloff);

        vec3 light_dir = normalize(LIGHT[i].position - LIGHT[i].target);
        vec3 half_vector = normalize(light_dir + camera_vector);

        float n_dot_l = max(dot(normal, light_dir), 0.0);
        float n_dot_h = ATTR.specular_level *
                        pow(max(dot(normal, half_vector),0.0), ATTR.glossiness);

        diffuse += power * LIGHT[i].diffuse * albedo * n_dot_l;
        ambient += power * LIGHT[i].ambient * albedo;
        specular += power * LIGHT[i].specular * spec * n_dot_h;
    }

    gl_FragColor.rgb = diffuse + ambient + specular;
    gl_FragColor.a = texture2D(MAP.diffuse, V_UVS).a;
}
