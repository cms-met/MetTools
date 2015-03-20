
#### ScatterMyRelVals 
  Tools for scatter plots of two different RelVals  
  
###### Workflow
1. Make pickle file that contains all the information on the currently available relvals: Run `python findMyRelvalFilesFromEOS.py` on lxplus. It takes two more arguments, `--pattern='CMSSW_X'` defines a pattern that is searched for in the release directory name. The default is `--pattern='CMSSW_7'`, i.e. the full 7X series. The second argument `--outputFile='relValFiles_73X.pkl'` specifies the output file. The file stores a python dict with structure `{'releaseTag':[/eos/.../filename.root, ...], ...}` and can also be created by hand.
2. Write the event information: Run `python writeAllEventInformation.py` to write the zpkl files (zipped pickles) that store the event information. Inside, a temporary release area is created and another script `writeEventInformation.py` is called which does the actual event loop (I couldn't get fwlite working on RECO files from different releases). Arguments for `writeAllEventInformation`: The input pickle from the previous step (`--input=='relValFiles_73X.pkl'`), the path to a temporary directory (`--tempdir='some/path'`) and a pattern that is searched for in the keys of the relval dictionary (`--pattern='RECO'` would find all RelVals with 'RECO' in their releaseTag). The available release tags can be found by using an empty pattern and the `--p` option. This latter options only pretends to write the event information.
3. Scatter!
