#ifndef _RENDER_HXX_
#define _RENDER_HXX_

#include "cs237.hxx"
#include "mesh.hxx"
//#include "view.hxx" //need this for modes enum


//! an abstract base class that wraps a renderer behavior
//
 enum RenderMode {
    WIREFRAME = 0,      //!< render scene as a wireframe
    FLAT_SHADING,       //!< render the scene using flat shading
    NUM_RENDER_MODES    //!< == to the number of rendering modes
};

//container strcut for light information
struct Sunlight {
  cs237::vec3f lightDir;
  cs237::color3f lightInten;
  cs237::color3f lightAmb;
};

class Renderer {
  public:

  //! enable the renderer
  //! \param projectMat the projection matrix for the current camera state
    virtual void Enable (cs237::mat4f const &projectionMat, Sunlight sun) = 0;

    virtual ~Renderer ();

    //! render a mesh using this renderer
    //! \param modelViewMat the model-view matrix for the mesh and camera
    //! \param mesh the mesh to be rendered
    //! \param type switch between flatShading (1) or WireFrame (2)
    virtual void Render (cs237::mat4f const &modelViewMat, Mesh *mesh) = 0;
    virtual void RenderChunk(cs237::mat4f const &modelViewMat, VAO *vao, float hscale, float vscale,
                             float w, cs237::vec3d nw_pos, cs237::vec3d nw_tile) = 0;

  protected:
    //each one of the following classes has copies of these structs
    cs237::ShaderProgram	*_shader;	//!< the shader program

    

    Renderer (cs237::ShaderProgram *sh);

};

class WireframeRenderer : public Renderer {
  public:
    WireframeRenderer ();
    virtual ~WireframeRenderer ();

    //Holders for the memory locations of shader program variables
    int mvLoc;
    int projLoc;
    int colorLoc;

    int hscaleLoc;
    int vscaleLoc;
    int cellwidthLoc;
    int nwcornerLoc;

    void Enable (cs237::mat4f const &projectionMat, Sunlight sun);
    void Render (cs237::mat4f const &modelViewMat, Mesh *mesh);
    void RenderChunk(cs237::mat4f const &modelViewMat, VAO *vao, float hscale, float vscale,
                     float w, cs237::vec3d nw_pos, cs237::vec3d nw_tile);

};

class SkyBoxRenderer : public Renderer {
  public:
    SkyBoxRenderer ();
    virtual ~SkyBoxRenderer ();

    //Holders for the memory locations of shader program variables
    int mvLoc;
    int projLoc;
    int texSamplerLoc;

    void Enable (cs237::mat4f const &projectionMat, Sunlight sun);
    void Render (cs237::mat4f const &modelViewMat, Mesh *mesh);
    void RenderChunk(cs237::mat4f const &modelViewMat, VAO *vao, float hscale, float vscale,
                     float w, cs237::vec3d nw_pos, cs237::vec3d nw_tile);

};

class FullRenderer : public Renderer {
    public:
        FullRenderer();
        virtual ~FullRenderer();

        //transformation uniforms
        int mvLoc;
        int projLoc;
        int hscaleLoc;
        int vscaleLoc;
        int cellwidthLoc;
        int nwcornerLoc;
        int nwtileLoc;

        //texture sampling uniforms
        int texSamplerLoc;
        int normSamplerLoc;

        //directional lighting uniforms
        int lightDirLoc;
        int lightIntenLoc;
        int lightAmbLoc;

        void Enable(cs237::mat4f const &projectionMat, Sunlight sun);
        void Render (cs237::mat4f const &modelViewMat, Mesh *mesh);
        void RenderChunk(cs237::mat4f const &modelViewMat, VAO *vao, float hscale, float vscale,
                         float w, cs237::vec3d nw_pos, cs237::vec3d nw_tile);
};

#endif // !_RENDER_HXX_