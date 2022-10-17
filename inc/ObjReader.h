#ifndef OBJREADER_H
#define OBJREADER_H

#include <vector>

class ObjReader
{
    struct vertex {
        double x;
        double y;
        double z;
    };
    struct face {
        unsigned int v1, v2, v3;
    };
    std::vector<vertex> vetexes;
    std::vector<face> faces;

public:
    void readfile(const char* filename);
    void draw();

};


#endif // !OBJREADER_H


