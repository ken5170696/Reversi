#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform float aspect;

void main()
{
    vec3 col = texture(screenTexture, TexCoords).rgb;
    if ( 0.5 - 0.001 < TexCoords.x && TexCoords.x < 0.5 + 0.001 &&
        0.5 - 0.001 < TexCoords.y && TexCoords.y < 0.5 + 0.001){
        FragColor = vec4(1.0,0.0,0.0, 1.0);
    } else{
        FragColor = vec4(col, 1.0);
    }
} 