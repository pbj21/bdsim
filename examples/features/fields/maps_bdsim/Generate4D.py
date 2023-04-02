from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as _plt
import numpy as _np
import pybdsim
from subprocess import check_call as _check_call

def main():
    # generate x,y,z,t points along their own axes
    x = _np.arange(-30, 30, 6.1)
    y = _np.arange(-25, 25, 7.2)
    z = _np.arange(-35, 35, 8)
    t = _np.arange(-2e-9, 2e-9, 5e-10)

    # define functions for each dimension
    def fx(x,y,z):
        return 4*_np.cos(0.08*x)*_np.sin(0.1*y) + z
    def fy(x,y,z):
        return 2*_np.sin(0.08*x)*_np.cos(0.1*y) - 2*z
    def fz(x,y,z):
        return 3*_np.cos(0.05*x)*0.4*z
    # overal modulation in time
    def ft(fxc,fyc,fzc,t):
        factor = _np.sin(t/1e-9)
        return fxc*factor, fyc*factor, fzc*factor
    
    data = []
    # loop over and build up 3d lists of lists of lists
    for xi in x:
        u = []
        for yi in y:
            v = []
            for zi in z:
                w = []          
                for ti in t:
                    xc = fx(xi,yi,zi)
                    yc = fy(yi,yi,zi)
                    zc = fz(zi,yi,zi)
                    xc,yc,zc = ft(xc,yc,zc,ti)
                    w.append([xi,yi,zi,ti,xc,yc,zc])
                v.append(w)
            u.append(v)
        data.append(u)

    # convert to numpy array
    data = _np.array(data)

    # construct a BDSIM format field object and write it out
    f = pybdsim.Field.Field4D(data)
    f.Write('4dexample.dat')
    
    # compress the result
    _check_call(['gzip', "-f", "4dexample.dat"])

    return data
    
if __name__ == "__main__":
    main()
