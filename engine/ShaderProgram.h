
#ifndef SHADER_PROGRAM_CLASS
#define SHADER_PROGRAM_CLASS

#include <GL/glew.h>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstring>

class ShaderProgram {
public:
	ShaderProgram();
	ShaderProgram(std::string vertexShader, std::string fragmentShader);
	ShaderProgram(std::string vertexShader, std::string geometryShader, std::string fragmentShader);
	~ShaderProgram();

	GLuint getUniformLocation(std::string name);
	GLuint getAttributeLocation(std::string name);

	void useProgram();

private:
	void readTextFromFile( const char* filename, char* &output );
	void printLog( GLuint handle );
	GLuint compileShader( const char* filename, GLenum shaderType );

	GLuint programHandle;
};

#endif
