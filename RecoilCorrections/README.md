#Instructions how to produce and apply recoil correction functions:

##Producing recoil corrections
fitRecoil.C produces the recoil correction functions and graphs. This is set up to read from a flat ntuple (see line ~250), where the recoil components (u1 and u2) have already been calculated and stored as a branch in the ntuple. You also need the boson information, e.g. dilepton pT for Z (data or MC) or gen-level boson pT for W MC. You will need to update this part of the code yourself to match your ntuple. You can also change the pT bins by editing the ptbins array.

It can be run using runFits.sh, and produces a ROOT file containing the corrections.

Note: The original version of this code has some external dependencies which are needed to draw and save the plots of all quantities with the fitted functions on them. At the moment, everything with the external dependence is commented out, so it should compile and run and produce the root file with the TGraphs and fitted correction functions. There just won't be any plots printed, which are helpful to check the quality of the fits. 

##Applying recoil corrections

RecoilCorrector.hh contains correction functions for both Type 0 and Type 2 recoil corrections. Here are some instructions on how to load the files containing the corrections and then apply the corrections. 

###Type 0 Corrections
Create a new RecoilCorrector object, and intitialize it with the correction functions from data. 
```
RecoilCorrector *recoilCorr = new RecoilCorrector("my_filename.root");
```
The corrections are applied per event:
```
recoilCorr->CorrectType0(corrMet,corrMetPhi,genVPt,genVPhi,dilepton.Pt(),dilepton.Phi(),pU1,pU2,nSigMean,nSigWidths);
```
genVPt and genVPhi are generator-level boson quantities.
dilepton.Pt() and dilepton.Phi() are reconstructed boson quantities in MC
The MET after recoil corrections are applied is stored in corrMet and corrMetPhi, and recoil is stored in pU1 and pU2. 
The last two arguments are to shift the corrections up and down by a specified number sigma of the fits, which can be used to evaluate uncertainty. To get the correction, these should be set to zero, and for uncertainty +1 or -1.


###Type 2 Corrections

Type 2 corrections require corrections derived from both data and MC (data and MC recoil for Z, and MC recoil for whatever other process you are looking at).
You need to add the correction files:
```
RecoilCorrector *recoilCorr = new  RecoilCorrector("my_corrections_from_MC.root","fcnPF");
recoilCorr->addMCFile("my_corrections_from_MC.root");
recoilCorr->addDataFile("my_corrections_from_data.root");
```

The corrections are applied in the same way as the Type 0 corrections. 
```
recoilCorr->CorrectType2(corrMet,corrMetPhi,genVPt,genVPhi,dilepton.Pt(),dilepton.Phi(),pU1,pU2,nSigMean,nSigWidths);
```