#version 440 core

out vec4 out_color;

layout(location = 1) uniform vec4 uniform_color;

void main()
{
    //out_color = vec4(uniform_color.x, uniform_color.y, uniform_color.z, uniform_color.w / 2);

    //if (gl_FrontFacing) // is the fragment part of a front face?
    //    out_color = uniform_color;
    //else
    //    out_color = uniform_color * 0.1f;

    out_color = uniform_color;
}
