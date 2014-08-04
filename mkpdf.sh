#!/bin/bash 
sphinx-build -b latex -d _build/doctrees . _build/xetex && pushd _build/xetex; xelatex cbook301 && xelatex cbook301 && popd
