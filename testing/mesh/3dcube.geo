Point(1) = {-1, -1, 0, 1.0};
Point(2) = {1, -1, 0, 1.0};
Point(3) = {1, 1, 0, 1.0};
Point(4) = {-1, 1, 0, 1.0};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 1};

Curve Loop(1) = {1, 2, 3, 4};
Surface(1) = {1};

Extrude {0, 0, 2} {
  Surface{1};
}

Surface Loop(1) = {25, 1, 13, 17, 21, 26};
Volume(2) = {1};

//+
Transfinite Surface {26};
//+
Transfinite Surface {1};
//+
Transfinite Surface {25};
//+
Transfinite Surface {17};
//+
Transfinite Surface {21};
//+
Transfinite Surface {13};
//+
Recombine Surface {26};
//+
Recombine Surface {1};
//+
Recombine Surface {21};
//+
Recombine Surface {17};
//+
Recombine Surface {13};
//+
Recombine Surface {25};
//+
Transfinite Volume{2} = {1, 2, 3, 4, 5, 6, 10, 14};
//+
Physical Surface("in") = {26};
//+
Physical Surface("out") = {1};
//+
Physical Surface("per1") = {25};
//+
Physical Surface("per2") = {17};
//+
Physical Surface("top") = {21};
//+
Physical Surface("bot") = {13};
