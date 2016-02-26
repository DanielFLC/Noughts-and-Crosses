#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include "RgbImage.h"
#include "SkyBox.h"


void SkyBox::Load_Texture(GLint textureArrayPosition, 	char* path) {
	glGenTextures(1, &skyBoxTextures[textureArrayPosition]);  //Generate texture names
	glBindTexture(GL_TEXTURE_2D, skyBoxTextures[textureArrayPosition]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//Filtragem linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//Filtragem linear mipmap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	imag.LoadBmpFile(path);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

}

void SkyBox::Init(int mode, GLfloat size, GLfloat yyIncrement) {
	//Paths:
	//"MountainPath"
	//"Snow"

	yy = yyIncrement;//Server para mudarmos a posicao do y (subir ou descer a skybox)
	//Define o tamanho do mundo.
	worldSize = size;
	if(mode==1){		// MountainPath
		Load_Texture(0,"Textures/SkyBox/MountainPath/left.bmp"); //load da textura da face esquerda da skybox
		Load_Texture(1,"Textures/SkyBox/MountainPath/right.bmp"); //load da textura da face direita da skybox
		Load_Texture(2,"Textures/SkyBox/MountainPath/up.bmp"); //load da textura da face de cima da skybox
		Load_Texture(3,"Textures/SkyBox/MountainPath/down.bmp"); //load da textura da face de baixo da skybox
		Load_Texture(4,"Textures/SkyBox/MountainPath/front.bmp"); //load da textura da face frontal da skybox
		Load_Texture(5,"Textures/SkyBox/MountainPath/back.bmp"); //load da textura da face de tras da skybox
	}else{
		Load_Texture(0,"Textures/SkyBox/Snow/left.bmp"); //load da textura da face esquerda da skybox
		Load_Texture(1,"Textures/SkyBox/Snow/right.bmp"); //load da textura da face direita da skybox
		Load_Texture(2,"Textures/SkyBox/Snow/up.bmp"); //load da textura da face de cima da skybox
		Load_Texture(3,"Textures/SkyBox/Snow/down.bmp"); //load da textura da face de baixo da skybox
		Load_Texture(4,"Textures/SkyBox/Snow/front.bmp"); //load da textura da face frontal da skybox
		Load_Texture(5,"Textures/SkyBox/Snow/back.bmp"); //load da textura da face de tras da skybox
	}
}


//Desenha a skybox
void SkyBox::draw() {
	glPushMatrix();
		glColor3f(1.0f,1.0f,1.0f);			//Definir cor para branco
		glEnable(GL_TEXTURE_2D);			//Ligar texture mapping
		glDisable (GL_DEPTH_TEST);			//Desligar depth testing

		//Efectuar o Bind da textura
		glBindTexture(GL_TEXTURE_2D, skyBoxTextures[0]);
		//Face esquerda
		glBegin (GL_QUADS);	
			//É necessário especificar os quatro vértices do quadrado. 
			//glTextCoord - especificar coordenadas da textura applicada no quadrado
			//glVertex3d - especificar a localização do vértice do quadrado
			glTexCoord2d (0.0 ,0.0 );	glVertex3d (-worldSize,-worldSize+yy,worldSize);
			glTexCoord2d (1.0,0.0);	glVertex3d (-worldSize,-worldSize+yy,-worldSize);
			glTexCoord2d (1.0,1.0);	glVertex3d (-worldSize,worldSize+yy,-worldSize);
			glTexCoord2d (0.0,1.0);	glVertex3d (-worldSize,worldSize+yy,worldSize);
		glEnd ();				

		//Face frontal
		glBindTexture(GL_TEXTURE_2D, skyBoxTextures[4]);
		glBegin (GL_QUADS);
			glTexCoord2d (0.0,0.0);	glVertex3d (-worldSize,-worldSize+yy,-worldSize);
			glTexCoord2d (1.0,0.0);	glVertex3d (worldSize,-worldSize+yy,-worldSize);
			glTexCoord2d (1.0,1.0);	glVertex3d (worldSize,worldSize+yy,-worldSize);
			glTexCoord2d (0.0,1.0);	glVertex3d (-worldSize,worldSize+yy,-worldSize);
		glEnd ();

		//Face da direita
		glBindTexture(GL_TEXTURE_2D, skyBoxTextures[1]);
		glBegin (GL_QUADS);
			glTexCoord2d (0.0,0.0); glVertex3d (worldSize,-worldSize+yy,-worldSize);
			glTexCoord2d (1.0,0.0); glVertex3d (worldSize,-worldSize+yy,worldSize);
			glTexCoord2d (1.0,1.0); glVertex3d (worldSize,worldSize+yy,worldSize);
			glTexCoord2d (0.0 ,1.0); glVertex3d (worldSize,worldSize+yy,-worldSize);
		glEnd ();

		//Face de tras
		glBindTexture(GL_TEXTURE_2D, skyBoxTextures[5]);	
		glBegin (GL_QUADS);
			glTexCoord2d (0.0,0.0); glVertex3d (worldSize,-worldSize+yy,worldSize);
			glTexCoord2d (1.0,0.0); glVertex3d (-worldSize,-worldSize+yy,worldSize);
			glTexCoord2d (1.0,1.0); glVertex3d (-worldSize,worldSize+yy,worldSize);
			glTexCoord2d (0.0,1.0); glVertex3d (worldSize,worldSize+yy,worldSize);
		glEnd ();

		//Chao
		glBindTexture(GL_TEXTURE_2D, skyBoxTextures[3]);	
		glBegin (GL_QUADS);
			glTexCoord2d (0.0,0.0); glVertex3d (-worldSize,-worldSize+yy,worldSize);
			glTexCoord2d (1.0,0.0); glVertex3d (worldSize,-worldSize+yy,worldSize);
			glTexCoord2d (1.0,1.0); glVertex3d (worldSize,-worldSize+yy,-worldSize);
			glTexCoord2d (0.0,1.0); glVertex3d (-worldSize,-worldSize+yy,-worldSize);
		glEnd ();

		//Teto
		glBindTexture(GL_TEXTURE_2D, skyBoxTextures[2]);
		glBegin (GL_QUADS);
			glTexCoord2d (0.0,0.0); glVertex3d (-worldSize,worldSize+yy,-worldSize);
			glTexCoord2d (1.0,0.0); glVertex3d (worldSize,worldSize+yy,-worldSize);
			glTexCoord2d (1.0,1.0); glVertex3d (worldSize,worldSize+yy,worldSize);
			glTexCoord2d (0.0,1.0); glVertex3d (-worldSize,worldSize+yy,worldSize);		
		glEnd ();
	glPopMatrix();
}
