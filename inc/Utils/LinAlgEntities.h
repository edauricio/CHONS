// Definition of linear algebra entities, i.e. vector, matrices and higher order
// tensors.

#ifndef LINALGENTITIES_H
#define LINALGENTITIES_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <iterator>
#include <type_traits>
#include <typeinfo> // remove later
#include <iostream> // remove later

namespace CHONS {

class Matrix; // Forward declaration for Cross Product


// Static vector class
class Vector {

    // Iterator helper class 
        class VectorIterator {
            public:
                // Type aliases
                using iterator_category = std::random_access_iterator_tag;
                using value_type = double;
                using difference_type = ptrdiff_t;
                using pointer = double*;
                using reference = double&;

                // Constructors
                VectorIterator() = default;
                VectorIterator(double* pt) : element(pt) {}
                VectorIterator(const VectorIterator&);
                VectorIterator(VectorIterator&&);

                // Assignments
                VectorIterator& operator=(const VectorIterator&);
                VectorIterator& operator=(VectorIterator&&);

                // Operators overloading
                double& operator*() { return *element; }
                double& operator[](const int& i) { return element[i]; }
                VectorIterator& operator++() { ++element; return *this; }
                VectorIterator operator++(int) { pointer ele=element++; 
                                                    return ele; }
                VectorIterator& operator--() { --element; return *this; }
                VectorIterator operator--(int) { pointer ele=element--; 
                                                    return ele; }
                VectorIterator operator-(const int& n) { return (element-n); }
                difference_type operator-(const VectorIterator& v) { 
                                                return (element-v.element); }
                VectorIterator operator+(const int& n) { return (element+n); }
                VectorIterator operator+=(const int& n) { return *this + n; }
                VectorIterator operator-=(const int& n) { return *this - n; }
                bool operator==(const VectorIterator& veq) { 
                                            return (element == veq.element); }
                bool operator!=(const VectorIterator& veq) { 
                                            return (element != veq.element); }
                bool operator<(const VectorIterator& vel) { 
                                            return (element < vel.element); }
                bool operator>(const VectorIterator& veg) { 
                                            return (element > veg.element); }
                bool operator<=(const VectorIterator& vele) { 
                                            return (element <= vele.element); }
                bool operator>=(const VectorIterator& vege) { 
                                            return (element >= vege.element); }
            private:
                pointer element;
        };

        // Cross product helper class
        class CrossProduct {
            public:
                CrossProduct(Vector& v) : vec(v) {}

                int size() { return vec.size(); }
                const int size() const { return vec.size(); }

                Matrix operator*(const CrossProduct&);
                double& operator[](const int& i) { return vec[i]; }
                const double& operator[](const int& i) const { return vec[i]; }

            private:
                Vector& vec;
        };

    public:
        // Type aliases
        using iterator = VectorIterator;
        using cross_product = CrossProduct;

        // Constructors
        explicit Vector(const int&);
        Vector(const int&, const double&);
        explicit Vector(const std::initializer_list<double>&);
        Vector(const Vector&); // Copy constructor
        Vector(Vector&&); // Move constructor

        // Assignments
        Vector& operator=(const Vector&); // Copy assignment
        Vector& operator=(Vector&&); // Move assignment


        // Destructor
        ~Vector();

        // Member functions

        const int size() const { return s_size; }
        double* data() { return s_elements; }
        const double* data() const { return s_elements; }
        iterator begin() { return iterator{s_elements}; }
        iterator end() { return iterator{s_elements+s_size}; }
        cross_product cross() { return CrossProduct(*this); }
        // bool empty() { return s_isEmpty; };
        // void clear();


        // Operators overloading
        double& operator[](const int& i) { check_range(i);
                                                return s_elements[i]; }
        const double& operator[](const int& i) const { check_range(i);
                                                    return s_elements[i]; }
        Vector operator+(const Vector&);
        Vector operator+(const double&); // Element-wise addition
        Vector operator-(const Vector&);
        Vector operator-(const double&); // Element-wise subtraction
        double operator*(const Vector&); 
        Vector operator*(const double&);

        
    private:
        void check_range(const int& i) const {
        if ((i >= s_size) || (i < 0))
            throw std::out_of_range("invalid or out of range index for Vector"
                                            " subscripting"); };


        // bool s_isEmpty = true;
        int s_size;
        double *s_elements;
};

// Multiply / scaling operations with a double on the left side
Vector operator*(const double&, const Vector&);
Vector operator+(const double&, const Vector&); // Element-wise addition
Vector operator-(const double&, const Vector&); // Element-wise subtraction


class Matrix {
    // Helper class to make double subscripting possible
    class MatrixRow { 
        public:
            MatrixRow(double* ele, const int& col) : row_ele(ele), 
                                                        rowSize(col) {}

            double& operator[](const int& j) { check_col_range(j);
                                                return *(row_ele+j); }
            const double& operator[](const int& j) const { check_col_range(j);
                                                return *(row_ele+j); }
        private:
            void check_col_range(const int& j) const { 
            if ((j >= rowSize) || (j < 0))
                throw std::out_of_range("invalid or out of range index for"
                                    " Matrix column subscripting");
            }
            double* row_ele;
            int rowSize;
    };

    public:
        // Constructors
        explicit Matrix(const int&);
        Matrix(const int&, const int&);
        explicit Matrix(const std::initializer_list<
                                            std::initializer_list<double>>&);
        Matrix(const Matrix&); // Copy constructor
        Matrix(Matrix&&); // Move constructor

        // Assignments
        Matrix& operator=(const Matrix&); // Copy assignment
        Matrix& operator=(Matrix&&); // Move assignment

        // Destructor
        ~Matrix();

        // Member functions
        const int rows() const { return s_colSize; }
        const int cols() const { return s_rowSize; }
        double* data() { return s_elements; }
        const double* data() const { return s_elements; }


        // Operators overloading
        MatrixRow operator[](const int& i) { check_row_range(i); 
                        return MatrixRow(&s_elements[i*s_rowSize], s_rowSize); }
        const MatrixRow operator[](const int& i) const { check_row_range(i); 
                        return MatrixRow(&s_elements[i*s_rowSize], s_rowSize); }

            // BLAS Level 2 operations
        Vector operator*(const Vector&);
            // BLAS Level 3 operations
        Matrix operator*(const Matrix&);
        Matrix operator*(const double&);
        Matrix operator+(const Matrix&);
        Matrix operator+(const double&);
        Matrix operator-(const Matrix&);
        Matrix operator-(const double&);

    
    private:
        void check_row_range(const int& i) const { 
            if ((i >= s_rowSize) || (i < 0))
                throw std::out_of_range("invalid or out of range index for"
                                    " Matrix row subscripting");
        }
        void check_ctor_lists_size(const std::initializer_list<
                                            std::initializer_list<double>>&);

        int s_rowSize;
        int s_colSize;
        double* s_elements;        
};

Vector operator*(const Vector&, const Matrix&);
Matrix operator*(const double&, const Matrix&);
Matrix operator+(const double&, const Matrix&);
Matrix operator-(const double&, const Matrix&);

} // end of CHONS namespace

#endif