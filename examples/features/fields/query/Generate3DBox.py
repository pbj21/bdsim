import numpy as _np
import pybdsim

def main():
    GenerateBoxField("field-3D-1T-unitY")
    GenerateBoxField("field-3D-1T-unitX",fx=1,fy=0)
    GenerateBoxField("field-3D-1T-unitZ",fx=0,fy=0,fz=1)

def GenerateBoxField(filename, fx=0, fy=1, fz=0, xmax=20, ymax=20, zmax=10):
    data = []
    
    # loop over and build up 3d lists of lists of lists
    for zi in [-zmax, zmax]:
        v = []
        for yi in [-ymax, ymax]:
            u = []
            for xi in [-xmax, xmax]:
                u.append([xi,yi,zi,fx,fy,fz])
            v.append(u)
        data.append(v)

    # convert to numpy array
    data = _np.array(data)
    
    # construct a BDSIM format field object and write it out
    f = pybdsim.Field.Field3D(data)
    f.Write(filename+'.dat')
    
    return data
    
if __name__ == "__main__":
    main()
