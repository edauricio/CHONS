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

