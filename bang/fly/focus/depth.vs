#version 120

uniform mat4 WORLD_MATRIX;
uniform mat4 PROJ_MATRIX;
uniform mat4 VIEW_MATRIX;
uniform float TIME_UPMS;

void main()
{
    vec4 pos = gl_Vertex;
    pos.z -= TIME_UPMS * 0.1;

    gl_Position = PROJ_MATRIX * VIEW_MATRIX * WORLD_MATRIX * pos;
}
