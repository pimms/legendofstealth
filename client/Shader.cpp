#include "Shader.h"

/*
================
Shader Public 
================
*/
Shader::Shader(string fragfile, string vertfile)
	:	_vert(0),
		_frag(0),
		_prog(0)
{
	if (!LoadShader(fragfile, vertfile)) {
		throw std::runtime_error("Failed to load shader");
	}
}

Shader::~Shader()
{
	if (_vert) 
		glDeleteShader(_vert);	
	if (_frag) 
		glDeleteShader(_frag);	
	if (_prog) 
		glDeleteProgram(_prog);
}

void Shader::Enable()
{
	glUseProgram(_prog);
}

void Shader::Disable()
{
	glUseProgram(0);
}

GLint Shader::GetUniformLocation(string uniform)
{
	if (!_uniformID.count(uniform)) {
		GLint loc = glGetUniformLocation(_prog, uniform.c_str());
		_uniformID[uniform] = loc;
	}

	return _uniformID[uniform];
}

void Shader::SetUniform1i(string n, GLint a) 
{
	GLint arr[] = {a};
	SetUniformXi(n, 1, arr);
}

void Shader::SetUniform2i(string n, GLint a, GLint b) 
{
	GLint arr[] = {a,b};
	SetUniformXi(n, 2, arr);
}

void Shader::SetUniform3i(string n, GLint a, GLint b, GLint c) 
{
	GLint arr[] = {a,b,c};
	SetUniformXi(n,3,arr);
}

void Shader::SetUniform4i(string n, GLint a, GLint b, GLint c, GLint d) 
{
	GLint arr[] = {a,b,c,d};
	SetUniformXi(n,4,arr);
}

void Shader::SetUniform1f(string n, GLfloat a) 
{
	GLfloat arr[] = {a};
	SetUniformXf(n,1,arr);
}

void Shader::SetUniform2f(string n, GLfloat a, GLfloat b) 
{
	GLfloat arr[] = {a,b};
	SetUniformXf(n,2,arr);
}


void Shader::SetUniform3f(string n, GLfloat a, GLfloat b, GLfloat c) 
{
	GLfloat arr[] = {a,b,c};
	SetUniformXf(n,3,arr);
}


void Shader::SetUniform4f(string n, GLfloat a, GLfloat b, GLfloat c, GLfloat d) 
{
	GLfloat arr[] = {a,b,c,d};
	SetUniformXf(n,4,arr);
}


/*
================
Shader Private
================
*/
bool Shader::LoadShader(string fragfile, string vertfile)
{
	_frag = glCreateShader(GL_FRAGMENT_SHADER);
	_vert = glCreateShader(GL_VERTEX_SHADER);
	if (!_frag || !_vert) 
		return false;

	if (!CompileShader(_frag, fragfile) || !CompileShader(_vert, vertfile)) 
		return false;

	_prog = glCreateProgram();
	if (!_prog)
		return false;

	glAttachShader(_prog, _vert);
	glAttachShader(_prog, _frag);

	if (!LinkProgram())
		return false;

	return true;
}

bool Shader::CompileShader(GLuint &shader, string sourcefile) 
{
	string source = GetShaderSource(sourcefile);

	const char *cSource = source.c_str();
	int sourceLength = source.length();

	glShaderSource(shader, 1, &cSource, &sourceLength);
	glCompileShader(shader);

	// Check for errors
	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
		// Report errors
		GLint logSize = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

		char *errLog = new char[logSize];
		glGetShaderInfoLog(shader, logSize, &logSize, errLog);

		printf("Failed to compile shader:\n%s\n", errLog);
		delete[] errLog;

		glDeleteShader(shader);
		shader = 0;

		return false;
	}

	return true;
}

bool Shader::LinkProgram() 
{
	if (!_prog) {
		return false;
	}

	glLinkProgram(_prog);

	GLint linkStatus = 0;
	glGetProgramiv(_prog, GL_LINK_STATUS, &linkStatus);

	if (linkStatus == GL_FALSE) {
		GLint logSize = 0;
		glGetProgramiv(_prog, GL_INFO_LOG_LENGTH, &logSize);

		char *errLog = new char[logSize];
		glGetProgramInfoLog(_prog, logSize, &logSize, errLog);

		printf("Failed to link program:\n%s\n", errLog);
		delete[] errLog;

		glDeleteShader(_frag);
		glDeleteShader(_vert);
		glDeleteProgram(_prog);
		_prog = 0;
		_frag = 0;
		_vert = 0;

		return false;
	}

	return true;
}

string Shader::GetShaderSource(string filename) 
{
	ifstream file;
	string source;

	file.open(filename);
	while (file) {
		string line;
		getline(file, line);
		source += line + "\n";
	}

	return source;
}


void Shader::SetUniformXi(string name, GLint len, GLint arr[]) 
{
	glUseProgram(_prog);

	switch (len) {
	case 1:
		glUniform1i(GetUniformLocation(name), arr[0]);
		break;
	case 2:
		glUniform2i(GetUniformLocation(name), arr[0], arr[1]);
		break;
	case 3:
		glUniform3i(GetUniformLocation(name), arr[0], arr[1], arr[2]);
		break;
	case 4:
		glUniform4i(GetUniformLocation(name), arr[0], arr[1], arr[2], arr[3]);
		break;
	}

	glUseProgram(0);
}

void Shader::SetUniformXf(string name, GLint len, GLfloat arr[]) 
{
	glUseProgram(_prog);

	switch (len) {
	case 1:
		glUniform1f(GetUniformLocation(name), arr[0]);
		break;
	case 2:
		glUniform2f(GetUniformLocation(name), arr[0], arr[1]);
		break;
	case 3:
		glUniform3f(GetUniformLocation(name), arr[0], arr[1], arr[2]);
		break;
	case 4:
		glUniform4f(GetUniformLocation(name), arr[0], arr[1], arr[2], arr[3]);
		break;
	}

	glUseProgram(0);
}
