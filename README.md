# LinkedList

=================================================================================================

File    : Serial_Linked_List.c
Purpose : Implement a linked list as a serial program
Compile : gcc -g -Wall -o serial_linked_list serial_linked_list.c
 
Run     : ./Serial_Linked_List <n> <m> <mMember> <mInsert> <mDelete>

To run serial linked list multiple times use Serial_Linked_List.sh file

Run: ./Serial_Linked_List.sh <n> <m> <mMember> <mInsert> <mDelete> <sampleSize>

==================================================================================================

File    : mutex_linked_list.c
Purpose : Implement a linked list as a parallel program with one mutex for the entire linked list
Compile : gcc -g -Wall -o mutex_linked_list mutex_linked_list.c -lpthread
Run     : ./mutex_linked_list <n> <m> <threadCount> <mMember> <mInsert> <mDelete>

To run mutex linked list multiple times use mutex_linked_list.sh file

Run: ./mutex_linked_list.sh <n> <m> <threadCount> <mMember> <mInsert> <mDelete> <sampleSize>

==================================================================================================

File    : Read_Write_Linked_List.c
Purpose : Implement a linked list as a parallel program with read-write locks for the entire linked list
Compile : gcc -g -Wall -o rw_linked_list rw_linked_list.c -lpthread

Run     : ./Read_Write_Linked_List <thread_count> <n> <m> <mMember> <mInsert> <mDelete>

To run mutex linked list multiple times use Read_Write_Linked_List.sh file

Run: ./Read_Write_Linked_List.sh <n> <m> <threadCount> <mMember> <mInsert> <mDelete> <sampleSize>

==================================================================================================
