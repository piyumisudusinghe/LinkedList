#!/bin/bash

gcc -g -Wall -o mutex_linked_list mutex_linked_list.c -lpthread

 for i in `seq 1 $1`;
        do
                echo "Number of times: " $i
                ./mutex_linked_list 1000 10000 1 0.99 0.005 0.005
        done   