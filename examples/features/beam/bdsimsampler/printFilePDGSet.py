from collections import defaultdict
import pybdsim


def Analyse(filename, samplerName="c1"):

    d = pybdsim.Data.Load(filename)

    histogram = defaultdict(int)

    eventIndex = 0
    sets = []
    for event in d.GetEventTree():

        s = getattr(event, samplerName)

        pdgids = set(list(s.partID))
        
        sets.append(pdgids)
        print("Event #:", eventIndex)
        print(sorted(pdgids))
        eventIndex += 1

    # print out unique pdgids in each event
    for i,s in enumerate(sets):
        # form a set of all pdgids across all events but this one
        allothereventpdgids = set().union(*[v for j,v in enumerate(sets) if j != i])
        unique = s.difference(allothereventpdgids)
        if len(unique) > 0:
            print("Event ",i,"unique pdgids: ",unique)
        else:
            print("Event ",i,"no unique pdgids")

        for key in s:
            histogram[key] += 1

    allpdgids = set().union(*sets)
    print("\nAll pdgids across the file: ",sorted(allpdgids))

    print("\nCount of number of events with a given PDG ID")
    for key in sorted(histogram.keys()):
        print(key,histogram[key])


if __name__ == "__main__":
    Analyse("../../data/shower.root")
