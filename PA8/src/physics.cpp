#include "physics.h"

Physics::Physics()
{

}

Physics::~Physics()
{
  dynamicsWorld->removeRigidBody(fallRigidBody);
  delete fallRigidBody->getMotionState();
  delete fallRigidBody;

  dynamicsWorld->removeRigidBody(groundRigidBody);
  delete groundRigidBody->getMotionState();
  delete groundRigidBody;

  delete dynamicsWorld;
  delete solver;
  delete collisionConfiguration;
  delete dispatcher;
  delete broadphase;
}

bool Physics::Initialize()
{
   broadphase = new btDbvtBroadphase();

   collisionConfiguration = new btDefaultCollisionConfiguration();
   dispatcher = new btCollisionDispatcher(collisionConfiguration);

   solver = new btSequentialImpulseConstraintSolver;

   dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

   dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

   return true;
}

glm::mat4 Physics::Update(unsigned int dt)
{
  btScalar m[16];
  std::cout << std::endl << "Before: " << dynamicsWorld << std::endl;
  dynamicsWorld->stepSimulation(dt, 10);
  fallRigidBody->getMotionState()->getWorldTransform(trans);
  trans.getOpenGLMatrix(m);

  std::cout << "sphere height: " << trans.getOrigin().getY() << std::endl;

  return glm::make_mat4(m);
}

void Physics::addGroundPlane(btCollisionShape* groundShape)
{
  btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
  btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
  groundRigidBody = new btRigidBody(groundRigidBodyCI);
  dynamicsWorld->addRigidBody(groundRigidBody);
}

void Physics::addObject(btCollisionShape* shape, btDefaultMotionState* motionState)
{
  btScalar mass = 1;
  btVector3 fallInertia(0, 0, 0);
  shape->calculateLocalInertia(mass, fallInertia);
  btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, motionState, shape, fallInertia);
  fallRigidBody = new btRigidBody(fallRigidBodyCI);
  dynamicsWorld->addRigidBody(fallRigidBody);
}