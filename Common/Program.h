#pragma once

/**
	@class	Program
	@date	2018/3/28
	@author Kim Minkyu(snow_chris@naver.com)
	@brief	Shader Program
 */

#include <GL/glew.h>
#pragma comment(lib, "glew32.lib")
#include <string>

class Program
{
public :
	Program(const char* vertexFileName, const char* fragmentFileName);
	~Program();

	void use();

private :
	GLuint vertexShaderID;
	GLuint fragmentShaderID;
	GLuint programID;

private :
	/**
		@return : loaded code 
		@exception : if there is no file
	 */
	std::string load(const char* fileName);

	/**
		@brief : compile shader 
		@exception : compile error message
	*/
	void compile(GLuint shaderID, const std::string& code);

	void link();
};