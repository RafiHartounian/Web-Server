#!/bin/bash

# Clean the test dir
rm -rf ../crud/test

echo '{"a":2}' > ../crud/test/1 

SERVER_PATH=../build/bin/server
CONFIG_PATH=../configs/api_test_config

# start server in the background
$SERVER_PATH $CONFIG_PATH &
sleep 1

SERVER_PID=$!

TIMEOUT=1
SERVER_IP=localhost
SERVER_PORT=8080

# POST Test
POST_REQ_SUCCESS=0 
echo "POST test"
timeout $TIMEOUT curl -s -i -X POST -H "Host:" -H "User-Agent:" $SERVER_IP:$SERVER_PORT/api/test -d '{"a":1}'
sleep 1
echo "Compare the result" ../
diff ../crud/test/2 POST_test #testing for persistent storage across server restarts
MATCHES=$?
echo $MATCHES
if [ $MATCHES -eq 1 ];
then
  echo "Test POST request Failed"
else 
  POST_REQ_SUCCESS=1 
  echo "Test POST Request Succeeded"
fi

DELETE_REQ_SUCCESS=0

# Curl server and delete
timeout $TIMEOUT curl -s -i -X DELETE -H "Host:" -H "User-Agent:" $SERVER_IP:$SERVER_PORT/api/test/1

# Check if file exists or not
FILE=../crud/test/1

test -f "$FILE";
RESULT=$?

if [ $RESULT -ne 1 ]; 
then
    echo "Test DELETE request failed"
else 
    DELETE_REQ_SUCCESS=1
    echo "Test DELETE request succeeded"
fi

rm -rf ../crud/test

# kill server and return test results
kill -9 $SERVER_PID

if [ $POST_REQ_SUCCESS -eq 1 ] && [ $DELETE_REQ_SUCCESS -eq 1 ];
then
  exit 0
fi

exit 1