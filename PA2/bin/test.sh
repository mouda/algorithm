#!/bin/bash

inputDir="../inputs";
outputDir="../outputs";
inputFileNames="$(ls $inputDir)";
option="RC"

for name in $inputFileNames
do
  subString=${name%.in};
  inputArgument=${inputDir}'/'$subString".in"
  outputArgument=${outputDir}'/'${subString}".out"

  if [[ -f  $outputArgument  ]]; then
    rm $outputArgument 
  fi

  echo "*******************  $option  ********************";
  echo "";
  echo "The input file:  $inputArgument";
  echo "The output file: $outputArgument";
  ./01knapsack '-'$option $inputArgument $outputArgument
  cat $outputArgument;
done
