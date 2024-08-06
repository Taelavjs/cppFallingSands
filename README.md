Falling sand simulator using C++ 

TODO

Seperate functionality into classes - LGTM
- Element Base Class
- Moveable Solid
- Static Solid
- Liquid

REFACTOR IS REQUIRED - Pixel class desperately needs cleaning/seperating(?)
- Unnecessary repitition is common
- Common functionality repeated between differing classes(seperate moving classes for liquid and solid?)

CONST GLOBAL FOR GRAVITY(?)

COLOUR CHANGE BASED ON VELOCITY

RESET VELOCITY APPROPIATELY

TRANSFER Y VELOCITY INTO X AND VICE VERSA(or atleast make it look like it)

CURRENT PROBLEMS  : FILES ARE MESSY AND FLAMMABLE FUNCTIONALITY IS IN PIXEL(bad setup? avoids dynamic casting at least and currently works well)
