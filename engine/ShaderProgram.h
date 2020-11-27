
#ifndef SHADER_PROGRAM_CLASS
#define SHADER_PROGRAM_CLASS

#include <GL/glew.h>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstring>
#include <glm/glm.hpp>

// Base class of shader program. Can be used to create a shader from files with shader code,
// or can be extended to implement other shaders
class ShaderProgram {
public:
	// Create a new program. Failing to pass in an argument creates an invalid program that will not run, but can still exist and be attached to things.
	ShaderProgram();
	ShaderProgram(std::string vertexShader, std::string fragmentShader);
	ShaderProgram(std::string vertexShader, std::string geometryShader, std::string fragmentShader);
	ShaderProgram(std::string vertexShader, std::string tessControlShader, std::string tessEvalShader, std::string fragmentShader);
	ShaderProgram(std::string vertexShader, std::string tessControlShader, std::string tessEvalShader, std::string geometryShader, std::string fragmentShader);
	virtual ~ShaderProgram();

	// Get the locations of uniforms and atrributes by name
	GLuint getUniformLocation(std::string name);
	GLuint getAttributeLocation(std::string name);

	// Start using this shader program for draw calls
	void useProgram();

	// Send the given data type to the uniform of the given name
	void uniformFloat(std::string pos, GLfloat value);
	void uniformInt(std::string pos, GLint value);
	void uniformVec3(std::string pos, glm::vec3 value);
	void uniformMat4(std::string pos, glm::mat4 value);

	// Enable shader attributes
	virtual void enableAttribs();

protected:
	// Helpers for compiling shaders
	void readTextFromFile( const char* filename, char* &output );
	void printLog( GLuint handle );
	GLuint compileShader( const char* filename, GLenum shaderType );
	GLuint compileShaderText(const char* shaderString, GLenum shaderType);

	// The handle of this shader program on the GPU
	GLuint programHandle;
};

#endif
