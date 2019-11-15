
#### Met phi corrections
  Tools for the derivation of MET phi corrections. The actual corrections are in cms-met/cmssw\

  The full recipe below is supposed to work on `CMSSW_10_2_11`.
  
###### How to derive MET phi corrections
1. (Skip this step if the binning in PF Candidate species and eta stays unchanged) Met-phi corrections are constructed separately in bins of PF Candidate species and eta, roughly corresponding to the subdetectors. These Categories are defined in `MetTools/MetPhiCorrections/python/tools/categories.py`\
Once the species are defined (no/only small changes should be necessary in categories.py), the following command creates a cfg that will be used for obtaining the MEx,y profiles which later are parametrized.   
`python MetTools/MetPhiCorrections/python/tools/writePythonCFG.py --postfix 102X`\
 This will create a file `phiCorrBins_102X_cfi.py`. Change the postfix to something that identifies your usecase.\
 Move this file to `MetTools/MetPhiCorrections/python` and use the following comment with the same postfix as before `python MetTools/MetPhiCorrections/python/write_cff.py --postfix 102X` to create a `_cff.py` that imports the cfi file.
2. Use the `_cff.py` file from the previous step to create the MEx,y profiles. If the previous steps were skipped, use `MetTools/MetPhiCorrections/python/phiCorr_102X_cff.py`.\
Edit `MetTools/MetPhiCorrections/test/run_metPhiCorr.py` to make sure your `_cff.py` from the previous step is used. As a test, issue\
`cmsRun run_metPhiCorr.py`.\
`run_metPhiCorr.py` should be updated manually with the correct global tag for the year on which you want to run (different for data and MC).\
The output file(s) contain the histograms and MEx,y profiles. When the test job ran satisfactorily, check the output root file and if it looks good, you can run the cfg on crab on the data you want to produce the corrections for. You may use the existing crab config files as an example.\
Note: Output files are small and do not depend on the size of the dataset.\
Do a `hadd` of all output files.\
You need to do this separately for each era, and once for MC for each year (no automatization of this exists yet). In principle, different eras could be regrouped, but it has been showed that the corrections depend on the era.\
3. Once you have the MEx,y go to `MetTools/MetPhiCorrections/python/tools/`. Look at the input parameters of `multiplicityFit.py` and change them in the calling script `fits.sh` according to your needs. The command `python multiplicityFit.py -h` should give you an idea. The script `fits.sh` performs the fits and writes the final cfi file. You can give it three parameters(a label for the production, input root file path, output plot directory path), e.g. `./fits.sh my102X <input.root> plots` which then creates `multPhiCorr_my102X_cfi.py` from the DY shifts that are stored in a root file in that directory, as well as plots showing the fit results. Check the parametrization in the plot and modify funtional form, fitrange etc., if needed. When you're done check the output `.py` file (e.g. one of those from the repository, `multPhiCorr_XXX_cfi.py`, or the one you produced). Check that there is no overlap in eta for the same candidate species and that all categories are included. (One can also use `runfits.sh` to run fits.sh multiple times subsequently.)\
There are 3 parametrizations possible for the corrections: as a function of multiplicity, of the (scalar) pT sum or of the number of vertices. The current recommendation is to use the number of vertices, which is found the be the most independant of the hard physics process. Later steps assume this choice is made.\
The file `multPhiCorr_my102X_cfi.py` is the one that is meant to be put in the `JetMETCorrections/Type1MET/python` module of CMSSW (if you do so, don't forget to change the module name in `pfMETmultShiftCorrections_cfi.py`). By doing so, you ensure that your corrections will be applied, based on the fits (which are quadratic functions) you just created. Be aware, however, that the current framework allows only to have one file, while it was shown that the corrections may be very different from a year to another, from data to MC, and even from an era to another within the same year.\
From there, you might want to check the impact of your corrections by applying them, using a simple analyzer.\
4. Once you have run on all the data, and on DY MC, for one given year, you can use the scripts in `MetTools/MetPhiCorrections/test/plotting`. Update the names of the input files to match the ones you obtained. There are 3 scripts you can use:
  - (Optional) `root -l -q -b 'prepare_plots.cc++(YEAR)'` (where YEAR is 2016, 2017 or 2018) -> will produce plots for meaningful variables, superimposed for all eras
  - `root -l -q -b 'combine_corrections.cc++(YEAR)'` -> will produce, for each era, a single combined plot for the total correction in MET X shift and another in MET Y shift, as a function of the number of vertices. It also performs a fit with a linear function (expected behavior is linear), and write the fit parameters in a text file. It is recommended to use this output if you want to apply corrections to your analysis.
  - (Optional) `root -l -q -b 'prepare_combined_plots.cc++(YEAR)'` -> will superimpose the plots produced by `combine_corrections.cc`, and compare them with the fits obtained by the alternative method, consisting in computing the corrections directly from the MET X/Y coordinates in the event. Additional options can be given to plot only several eras (to avoid overcrowded plots).
