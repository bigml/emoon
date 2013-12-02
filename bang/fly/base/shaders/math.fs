#version 120

vec3 pow3(vec3 col, float exponent)
{
    vec3 ret;
    ret.r = pow(col.r, exponent);
    ret.g = pow(col.g, exponent);
    ret.b = pow(col.b, exponent);
    return ret;
}
