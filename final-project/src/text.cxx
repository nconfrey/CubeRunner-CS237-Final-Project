#include "text.hxx"

Text::Text(cs237::vec2f screenSize) {
	this->screenSize = screenSize;
	//cs237 norm = cs237::vec3f{  0.0f,  0.0f,  1.0f };
	const uint32_t indices[4] = {0, 1, 2, 3};

    const cs237::vec3f verts[4] = {cs237::vec3f{  1.0f,  1.0f, -1.0f },
    cs237::vec3f{ -1.0f,  1.0f, -1.0f },
    cs237::vec3f{ -1.0f, -1.0f, -1.0f },
    cs237::vec3f{  1.0f, -1.0f, -1.0f }};

    cs237::image2d * image = new cs237::image2d("../assets/../assets/score.png");
    this->scoreImage = new cs237::texture2D (GL_TEXTURE_2D, image);
    this->scoreImage->Bind(); 
 
//4
	this->scoreImage->Parameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	this->scoreImage->Parameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    //Mesh *board = new Mesh(GL_TRIANGLE_FAN);

    //board->LoadVertices(4, cur.corners);
    //board->LoadIndices(4, indices); //just a square

    glGenVertexArrays(1, &(this->vaoId));

    this->_shader = new cs237::ShaderProgram ("../shaders/full.vsh", "../shaders/full.fsh");

	texLoc = this->_shader->UniformLocation ("image");
	screenLoc = this->_shader->UniformLocation ("screenSize");
}

void Text::Render(cs237::vec2f const &screen) {
	this->_shader->Use();
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//CS237_CHECK( glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) );

	glBindVertexArray(this->vaoId);
	CS237_CHECK(glActiveTexture(GL_TEXTURE0));
	this->scoreImage->Bind();
    cs237::setUniform (this->texLoc, 0);
    cs237::setUniform (this->screenLoc, screen);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //CS237_CHECK( glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) );
}