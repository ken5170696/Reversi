#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D edgeTexture;

uniform float aspect;

uniform float delta = 0.004;

vec3 edge() {

    vec3 result = texture(edgeTexture, TexCoords).rgb * 8 -
                texture(edgeTexture, TexCoords + vec2(-delta,   -delta)).rgb -
                texture(edgeTexture, TexCoords + vec2(0.000f,   -delta)).rgb -
                texture(edgeTexture, TexCoords + vec2(delta,    -delta)).rgb -
                texture(edgeTexture, TexCoords + vec2(-delta,   0.000f)).rgb -
                texture(edgeTexture, TexCoords + vec2(delta,    0.000f)).rgb -
                texture(edgeTexture, TexCoords + vec2(-delta,   delta)).rgb  -
                texture(edgeTexture, TexCoords + vec2(0.000f,   delta)).rgb  -
                texture(edgeTexture, TexCoords + vec2(delta,    delta)).rgb;


    if(result.r > 0.5 )
        return vec3(1.0,1.0,1.0);

    return vec3(0.0,0.0,0.0);
}

void main()
{
    vec3 col = texture(screenTexture, TexCoords).rgb;
    if ( 0.5 - 0.001 < TexCoords.x && TexCoords.x < 0.5 + 0.001 &&
        0.5 - 0.001 < TexCoords.y && TexCoords.y < 0.5 + 0.001){
        FragColor = vec4(1.0,0.0,0.0, 1.0);
    } else{
        if(edge().r == 1.0f)
            FragColor = vec4(1.0f, 0.843f, 0.0f, 1.0f);
        else
            FragColor = vec4(col, 1.0f);
    }
} 
