#include "Player.h"


#include <fstream>
#include <sstream>
#include <iostream>



Player::Player()
{
}

Player::Player(string name)
	: ShaderProgramID(0)
	, VAO(0)
	, VBO(0)
	, MatrixLocation(-1)
	, cubemap(0)
{
	// invisable
	Vertices = NULL;
	debugName = name;

}

Player::Player(string name, string path)
	: ShaderProgramID(0)
	, VAO(0)
	, VBO(0)
	, MatrixLocation(-1)
	, cubemap(0)
{
	ShaderPath = path;
	Vertices = NULL;
	debugName = name;
}

Player::~Player()
{
	if (ShaderProgramID != 0)	glDeleteProgram(ShaderProgramID);
	if (VAO != 0)				glDeleteVertexArrays(1, &VAO);
	if (VBO != 0)				glDeleteBuffers(1, &VBO);

	delete Vertices;
}

void Player::PInit()
{
	if (!ShaderPath.empty()) {
		if(CreateShader()) cout<< "shader creation successful\n";
		if(bindtexture()) cout<<"texture binded\n";
	} 
	setup();
}

void Player::LoadVertices(string path)
{
	// loading .obj / .stl/ .usd etc.
}

void Player::DefineVertices()
{
		Vertices = new float[36*3] {
		// positions          
		-50.0f,  50.0f, -50.0f,
		-50.0f, -50.0f, -50.0f,
		 50.0f, -50.0f, -50.0f,
		 50.0f, -50.0f, -50.0f,
		 50.0f,  50.0f, -50.0f,
		-50.0f,  50.0f, -50.0f,
		-50.0f, -50.0f,  50.0f,
		-50.0f, -50.0f, -50.0f,
		-50.0f,  50.0f, -50.0f,
		-50.0f,  50.0f, -50.0f,
		-50.0f,  50.0f,  50.0f,
		-50.0f, -50.0f,  50.0f,
		 50.0f, -50.0f, -50.0f,
		 50.0f, -50.0f,  50.0f,
		 50.0f,  50.0f,  50.0f,
		 50.0f,  50.0f,  50.0f,
		 50.0f,  50.0f, -50.0f,
		 50.0f, -50.0f, -50.0f,
		-50.0f, -50.0f,  50.0f,
		-50.0f,  50.0f,  50.0f,
		 50.0f,  50.0f,  50.0f,
		 50.0f,  50.0f,  50.0f,
		 50.0f, -50.0f,  50.0f,
		-50.0f, -50.0f,  50.0f,
		-50.0f,  50.0f, -50.0f,
		 50.0f,  50.0f, -50.0f,
		 50.0f,  50.0f,  50.0f,
		 50.0f,  50.0f,  50.0f,
		-50.0f,  50.0f,  50.0f,
		-50.0f,  50.0f, -50.0f,
		-50.0f, -50.0f, -50.0f,
		-50.0f, -50.0f,  50.0f,
		 50.0f, -50.0f, -50.0f,
		 50.0f, -50.0f, -50.0f,
		-50.0f, -50.0f,  50.0f,
		 50.0f, -50.0f,  50.0f
	};

}

void Player::Render(Matrix4 projectionview)
{
	glDepthFunc(GL_LEQUAL);
	glUseProgram(ShaderProgramID);
	glUniformMatrix4fv(MatrixLocation, 1, GL_FALSE, projectionview.get());
	glActiveTexture(GL_TEXTURE0);
	//glUniformMatrix4fv(m_nSkyBoxLocation, 1, GL_FALSE, GetCurrentViewProjectionMatrix(nEye).get());
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

bool Player::CreateShader()
{
	// load shader
	string ShaderVert;
	string ShaderFrag;
	LoadShader(ShaderVert, ShaderFrag);
	//cout << ShaderFrag << endl;

	//ShaderProgramID = CompileGLShader(debugName.c_str(), ShaderVert.c_str(), ShaderFrag.c_str());

	ShaderProgramID = CompileGLShader(
		"skybox",
		// VS
		"#version 410\n"
		"layout(location = 0) in vec3 aPos;\n"
		"out vec3 TexCoords;\n"
		"uniform mat4 ViewProjection;"
		"void main()\n"
		"{\n"
		"TexCoords = aPos;\n"
		"vec4 pos = ViewProjection * vec4(aPos, 1.0);\n"
		"gl_Position = pos.xyww;\n"
		"}\n",
		//FS
		"#version 410\n"
		"out vec4 FragColor;\n"
		"in vec3 TexCoords;\n"
		"uniform samplerCube skybox;\n"
		"void main()\n"
		"{\n"
		"	FragColor = texture(skybox, TexCoords);\n"
		"}\n"
	);

	MatrixLocation = glGetUniformLocation(ShaderProgramID, "ViewProjection");
	//if (m_unSkyBoxProgramID == 1) std::cout << "created skybox shader" << std::endl;
	if (MatrixLocation == -1)
	{
		dprintf("Unable to find viewprojection matrix in %s shader\n", debugName.c_str());
		return false;
	}
	return true;
}

void Player::LoadShader(string& Vert, string& Frag)
{

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// ShaderPath = "../../Shaders/",
	// debugName = skybox

	string vertpath;
	string fragpath;
	string sExecutableDirectory = Path_StripFilename(Path_GetExecutablePath());

	vertpath = Path_MakeAbsolute(ShaderPath+debugName+".vert", sExecutableDirectory);
	fragpath = Path_MakeAbsolute(ShaderPath + debugName + ".frag", sExecutableDirectory);
	cout << "vert path: " << vertpath << endl;
	cout << "frag path: " << fragpath << endl;


	try
	{
		// open files
		vShaderFile.open(vertpath);
		fShaderFile.open(fragpath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		Vert = vShaderStream.str();
		Frag = fShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		dprintf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: %c \n", e.what());
	}
}

void Player::setup()
{
	//DefineVertices();

	float Vertices[] = {
		// positions          
		-50.0f,  50.0f, -50.0f,
		-50.0f, -50.0f, -50.0f,
		 50.0f, -50.0f, -50.0f,
		 50.0f, -50.0f, -50.0f,
		 50.0f,  50.0f, -50.0f,
		-50.0f,  50.0f, -50.0f,
		-50.0f, -50.0f,  50.0f,
		-50.0f, -50.0f, -50.0f,
		-50.0f,  50.0f, -50.0f,
		-50.0f,  50.0f, -50.0f,
		-50.0f,  50.0f,  50.0f,
		-50.0f, -50.0f,  50.0f,
		 50.0f, -50.0f, -50.0f,
		 50.0f, -50.0f,  50.0f,
		 50.0f,  50.0f,  50.0f,
		 50.0f,  50.0f,  50.0f,
		 50.0f,  50.0f, -50.0f,
		 50.0f, -50.0f, -50.0f,
		-50.0f, -50.0f,  50.0f,
		-50.0f,  50.0f,  50.0f,
		 50.0f,  50.0f,  50.0f,
		 50.0f,  50.0f,  50.0f,
		 50.0f, -50.0f,  50.0f,
		-50.0f, -50.0f,  50.0f,
		-50.0f,  50.0f, -50.0f,
		 50.0f,  50.0f, -50.0f,
		 50.0f,  50.0f,  50.0f,
		 50.0f,  50.0f,  50.0f,
		-50.0f,  50.0f,  50.0f,
		-50.0f,  50.0f, -50.0f,
		-50.0f, -50.0f, -50.0f,
		-50.0f, -50.0f,  50.0f,
		 50.0f, -50.0f, -50.0f,
		 50.0f, -50.0f, -50.0f,
		-50.0f, -50.0f,  50.0f,
		 50.0f, -50.0f,  50.0f
	};


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices, GL_STATIC_DRAW);

	GLsizei stride = sizeof(Vector3);// if texcoord needed change to VertexDataScene
	uintptr_t offset = 0;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (const void*)offset);

	glBindVertexArray(0);
}

bool Player::bindtexture()
{
	std::vector<std::string> faces
	{
		"../../assets/Texture/skybox/right.png",
		"../../assets/Texture/skybox/left.png",
		"../../assets/Texture/skybox/top.png",
		"../../assets/Texture/skybox/bottom.png",
		"../../assets/Texture/skybox/front.png",
		"../../assets/Texture/skybox/back.png"
	};

	glGenTextures(1, &cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

	std::string strFullPath;
	std::string sExecutableDirectory = Path_StripFilename(Path_GetExecutablePath());

	std::vector<unsigned char> imageRGBA;
	unsigned nImageWidth, nImageHeight;

	for (GLuint i = 0; i < faces.size(); i++)
	{
		strFullPath = Path_MakeAbsolute(faces[i], sExecutableDirectory);
		std::cout << "cube map path: " << strFullPath << std::endl;
		unsigned nError = lodepng::decode(imageRGBA, nImageWidth, nImageHeight, strFullPath.c_str());

		if (nError != 0)
			return false;

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, nImageWidth, nImageHeight,
			0, GL_RGBA, GL_UNSIGNED_BYTE, &imageRGBA[0]);
		imageRGBA.clear();

	}
	//glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return (cubemap != 0);
}
