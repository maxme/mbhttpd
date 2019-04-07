#!/bin/sh

PORT=6543

cd ..
./mbhttpd -p $PORT &
cd -
find $(pwd) -exec wget -O - http://localhost:$PORT{} > /dev/null 2>&1 \;
killall mbhttpd