#version 450

layout (location = 0) in vec3 firstPoint;
layout (location = 1) in vec3 secondPoint;

uniform mat4 projection;
uniform mat4 view;

void main() {
    // OpenGL автоматически вызовет этот шейдер для каждой вершины.
    // Используем gl_VertexID для определения, какую вершину выводить.
    vec3 position = (gl_VertexID == 0) ? firstPoint : secondPoint;
    
    gl_Position = projection * view * vec4(position, 1.0);
}
