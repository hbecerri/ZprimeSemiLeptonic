#!/bin/bash
#where UHH2 code installed
pathGL_code=/nfs/dust/cms/user/hugobg/UHH2_v2/CMSSW_10_2_17/src/UHH2/
#where (NOT MERGED) trees after preselection stored
path_data=/nfs/dust/cms/group/zprime-uhh/Presel_2016/workdir_Zprime_PreSelec_2016v3_CHS_TimePtEta_up/ 

mkdir $pathGL_code/ZprimeSemiLeptonic/data/Skimming_datasets_2016v3_CHS_common_JECup_Timepteta
cd $pathGL_code/ZprimeSemiLeptonic/data/Skimming_datasets_2016v3_CHS_common_JECup_Timepteta


 #MC
for sample_name in TTToSemiLeptonic_2016v3

do
    echo $sample_name

       $pathGL_code/scripts/create-dataset-xmlfile ${path_data}"uhh2.AnalysisModuleRunner.MC."${sample_name}"*.root" MC_$sample_name.xml
       python $pathGL_code/scripts/crab/readaMCatNloEntries.py 10 MC_$sample_name.xml True
done

pwd
cd $pathGL_code/ZprimeSemiLeptonic/macros
