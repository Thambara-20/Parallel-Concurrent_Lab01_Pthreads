#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

const int N = 1000;
const int M = 10000;
const float M_MEMBER = 0.99f;
const float M_INSERT = 0.005f;
const float M_DELETE = 0.005f;
const int THRESHOLD = 65536;
const int NUMBER_OF_RUNS = 100;

struct Node {
    int data;
    struct Node* next;
};

void Insert(struct Node** head, int value) {
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->next = NULL;

    if (*head == NULL || (*head)->data >= value) {
        newNode->next = *head;
        *head = newNode;
        return;
    }

    struct Node* current = *head;
    while (current->next != NULL && current->next->data < value) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
}

void Delete(struct Node** head, int value) {
    struct Node* temp = *head;
    struct Node* prev = NULL;

    if (temp != NULL && temp->data == value) {
        *head = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->data != value) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return;

    prev->next = temp->next;
    free(temp);
}

bool Member(struct Node* head, int value) {
    struct Node* temp = head;
    while (temp != NULL) {
        if (temp->data == value)
            return true;
        temp = temp->next;
    }
    return false;
}

int main() {
    double times[NUMBER_OF_RUNS];
    srand(time(0));

    for (int run = 0; run < NUMBER_OF_RUNS; ++run) {
        struct Node* head = NULL;
        for (int i = 0; i < N; i++) {
            int value = rand() % THRESHOLD;
            if (!Member(head, value)) {
                Insert(&head, value);
            }
        }

        int mMember = M_MEMBER * M;
        int mInsert = M_INSERT * M;
        int mDelete = M_DELETE * M;

        clock_t start = clock();

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

        clock_t end = clock();
        double duration = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
        times[run] = duration;
    }

    double sum = 0;
    for (int i = 0; i < NUMBER_OF_RUNS; i++) {
        sum += times[i];
    }
    double mean = sum / NUMBER_OF_RUNS;

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
