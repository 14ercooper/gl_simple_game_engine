
#ifndef SHADER_PROGRAM_CLASS
#define SHADER_PROGRAM_CLASS

#include <GL/glew.h>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstring>
#include <glm/glm.hpp>

class ShaderProgram {
public:
	ShaderProgram();
	ShaderProgram(std::string vertexShader, std::string fragmentShader);
	ShaderProgram(std::string vertexShader, std::string geometryShader, std::string fragmentShader);
	~ShaderProgram();

	GLuint getUniformLocation(std::string name);
	GLuint getAttributeLocation(std::string name);

	void useProgram();

	void uniformFloat(std::string pos, GLfloat value);
	void uniformVec3(std::string pos, glm::vec3 value);
	void uniformMat4(std::string pos, glm::mat4 value);

protected:
	void readTextFromFile( const char* filename, char* &output );
	void printLog( GLuint handle );
	GLuint compileShader( const char* filename, GLenum shaderType );

	GLuint programHandle;
};

#endif
