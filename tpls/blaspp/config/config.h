#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// -----------------------------------------------------------------------------
#if defined(FORTRAN_UPPER)
    #define FORTRAN_NAME( lower, UPPER ) UPPER
#elif defined(FORTRAN_LOWER)
    #define FORTRAN_NAME( lower, UPPER ) lower
#else
    // default is ADD_
    #define FORTRAN_NAME( lower, UPPER ) lower ## _
#endif

// -----------------------------------------------------------------------------
#if defined(BLAS_ILP64) || defined(LAPACK_ILP64)
    // long long is >= 64 bits
    typedef int64_t blas_int;
    typedef int64_t lapack_int;
#else
    typedef int blas_int;
    typedef int lapack_int;
#endif

#endif // CONFIG_H
