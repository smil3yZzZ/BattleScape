#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
    // Radius variable calculation (circle formula)
    float powRadius = pow(TexCoord.x - 0.5, 2.0) + pow(TexCoord.y - 0.5, 2.0);

    float radius = sqrt(powRadius);
    // Output to screen
    if (radius < 0.5) {
        FragColor = texture(texture1, TexCoord);
    }
    else {
        FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    }

}