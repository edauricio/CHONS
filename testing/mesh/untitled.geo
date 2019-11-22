//+
Point(1) = {-1, -1, 0, 1.0};
//+
Point(2) = {0, -0.8, 0, 1.0};
//+
Point(3) = {1, -1, 0, 1.0};
//+
BSpline(1) = {1, 2, 3};
//+
Recursive Delete {
  Curve{1}; 
}
//+
Point(1) = {-0, -1, 0, 1.0};
//+
Point(2) = {-0.5, -0.9, 0, 1.0};
//+
Point(3) = {-1, -1, 0, 1.0};
//+
Spline(1) = {3, 2, 1};
//+
Delete {
  Curve{1}; 
}
//+
BSpline(1) = {3, 2, 1};
//+
Delete {
  Curve{1}; 
}
//+
Bezier(1) = {3, 2, 1};
//+
Delete {
  Curve{1}; 
}
//+
Delete {
  Point{2}; 
}
//+
Point(4) = {-0.7, -0.9, 0, 1.0};
//+
Point(5) = {-0.4, -0.9, 0, 1.0};
//+
BSpline(1) = {3, 4, 5, 1};
//+
Delete {
  Curve{1}; 
}
//+
Spline(1) = {3, 4, 5, 1};
