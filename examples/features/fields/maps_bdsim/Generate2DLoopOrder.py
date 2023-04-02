import numpy as _np
import pybdsim

def main():
    B = 2.0

    # LOOP METHOD 1
    data = []
    # loop over and build up 3d lists of lists of lists
    for x in [-1,0,1]:
        v = []
        for z in [3,4]:
            v.append([x, z, B*x, B*x*z, B*z])
        data.append(v)

    # convert to numpy array
    data = _np.array(data)

    # we looped in x first as per bdsim, so we need only tell it that
    # the 2nd column is Z and not Y
    f = pybdsim.Field.Field2D(data, secondColumn='Z')
    f.Write('2dexample_loopOrder_for_xz.dat')
    # but we can purposively write it out the other loop way for testing purposes
    # note the header keys are still the same apart from loopOrder> tzyx
    f.Write('2dexample_loopOrder_for_xz_tzyx.dat', writeLoopOrderReversed=True)


    # LOOP METHOD 2
    data2 = []
    # loop over other way - outer dimension first
    # this isn't the bdsim way, but we may get a field map from some other source that's
    # structured like this - so even if you're not creating it in a loop, it may have this
    # structure already.
    for z in [3,4]:
        v = []
        for x in [-1,0,1]:
            v.append([x, z, B*x, B*x*z, B*z]) # values must still be in xyzt order
        data2.append(v)

    # convert to numpy array
    data2 = _np.array(data2)

    # array structure is z is outer dimension, then x - we need it the other way
    # around, so we use flip=True when constructing the field instance
    g = pybdsim.Field.Field2D(data2, flip=True, secondColumn='Z')
    # this will write out a file identical to the very first one
    g.Write('2dexample_loopOrder_for_zx.dat')
    # this will write out a file identical to the second one
    g.Write('2dexample_loopOrder_for_zx_tzyx.dat', writeLoopOrderReversed=True)
    
    return data,f,g
    
if __name__ == "__main__":
    main()
