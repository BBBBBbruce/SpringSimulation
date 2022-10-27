#ifndef CUBEMAP_H
#define CUBEMAP_H
#pragma once

#include "Player.h"
class CubeMap :
    public Player

{
public: 
    using Player::Player;
    void DefineVertices(string path = "no path");
    void Render(Matrix4 projectionview);
    void setup();

protected:
    vector<float> Vertices;
    bool bindtexture();
    GLuint diffuse;

};
#endif

