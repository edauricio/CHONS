#include "catch2/include/catch.hpp"
#include "MathModule/LinAlgEntities.h"

TEST_CASE("Matrix constructors", "[matctors]") {
    int N = 4, M = 3;
    CHONS::Math::Matrix a(N);
    CHONS::Math::Matrix b(N,M);
    CHONS::Math::Matrix c{ {2, -1, 56, -1},
                           {4, 1, -2, -10},
                           {32, 55, -2, 3}
                          };
    CHONS::Math::Matrix d{c};

    REQUIRE(a.rows() == N);
    REQUIRE(a.cols() == N);

    REQUIRE(b.rows() == N);
    REQUIRE(b.cols() == M);

    REQUIRE(c.cols() == b.rows());
    REQUIRE(c.rows() == b.cols());

    REQUIRE(d == c);
}

TEST_CASE("Matrix assignments", "[matass]") {
    CHONS::Math::Matrix a{ {2, -1, 56, -1},
                           {4, 1, -2, -10},
                           {32, 55, -2, 3}
                          };
    CHONS::Math::Matrix b = a;

    REQUIRE(a == b);
}

TEST_CASE("Matrix addition", "[matadd]") {
    CHONS::Math::Matrix a{ {2, -1, 56, -1},
                           {4, 1, -2, -10},
                           {32, 55, -2, 3}
                          };
    CHONS::Math::Matrix b{ {12, 51, 2, -3},
                           {-55, 122, 98, 1},
                           {2, -3, 44, -11}
                          };

    CHONS::Math::Matrix result{ {14, 50, 58, -4},
                           {-51, 123, 96, -9},
                           {34, 52, 42, -8}
                          };

    REQUIRE(a + b == result);
}

TEST_CASE("Matrix subtraction", "[matsub]") {
    CHONS::Math::Matrix a{ {2, -1, 56, -1},
                           {4, 1, -2, -10},
                           {32, 55, -2, 3}
                          };
    CHONS::Math::Matrix b{ {12, 51, 2, -3},
                           {-55, 122, 98, 1},
                           {2, -3, 44, -11}
                          };

    CHONS::Math::Matrix result1{ {-10, -52, 54, 2},
                           {59, -121, -100, -11},
                           {30, 58, -46, 14}
                          };

    CHONS::Math::Matrix result2{ {10, 52, -54, -2},
                           {-59, 121, 100, 11},
                           {-30, -58, 46, -14}
                          };                          

    REQUIRE(a - b == result1);
    REQUIRE(b - a == result2);
}

TEST_CASE("Matrix-matrix multilpy", "[matmul]") {
    CHONS::Math::Matrix a{ {2, -1, 56, -1},
                           {4, 1, -2, -10},
                           {32, 55, -2, 3}
                          };
    CHONS::Math::Matrix b{ {12, 51, 2},
                           {-55, 122, 98},
                           {2, -3, 44},
                           {-3, 1, -11}
                          };

    CHONS::Math::Matrix result1{ {194, -189, 2381},
                           {19, 322, 128},
                           {-2654, 8351, 5333}
                          };

    CHONS::Math::Matrix result2{ {292, 149, 566, -516},
                           {3514, 5567, -3520, -871},
                           {1400, 2415, 30, 160},
                           {-354, -601, -148, -40}
                          };                          

    REQUIRE(a * b == result1);
    REQUIRE(b * a == result2);
}

TEST_CASE("Matrix scaling", "[matscal]") {
    CHONS::Math::Matrix a{ {2, -1, 56, -1},
                           {4, 1, -2, -10},
                           {32, 55, -2, 3}
                          };
    double scal = 3.0;

    CHONS::Math::Matrix result{ {6, -3, 168, -3},
                           {12, 3, -6, -30},
                           {96, 165, -6, 9}
                          };

    REQUIRE(scal * a == result);
    REQUIRE(a * scal == result);
}

TEST_CASE("Matrix-vector multiply", "[matvec]") {
    CHONS::Math::Matrix a{ {2, -1, 56, -1},
                           {4, 1, -2, -10},
                           {32, 55, -2, 3}
                          };

    CHONS::Math::Vector b{3, -15, 23, 4};
    CHONS::Math::Vector c{3, -15, 23};

    CHONS::Math::Vector result1{1305, -89, -763};

    CHONS::Math::Vector result2{682, 1247, 152, 216};

    REQUIRE(a * b == result1);
    REQUIRE(c * a == result2);
}

TEST_CASE("Transpose matrix-matrix multiply", "[trmatmul]") {
    CHONS::Math::Matrix a{ {2, -1, 56, -1},
                           {4, 1, -2, -10},
                           {32, 55, -2, 3}
                          };
    CHONS::Math::Matrix b{ {12, -55, 2, -3},
                           {51, 122, -3, 1},
                           {2, 98, 44, -11}
                          };

    CHONS::Math::Matrix result1{ {194, -189, 2381},
                                 {19, 322, 128},
                                 {-2654, 8351, 5333}
                               };

    CHONS::Math::Matrix result2{ {292, 3514, 1400, -354},
                                 {149, 5567, 2415, -601},
                                 {566, -3520, 30, -148},
                                 {-516, -871, 160, -40}
                               };

    REQUIRE(a * b.transpose() == result1);
    REQUIRE(a.transpose() * b == result2);
}