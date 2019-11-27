#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <numeric>
#include <algorithm>
#include "MathModule/LinAlgEntities.h"

/*subroutine daxpy    (   integer     N,
double precision    DA,
double precision, dimension(*)  DX,
integer     INCX,
double precision, dimension(*)  DY,
integer     INCY 
)       */

extern "C" {extern void daxpy_(int*, double*, double*, int*, double*, int*);}

/*subroutine dgemv    (   character   TRANS,
integer     M,
integer     N,
double precision    ALPHA,
double precision, dimension(lda,*)  A,
integer     LDA,
double precision, dimension(*)  X,
integer     INCX,
double precision    BETA,
double precision, dimension(*)  Y,
integer     INCY 
)   */
// extern "C" {extern void dgemv_(char*, int*, int*, double*, double*, int*, 
//                                 double*, int*, double*, double*, int*);}

/*subroutine dgemm    (   character   TRANSA,
character   TRANSB,
integer     M,
integer     N,
integer     K,
double precision    ALPHA,
double precision, dimension(lda,*)  A,
integer     LDA,
double precision, dimension(ldb,*)  B,
integer     LDB,
double precision    BETA,
double precision, dimension(ldc,*)  C,
integer     LDC 
)       */                                
extern "C" {extern void dgemm_(char*, char*, int*, int*, int*, double*, double*,
                                int*, double*, int*, double*, double*, int*);}


int main() {

    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine gen(seed);
    std::uniform_real_distribution<double> dist(0., 10.);

    int N = 4;
    int incx=1, incy=1, lda=N, ldb=N, ldc=N, m, n, k;
    // double *A, *B, *C, *x, *y;
    CHONS::Math::Vector x(N), y(N);
    CHONS::Math::Matrix A(N,N), B(N,N);
    double alpha=1.0, beta=0.0;
    char transA='N', transB='N';

    for (int i = 0; i != N; ++i) {
        x[i] = dist(gen);
        y[i] = dist(gen);
        for (int j = 0; j != N; ++j) {
            A[i][j] = dist(gen);
            B[i][j] = dist(gen);
        //     C[i*N+j] = dist(gen);
        }
    }

    CHONS::Math::Matrix C{ {2.3, 5, 12.56, 6},
                     {6.5, 1.2, 90, 236.1},
                     {1.5, 123.1, 3., 0.2},
                     {11, 15, 19, 20.23}
                    };

    std::ofstream oFile ("out.m");
    oFile << "x = [";
    for (int i = 0; i != N; ++i) {
        if (i != N-1)
            oFile << x[i] << ", ";
        else
            oFile << x[i] << "];\n\n";
    }

    oFile << "y = [";
    for (int i = 0; i != N; ++i) {
        if (i != N-1)
            oFile << y[i] << ", ";
        else
            oFile << y[i] << "];\n\n";
    }

    oFile << "A = [";
    for (int i = 0; i != A.rows(); ++i) {
        for (int j = 0; j != A.rows(); ++j) {
            if (j != A.rows()-1)
                oFile << A[i][j] << ", ";
            else if (i != A.rows()-1)
                oFile << A[i][j] << "; ";
            else
                oFile << A[i][j] << "];\n\n";
        }
    }

    oFile << "B = [";
    for (int i = 0; i != B.rows(); ++i) {
        for (int j = 0; j != B.rows(); ++j) {
            if (j != B.rows()-1)
                oFile << B[i][j] << ", ";
            else if (i != B.rows()-1)
                oFile << B[i][j] << "; ";
            else
                oFile << B[i][j] << "];\n\n";
        }
    }

    oFile << "C = [";
    for (int i = 0; i != C.rows(); ++i) {
        for (int j = 0; j != C.rows(); ++j) {
            if (j != C.rows()-1)
                oFile << C[i][j] << ", ";
            else if (i != C.rows()-1)
                oFile << C[i][j] << "; ";
            else
                oFile << C[i][j] << "];\n\n";
        }
    }

    CHONS::Math::Vector res(N);
    double a = 2.;
    res = x+y;

    oFile << "r1 = [";
    for (int i = 0; i != N; ++i) {
        if (i != N-1)
            oFile << res[i] << ", ";
        else
            oFile << res[i] << "];\n\n";
    }

    res = x-y;

    oFile << "r2 = [";
    for (int i = 0; i != N; ++i) {
        if (i != N-1)
            oFile << res[i] << ", ";
        else
            oFile << res[i] << "];\n\n";
    }

    res = 3.*x;

    oFile << "r3 = [";
    for (int i = 0; i != N; ++i) {
        if (i != N-1)
            oFile << res[i] << ", ";
        else
            oFile << res[i] << "];\n\n";
    }

    res = x*3.;

    oFile << "r4 = [";
    for (int i = 0; i != N; ++i) {
        if (i != N-1)
            oFile << res[i] << ", ";
        else
            oFile << res[i] << "];\n\n";
    }

    CHONS::Math::Matrix crossp(x.size());

    crossp = cross_product(x, y);

    oFile << "r5 = [";
    for (int i = 0; i != N; ++i) {
        for (int j = 0; j != N; ++j) {
            if (j != N-1)
                oFile << crossp[i][j] << ", ";
            else if (i != crossp.rows()-1)
                oFile << crossp[i][j] << "; ";
            else
                oFile << crossp[i][j] << "];\n\n";
        }
    }

    std::cout << dot_product(x,y) << "\n";
    CHONS::Math::Vector res2 = a*x;
    // auto t1 = std::chrono::high_resolution_clock::now();
    // auto t2 = std::chrono::high_resolution_clock::now();
    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()/1000. << " sec\n";
    // daxpy_(&N, &alpha, x, &incx, y, &incy);

    // oFile << "ny = [";
    // for (int i = 0; i != N; ++i) {
    //     if (i != N-1)
    //         oFile << res[i] << ", ";
    //     else
    //         oFile << res[i] << "];\n\n";
    // }

    // dgemv_(&transA, &N, &N, &alpha, A, &N, x, &incx, &beta, y, &incy);

    // oFile << "ny2 = [";
    // for (int i = 0; i != N; ++i) {
    //     if (i != N-1)
    //         oFile << res2[i] << ", ";
    //     else
    //         oFile << res2[i] << "];\n\n";
    // }

    // dgemm_(&transA, &transB, &N, &N, &N, &alpha, B.data(), &N, A.data(), &N, &beta, C.data(), &N);

    // oFile << "nC = [";
    // for (int i = 0; i != N; ++i) {
    //     for (int j = 0; j != N; ++j) {
    //         if (j != N-1)
    //             oFile << C[i][j] << ", ";
    //         else if (i != C.rowSize()-1)
    //             oFile << C[i][j] << "; ";
    //         else
    //             oFile << C[i][j] << "];\n\n";
    //     }
    // }

}