import glob
import numpy as np
import re

def FindMissingSeeds(dirAndGlob,
                     outputFileName,
                     pattern = r"(\d+)_\w+.root",
                     minSeed=None,
                     maxSeed=None):
    """
    For files names 1234_sometext.root where 1234 is commonly the seed
    of the simulation. We can identify the missing ones from a run on
    the farm by making a set of file names and comparing.

    FindMissingSeeds("/path/to/my/files/*.root", "missingseeds.txt")

    The regular expression (as a raw string) is used to match the seed
    number from the name. The 1st capture group should be the number.
    """
    files = glob.glob(dirAndGlob)
    p = re.compile(pattern)

    seedsFound = []
    for f in files:
        f = f.split('/')[-1] # strip off dir
        match = p.search(f)
        if not match:
            continue
        else:
            seed = int(match.group(1))
            seedsFound.append(seed)

    seedsFoundA = np.array(seedsFound)
    sMin = minSeed if minSeed else np.min(seedsFoundA)
    sMax = maxSeed if maxSeed else np.max(seedsFoundA)

    allSeeds = np.arange(sMin, sMax+1)
    allSeeds = set(allSeeds)
    seedsFound = set(seedsFound)

    missing = list(allSeeds.difference(seedsFound))

    of = open(outputFileName,"w")
    of.write('\n'.join([str(s) for s in missing]))
    of.close()
