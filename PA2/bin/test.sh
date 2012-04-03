#!/bin/bash

inputDir="../inputs";
outputDir="../outputs";
inputFileNames="$(ls $inputDir)";
option="GD"

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
  ./01kanpsack '-'$option $inputArgument $outputArgument
  cat $outputArgument;
done
