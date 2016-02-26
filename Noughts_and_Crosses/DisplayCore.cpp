#include <math.h>
#include <GL/glut.h>
#include "DisplayCore.h"

//Malha de Poligonos
bool		activeMeshPolygons		=		true;		//Ativa ou desativa o uso de malha de poligonos.
GLint		meshFactor				=		7;			//Controla a quantidade de poligonos em relacao ao tamanho (tamanhoObjecto/meshfactor).

//	Funcao que altera o meshFactor.
void changeMeshFactor(void){
	meshFactor = ((meshFactor + 1) % 15);
	if(meshFactor==0)
		meshFactor=1;
}

//	Funcao que ativa/desativa a malha de poligonos.
void meshPolygonsState(void){
	activeMeshPolygons = !activeMeshPolygons;
}

//	Funcao que desenha um cilindro sem textura.
void drawCylinderColor(GLUquadricObj * cylinderQuadric, GLfloat radiusB, GLfloat radiusT, GLfloat height)
{
	if(activeMeshPolygons)
		gluCylinder(cylinderQuadric,radiusB, radiusT, height,18*4,18*4);
	else
		gluCylinder(cylinderQuadric,radiusB, radiusT, height,18,18);
}

//	Funcao que desenha um cilindro e aplica-lhe uma textura.
void drawCylinder(GLUquadricObj * cylinderQuadric, GLuint texture, GLfloat radius, GLfloat height)
{
	glBindTexture(GL_TEXTURE_2D,texture);
	glColor4f(ABAJUR);
	if(activeMeshPolygons)
		gluCylinder(cylinderQuadric,radius, radius, height,18*4,18*4);
	else
		gluCylinder(cylinderQuadric,radius, radius, height,18,18);
}

//	Funcao que desenha uma esfera sem textura.
void drawSphereColor(GLUquadricObj * sphereQuadric, GLfloat radius)
{
	if(activeMeshPolygons)
		gluSphere(sphereQuadric,radius,18*4,18*4);
	else
		gluSphere(sphereQuadric,radius,18,18);
}

//	Funcao que desenha uma esfera e aplica-lhe uma textura.
void drawSphere(GLUquadricObj * sphereQuadric, GLuint texture, GLfloat radius)
{
	glBindTexture(GL_TEXTURE_2D,texture);
	if(activeMeshPolygons)
		gluSphere(sphereQuadric,radius,18*4,18*4);
	else
		gluSphere(sphereQuadric,radius,18,18);
}

//	Funcao para desenhar fumo estatico.
void drawSmoke(GLUquadricObj * sphereQuadric){
	for(int i=0; i<30; i++){
		glColor4f(0.5,0.5,0.5,1.0 - (GLfloat) (i*(1.0/30.0)));
		glPushMatrix();
			glTranslatef(0, (GLfloat)i/3, 0);
			drawSphereColor(sphereQuadric, 0.35);
		glPopMatrix();
	}
}

//	Funcao que desenha um boneco de neve.
void drawSnowMan(GLUquadricObj * sphereQuadric,GLUquadricObj * cylinderQuadric , GLuint bodyTexture, GLint eyesColor)
{
	//Desenha a base do boneco.
	glPushMatrix();
		glTranslatef(0, 0, -550);
		drawSphere(sphereQuadric, bodyTexture, 55);
	glPopMatrix();	
	//Desenha o meio.
	glPushMatrix();
		glTranslatef(0, 70, -550);
		drawSphere(sphereQuadric, bodyTexture, 39);
	glPopMatrix();
	//Desenha a cabeca
	glPushMatrix();
		glTranslatef(0, 130, -550);
		drawSphere(sphereQuadric, bodyTexture, 30);
	glPopMatrix();

	//Nariz
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
		glTranslatef(0, 123, -525);
		glColor4f(ORANJE);
		drawCylinderColor(cylinderQuadric, 3, 0, 20);
	glPopMatrix();

	//Cartola
	glPushMatrix();
		glTranslatef(0, 187, -550);
		glColor4f(BLACK);
		glRotatef(90, 1,0,0);
		drawCylinderColor(cylinderQuadric, 20, 20, 47);
	glPopMatrix();
	//Parte de baixo
	glPushMatrix();
		glTranslatef(0, 153, -550);
		glColor4f(BLACK);
		glRotatef(90, 1,0,0);
		drawCircle(35);
	glPopMatrix();
	
	// Bracos
	glColor4f(BLACK);
	glBegin(GL_LINES);
	glVertex3i( -102, 35, -520); 
	glVertex3i( -18, 95, -545); 
	glEnd();
	glBegin(GL_LINES);
	glVertex3i( 18, 95, -545); 
	glVertex3i( 102, 35, -520); 
	glEnd();
		
	//	Boca
	glColor4f(REDDARK);
	glBegin(GL_LINES);
	glVertex3i( -8, 113, -524); 
	glVertex3i( 8, 113, -524); 
	glEnd();

	//Olhos
	if(eyesColor == 0)
		glColor4f(REDDARK);
	else if(eyesColor ==1)
		glColor4f(BLACK);
	else
		glColor4f(WHITE);
	//O<-
	glPushMatrix();
		glTranslatef(-12, 134, -521);
		drawSphereColor(sphereQuadric, 3);
	glPopMatrix();
	//->O
	glPushMatrix();
		glTranslatef(12, 134, -521);
		drawSphereColor(sphereQuadric, 3);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
}

//	Funcao para desenhar um copo com agua.
void drawGlassWater(GLUquadricObj *cylinderQuadric)
{
	//Copo interior
	glFrontFace(GL_CW);		// Alteramos a face que sera visivel.
	glPushMatrix();
		glColor4f(GLASS);
		glRotatef(-90, 1,0,0);
		drawCylinderColor(cylinderQuadric, 4.5, 4.5, 20);
	glPopMatrix();
	//Agua interior
	glPushMatrix();
		glColor4f(WATER);
		glTranslatef(0, 1, 0);
		glRotatef(-90, 1,0,0);
		drawCylinderColor(cylinderQuadric, 4.0, 4.0, 14.5);
	glPopMatrix();
	glFrontFace(GL_CCW);	// Voltamos ao default.
	//Parte inferior da agua.
	glPushMatrix();
		glColor4f(WATER);
		glTranslatef(0, 1.1, 0);
		glRotatef(-90, 1,0,0);
		drawCircle(4.0);
	glPopMatrix();
	//Parte superior da agua.
	glPushMatrix();
		glTranslatef(0, 15.5, 0);
		glRotatef(-90, 1,0,0);
		drawCircle(4.0);
	glPopMatrix();
	//Agua Exterior
	glPushMatrix();
		glColor4f(WATER);
		glTranslatef(0, 1, 0);
		glRotatef(-90, 1,0,0);
		drawCylinderColor(cylinderQuadric, 4.0, 4.0, 14.5);
	glPopMatrix();
	//Copo exterior
	glPushMatrix();
		glColor4f(GLASS);
		glRotatef(-90, 1,0,0);
		drawCylinderColor(cylinderQuadric, 4.5, 4.5, 20);
	glPopMatrix();
}

//	Funcao para desenhar os suportes do abajur.
void drawBlackAxis(void)
{
	//	Eixo dos zz
	glColor4f(BLACK);
	glBegin(GL_LINES);
	glVertex3i( 0, 125, -29.9); 
	glVertex3i(0, 125, 29.9); 
	glEnd();
	//	Eixo dos zz  rodado 45graus
	glPushMatrix();
		glRotatef(45, 0, 1, 0);
		glBegin(GL_LINES);
		glVertex3i( 0, 125, -29.9); 
		glVertex3i(0, 125, 29.9); 
		glEnd();
	// Eixo dos xx rodado 45graus
		glBegin(GL_LINES);
		glVertex3i( -29.9, 125, 0); 
		glVertex3i( 29.9, 125, 0); 
		glEnd();	
	glPopMatrix();
	//	Eixo dos yy
	glBegin(GL_LINES);
	glVertex3i(0,  125, 0); 
	glVertex3i(0, 170, 0); 
	glEnd();
	// Eixo dos xx
	glBegin(GL_LINES);
	glVertex3i( -29.9, 125, 0); 
	glVertex3i( 29.9, 125, 0); 
	glEnd();
}

//	Funcao que desenha um poligono de 4 lados com textura, virado para (0, 0, 1). Com malha de poligonos. 
void drawQuads(GLuint texture,GLint dimX, GLint dimY){
	GLint meshX, meshY;
	if(activeMeshPolygons==false){//Caso a malha de poligonos nao esteja ativa
		meshX=1;
		meshY=1;
	}else{//Malha de poligonos ativa
		meshX=(GLint)dimX/meshFactor;
		meshY=(GLint)dimY/meshFactor;

		if(meshX<1)//No minimo tem que ser 1x1
			meshX=1;
		if(meshY<1)
			meshY=1;
	}
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glNormal3f(0,0,1);
	float moveX = (float) dimX/meshX;
	float moveY = (float) dimY/meshY;
	for(int i=0; i<meshY; i++){
		for(int j=0; j<meshX; j++){
			glTexCoord2f((GLfloat) (j+0)/meshX,(GLfloat) (i+0)/meshY);			glVertex3d((float)j*moveX,(float)i*moveY,0);				// P1
			glTexCoord2f((GLfloat) (j+1)/meshX,(GLfloat) (i+0)/meshY);			glVertex3d((float)(j+1)*moveX,(float)i*moveY,0);			// P2
			glTexCoord2f((GLfloat) (j+1)/meshX,(GLfloat) (i+1)/meshY);			glVertex3d((float)(j+1)*moveX,(float)(i+1)*moveY,0);		// P3
			glTexCoord2f((GLfloat) (j+0)/meshX,(GLfloat) (i+1)/meshY);			glVertex3d((float)j*moveX,(float)(i+1)*moveY,0);			// P4
		}
	}
	glEnd();
}

//	Funcao que desenha um poligono de 4 lados sem textura, virado para (0, 0, 1).
void drawQuadsSimple(GLfloat dimX, GLfloat dimY){
	glBegin(GL_QUADS);
	glNormal3f(0,0,1);
	glVertex3d(0,0,0);				// P1
	glVertex3d(dimX,0,0);			// P2
	glVertex3d(dimX,dimY,0);		// P3
	glVertex3d(0,dimY,0);			// P4
	glEnd();
}

//	Funcao que desenha texto.
void drawText(char *string, GLfloat x, GLfloat y, GLfloat z) 
{  
	glRasterPos3f(x,y,z); 
	while(*string)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string++); 
}

//	Funcao que desenha um cubo de gelo.
void drawIceCube(GLuint cubeTexture)
{
//		Face Superior:
	glPushMatrix();
	glTranslatef(-35,-1, 35);			// Faz translate para o local pretendido. 
	glRotatef(-90, 1, 0, 0);			// Roda o objecto 90 graus em torno dos xx.
	drawQuads(cubeTexture, 70, 70);
	glPopMatrix();
	//		Face Inferior:
	glPushMatrix();
	glTranslatef(-35,-21, -35);			// Faz translate para o local pretendido. 
	glRotatef(90, 1, 0, 0);				// Roda o objecto 90 graus em torno dos xx.
	drawQuads(cubeTexture, 70, 70);
	glPopMatrix();
	//	Faces laterais:
	//			Frontal
	glPushMatrix();
	glTranslatef(-35,-21, 35);			// Faz translate para o local pretendido. 
	drawQuads(cubeTexture, 70, 20);
	glPopMatrix();
	//			Traseira
	glPushMatrix();
	glTranslatef(35,-21, -35);			// Faz translate para o local pretendido.
	glRotatef(180, 0, 1, 0);			// Roda o objecto 90 graus em torno dos yy.
	drawQuads(cubeTexture, 70, 20);
	glPopMatrix();
	//			Direita
	glPushMatrix();
	glTranslatef(35,-21, 35);			// Faz translate para o local pretendido. 
	glRotatef(90, 0, 1, 0);				// Roda o objecto 90 graus em torno dos yy.
	drawQuads(cubeTexture, 70, 20);
	glPopMatrix();
	//			Esquerda
	glPushMatrix();
	glTranslatef(-35,-21, -35);			// Faz translate para o local pretendido. 
	glRotatef(-90, 0, 1, 0);			// Roda o objecto -90 graus em torno dos yy.
	drawQuads(cubeTexture, 70, 20);
	glPopMatrix();
}

//	Funcao que desenha um iglo.
void drawIgloo(GLUquadricObj * cylinderQuadric, GLUquadricObj * sphereQuadric, GLuint wallTexture, GLuint floorTexture, GLuint doorTexture, GLuint tunnelTexture, GLuint outsideDoorTexture)
{
	//Desenha o iglo.
	//Parte de dentro.	
	glFrontFace(GL_CW);					// Alteramos a face que sera visivel
	glPushMatrix();
		glRotatef(-90, 1,0,0);
		drawSphere(sphereQuadric, wallTexture, 250);
	glPopMatrix();
	glFrontFace(GL_CCW);				// Voltamos ao default.
	//Parte de fora.
	glPushMatrix();
		glRotatef(-90, 1,0,0);
		drawSphere(sphereQuadric, floorTexture, 250);
	glPopMatrix();

	//Chao (neve)
	glPushMatrix();
		glTranslatef(0, -20, 0);
		glRotatef(-90, 1,0,0);
		drawCircleTexture(250, floorTexture);
	glPopMatrix();

	//Saida do iglo:
	glPushMatrix();//Corpo
		glTranslatef(0, 0, -300);
		drawCylinder(cylinderQuadric, tunnelTexture , 100, 100);
	glPopMatrix();
	//Saida
	glPushMatrix();
		glTranslatef(0, 0, -200);
		drawCircleTexture(100, doorTexture);
	glPopMatrix();

	//Entrada
	glPushMatrix();
		glTranslatef(0, 0, -300);
		glRotatef(180,0,1,0);
		drawCircleTexture(100, outsideDoorTexture);
	glPopMatrix();
}

//	Funcao que desenha uma peca do jogo do galo. colourFlag-> 0 Green, 1 Red
void drawPiece(GLfloat xBaseCenter, GLfloat yBaseCenter, GLfloat zBaseCenter, GLint colourFlag)
{
	if(colourFlag==1)
		glColor4f(RED);
	else
		glColor4f(GREEN);
	glBegin(GL_QUADS);
	//		Base da cruz:
	glNormal3f(0,-1,0);
	glVertex3f(xBaseCenter + 0.5, yBaseCenter, zBaseCenter + 0.5);			// P1
	glVertex3f(xBaseCenter + 0.5, yBaseCenter, zBaseCenter - 0.5);			// P2
	glVertex3f(xBaseCenter - 0.5, yBaseCenter, zBaseCenter - 0.5);			// P3
	glVertex3f(xBaseCenter - 0.5, yBaseCenter, zBaseCenter + 0.5);			// P4
	//		Topo da cruz:
	glNormal3f(0,1,0);
	glVertex3f(xBaseCenter + 0.5, yBaseCenter + 6, zBaseCenter + 0.5);		// P1
	glVertex3f(xBaseCenter + 0.5, yBaseCenter + 6, zBaseCenter - 0.5);		// P2
	glVertex3f(xBaseCenter - 0.5, yBaseCenter + 6, zBaseCenter - 0.5);		// P3
	glVertex3f(xBaseCenter - 0.5, yBaseCenter + 6, zBaseCenter + 0.5);		// P4

	glNormal3f(0,0,1);
	glVertex3f(xBaseCenter + 0.5, yBaseCenter, zBaseCenter + 0.5);			// P1
	glVertex3f(xBaseCenter + 0.5, yBaseCenter + 6, zBaseCenter + 0.5);		// P2
	glVertex3f(xBaseCenter - 0.5, yBaseCenter + 6 , zBaseCenter + 0.5);		// P3
	glVertex3f(xBaseCenter - 0.5, yBaseCenter, zBaseCenter + 0.5);			// P4

	glNormal3f(1,0,0);
	glVertex3f(xBaseCenter + 0.5, yBaseCenter, zBaseCenter - 0.5);			// P1
	glVertex3f(xBaseCenter + 0.5, yBaseCenter + 6, zBaseCenter - 0.5);		// P2
	glVertex3f(xBaseCenter + 0.5, yBaseCenter + 6 , zBaseCenter + 0.5);		// P3
	glVertex3f(xBaseCenter + 0.5, yBaseCenter, zBaseCenter + 0.5);			// P4

	glNormal3f(0,0,-1);
	glVertex3f(xBaseCenter - 0.5, yBaseCenter, zBaseCenter - 0.5);			// P1
	glVertex3f(xBaseCenter - 0.5, yBaseCenter + 6, zBaseCenter - 0.5);		// P2
	glVertex3f(xBaseCenter + 0.5, yBaseCenter + 6 , zBaseCenter - 0.5);		// P3
	glVertex3f(xBaseCenter + 0.5, yBaseCenter, zBaseCenter - 0.5);			// P4

	glNormal3f(-1,0,0);
	glVertex3f(xBaseCenter - 0.5, yBaseCenter, zBaseCenter + 0.5);			// P1
	glVertex3f(xBaseCenter - 0.5, yBaseCenter + 6, zBaseCenter + 0.5);		// P2
	glVertex3f(xBaseCenter - 0.5, yBaseCenter + 6 , zBaseCenter - 0.5);		// P3
	glVertex3f(xBaseCenter - 0.5, yBaseCenter, zBaseCenter - 0.5);			// P4
	glEnd();
}

//	Funcao que desenha um quadro.
void drawFrame(GLuint frameTexture, GLuint pictureTexture)
{
	glColor3f(1.0f,1.0f,1.0f);
	//		Face frontal do quadro:
	glPushMatrix();
	glTranslatef(75,20, 248);					// Faz translate para o local pretendido. 
	glRotatef(-180, 0,1,0);						// Roda o objecto em torno dos yy.
	drawQuads(pictureTexture, 150, 100);
	glPopMatrix();
	//		Face de tras do quadro:
	//glPushMatrix();
	//glTranslatef(0,70, 250);					// Faz translate para o local pretendido. 
	//glTranslatef(-75,-50, 0);					// Faz translate para a origem. 
	//drawQuads(frameTexture, 150, 100);
	//glPopMatrix();
	//		Faces Laterais:
	//		Lateral direita
	glPushMatrix();
	glTranslatef(0,70, 250);					// Faz translate para o local pretendido. 
	glTranslatef(75,0.0, -1.0);					// Faz translate para o local correto
	glRotatef(90, 0,1,0);						// Roda o objecto em torno dos yy.
	glTranslatef(-1,-50, 0.0);					// Faz translate para a origem. 
	drawQuads(frameTexture, 2, 100);
	glPopMatrix();
	//		Lateral esquerda
	glPushMatrix();
	glTranslatef(0,70, 250);					// Faz translate para o local pretendido. 
	glTranslatef(-75,0.0, -1.0);				// Faz translate para o local correto
	glRotatef(-90, 0,1,0);						// Roda o objecto em torno dos yy.
	glTranslatef(-1,-50, 0.0);					// Faz translate para a origem. 
	drawQuads(frameTexture, 2, 100);
	glPopMatrix();
	//		Lateral superior
	glPushMatrix();
	glTranslatef(0,70, 250);					// Faz translate para o local pretendido. 
	glTranslatef(0,50.0, -1.0);					// Faz translate para o local correto
	glRotatef(-90, 1,0,0);						// Roda o objecto em torno dos xx.
	glTranslatef(-75,-1, 0.0);					// Faz translate para a origem. 
	drawQuads(frameTexture, 150, 2);
	glPopMatrix();	
	//		Lateral inferior
	glPushMatrix();
	glTranslatef(0,70, 250);					// Faz translate para o local pretendido. 
	glTranslatef(0,-50.0, -1.0);					// Faz translate para o local correto
	glRotatef(90, 1,0,0);						// Roda o objecto em torno dos xx.
	glTranslatef(-75,-1, 0.0);					// Faz translate para a origem. 
	drawQuads(frameTexture, 150, 2);
	glPopMatrix();
}

//	Funcao que desenha o relogio na parede.
void drawClock(GLuint frontTexture, GLuint othersTexture, GLfloat hourAngle, GLfloat minutesAngle, GLfloat secondsAngle){
	glEnable(GL_TEXTURE_2D);
	glColor4f(WHITE);
	//		Face do relogio:
	glPushMatrix();
	glTranslatef(110.0,80, -248);				// Translate para a parede.
	glTranslatef(-25,-25, 0.0);					// Faz translate para a origem. 
	drawQuads(frontTexture, 50, 50);
	glPopMatrix();

	/*//		Parte de tras do relogio:
	glPushMatrix();
	glTranslatef(110.0,80, -250);				// Translate para a parede. 
	glRotatef(-180, 0,1,0);						// Roda o objecto em torno dos yy.
	glTranslatef(-25,-25, 0.0);					// Faz translate para a origem. 
	drawQuads(othersTexture, 50, 50);
	glPopMatrix();*/

	//	Lateral direita do relogio:
	glPushMatrix();
	glTranslatef(110.0,80, -248);				// Translate para a parede.
	glTranslatef(25,0.0, -1.0);					// Faz translate para o local correto
	glRotatef(90, 0,1,0);						// Roda o objecto em torno dos yy.
	glTranslatef(-1,-25, 0.0);					// Faz translate para a origem. 
	drawQuads(othersTexture, 2, 50);
	glPopMatrix();

	//	Lateral esquerda do relogio:
	glPushMatrix();
	glTranslatef(110.0,80, -248);				// Translate para a parede.
	glTranslatef(-25,0.0, -1.0);				// Faz translate para o local correto
	glRotatef(-90, 0,1,0);						// Roda o objecto em torno dos yy.
	glTranslatef(-1,-25, 0.0);					// Faz translate para a origem. 
	drawQuads(othersTexture, 2, 50);
	glPopMatrix();

	//	Lateral superior do relogio:
	glPushMatrix();
	glTranslatef(110.0,80, -248);				// Translate para a parede.
	glRotatef(90, 0,0,1);						// Roda o objecto em torno dos yy.
	glTranslatef(25,0.0, -1.0);					// Faz translate para o local correto
	glRotatef(90, 0,1,0);						// Roda o objecto em torno dos yy.
	glTranslatef(-1,-25, 0.0);					// Faz translate para a origem. 
	drawQuads(othersTexture, 2, 50);
	glPopMatrix();

	//	Lateral inferior do relogio:
	glPushMatrix();
	glTranslatef(110.0,80, -248);				// Translate para a parede.
	glRotatef(-90, 0,0,1);						// Roda o objecto em torno dos yy.
	glTranslatef(25,0.0, -1.0);					// Faz translate para o local correto
	glRotatef(90, 0,1,0);						// Roda o objecto em torno dos yy.
	glTranslatef(-1,-25, 0.0);					// Faz translate para a origem. 
	drawQuads(othersTexture, 2, 50);
	glPopMatrix();
	//		Ponteiro dos minutos:
	glDisable(GL_TEXTURE_2D);
	glColor4f(BLACK);
	glPushMatrix();
	glTranslatef(110.0,80, -248);				// Translate para a parede.
	glRotatef(minutesAngle,0,0,1);				// Roda o objecto em torno dos zz.
	glTranslatef(-0.35,0, 0.06);				// Faz translate para a origem.
	drawQuadsSimple(0.7, 12.5);
	glPopMatrix();
	//		Ponteiro das horas:
	glColor4f(BLUE);
	glPushMatrix();
	glTranslatef(110.0,80, -248);			// Translate para a parede.
	glRotatef(hourAngle,0,0,1);				// Roda o objecto em torno dos zz.
	glTranslatef(-0.5, 0, 0.08);			// Faz translate para a origem. 
	drawQuadsSimple(1, 9);
	glPopMatrix();
	//		Ponteiro dos segundos:
	glColor4f(RED);
	glPushMatrix();
	glTranslatef(110.0,80, -248);			// Translate para a parede.
	glRotatef(secondsAngle,0,0,1);			// Roda o objecto em torno dos zz.
	glTranslatef(-0.2, 0, 0.1);				// Faz translate para a origem. 
	drawQuadsSimple(0.4, 15);
	glPopMatrix();
}


//	Funcao que desenha uma carpete. 120x88
void drawCarpet(GLuint carpetTexture)
{
	//		Carpete:
	glPushMatrix();
	glColor3f(1.0f,1.0f,1.0f);
	glTranslatef(0,-79.9, -200);				// Faz translate para o local pretendido. 
	glRotatef(-90, 1, 0, 0);					// Roda o objecto -90 graus em torno dos xx.
	glTranslatef(-60,-44, 0);					// Faz translate para a origem. 
	drawQuads(carpetTexture, 120, 88);
	glPopMatrix();
}


//	Funcao que desenha a sala/casa.
void drawRoom(GLuint floorTexture, GLuint wallTexture, GLuint wallUpperDoorTexture , GLuint doorTexture, GLuint windowLeftTexture, GLuint leftWallWindow, GLuint rightWallWindow, GLuint upperWallWindow, GLuint bottomWallWindow, GLuint windowRightTexture, GLuint windowBottomTexture, GLuint windowUpperTexture, GLuint windowMidleUpTexture, GLuint windowMidleRightTexture, GLfloat windowRotationAngle)
{
	glColor3f(1.0f,1.0f,1.0f);
	//		Chao:
	glPushMatrix();
	glTranslatef(-250,-80.0, -250);		// Faz translate para o local pretendido. 
	glRotatef(90, 0, 0, -1);			// Roda o objecto 90 graus em torno dos -zz.
	glRotatef(-90, 0, 1, 0);			// Roda o objecto -90 graus em torno dos yy.
	drawQuads(floorTexture, 500, 500);
	glPopMatrix();
	//		Teto:
	glPushMatrix();
	glTranslatef(250,170.0, -250);		// Faz translate para o local pretendido. 
	glRotatef(-90, 0, 0, -1);			// Roda o objecto -90 graus em torno dos -zz.
	glRotatef(-90, 0, 1, 0);			// Roda o objecto -90 graus em torno dos yy.
	drawQuads(floorTexture, 500, 500);
	glPopMatrix();
	//		Face Direita:
	glPushMatrix();
	glTranslatef(250.0,-80.0,-250);		// Faz translate para o local pretendido. 
	glRotatef(-90, 0, 1, 0);			// Roda o objecto -90 graus em torno dos yy.
	drawQuads(wallTexture, 500, 250);
	glPopMatrix();
	//		Face Fundo Esquerda:
	glPushMatrix();
	glTranslatef(-250.0,-80,-250);  // Faz translate para o local pretendido. 
	drawQuads(wallTexture, 214, 250);
	glPopMatrix();
	//		Face Fundo Direita:
	glPushMatrix();
	glTranslatef(36,-80,-250);  // Faz translate para o local pretendido. 
	drawQuads(wallTexture, 214, 250);
	glPopMatrix();
	//		Face Fundo por cima da porta:
	glPushMatrix();
	glTranslatef(-36,110,-250);  // Faz translate para o local pretendido. 
	drawQuads(wallUpperDoorTexture, 72, 60);
	glPopMatrix();
	//		PORTA:
	glPushMatrix();
	glTranslatef(-36.0,-80.0,-250);  // Faz translate para o local pretendido. 
	drawQuads(doorTexture, 72, 190);
	glPopMatrix();
	//		Faces da Esquerda:
	//Esquerda
	glPushMatrix();
	glTranslatef(-250.0,-80,250);		// Faz translate para o local pretendido. 
	glRotatef(90, 0, 1, 0);			// Roda o objecto -90 graus em torno dos yy.
	drawQuads(leftWallWindow, 189, 250);
	glPopMatrix();
	//Cima da janela
	glPushMatrix();
	glTranslatef(-250.0,130,61.5);		// Faz translate para o local pretendido. 
	glRotatef(90, 0, 1, 0);			// Roda o objecto -90 graus em torno dos yy.
	drawQuads(upperWallWindow, 123, 40);
	glPopMatrix();
	//De baixo da janela
	glPushMatrix();
	glTranslatef(-250.0,-80,61.5);		// Faz translate para o local pretendido. 
	glRotatef(90, 0, 1, 0);			// Roda o objecto -90 graus em torno dos yy.
	drawQuads(bottomWallWindow, 123, 60);
	glPopMatrix();
	//Direita
	glPushMatrix();
	glTranslatef(-250.0,-80,-61.5);		// Faz translate para o local pretendido. 
	glRotatef(90, 0, 1, 0);			// Roda o objecto -90 graus em torno dos yy.
	drawQuads(rightWallWindow,189, 250);
	glPopMatrix();
	//		Janela:
	//Parte de baixo
	glPushMatrix();
	glTranslatef(-250,-20, 61.5);					// Faz translate para o local pretendido. 
	glRotatef(90, 0, 1,0);							// Rotacao 90graus yy.
	drawQuads(windowBottomTexture, 123, 28);
	glPopMatrix();
	//Parte de cima
	glPushMatrix();
	glTranslatef(-250,110, 61.5);					// Faz translate para o local pretendido. 
	glRotatef(90, 0, 1,0);							// Rotacao 90graus yy.
	drawQuads(windowUpperTexture, 123, 20);
	glPopMatrix();
	//Parte da esquerda
	glPushMatrix();
	glTranslatef(-250,8, 61.5);					// Faz translate para o local pretendido. 
	glRotatef(90, 0, 1,0);							// Rotacao 90graus yy.
	drawQuads(windowLeftTexture, 19, 102);
	glPopMatrix();
	//Parte da direita
	glPushMatrix();
	glTranslatef(-250,8, -36.5);					// Faz translate para o local pretendido. 
	glRotatef(90, 0, 1,0);							// Rotacao 90graus yy.
	drawQuads(windowRightTexture, 25.5, 102);
	glPopMatrix();
	//Partes do meio:
	//   |
	glPushMatrix();
	glTranslatef(-249.9, 8, 3.9+37.45);					// Faz translate para o local pretendido. 
	glRotatef(90+windowRotationAngle, 0, 1,0);							// Rotacao 90graus yy.
	glTranslatef(37.45, 0, 0);					// Faz translate para o local pretendido. 
	drawQuads(windowMidleUpTexture, 4.1, 102);
	glPopMatrix();
	//  --
	glPushMatrix();
	glTranslatef(-249.95, 55.65, 42.5);					// Faz translate para o local pretendido. 
	glRotatef(90+windowRotationAngle, 0, 1,0);							// Rotacao 90graus yy.
	drawQuads(windowMidleRightTexture, 79, 2);
	glPopMatrix();

	//		Face Frente:
	glPushMatrix();
	glTranslatef(250,-80, 250);		// Faz translate para o local pretendido. 
	glRotatef(-180, 0, -1, 0);			// Roda o objecto -90 graus em torno dos yy.
	drawQuads(wallTexture, 500, 250);
	glPopMatrix();
}

//	Funcao para desenhar os eixos.
void drawAxis(void)
{
	//	Eixo dos zz
	glColor4f(BLUE);
	glBegin(GL_LINES);
	glVertex3i( 0, 0, -xC); 
	glVertex3i(0, 0, xC); 
	glEnd();

	//	Eixo dos yy
	glColor4f(GREEN);
	glBegin(GL_LINES);
	glVertex3i(0,  -xC, 0); 
	glVertex3i(0, xC, 0); 
	glEnd();

	// Eixo dos xx
	glColor4f(RED);
	glBegin(GL_LINES);
	glVertex3i( -xC, 0, 0); 
	glVertex3i( xC, 0, 0); 
	glEnd();
}

// Funcao que desenha um circulo e aplica-lhe uma textura.
void drawCircleTexture(float radius, GLuint texture)
{
	glColor4f(WHITE);
	float angle, rad, x, y, xcos, ysin, tx, ty;
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_POLYGON);
	for (int i=0; i<360; i++) {
		rad = i * (PI/180.0f);
		xcos = (float)cos(rad);
		ysin = (float)sin(rad);
		x = xcos * radius;
		y = ysin * radius;
		tx = xcos * 0.5 + 0.5;
		ty = ysin * 0.5 + 0.5;

		glTexCoord2f(tx, ty);
		glVertex2f(x, y);
	}
	glEnd();
}

// Funcao que desenha um circulo.
void drawCircle(float radius)
{
	float rad;	 
	glBegin(GL_POLYGON);
	for (int i=0; i<360; i++) {
		rad = i*PI/180.0;
		glVertex2f( radius*cos(rad), radius*sin(rad));
	}
	glEnd();
}

//	Funcao que desenha a mesa.
void drawTable(GLuint tableTexture)
{
	glColor3f(1.0f,1.0f,1.0f);
	//		Face Superior:
	glPushMatrix();
	glTranslatef(-65,-1, 35);			// Faz translate para o local pretendido. 
	glRotatef(-90, 1, 0, 0);			// Roda o objecto 90 graus em torno dos xx.
	drawQuads(tableTexture, 130, 70);
	glPopMatrix();
	//		Face Inferior:
	glPushMatrix();
	glTranslatef(-65,-4, -35);			// Faz translate para o local pretendido. 
	glRotatef(90, 1, 0, 0);			// Roda o objecto 90 graus em torno dos xx.
	drawQuads(tableTexture, 130, 70);
	glPopMatrix();
	//	Faces laterais:
	//			Frontal
	glPushMatrix();
	glTranslatef(-65,-4, 35);		// Faz translate para o local pretendido. 
	drawQuads(tableTexture, 130, 3);
	glPopMatrix();
	//			Traseira
	glPushMatrix();
	glTranslatef(65,-4, -35);		// Faz translate para o local pretendido.
	glRotatef(180, 0, 1, 0);			// Roda o objecto 90 graus em torno dos yy.
	drawQuads(tableTexture, 130, 3);
	glPopMatrix();
	//			Direita
	glPushMatrix();
	glTranslatef(65,-4, 35);		// Faz translate para o local pretendido. 
	glRotatef(90, 0, 1, 0);			// Roda o objecto 90 graus em torno dos yy.
	drawQuads(tableTexture, 70, 3);
	glPopMatrix();
	//			Esquerda
	glPushMatrix();
	glTranslatef(-65,-4, -35);		// Faz translate para o local pretendido. 
	glRotatef(-90, 0, 1, 0);			// Roda o objecto -90 graus em torno dos yy.
	drawQuads(tableTexture, 70, 3);
	glPopMatrix();

	//			Pernas direita-frente da mesa:
	//Frente
	glPushMatrix();
	glTranslatef(55,-4, 30);		// Faz translate para o local pretendido. 
	glRotatef(-90, 0,0, 1);			// Roda o objecto -90 graus em torno dos zz.
	drawQuads(tableTexture, 76, 5);
	glPopMatrix();
	//Tras
	glPushMatrix();
	glTranslatef(60,-4, 25);		// Faz translate para o local pretendido. 
	glRotatef(-180, 0,1, 0);		// Roda o objecto -180 graus em torno dos yy.
	glRotatef(-90, 0,0, 1);			// Roda o objecto -90 graus em torno dos zz.
	drawQuads(tableTexture, 76, 5);
	glPopMatrix();
	//Esquerda
	glPushMatrix();
	glTranslatef(55,-4, 25);		// Faz translate para o local pretendido. 
	glRotatef(-90, 0,1, 0);			// Roda o objecto 90 graus em torno dos yy.
	glRotatef(-90, 0,0, 1);			// Roda o objecto -90 graus em torno dos zz.
	drawQuads(tableTexture, 76, 5);
	glPopMatrix();
	//Direita
	glPushMatrix();
	glTranslatef(60,-4, 30);			// Faz translate para o local pretendido. 
	glRotatef(90, 0,1, 0);				// Roda o objecto 90 graus em torno dos yy.
	glRotatef(-90, 0,0, 1);				// Roda o objecto -90 graus em torno dos zz.
	drawQuads(tableTexture, 76, 5);
	glPopMatrix();

	//			Pernas direita-tras da mesa:
	//Frente
	glPushMatrix();
	glTranslatef(55,-4, -25);		// Faz translate para o local pretendido. 
	glRotatef(-90, 0,0, 1);			// Roda o objecto -90 graus em torno dos zz.
	drawQuads(tableTexture, 76, 5);
	glPopMatrix();
	//Tras
	glPushMatrix();
	glTranslatef(60,-4, -30);		// Faz translate para o local pretendido. 
	glRotatef(-180, 0,1, 0);		// Roda o objecto -180 graus em torno dos yy.
	glRotatef(-90, 0,0, 1);			// Roda o objecto -90 graus em torno dos zz.
	drawQuads(tableTexture, 76, 5);
	glPopMatrix();
	//Esquerda
	glPushMatrix();
	glTranslatef(55,-4, -30);		// Faz translate para o local pretendido. 
	glRotatef(-90, 0,1, 0);			// Roda o objecto 90 graus em torno dos yy.
	glRotatef(-90, 0,0, 1);			// Roda o objecto -90 graus em torno dos zz.
	drawQuads(tableTexture, 76, 5);
	glPopMatrix();
	//Direita
	glPushMatrix();
	glTranslatef(60,-4, -25);			// Faz translate para o local pretendido. 
	glRotatef(90, 0,1, 0);				// Roda o objecto 90 graus em torno dos yy.
	glRotatef(-90, 0,0, 1);				// Roda o objecto -90 graus em torno dos zz.
	drawQuads(tableTexture, 76, 5);
	glPopMatrix();

	//			Pernas esquerda-frente da mesa:
	//Frente
	glPushMatrix();
	glTranslatef(-60,-4, 30);		// Faz translate para o local pretendido. 
	glRotatef(-90, 0,0, 1);			// Roda o objecto -90 graus em torno dos zz.
	drawQuads(tableTexture, 76, 5);
	glPopMatrix();
	//Tras
	glPushMatrix();
	glTranslatef(-55,-4, 25);		// Faz translate para o local pretendido. 
	glRotatef(-180, 0,1, 0);		// Roda o objecto -180 graus em torno dos yy.
	glRotatef(-90, 0,0, 1);			// Roda o objecto -90 graus em torno dos zz.
	drawQuads(tableTexture, 76, 5);
	glPopMatrix();
	//Esquerda
	glPushMatrix();
	glTranslatef(-60,-4, 25);		// Faz translate para o local pretendido. 
	glRotatef(-90, 0,1, 0);			// Roda o objecto 90 graus em torno dos yy.
	glRotatef(-90, 0,0, 1);			// Roda o objecto -90 graus em torno dos zz.
	drawQuads(tableTexture, 76, 5);
	glPopMatrix();
	//Direita
	glPushMatrix();
	glTranslatef(-55,-4, 30);			// Faz translate para o local pretendido. 
	glRotatef(90, 0,1, 0);				// Roda o objecto 90 graus em torno dos yy.
	glRotatef(-90, 0,0, 1);				// Roda o objecto -90 graus em torno dos zz.
	drawQuads(tableTexture, 76, 5);
	glPopMatrix();

	//			Pernas esquerda-tras da mesa:
	//Frente
	glPushMatrix();
	glTranslatef(-60,-4, -25);		// Faz translate para o local pretendido. 
	glRotatef(-90, 0,0, 1);			// Roda o objecto -90 graus em torno dos zz.
	drawQuads(tableTexture, 76, 5);
	glPopMatrix();
	//Tras
	glPushMatrix();
	glTranslatef(-55,-4, -30);		// Faz translate para o local pretendido. 
	glRotatef(-180, 0,1, 0);		// Roda o objecto -180 graus em torno dos yy.
	glRotatef(-90, 0,0, 1);			// Roda o objecto -90 graus em torno dos zz.
	drawQuads(tableTexture, 76, 5);
	glPopMatrix();
	//Esquerda
	glPushMatrix();
	glTranslatef(-60,-4, -30);		// Faz translate para o local pretendido. 
	glRotatef(-90, 0,1, 0);			// Roda o objecto 90 graus em torno dos yy.
	glRotatef(-90, 0,0, 1);			// Roda o objecto -90 graus em torno dos zz.
	drawQuads(tableTexture, 76, 5);
	glPopMatrix();
	//Direita
	glPushMatrix();
	glTranslatef(-55,-4, -25);			// Faz translate para o local pretendido. 
	glRotatef(90, 0,1, 0);				// Roda o objecto 90 graus em torno dos yy.
	glRotatef(-90, 0,0, 1);				// Roda o objecto -90 graus em torno dos zz.
	drawQuads(tableTexture, 76, 5);
	glPopMatrix();
}

//	Funcao que desenha o tabuleiro do jogo do galo.
void drawGameBoard(GLuint upperFaceTexture, GLuint otherFacesTexture)
{
	glColor3f(1.0f,1.0f,1.0f);
	//		Face Superior:
	glPushMatrix();
	glTranslatef(0,1, 0);						// Faz translate para o local pretendido. 
	glRotatef(-90, 1, 0, 0);					// Roda o objecto -90 graus em torno dos xx.
	glTranslatef(-20,-16, 0);					// Faz translate para a origem. 
	drawQuads(upperFaceTexture, 40, 32);
	glPopMatrix();

	//		Faces Laterais:
	//		Frontal:
	glPushMatrix();
	glTranslatef(-20,-1, 16);					// Faz translate para o local pretendido. 
	drawQuads(otherFacesTexture, 40, 2);
	glPopMatrix();
	//		Traseira:
	glPushMatrix();
	glTranslatef(20,-1, -16);					// Faz translate para o local pretendido. 
	glRotatef(180, 0, 1,0);						// Rotacao 180graus yy.
	drawQuads(otherFacesTexture, 40, 2);
	glPopMatrix();
	//		Direita:
	glPushMatrix();
	glTranslatef(20,-1, 16);					// Faz translate para o local pretendido. 
	glRotatef(90, 0, 1,0);						// Rotacao 90graus yy.
	drawQuads(otherFacesTexture, 32, 2);
	glPopMatrix();
	//		Esquerda:
	glPushMatrix();
	glTranslatef(-20,-1, -16);					// Faz translate para o local pretendido. 
	glRotatef(-90, 0, 1,0);						// Rotacao 90graus yy.
	drawQuads(otherFacesTexture, 32, 2);
	glPopMatrix();
}
