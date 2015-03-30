import subprocess
import os, pickle
from optparse import OptionParser

parser = OptionParser()
parser.add_option("--pattern", dest="pattern", default='CMSSW_7', type="string", action="store", help="pattern to define a subset")
parser.add_option("--outputFile", dest="outputFile", default='relValFiles_73X.pkl', type="string", action="store", help="output pickle file")

(options, args) = parser.parse_args()

#if not os.path.ismount('~/eos'):
os.system('mkdir -p ~/eos;/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select -b fuse mount ~/eos')

eosRelValDir = "~/eos/cms/store/relval"
#eosCMD = "/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select"

def oneLevelDeeper(dir, subdir=None):
#  p = os.popen(" ".join([eosCMD, "ls", dir]),"r")
  p = os.popen(" ".join(["ls", dir]),"r")
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
  if options.pattern not in rel:continue
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
        if files==[]:continue
        results[cond] = files
        print "Found",cond

pickle.dump(results, file(options.outputFile,'w'))
print "Written",options.outputFile
