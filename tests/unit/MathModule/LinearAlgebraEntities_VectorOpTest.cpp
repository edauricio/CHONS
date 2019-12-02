#include "catch2/include/catch.hpp"
#include "MathModule/LinAlgEntities.h"

TEST_CASE("Vector subscription", "[vecsubsc]") {
    CHONS::Math::Vector a{ 1, 6, 3, 7};

    REQUIRE(a[0] == 1);
    REQUIRE(a[1] == 6);
    REQUIRE(a[2] == 3);
    REQUIRE(a[3] == 7);
}

TEST_CASE("Vector constructors", "[vecctors]") {
    CHONS::Math::Vector a{1, 6, 7, 3};
    CHONS::Math::Vector b{a};
    CHONS::Math::Vector c = b;

    REQUIRE(b == a);
    REQUIRE(c == a);
}

TEST_CASE("Vector addition", "[vecadd]") {
    CHONS::Math::Vector a{ 1, 6, 3, 7};
    CHONS::Math::Vector b{ 3, 7, 1, 3};

    CHONS::Math::Vector result{ 4, 13, 4, 10};

    REQUIRE( a + b == result );
}

TEST_CASE("Vector subtraction", "[vecsub]") {
    CHONS::Math::Vector a{ 1, 6, 3, 7};
    CHONS::Math::Vector b{ 3, 7, 1, 3};

    CHONS::Math::Vector result{ -2, -1, 2, 4};

    REQUIRE( a - b == result );
}

TEST_CASE("Vector scaling", "[vecscal]") {
    CHONS::Math::Vector a{ 1, 6, 3, 7};
    double b = 3.0;

    CHONS::Math::Vector result{ 3, 18, 9, 21};

    REQUIRE(b*a == result);
    REQUIRE(a*b == result);
}

TEST_CASE("Vector dot product", "[vdot]") {
    CHONS::Math::Vector a{ 1, 6, 3, 7};
    CHONS::Math::Vector b{ 3, 7, 1, 3};
    double result = 69.0;

    REQUIRE(dot_product(a,b) == result);
    REQUIRE(dot_product(b,a) == result);
}

TEST_CASE("Vector rank upgrade", "[vecrankup]") {
    CHONS::Math::Vector a{ 1, 6, 3, 7};
    CHONS::Math::Vector b{ 3, 7, 1, 3};
    CHONS::Math::Matrix result1{ { 3, 7, 1, 3} ,
                                { 18, 42, 6, 18} ,
                                { 9, 21, 3, 9} ,
                                { 21, 49, 7, 21}
                              };

    CHONS::Math::Matrix result2{ { 3, 18, 9, 21} ,
                                { 7, 42, 21, 49} ,
                                { 1, 6, 3, 7} ,
                                { 3, 18, 9, 21}
                              };                              

    REQUIRE(rank_up(a,b) == result1);
    REQUIRE(rank_up(b,a) == result2);
}