#include "Color.h"

Color Color::Red(255, 0, 0, 0);
Color Color::Green(0, 255, 0, 0);
Color Color::BrightBlue(0, 255, 255, 0);
Color Color::Blue(0, 0, 255, 0);
Color Color::Purple(255, 0, 255, 0);
Color Color::Yellow(255, 255, 0, 0);

Color::Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
	mR = static_cast<unsigned char>(r);
	mG = static_cast<unsigned char>(g);
	mB = static_cast<unsigned char>(b);
	mA = static_cast<unsigned char>(a);
}

Color::~Color()
{
}
