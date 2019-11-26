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