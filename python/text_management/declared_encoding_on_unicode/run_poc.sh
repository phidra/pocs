#!/bin/bash

for i in declared_encoding_on_unicode*.py
do
    echo ""
    echo "RUNNING =   $i"
    ./$i
done
