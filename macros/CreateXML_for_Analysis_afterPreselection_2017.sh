#!/bin/bash
#where UHH2 code installed
pathGL_code=/nfs/dust/cms/user/deleokse/analysis/CMSSW_10_2_10/src/UHH2/
#where (NOT MERGED) trees after preselection stored
path_data=/nfs/dust/cms/user/deleokse/analysis/ZPrime_2017/Preselection_QCD_HT/Output_2017/workdir_Zprime_PreSelec_2017/uhh2.AnalysisModuleRunner. 

mkdir $pathGL_code/ZprimeSemiLeptonic/data/Skimming_datasets_2017
cd $pathGL_code/ZprimeSemiLeptonic/data/Skimming_datasets_2017


# #MC

#for sample_name in TTToSemiLeptonic_2017 TTToHadronic_2017 TTTo2L2Nu_2017 WW_2017 WZ_2017 ZZ_2017 ST_t-channel_antitop_2017 ST_t-channel_top_2017 ST_tW_antitop_5f_inclusiveDecays_2017 ST_tW_top_5f_inclusiveDecays_2017 QCD_HT100to200_2017 QCD_HT200to300_2017 QCD_HT300to500_2017 QCD_HT500to700_2017 QCD_HT700to1000_2017 QCD_HT1000to1500_2017 QCD_HT1500to2000_2017 QCD_HT2000toInf_2017 WJetsToLNu_HT-70To100_2017 WJetsToLNu_HT-100To200_2017 WJetsToLNu_HT-200To400_2017 WJetsToLNu_HT-400To600_2017 WJetsToLNu_HT-600To800_2017 WJetsToLNu_HT-800To1200_2017 WJetsToLNu_HT-1200To2500_2017 WJetsToLNu_HT-2500ToInf_2017 ZprimeToTTJet_M1000_2017  

for sample_name in DYJetsToLL_M-50_HT-100to200_2017 DYJetsToLL_M-50_HT-1200to2500_2017 DYJetsToLL_M-50_HT-200to400_2017 DYJetsToLL_M-50_HT-2500toInf_2017 DYJetsToLL_M-50_HT-400to600_2017 DYJetsToLL_M-50_HT-600to800_2017 DYJetsToLL_M-50_HT-800to1200_2017  

do
    echo $sample_name

       $pathGL_code/scripts/create-dataset-xmlfile ${path_data}"MC."${sample_name}"*.root" MC_$sample_name.xml
       python $pathGL_code/scripts/crab/readaMCatNloEntries.py 10 MC_$sample_name.xml True
done

## # #DATA
#for sample_name in DATA_SingleMuon_Run2017C DATA_SingleMuon_Run2017D DATA_SingleMuon_Run2017E DATA_SingleMuon_Run2017F DATA_SingleElectron_Run2017C DATA_SingleElectron_Run2017D DATA_SingleElectron_Run2017E DATA_SingleElectron_Run2017F 
#
#do
#    echo $sample_name 
#    $pathGL_code/scripts/create-dataset-xmlfile ${path_data}"DATA."${sample_name}"*.root" DATA_$sample_name.xml
#    python $pathGL_code/scripts/crab/readaMCatNloEntries.py 10 DATA_$sample_name.xml True
#
#done
pwd
cd $pathGL_code/ZprimeSemiLeptonic/macros