#include "HookeSpring.h"
#include <iostream>

Spring::Spring()
{
    SpringConstant = 0;
    OriginalLength = 0;
    MaxForce = 0;
    Currentlength = 0;
    Compressed = false;
    ExternalForce = 0;
    plastic = true;

}

Spring::Spring(float sc, float length, float maxforce)
{
    SpringConstant = sc;
    OriginalLength = length;
    MaxForce = maxforce;
    Currentlength = OriginalLength;
    Compressed = false;
    ExternalForce = 0;
    plastic = false;
  
}

Spring::~Spring()
{
}

float Spring::get_springConstant()
{
    return SpringConstant;
}

float Spring::get_length()
{
    return OriginalLength;
}

float Spring::get_maxforce()
{
    return MaxForce;
}

void Spring::set_externalforce(float f)
{
    if (!plastic) {
        ExternalForce = f;
        Compressed = bool(f);// check if f == 0;
        updatestate();
    }
    else {
        std::cout << "the spring is broken now \n";
    }
    
}

void Spring::updatestate()
{
    if (ExternalForce < MaxForce) {
        float internalforce = SpringConstant * (OriginalLength - Currentlength);
        Currentlength = (ExternalForce - internalforce) / SpringConstant;
    }
    else {
        Currentlength = MaxForce / SpringConstant;
        plastic = true;
    }
}
