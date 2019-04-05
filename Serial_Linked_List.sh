#!/bin/bash

gcc -g -Wall -o Serial_Linked_List Serial_Linked_List.c

 for i in `seq 50 $6`;
        do
                echo "Number of times: " $i
                ./Serial_Linked_List $1 $2 $3 $4 $5
        done