#include "cube.hxx"

#define HEIGHT 1.0f
#define WIDTH 1.0f

//create a master cube
Cube::Cube(Sunlight sun, float height, float width)
{
	this->sun = sun;
	cs237::vec3f cubeVertices[8]= {cs237::vec3f(-1.0f,  -1.0f,  1.0f), //0
					                   cs237::vec3f (-1.0f,  1.0f,  1.0f), //1
					                   cs237::vec3f ( 1.0f,  1.0f,  1.0f), //2
					                   cs237::vec3f( 1.0f,  -1.0f,  1.0f), //3
					                   cs237::vec3f (-1.0f,  -1.0f, -1.0f), //4
					                   cs237::vec3f (-1.0f,  1.0f, -1.0f), //5
					                   cs237::vec3f ( 1.0f,  1.0f, -1.0f), //6
					                   cs237::vec3f ( 1.0f,  -1.0f, -1.0f)}; //7      

	/* the indices that allow us to create the cube. */ 
	uint32_t cubeIndices[36] = {
	    0,2,1,  0,3,2,
	    4,3,0,  4,7,3,
	    4,1,5,  4,0,1,
	    3,6,2,  3,7,6,
	    1,6,5,  1,2,6,
	    7,5,6,  7,4,5
	  };

	cs237::vec3f cubeNormals[8] = {

		cs237::vec3f(  0.0f,  0.0f,  -1.0f ),
		cs237::vec3f(  0.0f,  0.0f,  -1.0f ),
		cs237::vec3f(  1.0f,  0.0f,  0.0f ),
		cs237::vec3f(  0.0f, -1.0f,  1.0f ),
		cs237::vec3f(  -1.0f,  0.0f,  0.0f ),
		cs237::vec3f(  1.0f,  0.0f,  0.0f ),
		cs237::vec3f(  0.0f,  0.0f,  1.0f ),
		cs237::vec3f(  0.0f,  0.0f,  1.0f ),
	};
	this->mesh = new Mesh(GL_TRIANGLES);
    this->mesh->LoadVertices(8, cubeVertices);
    this->mesh->LoadIndices(36, cubeIndices);
    this->mesh->LoadNormals(8, cubeNormals);

    this->height = height;
    this->width = width;

    this->_shader = new cs237::ShaderProgram ("../shaders/cube.vsh", "../shaders/cube.fsh");
    mvLoc = _shader->UniformLocation ("modelView");
  	projLoc = _shader->UniformLocation("projection");
  	htLoc = _shader->UniformLocation("ht");
  	widLoc = _shader->UniformLocation("wid");
  	colorLoc = _shader->UniformLocation("color");
  	lightDirLoc = _shader->UniformLocation("lightDir");
    lightIntenLoc = _shader->UniformLocation("lightInten");
    lightAmbLoc = _shader->UniformLocation("lightAmb");

    //fog uniforms
    hasFogLoc = _shader->UniformLocation("hasFog");
    fogColorLoc = _shader->UniformLocation("fogColor");
    fogDensityLoc = _shader->UniformLocation("fogDensity");
}

void Cube::Render(cs237::vec3f pos, cs237::color4f color, cs237::mat4f const &projectionMat, cs237::mat4f const &modelViewMat,
	bool hasFog, cs237::color3f fogColor, float fogDensity)
{
	this->_shader->Use();
	//printf("rendering cube with %f, %f, %f\n", pos.x, pos.y, pos.z);
    cs237::setUniform(projLoc, projectionMat);
    cs237::setUniform(mvLoc, cs237::translate(modelViewMat, pos));
    cs237::setUniform(colorLoc, color);
    cs237::setUniform(htLoc, this->height);
    cs237::setUniform(widLoc, this->width);
    cs237::setUniform(hasFogLoc, hasFog);
    cs237::setUniform(fogColorLoc, fogColor);
    cs237::setUniform(fogDensityLoc, fogDensity);
    CS237_CHECK(cs237::setUniform(lightDirLoc, sun.lightDir));
    CS237_CHECK(cs237::setUniform(lightIntenLoc, sun.lightInten));
    CS237_CHECK(cs237::setUniform(lightAmbLoc, sun.lightAmb));
	this->mesh->Draw();
}

cs237::AABBd Cube::CubeAABB(cs237::vec3f *pos)
{
	return cs237::AABBd(cs237::toDouble((*pos) - cs237::vec3f(this->width, this->height, this->width)), 
		   cs237::toDouble((*pos) + cs237::vec3f(this->width, this->height, this->width)));
}