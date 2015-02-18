# MetTools

Install: Get your favourite 72X CMSSW and then just do

`git clone https://github.com/cms-met/MetTools.git`

in $CMSSW_BASE/src

#### Met performance
  Tools for making the MET performance plots.
  (Laras description)
  
#### Met phi corrections
  Tools for the derivation of MET phi corrections, the actual corrections go to cms-met/cmssw
  
###### How to derive multiplicity based MET phi corrections
1. (should usually be skipped) Categories are defined in MetTools/MetPhiCorrections/python/tools/categories.py
Use this file to change eta categories in pfCandidate species. Then, create a cfg issuing
`python MetTools/MetPhiCorrections/python/tools/writePythonCFG.py --postfix PHYS14`
This will give you a file `multPhi_PHYS14_cfi.py`. Change the postfix to your usecase.
2. Use this file to create the correction histograms
######## Create cfg
`git clone https://github.com/cms-met/MetTools.git`
