#!/bin/sh

############################################################################# 
# File       [checkSubmitPA3.sh]
# Author     [jjhwang]
# Synopsis   [simple test for algorithm PA3 homework format program ]
# Date       [Ver. 1.0 started 2012/05/04]
############################################################################# 


if [ -z $1 ]; then
    echo "Usage:   ./checkSubmitPA3.sh <fileName>"
    echo "Example: ./checkSubmitPA3.sh b98901000-p1.tgz"
    echo "This program will build a dir name checkSubmitPA3_$$ env"
    exit
fi

binaryFileName='graphlab'
fileName=$1

echo $fileName
if [ ! -f $fileName ]; then
    echo "Can not find tgz file"
fi
if [ -d checkSubmitPA3_$$ ]; then
    echo "There exist checkSubmitPA3_$$ dir"
    echo "can not bulid test environment"
    echo "Please try again"
    exit
fi

mkdir checkSubmitPA3_$$ 
cd checkSubmitPA3_$$

tar zxvf ../$fileName

cc=`echo $fileName | sed -e 's/-.*//'`

if [ ! -d ${cc}-pa3 ]; then
    echo "Can not find ${cc}-pa3 dir"
    exit
fi

cd ${cc}-pa3

if [ -d inputs ]; then
    echo "Please delete inputs dir"
    exit
fi

if [ -d outputs ]; then
    echo "Please delete outputs dir"
    exit
fi

if [ ! -d src ]; then
    echo "Can not find src dir"
    exit
fi

make clean

if [ -f bin/${binaryFileName} ]; then
    echo "Make clean should remove bin/$binaryFileName"
    exit
fi

make

if [ ! -f bin/${binaryFileName} ]; then
    echo "Con not find bin/$binaryFileName"
    exit
fi

cd ../..
rm checkSubmitPA3_$$ -rf
echo "Congratulations!"

