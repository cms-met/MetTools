from optparse import OptionParser
parser = OptionParser()
parser.add_option("--xAxis", dest="xAxis", default='', type="string", action="store", help="[moniker:filname.zpkl for x-axis")
parser.add_option("--yAxis", dest="yAxis", default='', type="string", action="store", help="[moniker:filname.zpkl for y-axis]")
parser.add_option("--plotDirectory", dest="plotDirectory", default='.', type="string", action="store", help="Where should the plots go?")

(options, args) = parser.parse_args()

import ROOT
from DataFormats.FWLite import Events, Handle
from PhysicsTools.PythonAnalysis import *
from math import *
import sys, os, copy, random, subprocess, datetime

from MetTools.Commons.helpers import load

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

evts1 = load(ifile_x)
evts2 = load(ifile_y)

print "File %s for x-axis has %i events"%(ifile_x,len(evts1))
print "File %s for y-axis has %i events"%(ifile_y,len(evts2))

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
  maxVal = max([evts1[k][s] for k in commonKeys] + [evts2[k][s] for k in commonKeys])
  if not  maxVal>0:
    print "maxVal non-positive %f"%maxVal
    continue
  if 'mult' in s:
    binning=[min(100,maxVal),0,maxVal]
    pbinning=[min(20,maxVal),0,maxVal]
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
