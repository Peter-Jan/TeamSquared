#include "yspng.h"

GLuint decodePng()
{

	YsRawPngDecoder png[2];
	png[0].Decode("poo.png");
	png[1].Decode("emoji.png");

	GLuint texId;             	// Two unsigned integers for reserving texture identifiers

	glGenTextures(1, &texId);  	// Reserve one texture identifier
	glBindTexture(GL_TEXTURE_2D, texId);	// Making the texture identifier current 2D texture (or bring it to the deck)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);  // Specifying parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D
	(GL_TEXTURE_2D,
		0,    	// Level of detail
		GL_RGBA,  	// Internal data type.
		png[1].wid,
		png[1].hei,
		0,    	// Border width, but not supported and needs to be 0.
		GL_RGBA,  	// Source data type 
		GL_UNSIGNED_BYTE,  	// Source component type
		png[1].rgba);

	printf("TEX1: %d \n", texId);

	return texId;
}