
#### ScatterMyRelVals 
  Tools for scatter plots of two different RelVals  
  
###### Workflow
1. Make pickle file that contains all the information on the currently available relvals. Run `python findMyRelvalFilesFromEOS.py` on lxplus. It takes two more arguments, `--pattern='CMSSW_X'` defines a pattern that is searched
for in the release directory name. The default is `--pattern='CMSSW_7'`, i.e. the full 7X series. The second argument `--outputFile='relValFiles_73X.pkl'` specifies the output file. The file stores a python dict with structure `{'releaseTag':[list of fileNames], ...}` and can therefore easily be created by hand.
2. Write the event information. Run `python writeAllEventInformation.py` to write the zpkl files (zipped pickles) that have the event information. Arguments are the input pickle from the previous step, i.e. `--input=='relValFiles_73X.pkl', the path to a temporary directory `--tempdir='some/path'` and a pattern that is searched for in the keys of the relval dictionary, that is, `--patter='RECO'` would find allrelvals with 'RECO' in their releaseTag. The available release tags can be found by using an empty pattern and the `--p` option that only pretends to write the files.
3. Scatter!
