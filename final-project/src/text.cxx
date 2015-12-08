#include "text.hxx"

Text::Text(cs237::vec2f screenSize) {
	this->screenSize = screenSize;
	//cs237 norm = cs237::vec3f{  0.0f,  0.0f,  1.0f };
	const uint32_t indices[4] = {0, 1, 2, 3};

    const cs237::vec3f verts[4] = {cs237::vec3f{  1.0f,  1.0f, -1.0f },
    cs237::vec3f{ -1.0f,  1.0f, -1.0f },
    cs237::vec3f{ -1.0f, -1.0f, -1.0f },
    cs237::vec3f{  1.0f, -1.0f, -1.0f }};

    cs237::image2d * image = new cs237::image2d("../assets/../assets/bscore.png");
    this->scoreImage = new cs237::texture2D (GL_TEXTURE_2D, image);
    this->scoreImage->Bind(); 
	this->scoreImage->Parameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	this->scoreImage->Parameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    this->nums = new cs237::texture2D *[9];
    for (int i = 0; i < 10; i++)
    {
        cs237::image2d * num = new cs237::image2d("../assets/../assets/"+ std::to_string(i) + ".png");
        this->nums[i] = new cs237::texture2D (GL_TEXTURE_2D, num);
        this->nums[i]->Bind(); 
        this->nums[i]->Parameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
        this->nums[i]->Parameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        //printf("Loaded %d num\n", i+1);
    }

    //Mesh *board = new Mesh(GL_TRIANGLE_FAN);

    //board->LoadVertices(4, cur.corners);
    //board->LoadIndices(4, indices); //just a square

    glGenVertexArrays(1, &(this->vaoId));

    this->_shader = new cs237::ShaderProgram ("../shaders/full.vsh", "../shaders/full.fsh");

	texLoc = this->_shader->UniformLocation ("image");
    offsetLoc = this->_shader->UniformLocation ("offset");
    squishLoc = this->_shader->UniformLocation ("squish");
	screenLoc = this->_shader->UniformLocation ("screenSize");
}

void Text::Render(cs237::vec2f const &screen, int score) {
	this->_shader->Use();
    //glDisable(GL_DEPTH_TEST);
	//glEnable (GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	CS237_CHECK( glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) );

	glBindVertexArray(this->vaoId);
	CS237_CHECK(glActiveTexture(GL_TEXTURE0));
	this->scoreImage->Bind();
    cs237::setUniform (this->texLoc, 0);
    cs237::setUniform (this->screenLoc, screen);
    cs237::setUniform (this->offsetLoc, 0.0f);
    cs237::setUniform (this->squishLoc, 0.0f);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    for (int i = 12; i >= 0; --i)
    {
        int curInt = score%10;//i==0?score%10:(score/10*i) % 10;
        score /= 10;
        printf("Considering number in the %d place, it is %d\n", i, curInt);
        this->nums[curInt]->Bind();
        cs237::setUniform (this->texLoc, 0);
        cs237::setUniform (this->screenLoc, screen);
        float offset = (i==0?0.30f:0.0f);
        cs237::setUniform (this->offsetLoc, offset + (i*0.10f));
        cs237::setUniform (this->squishLoc, -0.20f);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    //CS237_CHECK( glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) );
}