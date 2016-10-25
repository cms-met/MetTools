from optparse import OptionParser
parser = OptionParser()
parser.add_option("--mode", dest="mode", default='sumPt', type="string", action="store", help="Which mode [multiplicity/ngoodVertices/sumPt]")
parser.add_option("--plotDir", dest="plotDir", default="/", type="string", action="store", help="dir name Default:/")
#parser.add_option("--Samples", dest="Samples", default=" ", type="string", action="store", help="Samples to be used")

(options, args) = parser.parse_args()
assert options.mode in ["multiplicity","ngoodVertices","sumPt"],"Mode %s not known. Must be one of [ngoodVertices/sumPt/multiplicity]"%options.mode

allMaps = ['hEtaPlus', 'hEtaMinus', 'h0Barrel', 'h0EndcapPlus', 'h0EndcapMinus', 'gammaBarrel', 'gammaEndcapPlus', 'gammaEndcapMinus', 'gammaForwardPlus', 'gammaForwardMinus', 'e', 'mu',
           'hHFMinus', 'hHFPlus', \
           'egammaHFMinus', 'egammaHFPlus', 
          ]
#allMaps = ['hEtaPlus', 'hEtaMinus', 'h0Barrel', 'h0EndcapPlus', 'h0EndcapMinus', 'gammaBarrel', 'gammaEndcapPlus', 'gammaEndcapMinus', 'gammaForwardPlus', 'gammaForwardMinus', 'e', 'mu',
#           'hHFMinus', 'hHFPlus', \
#           'egammaHFMinus', 'egammaHFPlus', 
#          ]

import ROOT
import pickle, os
from MetTools.MetPhiCorrections.tools.helpers import getObjFromFile

from math import pi, cos, sin, sqrt, atan2
ROOT.gROOT.ProcessLine(".L $CMSSW_BASE/src/MetTools/Commons/scripts/tdrstyle.C")
ROOT.setTDRStyle()
from MetTools.MetPhiCorrections.tools.helpers import getObjFromFile

i=0
j=0
px=[]
py=[]
fx=[]
fy=[]
for map in allMaps:
  #c1.Clear()
  cx = ROOT.TCanvas()
  cy = ROOT.TCanvas()  
  lx = ROOT.TLegend(0.55,0.83,.95,.95)
  ly = ROOT.TLegend(0.55,0.83,.95,.95)
  for Sample in ['DY', 'WJetsToLNu', 'TTJets', 'Run2016CDEFG']:
    fileName = options.plotDir+'_'+Sample+'/'+map+"_"+options.mode+".root"
    print "open: "+fileName
    px.append(getObjFromFile(fileName, map+"_"+options.mode+'_Px'))
    py.append(getObjFromFile(fileName, map+"_"+options.mode+'_Py'))
    fx.append(getObjFromFile(fileName, 'fx'))
    fy.append(getObjFromFile(fileName, 'fy'))
    if Sample == "DY":
      px[i].SetLineColor(ROOT.kRed)
      fx[i].SetLineColor(ROOT.kRed)
      px[i].SetMarkerStyle(21)
      px[i].SetMarkerSize(1)
      px[i].SetMarkerColor(ROOT.kRed)
      lx.AddEntry(px[i], "DY #slash{E}_{x}")
      py[i].SetLineColor(ROOT.kRed)
      fy[i].SetLineColor(ROOT.kRed)
      py[i].SetMarkerStyle(21)
      py[i].SetMarkerSize(1)
      py[i].SetMarkerColor(ROOT.kRed)
      ly.AddEntry(py[i], "DY #slash{E}_{y}")
    if Sample == "WJetsToLNu":
      px[i].SetLineColor(ROOT.kBlue)
      fx[i].SetLineColor(ROOT.kBlue)
      px[i].SetMarkerStyle(21)
      px[i].SetMarkerSize(1)
      px[i].SetMarkerColor(ROOT.kBlue)
      lx.AddEntry(px[i], "WJetsToLNu #slash{E}_{x}")
      py[i].SetLineColor(ROOT.kBlue)
      fy[i].SetLineColor(ROOT.kBlue)
      py[i].SetMarkerStyle(21)
      py[i].SetMarkerSize(1)
      py[i].SetMarkerColor(ROOT.kBlue)
      ly.AddEntry(py[i], "WJetsToLNu #slash{E}_{y}")
    if Sample == "TTJets":
      px[i].SetLineColor(ROOT.kGreen)
      fx[i].SetLineColor(ROOT.kGreen)
      px[i].SetMarkerStyle(21)
      px[i].SetMarkerSize(1)
      px[i].SetMarkerColor(ROOT.kGreen)
      lx.AddEntry(px[i], "TTJets #slash{E}_{x}")
      py[i].SetLineColor(ROOT.kGreen)
      fy[i].SetLineColor(ROOT.kGreen)
      py[i].SetMarkerStyle(21)
      py[i].SetMarkerSize(1)
      py[i].SetMarkerColor(ROOT.kGreen)
      ly.AddEntry(py[i], "TTJets #slash{E}_{y}")
    if Sample == "Run2016CDEFG":
      fx[i].SetLineColor(ROOT.kBlack)
      px[i].SetMarkerStyle(21)
      px[i].SetMarkerSize(1)
      lx.AddEntry(px[i], "Run2016CDEFG #slash{E}_{x}")
      fy[i].SetLineColor(ROOT.kBlack)
      py[i].SetMarkerStyle(21)
      py[i].SetMarkerSize(1)
      ly.AddEntry(py[i], "Run2016CDEFG #slash{E}_{y}")
    cx.cd()
    px[i].Draw('esame')
    fx[i].Draw('same')
    cy.cd()
    py[i].Draw('esame')
    fy[i].Draw('same')
    i+=1
    #fx = f.Get('fx')
    #if not fx:
    #  print "fx doesn't exist"
    #  exit(0)
    #ROOT.gDirectory.cd('PyROOT:/')
    #fx_clone = fx.Clone()
    #fx.Draw("same")
    #f.Close()
  cx.cd()
  lx.SetFillColor(0)
  lx.SetShadowColor(ROOT.kWhite)
  lx.SetBorderSize(1)
  lx.Draw()
  cx.Print("plotAll/"+map+"_"+options.mode+"_Px"+".pdf")
  cx.Print("plotAll/"+map+"_"+options.mode+"_Px"+".png")
  cy.cd()
  ly.SetFillColor(0)
  ly.SetShadowColor(ROOT.kWhite)
  ly.SetBorderSize(1)
  ly.Draw()
  cy.Print("plotAll/"+map+"_"+options.mode+"_Py"+".pdf")
  cy.Print("plotAll/"+map+"_"+options.mode+"_Py"+".png")



