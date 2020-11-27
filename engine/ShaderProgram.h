
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
	ShaderProgram(std::string vertexShader, std::string tessControlShader, std::string tessEvalShader, std::string fragmentShader);
	ShaderProgram(std::string vertexShader, std::string tessControlShader, std::string tessEvalShader, std::string geometryShader, std::string fragmentShader);
	~ShaderProgram();

	GLuint getUniformLocation(std::string name);
	GLuint getAttributeLocation(std::string name);

	void useProgram();

	void uniformFloat(std::string pos, GLfloat value);
	void uniformInt(std::string pos, GLint value);
	void uniformVec3(std::string pos, glm::vec3 value);
	void uniformMat4(std::string pos, glm::mat4 value);

	virtual void enableAttribs();

protected:
	void readTextFromFile( const char* filename, char* &output );
	void printLog( GLuint handle );
	GLuint compileShader( const char* filename, GLenum shaderType );
	GLuint compileShaderText(const char* shaderString, GLenum shaderType);

	GLuint programHandle;
};

#endif
