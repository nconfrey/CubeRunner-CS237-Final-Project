/*! \file mesh.hxx
 *
 * \author Nick Confrey
 */


#ifndef _MESH_HXX_
#define _MESH_HXX_

#include "cs237.hxx"

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

    Mesh (GLenum p);

  //! initialize the vertex data buffers for the mesh
    void LoadVertices (int nVerts, const cs237::vec3f *verts);

  //! initialize the element array for the mesh
    void LoadIndices (int n, const uint32_t *indices);

    //! initialize the texture-coordinate data buffer for the mesh
    void LoadTexCoords (int nCoords, cs237::vec2f *tcoords);

    //! initialize the normals data buffer for the mesh
    void LoadNormals (int nNorms, cs237::vec3f *norms);

  //! draw the mesh using a glDrawElements call
    void Draw ();

};

#endif // !_MESH_HXX_
