#include "skybox.hxx"

#define RED_COLOR  cs237::color4f(1.0f, 0.0f, 0.0f, 1.0f)   /*!< Red color */ 
#define GREEN_COLOR cs237::color4f(0.0f, 1.0f, 0.0f, 1.0f)    /*!< Green color */
#define YELLOW_COLOR  cs237::color4f(1.0f, 1.0f, 0.0f, 1.0f)    /*!< Yellow color */
#define BLUE_COLOR cs237::color4f(0.0f, 0.0f, 1.0f, 1.0f)   /*!< Blue color */
#define SKY_BLUE_COLOR cs237::color4f(0.0f, 1.0f, 1.0f, 1.0f)   /*!< Sky Blue color */
#define PINK_COLOR cs237::color4f(1.0f, 0.0f, 0.5f, 1.0f)   /*!< Pink */ 

struct Wall { 
  cs237::color4f color; 
  cs237::vec3f norm;
  cs237::vec3f corners[4]; 
  cs237::vec2f texCoords[4]; 
}; 

/* The vertices for the triangle */ 
    /*! the five visible walls of the box */  
    Wall  walls[6] = {
    { /* left */
        RED_COLOR,    // color
        cs237::vec3f{  1.0f,  0.0f,  0.0f },  // norm
        {cs237::vec3f{ -1.0f,  1.0f, -1.0f },  // corner[0]
        cs237::vec3f{ -1.0f,  1.0f,  1.0f },
        cs237::vec3f{ -1.0f, -1.0f,  1.0f },
        cs237::vec3f{ -1.0f, -1.0f, -1.0f }},
        {cs237::vec2f{  0.333f,  0.666f},  // texture coordinates 
        cs237::vec2f{  0.666f,  0.666f},
        cs237::vec2f{  0.666f,  0.333f},
        cs237::vec2f{  0.334f,  0.333f}}

    },
    { /* right */
        GREEN_COLOR,
        cs237::vec3f{ -1.0f,  0.0f,  0.0f },
        {cs237::vec3f{  1.0f,  1.0f,  1.0f },
        cs237::vec3f{  1.0f,  1.0f, -1.0f },
        cs237::vec3f{  1.0f, -1.0f, -1.0f },
        cs237::vec3f{  1.0f, -1.0f,  1.0f }}, 
        {cs237::vec2f{  0.666f,  0.666f}, //texture coordinates 
        cs237::vec2f{  1.0f,  0.666f},
        cs237::vec2f{  1.0f,  0.333f},
        cs237::vec2f{  0.666f,  0.333f}}

    },
    { /* top */
        YELLOW_COLOR,
        cs237::vec3f{  0.0f, -1.0f,  0.0f },
        {cs237::vec3f{ -1.0f,  1.0f, -1.0f },
        cs237::vec3f{  1.0f,  1.0f, -1.0f },
        cs237::vec3f{  1.0f,  1.0f,  1.0f },
        cs237::vec3f{ -1.0f,  1.0f,  1.0f }}, 
        {cs237::vec2f{  0.333f,  0.333f}, 
        cs237::vec2f{  0.666f,  0.333f},
        cs237::vec2f{  0.666f,  0.0f},
          cs237::vec2f{  0.333f,  0.0f}}
    },
    { /* bottom */
        BLUE_COLOR,
        cs237::vec3f{  0.0f,  1.0f,  0.0f },
        {cs237::vec3f{ -1.0f, -1.0f,  1.0f },
        cs237::vec3f{  1.0f, -1.0f,  1.0f },
        cs237::vec3f{  1.0f, -1.0f, -1.0f },
        cs237::vec3f{ -1.0f, -1.0f, -1.0f }}, 
        {cs237::vec2f{  0.0f,  0.666f}, //texture coordinates 
        cs237::vec2f{  0.333f,  0.666f},
        cs237::vec2f{  0.333f,  0.333f}, 
        cs237::vec2f{  0.0f,  0.333f}}
    },
    { /* front */
        SKY_BLUE_COLOR, 
        cs237::vec3f{  0.0f,  0.0f,  1.0f },
        {cs237::vec3f{  1.0f,  1.0f, -1.0f },
        cs237::vec3f{ -1.0f,  1.0f, -1.0f },
        cs237::vec3f{ -1.0f, -1.0f, -1.0f },
        cs237::vec3f{  1.0f, -1.0f, -1.0f }}, 
        {cs237::vec2f{  0.0f,  0.333f}, // texture coordinates 
        cs237::vec2f{  0.333f,  0.333f},
        cs237::vec2f{  0.333f,  0.0f},
        cs237::vec2f{  0.0f,  0.0f}}

    },
    { /* back */
        PINK_COLOR,
        cs237::vec3f{  0.0f,  0.0f, -1.0f },
        {cs237::vec3f{ -1.0f,  1.0f,  1.0f },
        cs237::vec3f{  1.0f,  1.0f,  1.0f },
        cs237::vec3f{  1.0f, -1.0f,  1.0f },
        cs237::vec3f{ -1.0f, -1.0f,  1.0f }}, 
        { cs237::vec2f{  0.666f,  0.333f},
            cs237::vec2f{  1.0f,  0.333f},
            cs237::vec2f{  1.0f,  0.0f}, 
            cs237::vec2f{  0.666f,  0.0f}}

    }
      };

Skybox::Skybox(int wid, int ht)
{
	printf("init skybox\n");
	this->wid = (float)wid;
	this->ht = (float)ht;

	this->faces = new cs237::texture2D *[6];
    for (int i = 0; i < 6; ++i)
    {
        cs237::image2d * image = new cs237::image2d("../assets/skybox/face" + std::to_string(i+1) + ".png");
        this->faces[i] = new cs237::texture2D (GL_TEXTURE_2D, image);
        this->faces[i]->Bind();
        this->faces[i]->Parameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
        this->faces[i]->Parameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        this->faces[i]->Parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        this->faces[i]->Parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        this->faces[i]->Parameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    const uint32_t indices[4] = {0, 1, 2, 3};
    this->meshes = new Mesh *[6];
    for(int i = 0; i < 6; i++)
    {
        Mesh *wall = new Mesh(GL_TRIANGLE_FAN);
        Wall cur = walls[i];
        //wall->color = cur.color;

        wall->LoadVertices(4, cur.corners);
        wall->LoadIndices(4, indices); //just a square
        wall->texture = this->faces[i];

        this->meshes[i] = wall;
    }

	//init our shader programs for this skybox. since we will only have one skybox per scene, this is safe to only call once
	this->_shader = new cs237::ShaderProgram ("../shaders/skybox.vsh", "../shaders/skybox.fsh");

	mvLoc = _shader->UniformLocation ("modelView");
  	projLoc = _shader->UniformLocation("projection");
  	texSamplerLoc = _shader->UniformLocation("skybox");
    camPosLoc = _shader->UniformLocation("camPos");
    sideLoc = _shader->UniformLocation("side");

    htLoc = _shader->UniformLocation("ht");
    widLoc = _shader->UniformLocation("wid");

    //fog uniforms
    hasFogLoc = _shader->UniformLocation("hasFog");
    fogColorLoc = _shader->UniformLocation("fogColor");
    fogDensityLoc = _shader->UniformLocation("fogDensity");
}

void Skybox::Render(cs237::mat4f const &projectionMat, cs237::mat4f const &modelViewMat, cs237::vec3d position,
                    bool hasFog, cs237::color3f fogColor, float fogDensity)
{
	//printf("Skybox render\n");
	glDepthMask(GL_FALSE);
	this->_shader->Use();

    //translation uniforms
    cs237::setUniform(projLoc, projectionMat);
    //cs237::mat4f noTrans = cs237::mat4f(cs237::mat3f(modelViewMat), cs237::vec4f(0,0,0,0));
    cs237::setUniform(mvLoc, modelViewMat);
    cs237::setUniform(camPosLoc, cs237::toFloat(position));
    
    cs237::setUniform(htLoc, this->ht);
    cs237::setUniform(widLoc, this->wid);

    //fog uniforms
    CS237_CHECK(cs237::setUniform(hasFogLoc, hasFog));
    CS237_CHECK(cs237::setUniform(fogColorLoc, fogColor));
    CS237_CHECK(cs237::setUniform(fogDensityLoc, fogDensity));

    CS237_CHECK(glActiveTexture(GL_TEXTURE0));
    
    cs237::setUniform(texSamplerLoc, 0);

    for(int i = 0; i < 6; i++)
    {
      cs237::setUniform(sideLoc, i);
      this->faces[i]->Bind();
      this->meshes[i]->Draw();
    }

	glDepthMask(GL_TRUE);
}