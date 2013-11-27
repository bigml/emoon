#version 120

struct post_map {
    sampler2D diffuse;
    sampler2D vignetting;
    sampler3D lut;
};

struct post_attr {
    int width;
    int height;
};

uniform post_map  MAP;
uniform post_attr ATTR;

vec3 pow3(vec3 col, float exponent)
{
    vec3 ret;
    ret.r = pow(col.r, exponent);
    ret.g = pow(col.g, exponent);
    ret.b = pow(col.b, exponent);
    return ret;
}

vec3 color_correction(vec3 color, sampler3D lut, int lut_size)
{
    float scale = (float(lut_size) - 1.0) / float(lut_size);
    float offset = 1.0 / (2.0 * float(lut_size));

    return texture3D(lut, clamp(color, 0.0, 1.0) * scale + offset).rgb;
}

void main(void)
{
    vec3 color = texture2D(MAP.diffuse, gl_TexCoord[0].xy).rgb;
    vec3 vignetting = texture2D(MAP.vignetting, gl_TexCoord[0].xy).rgb;

    color = color * mix(vignetting, vec3(1.0, 1.0, 1.0), 0.5);

    gl_FragColor.rgb = color_correction(color, MAP.lut, 64);
    gl_FragColor.a = 1.0;
}
