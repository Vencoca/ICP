#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

uniform mat4 uP_m = mat4(1.0f);
uniform mat4 uM_m = mat4(1.0f);
uniform mat4 uV_m = mat4(1.0f);

// Light properties
uniform vec3 light_position;

out VS_OUT
{ 
vec3 N;
vec3 L;
vec3 V;
} vs_out;

out vec4 color;

void main()
{
    vec4 aPosition = vec4(aPos, 1.0f);
    // Create Model-View matrix
    mat4 mv_m = uV_m * uM_m;
    // Calculate view-space coordinate - in P point we are computing the color
    vec4 P = mv_m * aPosition;
    // Calculate normal in view space
    vs_out.N = mat3(mv_m) * aNormal;
    // Calculate view-space light vector
    vs_out.L = light_position - P.xyz;
    // Calculate view vector (negative of the view-space position)
    vs_out.V = -P.xyz;
    // Calculate the clip-space position of each vertex
    gl_Position = uP_m * P;



    // Outputs the positions/coordinates of all vertices
    //gl_Position = uP_m * uV_m * uM_m * aPosition;
    color = vec4(aColor, 1.0f);
}