#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define MAX_THREAD_COUNT 1024
#define MAX_RANDOM_VALUE 65535

// Number of executed oprations member,insert and delete
int member_opt_count = 0, insert_opt_count =0, delete_opt_count =0;

// Number of nodes in the linked list
int n = 0;

// Number of random operations in the linked list
int m = 0;

// Number of threads to execute
int thread_count = 0;

// Total number of each operation
float m_insert = 0.0, m_delete = 0.0, m_member = 0.0;

struct list_node_s *head = NULL;
pthread_mutex_t mutex;

// Node definition
struct list_node_s {
    int data;
    struct list_node_s *next;
};

int Member(int value, struct list_node_s *head_p);

int Insert(int value, struct list_node_s **head_pp);

int Delete(int value, struct list_node_s **head_pp);

void getArguments(int argc, char *argv[]);

void *ManageThreads();

double CalculateTime(struct timeval starting_time, struct timeval ending_time);

int main(int argc, char *argv[]) {

    // Obtaining the inputs
    getArguments(argc, argv);

    for(int count=0;count<50;count++){
        pthread_t *thread_handlers;
        thread_handlers = malloc(sizeof(pthread_t) * thread_count);
        //float elapsedTime[5];

        // time variables
        struct timeval time_begin, time_end;

        // Linked List Generation with Random values
        int i = 0;
        //printf("%d",i);
        while (i < n) {
            if (Insert(rand() % 65535, &head) == 1)
                i++;
        }
         //printf("%d",i);
        // Initializing the mutex
        pthread_mutex_init(&mutex, NULL);

        // Getting the begin time stamp
        gettimeofday(&time_begin, NULL);
        //printf("time begin %d - %d",(double)time_begin.tv_sec/100000,(double)time_begin.tv_usec/1000000);

        // Thread Creation
        i = 0;
        while (i < thread_count) {
            pthread_create(&thread_handlers[i], NULL, (void *) ManageThreads, NULL);
            i++;
        }

        // Thread Join
        i = 0;
        while (i < thread_count) {
            pthread_join(thread_handlers[i], NULL);
            i++;
        }

        // Getting the end time stamp
        gettimeofday(&time_end, NULL);
        printf("time end %d - %d\n",(double)time_end.tv_sec/1000000,(double)time_end.tv_usec/1000000);
       // printf("Execurion time - %d is %.6f\n",count,CalculateTime(time_begin,time_end));
        head = NULL;

        // Destroying the mutex
        pthread_mutex_destroy(&mutex);
    }

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

        if (pred_p == NULL){ 
           if(temp_p->next==NULL){
                printf("head\n");
           }
            *head_pp = temp_p;
        }
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

//Getting the inputs
void getArguments(int argc, char *argv[]) {

    if(argc==7){
    
        n = (int) strtol(argv[1], (char **) NULL, 10);   //setting number of nodes - n 
        m = (int) strtol(argv[2], (char **) NULL, 10);   //setting number of operations - m

        //Validate n and m
        if(n<=0||m<=0){
            if(n<=0){
                printf("Please enter valid number of nodes as the first argument\n");
            }
            if(m<=0){
                printf("Please provide valid number of operations as the second argument\n");
            }
            exit(0);
        }

        thread_count = (int) strtol(argv[3],(char **) NULL,10);   //setting number of threads  

        //Validate number of threads
        if(thread_count<=0 || thread_count>MAX_THREAD_COUNT){
            printf("Please enter valid number of thread count in range (1,2^16-1)\n");
            exit(0);
        }

        float m_member_frac = (float) atof(argv[4]);      //setting fraction of member operations
        float m_insert_frac = (float) atof(argv[5]);      //setting fraction of insert operations
        float m_delete_frac = (float) atof(argv[6]);      //setting fraction of delete operations

        //Validate fractions of member operations,insert operations and delete operations
        if(m_member_frac<0||m_insert_frac<0||m_delete_frac<0||m_member_frac+m_insert_frac+m_delete_frac!=1){
            printf("Please provide valid fractions according to following order\n [./mutex_linked_list <n> <m> <thread_count> <mMember> <mInsert> <mDelete>]\n");
            exit(0);      
        }else{
            m_insert = m_insert_frac * m;
            m_delete = m_delete_frac * m;
            m_member = m_member_frac * m;
        }

    }else{
        printf("Please run [./mutex_linked_list <n> <m> <thread_count> <mMember> <mInsert> <mDelete>] with the all arguments\n");
        exit(0);
    }


}

// Thread Operations
void *ManageThreads() {

    int count_tot = 0;

    int is_member_finished = 0;
    int is_insert_finished = 0;
    int is_delete_finished = 0;

    while (count_tot < m) {

        // Variable to randomly generate values for operations
        int random_value = rand() % MAX_RANDOM_VALUE;

        // Variable to randomly select one of the three operations
        int random_select = rand() % 3;

        // Member operation
        if (random_select == 0 && is_member_finished == 0) {

            pthread_mutex_lock(&mutex);
            if (member_opt_count < m_member) {
                Member(random_value, head);
                member_opt_count++;
            }else
                is_member_finished =1;
            pthread_mutex_unlock(&mutex);
        }

        // Insert Operation
        if (random_select == 1 && is_insert_finished == 0) {

            pthread_mutex_lock(&mutex);
            if (insert_opt_count < m_insert) {
                Insert(random_value, &head);
                insert_opt_count++;
            }else
                is_insert_finished =1;
            pthread_mutex_unlock(&mutex);
        }

        // Delete Operation
        else if (random_select == 2 && is_delete_finished == 0) {

            pthread_mutex_lock(&mutex);
            if (delete_opt_count < m_delete) {
                Delete(random_value, &head);
                delete_opt_count++;
            }else
                is_delete_finished =1;
            pthread_mutex_unlock(&mutex);
        }

        // Updating the count
        count_tot = insert_opt_count + member_opt_count + delete_opt_count;
    }
    return NULL;
}

double CalculateTime(struct timeval starting_time, struct timeval ending_time){
    return (double) (ending_time.tv_usec -starting_time.tv_usec) / 1000000 + (double) (ending_time.tv_sec - starting_time.tv_sec);
}
