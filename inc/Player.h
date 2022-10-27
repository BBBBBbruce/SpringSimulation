#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include <glew.h>
#include <SDL_opengl.h>

#include "pathtools.h"
#include "lodepng.h"
#include "Matrices.h"
#include "Tool.h"



using namespace std;

struct Vertex {
	// position
	Vector3 Position;
	// normal
	Vector3 Normal;
	// texCoords
	Vector2 TexCoords;
	// tangent
	Vector3 Tangent;
	// bitangent
	Vector3 Bitangent;
};

struct Texture {
	unsigned int id;
	int type;
	string path;
};


class Player
{
private:
	string debugName;
	string ShaderPath;


protected:

	GLuint VBO;
	GLuint EBO;
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;


	int verticesCount;
	GLuint ShaderProgramID;
	GLuint VAO;
	GLuint MatrixLocation;
	Matrix4 TransformMatrix;

	string texturePath;


public:
	Player();
	Player(string name);
	Player(string name, string path);
	~Player();
	void PInit();
	virtual void DefineVertices(string path = "no path") = 0;
	virtual void setup() = 0;
	virtual void Render(Matrix4 projectionview) = 0;
	void tranlate(float x, float y, float z);
	void scale(float sx, float sy, float sz);
	void rotate(float angle, Vector3 axis );
	Matrix4 Get_TransformMatrix();
	
	void setTextureMap(string path);

// shaders
public:
	void setMat4_Shader(const std::string& name, const Matrix4& mat) const;
	void setVec3_Shader(const std::string& name, const Vector3& vec) const;

private:
	bool CreateShader();
	void LoadShader(string& Vert, string& Frag);
	
	

protected:
	virtual bool bindtexture() = 0;

};


#endif // !PLAYER_h



