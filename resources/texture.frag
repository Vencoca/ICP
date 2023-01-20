#version 430 core
in vec4 color;
out vec4 FragColor;
// Material properties
uniform vec3 ambient_material, diffuse_material, specular_material;
uniform float specular_shinines;
uniform sampler2D tex0; // texture unit from C++
// Input from vertex shader
in VS_OUT
{
	vec3 N;
	vec3 L;
	vec3 V;
	vec2 texcoord;
} fs_in;
void main(void)
{
// Normalize the incoming N, L and V vectors
vec3 N = normalize(fs_in.N);
vec3 L = normalize(fs_in.L);
vec3 V = normalize(fs_in.V);
// Calculate R by reflecting -L around the plane defined by N
vec3 R = reflect(-L, N);
// Calculate the ambient, diffuse and specular contributions
vec3 ambient = ambient_material * vec3(1.0);
vec3 diffuse = max(dot(N, L), 0.0) * diffuse_material;
vec3 specular = pow(max(dot(R, V), 0.0), 2.5) * specular_material;
//FragColor = vec4(ambient + diffuse + specular, 1.0);
//FragColor = vec4(ambient + diffuse + specular, 1.0);
FragColor = texture(tex0, fs_in.texcoord);
}