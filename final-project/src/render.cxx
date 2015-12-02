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
#include "camera.hxx"
#include "map-cell.hxx"
#include "buffer-cache.hxx"
#include "texture-cache.hxx"
#include "render.hxx"
#include "view.hxx"
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

cs237::vec3f vec3dToVec3f2(cs237::vec3d d)
{
    return cs237::vec3f((float)d[0],
                 (float)d[1],
                 (float)d[2]);
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
  hscaleLoc = _shader->UniformLocation("hscale");
  vscaleLoc = _shader->UniformLocation("vscale");
  nwcornerLoc = _shader->UniformLocation("nw_pos");
}

WireframeRenderer::~WireframeRenderer ()
{ }

void WireframeRenderer::Enable (cs237::mat4f const &projectionMat, Sunlight sun)
{
    //Enable the Shader
    CS237_CHECK(glEnable(GL_DEPTH_TEST));
    _shader->Use();
    cs237::setUniform(projLoc, projectionMat);
}


void WireframeRenderer::Render(cs237::mat4f const &modelViewMat, Mesh *mesh)
{
  CS237_CHECK(cs237::setUniform(mvLoc, modelViewMat * mesh->toWorld));
  CS237_CHECK(cs237::setUniform(hscaleLoc, 1.0f));
  CS237_CHECK(cs237::setUniform(vscaleLoc, 1.0f));
  cs237::color4f color = cs237::color4f(mesh->color, 1); //need to change the color into a 4vector for shader programs
  cs237::setUniform(colorLoc, color);
  //printf("about to draw mesh\n");
  mesh->Draw(); 
}

void WireframeRenderer::RenderChunk(cs237::mat4f const &modelViewMat, VAO *vao, uniforms *u)
{
    cs237::color4f color = cs237::color4f(0.0, 0.313, 0.85, 1.0f);
    CS237_CHECK(cs237::setUniform(mvLoc, modelViewMat));
    CS237_CHECK(cs237::setUniform(hscaleLoc, u->hscale));
    CS237_CHECK(cs237::setUniform(vscaleLoc, u->vscale));
    CS237_CHECK(cs237::setUniform(colorLoc, color));
    CS237_CHECK(cs237::setUniform(nwcornerLoc, vec3dToVec3f2(u->nw_pos)));
    vao->Render();
}



/***** ________________ FullRenderer class ________________ *****/
FullRenderer::FullRenderer ()
    : Renderer (LoadShader (SHADER_DIR "shader"))
{
    //transformation uniforms
    mvLoc = _shader->UniformLocation ("modelView");
    projLoc = _shader->UniformLocation("projection");
    hscaleLoc = _shader->UniformLocation("hscale");
    vscaleLoc = _shader->UniformLocation("vscale");
    nwcornerLoc = _shader->UniformLocation("nw_pos");
    cellwidthLoc = _shader->UniformLocation("cellwidth");

    //texture sampling uniforms
    texSamplerLoc = _shader->UniformLocation("texSampler");
    normSamplerLoc = _shader->UniformLocation("normSampler");
    nwtileLoc = _shader->UniformLocation("nwtile");

    //directional lighting uniforms
    lightDirLoc = _shader->UniformLocation("lightDir");
    lightIntenLoc = _shader->UniformLocation("lightInten");
    lightAmbLoc = _shader->UniformLocation("lightAmb");

    //fog uniforms
    hasFogLoc = _shader->UniformLocation("hasFog");
    fogColorLoc = _shader->UniformLocation("fogColor");
    fogDensityLoc = _shader->UniformLocation("fogDensity");

}

FullRenderer::~FullRenderer()
{}

void FullRenderer::Enable(cs237::mat4f const &projectionMat, Sunlight sun) 
{
    CS237_CHECK(glEnable(GL_DEPTH_TEST));
    _shader->Use();
    cs237::setUniform(projLoc, projectionMat);
    //lighting uniforms
    CS237_CHECK(cs237::setUniform(lightDirLoc, sun.lightDir));
    CS237_CHECK(cs237::setUniform(lightIntenLoc, sun.lightInten));
    CS237_CHECK(cs237::setUniform(lightAmbLoc, sun.lightAmb));
}

void FullRenderer::Render(cs237::mat4f const &modelViewMat, Mesh *mesh)
{
  //transformation uniforms
  CS237_CHECK(cs237::setUniform(mvLoc, modelViewMat * mesh->toWorld));
  CS237_CHECK(cs237::setUniform(hscaleLoc, 1.0f));
  CS237_CHECK(cs237::setUniform(vscaleLoc, 1.0f));

  //sampler uniforms

  mesh->Draw(); 
}

void FullRenderer::RenderChunk(cs237::mat4f const &modelViewMat, VAO *vao, uniforms *u)
{
    //transformation uniforms
    CS237_CHECK(cs237::setUniform(mvLoc, modelViewMat));
    CS237_CHECK(cs237::setUniform(hscaleLoc, u->hscale));
    CS237_CHECK(cs237::setUniform(vscaleLoc, u->vscale));
    CS237_CHECK(cs237::setUniform(nwcornerLoc, vec3dToVec3f2(u->nw_pos)));
    CS237_CHECK(cs237::setUniform(cellwidthLoc, u->tw));

    //sampler uniforms
    CS237_CHECK(cs237::setUniform(texSamplerLoc, 0));
    CS237_CHECK(cs237::setUniform(normSamplerLoc, 1));
    CS237_CHECK(cs237::setUniform(nwtileLoc, vec3dToVec3f2(u->nw_tile)));  

    //fog uniforms
    CS237_CHECK(cs237::setUniform(hasFogLoc, u->hasfog));
    CS237_CHECK(cs237::setUniform(fogColorLoc, u->fogcolor));
    CS237_CHECK(cs237::setUniform(fogDensityLoc, u->fogdensity));

    vao->Render();
}

//___________________________________________________
void Cell::InitTextures ()
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
