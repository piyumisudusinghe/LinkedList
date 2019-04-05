#!/bin/bash

gcc -g -Wall -o Read_Write_Linked_List Read_Write_Linked_List.c -lpthread

 for i in `seq 100 $7`;
        do
                echo "Number of times: " $i
                ./Read_Write_Linked_List $1 $2 $3 $4 $5 $6
        done