#version 120

vec3 color_correction(vec3 color, sampler3D lut, int lut_size)
{
    float scale = (float(lut_size) - 1.0) / float(lut_size);
    float offset = 1.0 / (2.0 * float(lut_size));

    return texture3D(lut, clamp(color, 0.0, 1.0) * scale + offset).rgb;
}
