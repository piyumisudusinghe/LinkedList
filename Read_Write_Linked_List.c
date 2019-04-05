/* Linked List with read write lock
*
* Compile : gcc -g -Wall -o rw_linked_list rw_linked_list.c
        * Run : ./rw_linked_list <n> <m> <mMember> <mInsert> <mDelete>
*
* */

#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define MAX_THREADS 1024
#define MAX_RANDOM 65535


// Number of nodes in the linked list
int n = 0;

// Number of random operations in the linked list
int m = 0;

// Number of threads to execute
int thread_count = 0;

// Fractions of each operation
float m_insert_frac = 0.0, m_delete_frac = 0.0, m_member_frac = 0.0;

// Total number of each operation
int m_insert = 0, m_delete = 0, m_member = 0;

struct list_node_s *head = NULL;
pthread_rwlock_t rwlock;

//Node definition
struct list_node_s {
    int data;
    struct list_node_s *next;
};

int Insert(int value, struct list_node_s **head_pp);

int Delete(int value, struct list_node_s **head_pp);

int Member(int value, struct list_node_s *head_p);

void getArgument(int argc, char *argv[]);

double CalcTime(struct timeval time_begin, struct timeval time_end);

void *Manage_Threads(void *id);

// Linked List Insertion function
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

// Linked List Deletion function
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


// Linked List Membership function
int Member(int value, struct list_node_s *head_p) {
    struct list_node_s *current_p = head_p;

    while (current_p != NULL && current_p->data < value)
        current_p = current_p->next;

    if (current_p == NULL || current_p->data > value) {
        return 0;
    }
    else {
        return 1;
    }

}


// Getting the inputs
void getArgument(int argc, char *argv[]) {

    // Validating the number of arguments
    if (argc != 7) {
        printf("Please give the command: ./Read_Write_Linked_List <n> <m> <thread_count> <mMember> <mInsert> <mDelete>\n");
        exit(0);
    }

    // Set values for n,m and thread count
    n = (int) strtol(argv[1], (char **) NULL, 10);
    m = (int) strtol(argv[2], (char **) NULL, 10);
    thread_count = (int) strtol(argv[3], (char **) NULL, 10);

    //set values for fractions
    m_member_frac = (float) atof(argv[4]);
    m_insert_frac = (float) atof(argv[5]);
    m_delete_frac = (float) atof(argv[6]);

    // thread count validation
    if (thread_count <= 0 || thread_count > MAX_THREADS) {
        printf("Please give provide a valid number of threads in the range of 0 to %d\n", MAX_THREADS);
        exit(0);
    }

    //Argument validation
    if (n <= 0 || m <= 0 || m_member_frac + m_insert_frac + m_delete_frac != 1.0) {
        printf("Please give the command with the arguments: ./serial_linked_list <n> <m> <mMember> <mInsert> <mDelete>\n");

        if (n <= 0)
            printf("Please provide a valid number of nodes for the linked list (value of n)\n");

        if (m <= 0)
            printf("Please provide a valid number of operations for the linked list (value of m)\n");

        if (m_member_frac + m_insert_frac + m_delete_frac != 1.0)
            printf("Please provide valid fractions of operations for the linked list (value of mMember, mInsert, mDelete)\n");

        exit(0);
    }else{
        m_insert = (int) (m_insert_frac * m);
        m_delete = (int) (m_delete_frac * m);
        m_member = (int) (m_member_frac * m);

    }
}

// Thread Operations
void *Manage_Threads(void *thread_id) {

    int local_m = 0;
    int local_m_insert = 0;
    int local_m_delete = 0;
    int local_m_member = 0;

    int id = *(int *)thread_id;

    // Generate local no of insertion operations without loss
    if (m_insert % thread_count == 0 || m_insert % thread_count <= id) {
        local_m_insert = m_insert / thread_count;
    }
    else if (m_insert % thread_count > id) {
        local_m_insert = m_insert / thread_count + 1;
    }

    // Generate local no of member operations without loss
    if (m_member % thread_count == 0 || m_member % thread_count <= id) {
        local_m_member = m_member / thread_count;
    }
    else if (m_member % thread_count > id) {
        local_m_member = m_member / thread_count + 1;
    }

    // Generate local no of deletion operations without loss
    if (m_delete % thread_count == 0 || m_delete % thread_count <= id) {
        local_m_delete = m_delete / thread_count;
    }
    else if (m_delete % thread_count > id) {
        local_m_delete = m_delete / thread_count + 1;
    }

    local_m = local_m_insert + local_m_delete + local_m_member;

    int tot_count = 0;
    int member_count = 0;
    int insert_count = 0;
    int delete_count = 0;

    int finished_member = 0;
    int finished_insert = 0;
    int delete_finished = 0;

    int i = 0;
    while (tot_count < local_m) {

        int random_value = rand() % MAX_RANDOM;
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


        // Member operation
        if (rand_select == 0 && finished_member == 0) {
            if (member_count < local_m_member) {
                pthread_rwlock_rdlock(&rwlock);
                Member(random_value, head);
                pthread_rwlock_unlock(&rwlock);
                member_count++;
            } else
                finished_member = 1;
        }

            // Insert Operation
        else if (rand_select == 1 && finished_insert == 0) {
            if (insert_count < local_m_insert) {
                pthread_rwlock_wrlock(&rwlock);
                Insert(random_value, &head);
                pthread_rwlock_unlock(&rwlock);
                insert_count++;
            } else
                finished_insert = 1;
        }

            // Delete Operation
        else if (rand_select == 2 && delete_finished == 0) {

            if (delete_count < local_m_delete) {
                pthread_rwlock_wrlock(&rwlock);
                Delete(random_value, &head);
                pthread_rwlock_unlock(&rwlock);
                delete_count++;
            } else
                delete_finished = 1;
        }
        tot_count = insert_count + member_count + delete_count;
        i++;
    }
    return NULL;
}

// Calculating time
double CalcTime(struct timeval start_time, struct timeval end_time) {
    return (double) (end_time.tv_usec - start_time.tv_usec) / 1000000 + (double) (end_time.tv_sec - start_time.tv_sec);
}

int main(int argc, char *argv[]) {

    // Obtaining the inputs
    getArgument(argc, argv);

    pthread_t *thread_handlers;
    thread_handlers = (pthread_t*)malloc(sizeof(pthread_t) * thread_count);

    // time variables
    struct timeval start_time, end_time;

    int *thread_id;
    thread_id = (int *)malloc(sizeof(int) * thread_count);


    // Linked List Generation with Random values
    int i = 0;
    while (i < n) {
        if (Insert(rand() % 65535, &head) == 1)
            i++;
    }

    pthread_rwlock_init(&rwlock, NULL);
    gettimeofday(&start_time, NULL);

    // Thread Creation
    i = 0;
    while (i < thread_count) {
        thread_id[i] = i;
        pthread_create(&thread_handlers[i], NULL, (void *) Manage_Threads, (void *) &thread_id[i]);
        i++;
    }

    // Thread Join
    i = 0;
    while (i < thread_count) {
        pthread_join(thread_handlers[i], NULL);
        i++;
    }
    gettimeofday(&end_time, NULL);
    pthread_rwlock_destroy(&rwlock);

    printf("Linked List with read write locks Time Spent : %.6f\n", CalcTime(start_time, end_time));

    return 0;
}
