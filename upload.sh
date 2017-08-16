#!/bin/bash -x

scp -r _build/html jsommers@cs.colgate.edu:~/public_html/cbook
scp _build/xetex/cbook301.pdf jsommers@cs.colgate.edu:~/public_html/cbook
scp index.html jsommers@cs.colgate.edu:~/public_html/cbook
