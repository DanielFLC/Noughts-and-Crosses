//		Colours:
#define	ABAJUR		1.0, 1.0, 1.0, 0.9
#define	LAMP_ON		1.0, 1.0, 0.7, 1.0
#define	LAMP_OFF	0.7, 0.7, 0.7, 0.9
#define WINDOWGLASS	0.85, 0.85, 0.95, 0.2
#define GLASS		0.85, 0.85, 0.85, 0.55
#define WATER		0.81, 0.87, 0.96, 0.30
#define	RED			1.0, 0.0, 0.0, 1.0
#define	REDDARK		0.8, 0.0, 0.0, 1.0
#define	GREEN		0.0, 1.0, 0.0, 1.0
#define	GREEN_ALT	0.0, 0.8, 0.0, 1.0
#define BLUE		0.0, 0.0, 1.0, 1.0
#define YELLOW		1.0, 1.0, 0.0, 1.0
#define ORANJE		1.0, 0.5, 0.1, 1.0
#define WHITE		1.0, 1.0, 1.0, 1.0
#define BLACK		0.0, 0.0, 0.0, 1.0
#define BLACKLESS	0.05, 0.05, 0.05, 1.0
#define GREY_DARK	0.2, 0.2, 0.2, 1.0
#define GREY_WHITE	0.70, 0.70, 1, 1.0

//		PI value:
#define PI			3.14159

//		Mundo:
#define xC			10000
#define	yC			10000
#define zC			10000



void drawAxis(void);
void drawCircle(float radius);
void drawGameBoard(GLuint upperFaceTexture, GLuint otherFacesTexture);
void drawTable(GLuint tableTexture);
void drawRoom(GLuint floorTexture, GLuint wallTexture, GLuint wallUpperDoorTexture , GLuint doorTexture, GLuint windowLeftTexture, GLuint leftWallWindow, GLuint rightWallWindow, GLuint upperWallWindow, GLuint bottomWallWindow, GLuint windowRightTexture, GLuint windowBottomTexture, GLuint windowUpperTexture, GLuint windowMidleUpTexture, GLuint windowMidleRightTexture, GLfloat windowRotationAngle);
void drawCarpet(GLuint carpetTexture);
void drawPiece(GLfloat xBaseCenter, GLfloat yBaseCenter, GLfloat zBaseCenter, GLint colourFlag);
void drawText(char *string, GLfloat x, GLfloat y, GLfloat z) ;
void drawClock(GLuint frontTexture, GLuint othersTexture, GLfloat hourAngle, GLfloat minutesAngle, GLfloat secondsAngle);
void drawFrame(GLuint frameTexture, GLuint pictureTexture);
void drawSphere(GLUquadricObj* sphereQuadric, GLuint texture, GLfloat radius);
void drawCylinder(GLUquadricObj * cylinderQuadric, GLuint texture, GLfloat radius, GLfloat height);
void drawBlackAxis(void);
void drawSphereColor(GLUquadricObj * sphereQuadric, GLfloat radius);
void drawCylinderColor(GLUquadricObj * cylinderQuadric, GLfloat radiusB, GLfloat radiusT, GLfloat height);
void drawCircleTexture(float radius, GLuint texture);
void drawSmoke(GLUquadricObj * sphereQuadric);
void changeMeshFactor(void);
void drawGlassWater(GLUquadricObj *cylinderQuadric);
void drawQuadsSimple(GLfloat dimX, GLfloat dimY);
void drawQuads(GLuint texture,GLint dimX, GLint dimY);
void drawIgloo(GLUquadricObj * cylinderQuadric, GLUquadricObj * sphereQuadric, GLuint wallTexture, GLuint floorTexture, GLuint doorTexture, GLuint tunnelTexture,GLuint outsideDoorTexture);
void drawIceCube(GLuint cubeTexture);
void drawSnowMan(GLUquadricObj * sphereQuadric,GLUquadricObj * cylinderQuadric , GLuint bodyTexture, GLint eyesColor);