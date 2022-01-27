#!/bin/bash

StringVal='
WJetsToLNu_HT-100To200_2018_1.xml
WJetsToLNu_HT-1200To2500_2018_1.xml
WJetsToLNu_HT-200To400_2018_1.xml
WJetsToLNu_HT-2500ToInf_2018_1.xml
WJetsToLNu_HT-400To600_2018_1.xml
WJetsToLNu_HT-600To800_2018_1.xml
WJetsToLNu_HT-70To100_2018_1.xml
WJetsToLNu_HT-800To1200_2018_1.xml
'
for val in $StringVal;



do
    sframe_main ${val}
done


