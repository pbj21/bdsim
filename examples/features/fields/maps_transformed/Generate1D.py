#!/usr/bin/env python

import numpy as _np
import pybdsim
#from subprocess import check_call as _check_call

def main():
    amplitude = 0.5
    length    = 60.0 # cm
    period    = 10.0 # cm
    
    z = _np.linspace(0, length, 120) # 120 points
    by = amplitude * _np.sin(2*_np.pi * z / period)

    data = []
    for zi,byi in zip(z,by):
        data.append([zi, 0.0, byi, 0.0])

    data = _np.array(data)

    f = pybdsim.Field.Field1D(data)
    f.Write("1dexample-along-x.dat")

    f = pybdsim.Field.Field1D(data, column="Y")
    f.Write("1dexample-along-y.dat")

    f = pybdsim.Field.Field1D(data, column="Z")
    f.Write("1dexample-along-z.dat")

    f = pybdsim.Field.Field1D(data, column="T")
    f.Write("1dexample-along-t.dat")

    # now one that doesn't start at 0 in that axis
    z = _np.linspace(0.2*length, 1.2*length, 120)
    by = amplitude * _np.sin(2*_np.pi * z / period)

    data = []
    for zi,byi in zip(z,by):
        data.append([zi, 0.0, byi, 0.0])
    data = _np.array(data)
    f = pybdsim.Field.Field1D(data, column="Z")
    f.Write("1dexample-along-z-offset.dat")
    
    return data

if __name__ == "__main__":
    main()
