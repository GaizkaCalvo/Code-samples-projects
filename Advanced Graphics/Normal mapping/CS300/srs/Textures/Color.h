///
// Brief description: Class we will use to create the texture that we use on the program
///
#pragma once

class Color
{
public:
	Color(unsigned int  r, unsigned int g, unsigned int b, unsigned int a);
	~Color();

	static Color Red;
	static Color Blue;
	static Color BrightBlue;
	static Color Green;
	static Color Yellow;
	static Color Purple;

private:
	unsigned char mR;
	unsigned char mG;
	unsigned char mB;
	unsigned char mA;
};