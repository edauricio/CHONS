#include "MathModule/LinAlgEntities.h"
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

namespace Math {

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

const bool Vector::operator==(const Vector& v) const {
    double tol = 1.e-6;
    bool eq = true;
    int ind = 0;

    if (s_size != v.size()) return false;

    while (eq && (ind < s_size)) {
        eq = std::abs(s_elements[ind] - v[ind]) < tol;
        ind++;
    }

    return eq;
}

Vector Vector::operator+(const Vector& vsum) {
    if (s_size != vsum.size()) throw std::out_of_range("Unable to sum"
                    " vectors of unequal size.");
    Vector retval(vsum);
    blas::axpy(s_size, 1.0, s_elements, 1, retval.s_elements, 1);
    return retval;
}

Vector Vector::operator-(const Vector& vsub) {
    if (s_size != vsub.size()) throw std::out_of_range("Unable to subtract"
                    " vectors of unequal size.");
    Vector retval(*this);
    blas::axpy(s_size, -1.0, vsub.s_elements, 1, retval.s_elements, 1);
    return retval;
}

Vector Vector::operator*(const double& scalar) {
    Vector retval(*this);
    blas::scal(s_size, scalar, retval.s_elements, 1);
    return retval;
}

Vector Vector::operator*(const Matrix& m) {
    if (size() != m.rows()) throw std::length_error("invalid size for "
        "vector-matrix multiplication");
    Vector retval(m.cols());
    blas::gemv(blas::Layout::RowMajor, blas::Op::Trans, m.rows(), m.cols(), 1.0,
                m.data(), m.cols(), data(), 1, 0.0, retval.data(), 1);
    return retval;
}

Vector operator*(const double& scalar, const Vector& vec) {
    Vector retval(vec);
    blas::scal(vec.size(), scalar, retval.data(), 1);
    return retval;
}

Matrix cross_product(const Vector& a, const Vector& b) {
    Matrix retval(a.size(), b.size());
    blas::ger(blas::Layout::RowMajor, a.size(), b.size(), 1.0, 
                a.data(), 1, b.data(), 1, retval.data(), retval.cols());
    return retval;
}

double dot_product(const Vector& a, const Vector& b) {
    if (a.size() != b.size()) throw std::out_of_range("Unable to calculate"
                    "dot product for vectors of unequal size.");
    return blas::dot(a.size(), a.data(), 1, b.data(), 1);
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

Matrix::Matrix(const int& n) : s_colSize(n), s_rowSize(n),
                            s_elements(new double[s_rowSize*s_colSize]{0.0}) {

}

Matrix::Matrix(const int& m, const int& n) : s_colSize(m), s_rowSize(n), 
                            s_elements(new double[s_rowSize*s_colSize]{0.0}) {

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

const bool Matrix::operator==(const Matrix& m) const {
    double tol = 1.e-6;
    bool eq = true;
    int ind = 0;

    if ((s_rowSize*s_colSize != m.rows()*m.cols()) 
        || (rows() != m.rows())
        || (cols() != m.cols()) )
        return false;

    while (eq && (ind < s_rowSize*s_colSize)) {
        eq = std::abs(s_elements[ind] - m.data()[ind]) < tol;
        ind++;
    }

    return eq;
}

Vector Matrix::operator*(const Vector& v) {
    if (cols() != v.size()) throw std::length_error("invalid length for "
        "matrix-vector multiplication");
    Vector retval(rows());
    blas::gemv(blas::Layout::RowMajor, blas::Op::NoTrans, rows(), cols(), 1.0,
                s_elements, cols(), v.data(), 1, 0.0, retval.data(), 1);
    return retval;
}

Matrix Matrix::operator*(const Matrix& mm) {
    if (cols() != mm.rows()) throw std::length_error("invalid size for "
                    "matrix-matrix multiplication");
    Matrix retval(rows(), mm.cols());
    blas::gemm(blas::Layout::RowMajor, blas::Op::NoTrans, blas::Op::NoTrans,
            rows(), mm.cols(), cols(), 1.0, s_elements, cols(), mm.s_elements,
            mm.cols(), 0.0, retval.data(), retval.cols());
    return retval;
}

Matrix Matrix::operator*(const double& scal) {
    Matrix retval(*this);
    blas::scal(s_rowSize*s_colSize, scal, retval.data(), 1);
    return retval;
}

Matrix operator*(const double& scal, const Matrix& m) {
    Matrix retval(m);
    blas::scal(retval.rows()*retval.cols(), scal, retval.data(), 1);
    return retval;
}

Matrix Matrix::operator+(const Matrix& madd) {
    if ((rows() != madd.rows()) || (cols() != madd.cols()))
        throw std::length_error("invalid size for matrix-matrix addition");
    Matrix retval(madd);
    blas::axpy(rows()*cols(), 1.0, s_elements, 1, retval.s_elements, 1);
    return retval;
}

Matrix Matrix::operator-(const Matrix& msub) {
    if ((rows() != msub.rows()) || (cols() != msub.cols()))
        throw std::length_error("invalid size for matrix-matrix subtraction");
    Matrix retval(*this);
    blas::axpy(rows()*cols(), -1.0, msub.s_elements, 1, retval.s_elements, 1);
    return retval;
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

// ---------- TrMatrix Member Function Definitions --------- //

Matrix Matrix::TrMatrix::operator*(const Matrix& mm) {
    Matrix retval(trM.cols(), mm.cols());
    blas::gemm(blas::Layout::RowMajor, blas::Op::Trans, blas::Op::NoTrans,
                trM.cols(), mm.cols(), trM.rows(), 1.0, trM.data(), trM.cols(),
                mm.data(), mm.cols(), 0.0, retval.data(), retval.cols());
    return retval;
}

Matrix operator*(const Matrix& m, const Matrix::TrMatrix& trm) {
    Matrix retval(m.rows(), trm.trM.rows());
    blas::gemm(blas::Layout::RowMajor, blas::Op::NoTrans, blas::Op::Trans,
                m.rows(), trm.trM.rows(), m.cols(), 1.0, m.data(), m.cols(),
                trm.trM.data(), trm.trM.cols(), 
                0.0, retval.data(), retval.cols());
    return retval;
}

// ---------- End of TrMatrix Member Function Definitions --------- //


} // end of Math namespace

} // end of CHONS namespace