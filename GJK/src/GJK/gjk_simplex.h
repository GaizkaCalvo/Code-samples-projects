#pragma once
#include <bitset>
#include <glm\fwd.hpp>
#include <glm\fwd.hpp>
#include <glm\ext\vector_float3.hpp>

namespace cs350
{

    class gjk_simplex
    {
    private:
        std::array<glm::vec3, 4> m_simplex{};
        std::bitset<4>      m_simplex_active{};

        // A X X D
        // 1 0 0 1

        // A D X X
        // 1 1 0 0

    public:

        void add_point(const glm::vec3& point);
        void reduce(); // Remove unused points
        glm::vec3 iterate();

        [[nodiscard]] size_t                     point_count() const noexcept { return m_simplex_active.count(); }
        [[nodiscard]] const std::array<glm::vec3, 4>& points() const noexcept { return m_simplex; }


    };



    glm::vec3 closest_point(glm::vec3 const& a, glm::vec3 const& b, glm::vec3 const& pt, bool* a_used, bool* b_used);
    glm::vec3 closest_point(glm::vec3 const& a, glm::vec3 const& b, glm::vec3 const& c, glm::vec3 const& pt, bool* a_used, bool* b_used, bool* c_used);

    int PointOutsideOfPlane(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& p);
    glm::vec3 closest_point(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, glm::vec3 const& pt, bool* a_used, bool* b_used, bool* c_used, bool* d_used);
}
