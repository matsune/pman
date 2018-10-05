#!/bin/sh
TIME=5  #$(( $RANDOM % 5 ))
echo "start sub.sh $1"
echo "sleep $TIME"
sleep $TIME
echo "end sub.sh $1"
exit 0
