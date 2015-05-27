import subprocess
import os, pickle
from helpers import getContent
from optparse import OptionParser

parser = OptionParser()
parser.add_option("--releasePattern", dest="releasePattern", default='CMSSW_7_4', type="string", action="store", help="releasePattern to define a subset")
parser.add_option("--outputFile", dest="outputFile", default='relValFiles_74X.pkl', type="string", action="store", help="output pickle file")

(options, args) = parser.parse_args()

eosRelValDir = "/eos/cms/store/relval"

results={}
releases = getContent(eosRelValDir, eos=True)
for rel in releases:
  if options.releasePattern not in rel:continue
  relVals = getContent(rel, eos=True)
  for relVal in relVals:
    dataTiers = getContent(relVal, eos=True)
    for dt in dataTiers:
      if (not 'RECO' in dt) and (not 'AOD' in dt): continue
      conditions = getContent(dt, eos=True)
      for cond in conditions:
        files=[]
        cont = getContent(cond, eos=True)
        for s in cont:
          files+= getContent(s, eos=True)
        if files==[]:continue
        results[cond] = [f.replace('/eos/cms/', 'root://eoscms.cern.ch//') for f in files]
        print "Found %s with %i files" %(cond,len(results[cond]))

pickle.dump(results, file(options.outputFile,'w'))
print "Written",options.outputFile
