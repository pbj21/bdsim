from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as _plt
import numpy as _np
import pybdsim
from subprocess import check_call as _check_call

def main():
    # generate x,y,z points along their own axes
    x = _np.arange(-30, 30, 6.1)
    y = _np.arange(-25, 25, 7.2)
    z = _np.arange(-35, 35, 8)

    # define functions for each dimension
    def fx(x,y,z):
        return 4*_np.cos(0.08*x)*_np.sin(0.1*y) + z
    def fy(x,y,z):
        return 2*_np.sin(0.08*x)*_np.cos(0.1*y) - 2*z
    def fz(x,y,z):
        return 3*_np.cos(0.05*x)*0.4*z
    
    data = []
    # loop over and build up 3d lists of lists of lists
    for xi in x:
        u = []
        for yi in y:
            v = []
            for zi in z:
                v.append([xi,yi,zi,fx(xi,yi,zi),fy(xi,yi,zi),fz(xi,yi,zi)])
            u.append(v)
        data.append(u)

    # convert to numpy array
    data = _np.array(data)

    # construct a BDSIM format field object and write it out
    f = pybdsim.Field.Field3D(data)
    f.Write('3dexample.dat')

    # compress the result
    _check_call(['gzip', "-f", "3dexample.dat"])

    # construct flipped ordered data for testing purposes
    ff = pybdsim.Field.Field3D(data)
    ff.Write('3dexample_zyx.dat', writeLoopOrderReversed=True)

    # compress the result
    _check_call(['gzip', "-f", "3dexample_zyx.dat"])

    #Plot(data)
    return data

def Plot(array):

    a = array #shortcut
    #mag = _np.sqrt(a[:,:,:,3]**2 + a[:,:,:,4]**2 + a[:,:,:,5]**2)
    
    fig = _plt.figure()
    ax = fig.gca(projection='3d')
    ax.quiver(a[:,:,:,0], a[:,:,:,1], a[:,:,:,2], a[:,:,:,3], a[:,:,:,4], a[:,:,:,5], length=0.2, cmap=_plt.cm.magma)
    _plt.xlabel('X (cm)')
    _plt.ylabel('Y (cm)')
    ax.set_zlabel('Z (cm)')
    _plt.suptitle('3D Example Data')
    _plt.savefig('3dexample.png',dpi=400)

    _plt.show()
    
if __name__ == "__main__":
    main()
