/**
 * @file mesh.hpp
 * @author Gaizka Calvo (gaizka.c@digipen.edu)
 * @date 21/09/2020
 *
 * @copyright Copyright (c) 2020
 * @brief class that will have the information of the meshes
 */
#pragma once
#include "mesh_data.hpp"
#include "Helpers/opengl.hpp"

namespace cs350 {
    class mesh
    {
      private:
          GLuint mVao = {};
          GLuint mVbo = {};

          bool mCreated;
      public:
          GLuint vao_handle() const;
          GLuint get_count() const;
          
          void create(mesh_data const & data);
          void load(const char* filename);
          void bind();

          mesh_data mMeshData;

          mesh();
          explicit mesh(mesh_data const& data);
          ~mesh();
          void clear();
    };

}