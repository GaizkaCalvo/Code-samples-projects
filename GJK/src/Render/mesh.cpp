#include "Helpers/pch.hpp"
#include "mesh.hpp"

namespace cs350 {
    /**
    * geter for the vao of the mesh
    * @return returnthe handle of the vao
    */
    GLuint mesh::vao_handle() const
    {
        return mVao;
    }
    /**
    * geter of the vertex vecter
    * @return the size of vertex vector
    */
    GLuint mesh::get_count() const
    {
        return static_cast<GLuint>(mMeshData.positions.size());
    }
    /**
    * we create the mesh with the data given
    * @param data data that we will use to create the mesh 
    */
    void mesh::create(mesh_data const & data)
    {
        mCreated = true;

        mMeshData = data;

        // VAO, 
        glGenVertexArrays(1, &mVao);
        
        // VBO
        glGenBuffers(1, &mVbo);
        

        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(mVao);
        //we bind the VBO to use it with the vertices that we need
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        //here we put the vertices in the VBO, and as its binded we dont need to specify anything
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mMeshData.positions.size(), &mMeshData.positions[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
    }
    /**
    * we load the data of the file and we create the mesh
    * @param filename name of the file
    */
    void mesh::load(const char* filename)
    {
        create(load_obj(filename));
    }
    void mesh::bind()
    {
        glBindVertexArray(mVao);
    }
    mesh::mesh() : mCreated(false)
    {
    }
    /**
    * constructor of the mesh
    * @param data data to create the mesh 
    */
    mesh::mesh(mesh_data const & data)
    {
        create(data);
    }
    /**
    * destructor of the mesh where we clean everything
    */
    mesh::~mesh()
    {
        if (!mCreated)
            return;
        mCreated = false;
        clear();
    }
    void mesh::clear()
    {
        // Delete the VAO
        glDeleteVertexArrays(1, &mVao);
        // Delete the VBOs
        glDeleteBuffers(1, &mVbo);
    }
}