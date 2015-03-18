import subprocess
import os, pickle

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

eosRelValDir = "/eos/cms/store/relval"
eosCMD = "/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select"
#relVal = "RelValTTbar_13"
relVal = "RelValSingleMuPt10_UP15"
p = os.popen(" ".join([eosCMD, "ls", eosRelValDir]),"r")
results={}
while True:
  l = p.readline()
  if not l: break
  dn=l[:-1]
  if not dn.startswith('CMSSW_7'):continue
  rvd = oneLevelDeeper(eosRelValDir+'/'+dn, relVal)
  if rvd: pass #print rvd
  else: 
    print "Couldn't find "+relVal+" in "+eosRelValDir+'/'+dn
    continue
  recoDir = oneLevelDeeper(rvd, 'GEN-SIM-RECO')
  if not recoDir: recoDir = oneLevelDeeper(rvd, 'GEN-SIM-DIGI-RECO')
  if not recoDir: 
    print "No RECO found"
    continue
  dirsWithTags = oneLevelDeeper(recoDir)
  dirsWithTags = [oneLevelDeeper(d,"*") for d in dirsWithTags]
  for d in dirsWithTags:
    files = ['root://eoscms.cern.ch/'+f for f in oneLevelDeeper(d) ]
    if len(files)>0:
      results[d]=files
      print "Found %i files for %s"%(len(files), d)
pickle.dump(results, file(relVal+"_relValFiles.pkl",'w'))
print "Written "+relVal+"_relValFiles.pkl"
#of = file('relValFiles.txt','w')
#for k in results.keys():
#  of.write(k+'\n')
#  for f in results[k]:
#    of.write('  '+f+'\n')
#of.close()
#print "Written relValFiles.txt"
#of = file('relValFiles_keys.txt','w')
#for k in results.keys():
#  of.write(k+'\n')
#of.close()
#print "Written relValFiles_keys.txt"
#  
