#include "cs237.hxx"

class Skybox {

	cs237::texture2D **faces;
    cs237::ShaderProgram    *_shader;
    //Holders for the memory locations of shader program variables
    int mvLoc;
    int projLoc;
    int texSamplerLoc;

    GLuint    vaoId;
    GLuint    verticesVBOId;

	GLuint textureID;
    int wid, ht;

    public:
    Skybox(int wid, int ht);
    void Render(cs237::mat4f const &projectionMat, cs237::mat4f const &modelViewMat);
	
};