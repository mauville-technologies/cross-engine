#version 300 es
precision highp float;
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(ourTexture, TexCoord), texture(texture2, TexCoord), 0.2) * vec4(ourColor, 1.f);
}