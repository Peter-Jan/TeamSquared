#include "yspng.h"

GLuint decodePng()
{

	YsRawPngDecoder png[2];
	png[0].Decode("emoji.png");
	png[1].Decode("poo.png");

	GLuint texId;             	// Two unsigned integers for reserving texture identifiers

	glGenTextures(1, &texId);  	// Reserve one texture identifier
	glBindTexture(GL_TEXTURE_2D, texId);	// Making the texture identifier current 2D texture (or bring it to the deck)
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, png[0].wid, png[0].hei, GL_RGBA, GL_UNSIGNED_BYTE, png[0].rgba);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);  // Specifying parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//glTexImage2D
	//(GL_TEXTURE_2D,
	//	0,    	// Level of detail
	//	GL_RGBA,  	// Internal data type.
	//	png[0].wid,
	//	png[0].hei,
	//	0,    	// Border width, but not supported and needs to be 0.
	//	GL_RGBA,  	// Source data type 
	//	GL_UNSIGNED_BYTE,  	// Source component type
	//	png[0].rgba);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	printf("TEX1: %d \n", texId);

	return texId;
}