#!/bin/bash
#where UHH2 code installed
pathGL_code=/nfs/dust/cms/user/hugobg/UHH2_v2/CMSSW_10_2_17/src/UHH2/
#where (NOT MERGED) trees after preselection stored
path_data=/nfs/dust/cms/group/zprime-uhh/Presel_2017/workdir_Zprime_PreSelec_2017_TimePtEta_down/ 

mkdir $pathGL_code/ZprimeSemiLeptonic/data/Skimming_datasets_2017_CHS_common_JECdown_Timepteta
cd $pathGL_code/ZprimeSemiLeptonic/data/Skimming_datasets_2017_CHS_common_JECdown_Timepteta


 #MC
for sample_name in TTToSemiLeptonic_2017v2

do
    echo $sample_name

       $pathGL_code/scripts/create-dataset-xmlfile ${path_data}"uhh2.AnalysisModuleRunner.MC."${sample_name}"*.root" MC_$sample_name.xml
       python $pathGL_code/scripts/crab/readaMCatNloEntries.py 10 MC_$sample_name.xml True
done

pwd
cd $pathGL_code/ZprimeSemiLeptonic/macros
