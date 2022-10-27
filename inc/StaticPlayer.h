// adopted from learnopengl.com
#ifndef STATICPLAYER_H
#define STATICPLAYER_H

#pragma once
#include "Player.h"
#include "Matrices.h"
#include "lodepng.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

enum TextureType { DiffuseMap, MetallicMap, NormalMap, RoughnessMap };

class StaticPlayer :
    public Player
{

public:
    using Player::Player;
    
    void DefineVertices(string path = "no path");
    void Render(Matrix4 projectionview);

protected:

    bool bindtexture();
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);
    Texture loadMaterialTextures(TextureType typeName);
    GLuint TextureFromFile(string path);


public:
    void setup();
};

#endif

