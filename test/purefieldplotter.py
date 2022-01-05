import matplotlib.pyplot as plt
import numpy as np
import glob


def PlotAll():
    for filename in glob.glob("*radial.dat"):
        Plot(filename)

def Plot(filename):
    rdata = Load(filename)
    rpos = rdata['position']
    rval = rdata['field']
    rmag = rdata['magnitude']
    
    cfilename = filename.split('radial.dat')[0] + 'carteasian.dat'
    cdata = Load(cfilename)
    cpos = cdata['position']
    cval = cdata['field']
    cmag = cdata['magnitude']
    nx = int(cdata['header']['nX'])
    ny = int(cdata['header']['nY'])
    
    cpos = cpos.reshape((nx,ny,3))
    cval = cval.reshape((nx,ny,3))
    cmag = cmag.reshape((nx,ny))
    cx = cpos[0,:,0] # first vertical position (row), all columns, x component of vector
    cy = cpos[:,0,1]
    # modern matplotlib's streamplot has a very strict check on the spacing
    # of points being equal, which they're meant to be but then theres's a
    # rounding error in the file written out by C++.
    # regenerate it
    cx = np.linspace(np.min(cx), np.max(cx), len(cx))
    cy = np.linspace(np.min(cy), np.max(cy), len(cy))
    cfx = cval[:,:,0]
    cfy = cval[:,:,1]
    cfmag = np.sqrt(cfx**2 + cfy**2)
    
    xmax = np.max(cx)
    xmin = np.min(cx)
    ymax = np.max(cy)
    ymin = np.min(cy)
    xl = (1.5*xmin, 1.5*xmax)
    yl = (1.5*ymin, 1.5*ymax)

    #FIRST PLOT
    f = plt.figure(figsize=(8,6.5))
    ax = f.add_subplot(111, aspect='equal')

    plt.quiver(rpos[:,0],rpos[:,1],rval[:,0],rval[:,1],color="lightgrey",pivot="middle")
    plt.streamplot(cx,cy,cfx,cfy, color=cfmag)

    plt.xlabel('Position (mm)')
    plt.ylabel('Position (mm)')

    basefilename = filename.split('.')[0]
    plt.title(basefilename + ' field map')
    plt.colorbar(label='Field (T)')
    plt.tight_layout()
    plt.savefig(basefilename + '.pdf')

    # SECOND PLOT
    f = plt.figure(figsize=(8,6.5))
    ax = f.add_subplot(111, aspect='equal')
    plt.streamplot(cx,cy,cfx,cfy, color='silver')
    # this is plotted afterwards so a) it's on top and b) the colourbar works
    plt.quiver(rpos[:,0],rpos[:,1],rval[:,0],rval[:,1],rmag,pivot="middle")
    #plt.contour(cpos[:,:,0],cpos[:,:,1],cmag,20) #20 contours

    plt.xlabel('Position (mm)')
    plt.ylabel('Position (mm)')

    basefilename = filename.split('.')[0]    
    plt.title(basefilename + ' field map')
    plt.colorbar(label='Field (T)')
    plt.tight_layout()
    plt.savefig(basefilename + '2.pdf')

    plt.close()
    #return cx,cy,cfx,cfy,cfmag

    del cdata
    del rdata
    del cpos, cval, cmag
    del rpos, rval, rmag
    del cx, cy, cfx, cfy


def Load(filename):
    print('Load> ',filename)
    pos = []
    val = []
    header = {}
    f = open(filename,"r")
    for line in f.readlines():
        if line[0] == '#':
            #comment line
            pass
        elif line[0] == '>':
            #header
            vs = line.strip().split('=')
            header[vs[0][1:].strip()] = float(vs[1].strip())
        else:
            #data
            vs = line.strip().split()
            pos.append(vs[0].strip(')').strip('(').split(','))
            val.append(vs[1].strip(')').strip('(').split(','))

    pos = np.array(pos,dtype=float)
    val = np.array(val,dtype=float)
    mag = np.sqrt(val[:,0]**2 + val[:,1]**2)

    data = {}
    data['header']    = header
    data['position']  = pos
    data['field']     = val
    data['magnitude'] = mag
    f.close()
    return data
