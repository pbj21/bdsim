from math import sqrt
import matplotlib.pyplot as _plt
import numpy as _np
import pybdsim
from subprocess import check_call as _check_call

def main():
    GenerateBoxField(0.0, 1.0, 0.0, "2dboxexample-1T",xmax=20,ymax=20)
    GenerateBoxField(sqrt(0.5), sqrt(0.5), 1.0, "2dboxexample")

def GenerateBoxField(fx,fy,fz, filename, xmax=10, ymax=20, plot=False):
    #fx,fy,fz = sqrt(0.5),sqrt(0.5),0
    fx,fy,fz = 0,1.0,0
    data = []
    # loop over and build up 3d lists of lists of lists
    for xi in [-xmax,xmax]:
        v = []
        for yi in [-ymax,ymax]:
            v.append([xi,yi,fx,fy,fz])
        data.append(v)

    # convert to numpy array
    data = _np.array(data)
    
    # construct a BDSIM format field object and write it out
    f = pybdsim.Field.Field2D(data)
    f.Write(filename+'.dat')

    fd = pybdsim.Field.Field2D(data,doublePrecision=True)
    fd.Write(filename+'_dp.dat')

    # compress the result
    _check_call(['gzip', "-f", filename+'.dat'])
    _check_call(['gzip', "-f", filename+'_dp.dat'])

    if plot:
        Plot(data)
    return data

def Plot(array):

    a = array #shortcut
    mag = _np.sqrt(a[:,:,2]**2 + a[:,:,3]**2)

    _plt.figure()
    _plt.quiver(a[:,:,0], a[:,:,1], a[:,:,2], a[:,:,3], mag, cmap=_plt.cm.magma, pivot='mid', scale=None)
    _plt.xlabel('X (cm)')
    _plt.ylabel('Y (cm)')
    _plt.title('2D Example Data')
    _plt.colorbar(label='Tesla')
    _plt.tight_layout()
    _plt.axes().set_aspect('equal', 'datalim')
    _plt.savefig('2dexample.png',dpi=400)
    
if __name__ == "__main__":
    main()
