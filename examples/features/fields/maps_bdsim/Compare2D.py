import matplotlib.pyplot as _plt
import pybdsim

def Compare2D(original, nearest, linear, linearmag, cubic):
    kws = {'vmin':-4, 'vmax':4}
    pybdsim.Field.Plot2DXYFxFyFz(original, 'Original', **kws)
    # for this one we shrink the limits
    axs = _plt.gcf().get_axes()
    for i in range(3):
        axs[i].set_ylim(-50,50)
    Save(original)
    
    pybdsim.Field.Plot2DXYFxFyFz(nearest, 'Nearest', **kws)
    Save(nearest)
    
    pybdsim.Field.Plot2DXYFxFyFz(linear, 'Linear', **kws)
    Save(linear)
    
    pybdsim.Field.Plot2DXYFxFyFz(linearmag, 'Linear Mag', **kws)
    Save(linearmag)
    
    pybdsim.Field.Plot2DXYFxFyFz(cubic, 'Cubic', **kws)
    Save(cubic)

def _FN(filename):
    fn = filename.split('.')[0]+".png"
    print(fn)
    return fn

def Save(filename):
    _plt.savefig(_FN(filename), dpi=400)

def DoIt():
    Compare2D("2dexample.dat.gz",
              "2d_interpolated_nearest.dat",
              "2d_interpolated_linear.dat",
              "2d_interpolated_linear_mag.dat",
              "2d_interpolated_cubic.dat")

if __name__ == "__main__":
    DoIt()
