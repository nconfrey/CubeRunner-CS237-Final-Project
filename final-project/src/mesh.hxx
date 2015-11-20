/*! \file mesh.hxx
 *
 * \author Nick Confrey
 */


#ifndef _MESH_HXX_
#define _MESH_HXX_

#include "cs237.hxx"
#include "map-cell.hxx"
#include "buffer-cache.hxx"

//! the information needed to render a mesh
struct Mesh {

    GLuint    vaoId;
    GLuint    verticesVBOId;
    GLuint    indicesVBOId;
    GLuint    normVBOId;
    GLuint    texVBOId;

    GLuint    nIndices;
    GLenum    prim; //usually GL_TRIANGLES

    cs237::color3f color;
    cs237::vec3f    pos;    //!< the world-space coordinates of the object
    cs237::mat4f toWorld;

    cs237::texture2D * texture;
    cs237::image2d * image;
    cs237::color3f  ambient;

    VAO *vao;

    Mesh (GLenum p);

  //! initialize the vertex data buffers for the mesh
    void LoadVertices (int nVerts, const struct Vertex *v);

  //! initialize the element array for the mesh
    void LoadIndices (int n, const uint16_t *indices);

    //! initialize the texture-coordinate data buffer for the mesh
    void LoadTexCoords (int nCoords, cs237::vec2f *tcoords);

    //! initialize the normals data buffer for the mesh
    void LoadNormals (int nNorms, cs237::vec3f *norms);

  //! draw the mesh using a glDrawElements call
    void Draw ();

    //! draw the mesh assuming the vao was created using Reppy's code
    void DrawFromVAOObj();

    //set the color of the mesh
    void SetColor(cs237::color3f c); 

    //set the world transformation matrix of the mesh
    void SetToWorldMatrix(cs237::mat4f m); 

    //set the texture of the mesh
    //NOTE: you still have to use LoadTexCoords
    void SetTexture(cs237::texture2D * texture, cs237::image2d * image); 

    //set the position of the mesh
    void SetPosition(cs237::vec3f pos);

};

#endif // !_MESH_HXX_
