#!/bin/bash -x

scp -r _build/html jsommers@cs.colgate.edu:~/public_html/cosc301/cbook
scp _build/xetex/cbook301.pdf jsommers@cs.colgate.edu:~/public_html/cosc301/cbook
