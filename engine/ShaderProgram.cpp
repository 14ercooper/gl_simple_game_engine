
#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() {
	// Mark as invalid
	programHandle = -1;
}

ShaderProgram::ShaderProgram(std::string vertexShader, std::string fragmentShader) {
	GLuint vertex = compileShader(vertexShader.c_str(), GL_VERTEX_SHADER);
	GLuint fragment = compileShader(fragmentShader.c_str(), GL_FRAGMENT_SHADER);

	programHandle = glCreateProgram();

	glAttachShader(programHandle, vertex);
	glAttachShader(programHandle, fragment);

	glLinkProgram(programHandle);

	printLog(programHandle);

	glDetachShader(programHandle, vertex);
	glDetachShader(programHandle, fragment);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

ShaderProgram::ShaderProgram(std::string vertexShader, std::string geometryShader, std::string fragmentShader) {
	GLuint vertex = compileShader(vertexShader.c_str(), GL_VERTEX_SHADER);
	GLuint geometry = compileShader(geometryShader.c_str(), GL_GEOMETRY_SHADER);
	GLuint fragment = compileShader(fragmentShader.c_str(), GL_FRAGMENT_SHADER);

	programHandle = glCreateProgram();

	glAttachShader(programHandle, vertex);
	glAttachShader(programHandle, geometry);
	glAttachShader(programHandle, fragment);

	glLinkProgram(programHandle);

	printLog(programHandle);

	glDetachShader(programHandle, vertex);
	glDetachShader(programHandle, geometry);
	glDetachShader(programHandle, fragment);

	glDeleteShader(vertex);
	glDeleteShader(geometry);
	glDeleteShader(fragment);
}

ShaderProgram::~ShaderProgram() {
	if (programHandle == -1)
		return;
	glDeleteProgram(programHandle);
}

GLuint ShaderProgram::getUniformLocation(std::string name) {
	if (programHandle == -1)
		return -1;
	return glGetUniformLocation(programHandle, name.c_str());
}

GLuint ShaderProgram::getAttributeLocation(std::string name) {
	if (programHandle == -1)
		return -1;
	return glGetAttribLocation(programHandle, name.c_str());
}

void ShaderProgram::useProgram() {
	if (programHandle == -1)
		return;
	glUseProgram(programHandle);
}

void ShaderProgram::uniformInt(std::string pos, GLint value) {
	useProgram();
	glUniform1iv(getUniformLocation(pos), 1, &value);
}

void ShaderProgram::uniformFloat(std::string pos, GLfloat value) {
	useProgram();
	glUniform1fv(getUniformLocation(pos), 1, &value);
}

void ShaderProgram::uniformVec3(std::string pos, glm::vec3 value) {
	useProgram();
	glUniform3fv(getUniformLocation(pos), 1, &value[0]);
}

void ShaderProgram::uniformMat4(std::string pos, glm::mat4 value) {
	useProgram();
	glUniformMatrix4fv(getUniformLocation(pos), 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::enableAttribs() {
	// Do nothing
}

void ShaderProgram::readTextFromFile( const char* filename, char* &output ){
	std::string buf = std::string("");
	std::string line;

	std::ifstream in( filename );
	while( getline(in, line) ) {
		buf += line + "\n";
	}
	output = new char[buf.length()+1];
	strncpy(output, buf.c_str(), buf.length());
	output[buf.length()] = '\0';

	in.close();
}

void ShaderProgram::printLog( GLuint handle ) {
	int infoLogLength = 0;
	int maxLength;

	// check if the handle is to a shader
	bool isShader;
	if( glIsShader( handle ) ) {
		glGetShaderiv(  handle, GL_INFO_LOG_LENGTH, &maxLength );
		isShader = true;
	}
	// check if the handle is to a shader program
	else {
		glGetProgramiv( handle, GL_INFO_LOG_LENGTH, &maxLength );
		isShader = false;
	}

	// create a buffer of designated length
	char infoLog[maxLength];


	if( isShader ) {
		// get the info log for the shader
		glGetShaderInfoLog( handle, maxLength, &infoLogLength, infoLog );
	} else {
		// get the info log for the shader program
		glGetProgramInfoLog( handle, maxLength, &infoLogLength, infoLog );
	}

	// if the length of the log is greater than 0
	if( infoLogLength > 0 ) {
		// print info to terminal
		printf( "[INFO]: %s Handle %d: %s\n", (isShader ? "Shader" : "Program"), handle, infoLog );
	}
}

GLuint ShaderProgram::compileShader( const char* filename, GLenum shaderType ) {
	char *shaderString;

	// read in each text file and store the contents in a string
	readTextFromFile( filename, shaderString );

	// create a handle for our shader of the corresponding type
	
	GLuint shaderHandle = glCreateShader(shaderType);
	if (shaderHandle == 0)
		fprintf(stderr, "Cannot get handle for shader\n");

	// send the contents of each program to the GPU
	glShaderSource(shaderHandle, 1, (const char**)&shaderString, NULL);

	// we are good programmers so free up the memory used by each buffer
	delete [] shaderString;

	// compile each shader on the GPU
	glCompileShader(shaderHandle);

	// print the log for this shader handle to verify it compiled correctly
	printLog( shaderHandle );

	return shaderHandle;
}

GLuint ShaderProgram::compileShaderText( const char* shaderString, GLenum shaderType ) {
	GLuint shaderHandle = glCreateShader(shaderType);
	if (shaderHandle == 0)
		fprintf(stderr, "Cannot get handle for shader\n");

	// send the contents of each program to the GPU
	glShaderSource(shaderHandle, 1, (const char**)&shaderString, NULL);

	// compile each shader on the GPU
	glCompileShader(shaderHandle);

	// print the log for this shader handle to verify it compiled correctly
	printLog( shaderHandle );

	return shaderHandle;
}
