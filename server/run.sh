#!/bin/bash
cd `dirname $0`
eval $(ps -ef | grep "[p]ython3 server.py" | awk '{print "kill "$2}')
nohup python3 server.py >> xk.log 2>&1 &
