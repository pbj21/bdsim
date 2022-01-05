from math import sqrt
import matplotlib.pyplot as _plt
import numpy as _np
import pybdsim

def main():
    GenerateBoxField("field-1T-unitY")
    GenerateBoxField("field-1T-unitX",fx=1,fy=0)
    GenerateBoxField("field-1T-unitZ",fx=0,fy=0,fz=1)

def GenerateBoxField(filename, fx=0, fy=1, fz=0, xmax=20, ymax=20, plot=False):
    data = []
    # loop over and build up 3d lists of lists of lists
    for yi in [-ymax, ymax]:
        v = []
        for xi in [-xmax, xmax]:
            v.append([xi,yi,fx,fy,fz])
        data.append(v)

    # convert to numpy array
    data = _np.array(data)
    
    # construct a BDSIM format field object and write it out
    f = pybdsim.Field.Field2D(data)
    f.Write(filename+'.dat')

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
