#!/bin/bash

for option in  '-IS' 
do
  if [[ -f  ../outputs/5.ac.outTest   ]]; then
    rm ../outputs/5.ac.outTest  
  fi
  ./mysort $option ../inputs/5.ac.in ../outputs/5.ac.outTest 
done
diff ../outputs/5.ac.out ../outputs/5.ac.outTest
