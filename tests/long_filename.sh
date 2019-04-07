#!/bin/zsh

size=50
PORT=8080
opwd=$(pwd)

mkdir -p /tmp/$(repeat $size echo -n cecinestpasuntest/)
cd /tmp/$(repeat $size echo -n cecinestpasuntest/)
echo test > test.txt
echo http://localhost:$PORT/tmp/$(repeat $size echo -n cecinestpasuntest/)test.txt > /tmp/test_url
cd $opwd/..
#./mbhttpd -p $PORT &
wget -O - -i /tmp/test_url
rm -rf /tmp/cecinestpasuntest/
rm -f /tmp/test_url
killall mbhttpd


