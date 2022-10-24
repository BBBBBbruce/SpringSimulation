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


class Player
{
private:
	string debugName;
	string ShaderPath;
	GLuint ShaderProgramID;
	GLuint VAO;
	GLuint VBO;
	GLuint MatrixLocation;

protected:
	GLuint cubemap;
	vector<float> Vertices;


public:
	Player();
	Player(string name);
	Player(string name, string path);
	~Player();
	void PInit();
	virtual void DefineVertices(string path = "no path") = 0;
	void Render(Matrix4 projectionview);

private:
	bool CreateShader();
	void LoadShader(string& Vert, string& Frag);
	void setup();
	

protected:
	virtual bool bindtexture() = 0;

};


#endif // !PLAYER_h



