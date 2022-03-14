import matplotlib.pyplot as _plt
import numpy as _np


class Splitting:
    def __init__(self, l1=(100,5), l2=(200,12)):
        self.xp = _np.array([0.0, 0.8*l1[0], l1[0], l2[0], 1e6])
        self.fp = _np.array([0.0, 0.0,       l1[1], l2[1], l2[1]])
        self.threshold = 0.8*l1[0]
        self.l1 = l1[0]
        self.l2 = l2[0]
        self.f1 = l1[1]
        self.f2 = l2[1]

    def Factor(self, eK):
        return _np.interp(eK, self.xp, self.fp)
                            

def MakePlot1():
    s = Splitting(l1=(0,5), l2=(0,5))
    eK = _np.linspace(1,250,200)
    y = s.Factor(eK)
    yint = _np.array(list(map(round, y)))

    _plt.rcParams['xtick.labelsize'] = 'large'
    _plt.rcParams['ytick.labelsize'] = 'large'

    _plt.figure()
    _plt.plot(eK, y, label='interpolated', alpha=0.7)
    _plt.plot(eK, yint, label='quantised', drawstyle='steps-mid')
    _plt.xticks([0],
                [r"0 GeV"],
                rotation=-70)
    _plt.yticks([0,s.f1],
                ['0', 'Factor 1'])
    _plt.legend()
    _plt.xlabel('Kinetic Energy', fontsize='xx-large')
    _plt.ylabel('Muon Splitting Factor', fontsize='xx-large')
    _plt.tight_layout()
    _plt.savefig('splittingfactor1.pdf')


def MakePlot1B():
    s = Splitting(l1=(150,5), l2=(150,5))
    eK = _np.linspace(1,250,200)
    y = s.Factor(eK)
    yint = _np.array(list(map(round, y)))

    _plt.rcParams['xtick.labelsize'] = 'large'
    _plt.rcParams['ytick.labelsize'] = 'large'

    _plt.figure()
    _plt.plot(eK, y, label='interpolated', alpha=0.7)
    _plt.plot(eK, yint, label='quantised', drawstyle='steps-mid')
    _plt.xticks([s.threshold, s.l1],
                [r"0.8 x E$_{k,1}$", r"E$_{k,1}$"],
                rotation=-70)
    _plt.yticks([0,s.f1],
                ['0', 'Factor 1'])
    _plt.legend()
    _plt.xlabel('Kinetic Energy', fontsize='xx-large')
    _plt.ylabel('Muon Splitting Factor', fontsize='xx-large')
    _plt.tight_layout()
    _plt.savefig('splittingfactor1b.pdf')

    
def MakePlot2():
    s = Splitting()
    eK = _np.linspace(1,250,200)
    y = s.Factor(eK)
    yint = _np.array(list(map(round, y)))

    _plt.rcParams['xtick.labelsize'] = 'large'
    _plt.rcParams['ytick.labelsize'] = 'large'

    _plt.figure()
    _plt.plot(eK, y, label='interpolated', alpha=0.7)
    _plt.plot(eK, yint, label='quantised', drawstyle='steps-mid')
    _plt.xticks([s.threshold, s.l1, s.l2],
                [r"0.8 x E$_{k,1}$", r"E$_{k,1}$", r"E$_{k,2}$"],
                rotation=-70)
    _plt.yticks([0,s.f1,s.f2],
                ['0', 'Factor 1', 'Factor 2'])
    _plt.legend()
    _plt.xlabel('Kinetic Energy', fontsize='xx-large')
    _plt.ylabel('Muon Splitting Factor', fontsize='xx-large')
    _plt.tight_layout()
    _plt.savefig('splittingfactor2.pdf')

if __name__ == "__main__":
    MakePlot1()
    MakePlot1B()
    MakePlot2()
