#!/usr/bin/env python

import matplotlib.pyplot as _plt
import numpy as _np
import pybdsim
from subprocess import check_call as _check_call

def main():
    # generate x and y points along their own axes
    x = _np.linspace(0, 30, 6)
    y = _np.linspace(0, 20, 6)
    z = _np.linspace(-50, 50, 5)
    t = _np.linspace(-20, 20, 10)  #ns
    
    # define functions for each field component
    def fx(x,y,z,t):
        return 4*_np.cos(0.02*x)*_np.sin(0.01*y)*_np.cos(0.15*t)
    def fy(x,y,z,t):
        return 3*_np.sin(0.02*x)*_np.cos(0.01*y)*_np.cos(0.15*t)
    def fz(x,y,z,t):
        return 0.01*_np.cos(0.01*z)*_np.cos(0.15*t)
    
    data = []
    # loop over and build up 3d lists of lists of lists
    for xi in x:
        vi = []
        for yi in y:
            vj = []
            for zi in z:
                vk = []
                for ti in t:
                    vk.append([xi,yi,zi,ti,fx(xi,yi,zi,ti),fy(xi,yi,zi,ti),fz(xi,yi,zi,ti)])
                vj.append(vk)
            vi.append(vj)
        data.append(vi)

    # convert to numpy array
    data = _np.array(data)
    
    # construct a BDSIM format field object and write it out
    f = pybdsim.Field.Field4D(data)
    f.Write('4dexample.dat')

    # compress the result
    _check_call(['gzip', "4dexample.dat"])
    f.Write('4dexample.dat') # write again to keep original
    
    return data
    
if __name__ == "__main__":
    main()
