#include "Helpers/pch.hpp"
#include "mesh_data.hpp"

namespace cs350 {
    /**
    * we load the data of an obj and we create a mesh data with it
    * @param filename name of the file
    * @return we return the mesh data that we loaded in the .obj
    */
	mesh_data load_obj(const char* filename)
	{   
        //mesh we create
        mesh_data result;

        // OBJ data read
        std::vector< glm::vec2 > temp_uvs;
        std::vector< glm::vec3 > temp_normals;
        std::vector< glm::vec3 > temp_vertices;

        // Every combination of vertices to give to VBO indexer
        std::vector< glm::vec2 > in_uvs;
        std::vector< glm::vec3 > in_normals;
        std::vector< glm::vec3 > in_vertices;

        //to know how much data we need to take
        bool b_vertices = false;
        bool b_normal = false;
        bool b_uvs = false;

        // Open OBJ file
        std::ifstream inFile(filename);
        if (!inFile.is_open())
        {
            std::cout << "Failed opening " << filename << "to read the OBJ \n";
            //return false;
            exit(1);
        }

        // Read until it breaks on EOF
        while (!inFile.eof())
        {
            std::string lineHeader;
            // Read the first word of the line
            inFile >> lineHeader;

            // Read lineHeader
            if (lineHeader == "v")
            {
                b_vertices = true;

                std::string value;
                // Add new vertex
                glm::vec3 vertex;

                inFile >> value;
                vertex.x = static_cast<float>(std::atof(value.c_str()));
                inFile >> value;
                vertex.y = static_cast<float>(std::atof(value.c_str()));
                inFile >> value;
                vertex.z = static_cast<float>(std::atof(value.c_str()));

                temp_vertices.push_back(vertex);
            }
            else if (lineHeader == "vt")
            {
                b_uvs= true;

                std::string value;

                // Add new uv coordinate
                glm::vec2 uv;

                inFile >> value;
                uv.x = static_cast<float>(std::atof(value.c_str()));
                inFile >> value;
                uv.y = static_cast<float>(std::atof(value.c_str()));

                temp_uvs.push_back(uv);
            }
            else if (lineHeader == "vn")
            {
                b_normal = true;

                std::string value;
                // Add new normal
                glm::vec3 normal;

                inFile >> value;
                normal.x = static_cast<float>(std::atof(value.c_str()));
                inFile >> value;
                normal.y = static_cast<float>(std::atof(value.c_str()));
                inFile >> value;
                normal.z = static_cast<float>(std::atof(value.c_str()));

                temp_normals.push_back(normal);
            }
            else if (lineHeader == "f")
            {
                std::string faceLine;
                // Add new Face
                int vertexIndex[3] = { 0 };
                int uvIndex[3] = { 0 };
                int normalIndex[3] = { 0 };

                // Get whole face line
                std::getline(inFile, faceLine);

                if(b_vertices && !b_normal && !b_uvs)
                    ParseFace(faceLine, vertexIndex);

                if (b_vertices && b_normal && !b_uvs)
                    ParseFace(faceLine, vertexIndex, normalIndex);

                if (b_vertices && b_normal && b_uvs)
                    ParseFace(faceLine, vertexIndex, uvIndex, normalIndex);

                // Decrement indexes 
                vertexIndex[0]--;
                vertexIndex[1]--;
                vertexIndex[2]--;
                normalIndex[0]--;
                normalIndex[1]--;
                normalIndex[2]--;
                uvIndex[0]--;
                uvIndex[1]--;
                uvIndex[2]--;

                // Create a new combination of position, normal, uv
                if (vertexIndex[0] >= 0)
                {
                    in_vertices.push_back(temp_vertices[vertexIndex[0]]);
                    in_vertices.push_back(temp_vertices[vertexIndex[1]]);
                    in_vertices.push_back(temp_vertices[vertexIndex[2]]);
                    
                    std::vector<glm::vec3> new_triangle;
                    new_triangle.resize(3);
                    new_triangle[0] = temp_vertices[vertexIndex[0]];
                    new_triangle[1] = temp_vertices[vertexIndex[1]];
                    new_triangle[2] = temp_vertices[vertexIndex[2]];
                    result.triangles.push_back(new_triangle);
                }
                else
                {
                    in_vertices.push_back(glm::vec3(0.0, 0.0, 0.0));
                    in_vertices.push_back(glm::vec3(0.0, 0.0, 0.0));
                    in_vertices.push_back(glm::vec3(0.0, 0.0, 0.0));
                    std::cout << "Failed IN OBJ \n";
                    exit(1);
                }
                if (normalIndex[0] >= 0)
                {
                    in_normals.push_back(temp_normals[normalIndex[0]]);
                    in_normals.push_back(temp_normals[normalIndex[1]]);
                    in_normals.push_back(temp_normals[normalIndex[2]]);
                }
                else
                {
                    in_normals.push_back(glm::vec3(0.0, 0.0, 0.0));
                    in_normals.push_back(glm::vec3(0.0, 0.0, 0.0));
                    in_normals.push_back(glm::vec3(0.0, 0.0, 0.0));
                    //std::cout << "Failed IN OBJ \n";
                    //exit(1);
                }
                if (uvIndex[0] >= 0)
                {
                    in_uvs.push_back(temp_uvs[uvIndex[0]]);
                    in_uvs.push_back(temp_uvs[uvIndex[1]]);
                    in_uvs.push_back(temp_uvs[uvIndex[2]]);
                }
                else
                {
                    in_uvs.push_back(glm::vec2(0.0, 0.0));
                    in_uvs.push_back(glm::vec2(0.0, 0.0));
                    in_uvs.push_back(glm::vec2(0.0, 0.0));
                    //std::cout << "Failed IN OBJ \n";
                    //exit(1);
                }
            }
            else
            {
                // Probably a comment
                std::string uselessBuffer;
                std::getline(inFile, uselessBuffer);
            }
        }

        inFile.close();

        result.positions = in_vertices;
        result.normals = in_normals;
        result.uvs = in_uvs;


        return result;
}
    /**
    * helper function that we use to get the data
    * @param line the line of the file that we got
    * @param vertexIndex vertex vector where we put the data
    * @param uvIndex uv vector where we put the data
    * @param normalIndex normal vector where we put the data
    */
    void ParseFace(const std::string& line, int* vertexIndex, int* uvIndex, int* normalIndex)
    {
        // Full data is given
        int matches = sscanf(line.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);;
        if (matches != 9)
            std::cout << "Problem reading fromthe OBJ";
    }
    /**
    * helper function that we use to get the data
    * @param line the line of the file that we got
    * @param vertexIndex vertex vector where we put the data
    * @param normalIndex normal vector where we put the data
    */
    void ParseFace(const std::string& line, int* vertexIndex, int* normalIndex)
    {
        // Full data is given
        int matches = sscanf(line.c_str(), "%d//%d %d//%d %d//%d", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
        if (matches != 6)
            std::cout << "Problem reading fromthe OBJ";
    }
    /**
    * helper function that we use to get the data
    * @param line the line of the file that we got
    * @param vertexIndex vertex vector where we put the data
    */
    void ParseFace(const std::string& line, int* vertexIndex)
    { 
        // Full data is given
        int matches = sscanf(line.c_str(), "%d %d %d ", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
        if (matches != 3)
            std::cout << "Problem reading fromthe OBJ";
    }
}
