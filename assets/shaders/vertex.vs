#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 modelModel;
uniform mat4 modelMesh;
uniform mat4 projection;
uniform mat4 view;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

void main()
{  
   mat4 model = modelMesh * modelModel;

   gl_Position = projection * view * model * vec4(aPos, 1.0);
   
   TexCoord = aTexCoord;
   Normal = mat3(transpose(inverse(model))) * aNormal;
   FragPos = vec3(model * vec4(aPos, 1.0));
   
}