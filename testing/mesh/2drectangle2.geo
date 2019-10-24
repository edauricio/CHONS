Point(1) = {-1, -1, 0, 1.0};
Point(2) = {1, -1, 0, 1.0};
Point(3) = {1, 1, 0, 1.0};
Point(4) = {-1, 1, 0, 1.0};
Point(5) = {-1, 0, 0, 1.0};
Point(6) = {0, 0, 0, 1.0};
Point(7) = {0, 1, 0, 1.0};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 7};
Line(4) = {7, 4};
Line(5) = {4, 5};
Line(6) = {5, 1};
Line(7) = {5, 6};
Line(8) = {6, 7};

Curve Loop(1) = {7, 8, 4, 5};
Plane Surface(1) = {1};
Curve Loop(2) = {1, 2, 3, -8, -7, 6};
Plane Surface(2) = {2};

//Transfinite Curve {2, 4} = 10 Using Progression 1;
//Transfinite Curve {1, 3} = 20 Using Progression 1;
//Transfinite Surface {1};

//Physical Curve("inflow") = {4};
//Physical Curve("outflow") = {2};
//Physical Curve("top") = {3};
//Physical Curve("bottom") = {1};
//+
Recombine Surface {1};
