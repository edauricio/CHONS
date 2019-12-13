import numpy as np
from scipy import special as sp

def lag(pts, x, i):
     prod = 1
     for j in range(np.size(pts)):
             if (j != i):
                     prod *= ( (x - pts[j]) / ( pts[i] - pts[j] ) )
     return prod

def line_map(xphys, xstd, r):
     sum = 0
     for i in range(np.size(xstd)):
             sum += xphys[i] * lag(xstd, r, i)
     return sum

def quad_map(xphys, yphys, xstd, ystd, r, s):
     basis = np.array([])
     for j in range(np.size(ystd)):
             for i in range(np.size(xstd)):
                     basis = np.append(basis, lag(xstd, r, i) * lag(ystd, s, j))
     return np.array([np.dot(xphys, basis), np.dot(yphys, basis)])


pts_eval = sp.roots_legendre(6)[0]
x_phys = np.array([-10.,   0.,  10., -10.,   0.,  10., -10.,   0.,  10.])
y_phys = np.array([-10., -7.5, -10., 0., 2.5, 0., 10., 12.5, 10.])
x_std = np.array([-1., 0., 1.])
y_std = np.array([-1., 0., 1.])

mapped_x = np.array([])
mapped_y = np.array([])
for i in range(np.size(pts_eval)):
     for j in range(np.size(pts_eval)):
             mapped_x = np.append(mapped_x, quad_map(x_phys, y_phys, x_std, y_std, pts_eval[j], pts_eval[i])[0])
             mapped_y = np.append(mapped_y, quad_map(x_phys, y_phys, x_std, y_std, pts_eval[j], pts_eval[i])[1])


print(mapped_x)
print(mapped_y)
