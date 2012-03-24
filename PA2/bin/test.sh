#!/bin/bash

inputDir="../inputs";
outputDir="../outputs";
inputFileNames="$(ls $inputDir)";

for name in $inputFileNames
do
  subString=${name%.in};
  inputArgument=${inputDir}'/'$subString".in"
  outputArgument=${outputDir}'/'${subString}".out"

  if [[ -f  $outputArgument  ]]; then
    rm $outputArgument 
  fi

  ./01kanpsack -GD $inputArgument $outputArgument
  #echo $inputArgument;
  #echo $outputArgument;
done
