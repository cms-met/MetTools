#!/bin/sh

mainOutput="multPhiCorr_combined_ngoodVertices_cfi.py"

rm multPhiCorr_combined_ngoodVertices_cfi.py
echo "import FWCore.ParameterSet.Config as cms">>$mainOutput
echo "multPhiCorr_combined = cms.VPSet(">>$mainOutput

./fits.sh MC_2018 "/user/npostiau/CMSSW_10_2_11_MetTools/src/MetTools/MetPhiCorrections/test/fullProd/histo_merged_MC2018.root" "/user/npostiau/public_html/MetPhi/MC_2018/" ngoodVertices --combine
./fits.sh MC_2017 "/user/npostiau/CMSSW_10_2_11_MetTools/src/MetTools/MetPhiCorrections/test/fullProd/histo_merged_MC2017.root" "/user/npostiau/public_html/MetPhi/MC_2017/" ngoodVertices --combine
./fits.sh MC_2016 "/user/npostiau/CMSSW_10_2_11_MetTools/src/MetTools/MetPhiCorrections/test/fullProd/histo_merged_MC2016.root" "/user/npostiau/public_html/MetPhi/MC_2016/" ngoodVertices --combine
./fits.sh Data_2018A "/user/npostiau/CMSSW_10_2_11_MetTools/src/MetTools/MetPhiCorrections/test/fullProd/histo_merged_DataDoubleMu2018A.root" "/user/npostiau/public_html/MetPhi/Data2018A/" ngoodVertices --combine
./fits.sh Data_2018B "/user/npostiau/CMSSW_10_2_11_MetTools/src/MetTools/MetPhiCorrections/test/fullProd/histo_merged_DataDoubleMu2018B.root" "/user/npostiau/public_html/MetPhi/Data2018B/" ngoodVertices --combine
./fits.sh Data_2018C "/user/npostiau/CMSSW_10_2_11_MetTools/src/MetTools/MetPhiCorrections/test/fullProd/histo_merged_DataDoubleMu2018C.root" "/user/npostiau/public_html/MetPhi/Data2018C/" ngoodVertices --combine
./fits.sh Data_2018D "/user/npostiau/CMSSW_10_2_11_MetTools/src/MetTools/MetPhiCorrections/test/fullProd/histo_merged_DataDoubleMu2018D.root" "/user/npostiau/public_html/MetPhi/Data2018D/" ngoodVertices --combine
./fits.sh Data_2017B "/user/npostiau/CMSSW_10_2_11_MetTools/src/MetTools/MetPhiCorrections/test/fullProd/histo_merged_DataDoubleMu2017B.root" "/user/npostiau/public_html/MetPhi/Data2017B/" ngoodVertices --combine
./fits.sh Data_2017C "/user/npostiau/CMSSW_10_2_11_MetTools/src/MetTools/MetPhiCorrections/test/fullProd/histo_merged_DataDoubleMu2017C.root" "/user/npostiau/public_html/MetPhi/Data2017C/" ngoodVertices --combine
./fits.sh Data_2017D "/user/npostiau/CMSSW_10_2_11_MetTools/src/MetTools/MetPhiCorrections/test/fullProd/histo_merged_DataDoubleMu2017D.root" "/user/npostiau/public_html/MetPhi/Data2017D/" ngoodVertices --combine
./fits.sh Data_2017E "/user/npostiau/CMSSW_10_2_11_MetTools/src/MetTools/MetPhiCorrections/test/fullProd/histo_merged_DataDoubleMu2017E.root" "/user/npostiau/public_html/MetPhi/Data2017E/" ngoodVertices --combine
./fits.sh Data_2017F "/user/npostiau/CMSSW_10_2_11_MetTools/src/MetTools/MetPhiCorrections/test/fullProd/histo_merged_DataDoubleMu2017F.root" "/user/npostiau/public_html/MetPhi/Data2017F/" ngoodVertices --combine
./fits.sh Data_2016B "/user/npostiau/CMSSW_10_2_11_MetTools/src/MetTools/MetPhiCorrections/test/fullProd/histo_merged_DataDoubleMu2016B.root" "/user/npostiau/public_html/MetPhi/Data2016B/" ngoodVertices --combine
./fits.sh Data_2016C "/user/npostiau/CMSSW_10_2_11_MetTools/src/MetTools/MetPhiCorrections/test/fullProd/histo_merged_DataDoubleMu2016C.root" "/user/npostiau/public_html/MetPhi/Data2016C/" ngoodVertices --combine
./fits.sh Data_2016D "/user/npostiau/CMSSW_10_2_11_MetTools/src/MetTools/MetPhiCorrections/test/fullProd/histo_merged_DataDoubleMu2016D.root" "/user/npostiau/public_html/MetPhi/Data2016D/" ngoodVertices --combine
./fits.sh Data_2016E "/user/npostiau/CMSSW_10_2_11_MetTools/src/MetTools/MetPhiCorrections/test/fullProd/histo_merged_DataDoubleMu2016E.root" "/user/npostiau/public_html/MetPhi/Data2016E/" ngoodVertices --combine
./fits.sh Data_2016F "/user/npostiau/CMSSW_10_2_11_MetTools/src/MetTools/MetPhiCorrections/test/fullProd/histo_merged_DataDoubleMu2016F.root" "/user/npostiau/public_html/MetPhi/Data2016F/" ngoodVertices --combine
./fits.sh Data_2016G "/user/npostiau/CMSSW_10_2_11_MetTools/src/MetTools/MetPhiCorrections/test/fullProd/histo_merged_DataDoubleMu2016G.root" "/user/npostiau/public_html/MetPhi/Data2016G/" ngoodVertices --combine
./fits.sh Data_2016H "/user/npostiau/CMSSW_10_2_11_MetTools/src/MetTools/MetPhiCorrections/test/fullProd/histo_merged_DataDoubleMu2016H.root" "/user/npostiau/public_html/MetPhi/Data2016H/" ngoodVertices --combine

echo ")">>$mainOutput
