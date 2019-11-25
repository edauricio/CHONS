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
    // These C implemenetations are coming from libblas.so that was
    // compiled when building OpenBLAS.
    // If a system only has the legacy BLAS (i.e. F77) installed, then
    // these will probably change (or libg2c.so must be used)
    // CMake phase should catch these peculiarities.

    // UPDATE: Use of BLAS++ API interface eliminates the need to hard-code
    // these declarations in advance, since the header file blas.hh correctly
    // does it for a number of vendor-optimized, BLAS implementations (through 
    // its identification in CMake phase and definition using Macros)

    // extern "C" {extern void dcopy_(int*, double*, int*, double*, int*);}
    // extern "C" {extern void dscal_(int*, double*, double*, int*);}
    // extern "C" {extern void daxpy_(int*, double*, double*, 
    //                                                 int*, double*, int*);}
    // extern "C" {extern double ddot_(int*, double*, int*, double*, int*);}

    // // LAPACK
    // extern "C" {extern void dlacpy_(char*, int*, int*, double*, 
                                                    // int*, double*, int*);}

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
    blas::copy(s_size, vtc.s_elements, 1, s_elements, 1);
}

Vector::Vector(Vector&& vtm) : s_size(vtm.s_size), 
                                s_elements(vtm.s_elements) {
    vtm.s_elements = nullptr;
}

Vector& Vector::operator=(const Vector& rhs) {
    if (s_elements != rhs.s_elements) { // if it's not a self-assignment
        // if sizes aren't the same, new memory allocation is needed
        if (s_size != rhs.s_size) {
            // deallocate previous memory
            delete[] s_elements;
            s_size = rhs.s_size;
            // allocate memory for new vector
            s_elements = new double[s_size];
        } // otherwise, if vectors are of same size there is already enough
        // memory allocated; just reuse it
        // Copy RHS into LHS
        blas::copy(s_size, rhs.s_elements, 1, s_elements, 1);
    }

    return *this;
}

Vector& Vector::operator=(Vector&& rhs) {
    // Delete old data (if not in a self-assignment)
    if (s_elements != rhs.s_elements) delete[] s_elements;

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
                    " vectors of unequal size.");
    Vector retval(vsum);
    blas::axpy(s_size, 1.0, s_elements, 1, retval.s_elements, 1);
    return retval;
}

Vector Vector::operator+(const double& add) {
    Vector added(this->size(), add);
    blas::axpy(s_size, 1.0, s_elements, 1, added.s_elements, 1);
    return added;
}

Vector operator+(const double& add, const Vector& vec) {
    Vector added(vec.size(), add);
    blas::axpy(added.size(), 1.0, vec.data(), 1, added.data(), 1);
    return added;
}

Vector Vector::operator-(const Vector& vsub) {
    if (s_size != vsub.size()) throw std::out_of_range("Unable to subtract"
                    " vectors of unequal size.");
    Vector retval(-1.*vsub);
    blas::axpy(s_size, 1.0, s_elements, 1, retval.s_elements, 1);
    return retval;
}

Vector Vector::operator-(const double& sub) {
    Vector subt(this->size(), -1.*sub);
    blas::axpy(s_size, 1.0, s_elements, 1, subt.s_elements, 1);
    return subt;
}

Vector operator-(const double& sub, const Vector& vec) {
    Vector subt(vec.size(), -1.*sub);
    blas::axpy(vec.size(), 1.0, vec.data(), 1, subt.data(), 1);
    return subt;
}

double Vector::operator*(const Vector& vdot) {
    if (s_size != vdot.size()) throw std::out_of_range("Unable to calculate"
                    "dot product for vectors of unequal size.");
    return blas::dot(s_size, s_elements, 1, vdot.s_elements, 1);
}

Vector Vector::operator*(const double& scalar) {
    Vector retval(*this);
    blas::scal(s_size, scalar, retval.s_elements, 1);
    return retval;
}

Vector operator*(const double& scalar, const Vector& vec) {
    Vector retval(vec);
    blas::scal(vec.size(), scalar, retval.data(), 1);
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

// ---------- CrossProduct Member Function Definitions --------- //

Matrix Vector::CrossProduct::operator*(const CrossProduct& vp) {
    Matrix retval(this->size(), vp.size());
    for (int i = 0; i != retval.rows(); ++i)
        for (int j = 0; j != retval.cols(); ++j)
            retval[i][j] = (*this)[i] * vp[j];

    return retval;
}

// ---------- End of CrossPdocut Member Function Definitions --------- //


// ---------- Matrix Member Function Definitions --------- //

Matrix::Matrix(const int& n) : s_colSize(n), s_rowSize(n),
                                 s_elements(new double[s_rowSize*s_colSize]) {

}

Matrix::Matrix(const int& m, const int& n) : s_colSize(m), s_rowSize(n), 
                                 s_elements(new double[s_rowSize*s_colSize]) {

}

Matrix::Matrix(const std::initializer_list<
                                    std::initializer_list<double>>& rows_cols) :
                        s_colSize(rows_cols.size()), 
                        s_rowSize(rows_cols.begin()->size()),
                        s_elements(new double[s_rowSize*s_colSize]) {
    check_ctor_lists_size(rows_cols);

    int row_ind = 0, col_ind = 0;
    for (auto row : rows_cols) {
        for (auto col_ele : row) {
            s_elements[row_ind*s_rowSize + col_ind++] = col_ele;
        }
        row_ind++;
        col_ind=0;
    }
}

Matrix::Matrix(const Matrix& mc) : s_rowSize(mc.s_rowSize), 
                                s_colSize(mc.s_colSize),
                                s_elements(new double[s_rowSize*s_colSize]) {
    // Copy 'mc' elements into *this
    blas::copy(s_rowSize*s_colSize, mc.s_elements, 1, s_elements, 1);
}

Matrix::Matrix(Matrix&& mm) : s_rowSize(mm.s_rowSize), 
                                s_colSize(mm.s_colSize),
                                s_elements(mm.s_elements) {
    // Leave 'mm' in a "null state"
    mm.s_elements = nullptr;
}

Matrix& Matrix::operator=(const Matrix& mca) {
    // Check if it's not a self-assignment
    if (s_elements != mca.s_elements) {
        // If total size of data aren't the same, need to reallocate memory
        if ((s_rowSize*s_colSize) != (mca.s_rowSize*mca.s_colSize)) {
            delete[] s_elements;
            s_elements = new double[s_rowSize*s_colSize];
        } 
        // otherwise, s_elements already has enough memory allocated
        s_rowSize = mca.s_rowSize;
        s_colSize = mca.s_colSize;

        // Copy RHS into LHS
        blas::copy(s_rowSize*s_colSize, mca.s_elements, 1, s_elements, 1);
    }
    return *this;
}

Matrix& Matrix::operator=(Matrix&& mcm) {
    // Delete old data (if not in a self assignment)
    if (s_elements != mcm.s_elements) delete[] s_elements;

    // Steal RHS data
    s_elements = mcm.s_elements;
    s_rowSize = mcm.s_rowSize;
    s_colSize = mcm.s_colSize;

    // Leave RHS in a "null state"
    mcm.s_elements = nullptr;

    return *this;
}

Matrix::~Matrix() {
    if (s_elements) delete[] s_elements;
}

void Matrix::check_ctor_lists_size(const std::initializer_list<
                                    std::initializer_list<double>>& rows_cols) {
    for (auto row : rows_cols)
        if (row.size() != s_rowSize)
            throw std::invalid_argument("Matrix construct: number of elements"
            " in each row (initializer-list) must match.");
}


// ---------- End of Matrix Member Function Definitions --------- //

} // end of CHONS namespace