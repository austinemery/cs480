#ifndef PHYSICS_H
#define PHYSICS_H

#include <iostream>
#include <vector>

#include "graphics_headers.h"

class Physics
{
  public:
    Physics();
    ~Physics();
    bool Initialize();
    glm::mat4 Update(unsigned int, int);

    void addGroundPlane(btCollisionShape*);
    void addObject(btCollisionShape*, btDefaultMotionState*, int);
    void updateCubeVelocity(float, float);

  private:
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;

    btTransform trans;

    btRigidBody* groundRigidBody;
    std::vector<btRigidBody*> physicsObjectVector;

};

#endif /* PHYSICS_H */