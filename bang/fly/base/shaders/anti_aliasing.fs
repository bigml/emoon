#version 120

vec3 fxaa_unsharp(sampler2D screen, vec2 pos, int width, int height);

vec3 fxaa_unsharp(sampler2D screen, vec2 pos, int width, int height)
{
    const float sharpen = 1.0;
    const float boundry = 0.2;

    float kernel = 1.0;

    float xoff = kernel / float(width);
    float yoff = kernel / float(height);

    vec3 rgb_ne = texture2D(screen, pos + vec2(-xoff,yoff)).rgb;
    vec3 rgb_n = texture2D(screen, pos + vec2(0.0,yoff)).rgb;
    vec3 rgb_nw = texture2D(screen, pos + vec2(xoff,yoff)).rgb;
    vec3 rgb_w = texture2D(screen, pos + vec2(xoff,0.0)).rgb;
    vec3 rgb_o = texture2D(screen, pos + vec2(0.0,0.0)).rgb;
    vec3 rgb_e = texture2D(screen, pos + vec2(-xoff,0.0)).rgb;
    vec3 rgb_sw = texture2D(screen, pos + vec2(-xoff,-yoff)).rgb;
    vec3 rgb_s = texture2D(screen, pos + vec2(0.0,-yoff)).rgb;
    vec3 rgb_se = texture2D(screen, pos + vec2(xoff,-yoff)).rgb;

    vec3 average = (rgb_ne + rgb_n + rgb_nw + rgb_w + rgb_e + rgb_sw + rgb_s + rgb_se) / 8.0;
    vec3 difference = rgb_o - average;

    rgb_o = rgb_o + (difference * sharpen);
    difference = rgb_o - average;

    float fdiff = abs(dot(vec3(1.0,1.0,1.0), difference));

    if (fdiff > boundry) {
        float alias_amount = clamp(fdiff * 2.0, 0.0, 0.75);
        return mix(rgb_o, average, alias_amount);
    } else {
        return rgb_o;
    }
}
