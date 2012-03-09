#!/bin/bash
index=1;
x=1;
outDirName="../outputs";
inDirName="../inputs";

for index in $(seq 1 6)
do

  number=`expr 1000 \* $x`;
  #echo $number;

  for suffix in  '.ac' '.bc' '.wc'
  do 
    inputName=${inDirName}'/'${number}${suffix}'.in'; 
    outputName=${outDirName}'/'${number}${suffix}'.out'; 
    
    if [[ -f  $outputName  ]]; then
      rm $outputName 
    fi

    for option in '-IS' '-MS' '-HS'
    do
      ./mysort $option  $inputName $outputName
    done

  done

  x=`expr $x \* 2`;
done
