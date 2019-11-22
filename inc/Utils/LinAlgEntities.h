// Definition of linear algebra entities, i.e. vector, matrices and higher order
// tensors.

#ifndef LINALGENTITIES_H
#define LINALGENTITIES_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <iterator>
#include <type_traits>

namespace CHONS {

// Static vector class
class Vector {
    public:
        // Constructors
        // Vector();
        explicit Vector(const int&);
        Vector(const int&, double&);
        explicit Vector(const std::initializer_list<double>&);
        Vector(const Vector&); // Copy constructor
        Vector(Vector&&); // Move constructor

        // Assignments
        Vector& operator=(const Vector&); // Copy assignment
        Vector& operator=(Vector&&); // Move assignment


        // Destructor
        ~Vector();

    private:
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

    public:
        // Type aliases
        using iterator = VectorIterator;

        // Member functions
        int size() const { return s_size; };
        double* data() const { return s_elements; }
        iterator begin() { return iterator{s_elements}; }
        iterator end() { return iterator{s_elements+s_size}; }
        // bool empty() { return s_isEmpty; };
        // void clear();


        // Operators overloading
        double& operator[](const int& i) { check_range(i); 
                                                return s_elements[i]; }
        double operator[](const int& i) const { check_range(i); 
                                                    return s_elements[i]; }
        Vector operator+(const Vector&);
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

Vector operator*(const double&, const Vector&);

class Matrix {
    public:
        // Constructors
        Matrix(const int&, const int&);

        template <typename... Lists>
        Matrix(const Lists&...);

        // Assignments


        // Destructor


        // Member functions


        // Operators overloading
    
    private:
        template <typename List>
        void check_ctor_lists(const List&);

        template <typename List, typename... LChecks>
        void check_ctor_lists(const List&, const LChecks&...);

        int s_rowSize;
        int s_colSize;
        double* s_elements;
};

// Templated c'tor
template<typename... Lists>
Matrix::Matrix(const Lists&... lst) {
    check_ctor_lists(lst...);
    
}

template <typename List>
void check_ctor_lists(const List& lst) {
    bool isConvertible = std::is_convertible<
                    std::remove_const<std::remove_reference<decltype(lst)>>,
                    std::initializer_list<double>>::value;
    if (!isConvertible)
        throw std::invalid_argument("Invalid argument for Matrix construction");
}

template <typename List, typename... LChecks>
void check_ctor_lists(const List& lst, const LChecks&... rest) {
    bool isConvertible = std::is_convertible<
                    std::remove_const<std::remove_reference<decltype(lst)>>,
                    std::initializer_list<double>>::value;
    if (!isConvertible)
        throw std::invalid_argument("Invalid argument for Matrix construction");

    check_ctor_lists(rest...);
}

} // end of CHONS namespace

#endif