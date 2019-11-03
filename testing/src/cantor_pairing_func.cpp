#include <random>
#include <iostream>
#include <set>
#include <cmath>
#include <chrono>
#include <cstdint>

inline size_t cantor(std::set<size_t>&& sc) {
    size_t a = *(sc.begin()), b = *(sc.rbegin());
    return 0.5*(a+b)*(a+b+1) + b;
}
inline size_t cantor_mod(const size_t& a, const size_t& b) {
    return 0.5*(a+b)*(a+b+1) + std::fmax(a,b);
}
inline size_t injection(const size_t& a, const size_t& b) {
    return 0.5*std::fmax(a,b)*(std::fmax(a,b)+1) + std::fmin(a,b);
}
inline long double injection(const long double& a, const long double& b) {
    return 0.5*std::fmax(a,b)*(std::fmax(a,b)+1) + std::fmin(a,b);
}
inline size_t power(const size_t& a, const size_t& b) {
    return (2ULL<<a)+(2ULL<<b);
}
inline double unordered_pairing(const double& a, const double& b) {
    return a * b + ((std::fabs(a-b) - 1)*(std::fabs(a-b) - 1) / 4);
}
inline size_t simple(const size_t&a, const size_t& b) {
    return (a+b)+(a*b);
}
inline size_t hash(const size_t& a, const size_t& b) {
    return ((a+b)<<32) | static_cast<int32_t>(std::fabs(static_cast<int32_t>(a-b)));
}
inline size_t general(const size_t&a, const size_t& b, const size_t& c) {
    return (a+b+c)+(a*b*c);
}
inline size_t general(const size_t&a, const size_t& b, const size_t& c, const size_t& d) {
    return (a+b+c+d)+(a*b*c*d);
}
inline size_t binary(const size_t& a, const size_t&b) {
    return std::max(a,b) ^ std::min(a,b);
}
inline size_t ros_strong(const size_t& a, const size_t& b) {
    return std::fmax(a,b)*std::fmax(a,b) + std::fmax(a,b) + a-b;
}

int main(int argc, char* argv[]) {

  std::default_random_engine eng(std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_real_distribution<double> dist(1.,1000000000.);
  std::uniform_int_distribution<size_t> dists(1,1000000000);
  size_t a, b, c, d, e, f, g, gt, gq, ia, ib, ic, id, ie;
  std::vector<size_t> vs;
  long double da, db;
  uint64_t ust;

  // std::cout << "Running general function on three numbers a, b and c...\n";
  // auto t0 = std::chrono::high_resolution_clock::now();
  // for (size_t i = 0; i != 10000000; ++i) {
  //   vs.clear();
  //   a = dists(eng);
  //   b = dists(eng);
  //   c = dists(eng);
  //   general(a,b,c);
  // }
  // auto t1 = std::chrono::high_resolution_clock::now();
  // std::cout << "Elapsed time: "
  //           << std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count()/1000.
  //           << " seconds\n";
  // //std::cout << a << " " << vs.front() << "\n";

  // std::cout << "Running general function on four numbers a, b, c and d...\n";
  // t0 = std::chrono::high_resolution_clock::now();
  // for (size_t i = 0; i != 10000000; ++i) {
  //   vs.clear();
  //   a = dists(eng);
  //   b = dists(eng);
  //   c = dists(eng);
  //   d = dists(eng);
  //   general(a,b,c,d);
  // }
  // t1 = std::chrono::high_resolution_clock::now();
  // std::cout << "Elapsed time: "
  //           << std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count()/1000.
  //           << " seconds\n";
  //std::cout << a << " " << vs.front() << "\n";

  // std::cout << "Running general function on three and four numbers together...\n";
  // auto t0 = std::chrono::high_resolution_clock::now();
  // for (size_t i = 0; i != 10000000; ++i) {
  //   vs.clear();
  //   a = dists(eng);
  //   b = dists(eng);
  //   c = dists(eng);
  //   d = dists(eng);
  //   e = dists(eng);
  //   f = dists(eng);
  //   g = dists(eng);
  //   gt = general(a,b,c);
  //   gq = general(a,b,d,e);
  //   if (gt == gq) {
  //       std::cout << "Ops.... genetral tri and quad doesn't integrate suitably!\n";
  //       exit(-1);
  //   }
  // }
  // auto t1 = std::chrono::high_resolution_clock::now();
  // std::cout << "Elapsed time: "
  //           << std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count()/1000.
  //           << " seconds\n";
  // std::cout << a << " " << gq << "\n";

  std::cout << "Running Mod. Cantor function on two numbers a and b...\n";
  auto t0 = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i != 10000000; ++i) {
    a = dists(eng);
    b = dists(eng);
    ia = cantor_mod(a,b);
    ib = cantor_mod(b,a);
    if (ia != ib) {
        std::cout << "Ops.... Mod. Cantor is not suitable!\n";
        exit(-1);
    }
  }
  auto t1 = std::chrono::high_resolution_clock::now();
  std::cout << "Elapsed time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count()/1000.
            << " seconds\n";
  std::cout << a << " " << ib << "\n";
  

  std::cout << "Running simple function on two numbers a and b...\n";
  t0 = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i != 10000000; ++i) {
    a = dists(eng);
    b = dists(eng);
    ia = simple(a,b);
    ib = simple(b,a);
    if (ia != ib) {
        std::cout << "Ops.... simple function is not suitable!\n";
        exit(-1);
    }
  }
  t1 = std::chrono::high_resolution_clock::now();
  std::cout << "Elapsed time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count()/1000.
            << " seconds\n";
  std::cout << a << " " << ib << "\n";
  exit(-1);

  // std::cout << "Running hash function on two numbers a and b...\n";
  // t0 = std::chrono::high_resolution_clock::now();
  // for (size_t i = 0; i != 10000000; ++i) {
  //   a = dists(eng);
  //   b = dists(eng);
  //   sa = hash(a,b);
  //   sb = hash(b,a);
  //   if (sa != sb) {
  //       std::cout << "Ops.... hash is not suitable!\n";
  //       exit(-1);
  //   }
  // }
  // t1 = std::chrono::high_resolution_clock::now();
  // std::cout << "Elapsed time: "
  //           << std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count()/1000.
  //           << " seconds\n";
  // std::cout << a << " " << sb << "\n";
  
  // std::cout << "Running binary count on two numbers a and b...\n";
  // t0 = std::chrono::high_resolution_clock::now();
  // for (size_t i = 0; i != 100000000; ++i) {
  //   a = dists(eng);
  //   b = dists(eng);
  //   c = dists(eng);
  //   d = dists(eng);
  //   ia = binary(a,b);
  //   ib = binary(b,a);
  //   ic = binary(a,c);
  //   id = binary(c,d);
  //   ie = binary(b,c);
  //   if ((ia != ib) || (ia == ic) || (ia == id) || (ia == ie)
  //       || (ic == id) || (ic == ie) || (id == ie)) {
  //       std::cout << "Ops.... binary count is not suitable!\n";
  //       exit(-1);
  //   }
  // }
  // t1 = std::chrono::high_resolution_clock::now();
  // std::cout << "Elapsed time: "
  //           << std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count()/1000.
  //           << " seconds\n";
  // std::cout << a << " " << ia << "\n";

  // std::cout << "Running size_t injection function on two numbers a and b...\n";
  // auto t0 = std::chrono::high_resolution_clock::now();
  // for (size_t i = 0; i != 10000000; ++i) {
  //   a = dist(eng);
  //   b = dist(eng);
  //   ia = injection(a,b);
  //   ib = injection(b,a);
  //   if (ia != ib) {
  //       std::cout << "Ops.... size_t injection is not suitable!\n";
  //       exit(-1);
  //   }
  // }
  // auto t1 = std::chrono::high_resolution_clock::now();
  // std::cout << "Elapsed time: "
  //           << std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count()/1000.
  //           << " seconds\n";
  // std::cout << a << " " << ia << "\n";

  // long double ia, ib;
  // std::cout << "Running double injection function on two numbers a and b...\n";
  // t0 = std::chrono::high_resolution_clock::now();
  // for (size_t i = 0; i != 10000000; ++i) {
  //   da = dist(eng);
  //   db = dist(eng);
  //   ia = injection(da,db);
  //   ib = injection(db,da);
  //   if (ia != ib) {
  //       std::cout << "Ops.... double injection is not suitable!\n";
  //       exit(-1);
  //   }
  // }
  // t1 = std::chrono::high_resolution_clock::now();
  // std::cout << "Elapsed time: "
  //           << std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count()/1000.
  //           << " seconds\n";
  // std::cout << da << " " << ib << "\n";
  exit(-1);
  std::cout << "Running unordered pairing function on two numbers a and b...\n";
  t0 = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i != 10000000; ++i) {
    da = dist(eng);
    db = dist(eng);
    ia = unordered_pairing(da,db);
    ib = unordered_pairing(db,da);
    if (ia != ib) {
        std::cout << "Ops.... unordered pairing is not suitable!\n";
        exit(-1);
    }
  }
  t1 = std::chrono::high_resolution_clock::now();
  std::cout << "Elapsed time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count()/1000.
            << " seconds\n";
  std::cout << da << " " << ib << "\n";

  // DISCARDED:
  // Too slow because of set<> object construction (pairs need to be sorted)
  //
  // std::cout << "Running Cantor pairing function on a set&&...\n";
  // t0 = std::chrono::high_resolution_clock::now();
  // for (size_t i = 0; i != 10000000; ++i) {
  //   a = dist(eng);
  //   b = dist(eng);
  //   if (cantor(std::set<size_t>{a,b}) != cantor(std::set<size_t>{b,a})) {
  //       std::cout << "Ops.... Cantor is not suitable!\n";
  //       exit(-1);
  //   }
  // }
  // t1 = std::chrono::high_resolution_clock::now();
  // std::cout << "Elapsed time: "
  //           << std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count()/1000.
  //           << " seconds\n";

  // std::cout << "Running power-of-2 function on two numbers a and b...\n";
  // t0 = std::chrono::high_resolution_clock::now();
  // for (size_t i = 0; i != 10000000; ++i) {
  //   a = dist(eng);
  //   b = dist(eng);
  //   if (power(a,b) != power(b,a)) {
  //       std::cout << "Ops.... Power is not suitable!\n";
  //       exit(-1);
  //   }
  // }
  // t1 = std::chrono::high_resolution_clock::now();
  // std::cout << "Elapsed time: "
  //           << std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count()/1000.
  //           << " seconds\n";

  
}