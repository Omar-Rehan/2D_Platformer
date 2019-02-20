#version 330 core
layout (location = 0) in vec3 positionAttribute;

uniform mat4 modelMat;

void main() {
    gl_Position = modelMat * vec4(positionAttribute, 1.0);
}
