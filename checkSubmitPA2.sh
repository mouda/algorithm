#!/bin/sh

############################################################################# 
# File       [checkSubmitPA2.sh]
# Author     [jjhwang]
# Synopsis   [simple test for algorithm PA2 homework format program ]
# Date       [Ver. 1.0 started 2012/03/30]
############################################################################# 


if [ -z $1 ]; then
    echo "Usage:   ./checkSubmitPA2.sh <fileName>"
    echo "Example: ./checkSubmitPA2.sh b98901000-p1.tgz"
    echo "This program will build a dir name checkSubmitPA2_$$ env"
    exit
fi

binaryFileName='01knapsack'
fileName=$1

echo $fileName
if [ ! -f $fileName ]; then
    echo "Can not find tgz file"
fi
if [ -d checkSubmitPA2_$$ ]; then
    echo "There exist checkSubmitPA2_$$ dir"
    echo "can not bulid test environment"
    echo "Please try again"
    exit
fi

mkdir checkSubmitPA2_$$ 
cd checkSubmitPA2_$$

tar zxvf ../$fileName

cc=`echo $fileName | sed -e 's/-.*//'`

if [ ! -d ${cc}-pa2 ]; then
    echo "Can not find ${cc}-pa2 dir"
    exit
fi

cd ${cc}-pa2

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
rm checkSubmitPA2_$$ -rf
echo "Congratulations!"

