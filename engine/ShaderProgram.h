
#ifndef SHADER_PROGRAM_CLASS
#define SHADER_PROGRAM_CLASS

#include <string>
#include <GL/glew.h>

class ShaderProgram {
public:
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
