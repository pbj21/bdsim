import matplotlib.pyplot as _plt
import numpy as _np
import pybdsim
from subprocess import check_call as _check_call

def main():
    # generate x and y points along their own axes
    x = _np.arange(-30, 30, 7.5)
    
    # define functions for each field component
    def fx(xv):
        return 4*_np.cos(0.08*xv)
    def fy(xv):
        return 2*_np.sin(0.1*xv)
    def fz(xv):
        return 3*_np.sin(0.02*xv + 0.2)

    data = []
    # loop over and build up 2d lists of lists
    for xi in x:
        data.append([xi,fx(xi),fy(xi),fz(xi)])

    # convert to numpy array
    data = _np.array(data)

    # construct a BDSIM format field object and write it out
    f = pybdsim.Field.Field1D(data)
    f.Write('1dexample.dat')

    # compress the result
    _check_call(['gzip', "-f", "1dexample.dat"])
    f.Write('1dexample.dat') # write again to keep original

    #Plot(data[:,0],data[:,1],data[:,2],data[:,3])
    return data
    
def Plot(x,xv,yv,zv):
    
    mag = _np.sqrt(xv**2 + yv**2 + zv**2)

    _plt.figure()
    _plt.quiver(x, _np.zeros_like(x), xv, yv, mag, cmap=_plt.cm.magma, pivot='mid', scale=None)
    _plt.xlabel('X (cm)')
    _plt.ylabel('Y (cm)')
    _plt.title('1D Example Data')
    _plt.colorbar()
    _plt.tight_layout()
    _plt.axes().set_aspect('equal', 'datalim')
    _plt.savefig('1dexample.png',dpi=400)

    _plt.figure()
    _plt.plot(x,xv, '.', label='Fx')
    _plt.plot(x,yv, '.', label='Fy')
    _plt.xlabel('X (cm)')
    _plt.ylabel('Y (cm)')
    _plt.legend()
    _plt.tight_layout()
    _plt.savefig('1dexample_1d.png', dpi=400)
    
if __name__ == "__main__":
    main()
