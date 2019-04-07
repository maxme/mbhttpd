err=0
port=8081

check_file() {
    wget -O - http://localhost:$port$1 > /tmp/test
    if [ $? -eq 0 ]; then
	if [ -f $1 ]; then
	    diff $1 /tmp/test
	fi
	if [ $? -ne 0 ]; then
	    err=$(($err + 1))
	    echo "error on file :" $1
	    echo "============================================="
	    echo $err ERRORS
	fi
    else
	if [ -f $1 ]; then
	    cat $1 > /dev/null 2> /dev/null
	    if [ $? -eq 0 ]; then
		echo "something gone wrong when fetching :" http://localhost:$port$1
	    fi
	fi
    fi
}

for i in $(cat files_to_test); do
    check_file $i
done

echo "============================================="
echo $err ERRORS
