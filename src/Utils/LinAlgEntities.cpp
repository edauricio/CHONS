#include "Utils/LinAlgEntities.h"
#include <array>
#include <iostream>

// Blas library header (if using other than fortran BLAS)
#ifdef BLAS_OTHER

    #ifdef BLAS_OPENBLAS

        #include <openblas.h>

    #elif defined BLAS_CBLAS

        #include <cblas.h>

    #endif

#endif

namespace CHONS {

#ifndef BLAS_OTHER
    // extern "C" definitions
    extern "C" {extern void daxpy_(int*, double*, double*, 
                                        int*, double*, int*);}
    extern "C" {extern double ddot_(int*, double*, int*, double*, int*);}

#endif

// Vector::Vector() : s_isEmpty(true) {

// }

Vector::Vector(const size_t& sz) : s_size(sz), 
                                    s_elements(new double[s_size]{0}) {

}

Vector::Vector(const size_t& sz, double& in) : s_size(sz), 
                                            s_elements(new double[s_size]{in}) {

}

Vector::Vector(const std::initializer_list<double>& lst) : s_size(lst.size()),
                                                s_elements(new double[s_size]) {
    int i = 0;
    for (auto it = lst.begin(); it != lst.end(); ++it)
        s_elements[i++] = *it;
}

Vector::Vector(const Vector& vtc) : s_size(vtc.s_size), 
                                        s_elements(new double[s_size]) {
    for (int i = 0; i != s_size; ++i)
        s_elements[i] = vtc.s_elements[i];
}

Vector::Vector(Vector&& vtm) : s_size(vtm.s_size), 
                                s_elements(vtm.s_elements) {
    vtm.s_elements = nullptr;
}

Vector& Vector::operator=(const Vector& rhs) {
    s_size = rhs.s_size;
    s_elements = new double[s_size];

    // Copy RHS to LHS
    for (int i = 0; i != s_size; ++i)
        s_elements[i] = rhs.s_elements[i];

    return *this;
}

Vector& Vector::operator=(Vector&& rhs) {
    // Steal RHS data
    s_size = rhs.s_size;
    s_elements = rhs.s_elements;

    // Leave RHS in a "null state"
    rhs.s_elements = nullptr;

    return *this;
}

Vector::~Vector() {
    delete[] s_elements;
}

Vector Vector::operator+(const Vector& vsum) {
    if (s_size != vsum.size()) throw std::out_of_range("Unable to sum"
                    " vectors for unequal size.");
    double alpha = 1.0;
    int inc=1;
    Vector retval(vsum);
    daxpy_(&s_size, &alpha, s_elements, &inc, retval.s_elements, &inc);
    return retval;
}

double Vector::operator*(const Vector& vdot) {
    int inc=1;
    return ddot_(&s_size, s_elements, &inc, vdot.s_elements, &inc);
}

Vector Vector::operator*(const double& scalar) {
    Vector retval(s_size);
    for (int i = 0; i != s_size; ++i)
        retval[i] = s_elements[i]*scalar;
    return retval;
}

Vector operator*(const double& scalar, const Vector& vec) {
    Vector retval(vec.size());
    for (int i = 0; i != retval.size(); ++i)
        retval[i] = vec[i]*scalar;
    return retval;   
}

} // end of CHONS namespace