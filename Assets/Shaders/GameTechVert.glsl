#version 400 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 colour;
layout (location = 2) in vec2 texCoord;
layout(location = 3)  in vec3 normal;

uniform mat4 modelMatrix 	= mat4(1.0f);
uniform mat4 viewMatrix 	= mat4(1.0f);
uniform mat4 projMatrix 	= mat4(1.0f);

uniform vec4 objectColour = vec4(1,1,1,1);
uniform bool hasVertexColours = false;


uniform bool reverse_normals = true;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 vertColour;

void main()
{
    FragPos = vec3(modelMatrix * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(modelMatrix))) * normal;  
    TexCoords = texCoord;

    vertColour = objectColour;

	if(hasVertexColours) {
		vertColour	= objectColour * colour;
	}
    
    gl_Position = projMatrix * viewMatrix * vec4(FragPos, 1.0);
}
