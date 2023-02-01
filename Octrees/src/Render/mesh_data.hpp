/**
 * @file mesh_data.hpp
 * @author Gaizka Calvo (gaizka.c@digipen.edu)
 * @date 21/09/2020
 *
 * @copyright Copyright (c) 2020
 * @brief class that will have the information of the meshes verteces and more
 */
#pragma once
#include "Debug/geometry.hpp" // triangle

namespace cs350 {
    struct mesh_data
    {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;
        //std::vector< std::array<glm::vec3, 3> > triangles;
        std::vector<std::vector<glm::vec3>> triangles;
        //std::vector<glm::vec3> tangent;
        //std::vector<glm::vec3> bitangent;
    };

    mesh_data load_obj(const char* filename);
    std::vector<mesh_data> load_objs(const char* filename);
    void ParseFace(const std::string& line, int* vertexIndex, int* uvIndex, int* normalIndex);
    void ParseFace(const std::string& line, int* vertexIndex, int* normalIndex);
    void ParseFace(const std::string& line, int* vertexIndex);
}