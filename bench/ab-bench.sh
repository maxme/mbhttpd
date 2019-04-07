#!/bin/zsh

PORT=5646

cd ..
./mbhttpd -p $PORT > /dev/null &

# ascii
ab -n 10000 -c 1000 http://localhost:$PORT/etc/passwd
ab -n 10000 -c 100 http://localhost:$PORT/etc/passwd
ab -n 10000 -c 10 http://localhost:$PORT/etc/passwd
ab -n 10000 -c 10 http://localhost:$PORT/etc/passwd

# binary
ab -n 10000 -c 10 http://localhost:$PORT/var/log/kern.log.1.gz

# pdf empty file
touch /tmp/httpd-test.pdf
ab -n 10000 -c 100 http://localhost:$PORT/tmp/httpd-test.pdf

# big file
rm -f /tmp/httpd-test
repeat 1000 cat /var/log/kern.log.1.gz >> /tmp/httpd-test
ab -n 100 -c 10 http://localhost:$PORT/tmp/httpd-test
rm -f /tmp/httpd-test

killall mbhttpd
