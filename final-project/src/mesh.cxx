/*! \file mesh.cxx
 *
 * \author Nick Confrey
 */

#include "mesh.hxx"

/*! The locations of the standard mesh attributes.  The layout directives in the shaders
 * should match these values.
 */
const GLint CoordAttrLoc = 0; //!< location of vertex coordinates attribute
const GLint NormAttrLoc = 1;  //!< location of vertex normal attribute
const GLint TexCoordAttrLoc = 2;  //!< location of texture coordinate attribute

//! create a MeshInfo object by initializing its buffer Ids.  The buffer data is
//! loaded separately.
Mesh::Mesh(GLenum p)
{
	CS237_CHECK(glGenVertexArrays (1, &vaoId));

    this->verticesVBOId = 0;
    this->indicesVBOId = 0 ;

    this->nIndices = 0;
    this->prim = p; //setting the primative
}

//! initialize the vertex data buffers for the mesh
void Mesh::LoadVertices (int nVerts, const struct Vertex *v)
{
	CS237_CHECK(glBindVertexArray(this->vaoId));
    CS237_CHECK(glGenBuffers(1, &this->verticesVBOId));
    
    //Set up the buffer so that it knows its holding vertice information
    CS237_CHECK(glBindBuffer(GL_ARRAY_BUFFER, this->verticesVBOId));
    //Now push the data into the buffer
    CS237_CHECK(glBufferData(GL_ARRAY_BUFFER, nVerts * sizeof(struct Vertex), v, GL_DYNAMIC_DRAW));

    //This describes the data as vertices
    //CoordAttrLoc is a location in memory
    CS237_CHECK(glVertexAttribPointer(CoordAttrLoc, 4, GL_SHORT, GL_FALSE, 0, ((GLvoid*) 0)));
    CS237_CHECK(glEnableVertexAttribArray(CoordAttrLoc));

    //unbind stuff here
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, 0) );
}

//! initialize the element array for the mesh
void Mesh::LoadIndices (int n, const uint16_t *indices)
{
    this->nIndices = n;
	CS237_CHECK( glBindVertexArray (this->vaoId) );
    CS237_CHECK( glGenBuffers (1, &this->indicesVBOId) );
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, this->indicesVBOId) );
    CS237_CHECK( glBufferData (GL_ELEMENT_ARRAY_BUFFER, n*sizeof(uint16_t), indices, GL_DYNAMIC_DRAW) );

    //unbind and cleanup
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0) );
}

//! initialize the texture-coordinate data buffer for the mesh
void Mesh::LoadTexCoords (int nCoords, cs237::vec2f *tcoords)
{
    CS237_CHECK(glBindVertexArray(this->vaoId));
    CS237_CHECK(glGenBuffers(1, &this->texVBOId));
    
    //Set up the buffer so that it knows its holding vertice information
    CS237_CHECK(glBindBuffer(GL_ARRAY_BUFFER, this->texVBOId));
    //Now push the data into the buffer
    CS237_CHECK(glBufferData(GL_ARRAY_BUFFER, nCoords * sizeof(cs237::vec2f), tcoords, GL_STATIC_DRAW));

    //This describes the data as vertices
    //CoordAttrLoc is a location in memory
    CS237_CHECK(glVertexAttribPointer(TexCoordAttrLoc, 3, GL_FLOAT, GL_FALSE, sizeof(tcoords[0]), ((GLvoid*) 0)));
    CS237_CHECK(glEnableVertexAttribArray(TexCoordAttrLoc));

    //unbind stuff here
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, 0) );
}

//! initialize the normals data buffer for the mesh
void Mesh::LoadNormals (int nNorms, cs237::vec3f *norms)
{
    CS237_CHECK(glBindVertexArray(this->vaoId));
    CS237_CHECK(glGenBuffers(1, &this->normVBOId));
    
    //Set up the buffer so that it knows its holding vertice information
    CS237_CHECK(glBindBuffer(GL_ARRAY_BUFFER, this->normVBOId));
    //Now push the data into the buffer
    CS237_CHECK(glBufferData(GL_ARRAY_BUFFER, nNorms * sizeof(cs237::vec3f), norms, GL_STATIC_DRAW));

    //This describes the data as vertices
    CS237_CHECK(glVertexAttribPointer(NormAttrLoc, 3, GL_FLOAT, GL_FALSE, sizeof(cs237::vec3f), ((GLvoid*) 0)));
    CS237_CHECK(glEnableVertexAttribArray(NormAttrLoc));

    //unbind stuff here
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, 0) );
}

    //set the color of the mesh
    void Mesh::SetColor(cs237::color3f c)
    {
        this->color = c;
    } 

    //set the world transformation matrix of the mesh
    void Mesh::SetToWorldMatrix(cs237::mat4f m)
    {
        this->toWorld = m;
    } 

    //set the texture of the mesh
    //NOTE: you still have to use LoadTexCoords
    void Mesh::SetTexture(cs237::texture2D * texture, cs237::image2d * image)
    {
        this->texture = texture;
        this->image = image;
    } 

    //set the position of the mesh
    void Mesh::SetPosition(cs237::vec3f pos)
    {
        this->pos = pos;
    }

void Mesh::Draw ()
{
    CS237_CHECK(glBindVertexArray(this->vaoId));
    CS237_CHECK(glBindBuffer(GL_ARRAY_BUFFER, this->verticesVBOId));
    CS237_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indicesVBOId));

    CS237_CHECK( glDrawElements (this->prim, this->nIndices, GL_UNSIGNED_SHORT, 0));
}

void Mesh::DrawFromVAOObj()
{
    CS237_CHECK(glBindVertexArray(this->vao->_id));
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, this->vao->_vBuf) );
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, this->vao->_iBuf) );
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(0xffff);
    CS237_CHECK( glDrawElements (this->prim, this->vao->_nIndices, GL_UNSIGNED_SHORT, 0));
    glDisable(GL_PRIMITIVE_RESTART);
}


