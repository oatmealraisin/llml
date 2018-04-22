#!/usr/bin/bash

url=http://yann.lecun.com/exdb/mnist/
for i in train-images-idx3-ubyte train-labels-idx1-ubyte t10k-images-idx3-ubyte t10k-labels-idx1-ubyte; do
	wget $url$i.gz
	gunzip --decompress $i.gz
done
