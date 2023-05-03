#!/bin/bash

SERVER_PATH=../build/bin/server
CONFIG_PATH=../configs/test_config

# start server in the background
$SERVER_PATH $CONFIG_PATH &
sleep 0.2

SERVER_PID=$!

TIMEOUT=0.2
SERVER_IP=localhost
SERVER_PORT=8080

# test valid request
VALID_REQ_SUCCESS=0
timeout $TIMEOUT curl -s -i -H "Host:" -H "User-Agent:" $SERVER_IP:$SERVER_PORT/echo > integ_test_valid_req_out
diff integ_test_valid_req_out integ_test_valid_req_expected

MATCHES=$?
echo $MATCHES
if [ $MATCHES -eq 1 ];
then
  echo "Test Valid Request Failed"
else 
  VALID_REQ_SUCCESS=1
  echo "Test Valid Request Succeeded"
fi

rm integ_test_valid_req_out

# #static
VALID_STATIC_SUCCESS=0
timeout $TIMEOUT curl -s -i -H "Host:" -H "User-Agent:" $SERVER_IP:$SERVER_PORT/static1/example.html > integ_test_html_res_out
diff integ_test_html_res_out integ_test_html_res_expected

MATCHES=$?
echo $MATCHES
if [ $MATCHES -eq 1 ];
then
  echo "Test Valid Static HTML Request Failed"
else 
  VALID_STATIC_SUCCESS=1
  echo "Test Valid Static HTML Request Succeeded"
fi

rm integ_test_html_res_out

# #txt
VALID_TEXT_SUCCESS=0
timeout $TIMEOUT curl -s -i -H "Host:" -H "User-Agent:" $SERVER_IP:$SERVER_PORT/static1/example.txt > integ_test_example_text_out
diff integ_test_example_text_out integ_test_example_expected

MATCHES=$?
echo $MATCHES
if [ $MATCHES -eq 1 ];
then
  echo "Test Valid Static Text Request Failed"
else 
  VALID_TEXT_SUCCESS=1
  echo "Test Valid Static Text Request Succeeded"
fi

rm integ_test_example_text_out
#empty header
NOT_FOUND_SUCCESS=0
timeout $TIMEOUT curl -s -i -H "Host:" -H "User-Agent:" $SERVER_IP:$SERVER_PORT > integ_test_not_found_res_out
diff integ_test_not_found_res_out integ_test_not_found_res_expected

MATCHES=$?
echo $MATCHES
if [ $MATCHES -eq 1 ];
then
  echo "Test NOT_FOUND Request Failed"
else 
  NOT_FOUND_SUCCESS=1
  echo "Test NOT_FOUND Request Succeeded"
fi

rm integ_test_not_found_res_out




# test invalid request
INVALID_REQ_SUCCESS=0
echo "invalid" | timeout $TIMEOUT nc $SERVER_IP $SERVER_PORT > integ_test_invalid_req_out
diff integ_test_invalid_req_out integ_test_invalid_req_expected

MATCHES=$?
echo $MATCHES
if [ $MATCHES -eq 1 ];
then
  echo "Test Invalid Request Failed"
else 
  INVALID_REQ_SUCCESS=1
  echo "Test Invalid Request Succeeded"
fi

rm integ_test_invalid_req_out

# kill server and return test results
kill -9 $SERVER_PID

if [ $VALID_REQ_SUCCESS -eq 1 ] && [ $VALID_STATIC_SUCCESS -eq 1 ] && [ $INVALID_REQ_SUCCESS -eq 1 ] && [ $NOT_FOUND_SUCCESS -eq 1 ] && [ $VALID_TEXT_SUCCESS -eq 1 ];
then
  exit 0
fi

exit 1