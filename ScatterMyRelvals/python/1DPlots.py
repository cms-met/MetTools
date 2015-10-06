from optparse import OptionParser
parser = OptionParser()
parser.add_option("--file1", dest="file1", default='', type="string", action="store", help="[moniker:filname.zpkl for 1")
parser.add_option("--file2", dest="file2", default='', type="string", action="store", help="[moniker:filname.zpkl for 2]")
parser.add_option("--vetoList", dest="vetoList", default='', type="string", action="store", help="vetoList pkl file")
parser.add_option("--run", dest="run", default=-1, type="int", action="store", help="which run")
parser.add_option("--percentile", dest="percentile", default=-1, type="float", action="store", help="percentile 0.99 -> plotrange adjusted to keep keep lower 99%")
parser.add_option("--plotDirectory", dest="plotDirectory", default='.', type="string", action="store", help="Where should the plots go?")
parser.add_option("--profile", dest="profile", action="store_true", help="Whether to do profile.")

(options, args) = parser.parse_args()

import ROOT
from DataFormats.FWLite import Events, Handle
from PhysicsTools.PythonAnalysis import *
from math import *
import sys, os, copy, random, subprocess, datetime

from helpers import load as zpklLoad

def load(f, nMax=-1):
  if f[-5:]=='.zpkl':
    print "Loading zpkl file %s"%f
    return zpklLoad(f)
  else:
    res={}
    print "Loading root file %s"%f
    c = ROOT.TChain("Events")
    c.Add(f)
    nEvents = c.GetEntries() if nMax<0 else min(c.GetEntries(), nMax)
    branches=c.GetListOfBranches()
    brNames=[]
    for ib in range(branches.GetSize()):
      name = branches.At(ib).GetName()
      if name not in ["run","lumi","event"]:
        brNames.append(branches.At(ib).GetName())
    for i in range(int(nEvents)):
      if i%10000==0 and i>0: print "Loading at event %i / %i"%(i,nEvents)
      c.GetEntry(i)
      key=":".join([str(int(c.GetLeaf(x).GetValue())) for x in ["run","lumi","event"]])
      res[key] = {name:c.GetLeaf(name).GetValue() for name in brNames}
#      print key, res[key]
    return res

ROOT.gStyle.SetOptStat(0)
if not hasattr(ROOT, "tdrStyle"):
  ROOT.gROOT.ProcessLine(".L $CMSSW_BASE/src/MetTools/Commons/scripts/tdrstyle.C")
  ROOT.setTDRStyle()
#  ROOT.tdrStyle.SetPadRightMargin(0.8)
  ROOT.gROOT.ProcessLine(".L $CMSSW_BASE/src/MetTools/Commons/scripts/useNiceColorPalette.C")
  ROOT.useNiceColorPalette(255)

maxEvts=-1

if ':' in options.file1:
  moniker_1, ifile_1 = options.file1.split(':')
else:
  moniker_1, ifile_1 = None, options.file1
if ':' in options.file2:
  moniker_2, ifile_2 = options.file2.split(':')
else:
  moniker_2, ifile_2 = None, options.file2

vetoList=[]
if options.vetoList!='':
  import pickle
  vetoList = pickle.load(file(options.vetoList))

nMax=-1
evts1 = load(ifile_1, nMax=nMax)
evts2 = load(ifile_2, nMax=nMax)

print "File %s for x-axis has %i events"%(ifile_1,len(evts1))
print "File %s for y-axis has %i events"%(ifile_2,len(evts2))
if options.run>0:
  for e in evts1.keys():
    if not e.startswith(str(options.run)+':'):
      del evts1[e]
  for e in evts2.keys():
    if not e.startswith(str(options.run)+':'):
      del evts2[e]
  print "After selecting run %i: File %s for x-axis has %i events"%(options.run, ifile_1,len(evts1))
  print "After selecting run %i: File %s for y-axis has %i events"%(options.run, ifile_2,len(evts2))

if vetoList!=[]:
  for v in vetoList:
    if evts1.has_key(v):
      del evts1[v]
    if evts2.has_key(v):
      del evts2[v]
  print "After vetoList: File %s for x-axis has %i events"%(ifile_1,len(evts1))
  print "After vetoList: File %s for y-axis has %i events"%(ifile_2,len(evts2))
    

commonKeys =  [val for val in evts1.keys() if val in evts2.keys()]
assert len(commonKeys)>0, "0 events!"
print "Found",len(commonKeys),"common events"
ks1 = evts1[commonKeys[0]].keys()
ks1.sort()
ks2 = evts2[commonKeys[0]].keys()
ks2.sort()

if ks1!=ks2:
  print "Warning! Not the same keys!"
  print "1st:",ks1
  print "2nd:",ks2

scatterPlots = [val for val in ks1 if val in ks2]

if not os.path.exists(options.plotDirectory):
  os.makedirs(options.plotDirectory)

for s in scatterPlots:
  if options.percentile>0:
    import numpy as np
    l = np.array([evts1[k][s] for k in commonKeys] + [evts2[k][s] for k in commonKeys])
    maxVal = np.percentile(l, 100*options.percentile) 
  else:
    maxVal = max([evts1[k][s] for k in commonKeys] + [evts2[k][s] for k in commonKeys])
    if not  maxVal>0:
      print "maxVal non-positive %f"%maxVal
      continue

  if 'mult' in s:
    binning=[min(100,int(maxVal)),0,int(maxVal)]
  else:
    scale = 10**(int(log(maxVal,10))-1)
    maximum= (floor(maxVal/scale)+1)*scale
    binning=[100,0,maximum]
#  if s=='met':binning=[100,0,5]
  histo1   = ROOT.TH1F('h'+s,'h'+s,*(binning))
  histo2   = ROOT.TH1F('h'+s,'h'+s,*(binning))
  for k in commonKeys:
    histo1.Fill(evts1[k][s])
    histo2.Fill(evts2[k][s])
  mstr_1 = ' ('+moniker_1+')' if moniker_1 else ''
  mstr_2 = ' ('+moniker_2+')' if moniker_2 else ''
  histo1.SetLineColor(ROOT.kBlue)
  histo1.SetMarkerStyle(0)
  histo1.SetMarkerColor(ROOT.kBlue)
  histo1.SetMarkerSize(0)
  histo2.SetLineColor(ROOT.kRed)
  histo2.SetMarkerStyle(0)
  histo2.SetMarkerColor(ROOT.kRed)
  histo2.SetMarkerSize(0)
  histo1.GetXaxis().SetTitle(s)
  histo1.GetXaxis().SetLabelSize(0.03)
  histo1.GetXaxis().SetTitleSize(0.0385)
  histo2.GetXaxis().SetTitle(s)
  histo2.GetXaxis().SetLabelSize(0.03)
  histo2.GetXaxis().SetTitleSize(0.0385)

  l = ROOT.TLegend(0.5,0.8,1,1)
  l.SetFillColor(0)
  l.SetShadowColor(ROOT.kWhite)
  l.SetBorderSize(1)

  l.AddEntry(histo1, s+mstr_1) 
  l.AddEntry(histo2, s+mstr_2) 

  c1 = ROOT.TCanvas()
  histo1.Draw('h')
  c1.SetLogy()
  histo2.Draw('hsame')
  text=ROOT.TLatex()
  text.SetNDC()
  text.SetTextSize(0.04)
  text.SetTextAlign(11)
  text.DrawLatex(0.2,0.9,"CMS Preliminary")
#  text.SetTextColor(ROOT.kRed)
  l.Draw()
  c1.Print(options.plotDirectory+'/1D_'+s+'.png')
  c1.Print(options.plotDirectory+'/1D_'+s+'.pdf')
  c1.Print(options.plotDirectory+'/1D_'+s+'.root')
  c1.RedrawAxis()
  del histo1
  del histo2
  del c1
