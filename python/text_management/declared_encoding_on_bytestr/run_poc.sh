#!/bin/bash

for i in declared_encoding_on_bytestring*.py
do
    echo ""
    echo "RUNNING =   $i"
    ./$i
done
