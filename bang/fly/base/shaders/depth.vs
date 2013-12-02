#version 120

uniform mat4 WORLD_MATRIX;
uniform mat4 PROJ_MATRIX;
uniform mat4 VIEW_MATRIX;

void main()
{
    gl_Position = PROJ_MATRIX * VIEW_MATRIX * WORLD_MATRIX * gl_Vertex;
}
