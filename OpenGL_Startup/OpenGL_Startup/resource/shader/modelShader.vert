#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Bitangent;

uniform mat4 model;

layout (std140) uniform Matrices
{
   mat4 projection;
   mat4 view;
};

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

void main()
{
	vs_out.TexCoords = aTexCoord;
	vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));

	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}