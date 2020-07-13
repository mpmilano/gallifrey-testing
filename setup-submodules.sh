#!/bin/bash

(cd submodules/mutils; mkdir build; mkdir install; (cd build; cmake ..; make -j3; make install DESTDIR=../install) )
