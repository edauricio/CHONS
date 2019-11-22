#include "PolyModule/Polynomials.h"
#include <vector>
#include <iostream>
#include <cmath>

using namespace CHONS;

int main (int argc, char** argv) {

    // if (argc != 4) { std::cout << "Usage: " << argv[0] << " alpha beta order\n"; exit(-1); }
    // int al = atoi(argv[1]);
    // int be = atoi(argv[2]);
    // int ord = atoi(argv[3]);

    // Jacobi jac(al, be, ord);

    std::vector<double> points(1000, -1);
    
    for (int i = 1; i != points.size(); ++i)
        points[i] = points[i-1] + 2.0/(points.size()-1);

    // std::vector<double> ev = jac.EvaluateAt(points);
    // std::vector<double> dev = jac.DerivativeAt(points);
    // std::vector<double> zs = jac.GetZeros();


    std::cout << "#x\t\ty\t\tdev\n";
    // double max = ev[0];
    // int max_i = 0;

    // for (int i = 0; i != ev.size(); ++i) {
    //     if (std::abs(ev[i]) > max) { max = ev[i]; max_i = i; }
    //     std::cout << points[i] << "\t\t" << ev[i] << "\t\t" << dev[i] << "\n";
    // }

    // for (int i = 0; i != zs.size(); ++i) {
    //     std::cout << zs[i] << "\t\t0.0\n";
    // }
    //std::cout << "Maximum: " << max << "\tDerivative: " << dev[max_i] << "\n";

    Legendre lg(3);
    std::vector<double> evl = lg.EvaluateAt(points);
    std::vector<double> devl = lg.DerivativeAt(points);

    // for (int i = 0; i != evl.size(); ++i) 
    //     std::cout << points[i] << "\t\t" << evl[i] << "\t\t" << devl[i] << "\n";


    std::vector<double> lag_pts(6, -1);
    std::vector<double> ypts{1, -1.2, 3.5, 1.1, -1, 0};
    for (int i = 1; i != lag_pts.size(); ++i)
        lag_pts[i] = lag_pts[i-1] + 2.0/(lag_pts.size()-1);


    Lagrange lag(lag_pts);
    // std::cout << "lag(8,2): " << lag.EvaluateAt(lag_pts[8],2) << "\n";
    std::vector<double> evlg(points.size(),0.0);
    std::vector<double> derivlg(points.size(), 0.0);
    for (int j = 0; j != evlg.size(); ++j) {
        evlg[j] = 0.0;
        derivlg[j] = 0.0;
        for (int i = 0; i != lag_pts.size(); ++i) {
            evlg[j] += ypts[i]*lag.EvaluateAt(points[j], i);
            derivlg[j] += ypts[i]*lag.DerivativeAt(points[j], i);
        }
        std::cout << points[j] << "\t\t" << evlg[j] << "\t\t" << derivlg[j] << "\n";
    }

    
    // for (int i = 0; i != evlg.size(); ++i)
    //     evlg[i] = lag.EvaluateAt(points[i], 3);

    // for (int i = 0; i != evlg.size(); ++i) 
    //     std::cout << points[i] << "\t\t" << evl[i] << "\t\t" << evlg[i] << "\n";
    

    
    
    
    return 0;
}