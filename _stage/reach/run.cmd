@echo off
rem run.cmd -- runs test.exe on cases.txt (written by expected.py), writes actual.txt
pushd "%~dp0"
set CB_CHECKOUT_ROOT=C:\Users\mrtit\Documents\GitHub\CodeBreaker
set CB_CORPUS_ROOT=C:\Users\mrtit\Documents\GitHub\Windows95\design\corpus\files
"%~dp0test.exe" cases.txt > actual.txt
popd
