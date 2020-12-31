#version 330 core

in vec3 quadColor;
in vec4 pPos;

out vec4 FragColor;

void main()
{
    /*
    float xPos = mod(pPos.x, 64);
    float yPos = mod(pPos.y, 64);
    if (xPos < 32 && yPos < 32) {
        float weight = min(xPos, yPos);
        //FragColor = vec4(quadColor/255.0, 1.0)*(weight/32);
        FragColor = vec4(1.0, 0.5, 0.5, 1.0);
    }
    if (xPos < 32 && yPos > 32) {
        float weight = min(xPos, 64 - yPos);
        //FragColor = vec4(quadColor/255.0, 1.0)*(weight/32);
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
    if (xPos > 32 && yPos < 32) {
        float weight = min(64 - xPos, yPos);
        //FragColor = vec4(quadColor/255.0, 1.0)*(weight/32);
        FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    }
    if (xPos > 32 && yPos > 32) {
        float weight = min(64 - xPos, 64 - yPos);
        //FragColor = vec4(quadColor/255.0, 1.0)*(weight/32);
        FragColor = vec4(0.0, 0.0, 1.0, 1.0);
    }*/
    FragColor = vec4(quadColor/255.0, 1.0);
}
