#ifndef PHYSICSRENDERER_H
#define PHYSICSRENDERER_H

#include <ctype.h>
#include "PxPhysicsAPI.h"


using namespace physx;

PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity = PxVec3(0));

void createStack(const PxTransform& t, PxU32 size, PxReal halfExtent);

void initPhysics(bool /*interactive*/);

void stepPhysics(bool /*interactive*/);

void cleanupPhysics(bool /*interactive*/);

void keyPress(const char key, const PxTransform& camera);

#endif