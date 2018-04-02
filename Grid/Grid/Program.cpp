#include "Program.h"

#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

Program::Program(const char * vertexFileName, const char * fragmentFileName)
{
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	try {
		compile(vertexShaderID, load(vertexFileName));
		compile(fragmentShaderID, load(fragmentFileName));
		link();
	}
	catch (const exception& e) {
		throw exception(e.what());
	}	
}

Program::~Program()
{
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

void Program::use()
{
	glUseProgram(programID);
}

GLuint Program::getUniformLocation(const char * name)
{
	return glGetUniformLocation(programID, name);
}

string Program::load(const char * fileName)
{
	ifstream shaderStream(fileName, ios::in);
	if (shaderStream.fail()) {
		stringstream ss;
		ss << fileName << " doesn't exist.";
		throw exception(ss.str().c_str());
	}

	stringstream ss;
	ss << shaderStream.rdbuf();

	return ss.str();
}

void Program::compile(GLuint shaderID, const std::string& code)
{
	char const* source = code.c_str();
	glShaderSource(shaderID, 1, &source, nullptr);
	glCompileShader(shaderID);

	register GLint result = GL_FALSE;
	register int infoLogLength;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0) {
		vector<char> shaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(shaderID, infoLogLength, nullptr, &shaderErrorMessage[0]);
		throw exception(&shaderErrorMessage[0]);
	}
}

void Program::link()
{
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	register GLint result;
	register int infoLogLength;
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0) {
		vector<char> programErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, nullptr, &programErrorMessage[0]);
		throw exception(&programErrorMessage[0]);
	}
}
