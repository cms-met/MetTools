import subprocess
import os, pickle

pattern = "CMSSW_7"
ofile = 'relValFiles_73X.pkl' 
eosRelValDir = "/eos/cms/store/relval"
eosCMD = "/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select"

def oneLevelDeeper(dir, subdir=None):
  p = os.popen(" ".join([eosCMD, "ls", dir]),"r")
  all=[]
  while True:
    l = p.readline()
    if not l: break
    dn=l[:-1]
    if subdir and (dn==subdir or subdir=="*"):
      return dir+'/'+dn
    if not subdir:
      all.append(dir+'/'+dn)
  return all 

results={}
releases = oneLevelDeeper(eosRelValDir)
for rel in releases:
  if pattern not in rel:continue
  relVals = oneLevelDeeper(rel)
  for relVal in relVals:
    dataTiers = oneLevelDeeper(relVal)
    for dt in dataTiers:
      if (not 'RECO' in dt) and (not 'AOD' in dt): continue
      conditions = oneLevelDeeper(dt)
      for cond in conditions:
        files=[]
        subDirs = oneLevelDeeper(cond)
        for s in subDirs:
          files+= oneLevelDeeper(s)
        results[cond] = files

pickle.dump(results, file(ofile,'w'))
print "Written",ofile 
