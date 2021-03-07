#version 400 core

uniform mat4 modelMatrix 	= mat4(1.0f);
uniform mat4 viewMatrix 	= mat4(1.0f);
uniform mat4 projMatrix 	= mat4(1.0f);
uniform mat4 shadowMatrix 	= mat4(1.0f);

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 normal;

uniform vec4 objectColour = vec4(1,1,1,1);

uniform bool hasVertexColours = false;

out Vertex
{
	vec4 colour;
	vec2 texCoord;
	vec4 shadowProj;
	vec3 normal;
	vec3 worldPos;
} OUT;


out vec3 FragPos; // 顶点位置转换到世界空间的坐标
out vec3 Normal;
out vec2 TexCoords;


void main(void)
{
	mat3 normalMatrix = transpose ( inverse ( mat3 ( modelMatrix )));
//
	OUT.shadowProj 	=  shadowMatrix * vec4 ( position,1);
	OUT.worldPos 	= ( modelMatrix * vec4 ( position ,1)). xyz ;
	OUT.normal 		= normalize ( normalMatrix * normalize ( normal ));
	
	OUT.texCoord	= texCoord;
	OUT.colour		= objectColour;

	if(hasVertexColours) {
		OUT.colour		= objectColour * colour;
	}

	// 改用光照来解决
	FragPos = vec3(modelMatrix * vec4(position, 1.0));
	Normal = mat3(transpose(inverse(modelMatrix))) * normal;
	TexCoords = texCoord;



	mat4 mvp = (projMatrix * viewMatrix * modelMatrix);

	gl_Position	= mvp * vec4(position, 1.0);
}

