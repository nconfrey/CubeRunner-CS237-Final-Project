#ifndef _CUBE_HXX_
#define _CUBE_HXX_


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

    //fog uniforms
    int hasFogLoc;
    int fogColorLoc;
    int fogDensityLoc;

    //Assuming that the sunlight isn't gonna suddenly change
    Sunlight sun;

    //internal values that make this cube unique
    float width;
    float height;

public:
    Cube(Sunlight sun, float height, float width);
    void Render(cs237::vec3f pos, cs237::color4f color, cs237::mat4f const &projectionMat, cs237::mat4f const &modelViewMat,
        bool hasFog, cs237::color3f fogColor, float fogDensity);
    cs237::AABBd CubeAABB(cs237::vec3f *pos);
	
};

#endif // ! _CUBE_HXX_