#ifndef PHYSICS_H
#define PHYSICS_H

#include <iostream>

#include "graphics_headers.h"

class Physics
{
  public:
    Physics();
    ~Physics();
    bool Initialize();
    glm::mat4 Update(unsigned int);

    void addGroundPlane(btCollisionShape*);
    void addObject(btCollisionShape*, btDefaultMotionState*);

  private:
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;

    btTransform trans;

    btRigidBody* groundRigidBody;
    btRigidBody* fallRigidBody;

};

#endif /* PHYSICS_H */