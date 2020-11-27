
#include "SecondPassShader.h"

const char* SecondPassShader::passthroughVertexProgram =
R"FOURTEENER(#version 410 core

uniform mat4 projectionMtx;

layout(location = 0) in vec3 vPos;
layout(location = 2) in vec2 vTexCoord;

layout(location = 2) out vec2 texCoord;

void main() {
    gl_Position = projectionMtx *  vec4(vPos, 1.0);
    texCoord = vTexCoord;
}
)FOURTEENER";

const char* SecondPassShader::passthroughFragmentProgram =
R"FOURTEENER(#version 410 core

uniform sampler2D fbo;

layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec4 fragColorOut;

void main() {
    vec4 texel = texture( fbo, texCoord );
    
    fragColorOut = clamp(texel, 0.0, 1.0);
}
)FOURTEENER";

SecondPassShader::SecondPassShader(int fboWidth, int fboHeight) {
	width = fboWidth;
	height = fboHeight;

	// Set up standard shader
	GLuint vertex = compileShaderText(SecondPassShader::passthroughVertexProgram, GL_VERTEX_SHADER);
	GLuint fragment = compileShaderText(SecondPassShader::passthroughFragmentProgram, GL_FRAGMENT_SHADER);

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

	// Set up FBOs
	glUniform1i(getUniformLocation("fbo"), 0);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureHandle, 0);

   struct VertexTextured {
        glm::vec3 pos;
        glm::vec2 texCoord;
    };

    const VertexTextured TEXTURED_QUAD_VERTICES[4] = {
            { glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f,  0.0f) }, // 0 - BL
            { glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec2(1.0f,  0.0f) }, // 1 - BR
            { glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec2(0.0f,  1.0f) }, // 2 - TL
            { glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec2(1.0f,  1.0f) }  // 3 - TR
    };

    const unsigned short TEXTURED_QUAD_INDICES[4] = {0, 1, 2, 3 };

    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &quadIBO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TEXTURED_QUAD_VERTICES), TEXTURED_QUAD_VERTICES, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TEXTURED_QUAD_INDICES), TEXTURED_QUAD_INDICES, GL_STATIC_DRAW);

    glEnableVertexAttribArray( getAttributeLocation("vPos") );
    glVertexAttribPointer(getAttributeLocation("vPos"), 3, GL_FLOAT, GL_FALSE, sizeof(VertexTextured), (void*) 0);

    glEnableVertexAttribArray(getAttributeLocation("vTexCoord"));
    glVertexAttribPointer(getAttributeLocation("vTexCoord"), 2, GL_FLOAT, GL_FALSE, sizeof(VertexTextured), (void*) (sizeof(GLfloat) * 3));
}

SecondPassShader::~SecondPassShader() {
    glDeleteTextures(1, &textureHandle);
    glBindVertexArray(quadVAO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteBuffers(1, &quadIBO);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteFramebuffers(1, &fbo);
    glDeleteRenderbuffers(1, &rbo);
}

void SecondPassShader::enableAttribs() {
	// We don't actually need to do this, since this is in setup
}

void SecondPassShader::setDrawToBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, width, height);
	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SecondPassShader::drawBufferToScreen() {
	glFlush();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLint windowWidth, windowHeight;
    glfwGetFramebufferSize( GameEngine::engineWindow, &windowWidth, &windowHeight );
    glViewport(0, 0, windowWidth, windowHeight);
    glm::mat4 projMtx = glm::ortho(-1, 1, -1, 1, -1, 1);
    useProgram();
    glUniformMatrix4fv(getUniformLocation("projectionMtx"), 1, GL_FALSE, &projMtx[0][0]);
    glBindTexture(GL_TEXTURE_2D, textureHandle);
    glBindVertexArray(quadVAO);
    glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (void*)0 );
}
