#ifndef MESH_H
#define MESH_H
#pragma once

#include "Matrices.h"
#include <string>
#include <vector>

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
    string type;
    string path;
};


class mesh {
public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    //unsigned int VAO;

    // constructor
    mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

private:
    // render data 
    //unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
    //void setupMesh();
};

#endif

