#include "Utils/LinAlgEntities.h"
#include "blaspp/include/blas.hh"
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

    // BLAS
    extern "C" {extern void dcopy_(int*, double*, int*, double*, int*);}
    extern "C" {extern void dscal_(int*, double*, double*, int*);}
    extern "C" {extern void daxpy_(int*, double*, double*, 
                                                    int*, double*, int*);}
    extern "C" {extern double ddot_(int*, double*, int*, double*, int*);}

    // LAPACK
    extern "C" {extern void dlacpy_(char*, int*, int*, double*, 
                                                    int*, double*, int*);}

#endif

// ---------- Vector Member Function Definitions --------- //

Vector::Vector(const int& sz) : s_size(sz), 
                                    s_elements(new double[s_size]{0}) {

}

Vector::Vector(const int& sz, const double& in) : s_size(sz), 
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
    // Copy vtc into *this
    int inc=1;
    dcopy_(&s_size, vtc.s_elements, &inc, s_elements, &inc);
}

Vector::Vector(Vector&& vtm) : s_size(vtm.s_size), 
                                s_elements(vtm.s_elements) {
    vtm.s_elements = nullptr;
}

Vector& Vector::operator=(const Vector& rhs) {
    // Temp. variable to hold lhs elements
    double *temp = s_elements;

    // TODO: Check if data are already of same size, so no memory allocation is needed
    if (s_elements != rhs.s_elements) {
        s_size = rhs.s_size;
        s_elements = new double[s_size];
        // Copy RHS into LHS
        int inc=1;
        dcopy_(&s_size, rhs.s_elements, &inc, s_elements, &inc);
        delete[] temp;
    }

    return *this;
}

Vector& Vector::operator=(Vector&& rhs) {
    // Temp. variable to hold lhs elements;
    double *temp = s_elements;

    // Steal RHS data
    s_size = rhs.s_size;
    s_elements = rhs.s_elements;

    // Delete old data (if not in a self-assignment)
    if (temp != s_elements) delete[] temp;

    // Leave RHS in a "null state"
    rhs.s_elements = nullptr;

    return *this;
}

Vector::~Vector() {
    delete[] s_elements;
}

Vector Vector::operator+(const Vector& vsum) {
    if (s_size != vsum.size()) throw std::out_of_range("Unable to sum"
                    " vectors of unequal size.");
    double alpha = 1.0;
    int inc=1;
    Vector retval(vsum);
    // daxpy_(&s_size, &alpha, s_elements, &inc, retval.data(), &inc);
    blas::axpy(s_size, 1.0, s_elements, 1, retval.data(), 1);
    return retval;
}

double Vector::operator*(const Vector& vdot) {
    if (s_size != vdot.size()) throw std::out_of_range("Unable to calculate"
                    "dot product for vectors of unequal size.");
    int inc=1;
    return ddot_(&s_size, s_elements, &inc, vdot.data(), &inc);
}

Vector Vector::operator*(const double& scalar) {
    int inc=1;
    double alpha=scalar;
    Vector retval(*this);
    dscal_(&s_size, &alpha, retval.data(), &inc);
    return retval;
}

Vector operator*(const double& scalar, const Vector& vec) {
    int inc=1, sz=vec.size();
    double alpha=scalar;
    Vector retval(vec);
    dscal_(&sz, &alpha, retval.data(), &inc);;
    return retval;   
}

// ---------- End of Vector Member Function Definitions --------- //

// ---------- VectorIterartor Member Function Definitions --------- //

Vector::VectorIterator::VectorIterator(const VectorIterator& vic) {
    element = vic.element;
}

Vector::VectorIterator::VectorIterator(VectorIterator&& vim) {
    element = vim.element;
    vim.element = nullptr;
}

Vector::VectorIterator& Vector::VectorIterator::operator=(
                                                const VectorIterator& vic) {
    element = vic.element;
    return *this;
}

Vector::VectorIterator& Vector::VectorIterator::operator=(VectorIterator&& vim) {
    element = vim.element;
    vim.element = nullptr;   
    return *this;
}

// ---------- End of VectorIterator Member Function Definitions --------- //


// ---------- Matrix Member Function Definitions --------- //

Matrix::Matrix(const int& m, const int& n) : s_rowSize(m), s_colSize(n), 
                                 s_elements(new double[s_rowSize*s_colSize]) {

}

Matrix::Matrix(const std::initializer_list<
                                    std::initializer_list<double>>& rows_cols) :
                        s_rowSize(rows_cols.size()), 
                        s_colSize(rows_cols.begin()->size()),
                        s_elements(new double[s_rowSize*s_colSize]) {
    check_ctor_lists_size(rows_cols);

    int row_ind = 0, col_ind = 0;
    for (auto row : rows_cols) {
        for (auto col_ele : row) {
            s_elements[row_ind*s_colSize + col_ind++] = col_ele;
        }
        row_ind++;
        col_ind=0;
    }
}

Matrix::Matrix(const Matrix& mc) : s_rowSize(mc.s_rowSize), 
                                s_colSize(mc.s_colSize),
                                s_elements(new double[s_rowSize*s_colSize]) {
    // Copy MC elements into *this
    char uplo='A';
    dlacpy_(&uplo, &s_rowSize, &s_colSize, mc.data(), 
            &s_rowSize, s_elements, &s_rowSize);
}

Matrix::Matrix(Matrix&& mm) : s_rowSize(mm.s_rowSize), 
                                s_colSize(mm.s_colSize),
                                s_elements(mm.s_elements) {
    // Leave mm in a "null state"
    mm.s_elements = nullptr;
}

Matrix& Matrix::operator=(const Matrix& mca) {
    // Check if it's not a self-assignment
    if (s_elements != mca.s_elements) {
        // If total size of data aren't the same, need to reallocate memory
        if ((s_rowSize*s_colSize) != (mca.s_rowSize*mca.s_colSize)) {
            delete[] s_elements;
            s_elements = new double[s_rowSize*s_colSize];
        } else { // otherwise, s_elements already has enough memory allocated
            s_rowSize = mca.s_rowSize;
            s_colSize = mca.s_colSize;
        }

        // Copy RHS into LHS
        char uplo='A';
        dlacpy_(&uplo, &s_rowSize, &s_colSize, mca.data(), 
            &s_rowSize, s_elements, &s_rowSize);

    }

    return *this;
}

Matrix::~Matrix() {
    if (s_elements) delete[] s_elements;
}

void Matrix::check_ctor_lists_size(const std::initializer_list<
                                    std::initializer_list<double>>& rows_cols) {
    for (auto row : rows_cols)
        if (row.size() != s_colSize)
            throw std::invalid_argument("Matrix construct: number of elements"
            " in each row must match.");
}


// ---------- End of Matrix Member Function Definitions --------- //

} // end of CHONS namespace