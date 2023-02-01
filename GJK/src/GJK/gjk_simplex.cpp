#include "Helpers/pch.hpp"
#include "gjk_simplex.h"
#include "Helpers/math.hpp"

void cs350::gjk_simplex::add_point(const glm::vec3& point)
{
    for (unsigned i = 0; i < m_simplex_active.size(); i++)
    {
        if (m_simplex_active[i] == 0)
        {
            m_simplex[i] = point;
            m_simplex_active[i] = 1;
            break;
        }
    }
}

void cs350::gjk_simplex::reduce()
{
    for (unsigned i = 0; i < m_simplex_active.size(); i++)
    {
        if (m_simplex_active[i] == 0)
        {
            for (unsigned j = i+1; j < m_simplex_active.size(); j++)
            {
                if (m_simplex_active[j] == 1)
                {
                    m_simplex_active[i] = 1;
                    m_simplex[i] = m_simplex[j];
                    m_simplex_active[j] = 0;
                    break;
                }
            }
        }
    }
}

glm::vec3 cs350::gjk_simplex::iterate()
{
    size_t size = point_count();
    glm::vec3 origin{ 0.0f, 0.0f, 0.0f };
    glm::vec3 result{ 0.0f, 0.0f, 0.0f };

    if (size == 0)
        std::cout << "ERRO with the simplex iteration \n";

    if (size == 1)
        return m_simplex[0];
    if (size == 2)
    {
        bool a, b;
        result = cs350::closest_point(m_simplex[0], m_simplex[1], origin, &a, &b);
        m_simplex_active[0] = a;
        m_simplex_active[1] = b;
    }
    if (size == 3)
    {
        bool a, b, c;
        result = closest_point(m_simplex[0], m_simplex[1], m_simplex[2], origin, &a, &b, &c);
        m_simplex_active[0] = a;
        m_simplex_active[1] = b;
        m_simplex_active[2] = c;
    }
    if (size == 4)
    {
        bool a, b, c, d;
        result = closest_point(m_simplex[0], m_simplex[1], m_simplex[2], m_simplex[3], origin, &a, &b, &c, &d);
        m_simplex_active[0] = a;
        m_simplex_active[1] = b;
        m_simplex_active[2] = c;
        m_simplex_active[3] = d;
    }
    return result;
}

/**
* @brief
*  Retrieves the closest point to the segment a-b
* @param a
*  Segment start
* @param b
*  Segment end
* @param pt
*  Point against to which to check
* @param a_used
*  Whether a contributes to the computation of the closest point
* @param b_used
*  Whether b contributes to the computation of the closest point
* @return vec3
*  The closest point in the segment a-b to pt (internal or vertex)
*/
glm::vec3 cs350::closest_point(glm::vec3 const& a, glm::vec3 const& b, glm::vec3 const& pt, bool* a_used, bool* b_used)
{
    //calculate where the closest point is located
    glm::vec3 ab = b - a;
    float t = glm::dot(pt - a, ab);

    //The point is outside, closer to a in the segment
    if (t <= 0.0f)
    {
        t = 0.0f;
        *a_used = true;
        *b_used = false;

        return a;
    }

    float tdiv = glm::dot(ab, ab);

    //The point is outside, closer to b in the segment
    if (t >= tdiv)
    {
        t = 1.0f;
        *a_used = false;
        *b_used = true;

        return b;
    }

    //The point is inside, between the points a and b
    else
    {
        t = t / tdiv;

        *a_used = true;
        *b_used = true;

        return a + t * ab;
    }
}

/**
* @brief
*  Retrieves the closest point in the triangle abc to the point pt
* @param a
*  Triangle point
* @param b
*  Triangle point
* @param c
*  Triangle point
* @param pt
*  Point against to which to check
* @param a_used
*  Whether the point A is used to compute the closest point
* @param b_used
*  Whether the point B is used to compute the closest point
* @param c_used
*  Whether the point C is used to compute the closest point
* @return vec3
*  The closest point in the triangle ABC to pt. (internal, edge, or vertex)
*/
glm::vec3 cs350::closest_point(glm::vec3 const& a, glm::vec3 const& b, glm::vec3 const& c, glm::vec3 const& pt, bool* a_used, bool* b_used, bool* c_used) 
{
    glm::vec3 ab = b - a;
    glm::vec3 ac = c - a;
    glm::vec3 apt = pt - a;

    float d1 = glm::dot(ab, apt);
    float d2 = glm::dot(ac, apt);
    if (d1 <= cEpsilon && d2 <= cEpsilon)
    {
        *a_used = true;
        *b_used = false;
        *c_used = false;
        return a; // barycentric coordinates (1,0,0)
    }

    // Check if P in vertex region outside B
    glm::vec3 bpt = pt - b;
    float d3 = glm::dot(ab, bpt);
    float d4 = glm::dot(ac, bpt);
    if (d3 >= -cEpsilon && d4 <= d3)
    {
        *a_used = false;
        *b_used = true;
        *c_used = false;
        return b; // barycentric coordinates (0,1,0)
    }
    // Check if P in edge region of AB, if so return projection of P onto AB
    float vc = d1 * d4 - d3 * d2;
    if (vc <= cEpsilon && d1 >= -cEpsilon && d3 <= cEpsilon)
    {
        *a_used = true;
        *b_used = true;
        *c_used = false;
        float v = d1 / (d1 - d3);
        return a + v * ab; // barycentric coordinates (1-v,v,0)
    }
    // Check if P in vertex region outside C
    glm::vec3 cp = pt - c;
    float d5 = glm::dot(ab, cp);
    float d6 = glm::dot(ac, cp);
    if (d6 >= -cEpsilon && d5 <= d6)
    {
        *a_used = false;
        *b_used = false;
        *c_used = true;
        return c; // barycentric coordinates (0,0,1)
    }

    // Check if P in edge region of AC, if so return projection of P onto AC
    float vb = d5 * d2 - d1 * d6;
    if (vb <= cEpsilon && d2 >= -cEpsilon && d6 <= cEpsilon)
    {
        *a_used = true;
        *b_used = false;
        *c_used = true;
        float w = d2 / (d2 - d6);
        return a + w * ac; // barycentric coordinates (1-w,0,w)
    }
    // Check if P in edge region of BC, if so return projection of P onto BC
    float va = d3 * d6 - d5 * d4;
    if (va <= cEpsilon && (d4 - d3) >= -cEpsilon && (d5 - d6) >= -cEpsilon)
    {
        *a_used = false;
        *b_used = true;
        *c_used = true;
        float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        return b + w * (c - b); // barycentric coordinates (0,1-w,w)
    }

    *a_used = true;
    *b_used = true;
    *c_used = true;
    // P inside face region. Compute Q through its barycentric coordinates (u,v,w)
    float denom = 1.0f / (va + vb + vc);
    float v = vb * denom;
    float w = vc * denom;
    return a + ab * v + ac * w; // = u*a + v*b + w*c, u = va * denom = 1.0f-v-w
}

// Test if point p lies outside plane through abc
int cs350::PointOutsideOfPlane(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& p)
{
    return glm::dot(p - a, glm::cross(b - a, c - a)) >= -cEpsilon; // [AP AB AC] >= 0
}

/**
* @brief
*  Computes the closest point in the tetrahedron ABCD to pt.
*  NOTE: The winding of the tetrahedron is arbitrary!
* @param a
*  A point in the tetrahedron
* @param b
*  B point in the tetrahedron
* @param c
*  C point in the tetrahedron
* @param d
*  D point in the tetrahedron
* @param pt
*  Point to check against
* @param a_used
*  Whether the point A is used to compute the closest point
* @param b_used
*  Whether the point B is used to compute the closest point
* @param c_used
*  Whether the point C is used to compute the closest point
* @param d_used
*  Whether the point D is used to compute the closest point
* @return vec3
*  The closest point in the tetrahedron ABCD to pt (internal, face, edge or vertex)
*/
glm::vec3 cs350::closest_point(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, glm::vec3 const& pt, bool* a_used, bool* b_used, bool* c_used, bool* d_used)
{
    bool used_a = false;
    bool used_b = false;
    bool used_c = false;
    bool used_d = false;

    // Start out assuming point inside all halfspaces, so closest to itself
    glm::vec3 closestPt = pt;
    float bestSqDist = FLT_MAX;
    // If point outside face abc then compute closest point on abc
    if (PointOutsideOfPlane(a, b, c, pt))
    {
        glm::vec3 q = closest_point(a, b, c, pt, &used_a, &used_b, &used_c);
        float sqDist = glm::dot(q - pt, q - pt);
        // Update best closest point if (squared) distance is less than current best
        if (sqDist < bestSqDist)
        {
            bestSqDist = sqDist;
            closestPt = q;

            *a_used = used_a;
            *b_used = used_b;
            *c_used = used_c;
            *d_used = false;
        }
    }
    if (PointOutsideOfPlane(a, c, b, pt))
    {
        glm::vec3 q = closest_point(a, c, b, pt, &used_a, &used_c, &used_b);
        float sqDist = glm::dot(q - pt, q - pt);
        // Update best closest point if (squared) distance is less than current best
        if (sqDist < bestSqDist)
        {
            bestSqDist = sqDist;
            closestPt = q;

            *a_used = used_a;
            *b_used = used_b;
            *c_used = used_c;
            *d_used = false;
        }
    }

    // Repeat test for face acd
    if (PointOutsideOfPlane(a, c, d, pt))
    {
        glm::vec3 q = closest_point(a, c, d, pt, &used_a, &used_c, &used_d);
        float sqDist = glm::dot(q - pt, q - pt);
        if (sqDist < bestSqDist)
        {
            bestSqDist = sqDist;
            closestPt = q;

            *a_used = used_a;
            *b_used = false;
            *c_used = used_c;
            *d_used = used_d;
        }
    }
    if (PointOutsideOfPlane(a, d, c, pt))
    {
        glm::vec3 q = closest_point(a, d, c, pt, &used_a, &used_d, &used_c);
        float sqDist = glm::dot(q - pt, q - pt);
        if (sqDist < bestSqDist)
        {
            bestSqDist = sqDist;
            closestPt = q;

            *a_used = used_a;
            *b_used = false;
            *c_used = used_c;
            *d_used = used_d;
        }
    }

    // Repeat test for face adb
    if (PointOutsideOfPlane(a, d, b, pt))
    {
        glm::vec3 q = closest_point(a, d, b, pt, &used_a, &used_d, &used_b);
        float sqDist = glm::dot(q - pt, q - pt);
        if (sqDist < bestSqDist)
        {
            bestSqDist = sqDist;
            closestPt = q;

            *a_used = used_a;
            *b_used = used_b;
            *c_used = false;
            *d_used = used_d;
        }
    }
    if (PointOutsideOfPlane(a, b, d, pt))
    {
        glm::vec3 q = closest_point(a, d, b, pt, &used_a, &used_b, &used_d);
        float sqDist = glm::dot(q - pt, q - pt);
        if (sqDist < bestSqDist)
        {
            bestSqDist = sqDist;
            closestPt = q;

            *a_used = used_a;
            *b_used = used_b;
            *c_used = false;
            *d_used = used_d;
        }
    }

    // Repeat test for face bdc
    if (PointOutsideOfPlane(b, d, c, pt))
    {
        glm::vec3 q = closest_point(b, d, c, pt, &used_b, &used_d, &used_c);
        float sqDist = glm::dot(q - pt, q - pt);
        if (sqDist < bestSqDist)
        {
            bestSqDist = sqDist;
            closestPt = q;

            *a_used = false;
            *b_used = used_b;
            *c_used = used_c;
            *d_used = used_d;
        }
    }
    if (PointOutsideOfPlane(b, c, d, pt))
    {
        glm::vec3 q = closest_point(b, c, d, pt, &used_b, &used_c, &used_d);
        float sqDist = glm::dot(q - pt, q - pt);
        if (sqDist < bestSqDist)
        {
            bestSqDist = sqDist;
            closestPt = q;

            *a_used = false;
            *b_used = used_b;
            *c_used = used_c;
            *d_used = used_d;
        }
    }

    //check if the point is inside the tetraedron
    glm::vec3 vap = pt - a;
    glm::vec3 vbp = pt - b;

    glm::vec3 vab = b - a;
    glm::vec3 vac = c - a;
    glm::vec3 vad = d - a;

    glm::vec3 vbc = c - b;
    glm::vec3 vbd = d - b;
    // ScTP computes the scalar triple product
    float va6 = glm::dot(vbp, glm::cross(vbd, vbc));
    float vb6 = glm::dot(vap, glm::cross(vac, vad));
    float vc6 = glm::dot(vap, glm::cross(vad, vab));
    float vd6 = glm::dot(vap, glm::cross(vab, vac));
    float v6 = 1 / glm::dot(vab, glm::cross(vac, vad));
    glm::vec4 bari_coord = glm::vec4(va6 * v6, vb6 * v6, vc6 * v6, vd6 * v6);

    if (bari_coord.x > -cEpsilon && bari_coord.y > -cEpsilon && bari_coord.z > -cEpsilon && bari_coord.w > -cEpsilon)
        return {};

    return closestPt;
}
