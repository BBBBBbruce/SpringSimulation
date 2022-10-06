#ifndef HOOKESPRING_H
#define HOOKESPRING_H

class Spring {
private:
	float SpringConstant;
	float OriginalLength;
	float MaxForce;
	float Currentlength;
	bool Compressed; // not supporting stretching case;
	float ExternalForce;
	bool plastic;
	void updatestate();
	
public:
	Spring();
	Spring(float sc, float length, float maxforce);
	~Spring();
	float get_springConstant();
	float get_OriginalLength();
	float get_CurrentLength();
	float get_maxforce();
	float get_currentforce();
	void set_externalforce(float f);
	

};
#endif