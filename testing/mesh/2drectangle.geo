Point(1) = {-1, -1, 0, 1.0};
Point(2) = {1, -1, 0, 1.0};
Point(3) = {1, 1, 0, 1.0};
Point(4) = {-1, 1, 0, 1.0};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 1};

Curve Loop(1) = {1, 2, 3, 4};
Plane Surface(1) = {1};

Transfinite Curve {2, 4} = 10 Using Progression 1;
Transfinite Curve {1, 3} = 20 Using Progression 1;
Transfinite Surface {1};

Physical Curve("inflow") = {4};
Physical Curve("outflow") = {2};
Physical Curve("top") = {3};
Physical Curve("bottom") = {1};
