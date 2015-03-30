from optparse import OptionParser
import time
parser = OptionParser()
parser.add_option("--input", dest="input", default='', type="string", action="store", help="input pickle file created by findMyRelvalFilesFromEOS.py")
parser.add_option("--tmpdir", dest="tmpdir", default='/tmp/', type="string", action="store", help="temporary directory")
parser.add_option("--pattern", dest="pattern", default='', type="string", action="store", help="pattern to define a subset")
parser.add_option("--p", dest="pretend", action="store_true", help="just pretend")

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

small = False
maxEvts=-1

archDict={}

print "\nRetrieving information on architecture from scramv1"
p = os.popen("/cvmfs/cms.cern.ch/common/scramv1 list -a CMSSW","r")
while True:
  l = p.readline()
  if not l:break
  if not ('slc' in l and 'CMSSW' in l): continue
  l=l[:-1]
  s=l.split('/')
  arch = filter(lambda x:x.startswith('slc'),s)
  rel = filter(lambda x:x.startswith('CMSSW'),s)
  if not (len(arch)==1 and len(rel)>=1):
    print "Don't understand",l
  else:
    archDict[rel[0]]=arch[0]
    print "[scramv1] Architecture:", rel[0],'->',arch[0] 

print "\nRetrieving information on architecture from https://cmssdt.cern.ch/SDT/cgi-bin/ReleasesXML"
os.system('wget https://cmssdt.cern.ch/SDT/cgi-bin/ReleasesXML -P '+options.tmpdir+' --no-check-certificate')
from xml.dom import minidom
xmldoc = minidom.parse(options.tmpdir+'/ReleasesXML')
de=xmldoc.documentElement
archs = de.getElementsByTagName('architecture')
for a in archs:
  dict={str(r.getAttribute('label')): str(a.getAttribute('name')) for r in a.getElementsByTagName('project')}
  archDict.update(dict)
  for k in dict.keys():
    print "[ReleasesXML] Architecture:", k,'->',archDict[k]

print

for k in allRelVals.keys():
  if options.pattern not in k:continue
  ofile= k.replace('/eos/cms/store/','').replace('/','_').replace('~','')
  fname = '/tmp/relValData_'+ofile+'.zpkl'
  if os.path.isfile(fname):
    print "Found",fname,"->skipping!"
    continue
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

  if options.pretend: 
    print "Release: %s Architecture: %s tmpDir: %s RelVal: %s"%(release, arch, options.tmpdir, k)
  else:
    print "Now doing relval:",k,"in release:",release
    execFileName = options.tmpdir+'/exec.sh'
    sfile = file(execFileName,'w')
    sfile.write('#!/bin/sh\n')
    sfile.write("export chm=$PWD\n")
    sfile.write("export SCRAM_ARCH="+arch+"\n")
  #  sfile.write('source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.sh\n')
    sfile.write("cd "+options.tmpdir+'\n')
    sfile.write("rm -rf "+release+'\n')
    sfile.write("scramv1 project CMSSW "+release+'\n')
    sfile.write('cd '+release+'/src\n')
    sfile.write('eval `scramv1 runtime -sh`\n')
    opts = ['--inputFiles='+','.join(['root://eoscms.cern.ch/'+x.replace('~','') for x in allRelVals[k]]), '--outputFile='+fname]
    if 'miniaod' in k.lower():
      opts.append('--miniAOD')
    sfile.write('python $chm/writeEventInformation.py '+' '.join(opts)+'\n')
    sfile.close()
    time.sleep(1)
    os.system('chmod +x '+execFileName+';sh '+execFileName+';rm '+execFileName)
