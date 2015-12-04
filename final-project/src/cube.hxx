#include "cs237.hxx"
#include "mesh.hxx"

class Cube {

    Mesh * mesh;
    cs237::ShaderProgram    *_shader;
    int mvLoc;
    int projLoc;
    int texSamplerLoc;

public:
    Cube();
    void Render(cs237::vec3f pos, cs237::color3f color, cs237::mat4f const &projectionMat, cs237::mat4f const &modelViewMat);
    //CubeAABB(cs237::vec3f pos);
	
};