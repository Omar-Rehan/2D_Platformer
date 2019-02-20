#version 330 core
out vec4 fragmentColor;

uniform bool hyper = false;

void main() {
    fragmentColor = vec4((hyper ? 0.5f : 1.0f), 0.0f, 0.0f, 1.0f);
}
