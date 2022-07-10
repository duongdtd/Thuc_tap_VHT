#!/bin/sh

echo "1000000">freq.txt
sleep 5
echo "100000">freq.txt
sleep 5
echo "10000">freq.txt
sleep 5 
echo "1000">freq.txt
sleep 5
echo "100">freq.txt
sleep 5 && kill -9 `ps ax | grep ./b1 | grep -v grep| awk '{print $1}'`