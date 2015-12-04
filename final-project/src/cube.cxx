#include "cube.hxx"

//create a master cube
Cube::Cube()
{
	cs237::vec3f cubeVertices[8]= {cs237::vec3f(-100.0f,  -100.0f,  100.0f), //0
					                   cs237::vec3f (-100.0f,  100.0f,  100.0f), //100
					                   cs237::vec3f ( 100.0f,  100.0f,  100.0f), //2
					                   cs237::vec3f( 100.0f,  -100.0f,  100.0f), //3
					                   cs237::vec3f (-100.0f,  -100.0f, -100.0f), //4
					                   cs237::vec3f (-100.0f,  100.0f, -100.0f), //5
					                   cs237::vec3f ( 100.0f,  100.0f, -100.0f), //6
					                   cs237::vec3f ( 100.0f,  -100.0f, -100.0f)}; //7 

	/* the indices that allow us to create the cube. */ 
	uint16_t cubeIndices[36] = {
	    0,2,1,  0,3,2,
	    4,3,0,  4,7,3,
	    4,1,5,  4,0,1,
	    3,6,2,  3,7,6,
	    1,6,5,  1,2,6,
	    7,5,6,  7,4,5
	  };
	this->mesh = new Mesh(GL_TRIANGLES);
    this->mesh->LoadVertices(8, cubeVertices);
    this->mesh->LoadIndices(36, cubeIndices);

    this->_shader = new cs237::ShaderProgram ("../shaders/skybox.vsh", "../shaders/skybox.fsh");
    mvLoc = _shader->UniformLocation ("modelView");
  	projLoc = _shader->UniformLocation("projection");
  	texSamplerLoc = _shader->UniformLocation("skybox");
}

void Cube::Render(cs237::vec3f pos, cs237::color3f color, cs237::mat4f const &projectionMat, cs237::mat4f const &modelViewMat)
{
	//need to make a toWorld transformation here!!
	this->_shader->Use();
    cs237::setUniform(projLoc, projectionMat);
    //cs237::mat4f noTrans = cs237::mat4f(cs237::mat3f(modelViewMat), cs237::vec4f(0,0,0,0));
    cs237::setUniform(mvLoc, modelViewMat);
    cs237::setUniform(texSamplerLoc, 0);
	this->mesh->Draw();
}

// Cube::CubeAABB(cs237::vec3f pos)
// {
	
// }