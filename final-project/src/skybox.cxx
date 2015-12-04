#include "skybox.hxx"

Skybox::Skybox(int wid, int ht)
{
	printf("init skybox\n");
	this->wid = (float)wid;
	this->ht = (float)ht;

	//First, deal with the actual structure of the box in vertex information
	CS237_CHECK(glGenVertexArrays (1, &vaoId));
	CS237_CHECK(glBindVertexArray(this->vaoId));
    CS237_CHECK(glGenBuffers(1, &this->verticesVBOId));
    
    //Set up the buffer so that it knows its holding vertice information
    CS237_CHECK(glBindBuffer(GL_ARRAY_BUFFER, this->verticesVBOId));

    GLfloat skyboxVertices[] = {
    // Positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
	};

    //Now push the data into the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    //This describes the data as vertices
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), ((GLvoid*) 0));
    glBindVertexArray(0);
    

    //unbind stuff here
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, 0) );
//
	//Now load the textures for the sides of the box
	glGenTextures(1, &this->textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	this->faces = new cs237::texture2D *[6];
	cs237::image2d * image = new cs237::image2d("../assets/skybox/face1.png");
 	this->faces[0] = new cs237::texture2D (GL_TEXTURE_2D, image);
 	this->faces[0]->Bind();
 	this->faces[0]->Parameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	this->faces[0]->Parameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	//this->faces = new cs237::texture2D *[6];
	//unsigned char* image;
 	/*for(int i = 1; i < 7; i++)
 	{
 		// cs237::image2d * image = new cs237::image2d("../assets/skybox/face" + i + ".jpg");
 		// this->faces[i] = new cs237::texture2D (GL_TEXTURE_CUBE_MAP_POSITVE_X + i, 0, wid, ht, 0, GL_FLOAT, image);
 		image = SOIL_load_image("../assets/skybox/face" + std::to_string(i) + ".png", wid, ht, 0, SOIL_LOAD_RGB);
    	glTexImage2D(
        	GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
        	0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
    	);
 	}*/

	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
//
	//init our shader programs for this skybox. since we will only have one skybox per scene, this is safe to only call once
	this->_shader = new cs237::ShaderProgram ("../shaders/skybox.vsh", "../shaders/skybox.fsh");

	mvLoc = _shader->UniformLocation ("modelView");
  	projLoc = _shader->UniformLocation("projection");
  	texSamplerLoc = _shader->UniformLocation("skybox");

    htLoc = _shader->UniformLocation("ht");
    widLoc = _shader->UniformLocation("wid");
}

void Skybox::Render(cs237::mat4f const &projectionMat, cs237::mat4f const &modelViewMat)
{
	//printf("Skybox render\n");
	glDepthMask(GL_FALSE);
	this->_shader->Use();

    //translation uniforms
    cs237::setUniform(projLoc, projectionMat);
    //cs237::mat4f noTrans = cs237::mat4f(cs237::mat3f(modelViewMat), cs237::vec4f(0,0,0,0));
    cs237::setUniform(mvLoc, modelViewMat);
    cs237::setUniform(texSamplerLoc, 0);
    cs237::setUniform(htLoc, this->ht);
    cs237::setUniform(widLoc, this->wid);

    glBindVertexArray(this->vaoId);

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    CS237_CHECK(glActiveTexture(GL_TEXTURE0));
    this->faces[0]->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}