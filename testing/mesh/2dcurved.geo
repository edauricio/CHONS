// Gmsh project created on Thu Oct 31 14:00:24 2019
//+
Point(1) = {-1, -1, 0, 1.0};
//+
Point(2) = {1, -1, 0, 1.0};
//+
Point(3) = {1, 1, 0, 1.0};
//+
Point(4) = {-1, 1, 0, 1.0};
//+
Point(5) = {0, -0.5, 0, 1.0};
//+
BSpline(1) = {1, 5, 2};
//+
Line(2) = {2, 3};
//+
Line(3) = {3, 4};
//+
Line(4) = {4, 1};
//+
Curve Loop(1) = {1, 2, 3, 4};
//+
Plane Surface(1) = {1};
//+
Physical Curve("in") = {4};
//+
Physical Curve("out") = {2};
//+
Physical Curve("top") = {3};
//+
Physical Curve("wall") = {1};
//+
Transfinite Curve {1, 3} = 10 Using Progression 1;
//+
Transfinite Curve {4, 2} = 20 Using Progression 1;
//+
Transfinite Surface {1};
//+
//+
Extrude {0, 0, 1} {
  Curve{1}; Curve{2}; Curve{3}; Curve{4}; Layers{2}; QuadTriAddVerts;
}
//+
Curve Loop(2) = {5, 9, 13, 17};
//+
Surface(21) = {2};
