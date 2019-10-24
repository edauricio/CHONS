// Gmsh project created on Thu Oct 24 13:21:54 2019
//+
SetFactory("OpenCASCADE");
Sphere(1) = {-0, 0, 0, 0.5, -Pi/2, Pi/2, 2*Pi};
//+
Physical Volume("sphere") = {1};
//+
Physical Surface("surf") = {1};
//+
Physical Curve("curv") = {2};
//+
Physical Point("points") = {1, 2};
