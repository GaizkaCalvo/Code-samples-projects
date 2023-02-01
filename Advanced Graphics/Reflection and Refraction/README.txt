/* ---------------------------------------------------------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen 
Institute of Technology is prohibited.
Project: cs300_gaizka.c_2
Author: Gaizka Calvo, gaizka.c
Creation date: 21/06/2020
----------------------------------------------------------------------------------------------------------*/

•	Reflection and refraction Demo inside the folder CS300.

•	How to run your program: 
	Platform selected in x64 mode

•	How to use your program:
	- Move the camera around in always looking at the object.
	W: Move up.
	S: Move down.
	A: Move left.
	D: Move right.
	E: Further from object.
	Q: Closer to object.
	
	- Select the light types: 
	P: Toggle to pause/start the light animation.
	
	- Select shape to be rendered through the number keys.
     	Numbers 1 to 5: Change the shape to be rendered
        1: Plane
        2: Cube
        3: Cone
        4: Cylinder
        5: Sphere
	+: Increase the shape subdivisions
	-: Decrease the shape subdivisions

	P: Toggle to pause/start the light animation
	O: Toggle to pause/start adjacent object animation
	N: Toggle normal rendering
	F: Toggle face/averaged normal
	M: Toggle wireframe mode on/off
	B: Toggle UV texturing on/off (No lights, for debugging purspose)
	T: Toggle the reflection refraction

        - Object rotation for center shape.
    	Arrows Up/Down: Rotate the shape along Y-axis
    	Arrows Right/Left: Rotate the shape along X-axis

•	Important parts of the code: 
	In Light.cpp you can change the parameters that are used to compute the light (Constructor of Light)
	In GameObject.cpp you can change the material parameters used to compute lights (Constructor)

•	Known issues and problems: 
	
	If the scale of the object is not uniform, some of the normals will not be correct