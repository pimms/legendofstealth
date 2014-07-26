#pragma once

#ifdef _WIN32
	#include <Trutle.h>
#else
	#include <trutle/Trutle.h>
#endif

class Shader {
public:
	Shader(string fragfile, string vertfile); 
	~Shader();

	void Enable();
	void Disable();

	GLint GetUniformLocation(string uniform);
	void SetUniform1i(string n, GLint a);
	void SetUniform2i(string n, GLint a, GLint b);
	void SetUniform3i(string n, GLint a, GLint b, GLint c);
	void SetUniform4i(string n, GLint a, GLint b, GLint c, GLint d);
	void SetUniform1f(string n, GLfloat a);
	void SetUniform2f(string n, GLfloat a, GLfloat b);
	void SetUniform3f(string n, GLfloat a, GLfloat b, GLfloat c);
	void SetUniform4f(string n, GLfloat a, GLfloat b, GLfloat c, GLfloat d);

private:
	GLuint _vert;
	GLuint _frag;
	GLuint _prog;

	map<string, GLint> _uniformID;

	bool LoadShader(string fragfile, string vertfile);
	bool CompileShader(GLuint &shader, string sourcefile);
	bool LinkProgram();
	string GetShaderSource(string file);
	
	void SetUniformXi(string, GLint, GLint[]);
	void SetUniformXf(string, GLint, GLfloat[]);
};
