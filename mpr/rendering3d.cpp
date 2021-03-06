#include "rendering3d.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <exception>
#include <vtkPlaneSource.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <array>
using namespace std;

void teste_opengl()
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::stringstream ss;
		ss << "erro de opengl " << err;
		throw std::exception(ss.str().c_str());
	}
}

std::string Shader::ReadShaderFile(std::string path)
{
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(path.c_str(), std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else
	{
		throw std::exception("arquivo n�o foi aberto");
	}
	std::cout << VertexShaderCode << std::endl;
	return VertexShaderCode;
}

void Shader::introspectProgram(GLuint programId, map<string, GLuint>& attributes, map<string, GLuint>& uniforms)
{
	GLint numberOfAttributes, largestAttributeName;
	glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &numberOfAttributes);
	glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &largestAttributeName);
	//para cada atributo, pegar as propriedades e guardar.
	for (GLint i = 0; i < numberOfAttributes; i++)
	{
		char* nameBuffer = new char[largestAttributeName];
		GLsizei length;
		GLint size;
		GLenum type;
		//Pega, entre outras coisas, o nome do atributo.
		glGetActiveAttrib(programId, i, largestAttributeName, &length, &size, &type, nameBuffer);
		GLint attribLocation = glGetAttribLocation(programId, nameBuffer);
		std::string _name(nameBuffer);
		attributes.insert(std::make_pair(_name, attribLocation));
		std::cout << _name << std::endl;
	}
	//Agora pra uniforms
	GLint numberOfUniforms, largesUniformName;
	glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &numberOfUniforms);
	glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &largesUniformName);
	//para cada atributo, pegar as propriedades e guardar.
	for (GLint i = 0; i < numberOfUniforms; i++)
	{
		char* buffer = new char[largesUniformName];
		GLsizei length;
		GLint size;
		GLenum type;
		glGetActiveUniform(programId, i, largesUniformName, &length,
			&size, &type, buffer);
		GLint unifLoc = glGetUniformLocation(programId, buffer);
		std::string _name(buffer);
		uniforms.insert(std::make_pair(_name, unifLoc));
		std::cout << _name << std::endl;
	}
}

string Shader::GetShaderInfoLog(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog)
{
	GLint log_length;
	char *log;
	glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
	log = static_cast<char*>(malloc(log_length));
	glGet__InfoLog(object, log_length, nullptr, log);
	return log;
}

GLuint Shader::MakeShader(GLenum type, std::string source)
{
	GLint length[] = { source.length() };
	const GLchar *srcLn = source.c_str();
	GLuint shader;
	GLint shader_ok;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &srcLn, length);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
	if (!shader_ok)
	{
		stringstream ss;
		string _probl = GetShaderInfoLog(shader, glGetShaderiv, glGetShaderInfoLog);
		ss << "Erro em " << __FUNCTION__ << " : " << _probl;
		glDeleteShader(shader);
		throw std::runtime_error(ss.str().c_str());
	}
	else
	{
		return shader;
	}
}

GLuint Shader::MakeProgram(GLuint vertex_shader, GLuint fragment_shader)
{
	GLint program_ok;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
	if (!program_ok) {
		string _probl = GetShaderInfoLog(program, glGetProgramiv, glGetProgramInfoLog);
		glDeleteProgram(program);
		throw std::runtime_error(_probl.c_str());
	}
	return program;
}

Shader::Shader(string vsPath, string fsPath)
{
	std::string vsSrc = ReadShaderFile(vsPath);
	vsId = MakeShader(GL_VERTEX_SHADER, vsSrc);
	std::string fsSrc = ReadShaderFile(fsPath);
	fsId = MakeShader(GL_FRAGMENT_SHADER, fsSrc);
	programId = MakeProgram(vsId, fsId);
	introspectProgram(programId, attributes, uniforms);
}

Shader::~Shader()
{
	glDeleteShader(vsId);
	glDeleteShader(fsId);
	glDeleteShader(programId);
}

GLuint Shader::GetUniform(string name)
{
	return uniforms.at(name);
}

GLuint Shader::GetAttribute(string name)
{
	return attributes.at(name);
}

GLuint Shader::GetProgramId()
{
	return programId;
}

void Shader::UseProgram()
{
	glUseProgram(this->programId);
}
///-------------------------------------------------------------------------------------------------------------------
Object3d::Object3d(std::string vsfile, std::string fsfile, itk::Image<float, 3>::Pointer imagem) : shader(vsfile, fsfile)
{
	//Seta a normal inicial.
	planeNormal << 0, 1, 0;
	//Cria��o da parte 3d.
	glDisable(GL_CULL_FACE);
	this->image = imagem;
	//Cria��o da textura
	texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_3D, texture);
	glTexImage3D(GL_TEXTURE_3D,
				0,
				GL_R16F,
				imagem->GetLargestPossibleRegion().GetSize()[0],
				imagem->GetLargestPossibleRegion().GetSize()[1],
				imagem->GetLargestPossibleRegion().GetSize()[2],
				0,
				GL_RED,
				GL_FLOAT,
				imagem->GetBufferPointer());
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);



	vertexes.push_back(-1.0f); vertexes.push_back(-1.0f); vertexes.push_back(0.0f);
	vertexes.push_back(1.0f); vertexes.push_back(-1.0f); vertexes.push_back(0.0f);
	vertexes.push_back(-1.0f); vertexes.push_back(1.0f); vertexes.push_back(0.0f);
	vertexes.push_back(1.0f); vertexes.push_back(1.0f); vertexes.push_back(0.0f);
	//cores triangulo 1
	colors.push_back(1.0f); colors.push_back(0.0f); colors.push_back(0.0f);
	colors.push_back(0.0f); colors.push_back(1.0f); colors.push_back(0.0f);
	colors.push_back(0.0f); colors.push_back(0.0f); colors.push_back(1.0f);
	colors.push_back(0.0f); colors.push_back(0.1f); colors.push_back(0.0f);
	//TexCoord triangulo 1
	texCoords.push_back(0.0f); texCoords.push_back(0.0f); texCoords.push_back(0.0f);//vert 0,0
	texCoords.push_back(0.0f); texCoords.push_back(1.0f); texCoords.push_back(0.0f);//vert 1.0
	texCoords.push_back(1.0f); texCoords.push_back(0.0f); texCoords.push_back(0.0f);//vert 0,1
	texCoords.push_back(1.0f); texCoords.push_back(1.0f); texCoords.push_back(0.0f);//vert 1,1
	vertexesVbo = 0;//Cria o buffer dos vertices e passa os dados pra ele.
	glGenBuffers(1, &vertexesVbo);
	glBindBuffer(GL_ARRAY_BUFFER, vertexesVbo);
	glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(float), vertexes.data(), GL_STATIC_DRAW);
	colorsVbo = 0;
	glGenBuffers(1, &colorsVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorsVbo);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);
	texVbo = 0;
	glGenBuffers(1, &texVbo);
	glBindBuffer(GL_ARRAY_BUFFER, texVbo);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), texCoords.data(), GL_STATIC_DRAW);
	vao = 0;//Cria o vertex array object e liga o buffer a ele
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	shader.UseProgram();
	GLuint vpLocation = shader.GetAttribute("vp");
	GLuint vcLocation = shader.GetAttribute("vc");
	GLuint uvLocation = shader.GetAttribute("uv");
	glEnableVertexAttribArray(vpLocation);
	glEnableVertexAttribArray(vcLocation);
	glEnableVertexAttribArray(uvLocation);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexesVbo);
	glVertexAttribPointer(vpLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colorsVbo);
	glVertexAttribPointer(vcLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, texVbo);
	glVertexAttribPointer(uvLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);

}

void Object3d::Render()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < 50; i++)
	{
		shader.UseProgram();
		glBindVertexArray(vao);
		GLuint vpLocation = shader.GetAttribute("vp");
		GLuint vcLocation = shader.GetAttribute("vc");
		GLuint uvLocation = shader.GetAttribute("uv");
		GLuint textureSamplerLocation = shader.GetUniform("myTextureSampler");
		GLuint useTextureLocation = shader.GetUniform("useTexture");
		GLuint windowLevelLocation = shader.GetUniform("windowLevel");
		GLuint windowWidthLocation = shader.GetUniform("windowWidth");

		glUniform1i(useTextureLocation, true);//Flag de controle no shader
		glUniform1f(windowLevelLocation, 300);
		glUniform1f(windowWidthLocation, 1000);

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(textureSamplerLocation, 0);
		glBindTexture(GL_TEXTURE_3D, texture);

		glBindAttribLocation(shader.GetProgramId(), vpLocation, "vp");
		glBindAttribLocation(shader.GetProgramId(), vcLocation, "vc");
		glBindAttribLocation(shader.GetProgramId(), uvLocation, "uv");

		//recalcula a tex coordinate
		vtkSmartPointer<vtkPlaneSource> ps = vtkSmartPointer<vtkPlaneSource>::New();
		//ps->SetNormal(0, 0.5, 0.866);
		ps->SetNormal(planeNormal[0], planeNormal[1], planeNormal[2]);
		ps->SetCenter(0.5, 0.5, 0.5);
		ps->Push(i*0.0099);
		ps->Update();
		vtkSmartPointer<vtkPolyData> resultingPlane = ps->GetOutput();
		array<array<double, 3>, 4> pts;
		resultingPlane->GetPoint(0, pts[0].data());
		resultingPlane->GetPoint(1, pts[1].data());
		resultingPlane->GetPoint(2, pts[2].data());
		resultingPlane->GetPoint(3, pts[3].data());
		glBindBuffer(GL_ARRAY_BUFFER, texVbo);
		texCoords.clear();
		for (array<double, 3> p : pts)
		{
			texCoords.push_back(p[0]); texCoords.push_back(p[1]); texCoords.push_back(p[2]);
		}
		glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), texCoords.data(), GL_STATIC_DRAW);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	}
}

