Point(1) = {-1, -1, 0, 1.0};
Point(2) = {1, -1, 0, 1.0};
Point(3) = {-1, 1, 0, 1.0};
Point(4) = {1, 1, 0, 1.0};

Line(1) = {1, 2};
Line(2) = {2, 4};
Line(3) = {3, 4};
Line(4) = {3, 1};

Transfinite Curve {1, 3} = 3 Using Progression 1;
Transfinite Curve {4, 2} = 3 Using Progression 1;

Curve Loop(1) = {1, 2, -3, 4};
Surface(1) = {1};

Transfinite Surface {1};
