
/*
 * Serial Linked List
 *
 * Compile : gcc -g -Wall -o serial_linked_list serial_linked_list.c
 * Run : ./serial_linked_list <n> <m> <mMember> <mInsert> <mDelete>
 *
 * */

#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>
#include <math.h>
#define MAX_RANDOM 65535

// Number of nodes in the linked list
int n = 0;

// Number of random operations in the linked list
int m = 0;

// Fractions of each operation
float m_insert_frac, m_delete_frac, m_member_frac;
float m_insert,m_delete,m_member;




//Node definition
struct list_node_s {
    int data;
    struct list_node_s *next;
};

int Insert(int value, struct list_node_s **head_pp);

int Delete(int value, struct list_node_s **head_pp);

int Member(int value, struct list_node_s *head_p);

void getArguments(int argc, char *argv[]);

double CalcTime(struct timeval start_time, struct timeval end_time);

void RecordResults(float time);


//Linked List Deletion function
int Delete(int value, struct list_node_s **head_pp) {
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value) {
        if (pred_p == NULL) {
            *head_pp = curr_p->next;
            free(curr_p);
        }
        else {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;
    }
    else
        return 0;
}


//Linked List Insertion function
int Insert(int value, struct list_node_s **head_pp) {
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;
    struct list_node_s *temp_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;

        if (pred_p == NULL)
            *head_pp = temp_p;
        else
            pred_p->next = temp_p;

        return 1;
    }
    else
        return 0;
}

//Linked List Membership function
int Member(int value, struct list_node_s *head_p) {
    struct list_node_s *current_p = head_p;

    while (current_p != NULL && current_p->data < value)
        current_p = current_p->next;

    if (current_p == NULL || current_p->data > value) {
        return 0;
    }
    else
        return 1;
}


//Getting the inputs
void getArguments(int argc, char *argv[]) {
    if (argc != 6) {
        printf("Please give the command: ./Serial_Linked_List <n> <m> <mMember> <mInsert> <mDelete>\n");
        exit(0);
    }

    n = (int) strtol(argv[1], (char **) NULL, 10);
    m = (int) strtol(argv[2], (char **) NULL, 10);

    m_member_frac = (float) atof(argv[3]);
    m_insert_frac = (float) atof(argv[4]);
    m_delete_frac = (float) atof(argv[5]);


    //Handling user inputs
    if (n <= 0 || m <= 0 || m_member_frac + m_insert_frac + m_delete_frac != 1.0) {
        printf("Please enter valid argument types\n");

        if (n <= 0)
            printf("Value of n should be greater than 0\n");

        if (m <= 0)
            printf("value of m should be greater than 0\n");

        if (m_member_frac + m_insert_frac + m_delete_frac != 1.0)
            printf("Please enter valid fraction values\n");

        exit(0);
    }else{
        m_insert = m_insert_frac * m;
        m_delete = m_delete_frac * m;
        m_member = m_member_frac * m;
    }
}

//Calculating time
double CalcTime(struct timeval time_begin, struct timeval time_end) {
    return (double) (time_end.tv_usec - time_begin.tv_usec) / 1000000 + (double) (time_end.tv_sec - time_begin.tv_sec);
}

//main method which handle the execution of m operations
int main(int argc, char *argv[]){

    struct list_node_s *head = NULL;
    struct timeval start_time, end_time;


    //Getting the inputs
    getArguments(argc, argv);

    //Linked List Generation with Random values
    int i = 0;
    while (i < n) {
        if (Insert(rand() % MAX_RANDOM, &head) == 1)
            i++;
    }

    //Operations in the linked list
        int operation_count = 0;
        int member_count = 0;
        int insert_count = 0;
        int delete_count = 0;

        gettimeofday(&start_time, NULL);
        while (operation_count < m) {

            int rand_value = rand() % MAX_RANDOM;
            int rand_select;

            if(member_count >= m_member){
                rand_select = rand() % (2 + 1 - 1) + 1;
            } else if (delete_count >= m_delete) {
                rand_select = rand() % (1 + 1 - 0) + 0;
            }else if (insert_count >= m_insert){
                rand_select = rand() % (2 + 1 -0) + 0 ;
                while(rand_select == 1){
                    rand_select = rand() % (2 + 1 -0) + 0;
                }
            }else {
                rand_select = rand() % 3;
            }


            if (rand_select == 0 && member_count < m_member) {
                Member(rand_value, head);
                member_count++;
            }

            if (rand_select == 1 && insert_count < m_insert) {
                Insert(rand_value, &head);
                insert_count++;
            }

            else if (rand_select == 2 && delete_count < m_delete) {
                Delete(rand_value, &head);
                delete_count++;
            }

            operation_count = insert_count + member_count + delete_count;
        }

        gettimeofday(&end_time, NULL);
        float time_spent = CalcTime(start_time, end_time);
        RecordResults(time_spent);
        printf("Serial Linked List Time Spent : %.6f secs\n",time_spent);

}

void RecordResults(float value){
    FILE * fp;
    fp = fopen ("Serial_Linked_List.csv", "a");
    fprintf(fp, "%6f\n", value);
    fclose(fp);
}




