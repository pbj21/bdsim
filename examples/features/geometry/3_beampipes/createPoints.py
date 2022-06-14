import numpy as np

def MakePoints(nPoints=50):
    r0 = 3
    rsmall = 0.2*r0
    npoints = 30
    omega = 10*np.pi / nPoints
    thetaStep = 2*np.pi / nPoints

    result = []
    
    for i in range(nPoints):

        r = r0 + np.sin(omega*i)

        x,y = r*np.cos(i*thetaStep), r*np.sin(i*thetaStep)

        result.append([x,y])

    return result


def Write(points, filename):

    f = open(filename,"w")

    for p in points:
        f.write("\t".join(map(str, p))+"\n")
    
    f.close()


if __name__ == "__main__":
    p = MakePoints()
    Write(p, "12_points.dat")
