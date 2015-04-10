from optparse import OptionParser
import time
parser = OptionParser()
parser.add_option("--input", dest="input", default='', type="string", action="store", help="input pickle file created by findMyRelvalFilesFromEOS.py")
parser.add_option("--plotDir", dest="plotDir", default='/tmp/', type="string", action="store", help="temporary directory")
parser.add_option("--tmpDir", dest="tmpDir", default='/tmp/', type="string", action="store", help="temporary directory")
parser.add_option("--relValKey", dest="relValKey", default='', type="string", action="store", help="relValKey in the input pickle")

(options, args) = parser.parse_args()

import ROOT
from DataFormats.FWLite import Events, Handle
from PhysicsTools.PythonAnalysis import *
from math import *
import sys, os, copy, random, subprocess, datetime
import pickle

#fileNames = pickle.load(file('relValFiles_73X.pkl'))
assert options.input!='', "Need to specify input file created with findMyRelvalsFromEOS.py. Syntax: --input=fileName.pkl"
allRelVals=pickle.load(file(options.input))

assert allRelVals.has_key(options.relValKey), "Key %s not found in input file %s" %(options.relValKey, options.input)

small = False
maxEvts=-1

from helpers import architectures
archDict = architectures()

k = options.relValKey
osubdir= k.replace('/eos/cms/store/','').replace('/','_').replace('~','')
plotDir = options.plotDir+'/'+osubdir
print "Will store plots in %s"%plotDir
try:
  release = filter(lambda x:x.startswith('CMSSW_'), k.split('/'))[0]
except:
  print "Could not determine release from key",k
#  assert archDict.has_key('release'), "No architecture found for release %s"%release
try:
  arch = archDict[release]
except:
  arch = "slc6_amd64_gcc481"
  print "No information on release %s using default %s"%(release,arch)

print "Plotting:",k,"in release:",release
execFileName = options.tmpDir+'/exec.sh'
sfile = file(execFileName,'w')
sfile.write('#!/bin/sh\n')
sfile.write("export chm=$PWD\n")
sfile.write("export SCRAM_ARCH="+arch+"\n")
#  sfile.write('source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.sh\n')
sfile.write("cd "+options.tmpDir+'\n')
sfile.write("rm -rf "+release+'\n')
sfile.write("scramv1 project CMSSW "+release+'\n')
sfile.write('cd '+release+'/src\n')
sfile.write('eval `scramv1 runtime -sh`\n')
opts = ['--inputFiles='+','.join(['root://eoscms.cern.ch/'+x.replace('~','') for x in allRelVals[k]]), '--plotDir='+plotDir]
if 'miniaod' in k.lower():
  opts.append('--miniAOD')
sfile.write('python $chm/occupancyPlotsFromFiles.py '+' '.join(opts)+'\n')
sfile.close()
time.sleep(1)
os.system('chmod +x '+execFileName+';sh '+execFileName+';rm '+execFileName)
