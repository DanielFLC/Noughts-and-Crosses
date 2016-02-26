#include <windows.h>
#include <stdio.h>			//  Standard Input\Output C Library
#include <GL/glut.h>		//  Include GLUT, OpenGL, and GLU libraries
#include "DisplayCore.h"
#include "BoardGameCore.h"
#include "Sounds.h"

//		Jogo do Galo:
GLint		noughtsAndCrossesMode		=	1;		//1- Desligado. 0- Ligado
GLint		noughtsAndCrossesCameraMode	=	1;		//Estilo de camera a usar no jogo do galo.
GLfloat		boardMatrix[3][3]			=	{ {-1, -1, -1 }, { -1, -1, -1 }, { -1, -1, -1 } };	// Matriz que representa o tabuleiro do jogo. -1->Vazio. 0->Bola. 1->Cruz.
GLfloat	    positionsMatrix[3][3][3]	=	{ { {-5.7, 1.1, -5.1}, {0, 1.1, -5.1}, {5.7, 1.1, -5.1} }, { {-5.7, 1.1, 0}, {0, 1.1, 0}, {5.7, 1.1, 0} }, { {-5.7, 1.1, 5.1}, {0, 1.1, 5.1}, {5.7, 1.1, 5.1} } };
GLfloat		numberPieces				=	0;		//Numero de pecas.
GLfloat		pieceStanding				=	0;		//Se existe alguma peca no ar. 0-Nao 1-Sim
GLfloat		currentPieceColor;						//Cor da peca a ser jogada.
GLfloat		xPos, yPos;								//Posicao na matriz da peca a ser jogada.
GLint		greenWins = 0, redWins = 0;				//Guarda o resultado dos jogos realizados.
bool		resultUpdated		=	false;			//Se ja incrementou o resultado
//		Sounds:
bool		victoryDrawSound	=	false;			//Se ja reproduzir o som de vitoria (evitar loops)
bool		firstWin			=	true;			//Se ja houve alguma vitoria (som diferente para a primeira vitoria)

//		Funcao que faz reset ao resultado.
void ResetScore(void)
{
	greenWins	=	0;
	redWins		=	0;
	firstWin	=	true;
	resultUpdated		=	false;
}

//		Funcao para detetar acoes do teclado durante o jogo do galo.
void NoughtsAndCrossesKeyboardHandler(unsigned char key){
	switch(key){
		//		Sair do modo de jogo do galo.
		case 'e':
		case 'E':
			noughtsAndCrossesMode = 1;
			break;
		//		Change camera:
		case '1':
			noughtsAndCrossesCameraMode = (noughtsAndCrossesCameraMode + 1)%2;
			break;
		//		Andar para a frente com a peca:
		case 'w':
		case 'W':
			xPos=xPos-1;
			if(xPos<0){
				xPos=2;
			}
			break;
		//		Andar para tras com a peca:
		case 's':
		case 'S':
			xPos=xPos+1;
			if(xPos>2){
				xPos=0;
			}
			break;
		//		Andar para a esquerda com a peca:
		case 'a':
		case 'A':
			yPos=yPos-1;
			if(yPos<0){
				yPos=2;
			}
			break;
		//		Andar para a direita com a peca:
		case 'd':
		case 'D':
			yPos=yPos+1;
			if(yPos>2){
				yPos=0;
			}
			break;
		//	Efetua a jogada se possivel.
		case ' ':
			if(pieceStanding==0)
				break;
			if((GLint) boardMatrix[(GLint)xPos][(GLint)yPos]==-1){
				boardMatrix[(GLint)xPos][(GLint)yPos]=currentPieceColor;
				xPos=1;
				yPos=1;
				currentPieceColor = ((GLint)currentPieceColor + 1)%2;
				numberPieces = numberPieces + 1;
				PlaySound(MOVEBLOCKSOUND, NULL, SND_ASYNC|SND_FILENAME);
			}else{
				PlaySound(DENIEDSOUND, NULL, SND_ASYNC|SND_FILENAME);
			}
			break;
		// Restart (mete a 0-0 e limpa o tabuleiro)
		case 'r':
		case 'R':
			firstWin = true;
			ResetScore();
			PlaySound(STARTGAMESOUND, NULL, SND_ASYNC|SND_FILENAME);
		// Limpa o tabuleiro.
		case 'c':
		case 'C':
			numberPieces = 0;
			for(int i=0; i<3; i++){
				for(int j=0; j<3; j++){
					boardMatrix[i][j]=-1;
				}
			}
			victoryDrawSound=false;
			resultUpdated=false;
			break;
	}
}

GLint gameMode(){
	return noughtsAndCrossesMode;
}

GLint cameraMode(){
	return noughtsAndCrossesCameraMode;
}

void turnGameOn(){
	noughtsAndCrossesMode = 0;
	noughtsAndCrossesCameraMode = 1;
}

/*	Funcao que verifica a matriz do tabuleiro do jogo do galo para: 
		-	Verificar se algum jogador ja ganhou.
	Retorna:
		-1 em caso de não se verificar vitória;
		0 em caso de vitoria da bola; 
		1 em casa de vitoria da cruz;
*/
GLint NoughtsAndCrossesCheckState(void)
{
	// Verifica a primeira linha, a primeira coluna e a diagonal de cima para baixo.
	if((GLint) boardMatrix[0][0] != -1){
		if(boardMatrix[0][0] == boardMatrix[0][1] && boardMatrix[0][1] == boardMatrix[0][2]){
			return (GLint) boardMatrix[0][2];
		}
		if(boardMatrix[0][0] == boardMatrix[1][1] && boardMatrix[1][1] == boardMatrix[2][2]){
			return (GLint) boardMatrix[2][2];
		}
		if(boardMatrix[0][0] == boardMatrix[1][0] && boardMatrix[1][0] == boardMatrix[2][0]){
			return (GLint) boardMatrix[2][0];
		}
	}

	// Verifica a segunda linha.
	if((GLint) boardMatrix[1][0] != -1){
		if(boardMatrix[1][1] == boardMatrix[1][2] &&  boardMatrix[1][1] == boardMatrix[1][0]){
			return (GLint) boardMatrix[1][0];
		}
	}

	// Verifica a terceira linha e a diagonal de baixo para cima.
	if((GLint) boardMatrix[2][0] != -1){
		if(boardMatrix[2][1] == boardMatrix[2][2] &&  boardMatrix[2][0] == boardMatrix[2][2]){
			return (GLint) boardMatrix[2][0];
		}
		if(boardMatrix[1][1] == boardMatrix[0][2] &&  boardMatrix[2][0] == boardMatrix[1][1]){
			return (GLint) boardMatrix[2][0];
		}
	}

	// Verifica a segunda coluna.
	if((GLint) boardMatrix[0][1] != -1){
		if(boardMatrix[1][1] == boardMatrix[2][1] && boardMatrix[1][1] == boardMatrix[0][1]){
			return (GLint) boardMatrix[2][1];
		}
	}

	// Verifica a terceira coluna.
	if((GLint) boardMatrix[0][2] != -1){
		if(boardMatrix[1][2] == boardMatrix[2][2] && boardMatrix[1][2] == boardMatrix[0][2]){
			return (GLint) boardMatrix[0][2];
		}
	}

	return -1;
}

// Funcao que recebe o estado do jogo,	verifica-o, reproduz sons e apresenta o resultado.
void endGameStatus(GLint status){
	if(status==-1){
		if((GLint) numberPieces==10){
			glColor4f(BLACK);
			drawText("DRAW!",-18,3,-12);
			pieceStanding = 0;
			if(!victoryDrawSound){
				victoryDrawSound=true;
				PlaySound(DRAWSOUND, NULL, SND_ASYNC|SND_FILENAME);
			}
		}
		return;
	}
	if(status == 0){
		if(!resultUpdated){
			greenWins = greenWins + 1;
			resultUpdated=true;
		}
		glColor4f(GREEN);
		drawText("GREEN WINS!",-18,3,-12);
		pieceStanding = 0;

	}
	else{
		if(!resultUpdated){
			redWins = redWins + 1;
			resultUpdated=true;
		}
		glColor4f(RED);
		drawText("RED WINS!",-18,3,-12);
		pieceStanding = 0;
	}

	if(!victoryDrawSound){
		if(firstWin){
			PlaySound(FIRSTWINSOUND, NULL, SND_ASYNC|SND_FILENAME);
			firstWin=false;
		}
		else if(greenWins==redWins)
			PlaySound(PAYBACKSOUND, NULL, SND_ASYNC|SND_FILENAME);
		else
			PlaySound(WINSOUND, NULL, SND_ASYNC|SND_FILENAME);
		victoryDrawSound=true;
	}
}

/*	Funcao que verifica a matriz do tabuleiro do jogo do galo para: 
		-	Representar os objectos que ja se encontram no tabuleiro;
		-   Fazer o re-spawn da nova peca.
*/
void NoughtsAndCrossesMain(void)
{
	if((GLint) noughtsAndCrossesMode==0){
		if((GLint) numberPieces==0){
			currentPieceColor=0;
			xPos=1;
			yPos=1;
			drawPiece(positionsMatrix[(GLint) xPos][(GLint) yPos][0], positionsMatrix[(GLint) xPos][(GLint) yPos][1] + 15, positionsMatrix[(GLint) xPos][(GLint) yPos][2], currentPieceColor);
			numberPieces=1;
			pieceStanding=1;
			return;
		}
		char resultText[20]="";
		glColor4f(BLACK);
		sprintf (resultText, "RED: %d GREEN: %d", redWins, greenWins);
		drawText(resultText,8,3,-12);

		endGameStatus(NoughtsAndCrossesCheckState());

		if((GLint) pieceStanding == 1)
			drawPiece(positionsMatrix[(GLint) xPos][(GLint) yPos][0], positionsMatrix[(GLint) xPos][(GLint) yPos][1] + 9, positionsMatrix[(GLint) xPos][(GLint) yPos][2], currentPieceColor);
	}

	if((GLint) numberPieces==0)
		return;

	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			if((GLint) boardMatrix[i][j] != -1)
				drawPiece(positionsMatrix[i][j][0], positionsMatrix[i][j][1], positionsMatrix[i][j][2], (GLint) boardMatrix[i][j]);
}