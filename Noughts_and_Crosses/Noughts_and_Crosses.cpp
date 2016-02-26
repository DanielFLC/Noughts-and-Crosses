#include <windows.h>
#include <stdlib.h>			//  For mallocs and others...
#include <stdio.h>			//  Standard Input\Output C Library
#include <math.h>
#include <time.h>
#include <GL/glut.h>		//  Include GLUT, OpenGL, and GLU libraries
#include "DisplayCore.h"
#include "BoardGameCore.h"
#include "RgbImage.h"
#include "Sounds.h"
#include "System.h"
#include "SkyBox.h"

//		Gravidade:
#define G		9.8						//Necessitamos deste valor para o calculo de movimentacao (bola de basket no exterior da casa)

//		Window size:
GLfloat wScreen	=	800;
GLfloat hScreen	=	800;
bool fullScreen =	false;

//		Mudanca de Cenarios:
GLint	scene			=	0;					//Cenario.  0-Casa Original  1-Iglo na serra da estrela (com possibilidade de sair ao exterior)
bool	insideIgloo		=	true;				//Se esta dentro ou fora do iglo (so funciona no caso do cenario alternativo)
GLfloat skyboxSize				=	1800;		//Tamanho da skybox para o cenário principal.
GLfloat skyboxAlternativeSize	=	4500;		//Tamanho da skybox para o cenário alternativo.

//		Olhos a piscar do boneco de neve:
GLint	colorSnowMan	=	1;			//Mudar a cor aos olhos do boneco de neve.

//		HUD:
bool		hudMode		=	true;		//True se queremos que ele seja apresentado. Falso caso contrario.

//		Janela da Sala/Casa:
GLfloat		windowRotationAngle			=	0;  //Serve para ir incrementando ou diminuindo o angulo da janela consoante a abrimos ou fechamos.
GLfloat		windowRotationIncrement		=	0;	//Velocidade de rotacao da janela. (comeca a 0 porque nao queremos que se mova ao inicio, so quando interagimos com ela)

//		Basketball Outside House:
GLfloat		basketBallPosition[]			=	{ -630.0, -70.0, -700.0 };		//Posicao inicial da bola de basket.
GLfloat		incrementY = 10.0, incrementX = 0.0, incrementZ = 2.3;				//Velocidade de incremento consoante os eixos.
GLfloat		rotationAngle = 0.0, rotationIncrement = 3.5;						//Velocidade de rotacao da bola.

//		SkyBox:
SkyBox * skyBox;							//Skybox usada para desenhar o cenario exterior da casa e do cenario da neve.

//		Eixos:
bool	axisMode		=	false;			//Ativa ou desativa o desenho dos eixos (xx, yy, zz)

//		Sistema de particulas:
System particleSystem;				
bool	smokeParticles		=	true;		//Ativa/desativa o sistema de particulas do fumo do cigarro.
//		Cigarro a arder:
GLfloat burningIncrement	=	0.0015;		//Quanto maior este valor mais rapidamente o cigarro arde.
GLfloat	burningState		=	0.0;		//O quanto ja ardeu (0=cigarro inteiro);
bool	burnMode			=	false;		//Se queremos que o cigarro queime ou nao.

//		Mostrar FPS:
bool	showFPS = true;						//Se queremos que a informacao de frames por segundo seja mostrada.			

//		Relogio:
GLint hour, minute, second;
struct tm *current_time;	

//		Movimentacao:
bool		crouch				=	false;	//Se esta em modo ajoelhado ou de pe.

//		Observador:
GLfloat		angZoom				=	90;
GLfloat		rVisao				=	50;
GLfloat		aVisao				=	0.5 * PI;
GLfloat		incVisao			=	0.5;
GLfloat		obsP[]				=	{ rVisao*cos(aVisao), 70, rVisao*sin(aVisao) + 50.0 };		//Posicao do observador.
GLfloat		obsPfin[]			=	{ 0, 70, 0 };												//Direcao do olhar do observador.

//		Textures:
GLuint		textures[40];					//	Array que aremazena praticamente todas as texturas usadas.
RgbImage	imag;							//	Classe que trata da leitura das imagens.
GLint		wallTextureMode = 1;			//	Muda a textura da parede da sala. (false - madeira, true - tijolo)

//		Quadrics:
GLUquadricObj *sphereQuadric;
GLUquadricObj *cylinderQuadric;

//		Iluminacao:
// Luz Ambiente:
bool		nightMode				=	false;											//Controla a luz ambiente.
GLfloat ambientDayColorLight[4]		= {0.90f, 0.885f, 0.885f, 1.0f};					//Luz ambiente maxima (Dia).
GLfloat ambientNightColorLight[4]	= {0.021f, 0.0195f, 0.0195f, 1.0f};					//Luz ambiente minima (Noite).
GLfloat ambientColorLight[4]		= {0.24f, 0.225f, 0.225f, 1.0f};					//Luz inicial.
GLfloat colorIncrement				=  0.0004;							//Incremento inicial de mudanca da luz
//			Luz Pontual Teto:
bool		ceilingLightMode		=	false;							//Controla a luz do teto.
GLfloat		ceilingLightColor[4]	=	{1.0,1.0,0.9,1.0};				//Cor da luz
GLfloat		ceilingLightColDif[4]	=	{0.85, 0.85,0.75, 1.0};			//Cor da difusa
GLfloat		ceilingLightColEsp[4]	=	{1.0,1.0,0.9,1.0};				//Cor da especular
GLfloat		ceilingLightPos[4]		=	{0.0, 110, 0.0, 1.0};			//Posicao da luz do teto.
GLfloat		ceilingLightAttCon		=	1.0;				
GLfloat		ceilingLightAttLin		=	0.008;
GLfloat		ceilingLightAttQua		=	0.0;
//			Lanterna:
bool		flashLightMode			=	false;								//Controla a lanterna.
GLfloat		flashLightPos[4]		=	{obsP[0], obsP[1], obsP[2], 1.0};	//Posicao da lanterna
GLfloat		flashLightDir[3]		=	{obsPfin[0]-obsP[0], obsPfin[1]-obsP[1], obsPfin[2]-obsP[2]};	//Direcao da lanterna
GLfloat		flashLightExp			=	50;								//Angulo de abertura da luz.
GLfloat		flashLightCut			=	15.0f;					
GLfloat		flashLightColEsp[4]		=	{1.0 , 1.0, 1.0, 1.0};			//Cor da especular.
GLfloat		flashLightColDif[4]		=	{0.85, 0.85,0.85, 1.0};			//Cor da difusa.
//			Especular:
GLfloat matEspecular[]				=	{0.9, 0.9, 0.9, 1.0};			//Cor material especular
GLfloat matDefaultEspecular[]		=	{0.0,0.0,0.0,1.0};				//Cor default especular (nao ha especular).
GLint especMaterial					=	10;								//Tipo de especularidade.

//			Nevoeiro:
GLfloat		fogColor[]				=	{0.2, 0.2, 0.2, 1.0};			//Cor do nevoeiro de dia.
GLfloat		fogDayColor[]			=	{0.8, 0.8, 0.8, 1.0};			//Cor do nevoeiro de dia.
GLfloat		fogNightColor[]			=	{0.015, 0.015, 0.015, 1.0};		//Cor do nevoeiro de noite.
GLfloat		fogDensity				=	0.001;							//Densidade.
GLfloat		fogDensityIncrement		=	0.000001;						//Ao longo do tempo vai aumentando o fumo.
GLfloat		fogColorIncrement		=	0.0005;							//Incremento do nevoeiro (enquanto o cigarro arde)
bool		fogMode					=	false;							//Nevoeiro ativo ou nao.

// Desenha as particulas
void DrawParticles (void)
{
   for (int i = 1; i < particleSystem.getNumOfParticles(); i++)
   {
      glPushMatrix();
      // set color and fade value (alpha) of current particle
      glColor4f(particleSystem.getR(i), particleSystem.getG(i), particleSystem.getB(i), particleSystem.getAlpha(i));
      // move the current particle to its new position
      glTranslatef(particleSystem.getXPos(i), particleSystem.getYPos(i), particleSystem.getZPos(i));
      // rotate the particle (this is proof of concept for when proper smoke texture is added)
      glRotatef (particleSystem.getDirection(i) - 90, 0, 0, 1);
      // scale the wurrent particle (only used for smoke)
      glScalef(particleSystem.getScale(i), particleSystem.getScale(i), particleSystem.getScale(i));
      glEnable (GL_BLEND);//Ativa transaprencia.
	  drawSphereColor(sphereQuadric, 0.7);//Desenha esfera.
      glPopMatrix();
   }
}

//		Funcao para inicializar definicoes de nevoeiro.
void defineFog(){
	GLfloat end = 500;	//Fim do nevoeiro.

	if(scene==0){//Nevoeiro para a cena original
		if(!fogMode){							//Caso em que o modo de nevoeiro esta off.
			glDisable(GL_FOG);					//Desativa nevoeiro.
			return;
		}
		if(ceilingLightMode){
			glFogfv(GL_FOG_COLOR, fogDayColor);		//Define cor do nevoeiro para a mais clara.
		}else{
			glFogfv(GL_FOG_COLOR, fogColor);		//Define cor do nevoeiro.
		}
		glFogf (GL_FOG_DENSITY, fogDensity);		//Definimos a densidade, em GL_LINEAR nao se aplica.
	}else{//Nevoeiro na cena alternativa
		glFogfv(GL_FOG_COLOR, fogColor);		//Define cor do nevoeiro para a mais clara.
		end = 5000;
		glFogf (GL_FOG_DENSITY, 0.00055);		//Definimos a densidade, em GL_LINEAR nao se aplica.
	}
	

	glEnable(GL_FOG);						//Ativa nevoeiro.

	glFogi (GL_FOG_MODE, GL_EXP2);			//Temos que usar este modo de forma a controlarmos a densidade (dando o efeito do fumo a aumentar enquanto o cigarro queima)
	//glFogi(GL_FOG_MODE, GL_LINEAR);		//Define modo do nevoeiro.	GL_LINEAR - é o que costuma apresentar melhor detalhe.
	glFogf(GL_FOG_START, 0);				//Distancia a que se começa a ver o nevoeiro.
	glFogf(GL_FOG_END, end);				//Distancia ate ao fim do nevoeiro.
	glHint(GL_FOG_HINT, GL_DONT_CARE);		//Fog hint value. Dont care - deixa a decisao ao opengl se usa por vertex ou por pixel.
}

//		Funcao para inicializar as luzes.
void defineLights()
{
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColorLight);	//Define e ativa o modelo de ambiente global.

	// Luz teto da sala:
	glLightfv(GL_LIGHT0,	GL_POSITION,		ceilingLightPos );   
	glLightfv(GL_LIGHT0,	GL_AMBIENT,			ceilingLightColor );   
	glLightfv(GL_LIGHT0,	GL_DIFFUSE,			ceilingLightColDif );  
	glLightfv(GL_LIGHT0,	GL_SPECULAR,		ceilingLightColEsp ); 
	glLightf (GL_LIGHT0,	GL_CONSTANT_ATTENUATION, ceilingLightAttCon);
	glLightf (GL_LIGHT0,	GL_LINEAR_ATTENUATION,   ceilingLightAttLin) ;         
	glLightf (GL_LIGHT0,	GL_QUADRATIC_ATTENUATION,ceilingLightAttQua) ;

	// FlashLight
	glLightfv(GL_LIGHT1,	GL_POSITION,		flashLightPos);
	glLightfv(GL_LIGHT1,	GL_SPOT_DIRECTION,	flashLightDir);
	glLightf (GL_LIGHT1,	GL_SPOT_EXPONENT ,	flashLightExp);
	glLightf (GL_LIGHT1,	GL_SPOT_CUTOFF,		flashLightCut);
	glLightfv(GL_LIGHT1,	GL_DIFFUSE,			flashLightColDif);   
	glLightfv(GL_LIGHT1,	GL_SPECULAR,		flashLightColEsp);
}

//		Funcao que inicializa uma textura. GL_CLAMP e GL_LINEAR (mode=0), GL_REPEAT e GL_LINEAR (mode=1)
void initTexture(GLint textureArrayPosition, char* path, int mode){
	glGenTextures(1, &textures[textureArrayPosition]);  //Generate texture names
	glBindTexture(GL_TEXTURE_2D, textures[textureArrayPosition]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//Filtragem linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//Filtragem linear mipmap
	if(mode==0){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}else{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	imag.LoadBmpFile(path);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
}

//		Funcao para inicializar as texturas.
void createTextures()
{
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//GL_DECAL ignores the primary color and just shows the texel. (ou seja nao da usar iluminacao nas texturas)
																//GL_MODULE multiplies primary color and texel and shows the result. (neste ja da pra usar)
	//	Texturas da parede em tijolo vermelho:
	//	Room Walls:
	initTexture(12, "Textures/RedBrickWall/wall_texture.bmp", 0);
	initTexture(13, "Textures/RedBrickWall/wall_upper_door_texture_2.bmp", 0);
	initTexture(23, "Textures/RedBrickWall/wall_texture_left_window.bmp", 0);
	initTexture(24, "Textures/RedBrickWall/wall_texture_right_window.bmp", 0);
	initTexture(25, "Textures/RedBrickWall/wall_upper_window_texture.bmp", 0);
	initTexture(26, "Textures/RedBrickWall/wall_down_window_texture.bmp", 0);
	// Janela:
	initTexture(17, "Textures/Window/bottom_window.bmp", 0);
	initTexture(18, "Textures/Window/upper_window.bmp", 0);
	initTexture(27, "Textures/Window/right_window.bmp", 0);
	initTexture(28, "Textures/Window/left_window.bmp", 0);
	initTexture(29, "Textures/Window/window_midle_up_down.bmp", 0);
	initTexture(30, "Textures/Window/window_midle_right_left.bmp", 0);

	//	Game Board Texture:
	initTexture(0, "Textures/board_game_noughts and crosses_texture.bmp", 0);
	//	Game Board Lateral Texture:
	initTexture(1, "Textures/lateral_board.bmp", 0);
	//	Table Texture:
	initTexture(2, "Textures/table_wood_texture.bmp", 0);
	//	Room Floor/Walls Texture:
	initTexture(3, "Textures/natural_wood_texture.bmp", 0);
	//	Room Wall Upper Door Texture:
	initTexture(4, "Textures/wall_upper_door_texture.bmp", 0);
	//	Room Door Texture:
	initTexture(5, "Textures/door_texture_2.bmp", 0);
	//	Room Carpet Texture:
	initTexture(6, "Textures/carpet_texture.bmp", 0);
	//	Clock Texture:
	initTexture(7, "Textures/wood_clock_texture.bmp", 0);
	//	Back Clock Texture:
	initTexture(8, "Textures/back_clock_texture.bmp", 0);
	//	Picture Texture:
	initTexture(9, "Textures/picture_texture.bmp", 0);
	//	Earth Texture:
	initTexture(10, "Textures/earth_texture.bmp", 0);
	//	Abajur Texture:
	initTexture(11, "Textures/abajur_texture.bmp", 0);
	
	//	Ashtray Superior Face Texture:
	initTexture(14, "Textures/ashtray_superior_texture.bmp", 0);
	// Cigarro Texture:
	initTexture(15, "Textures/cigarette_texture.bmp", 0);
	// Cigarro frente a arder Texture:
	initTexture(16, "Textures/cigar_fire_texture.bmp", 0);
	//	Brick Wall Texture:
	initTexture(19, "Textures/brick_wall_texture.bmp", 0);
	//	Brick Wall Upper Door Texture:
	initTexture(20, "Textures/brick_wall_upper_door_texture.bmp", 0);
	//	Quadro com instrucoes Texture:
	initTexture(21, "Textures/green_board_texture.bmp", 0);
	//	Ball Texture
	initTexture(22, "Textures/basketball_texture.bmp", 0);
	//	Filtro Cigarro
	initTexture(31, "Textures/filter_cigar_texture.bmp", 0);
	//	Light switcher
	initTexture(32, "Textures/light_button_texture.bmp", 0);
	//	Trash
	initTexture(33, "Textures/trash_texture.bmp", 0);
	//	Iglo
	initTexture(34, "Textures/igloo.bmp", 0);
	//	Iglo Floor/Chao do cenario neve
	initTexture(35, "Textures/igloo_floor_texture.bmp", 1);
	//	Porta do iglo
	initTexture(36, "Textures/door_igloo.bmp", 0);
	//	Cubo de gelo
	initTexture(37, "Textures/ice_cube.bmp", 0);
	//	Porta lado de fora igloo
	initTexture(38, "Textures/outside_igloo_door.bmp", 0);
}

// Funcao que atualiza a posicao e direcao da lanterna.
void updateFlashLightPosition(){
	flashLightPos[0] = obsP[0];
	flashLightPos[1] = obsP[1];
	flashLightPos[2] = obsP[2];
	flashLightDir[0] = obsPfin[0]-obsP[0];
	flashLightDir[1] = obsPfin[1]-obsP[1];
	flashLightDir[2] = obsPfin[2]-obsP[2];
}

//	Funcao colisoes camera no cenario alternativo.
void alternativeColisionsCamera(){
	if(insideIgloo){//Dentro do iglo
		//XX
		if(obsP[0]>160)
			obsP[0]=160;
		else if(obsP[0]<-160)
			obsP[0]=-160;
		//YY
		if(obsP[1]<-19){
			obsP[1]=-19;
			obsPfin[1]	=	obsPfin[1] + 5;
		}
		else if(obsP[1]>105){
			obsP[1]=105;
			obsPfin[1]	=	obsPfin[1] - 5;
		}
		//ZZ
		if(obsP[2]>160)
			obsP[2]=160;
		else if(obsP[2]<-155)
			obsP[2]=-155;
	}
	else{//Fora do iglo
		//XX
		if(obsP[0]>1500)
			obsP[0]=1500;
		else if(obsP[0]<-1500)
			obsP[0]=-1500;
		//YY
		if(obsP[1]<-19){
			obsP[1]=-19;
			obsPfin[1]	=	obsPfin[1] + 5;
		}
		else if(obsP[1]>105){
			obsP[1]=105;
			obsPfin[1]	=	obsPfin[1] - 5;
		}
		//ZZ
		if(obsP[2]<-1500)
			obsP[2]=-1500;
		else if(obsP[2]>-355)
			obsP[2]=-355;
	}
}

//	Funcao colisoes camera.
void colisionsCamera(){
	if(scene==1){//Se for a cena alternativa chama a outra funcao.
		alternativeColisionsCamera();
		return;
	}

	//XX
	if(obsP[0]>230)
		obsP[0]=230;
	else if(obsP[0]<-230)
		obsP[0]=-230;
	//YY
	if(obsP[1]<-50){
		obsP[1]=-50;
		obsPfin[1]	=	obsPfin[1] + 5;
	}
	else if(obsP[1]>140){
		obsP[1]=140;
		obsPfin[1]	=	obsPfin[1] - 5;
	}
	//ZZ
	if(obsP[2]>230)
		obsP[2]=230;
	else if(obsP[2]<-230)
		obsP[2]=-230;
}

// Funcao que atualiza alguns parametros do observador.
void updateVisao(){
	colisionsCamera();
	obsPfin[0] =obsP[0]+rVisao*cos(aVisao);
	obsPfin[2] =obsP[2]-rVisao*sin(aVisao);
	updateFlashLightPosition();
	glutPostRedisplay();	
}

//		Funcao que trata da tecla E
void interactionKeyHandler(void)
{
	if((GLint)obsP[0]<90.0 && (GLint)obsP[0]>-90.0 && (GLint)obsP[2]<105.0 && (GLint)obsP[2]>-105.0){
			turnGameOn();
			PlaySound(STARTGAMESOUND, NULL, SND_ASYNC|SND_FILENAME);
			return;
	}

	if(scene == 0){//Cenario original
		//	So entra no jogo do galo se estiver perto o suficiente do tabuleiro.
		if((GLint)obsP[0]<-150.0 && (GLint)obsP[0]>-300.0 && (GLint)obsP[2]<50 && (GLint)obsP[2]>-50.0){//Abrir/Fechar janela.
			if(windowRotationAngle == 90.0){
				windowRotationIncrement = -1.1;
				PlaySound(CLOSEWINDOWSOUND, NULL, SND_ASYNC|SND_FILENAME);
			}
			else if(windowRotationAngle == 0.0){
				windowRotationIncrement = 1.1;
				PlaySound(OPENWINDOWSOUND, NULL, SND_ASYNC|SND_FILENAME);
			}
		}else if((GLint)obsP[0]>-50 && (GLint)obsP[0]<100.0 && (GLint)obsP[2]<-130 && (GLint)obsP[2]>-270.0){//Ligar/Desligar luz teto.
			ceilingLightMode=!ceilingLightMode;
			PlaySound(SWITCHLIGHTSOUND, NULL, SND_ASYNC|SND_FILENAME);
		}
	}else{//Cenario alternativo (iglo)
		if((GLint)obsP[0]<80.0 && (GLint)obsP[0]>-80.0 && (GLint)obsP[2]>-240.0 && (GLint)obsP[2]<-115.0){
			insideIgloo = false;
			obsP[2] = obsP[2] -250;
			PlaySound(WINDSOUND, NULL, SND_ASYNC|SND_FILENAME);
		}
		else if((GLint)obsP[0]<80.0 && (GLint)obsP[0]>-80.0 && (GLint)obsP[2]>-500.0 && (GLint)obsP[2]<-300.0){
			insideIgloo = true;
			obsP[2] = obsP[2] +250;
			PlaySound(NULL, 0, 0);
		}
	}
}

//		Funcao que reinicia a posicao do observador e para onde esta a olhar.
void restartCameraPosition()
{
	obsP[0]= rVisao*cos(aVisao);
	obsP[1]= 70;
	obsP[2]=rVisao*sin(aVisao) + 50.0;
	obsPfin[0] = obsPfin[2] = 0;
	obsPfin[1] = 70;
	updateVisao();
}

//		Funcao para detetar acoes do teclado fora do modo de jogo do galo.
void keyboardHandlerWalkMode(unsigned char key)
{
	GLfloat incZoom = 4;
	switch(key){
		//		Mudar de cenario:
		case 'n':
		case 'N':
			insideIgloo=true;
			restartCameraPosition();	
			scene = (scene + 1) % 2;
			if(scene==0)
				skyBox->Init(1, skyboxSize, 0);
			else{
				skyBox->Init(2, skyboxAlternativeSize, skyboxAlternativeSize/5);
			}
			PlaySound(NULL, 0, 0);
			break;
		//		Interecao com objetos/cenario.
		case 'e':
		case 'E':
			interactionKeyHandler();
			break;
		//		Ligar desligar HUD:
		case 'H':
		case 'h':
			hudMode		=   !hudMode;
			break;
		//		Decrease Zoom:
		case '-':
			angZoom=angZoom+incZoom;
			if (angZoom>1000)
				angZoom=1000;
			glutPostRedisplay();
			break;
		//		Increase Zoom:
		case '+':
			angZoom=angZoom-incZoom;
			if (angZoom<10)
				angZoom=10;
			glutPostRedisplay();
			break;
		//		Movimentacao:
		case 'a':
		case 'A':
			aVisao = (aVisao + 0.17);
			break;
		case 'w':
		case 'W':
			obsP[0] = obsP[0] + 7*(incVisao*cos(aVisao)); 
			obsP[2] = obsP[2] - 7*(incVisao*sin(aVisao)); 
			break;
		case 's':
		case 'S':
			obsP[0] = obsP[0] - 7*(incVisao*cos(aVisao)); 
			obsP[2] = obsP[2] + 7*(incVisao*sin(aVisao)); 
			break;
		case 'd':
		case 'D':
			aVisao = (aVisao - 0.17);
			break;
		//		Ajoelhar.
		case 'c':
		case 'C':
			crouch = !crouch;
			if(crouch){
				obsP[1]		=	obsP[1] - 80;
				obsPfin[1]	=	obsPfin[1] - 80;
			}else{
				obsP[1]		=	obsP[1] + 80;
				obsPfin[1]	=	obsPfin[1] + 80;
			}
			break;
		//		Diminuir y do observador.
		case '0':
			obsP[1]		=	obsP[1] - 5;
			obsPfin[1]	=	obsPfin[1] - 5;
			break;
		//		Aumentar y do observador.
		case '\'':
			obsP[1]		=	obsP[1] + 5;
			obsPfin[1]	=	obsPfin[1] + 5;
			break;
		default:
			return;
	}	
}

//		Funcao para detetar acoes do teclado.
void keyboardHandler(unsigned char key, int x, int y)
{
	// Teclas comuns aos diferentes modos de jogo.
	switch(key){
		//		Ligar/Desligar eixos.
		case 'x':
		case 'X':
			axisMode = !axisMode;
			return;
		//		Ligar/Desligar particulas.
		case 'p':
		case 'P':
			smokeParticles = !smokeParticles;
			return;
		//		Ligar/Desligar nevoeiro e arder ou nao o cigarro.
		case 'm':
		case 'M':
			if((GLint)obsP[0]<80.0 && (GLint)obsP[0]>-80.0 && (GLint)obsP[2]<95.0 && (GLint)obsP[2]>-95.0){
				if(fogMode){
					burnMode = false;
					particleSystem.modifyRenewParticle(false);
				}
				else{
					burnMode = true;
					particleSystem.modifyRenewParticle(true);
				}
				fogDensity = 0.001;
				fogMode = !fogMode;
			}
			return;
		//		Ligar/Desligar lanterna.
		case 'f':
		case 'F':
			if(scene==0){
				flashLightMode=!flashLightMode;
			}
			return;
		/*//		Ligar/Desligar luz teto.
		case 't':
		case 'T':
			ceilingLightMode=!ceilingLightMode;
			return;*/
		//		Alterar malha de poligonos.
		case '\\':
			 changeMeshFactor();
			return;
		//		Ativar/Desativar malha de poligonos.
		case '|':
			meshPolygonsState();
			return;
		//		Ativar/Desativar texto fps.
		case '<':
			showFPS = !showFPS;
			return;
		//		Altera a textura da parede para madeira.
		case '!':
			wallTextureMode = 0;
			return;
		//		Altera a textura da parede para tijolo vermelho.
		case '\"':
			wallTextureMode = 1;
			return;
		//		Altera a textura da parede para tijolo branco.
		case '#':
			wallTextureMode = 2;
			return;
		//		Esc - Sair do jogo.
		case 27:
			exit(0);
			break;
	}

	if((GLint)  gameMode() == 0){	//Se estiver no modo jogo do galo vai para outra funcao que trata das teclas especificas para o jogo.
		NoughtsAndCrossesKeyboardHandler(key);
		glutPostRedisplay();
		return;
	}

	keyboardHandlerWalkMode(key);		//Chama a funcao que trata das teclas for do modo de jogo do galo.
}

//	Funcao para detetar acoes do teclado nao ascii.
void keyboardNotASCIIHandler(int key, int x, int y)
{
	switch(key){//Teclas permitidas dentro e fora do modo jogo do galo.
		//		Ativar/desativar fullscreen.
		case GLUT_KEY_F1:
			fullScreen = !fullScreen;
			if(fullScreen)
					glutFullScreen();			//Requests that the current window be made full screen.
			else{
				glutReshapeWindow(800,800);		//Requests a change to the size of the current window.
				glutPositionWindow(700,150);	//Requests a change to the position of the current window.
			}
			glutPostRedisplay();	
			return;
	}

	if((GLint) gameMode() == 0)				//Se esta em modo jogo do galo entao sai.
		return;

	switch(key){//Estas keys so sao permitidas fora do modo jogo do galo.
		//	Sobe no y:
		case GLUT_KEY_UP:
			obsPfin[1] = (obsPfin[1] + 4.2);
			break;
		//	Desce no y:
		case GLUT_KEY_DOWN:
			obsPfin[1] = (obsPfin[1] - 4.2);
			break;
		//	Igual 'a tecla A
		case GLUT_KEY_LEFT:
			aVisao = (aVisao + 0.17);
			break;
		//	Igual 'a tecla D
		case GLUT_KEY_RIGHT:
			aVisao = (aVisao - 0.17);
			break;
	}	
}

//	Função que verifica quais luzes ativar.
void checkLights(void)
{
	//	Ativa ou desativa a luz do teto.
	if(ceilingLightMode && scene == 0)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);

	//	Ativa ou desativa a luz da lanterna.
	if(flashLightMode && scene == 0)
		glEnable(GL_LIGHT1);
	else
		glDisable(GL_LIGHT1);
}

//	Funcao que desenha os objetos opacos com textura.
void displayOpaqueWithTexture(void)
{
	glEnable(GL_TEXTURE_2D);	//Ativa texturas.

	//Lixo dentro do caixote.
	glPushMatrix();
		glTranslatef(-200, -42, -200);
		glRotatef(-90, 1,0,0);
		drawCircleTexture(15, textures[33]);
	glPopMatrix();

	//Parte superior do cinzeiro.
	glPushMatrix();
		glTranslatef(-35, -2, 15);
		glTranslatef(0, 3.7, 0);
		glRotatef(-90, 1,0,0);
		drawCircleTexture(5, textures[14]);
	glPopMatrix();

	//Cigarro
	//Filtro
	glPushMatrix();
		glTranslatef(-36, -2, 15);
		glTranslatef(8, 4.1, 0);
		glRotatef(-90, 0,1,0);
		glRotatef(180,0,0,1);
		drawCylinder(cylinderQuadric, textures[31], 0.3, 2);
	glPopMatrix();
	//Resto do cigarro
	glPushMatrix();
		glTranslatef(-38, -2, 15);
		glTranslatef(8, 4.1, 0);
		glRotatef(-90, 0,1,0);
		glRotatef(180,0,0,1);
		drawCylinder(cylinderQuadric, textures[15], 0.3, 5-burningState);
	glPopMatrix();
	//Cinza cigarro
	glPushMatrix();
		glTranslatef(-36+burningState, -2, 15);
		glTranslatef(1, 4.1, 0);
		glRotatef(-90, 0,1,0);
		drawCircleTexture(0.3, textures[16]);
	glPopMatrix();

	drawFrame(textures[8], textures[9]);			//Desenha o quadro com foto.

	glPushMatrix();
		glRotatef(90, 0, 1, 0);							//Roda 90 graus nos yy.
		drawFrame(textures[8], textures[21]);			//Desenha o quadro com as instrucoes.
	glPopMatrix();

	drawTable(textures[2]);							//Mesa.
	drawGameBoard(textures[0], textures[1]);		//Tabuleiro do jogo do galo.


	//A textura da janela da sala muda consoante dia/noite
	GLuint wallTexture, upperWallTexture;

	//Desenha a sala consoante a textura pretendida (madeira nas paredes ou tijolos)
	if(wallTextureMode == 0){
		wallTexture = textures[3];
		upperWallTexture = textures[4];
	}
	else if(wallTextureMode == 1){
		wallTexture = textures[12];
		upperWallTexture = textures[13];
	}
	else{
		wallTexture = textures[19];
		upperWallTexture = textures[20];
	}

	drawRoom(textures[3], wallTexture, upperWallTexture, textures[5], textures[28], textures[23], textures[24], textures[25], textures[26], textures[27],textures[17], textures[18], textures[29],textures[30], windowRotationAngle);
	
	drawCarpet(textures[6]);						//Desenha carpete.

	//Desenha o globo.
	glPushMatrix();
		glTranslatef(45, 13.6, -18);
		glRotatef(-60, 1,0,0);
		drawSphere(sphereQuadric, textures[10], 8);
	glPopMatrix();				

	//Desenha bola de basket.
	glPushMatrix();
		glTranslatef(210, -67, -220);
		glRotatef(-60, 1,0,0);
		drawSphere(sphereQuadric, textures[22], 13);
	glPopMatrix();

	//Desenha bola de basket na rua.
	glPushMatrix();
		glTranslatef(basketBallPosition[0], basketBallPosition[1], basketBallPosition[2]);
		glRotatef(rotationAngle, 1, 0 ,0);
		drawSphere(sphereQuadric, textures[22], 13);
	glPopMatrix();

	//Light switcher
	glPushMatrix();
		glTranslatef(50, 20, -249.8);
		drawQuads(textures[32],10,10);
	glPopMatrix();
}

//	Funcao que desenha os objetos opacos e sem textura.
void displayOpaque(void)
{
	glDisable(GL_TEXTURE_2D);		//Desativa modo texturas.
	glDisable(GL_BLEND);			//Desativa modo transparencias.

	// Alteramos definicoes de especularidade dos objectos.
	glMaterialfv(GL_FRONT,GL_SPECULAR, matEspecular); 
	glMateriali (GL_FRONT,GL_SHININESS,especMaterial);

	//Desenha o suporte para o abajur.
	glPushMatrix();
		glColor4f(BLACKLESS);
		glTranslatef(0, 164.5, 0);
		glRotatef(90, 1,0,0);
		drawCircle(5);
	glPopMatrix();
	glPushMatrix();
		glColor4f(BLACK);
		glTranslatef(0, 169.5, 0);
		glRotatef(90, 1,0,0);
		drawCylinderColor(cylinderQuadric, 5, 5, 5);
	glPopMatrix();

	//Desenha caixote do lixo:
	glPushMatrix();//Prte de fora
		glColor4f(BLACKLESS);
		glTranslatef(-200, -80, -200);
		glRotatef(-90, 1,0,0);
		drawCylinderColor(cylinderQuadric, 14, 16, 50);
	glPopMatrix();
	glFrontFace(GL_CW);		
	glPushMatrix();//Parte de dentro
		glColor4f(BLACKLESS);
		glTranslatef(-200, -80, -200);
		glRotatef(-90, 1,0,0);
		drawCylinderColor(cylinderQuadric, 14, 16, 50);
	glPopMatrix();
	glFrontFace(GL_CCW);		

	//Desenha o suporte para globo.
	glPushMatrix();
		glColor4f(BLACKLESS);
		glTranslatef(45, -1.00, -18);
		glRotatef(-90, 1,0,0);
		drawCylinderColor(cylinderQuadric, 3.5, 2, 7);
	glPopMatrix();
	
	//Desenha o suporte do cinzeiro
	glPushMatrix();
		glTranslatef(-35, -1, 15);
		glColor4f(REDDARK);
		glRotatef(-90, 1,0,0);
		drawCylinderColor(cylinderQuadric, 5.8, 4.93, 3);
	glPopMatrix();

	//Filtro do cigarro
	glPushMatrix();
		glColor4f(WHITE);
		glTranslatef(-36, -2, 15);
		glTranslatef(8, 4.1, 0);
		glRotatef(90, 0,1,0);
		drawCircle(0.3);
	glPopMatrix();

	drawBlackAxis();				//  Desenha os suportes do abajur.

	NoughtsAndCrossesMain();		//	Pecas do jogo do galo.

	glMaterialfv(GL_FRONT,GL_SPECULAR, matDefaultEspecular);	//Desativa a especular

	if(axisMode)
		drawAxis();					//	Desenha os eixos xx,yy,zz.
	drawClock(textures[7], textures[8], (GLfloat) -(hour*(360/12)),(GLfloat)  -(minute*(360/60)),(GLfloat)  -(second*(360/60)));		//Relogio.
}

//	Funcao que desenha os objetos transparentes.
void displayTransparent(void)
{
	glEnable(GL_BLEND);									//	Ativa transparencia.
	glDisable(GL_TEXTURE_2D);							//	Desativa  texturas.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//	Define parametros de transformacao da cor consoante a transparencia.

	//	Altera especular dos objetos.
	glMaterialfv(GL_FRONT,GL_SPECULAR, matEspecular);
	glMateriali (GL_FRONT,GL_SHININESS,especMaterial);

	//		JANELA VIDRO:
	glPushMatrix();
		glColor4f(WINDOWGLASS); 
		glTranslatef(-250, 59, 3+39.5);			// Faz translate para o local pretendido.
		glRotatef(windowRotationAngle, 0, 1, 0);	//ABRIR JANELA
		glTranslatef(0, 0, -39.5);				// Faz translate para a origem.
		glRotatef(90, 0, 1, 0);					// Roda o objecto 90 graus em torno dos yy.
		glTranslatef(-39.5, -51, 0);			// Faz translate para a origem.
		drawQuads(textures[0],79, 102);
	glPopMatrix();

	// Desenha o copo com agua:
	glPushMatrix();
		glTranslatef(-45, -1, -20);			//Translate para o local pretendido.
		drawGlassWater(cylinderQuadric);
	glPopMatrix();

	glMaterialfv(GL_FRONT,GL_SPECULAR, matDefaultEspecular);	// Muda a cor especular para 0 (ou seja desliga).

	if(burnMode){		//Se o cigarro esta aceso.
		if(smokeParticles){							//	Sistema de particulas.
			glPushMatrix();
				glTranslatef(-35+burningState, 2.6, 15);
				particleSystem.updateParticles();
				DrawParticles();
			glPopMatrix();
		}else{										//Desenhar fumo estatico.
			glPushMatrix();
				glTranslatef(-35+burningState, 2.7, 15);
				drawSmoke(sphereQuadric);  
			glPopMatrix();
		}
	}

	glEnable(GL_TEXTURE_2D);			//Ativa texturas.

	//Parte de dentro do abajur
	glFrontFace(GL_CW);					// Alteramos a face que sera visivel.
	glPushMatrix();
		glTranslatef(0, 110, 0);
		glRotatef(-90, 1,0,0);
		drawCylinder(cylinderQuadric, textures[11], 30, 20);
	glPopMatrix();
	glFrontFace(GL_CCW);				// Voltamos ao default.

	glDisable(GL_TEXTURE_2D);			//Desativa texturas.
	glMaterialfv(GL_FRONT,GL_SPECULAR, matEspecular);	//Ativa a especular.

	//Lampada.
	glPushMatrix();
		if(ceilingLightMode)
			glColor4f(LAMP_ON);
		else
			glColor4f(LAMP_OFF);
		glTranslatef(0, 120.5, 0);
		drawSphereColor(sphereQuadric, 4);		
	glPopMatrix();
	
	glMaterialfv(GL_FRONT,GL_SPECULAR, matDefaultEspecular);	//Desativa a especular.
	glEnable(GL_TEXTURE_2D);									//Ativa texturas.
	
	//Parte de fora do abujur
	glPushMatrix();
		glTranslatef(0, 110, 0);
		glRotatef(-90, 1,0,0);
		drawCylinder(cylinderQuadric, textures[11], 30, 20);
	glPopMatrix();
}

//	Funcao que trata de definir a camera.
void cameraHandler()
{	
	glLoadIdentity();
		glViewport(0,0,wScreen,hScreen);
	if((GLint) gameMode() == 1){//Fora do jogo do galo
		glMatrixMode(GL_PROJECTION);						//	Serve para especificar qual matriz deve ser usada.
		glLoadIdentity();									//	Resets the matrix back to its default state.
		gluPerspective(angZoom, wScreen/hScreen, 0.1, 3*zC);/*  Estabelece os parâmetros da Projeção Perspectiva, atualizando a matriz de projeção perspectiva. 
Parâmetros- fovy: ângulo na direção y (usada para determinar a "altura" do volume de visualização); aspect: razão de aspecto que determina a área de visualização na direção x, e seu valor é a razão em x (largura) e y (altura)
zNear: é a distância do observador até o plano de corte mais próximo (em z); zFar: é a distância do observador até o plano de corte mais afastado (em z). */
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(obsP[0], obsP[1], obsP[2], obsPfin[0],obsPfin[1],obsPfin[2], 0, 1, 0);
	}else{//Dentro do jogo do galo
		if(cameraMode()==1){
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45, wScreen/hScreen, 0.1, 3*zC);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(0, 60, 20, 0,1,0, 0, 0, -1);
		}else{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45, wScreen/hScreen, 0.1, 3*zC);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(20, 25, 35, 0,1,0, 0, 1, 0);
		}
	}
}

//	Funcao que faz o HUD (para apresentacao das FPS)
void displayHUD()
{
	glLoadIdentity();
		glViewport(wScreen-150,hScreen-35,10,10);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//	Funcao que faz o HUD (para apresentacao das FPS)
void displayInteractionHUD()
{
	if(!hudMode)//HUD desligado.
		return;

	glLoadIdentity();
		glViewport(wScreen/2 - 180,hScreen/2 - 100,100,100);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glColor4f(BLACK);
	//	Se estiver proximo o suficiente do tabuleiro, aparece uma hint ao jogador.
	if(gameMode()!=0 && (GLint)obsP[0]<80.0 && (GLint)obsP[0]>-80.0 && (GLint)obsP[2]<95.0 && (GLint)obsP[2]>-95.0){
		drawText("[E] - Interact With Board Game",0,0,0);
		if(scene==0){
			drawText("[M] - Interact With Cigarrete",0,-1,0);
		}
		return;
	}
	
	if(scene==0){//Cenario original
		if((GLint)obsP[0]<-150.0 && (GLint)obsP[0]>-300.0 && (GLint)obsP[2]<50 && (GLint)obsP[2]>-50.0){
			drawText("[E] - Interact With Window",0,0,0);
		}else if((GLint)obsP[0]>-50 && (GLint)obsP[0]<100.0 && (GLint)obsP[2]<-130 && (GLint)obsP[2]>-270.0){
			drawText("[E] - Switch Light On/Off",0,0,0);
		}
	}else{//Cenario alternativo iglo
		if((GLint)obsP[0]<80.0 && (GLint)obsP[0]>-80.0 && (GLint)obsP[2]>-400.0 && (GLint)obsP[2]<-115.0){
			drawText("[E] - Interact With Door",0,0,0);
		}
	}
}

//	Funcao que desenha o cenario alternativo.
void displayAlternativeScene(void)
{
	drawIgloo(cylinderQuadric, sphereQuadric, textures[34], textures[35], textures[36], textures[35], textures[38]); //Desenha o iglo.

	//Desenha o chao do cenario. (neve) 5000x5000
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, textures[35]);
	glTranslatef(-2500, -22, 2500);
	glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glTexCoord2f(0, 0);			glVertex3d(0,0,0);				// P1
		glTexCoord2f(1, 0);			glVertex3d(5000,0,0);			// P2
		glTexCoord2f(1, 1);			glVertex3d(5000,0,-5000);		// P3
		glTexCoord2f(0, 1);			glVertex3d(0,0,-5000);			// P4
	glEnd();
	glPopMatrix();

	drawGameBoard(textures[0], textures[1]);				//Tabuleiro do jogo do galo.
	drawIceCube(textures[37]);								//Mesa de gelo.

	drawSnowMan(sphereQuadric, cylinderQuadric, textures[35], colorSnowMan);	//Boneco de neve.

	glDisable(GL_TEXTURE_2D);
	NoughtsAndCrossesMain();		//	Pecas do jogo do galo.

	if(axisMode)
		drawAxis();					//	Desenha os eixos xx,yy,zz.
}

//	Função callback chamada para fazer o desenho
void displayMain(void)
{
	int start, end;
	start = glutGet(GLUT_ELAPSED_TIME);  //Guarda o tempo atual para o calculo dos fps.

	//	Determiar horas, minutos segundos
	time_t timer = time(0);
	current_time = localtime(&timer);
	hour = current_time->tm_hour;
	minute = current_time->tm_min;
	second = current_time->tm_sec;
    
	updateVisao();					
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);		//	Limpa a janela de visualização com a cor de fundo especificada.							
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_LINE_SMOOTH);
	cameraHandler();			// Define a camera.

	//Draw skybox
	if(scene==1){
		glPushMatrix();
		glTranslatef(0,500,0);
		skyBox->draw(); 
		glPopMatrix();
	}else{
		skyBox->draw();
	}

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_CULL_FACE);					//	Faces visiveis.
	glCullFace(GL_BACK);					//	Define-se a face que nao sera visivel.

	//	Ativa ou desativa o nevoeiro.
	defineFog();

	//	Ativa o modo de iluminacao.
	defineLights();
	glEnable(GL_LIGHTING);

	checkLights();
		
	if(scene==1)
		displayAlternativeScene();	//Cenario alternativo.
	else{
		displayOpaque();			//Chama a funcao que trata dos objetos opacos e sem textura.
		displayOpaqueWithTexture();	//Chama a funcao que trata dos objetos opacos com textura.
		displayTransparent();		//Chama a funcao que trata dos objetos transparentes.
	}

	displayInteractionHUD();		//Serve para criar um viewport onde vai ser apresentava informacao de interacao com o cenario.
	
	// Calcular fps
	GLint fps;
	glFinish();

	if (showFPS) {
		end = glutGet(GLUT_ELAPSED_TIME);
		if(end-start == 0)
			fps= 1000;
		else
			fps = (GLint) 1000/(end-start);
	
		displayHUD();		//Faz o viewport para de seguida apresentarmos as FPS
		char fpsText[20]="";
		sprintf (fpsText, "%d FPS  (%d ms)", fps, end-start);
		glColor4f(BLACK);
		drawText(fpsText,0,0,0);
	}

	glutPostRedisplay();									// Ao mudar-se a camera e' necessario atualizar para que as pecas fiquem reajustadas.
	glFlush();												//Faz com que qualquer comando OpenGL não executado seja executado.
	glutSwapBuffers();										//Como estamos a usar double-buffer precisamos de trocar os buffers para apresentar as alteracoes.
}

//		Função que inicializa parâmetros de rendering.
void init (void)
{
	glClearColor(BLACK);					//	Define a cor de fundo da janela de visualizacao como branca.
	glEnable(GL_DEPTH_TEST);				//	Profundidade
	glShadeModel(GL_SMOOTH);				//	Por default ele ja esta' a SMOOTH que permite interpolocao de cor com base na cor dos vertices, enquanto que FLAT nao permite.
	createTextures();						//	Carregar texturas
	ResetScore();							//	Mete o resultado do jogo do galo a 0-0

	hudMode					=	true;		//Liga o HUD interativo.
	flashLightMode			=	false;		//Desliga a lanterna.
	ceilingLightMode		=	false;		//Desliga a luz do teto.
	crouch					=	false;		//Desliga o modo ajoelhado.
	fogMode					=	false;		//Desativa o nevoeiro (fumarada).
	nightMode				=	false;		//Desativa o modo noite.
	burnMode				=	false;		//Desativa o cigarro a arder.
	axisMode				=	false;		//Desativa o desenho dos eixos.
	wallTextureMode			=	1;			//Textura da sala a ser usada inicialmente.

	//Criar SkyBox
	skyBox = new SkyBox();
	if(scene==0)//Cenario original
		skyBox->Init(1, skyboxSize, 0);
	else//Cenario alternativo
		skyBox->Init(2, skyboxAlternativeSize, skyboxAlternativeSize/5);				

	//	Sistema de particulas
	particleSystem.createParticles();

	//	Dados relativos ao cilindro
	cylinderQuadric=gluNewQuadric();
    gluQuadricDrawStyle(cylinderQuadric, GLU_FILL);
	gluQuadricTexture(cylinderQuadric, GL_TRUE);			// Create Texture Coords  
	gluQuadricNormals(cylinderQuadric, GLU_SMOOTH);			// Create Smooth Normals
	gluQuadricOrientation(cylinderQuadric, GLU_OUTSIDE);	// Pointing normals away from center
	// Dados relativos a esfera.
	sphereQuadric=gluNewQuadric();
    gluQuadricDrawStyle(sphereQuadric, GLU_FILL);
	gluQuadricTexture(sphereQuadric, GL_TRUE);				// Create Texture Coords  
	gluQuadricNormals(sphereQuadric, GLU_SMOOTH);			// Create Smooth Normals
	gluQuadricOrientation(sphereQuadric, GLU_OUTSIDE);		// Pointing normals away from center
}

//		Funcao timer olhos boneco de neve.
void timerFunc2(int value)
{
	//Boneco de neve mudanca da cor dos olhos
	colorSnowMan = (colorSnowMan +1) % 3;
	glutTimerFunc(100, timerFunc2, 1);								//Define qual a funcao que ira executar apos 500 ms.
}

//		Funcao timer.
void timerFunc(int value)
{
	//Ciclo dia/noite
	ambientColorLight[0] = ambientColorLight[0] + colorIncrement;
	ambientColorLight[1] = ambientColorLight[1] + colorIncrement;
	ambientColorLight[2] = ambientColorLight[2] + colorIncrement;
	fogColor[0] = fogColor[0] + colorIncrement;
	fogColor[1] = fogColor[1] + colorIncrement;
	fogColor[2] = fogColor[2] + colorIncrement;
	if(ambientColorLight[0] <= ambientNightColorLight[0]  || ambientColorLight[1] <= ambientNightColorLight[1] || ambientColorLight[2] <= ambientNightColorLight[2])
	{
		ambientColorLight[0] = ambientNightColorLight[0];
		ambientColorLight[1] = ambientNightColorLight[1];
		ambientColorLight[2] = ambientNightColorLight[2];
		colorIncrement = + 0.0005;
	}else if(ambientColorLight[0] >= ambientDayColorLight[0]  || ambientColorLight[1] >= ambientDayColorLight[1] || ambientColorLight[2] >= ambientDayColorLight[2]){
		ambientColorLight[0] = ambientDayColorLight[0];
		ambientColorLight[1] = ambientDayColorLight[1];
		ambientColorLight[2] = ambientDayColorLight[2];
		colorIncrement = - 0.0005;
	}

	//Cigarro a arder
	if(burnMode){
		burningState = burningState + burningIncrement;
		if(burningState > 3.5){//Quando queima o cigarro todo volta ao inicio.
			burningState = 0;
			//burningState = 3.5;//ESTA PARTE ERA PARA O CIGARRO PARAR DE ARDER E DESLIGAR A RENOVACAO DAS PARTICULAS
			//particleSystem.modifyRenewParticle(false);
		}
	}

	//Densidade do nevoeiro va incrementando consoante o cigarro vai queimando.
	if(fogDensity < 0.0045){//Limite de densidade da fumarada.
		fogDensity = fogDensity + fogDensityIncrement;		// A cada frame vai aumentando a densidade do fumo.(nevoeiro)
	}

	// Janela:
	windowRotationAngle = windowRotationAngle + windowRotationIncrement;
	if(windowRotationAngle >= 90.0){
		windowRotationIncrement = 0.0;
		windowRotationAngle = 90.0;
	}else if(windowRotationAngle <= 0.0){
		windowRotationIncrement = 0.0;
		windowRotationAngle = 0.0;
	}

	// Bola de basket exterior
	if(incrementY > 0.0)
		incrementY = incrementY - ((G*0.1)/2.0);
	else if (incrementY <= 0.0)
		incrementY = incrementY - ((G*0.1)/2.0);

	if (basketBallPosition[1] < -70.0)
		incrementY = 10.0;
	
	if(basketBallPosition[2] > 245.0){
		rotationIncrement = -rotationIncrement;
		incrementZ = - incrementZ;
	}
	else if (basketBallPosition[2] < -850.0){
		rotationIncrement = -rotationIncrement;
		incrementZ = - incrementZ;
	}

	basketBallPosition[0] = basketBallPosition[0] + incrementX;
	basketBallPosition[1] = basketBallPosition[1] + incrementY;
	basketBallPosition[2] = basketBallPosition[2] + incrementZ;

	rotationAngle = rotationAngle + rotationIncrement;  //Vai rodando a bola.

	glutPostRedisplay();
	glutTimerFunc(10, timerFunc, 1);								//Define qual a funcao que ira executar apos 100 ms.
}

//	Funcao para redimensior a janela.
void resizeWindow(GLsizei w, GLsizei h)
{
	//Alteram-se os valores das nossas variaveis que guardam a altura e largura da janela.
	wScreen=w;
	hScreen=h;
	glutPostRedisplay();	
}

//		Main Function
int main(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);		//Single-buffered (GLUT_SINGLE): todos os comandos de desenho sao feitos na janela de exibicao.
																	//Double-buffered (GLUT_DOUBE): os comandos de desenho são executados para criar uma cena fora da tela para depois rapidamente coloca-la na view (ou janela de visualizacao).
																	//GLUT_RGB: An alias for GLUT_RGBA. GLUT_DEPTH: Bit mask to select a window with a depth buffer.
	glutInitWindowSize(wScreen,hScreen);							//Defenir o tamanho da janela.
	glutInitWindowPosition(700,150);								//Definir a posicao da janela
	glutCreateWindow("Noughts & Crosses");							//Cria a janela com o nome pretenedido.
	init();
	glutReshapeFunc(resizeWindow);									//Define qual a funcao vai ser usada para redimensionamento da janela.
	glutSpecialFunc(keyboardNotASCIIHandler);						//Define qual a funcao que o GLUT vai usar quando uma tecla for pressionada.
	glutDisplayFunc(displayMain);									//Define qual a funcao que o GLUT vai usar para callback de exibicao.
	glutKeyboardFunc(keyboardHandler);								//Define qual a funcao que o GLUT vai usar quando uma tecla for pressionada.
	glutTimerFunc(10, timerFunc, 1);								//Define qual a funcao que ira executar apos 100 ms.
	glutTimerFunc(100, timerFunc2, 1);								//Define qual a funcao que ira executar apos 500 ms.
	glutMainLoop();													//Função que faz com que comece a execução da “máquina de estados”
	return 0;
}