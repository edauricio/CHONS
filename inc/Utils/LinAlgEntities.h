// Definition of linear algebra entities, i.e. vector, matrices and higher order
// tensors.

#ifndef LINALGENTITIES_H
#define LINALGENTITIES_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace CHONS {

// Static vector class
class Vector {
    public:
        // Constructors
        // Vector();
        explicit Vector(const size_t&);
        Vector(const size_t&, double&);
        explicit Vector(const std::initializer_list<double>&);
        Vector(const Vector&); // Copy constructor
        Vector(Vector&&); // Move constructor

        // Assignments
        Vector& operator=(const Vector&); // Copy assignment
        Vector& operator=(Vector&&); // Move assignment


        // Destructor
        ~Vector();


        // Member functions
        int size() const { return s_size; };
        double* data() { return s_elements; }
        // bool empty() { return s_isEmpty; };
        // void clear();


        // Operators overloading
        double& operator[](const size_t& i) { check_range(i); 
                                                return s_elements[i]; }
        double operator[](const size_t& i) const { check_range(i); 
                                                    return s_elements[i]; }
        Vector operator+(const Vector&);
        double operator*(const Vector&);
        Vector operator*(const double&);

        
    private:
        void check_range(const size_t& i) const {
        if (i >= s_size) throw std::out_of_range("index out of range for Vector"
                                            " subscripting"); };


        // bool s_isEmpty = true;
        int s_size;
        double *s_elements;
};

Vector operator*(const double&, const Vector&);

} // end of CHONS namespace

#endif