class SkyBox {
public:  

	SkyBox() { }

    void SkyBox::Load_Texture(GLint textureArrayPosition, 	char* path);
	void Init(int mode, GLfloat size, GLfloat yyIncrement);
	void SkyBox::draw();

	GLuint	skyBoxTextures[6];//Array que guarda as texturas
	GLfloat worldSize;	//Tamanho do mundo
	GLfloat yy;
	RgbImage	imag;
};
