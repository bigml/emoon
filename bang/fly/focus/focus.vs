#version 120

/*
 * all input variable UPPERCASE
 */
attribute vec3 TANGENT;
attribute vec3 BINORMAL;

uniform mat4 WORLD_MATRIX;
uniform mat4 PROJ_MATRIX;
uniform mat4 VIEW_MATRIX;
uniform float TIME_UPMS;

/*
 * all varying variable V_
 */
varying vec2 V_UVS;
varying vec4 V_POS;
varying mat4 V_TBN;

void main()
{
    vec2 temp_uvs = vec2(gl_MultiTexCoord0);
    V_UVS = vec2(temp_uvs.x, -temp_uvs.y);

    vec4 pos = gl_Vertex;
    pos.z -= TIME_UPMS * 0.1;
    V_POS = WORLD_MATRIX * pos;

    mat3 world_rot = mat3(WORLD_MATRIX);
    vec3 w_tangent = normalize(world_rot * TANGENT);
    vec3 w_binormal = normalize(world_rot * BINORMAL);
    vec3 w_normal = normalize(world_rot * gl_Normal);
    V_TBN = mat4(vec4(w_tangent,  0.0),
                 vec4(w_binormal, 0.0),
                 vec4(w_normal,   0.0),
                 vec4(0.0,0.0,0.0,1.0)
        );

    gl_Position = PROJ_MATRIX * VIEW_MATRIX * V_POS;
}
