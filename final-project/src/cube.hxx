#include "cs237.hxx"
#include "mesh.hxx"
#include "camera.hxx"

class Cube {
    Mesh * mesh;
    cs237::ShaderProgram    *_shader;
    int mvLoc;
    int projLoc;
    int colorLoc;
    int htLoc;
    int widLoc;

    //directional lighting uniforms
    int lightDirLoc;
    int lightIntenLoc;
    int lightAmbLoc;

    //Assuming that the sunlight isn't gonna suddenly change
    Sunlight sun;

public:
    Cube(Sunlight sun);
    void Render(cs237::vec3f pos, cs237::color4f color, cs237::mat4f const &projectionMat, cs237::mat4f const &modelViewMat);
    void RenderRandom(int xRand, int zRand, cs237::mat4f const &projectionMat, cs237::mat4f const &modelViewMat);
    //cs237::AABB CubeAABB(cs237::vec3f pos);
	
};