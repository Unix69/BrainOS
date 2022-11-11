#!/bin/bash

git init
git add *.c *.h *.S Makefile manqemu README.md link.ld
git commit -m "first commit"
git remote add origin "https://github.com/Unix69/BrainOS.git"
git push -u origin master:master 