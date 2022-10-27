#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 Transform;
uniform mat4 ViewProjection;

void main()
{
    TexCoords = aTexCoords;
    FragPos = vec3(Transform * vec4(aPos, 1.0));
    //Normal = mat3(transpose(inverse(Transform))) * aNormal; 
    Normal = mat3(Transform) * aNormal;
    gl_Position = ViewProjection* vec4(FragPos, 1.0); 
}