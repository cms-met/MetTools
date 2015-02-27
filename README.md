# MetTools

Install: Get your favourite 72X CMSSW and then just do

`git clone https://github.com/cms-met/MetTools.git`

in $CMSSW_BASE/src

#### Met performance
  Tools for making the MET performance plots.
  (Laras description)
  
#### Met phi corrections
  Tools for the derivation of MET phi corrections. The actual corrections are in cms-met/cmssw
  
###### How to derive MET phi corrections
1. (Skip this step if the binning in PF Candidate species and eta stays unchanged) Met-phi corrections are constructed separately in bins of PF Candidate species and eta, roughly corresponding to the subdetectors. These Categories are defined in `MetTools/MetPhiCorrections/python/tools/categories.py`  
Once the species are defined (no/only small changes should be necessary in categories.py), the following command creates a cfg that will be used for obtaining the MEx,y profiles which later are parametrized.   
`python MetTools/MetPhiCorrections/python/tools/writePythonCFG.py --postfix PHYS14`  
 This will create a file `phiCorr_PHYS14_cfi.py`. Change the postfix to something that identifies your usecase.  
 Move this file to `MetTools/MetPhiCorrections/python` and create a `_cff.py` that imports the cfi file. Start from the template `MetTools/MetPhiCorrections/python/phiCorr_PHYS14_cff.py`
2. Use the `_cff.py` file from the previous step to create the MEx,y profiles. If the previous steps were skipped, use `MetTools/MetPhiCorrections/python/phiCorr_PHYS14_cff.py`  
Edit `MetTools/MetPhiCorrections/test/run_metPhiCorr.py` to make sure your `_cff.py` from the previous step is used. As a test, issue 
`cmsRun run_metPhiCorr.py`  
The output file(s) contain the histograms and MEx,y profiles. When the test job ran satisfactorily, check the output root file and if it looks good, run the cfg on crab on the data you want the produce the corrections for. Note: Output files are small and do not depend on the size of the dataset. Do a `hadd` of all output files.
3. Once you have the MEx,y go to `MetTools/MetPhiCorrections/python/tools/`. Look at the input parameters of `multiplicityFit.py` and change them in the calling script `fits.sh` according to your need. The command `python multiplicityFit.py -h` should give you an idea.  The script `fits.sh` performs the fits and writes the final cfi file. You can give it a prefix parameter, e.g. `./fits.sh myphys14` which then creates `multPhiCorr_myphys14_cfi.py` from the DY shifts that are stored in a root file in that directory. Check the parametrization in the plot and modify funtional form, fitrange etc., if needed. When you're done check the output `.py` file (.e.g. the one from the repository, `multPhiCorr_phys14_cfi.py`, or the one you produced) looks fine. Check that there is no overlap in eta for the same candidate species and that all categories are included. `
