
if [[ -f  ../outputs/1000.ac.out  ]]; then
  rm ../outputs/1000.ac.out 
fi
./mysort -HS ../inputs/1000.ac.in ../outputs/1000.ac.out 
#cat ../outputs/1000.ac.out
