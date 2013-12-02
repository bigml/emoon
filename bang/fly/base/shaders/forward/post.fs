#version 120

struct post_map {
    sampler2D diffuse;
    sampler2D vignetting;
    sampler3D lut;
};

struct post_attr {
    int width;
    int height;
    int msaa;
};

uniform post_map  MAP;
uniform post_attr ATTR;

vec3 pow3(vec3 col, float exponent);
vec3 color_correction(vec3 color, sampler3D lut, int lut_size);
vec3 fxaa_unsharp(sampler2D screen, vec2 pos, int width, int height);

void main(void)
{
    vec3 color;
    if (ATTR.msaa > 0) {
        /* use OpenGL MultiSampling Anti-Aliasing */
        //color = texture2D(MAP.diffuse, gl_TexCoord[0].xy).rgb;
        color = fxaa_unsharp(MAP.diffuse, gl_TexCoord[0].xy, ATTR.width, ATTR.height);
    } else {
        color = fxaa_unsharp(MAP.diffuse, gl_TexCoord[0].xy, ATTR.width, ATTR.height);
    }

    vec3 vignetting = texture2D(MAP.vignetting, gl_TexCoord[0].xy).rgb;

    color = color * mix(vignetting, vec3(1.0, 1.0, 1.0), 0.5);

    gl_FragColor.rgb = color_correction(color, MAP.lut, 64);
    gl_FragColor.a = 1.0;
}
