#version 430 core

#define MASK_MAX_SIZE 64u

// Output
layout (location=0) out vec4 outColor;
layout (location=1) out vec4 outEmission;

layout (location=0) in vec2 texCoord;

// Input
uniform sampler2D postProcessing_0;
uniform sampler2D postProcessing_1;

uniform vec2 texelSize;

uniform bool horizontal;
uniform bool blend;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
	// Apply bluring horizontally or vertically depending on what is setted (more eficent than matrix kernel)
	vec3 result = texture(postProcessing_1, texCoord).rgb * weight[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(postProcessing_1, texCoord + vec2(texelSize.x * i, 0.0)).rgb * weight[i];
            result += texture(postProcessing_1, texCoord - vec2(texelSize.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(postProcessing_1, texCoord + vec2(0.0, texelSize.y * i)).rgb * weight[i];
            result += texture(postProcessing_1, texCoord - vec2(0.0, texelSize.y * i)).rgb * weight[i];
        }
    }

	// ================================================================================

	vec4 inColor = texture(postProcessing_0, texCoord);
	
	// If its the last pass, we will blend the result with the color texture
	vec3 finalColor = blend? inColor.rgb + result : inColor.rgb;

	outColor = vec4(finalColor, 1.0);
	outEmission = vec4(result * 1.2, 1.0);
}