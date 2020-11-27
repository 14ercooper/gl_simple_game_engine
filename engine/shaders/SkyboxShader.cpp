
#include "SkyboxShader.h"

const char* SkyboxShader::vertexProgram =
R"FOURTEENER(#version 410 core
// So we know where this is
layout (location = 0) in vec3 pos;

// Texture location
out vec3 uvCoords;

// Map into the correct coordinate space
uniform mat4 projection;
uniform mat4 view;

void main()
{
	// Transform and/or pass through
    uvCoords = pos;
    gl_Position = projection * view * vec4(pos, 1.0);
}
)FOURTEENER";

const char* SkyboxShader::fragmentProgram =
R"FOURTEENER(#version 410 core
out vec4 color;

in vec3 uvCoords;

// Cubemaps are nice since uv are implicit
uniform samplerCube skybox;

void main()
{
	// Use the cubemap to get the correct texture color from the cube
    color = texture(skybox, uvCoords);
}
)FOURTEENER";

SkyboxShader::SkyboxShader (std::vector<std::string> textures) {
	GLuint vertex = compileShaderText(SkyboxShader::vertexProgram, GL_VERTEX_SHADER);
	GLuint fragment = compileShaderText(SkyboxShader::fragmentProgram, GL_FRAGMENT_SHADER);

	programHandle = glCreateProgram();

	glAttachShader(programHandle, vertex);
	glAttachShader(programHandle, fragment);

	glLinkProgram(programHandle);

	printLog(programHandle);

	glDetachShader(programHandle, vertex);
	glDetachShader(programHandle, fragment);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	glUseProgram(programHandle);

	glGenTextures(1, &cubemapId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	int imageWidth, imageHeight, imageChannels;
    unsigned char *data;
    for (int i = 0; i < 6; i++) {
    	data = stbi_load(textures[i].c_str(), &imageWidth, &imageHeight, &imageChannels, 0);
    	if (data) {
	    	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	    	stbi_image_free(data);
    	}
    	else {
    		printf("Error loadin skybox image with id %i\n", i);
    	}
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    float skyboxVertices[] = {
	    -1.0f,  1.0f, -1.0f,
	    -1.0f, -1.0f, -1.0f,
	     1.0f, -1.0f, -1.0f,
	     1.0f, -1.0f, -1.0f,
	     1.0f,  1.0f, -1.0f,
	    -1.0f,  1.0f, -1.0f,
	    -1.0f, -1.0f,  1.0f,
	    -1.0f, -1.0f, -1.0f,
	    -1.0f,  1.0f, -1.0f,
	    -1.0f,  1.0f, -1.0f,
	    -1.0f,  1.0f,  1.0f,
	    -1.0f, -1.0f,  1.0f,
	     1.0f, -1.0f, -1.0f,
	     1.0f, -1.0f,  1.0f,
	     1.0f,  1.0f,  1.0f,
	     1.0f,  1.0f,  1.0f,
	     1.0f,  1.0f, -1.0f,
	     1.0f, -1.0f, -1.0f,
	    -1.0f, -1.0f,  1.0f,
	    -1.0f,  1.0f,  1.0f,
	     1.0f,  1.0f,  1.0f,
	     1.0f,  1.0f,  1.0f,
	     1.0f, -1.0f,  1.0f,
	    -1.0f, -1.0f,  1.0f,
	    -1.0f,  1.0f, -1.0f,
	     1.0f,  1.0f, -1.0f,
	     1.0f,  1.0f,  1.0f,
	     1.0f,  1.0f,  1.0f,
	    -1.0f,  1.0f,  1.0f,
	    -1.0f,  1.0f, -1.0f,
	    -1.0f, -1.0f, -1.0f,
	    -1.0f, -1.0f,  1.0f,
	     1.0f, -1.0f, -1.0f,
	     1.0f, -1.0f, -1.0f,
	    -1.0f, -1.0f,  1.0f,
	     1.0f, -1.0f,  1.0f
	};

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*) 0);
}

SkyboxShader::~SkyboxShader() {
	glDeleteTextures(1, &cubemapId);
	glBindVertexArray(vao);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);}

void SkyboxShader::enableAttribs() {
	// This is part of setup, so we don't need to do anything
}

void SkyboxShader::drawSkybox() {
	useProgram();
	glDepthMask(GL_FALSE);

	uniformMat4("projection", GameEngine::engineCamera->getProjectionMatrix(GameEngine::engineWindow));
	uniformMat4("view", glm::mat4(glm::mat3(GameEngine::engineCamera->getViewMatrix())));

	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthMask(GL_TRUE);
}
