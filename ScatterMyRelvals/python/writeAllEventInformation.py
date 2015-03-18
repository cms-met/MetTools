from optparse import OptionParser
import time
parser = OptionParser()
parser.add_option("--input", dest="input", default='', type="string", action="store", help="input pickle file created by findMyRelvalFilesFromEOS.py")
parser.add_option("--tmpdir", dest="tmpdir", default='/tmp/', type="string", action="store", help="temporary directory")
#parser.add_option("--inputFiles", dest="inputFiles", default='relValFiles.pkl', type="string", action="store", help="pkl file with eos filenames")
(options, args) = parser.parse_args()

import ROOT
from DataFormats.FWLite import Events, Handle
from PhysicsTools.PythonAnalysis import *
from math import *
import sys, os, copy, random, subprocess, datetime
import pickle

small = False
maxEvts=-1

archDict={}

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
    print "Architecture:", rel[0],'->',arch[0] 

#print "Retrieving information on architecture from https://cmssdt.cern.ch/SDT/cgi-bin/ReleasesXML"
#os.system('wget https://cmssdt.cern.ch/SDT/cgi-bin/ReleasesXML -P '+options.tmpdir)
#from xml.dom import minidom
#xmldoc = minidom.parse(options.tmpdir+'/ReleasesXML')
#de=xmldoc.documentElement
#archs = de.getElementsByTagName('architecture')
#for a in archs:
#  archDict.update({str(r.getAttribute('label')): str(a.getAttribute('name')) for r in a.getElementsByTagName('project')})


assert options.input!='', "Need to specify input file created with findMyRelvalsFromEOS.py. Syntax: --input=fileName.pkl"
allRelVals=pickle.load(file(options.input))

for k in allRelVals.keys():
  ofile= k.replace('/eos/cms/store/','').replace('/','_')
  fname = '/tmp/relValData_'+ofile+'.pkl'
  if os.path.isfile(fname):
    print "Found",fname,"->skipping!"
    continue
  try:
    release = filter(lambda x:x.startswith('CMSSW_'), k.split('/'))[0]
  except:
    print "Could not determine release from key",k
#  assert archDict.has_key('release'), "No architecture found for release %s"%release
  try:
    arch = archDict['release']
    print "Using architecture",arch
  except:
    arch = "slc6_amd64_gcc481"
    print "No information on release %s in https://cmssdt.cern.ch/SDT/cgi-bin/ReleasesXML therefore using default %s"%(release,arch)

  print "Now doing",k,"release:",release
  sfile = file('exec.sh','w')
  sfile.write('#!/bin/sh\n')
  sfile.write("export chm=$PWD\n")
  sfile.write("export SCRAMARCH="+arch+"\n")
#  sfile.write('source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.sh\n')
  sfile.write("cd "+options.tmpdir+'\n')
  sfile.write("rm -rf "+release+'\n')
  sfile.write("scramv1 project CMSSW "+release+'\n')
  sfile.write('cd '+release+'/src\n')
  sfile.write('eval `scramv1 runtime -sh`\n')
  sfile.write('python $chm/writeEventInformation.py --inputFiles='+','.join(allRelVals[k])+' --outputFile='+fname+'\n')
  sfile.close()
  time.sleep(1)
  os.system('chmod +x exec.sh;./exec.sh;rm exec.sh')
