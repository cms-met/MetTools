from optparse import OptionParser
parser = OptionParser()
parser.add_option("--inputFiles", dest="inputFiles", default='relValFiles.pkl', type="string", action="store", help="pkl file with eos filenames")
parser.add_option("--outputFile", dest="outputFile", default='relValData.zpkl', type="string", action="store", help="zpkl file with output data")
parser.add_option("--miniAOD", dest="miniAOD", action="store_true", help="Just do a miniAOD subset.")

(options, args) = parser.parse_args()

import ROOT
from DataFormats.FWLite import Events, Handle
from PhysicsTools.PythonAnalysis import *
from math import *
import sys, os, copy, random, subprocess, datetime

def translatePdgIdToType(pdg): 
  apdg = abs(pdg)
  if apdg==211: return 1
  if apdg==11:  return 2
  if apdg==13:  return 3
  if apdg==22:  return 4
  if apdg==130: return 5
  if apdg==1:   return 6
  if apdg==2:   return 7
  return 0

def save(object, filename, protocol = -1):
  import cPickle, gzip
  """Save an object to a compressed disk file.
     Works well with huge objects.
  """
  file = gzip.GzipFile(filename, 'wb')
  cPickle.dump(object, file, protocol)
  file.close()

small = False

ROOT.gSystem.Load("libFWCoreFWLite.so")
ROOT.AutoLibraryLoader.enable()

maxEvts=-1
if options.miniAOD:
  print "Running on miniAOD"
  edmCollections = { \
    'pfMet':("vector<pat::MET>", "slimmedMETs", ""), #, "RECO")
    'pfCandidates':("vector<pat::PackedCandidate>", "packedPFCandidates", "")
  } 
else:
  print "Running on RECO/AOD"
  edmCollections = { \
    'pfMet':("vector<reco::PFMET>", "pfMet", ""), #, "RECO")
    'caloMet':("vector<reco::CaloMET>", "caloMet",""),
    'pfCandidates':("vector<reco::PFCandidate>", "particleFlow", "")
  }

from categories import *
inputFiles = options.inputFiles.split(',')
print "Running over files:",inputFiles
print "Will write to:",options.outputFile

handles={k:Handle(edmCollections[k][0]) for k in edmCollections.keys()}
res={}

events = Events(inputFiles)
events.toBegin()
products={}
size=events.size() if not small else 10

missingCollections=[]
for nev in range(size):
  if nev%1000==0:print nev,'/',size
  events.to(nev)
  eaux=events.eventAuxiliary()
  run=eaux.run()            
  event=eaux.event()
  lumi=eaux.luminosityBlock()
  evkey = ":".join(str(x) for x in [run,lumi,event])
  for k in [ x for x in edmCollections.keys() if x not in missingCollections]:
    try:
      events.getByLabel(edmCollections[k][1:],handles[k])
      products[k]=handles[k].product()
    except:
      products[k]=None
      print "Not found:",k
      missingCollections.append(k)
  d={}
#  print "Event",nev,'size',products['pfCandidates'].size()
  if products['pfCandidates']:
    sumPt={"sumPt_"+t['name']:0. for t in allCategories}
    MEx={"MEx_"+t['name']:0. for t in allCategories}
    MEy={"MEy_"+t['name']:0. for t in allCategories}
    mult={"mult_"+t['name']:0 for t in allCategories}
    for p in range(products['pfCandidates'].size()):
      cand = products['pfCandidates'][p]
      candID = translatePdgIdToType(cand.pdgId()) if options.miniAOD else cand.particleId() 
      l=label[candID]
      for c in allCategories:
        if c['type']==l:
          aeta=abs(cand.eta())
          if aeta>=c['etaRange'][0] and aeta<c['etaRange'][1]:
            sumPt["sumPt_"+c['name']]+=cand.pt() 
            mult["mult_"  +c['name']]+=1 
            MEx["MEx_"    +c['name']]+=-cand.px() 
            MEy["MEy_"    +c['name']]+=-cand.py()
    d.update({'MET_'+c['name']:sqrt(MEx["MEx_"+c['name']]**2+MEy["MEy_"+c['name']]**2) for c in allCategories })
    d.update(sumPt)
    d.update(mult)
  if products["pfMet"] and "pfMet" not in missingCollections:
    d.update({'met':products["pfMet"][0].pt(),  'sumEt':products["pfMet"][0].sumEt(), 'metPhi':products["pfMet"][0].phi()})
  if not options.miniAOD and products["caloMet"] and "caloMet" not in missingCollections:
    d.update({'caloMet':products["caloMet"][0].pt(),  'caloSumEt':products["caloMet"][0].sumEt(), 'caloMetPhi':products["caloMet"][0].phi()})
  res[evkey]=d

if len(res)>0:
  print "Does the first event look ok?"
  print res[res.keys()[0]]

save(res, options.outputFile)
print "Written",options.outputFile
