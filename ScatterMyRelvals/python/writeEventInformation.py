from optparse import OptionParser
parser = OptionParser()
parser.add_option("--inputFiles", dest="inputFiles", default='relValFiles.pkl', type="string", action="store", help="pkl file with eos filenames")
parser.add_option("--run", dest="run", default=-1, type=int, action="store", help="run?")
parser.add_option("--outputFile", dest="outputFile", default='relValData.zpkl', type="string", action="store", help="zpkl file with output data")
parser.add_option("--maxEvents", dest="maxEvents", default=-1, type="int", action="store", help="how many events (max)")
parser.add_option("--miniAOD", dest="miniAOD", action="store_true", help="Just do a miniAOD subset.")

(options, args) = parser.parse_args()

import ROOT
from DataFormats.FWLite import Events, Handle
from PhysicsTools.PythonAnalysis import *
from math import *
import sys, os, copy, random, subprocess, datetime

from helpers import translatePdgIdToType
#from helpers import save

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

vars = ['event/l', 'run/I', 'lumi/I', 'met/F', 'sumEt/F', 'metPhi/F', 'caloMet/F', 'caloSumEt/F', 'caloMetPhi/F']
for t in allCategories:
  vars.append("sumPt_"+t['name']+'/F')
  vars.append("MET_"+t['name']+'/F')
  vars.append("mult_"+t['name']+'/I')
varsPerType={}
for v in vars:
  name, type=v.split('/')
  if not varsPerType.has_key(type):
    varsPerType[type]=[name]
  else:
    varsPerType[type].append(name)

def cDeclaration(k):
  if k=='l':return 'ULong_t'
  if k=='I':return 'Int_t'
  if k=='F':return 'Float_t'
  return 'Float_t'

structString = "struct MyStruct{"
for k in varsPerType.keys():
  structString+=cDeclaration(k)+' '+','.join(varsPerType[k])+';'
structString +='};'
#print structString
ROOT.gROOT.ProcessLine(structString)
from ROOT import MyStruct
s = MyStruct()

inputFiles = options.inputFiles.split(',')
print "Running over files:",inputFiles,"(maxEvents",options.maxEvents,")"

print "Will write to:",options.outputFile
gDir=ROOT.gDirectory
f = ROOT.TFile(options.outputFile, 'recreate')
f.cd()
t = ROOT.TTree( "Events", "Events", 1 )
gDir.cd()

for v in vars:
  name, type=v.split('/')
  t.Branch(name, ROOT.AddressOf(s, name), v)

handles={k:Handle(edmCollections[k][0]) for k in edmCollections.keys()}
res={}

events = Events(inputFiles)
events.toBegin()
products={}
size=events.size() if not small else 10

missingCollections=[]
counter=0
for nev in range(size):
  if nev%1000==0:print nev,'/',size
  events.to(nev)
  eaux=events.eventAuxiliary()
  run=eaux.run()           
  if options.run>0 and not run==options.run:
#    print run, options.run
    continue
  counter+=1
  if options.maxEvents>0 and counter>options.maxEvents: break
  event=eaux.event()
  lumi=eaux.luminosityBlock()
  setattr(s,"event",event)
  setattr(s,"lumi",lumi)
  setattr(s,"run",run)
#  evkey = ":".join(str(x) for x in [run,lumi,event])
  for k in [ x for x in edmCollections.keys() if x not in missingCollections]:
    try:
      events.getByLabel(edmCollections[k][1:],handles[k])
      products[k]=handles[k].product()
    except:
      products[k]=None
      print "Not found:",k
      missingCollections.append(k)
#  if options.miniAOD:
#    "Flag_METFilters"
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
  for c in allCategories:
    setattr(s,'MET_'+c['name'], sqrt(MEx["MEx_"+c['name']]**2+MEy["MEy_"+c['name']]**2))
    setattr(s,'mult_'+c['name'], mult["mult_"  +c['name']])
    setattr(s,'sumPt_'+c['name'], sumPt["sumPt_"  +c['name']])

  if products["pfMet"] and "pfMet" not in missingCollections:
    if options.miniAOD:
      d.update({'met':products["pfMet"][0].shiftedPt(12, 0),  'sumEt':products["pfMet"][0].sumEt(), 'metPhi':products["pfMet"][0].phi()})
      setattr(s,"met",products["pfMet"][0].shiftedPt(12, 0))
    else:
      d.update({'met':products["pfMet"][0].pt(),  'sumEt':products["pfMet"][0].sumEt(), 'metPhi':products["pfMet"][0].phi()})
      setattr(s,"met",products["pfMet"][0].pt())
    setattr(s,"sumEt",products["pfMet"][0].sumEt())
    setattr(s,"metPhi",products["pfMet"][0].phi())
  if not options.miniAOD and products["caloMet"] and "caloMet" not in missingCollections:
    d.update({'caloMet':products["caloMet"][0].pt(),  'caloSumEt':products["caloMet"][0].sumEt(), 'caloMetPhi':products["caloMet"][0].phi()})
    setattr(s,"caloMet",products["caloMet"][0].pt())
    setattr(s,"caloSumEt",products["caloMet"][0].sumEt())
    setattr(s,"caloMetPhi",products["caloMet"][0].phi())
#  res[evkey]=d
  if nev==0:
    print "Does the first event look ok?"
    print d 
  t.Fill()

f.cd()
t.Write()
f.Close()
print "Written",options.outputFile
