
#### Met phi corrections
  Tools for the derivation of MET phi corrections. The actual corrections are in cms-met/cmssw
  
###### How to derive MET phi corrections
1. (Skip this step if the binning in PF Candidate species and eta stays unchanged) Met-phi corrections are constructed separately in bins of PF Candidate species and eta, roughly corresponding to the subdetectors. These Categories are defined in `MetTools/MetPhiCorrections/python/tools/categories.py`  
Once the species are defined (no/only small changes should be necessary in categories.py), the following command creates a cfg that will be used for obtaining the MEx,y profiles which later are parametrized.   
`python MetTools/MetPhiCorrections/python/tools/writePythonCFG.py --postfix 76X`  
 This will create a file `phiCorrBins_76X_cfi.py`. Change the postfix to something that identifies your usecase.  
 Move this file to `MetTools/MetPhiCorrections/python` and use the following comment with the same postfix as before `python MetTools/MetPhiCorrections/python/write_cff.py --postfix 76X` to create a `_cff.py` that imports the cfi file.
2. Use the `_cff.py` file from the previous step to create the MEx,y profiles. If the previous steps were skipped, use `MetTools/MetPhiCorrections/python/phiCorr_76X_cff.py`  
Edit `MetTools/MetPhiCorrections/test/run_metPhiCorr.py` to make sure your `_cff.py` from the previous step is used. As a test, issue 
`cmsRun run_metPhiCorr.py`  
The output file(s) contain the histograms and MEx,y profiles. When the test job ran satisfactorily, check the output root file and if it looks good, run the cfg on crab on the data you want to produce the corrections for. Note: Output files are small and do not depend on the size of the dataset. Do a `hadd` of all output files.
3. Once you have the MEx,y go to `MetTools/MetPhiCorrections/python/tools/`. Look at the input parameters of `multiplicityFit.py` and change them in the calling script `fits.sh` according to your needs. The command `python multiplicityFit.py -h` should give you an idea.  The script `fits.sh` performs the fits and writes the final cfi file. You can give it three parameters(a label for the production, input root file path, output plot directory path), e.g. `./fits.sh my76X <input.root> plots` which then creates `multPhiCorr_my76X_cfi.py` from the DY shifts that are stored in a root file in that directory. Check the parametrization in the plot and modify funtional form, fitrange etc., if needed. When you're done check the output `.py` file (e.g. the one from the repository, `multPhiCorr_76X_cfi.py`, or the one you produced). Check that there is no overlap in eta for the same candidate species and that all categories are included.(One can also use runfits.sh to run fits.sh multiple times subsequently) 
4. JetMETCorrections:

   `cd $CMSSW_BASE/src`

   echo "/JetMETCorrections/Type1MET/" >> .git/info/sparse-checkout

   git read-tree -mu HEAD

   scram b -j9

5. Finally, move the file (`multPhiCorr_76X_cfi.py`) to the `JetMETCorrections/Type1MET` module, change the module name in `pfMETmultShiftCorrections_cfi.py`.
6. Apply the corrections: cmsRun JetMETCorrections/Type1MET/test/testMultShiftMETcorrInputProducer.py 
