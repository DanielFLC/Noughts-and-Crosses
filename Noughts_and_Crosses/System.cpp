#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include "System.h"

System::System(void)
{
	renewParticle = true;
}

System::~System(void)
{
}

/*
 * initalizes a single particle according to its type
 */
void System::createParticle(Particle *p)
{
	p->lifespan = (((rand()%125+1)/10.0f));
	p->age = 0.0f;
    p->scale = 0.25f;
    p->direction = 0;
   
	//Posicao inicial
    p->position[X] = 0;//((rand()%1)-(rand()%1));
	p->position[Y] = 0;
	p->position[Z] = 0;//((rand()%1)-(rand()%1));

	//Altera o movimento (se afasta mais ou nao, etc...)
    p->movement[X] = (((((((2) * rand()%8) + 1)) * rand()%8) + 1) * 0.0035) - (((((((2) * rand()%8) + 1)) * rand()%8) + 1) * 0.0035);
    p->movement[Y] = ((((((5) * rand()%8) + 2)) * rand()%8) + 5) * 0.015; 
    p->movement[Z] = (((((((2) * rand()%8) + 1)) * rand()%8) + 1) * 0.0015) - (((((((2) * rand()%8) + 1)) * rand()%8) + 1) * 0.0015);
 
    //Cor da particula:
    p->color[X] = 0.4f;
    p->color[Y] = 0.4f;
    p->color[Z] = 0.4f;

	//Vento:
    p->pull[X] = 0.0f;   //-0.25f (quando abrimos a janela)
    p->pull[Y] = 0.0f;
    p->pull[Z] = 0.0f;
}

/*
 * initalizes a particle system
 * calls create particle() to initalize individual particles
 */
void System::createParticles(void)
{
   systemPull[X] = systemPull[Y] = systemPull[Z] = 0.0f;
   int i = 0;
   for(i; i < MAX_PARTICLES; i++)
   {
      createParticle(&particles[i]);
   }
}

/*
 * updates required particle attributes for all particles in a system
 * also responsible for killing and respawning (via createparticle()) individual particles
 */
void System::updateParticles(void)
{
   int i = 0;
   for(i; i < MAX_PARTICLES; i++)
   {
      particles[i].age = particles[i].age + 0.10;	//Altera a vida de cada particula (altura em Y neste caso)
      
      particles[i].scale = particles[i].scale + 0.005; //Aumenta o incremento de tamanho ao longo da vida da particula

      particles[i].direction = particles[i].direction + ((((((int)(0.5) * rand()%10) + 1)) * rand()%10) + 1);

      particles[i].position[X] = particles[i].position[X] + particles[i].movement[X] + particles[i].pull[X];
      particles[i].position[Y] = particles[i].position[Y] + particles[i].movement[Y] + particles[i].pull[Y];
      particles[i].position[Z] = particles[i].position[Z] + particles[i].movement[Z] + particles[i].pull[Z];
      
	  // Aceleracao de acordo com a gravidade de definida
      particles[i].pull[X] = particles[i].pull[X] + systemPull[X];
      particles[i].pull[Y] = particles[i].pull[Y] + systemPull[Y]; 
      particles[i].pull[Z] = particles[i].pull[Z] + systemPull[Z];
 
      if (renewParticle && (particles[i].age > particles[i].lifespan || particles[i].position[Y] > 45 || particles[i].position[Y] < -35 || particles[i].position[X] > 80 || particles[i].position[X] < -80))
		createParticle(&particles[i]);
   }
}

int System::getNumOfParticles(void)
{
   return MAX_PARTICLES;
}

float System::getXPos(int i)
{
   return particles[i].position[X];
}

float System::getYPos(int i)
{
   return particles[i].position[Y];
}
float System::getZPos(int i)
{
   return particles[i].position[Z];
}

float System::getR(int i)
{
   return particles[i].color[X];
}

float System::getG(int i)
{
   return particles[i].color[Y];
}
float System::getB(int i)
{
   return particles[i].color[Z];
}

float System::getScale(int i)
{
   return particles[i].scale;
}

float System::getDirection(int i)
{
   return particles[i].direction;
}

float System::getAlpha(int i)
{
   return (1 - particles[i].age/particles[i].lifespan);
}

void System::modifyRenewParticle(bool value)
{
   renewParticle = value;
}

void System::modifySystemPull(float x, float y, float z)
{
   systemPull[X] += x;
   systemPull[Y] += y;
   systemPull[Z] += z;
}
