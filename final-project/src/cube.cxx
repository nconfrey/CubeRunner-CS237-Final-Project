#include "cube.hxx"

#define HEIGHT 3.0f
#define WIDTH 3.0f

//create a master cube
Cube::Cube(Sunlight sun)
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

	cs237::vec3f cubeNormals[12] = {
		cs237::vec3f(  1.0f,  0.0f,  0.0f ),
		cs237::vec3f(  1.0f,  0.0f,  0.0f ),
		cs237::vec3f( -1.0f,  0.0f,  0.0f ),
		cs237::vec3f( -1.0f,  0.0f,  0.0f ),
		cs237::vec3f(  0.0f, -1.0f,  0.0f ),
		cs237::vec3f(  0.0f, -1.0f,  0.0f ),
		cs237::vec3f(  0.0f,  1.0f,  0.0f ),
		cs237::vec3f(  0.0f,  1.0f,  0.0f ),
		cs237::vec3f(  0.0f,  0.0f,  1.0f ),
		cs237::vec3f(  0.0f,  0.0f,  1.0f ),
		cs237::vec3f(  0.0f,  0.0f, -1.0f ),
		cs237::vec3f(  0.0f,  0.0f, -1.0f )
	};
	this->mesh = new Mesh(GL_TRIANGLES);
    this->mesh->LoadVertices(8, cubeVertices);
    this->mesh->LoadIndices(36, cubeIndices);
    this->mesh->LoadNormals(12, cubeNormals);

    this->_shader = new cs237::ShaderProgram ("../shaders/cube.vsh", "../shaders/cube.fsh");
    mvLoc = _shader->UniformLocation ("modelView");
  	projLoc = _shader->UniformLocation("projection");
  	htLoc = _shader->UniformLocation("ht");
  	widLoc = _shader->UniformLocation("wid");
  	colorLoc = _shader->UniformLocation("color");
  	lightDirLoc = _shader->UniformLocation("lightDir");
    lightIntenLoc = _shader->UniformLocation("lightInten");
    lightAmbLoc = _shader->UniformLocation("lightAmb");
}

void Cube::Render(cs237::vec3f pos, cs237::color4f color, cs237::mat4f const &projectionMat, cs237::mat4f const &modelViewMat)
{
	this->_shader->Use();
	//printf("rendering cube with %f, %f, %f\n", pos.x, pos.y, pos.z);
    cs237::setUniform(projLoc, projectionMat);
    cs237::setUniform(mvLoc, cs237::translate(modelViewMat, pos));
    cs237::setUniform(colorLoc, color);
    cs237::setUniform(htLoc, HEIGHT);
    cs237::setUniform(widLoc, WIDTH);
    CS237_CHECK(cs237::setUniform(lightDirLoc, sun.lightDir));
    CS237_CHECK(cs237::setUniform(lightIntenLoc, sun.lightInten));
    CS237_CHECK(cs237::setUniform(lightAmbLoc, sun.lightAmb));
	this->mesh->Draw();
}

void Cube::RenderRandom(int xRand, int zRand, cs237::mat4f const &projectionMat, cs237::mat4f const &modelViewMat)
{
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float x = rand() % xRand;//5000;
	float z = rand() % zRand;
	Render(cs237::vec3f(x,500,z), cs237::color4f(r, g, b, 1.0), projectionMat, modelViewMat);
}

// Cube::CubeAABB(cs237::vec3f pos)
// {
// 	//return 
// }