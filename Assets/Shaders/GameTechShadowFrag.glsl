#version 400 core

out vec4 fragColor;

in vec4 FragPos;
uniform float farPlane;
uniform vec3 lightPos;
void main(void)
{
    fragColor = vec4 (1,1,1,1);
    gl_FragDepth = length(lightPos-FragPos.xyz)/farPlane;
}
