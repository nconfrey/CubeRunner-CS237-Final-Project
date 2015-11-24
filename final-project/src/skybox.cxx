#include "skybox.hxx"

Skybox::Skybox(int wid, int ht)
{
	this->wid = wid;
	this->ht = ht;

	glGenTextures(1, &this->textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	this->faces = new cs237::texture2D *[6];
 	for(int i = 0; i <=6; i++)
 	{
 		cs237::image2d * image = new cs237::image2d("../assets/skybox/face" + i + ".jpg");
 		this->faces[i] = new cs237::texture2D (GL_TEXTURE_CUBE_MAP_POSITVE_X + i, 0, wid, ht, 0, GL_FLOAT, image);
 	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);  
}