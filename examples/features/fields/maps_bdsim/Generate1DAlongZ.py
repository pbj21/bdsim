import numpy as _np
import pybdsim
from subprocess import check_call as _check_call

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

    f = pybdsim.Field.Field1D(data, column="Z")
    f.Write("1dexample-along-z.dat")

    # compress the result
    _check_call(['gzip', "-f", "1dexample-along-z.dat"])
    f.Write('1dexample-along-z.dat') # write again to keep original

    return data

if __name__ == "__main__":
    main()
