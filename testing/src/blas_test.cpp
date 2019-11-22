#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include "Utils/LinAlgEntities.h"

/*subroutine daxpy    (   integer     N,
double precision    DA,
double precision, dimension(*)  DX,
integer     INCX,
double precision, dimension(*)  DY,
integer     INCY 
)       */

// extern "C" {extern void daxpy_(int*, double*, double*, int*, double*, int*);}

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
// extern "C" {extern void dgemm_(char*, char*, int*, int*, int*, double*, double*,
//                                 int*, double*, int*, double*, double*, int*);}


int main() {

    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine gen(seed);
    std::uniform_real_distribution<double> dist(0., 10.);

    int N = 100000000;
    int incx=1, incy=1, lda=N, ldb=N, ldc=N, m, n, k;
    // double *A, *B, *C, *x, *y;
    CHONS::Vector x(N), y(N);
    double alpha=1.0, beta=0.0;
    char transA='T', transB='T';

    // A = new double[N*N];
    // B = new double[N*N];
    // C = new double[N*N];
    // x = new double[N];
    // y = new double[N];
    auto t1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i != N; ++i) {
        x[i] = dist(gen);
        y[i] = dist(gen);
        // for (int j = 0; j != N; ++j) {
        //     A[i*N+j] = dist(gen);
        //     B[i*N+j] = dist(gen);
        //     C[i*N+j] = dist(gen);
        // }
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()/1000. << " sec\n";

    // std::ofstream oFile ("out.m");
    // oFile << "x = [";
    // for (int i = 0; i != N; ++i) {
    //     if (i != N-1)
    //         oFile << x[i] << ", ";
    //     else
    //         oFile << x[i] << "];\n\n";
    // }

    // oFile << "y = [";
    // for (int i = 0; i != N; ++i) {
    //     if (i != N-1)
    //         oFile << y[i] << ", ";
    //     else
    //         oFile << y[i] << "];\n\n";
    // }

    CHONS::Vector res(N);
    //  t1 = std::chrono::high_resolution_clock::now();
    // res = x+y;
    //  t2 = std::chrono::high_resolution_clock::now();
    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()/1000. << " sec\n";

    t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i != res.size(); ++i)
        res[i] = x[i] + y[i];
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()/1000. << " sec\n";

    // oFile << "A = [";
    // for (int i = 0; i != N; ++i) {
    //     for (int j = 0; j != N; ++j) {
    //         if (j != N-1)
    //             oFile << A[i*N+j] << ", ";
    //         else if (i != N-1)
    //             oFile << A[i*N+j] << "; ";
    //         else
    //             oFile << A[i*N+j] << "];\n\n";
    //     }
    // }

    // oFile << "B = [";
    // for (int i = 0; i != N; ++i) {
    //     for (int j = 0; j != N; ++j) {
    //         if (j != N-1)
    //             oFile << B[i*N+j] << ", ";
    //         else if (i != N-1)
    //             oFile << B[i*N+j] << "; ";
    //         else
    //             oFile << B[i*N+j] << "];\n\n";
    //     }
    // }

    // oFile << "C = [";
    // for (int i = 0; i != N; ++i) {
    //     for (int j = 0; j != N; ++j) {
    //         if (j != N-1)
    //             oFile << C[i*N+j] << ", ";
    //         else if (i != N-1)
    //             oFile << C[i*N+j] << "; ";
    //         else
    //             oFile << C[i*N+j] << "];\n\n";
    //     }
    // }

    // daxpy_(&N, &alpha, x, &incx, y, &incy);

    // oFile << "ny = [";
    // for (int i = 0; i != N; ++i) {
    //     if (i != N-1)
    //         oFile << res[i] << ", ";
    //     else
    //         oFile << res[i] << "];\n\n";
    // }

    // dgemv_(&transA, &N, &N, &alpha, A, &N, x, &incx, &beta, y, &incy);

    // oFile << "ny = [";
    // for (int i = 0; i != N; ++i) {
    //     if (i != N-1)
    //         oFile << y[i] << ", ";
    //     else
    //         oFile << y[i] << "];\n\n";
    // }

    // dgemm_(&transA, &transB, &N, &N, &N, &alpha, A, &N, B, &N, &beta, C, &N);

    // oFile << "nC = [";
    // for (int i = 0; i != N; ++i) {
    //     for (int j = 0; j != N; ++j) {
    //         if (j != N-1)
    //             oFile << C[i*N+j] << ", ";
    //         else if (i != N-1)
    //             oFile << C[i*N+j] << "; ";
    //         else
    //             oFile << C[i*N+j] << "];\n\n";
    //     }
    // }

}