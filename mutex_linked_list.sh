#!/bin/bash

gcc -g -Wall -o mutex_linked_list mutex_linked_list.c -lpthread

 for i in `seq 1 $7`;
        do
                echo "Number of times: " $i
                ./mutex_linked_list $1 $2 $3 $4 $5 $6
        done