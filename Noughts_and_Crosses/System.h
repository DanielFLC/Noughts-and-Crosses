#pragma once

#define MAX_PARTICLES 30

typedef float fVector[3];


enum dimensions{X,Y,Z};

struct Particle
{
   float lifespan, age, scale, direction; // how long the particle will exist for, alpha blending variable; how old is it.
   fVector position; // initial onscreen position
   fVector movement; // movement vector
   fVector color; // color values
   fVector pull; // compounding directional pull in the x,y,z directions
}; 

class System
{
   bool	renewParticle;  //Variavel responsavel por dizer se queremos renovar as particulas ou nao.
   fVector systemPull; //used to store global compounding system pull in x,y,z
   Particle particles[MAX_PARTICLES]; //initalizes and array of type Particle
public:
   System(void);
   virtual ~System(void);
   void createParticle(Particle *p); //creates and initalizes a single particle
   void createParticles(); //calls createParticle() to initalize all particles in system
   void updateParticles(); //updates particles according to forces being used
   int getNumOfParticles(); // returns the number of particles in the system (legacy)
   float getXPos(int i); //returns x position of particle i
   float getYPos(int i); //returns y position of particle i
   float getZPos(int i); //returns z position of particle i
   float getR(int i); //returns red component of particle i
   float getG(int i); //returns green component of particle i
   float getB(int i); //returns blue component of particle i
   float getScale(int i); //returns scale of particle
   float getDirection(int i); //returns direction of particle for texture rotation
   float getAlpha(int i); //returns how faded (according to age) the particle should be
   void modifyRenewParticle(bool value);//Funcao para alterar o valor da variavel renewParticle.
   void modifySystemPull(float x, float y, float z); //used to modify x,y,z pull magnitudes
};
