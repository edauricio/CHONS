#ifndef BLAS_HERK_HH
#define BLAS_HERK_HH

#include "blas_util.hh"
#include "syrk.hh"

#include <limits>

namespace blas {

// =============================================================================
/// Hermitian rank-k update,
///     \f[ C = \alpha A A^H + \beta C, \f]
/// or
///     \f[ C = \alpha A^H A + \beta C, \f]
/// where alpha and beta are scalars, C is an n-by-n Hermitian matrix,
/// and A is an n-by-k or k-by-n matrix.
///
/// Generic implementation for arbitrary data types.
/// TODO: generic version not yet implemented.
///
/// @param[in] layout
///     Matrix storage, Layout::ColMajor or Layout::RowMajor.
///
/// @param[in] uplo
///     What part of the matrix C is referenced,
///     the opposite triangle being assumed from symmetry:
///     - Uplo::Lower: only the lower triangular part of C is referenced.
///     - Uplo::Upper: only the upper triangular part of C is referenced.
///
/// @param[in] trans
///     The operation to be performed:
///     - Op::NoTrans:   \f$ C = \alpha A A^H + \beta C. \f$
///     - Op::ConjTrans: \f$ C = \alpha A^H A + \beta C. \f$
///     - In the real    case, Op::Trans is interpreted as Op::ConjTrans.
///       In the complex case, Op::Trans is illegal (see @ref syrk instead).
///
/// @param[in] n
///     Number of rows and columns of the matrix C. n >= 0.
///
/// @param[in] k
///     - If trans = NoTrans: number of columns of the matrix A. k >= 0.
///     - Otherwise:          number of rows    of the matrix A. k >= 0.
///
/// @param[in] alpha
///     Scalar alpha. If alpha is zero, A is not accessed.
///
/// @param[in] A
///     - If trans = NoTrans:
///     the n-by-k matrix A, stored in an lda-by-k array [RowMajor: n-by-lda].
///     - Otherwise:
///     the k-by-n matrix A, stored in an lda-by-n array [RowMajor: k-by-lda].
///
/// @param[in] lda
///     Leading dimension of A.
///     If trans = NoTrans: lda >= max(1, n) [RowMajor: lda >= max(1, k)],
///     If Otherwise:       lda >= max(1, k) [RowMajor: lda >= max(1, n)].
///
/// @param[in] beta
///     Scalar beta. If beta is zero, C need not be set on input.
///
/// @param[in] C
///     The n-by-n Hermitian matrix C,
///     stored in an lda-by-n array [RowMajor: n-by-lda].
///
/// @param[in] ldc
///     Leading dimension of C. ldc >= max(1, n).
///
/// @ingroup herk

template< typename TA, typename TB, typename TC >
void herk(
    blas::Layout layout,
    blas::Uplo uplo,
    blas::Op trans,
    int64_t n, int64_t k,
    real_type<TA, TB, TC> alpha,  // note: real
    TA const *A, int64_t lda,
    real_type<TA, TB, TC> beta,  // note: real
    TC       *C, int64_t ldc )
{
    throw std::exception();  // not yet implemented
}

}  // namespace blas

#endif        //  #ifndef BLAS_HERK_HH
