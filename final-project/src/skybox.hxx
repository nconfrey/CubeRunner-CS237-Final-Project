#include "cs237.hxx"
#include "mesh.hxx"

class Skybox {

	cs237::texture2D **faces;
    cs237::texture2D **levels;
    Mesh **meshes;
    cs237::ShaderProgram    *_shader;
    //Holders for the memory locations of shader program variables
    int mvLoc;
    int projLoc;
    int texSamplerLoc;
    int camPosLoc;
    int widLoc;
    int htLoc;
    int sideLoc;

    //fog uniforms
    int hasFogLoc;
    int fogColorLoc;
    int fogDensityLoc;

    GLuint    vaoId;
    GLuint    verticesVBOId;
	GLuint    textureID;

    float wid, ht;

    public:
    Skybox(int wid, int ht);
    void Switch(int level);
    void Render(cs237::mat4f const &projectionMat, cs237::mat4f const &modelViewMat, cs237::vec3d position,
        bool hasFog, cs237::color3f fogColor, float fogDensity);
	
};