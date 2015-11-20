/*! \file render.cxx
 *
 * \author John Reppy
 *
 * These are additional functions for the View, Cell, Tile, and Chunk classes for
 * rendering the mesh.
 */

/* CMSC23700 Final Project sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "view.hxx"
#include "camera.hxx"
#include "map-cell.hxx"
#include "buffer-cache.hxx"
#include "texture-cache.hxx"
#include "render.hxx"
#include <map>

//! Colors to use for rendering wireframes at different levels of detail
static cs237::color4ub MeshColor[8] = {
    cs237::color4ub{ 255, 255,   0, 255 },
    cs237::color4ub{ 255,   0, 255, 255 },
    cs237::color4ub{   0, 255, 255, 255 },
    cs237::color4ub{ 255, 128, 128, 255 },
    cs237::color4ub{ 128, 255, 128, 255 },
    cs237::color4ub{ 128, 128, 255, 255 },
    cs237::color4ub{ 255,   0,   0, 255 },
    cs237::color4ub{   0, 255,   0, 255 }
    };



/* The path to the shaders; this is usually set from the compiler command-line.
 * but it defaults to a path relative to the build directory.
 */
#ifndef SHADER_DIR
#  define SHADER_DIR "../shaders/"
#endif

//! Load a shader from the file system and compile and link it.  We keep
//! a cache of shaders that have already been loaded to allow two renderers
//! to share the same shader program without compiling it twice.
//
static cs237::ShaderProgram *LoadShader (std::string const & shaderPrefix, bool hasGeom = false)
{
    static std::map<std::string, cs237::ShaderProgram *> Shaders;

    auto it = Shaders.find(shaderPrefix);
    if (it == Shaders.end()) {
      // load, compile, and link the shader program
    cs237::VertexShader vsh((shaderPrefix + ".vsh").c_str());
    cs237::FragmentShader fsh((shaderPrefix + ".fsh").c_str());
    cs237::ShaderProgram *shader;
    if (hasGeom) {
        cs237::GeometryShader gsh((shaderPrefix + ".gsh").c_str());
        shader = new cs237::ShaderProgram (vsh, gsh, fsh);
    }
    else {
        shader = new cs237::ShaderProgram (vsh, fsh);
    }
    if (shader == nullptr) {
        std::cerr << "Cannot build " << shaderPrefix << std::endl;
        exit (1);
    }
    Shaders.insert (std::pair<std::string, cs237::ShaderProgram *>(shaderPrefix, shader));
    return shader;
    }
    else {
    return it->second;
    }

}


/***** virtual class Renderer member functions *****/

Renderer::Renderer (cs237::ShaderProgram *sh)
    : _shader(sh)
{ 
    //These uniforms are needed for all rendering modes
}

Renderer::~Renderer ()
{ }

/***** class WireframeRenderer member functions *****/

WireframeRenderer::WireframeRenderer ()
    : Renderer (LoadShader (SHADER_DIR "wireframeshader"))
{ 
  mvLoc = _shader->UniformLocation ("modelView");
  projLoc = _shader->UniformLocation("projection");
  colorLoc = _shader->UniformLocation("color");
}

WireframeRenderer::~WireframeRenderer ()
{ }

void WireframeRenderer::Enable (cs237::mat4f const &projectionMat)
{
    //Enable the Shader
    CS237_CHECK(glEnable(GL_DEPTH_TEST));
    _shader->Use();
    cs237::setUniform(projLoc, projectionMat);
}


void WireframeRenderer::Render(cs237::mat4f const &modelViewMat, Mesh *mesh, int type)
{
  CS237_CHECK(cs237::setUniform(mvLoc, modelViewMat));
  cs237::color4f color = cs237::color4f(mesh->color, 1); //need to change the color into a 4vector for shader programs
  cs237::setUniform(colorLoc, color);
  //printf("about to draw mesh\n");
  mesh->Draw(); 
}


//___________________________________________________
void Cell::InitTextures (View *view)
{
  // load textures
    if (this->_map->hasColorMap()) {
    this->_colorTQT = new TQT::TextureQTree (this->Datafile("/color.tqt").c_str());
    }
    if (this->_map->hasNormalMap()) {
    this->_normTQT = new TQT::TextureQTree (this->Datafile("/norm.tqt").c_str());
    }
#ifndef NDEBUG
    if ((this->_colorTQT != nullptr) && (this->_normTQT != nullptr)) {
    assert (this->_colorTQT->Depth() == this->_normTQT->Depth());
    }
#endif

}
