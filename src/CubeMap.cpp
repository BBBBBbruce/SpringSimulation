#include "CubeMap.h"

void CubeMap::DefineVertices(string path )
{
	if (path == "no path") {
		Vertices = {
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
}

bool CubeMap::bindtexture()
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
