import numpy as np
import pyhepmc as hep

def GeneratePXPYPZ():
    pz = 0.9 + 0.1*np.random.uniform()
    pzm = np.sqrt(1.0 - pz)
    fraction = np.random.uniform()
    py = fraction * pzm
    px = (1.0-fraction)*pzm
    return px,py,pz

def GenerateXY(xyrange=2000):
    low = -0.5*xyrange
    x = low + xyrange*np.random.uniform()
    y = low + xyrange*np.random.uniform() 
    return x,y

def CreateEvent(pdgIDs, eventID):  
    evt = hep.GenEvent(hep.Units.GEV, hep.Units.MM)
    #evt.set_event_number(eventID)
    evt.event_number = eventID
    evt.weights = [1]*len(pdgIDs)
    evt.attributes['signal_process_id'] = -1

    for pdgID in pdgIDs:

        px,py,pz = GeneratePXPYPZ()
        # just fix at 20 GeV
        prt = hep.GenParticle((px,py,pz, 20.0), pdgID, 1) # status = 1
        x,y = GenerateXY()
        vtx = hep.GenVertex((x,y,0.0,20.0))
        vtx.add_particle_out(prt)
        #vtx.add_particle_in(prt)
        evt.add_vertex(vtx)
        evt.add_particle(prt)
        #evt.set_beam_particles(prt)

    return evt

def CreateDummyEventsInHEPMC2File(outputFileName):
    fHEP = hep.io.WriterAsciiHepMC2(outputFileName)

    # some events have muons
    events = [ [11,-11],     # e-+
               [13],         # mu-
               [2212, 2112], # p n
               [13,-13],
               [13,2212,11,22] ]

    hepEvents = []
    for i,event in enumerate(events):
        hepEvent = CreateEvent(event, i)
        hepEvents.append(hepEvent)
        fHEP.write(hepEvent)
    
    fHEP.close()
    return hepEvents


if __name__ == "__main__":
    print("This works ok but the file seems to be empty on loading... so effectively doesn't work")
    print("don't use this output file... for now")
    CreateDummyEventsInHEPMC2File("egf-hepmc2-dummy-sample.dat")
