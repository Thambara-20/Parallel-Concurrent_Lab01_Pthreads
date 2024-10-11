#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>

const int N = 1000;
const int M = 10000;
const float M_MEMBER = 0.99f;
const float M_INSERT = 0.005f;
const float M_DELETE = 0.005f;
const int THRESHOLD = 65536;
const int NUMBER_OF_THREADS = 1;
const int NUMBER_OF_RUNS = 100;

struct Node {
    int data;
    struct Node *next;
};

struct Node *head = NULL;
pthread_mutex_t list_mutex;

void Insert(struct Node **head, int value) {
    pthread_mutex_lock(&list_mutex);

    struct Node *newNode = (struct Node *) malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->next = NULL;

    if (*head == NULL || (*head)->data >= value) {
        newNode->next = *head;
        *head = newNode;
    } else {
        struct Node *current = *head;
        while (current->next != NULL && current->next->data < value) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }

    pthread_mutex_unlock(&list_mutex);
}

void Delete(struct Node **head, int value) {
    pthread_mutex_lock(&list_mutex);

    struct Node *temp = *head;
    struct Node *prev = NULL;

    if (temp != NULL && temp->data == value) {
        *head = temp->next;
        free(temp);
        pthread_mutex_unlock(&list_mutex);
        return;
    }

    while (temp != NULL && temp->data != value) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        pthread_mutex_unlock(&list_mutex);
        return;
    }

    prev->next = temp->next;
    free(temp);

    pthread_mutex_unlock(&list_mutex);
}

int Member(struct Node *head, int value) {
    pthread_mutex_lock(&list_mutex);

    struct Node *temp = head;
    while (temp != NULL) {
        if (temp->data == value) {
            pthread_mutex_unlock(&list_mutex);
            return 1;
        }
        temp = temp->next;
    }
    pthread_mutex_unlock(&list_mutex);
    return 0;
}

void *ThreadWork(void *arg) {
    int workloadOnThread = M / NUMBER_OF_THREADS;
    int mMember = M_MEMBER * workloadOnThread;
    int mInsert = M_INSERT * workloadOnThread;
    int mDelete = M_DELETE * workloadOnThread;

    for (int i = 0; i < mInsert; i++) {
        int value = rand() % THRESHOLD;
        if (!Member(head, value)) {
            Insert(&head, value);
        }
    }

    for (int i = 0; i < mMember; i++) {
        int value = rand() % THRESHOLD;
        Member(head, value);
    }
    for (int i = 0; i < mDelete; i++) {
        int value = rand() % THRESHOLD;
        if (!Member(head, value)) {
            Delete(&head, value);
        }
    }
    return NULL;
}

double get_time_in_ms() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000.0) + (time.tv_usec / 1000.0);
}

int main() {
    double times[NUMBER_OF_RUNS];
    srand(time(0));

    for (int run = 0; run < NUMBER_OF_RUNS; ++run) {
        head = NULL;
        pthread_mutex_init(&list_mutex, NULL);

        for (int i = 0; i < N; i++) {
            int value = rand() % THRESHOLD;
            if (!Member(head, value)) {
                Insert(&head, value);
            }
        }

        pthread_t threads[NUMBER_OF_THREADS];
        double start = get_time_in_ms();

        for (int i = 0; i < NUMBER_OF_THREADS; ++i) {
            pthread_create(&threads[i], NULL, ThreadWork, NULL);
        }

        for (int i = 0; i < NUMBER_OF_THREADS; ++i) {
            pthread_join(threads[i], NULL);
        }

        double end = get_time_in_ms();
        times[run] = end - start;

        pthread_mutex_destroy(&list_mutex);
        while (head != NULL) {
            struct Node *temp = head;
            head = head->next;
            free(temp);
        }
    }

    // Calculate mean
    double sum = 0;
    for (int i = 0; i < NUMBER_OF_RUNS; i++) {
        sum += times[i];
    }
    double mean = sum / NUMBER_OF_RUNS;

    // Calculate standard deviation
    double variance = 0;
    for (int i = 0; i < NUMBER_OF_RUNS; i++) {
        variance += (times[i] - mean) * (times[i] - mean);
    }
    variance /= NUMBER_OF_RUNS;
    double stdDev = sqrt(variance);

    printf("Mean Execution Time: %lf milliseconds\n", mean);
    printf("Standard Deviation: %lf milliseconds\n", stdDev);

    return 0;
}
