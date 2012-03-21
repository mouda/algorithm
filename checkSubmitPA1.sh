#!/bin/sh

############################################################################# 
# File       [checkSubmitPA1.sh]
# Author     [jjhwang]
# Synopsis   [simple test for algorithm P13 homework format program ]
# Date       [Ver. 1.0 started 2012/03/03]
############################################################################# 


if [ $1 == ""]; then
    echo "Usage:   ./checkSubmitPA1.sh <fileName>"
    echo "Example: ./checkSubmitPA1.sh b98901000-p1.tgz"
    echo "This program will build a dir name checkSubmitPA1_$$ env"
    exit
fi

binaryFileName='mysort'
fileName=$1

echo $fileName
if [ ! -f $fileName ]; then
    echo "Can not find tgz file"
fi
if [ -d checkSubmitPA1_$$ ]; then
    echo "There exist heckSubmitPA1_$$ dir"
    echo "can not bulid test enviroment"
    echo "Please try again"
    exit
fi

mkdir checkSubmitPA1_$$ 
cd checkSubmitPA1_$$

tar zxvf ../$fileName

cc=`echo $fileName | sed -e 's/-.*//'`

if [ ! -d ${cc}-p1 ]; then
    echo "Can not find ${cc}-p1 dir"
    exit
fi

cd ${cc}-p1

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
rm checkSubmitPA1_$$ -rf
echo "Congratulations!"

