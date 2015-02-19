# MetTools

Install: Get your favourite 72X CMSSW and then just do

`git clone https://github.com/cms-met/MetTools.git`

in $CMSSW_BASE/src

#### Met performance
  Tools for making the MET performance plots.
  (Laras description)
  
#### Met phi corrections
  Tools for the derivation of MET phi corrections, the actual corrections go to cms-met/cmssw
  
###### How to derive MET phi corrections
1. (Skip this step if the binning in PF Candidate species and eta stays unchanged) Met-phi corrections are constructed separately in bins of PF Candidate species and eta, roughly corresponding to the subdetectors. These Categories are defined in `MetTools/MetPhiCorrections/python/tools/categories.py`  
Once the species are defined (no/only small changes should be necessary in categories.py), the following command creates a cfg that will be used for obtaining the MEx,y profiles which later are parametrized.   
`python MetTools/MetPhiCorrections/python/tools/writePythonCFG.py --postfix PHYS14`  
 This will create a file `phiCorr_PHYS14_cfi.py`. Change the postfix to your usecase.  
 Move this file to `MetTools/MetPhiCorrections/python` and create a `_cff.py` that imports it starting from `MetTools/MetPhiCorrections/python/phiCorr_PHYS14_cff.py`
2. Use the two files from the previous step to create the MEx,y profiles. Change `MetTools/MetPhiCorrections/test/run_metPhiCorr.py` so that the `_cff.py` from the previous step is used, then issue  
`cmsRun run_metPhiCorr.py`
