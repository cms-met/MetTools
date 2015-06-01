from optparse import OptionParser
parser = OptionParser()
parser.add_option("--xAxis", dest="xAxis", default='', type="string", action="store", help="[moniker:filname.zpkl for x-axis")
parser.add_option("--yAxis", dest="yAxis", default='', type="string", action="store", help="[moniker:filname.zpkl for y-axis]")
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

def load(f):
  if f[-5:]=='.zpkl':
    print "Loading zpkl file %s"%f
    return zpklLoad(f)
  else:
    res={}
    print "Loading root file %s"%f
    c = ROOT.TChain("Events")
    c.Add(f)
    nEvents=c.GetEntries()
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
  ROOT.tdrStyle.SetPadRightMargin(0.18)
  ROOT.gROOT.ProcessLine(".L $CMSSW_BASE/src/MetTools/Commons/scripts/useNiceColorPalette.C")
  ROOT.useNiceColorPalette(255)

maxEvts=-1

if ':' in options.xAxis:
  moniker_x, ifile_x = options.xAxis.split(':')
else:
  moniker_x, ifile_x = None, options.xAxis
if ':' in options.yAxis:
  moniker_y, ifile_y = options.yAxis.split(':')
else:
  moniker_y, ifile_y = None, options.yAxis

vetoList=[]
if options.vetoList!='':
  import pickle
  vetoList = pickle.load(file(options.vetoList))

evts1 = load(ifile_x)
evts2 = load(ifile_y)

print "File %s for x-axis has %i events"%(ifile_x,len(evts1))
print "File %s for y-axis has %i events"%(ifile_y,len(evts2))
if options.run>0:
  for e in evts1.keys():
    if not e.startswith(str(options.run)+':'):
      del evts1[e]
  for e in evts2.keys():
    if not e.startswith(str(options.run)+':'):
      del evts2[e]
  print "After selecting run %i: File %s for x-axis has %i events"%(options.run, ifile_x,len(evts1))
  print "After selecting run %i: File %s for y-axis has %i events"%(options.run, ifile_y,len(evts2))

if vetoList!=[]:
  for v in vetoList:
    if evts1.has_key(v):
      del evts1[v]
    if evts2.has_key(v):
      del evts2[v]
  print "After vetoList: File %s for x-axis has %i events"%(ifile_x,len(evts1))
  print "After vetoList: File %s for y-axis has %i events"%(ifile_y,len(evts2))
    

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
    pbinning=[min(20,int(maxVal)),0,int(maxVal)]
  else:
    scale = 10**(int(log(maxVal,10))-1)
    maximum= (floor(maxVal/scale)+1)*scale
    binning=[100,0,maximum]
    pbinning=[20,0,maximum]
  histo   = ROOT.TH2F('h'+s,'h'+s,*(binning+binning))
  profile = ROOT.TProfile('p'+s,'p'+s,*(pbinning+[0,maximum]))
  for k in commonKeys:
    histo.Fill(evts1[k][s], evts2[k][s])
    profile.Fill(evts1[k][s], evts2[k][s])
  mstr_x = ' ('+moniker_x+')' if moniker_x else ''
  mstr_y = ' ('+moniker_y+')' if moniker_y else ''
  histo.GetXaxis().SetTitle(s+mstr_x)
  histo.GetXaxis().SetLabelSize(0.03)
  histo.GetXaxis().SetTitleSize(0.0385)
  histo.GetYaxis().SetTitle(s+mstr_y)
  histo.GetYaxis().SetLabelSize(0.03)
  histo.GetYaxis().SetTitleSize(0.0385)
  profile.GetXaxis().SetTitle(s+mstr_x)
  profile.GetXaxis().SetLabelSize(0.03)
  profile.GetXaxis().SetTitleSize(0.0385)
  profile.GetYaxis().SetTitle(s+mstr_y)
  profile.GetYaxis().SetLabelSize(0.03)
  profile.GetYaxis().SetTitleSize(0.0385)
  profile.SetLineColor(ROOT.kGray)
  profile.SetMarkerStyle(0)
  profile.SetMarkerSize(0)

  c1 = ROOT.TCanvas()
  histo.Draw('colz')
#  fit=True
#  tf1=ROOT.TF1("lin","pol1",0,histo.GetXaxis().GetXmax())
#  try:
#    frs=profile.Fit(tf1,'S')
#    fit=(frs.Status()==0)
#  except:
#    fit=False 
  c1.SetLogz()
  histo.Draw('colz')
  if options.profile:
    profile.Draw("eh1same")
  l=ROOT.TLine(0,0,histo.GetXaxis().GetXmax(),histo.GetXaxis().GetXmax())
  l.Draw()
#  if fit:
#    text=ROOT.TLatex()
#    text.SetNDC()
#    text.SetTextSize(0.04)
#    text.SetTextAlign(11)
#    text.SetTextColor(ROOT.kRed)
#    text.DrawLatex(0.2,0.9,"Fit: "+str(round(tf1.GetParameter(0),4))+"+("+str(round(tf1.GetParameter(1),4))+")*x")
  c1.Print(options.plotDirectory+'/'+s+'.png')
  c1.Print(options.plotDirectory+'/'+s+'.pdf')
  c1.Print(options.plotDirectory+'/'+s+'.root')
  c1.RedrawAxis()
  del profile
  del histo
  del c1
