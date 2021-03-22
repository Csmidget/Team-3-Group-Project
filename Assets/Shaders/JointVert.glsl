#version 420 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 colour;
layout (location = 2) in vec2 texCoord;
layout(location = 3)  in vec3 normal;
layout (location = 5) in vec4 jointWeights;
layout (location = 6) in vec4 jointIndices;

uniform mat4 modelMatrix 	= mat4(1.0f);
uniform mat4 viewMatrix 	= mat4(1.0f);
uniform mat4 projMatrix 	= mat4(1.0f);

uniform vec4 objectColour = vec4(1,1,1,1);
uniform bool hasVertexColours = false;

uniform mat4 joints[128];

uniform bool reverse_normals = true;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 vertColour;


void main()
{
    Normal = mat3(transpose(inverse(modelMatrix))) * normal;  
    TexCoords = texCoord;

    vec4 localPos = vec4(position, 1.0f);
    vec4 skelPos = vec4(0,0,0,0);

    vertColour = objectColour;

	if(hasVertexColours) {
		vertColour	= objectColour * colour;
	}

    for (int i = 0; i < 4; ++i) {
        int jointIndex = int(jointIndices[i]);
        float jointWeight = jointWeights[i];
        skelPos += joints[jointIndex] * localPos * jointWeight;
    }

    FragPos = vec3(modelMatrix  * vec4(skelPos.xyz, 1.0) );
    gl_Position = projMatrix * viewMatrix  * vec4(FragPos.xyz, 1.0);
}
