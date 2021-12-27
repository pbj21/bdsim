Cons {
x0=0,
y0=0,
z0=0,
rmin1=5
rmax1=500
rmin2=5
rmax2=500
z=250
material=Graphite,
phi0=0,
dphi=360,
temperature=1
}


Cons {
x0=0,
y0=0,
z0=375,
rmin1=100
rmax1=500
rmin2=5
rmax2=500
z=125
material=Graphite,
phi0=0,
dphi=360,
temperature=1
}

Cons {
x0=0,
y0=0,
z0=-375,
rmin1=5
rmax1=500
rmin2=100
rmax2=500
z=125
material=Graphite,
phi0=0,
dphi=360,
temperature=1
}

Box {
x0 = 0,
y0 = 0,
z0 = 390,
x = 25,
y = 30,
z = 15,
phi = 0,
theta = 0,
psi = 0,
materialName="G4_Cu"
}

Tubs {
x0 = 0,
y0 = 0,
z0 = -390,
rmin = 1,
rmax = 3,
z = 5,
phi0 = 0,
dphi = 360,
phi = 0,
theta = 0,
psi, = 0,
materialName="G4_Al"
}