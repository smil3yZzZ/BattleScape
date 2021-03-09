#version 330 core

//layout (pixel_center_integer​) in vec4 gl_FragCoord;

in vec3 quadColorPrimary;
//in vec3 quadColorSecondary;
in vec4 pPos;

out vec4 FragColor;

int max3(vec3 v);
float rand(vec2 co);


void main()
{
    int weight;
    int xPos = int(mod(pPos.x, 64));
    int yPos = int(mod(pPos.y, 64));
    if (xPos < 31.5 && yPos < 31.5) {
        int weight = int(min(xPos, yPos));
        float randValue = rand(vec2(xPos, yPos));
        vec4 color = mod(xPos, 2) == 0 || mod(yPos, 2) == 0 || weight < 2 || weight < 2 ? vec4(0.2, 0.2, 0.2, 1.0) : vec4(quadColorPrimary.x/255.0, quadColorPrimary.y/255.0, quadColorPrimary.z/255.0, 1.0);
        color[max3(color.xyz)] = color[max3(color.xyz)];// * randValue;
        //FragColor = weight < 4 ? vec4(0.0 + randValue/(33 - weight) * 4, 0.0 + randValue/(33 - weight) * 4, 0.0 + randValue/(33 - weight) * 4, 1.0) : color;
        FragColor = color;
    }
    else if (xPos < 31.5 && yPos > 31.5) {
        int weight = int(min(xPos, 63 - yPos));
        float randValue = rand(vec2(xPos, yPos));
        vec4 color = mod(xPos, 2) == 0 || mod(yPos, 2) == 0 || weight < 2 || weight < 2 ? vec4(0.2, 0.2, 0.2, 1.0) : vec4(quadColorPrimary.x/255.0, quadColorPrimary.y/255.0, quadColorPrimary.z/255.0, 1.0);
        color[max3(color.xyz)] = color[max3(color.xyz)];// * randValue;
        //FragColor = weight < 4 ? vec4(0.0 + randValue/(33 - weight) * 4, 0.0 + randValue/(33 - weight) * 4, 0.0 + randValue/(33 - weight) * 4, 1.0) : color;
        FragColor = color;
    }
    else if (xPos > 31.5 && yPos < 31.5) {
        int weight = int(min(63 - xPos, yPos));
        float randValue = rand(vec2(xPos, yPos));
        vec4 color = mod(xPos, 2) == 0 || mod(yPos, 2) == 0 || weight < 2 || weight < 2 ? vec4(0.2, 0.2, 0.2, 1.0) : vec4(quadColorPrimary.x/255.0, quadColorPrimary.y/255.0, quadColorPrimary.z/255.0, 1.0);
        color[max3(color.xyz)] = color[max3(color.xyz)];// * randValue;
        //FragColor = weight < 4 ? vec4(0.0 + randValue/(33 - weight) * 4, 0.0 + randValue/(33 - weight) * 4, 0.0 + randValue/(33 - weight) * 4, 1.0) : color;
        FragColor = color;
    }
    else if (xPos > 31.5 && yPos > 31.5) {
        int weight = int(min(63 - xPos, 63 - yPos));
        float randValue = rand(vec2(xPos, yPos));
        vec4 color = mod(xPos, 2) == 0 || mod(yPos, 2) == 0 || weight < 2 || weight < 2 ? vec4(0.2, 0.2, 0.2, 1.0) : vec4(quadColorPrimary.x/255.0, quadColorPrimary.y/255.0, quadColorPrimary.z/255.0, 1.0);
        color[max3(color.xyz)] = color[max3(color.xyz)];// * randValue;
        FragColor = color;
    }
    /*
    else {
        FragColor = vec4(quadColorPrimary.x/255.0, quadColorPrimary.y/255.0, quadColorPrimary.z/255.0, 1.0);
    }*/
}

int max3(vec3 v) {
  return v.y > v.x ? (v.z > v.y ? 2 : 1) : (v.z > v.x ? 2 : 0);
}

float rand(vec2 co) {
    float value = fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
    return value > 0.75 ? 0.95 : (value > 0.5 ? 0.6 : (value > 0.25 ? 0.3 : 0.05));
    //return value > 0.5 ? 0.75 : 0.25;
    //return 0.5;
}
