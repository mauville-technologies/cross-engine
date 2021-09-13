//
// Created by ozzadar on 2021-08-25.
//

#pragma once
#include <string>

const std::string simpleVertexShader = R"(#version 300 es
layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos, 1.0f);
}
)";

const std::string simpleFragmentShader = R"(#version 300 es
precision highp float;
out vec4 FragColor;

uniform vec4 ourColor;

void main()
{
    FragColor = ourColor;
}
)";