#version 330 core
layout (location = 0) in vec3 positionAttribute;

void main() {
    gl_Position = vec4(positionAttribute, 1.0);
}
