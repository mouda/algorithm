#!/bin/bash
index=1;
x=1;
outDirName="../outputs";
inDirName="../inputs";

for option in '-IS' '-MS' '-HS'
do 
  #echo $option 

  for index in $(seq 1 6)
  do

  number=`expr 1000 \* $x`;
  #echo $number;

    for suffix in  '.bc' '.wc' '.ac'
    do
      inputName=${inDirName}'/'${number}${suffix}'.in'; 
      outputName=${outDirName}'/'${number}${suffix}'.out'; 
    #echo $inputName
      if [[ -f  $outputName  ]]; then
        rm $outputName 
      fi
      ./mysort $option  $inputName $outputName
    done

  x=`expr $x \* 2`;
  done
  x=1;

done
