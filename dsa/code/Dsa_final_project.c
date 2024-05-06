#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define MAX 100
#define HSIZE 11
#define SHSIZE 8
#define SMAX 20
#define RMAX 100
#define TMAX 100
#define PRIME 101
#define max_inc 20
#define max_safe 20
#define SECOND_HASH_CONSTANT 8
#define MAX_NAME_LENGTH 100
#define HASH_SIZE 11

struct Incident
{
    int incident_id;
    char inc_name[50];
    char city[50];
    int dist;
};


struct Resource {
    int resource_id;
    char name[50];
    int quantity;
    int urgency;
};


struct Shelter
{
    char location[TMAX];
    int safescore;
};


struct stack
{
    int stk[MAX];
    int top;
};


struct hash
{
    struct Incident *R[MAX];
};


struct Node
{
    struct Resource* resource;
    struct TreeNode* left;
    struct TreeNode* right;
};


struct BST
{
    struct Node* root;
};


struct Queue
{
    int front;
    int rear;
    int size;
    int capacity;
    int *array;
};


int hfun(int key)
{
    return key % HSIZE;
}


int shfun(int key)
{
    return SHSIZE - (key % SHSIZE);
}


int dhfun(int key, int i)
{
    return (hfun(key) + i * shfun(key)) % HSIZE;
}


void inserthash(struct hash *ht, struct Incident *r)
{
    int index;
    index = hfun(r->incident_id);
    int i = 0;
    while (ht->R[index] != NULL)
    {
        index = dhfun(r->incident_id, i);
        i++;
    }
    ht->R[index] = r;
}


void displayhash(struct hash *ht)
{
    int i;
    for (i = 0; i < HSIZE; i++)
        {
        if (ht->R[i] != NULL)
        {
            printf("INCIDENT ID: %d\n", ht->R[i]->incident_id);
            printf("INCIDENT NAME: %s\n", ht->R[i]->inc_name);
            printf("CITY NAME: %s\n", ht->R[i]->city);
            printf("DISTANCE: %d\n", ht->R[i]->dist);
            printf("------------------------------\n");
        }
    }
}


struct Incident* search(struct hash* ht, int incident_id)
{
    int index = hfun(incident_id);
    int i = 0;
    while (ht->R[index] != NULL)
        {
        if (ht->R[index]->incident_id == incident_id)
        {
            return ht->R[index];
        }
        index = dhfun(incident_id, i);
        i++;
    }
    return NULL;
}


struct Node* create(struct Resource* r)
{
    struct Node* nw = (struct Node*)malloc(sizeof(struct Node));
    if (nw != NULL)
    {
        nw->resource = r;
        nw->left = nw->right = NULL;
    }
    return nw;
}


struct Node* insert(struct Node* root, struct Resource* r)
{
    if (root == NULL)
    {
        return create(r);
    }

    if (strcmp(r->name, root->resource->name) < 0)
    {
        root->left = insert(root->left, r);
    } else if (strcmp(r->name, root->resource->name) > 0)
    {
        root->right = insert(root->right, r);
    }

    return root;
}


struct Resource* fBST(struct Node* root, const char* name)
{
    while (root != NULL)
        {
        int cmp = strcmp(name, root->resource->name);
        if (cmp == 0)
        {
            return root->resource;
        } else if (cmp < 0)
        {
            root = root->left;
        } else
        {
            root = root->right;
        }
    }

    return NULL;
}


void initBST(struct BST* rBST, struct Resource* r, int Rnum)
{
    rBST->root = NULL;
    int i;
    for ( i = 0; i < Rnum; ++i)
    {
        rBST->root = insert(rBST->root, &r[i]);
    }
}


void alloctr(struct Incident *in, int Snum,int Inum, struct BST* rBST) {
    char name[50];
    int f=0,a;

    printf("Resource allocation:\n");

    for (int i = 0; i < Snum; ++i) {
            printf("Enter the incident for which you are trying to allocate resource\n");
           scanf("%s",name);
           for(int i=0;i<Inum ;i++){
            if(strcmp(name,in[i].inc_name)==0)
            {
                f=1;
           printf("Available resources are: WATER,FOOD,SHELTER,BLANKETS\n");
            while(1)
            {
                printf("Enter 1 for allocating and 0 to exit\n");
                scanf("%d",&a);
                if(a==1){
            printf("Enter the name of the resource to allocate: ");
            scanf("%s", name);

            // Look up the resource in the binary search tree
            struct Resource* fR = fBST(rBST->root, name);

            if (fR != NULL && fR->urgency > 0) {
                printf("- Resource: %s, Quantity: %d, Urgency: %d\n", fR->name, fR->quantity, fR->urgency);
                fR->urgency--;
            }
             printf("------------------------------\n");
                }
                else
                     return;
        }
     }
   }
   if(f==0)
    printf("Incident not found\n");
}

}


void merge(struct Resource arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    struct Resource L[n1], R[n2];
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i].urgency <= R[j].urgency)
        {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}


void mergeSort(struct Resource arr[], int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}


void display(struct Resource arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("Resource ID: %d Name: %s Quantity: %d Urgency: %d\n",
               arr[i].resource_id, arr[i].name, arr[i].quantity, arr[i].urgency);
    }
}


int compare(const void *a, const void *b)
{
    return strcmp(((struct Incident *)a)->city, ((struct Incident *)b)->city);
}


int partition(struct Incident *arr, int low, int high)
{
    struct Incident pivot = arr[low];
    int left = low + 1;
    int right = high;

    while (left <= right)
        {
        while (left <= right && compare(&arr[left], &pivot) < 0)
        {
            left++;
        }
        while (left <= right && compare(&arr[right], &pivot) > 0)
        {
            right--;
        }
        if (left <= right)
        {
            struct Incident temp = arr[left];
            arr[left] = arr[right];
            arr[right] = temp;
            left++;
            right--;
        }
    }

    struct Incident temp = arr[low];
    arr[low] = arr[right];
    arr[right] = temp;

    return right;
}


void quicksort(struct Incident *arr, int low, int high)
{
    if (low < high)
    {
        int pivot = partition(arr, low, high);
        quicksort(arr, low, pivot);
        quicksort(arr, pivot + 1, high);
    }
}


void heapify(struct Shelter arr[], int n, int i)
{
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l].safescore > arr[largest].safescore)
        largest = l;
    if (r < n && arr[r].safescore > arr[largest].safescore)
        largest = r;

    if (largest != i) {
        struct Shelter temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;

        heapify(arr, n, largest);
    }
}


void heapSort(struct Shelter arr[], int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--)
    {
        struct Shelter temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0);
    }
}

void compute(const char* pattern, int M, int* lps)
{
    int len = 0;
    lps[0] = 0;
    int i = 1;

    while (i < M)
    {
        if (pattern[i] == pattern[len])
        {
            len++;
            lps[i] = len;
            i++;
        } else
        {
            if (len != 0)
            {
                len = lps[len - 1];
            } else
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}


struct Resource* searchr(struct Resource* r, int Rnum, const char* Name)
{
    for (int i = 0; i < Rnum; ++i)
        {
        int M = strlen(Name);
        int N = strlen(r[i].name);

        int lps[M];
        compute(Name, M, lps);

        int j = 0;
        int k = 0;

        while (k < N)
        {
            if (Name[j] == r[i].name[k])
            {
                j++;
                k++;
            }

            if (j == M)
            {
                return &r[i];
            } else if (k < N && Name[j] != r[i].name[k])
            {
                if (j != 0)
                {
                    j = lps[j - 1];
                } else
                {
                    k++;
                }
            }
        }
    }

    return NULL;
}


int full(struct stack s)
{
   return s.top==MAX-1;;
}


void push(struct stack *s,int data)
{
    s->top=s->top+1;
    s->stk[s->top]=data;
}


int empty(struct stack s)
{
    return s.top==-1;
}


void displays(struct stack s)
{
    int i;
    for(i=s.top;i>=0;i--)
    {
        printf("DATA=%d\n",s.stk[i]);
    }
}


int pop(struct stack *s)
{
    int data;
    data=s->stk[s->top];
    //printf("YOUR RECENT CHOICE:%d\n",data);
    s->top=s->top-1;
    return data;
}


int peek(struct stack s)
{
    return s.stk[s.top];
}


void welcome()
{
    int i = 0;
    for (i = 0; i < 100; i++)
        printf("*");

    printf("\n\n\t\t\t\tRESOURCE ASSESSMENT AND DISASTER MANAGEMENT\n\n\n");

    for (i = 0; i < 100; i++)
        printf("*");

    printf("\n\n");
}


void feedback()
{
    printf("On a scale from Meh to Mind-Blown, how would you rate your experience today? We're aiming for the stars, but your thoughts matter most\n");
    char feed[1000];
    getchar();
    fgets(feed, sizeof(feed), stdin);

    char* username = getenv("USERPROFILE");
    char filepath[MAX];
    strcpy(filepath, username);
    strcat(filepath, "\\Downloads\\feedback.txt");

    FILE* fp = fopen(filepath, "a");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    fprintf(fp, "%s", feed);
    fclose(fp);
}


int chash(char str[], int l) {
    int hv = 0;
    for (int i = 0; i < l; i++) {
        hv = (hv * PRIME + str[i]) % TMAX;
    }
    return hv;
}


int searchl(struct Shelter s[], int Snum, char sl[]) {
    size_t l = strlen(sl);
    int sh = chash(sl, l);

    for (int i = 0; i < Snum; i++) {
        size_t sringl = strlen(s[i].location);

        int shash = chash(s[i].location, sringl);

        if (shash == sh) {
            size_t j;
            for (j = 0; j < l; j++) {
                if (sl[j] != s[i].location[j])
                    break;
            }

            if (j == l) {
                return i;
            }
        }
    }

    return -1;
}


void loadIncidentData(struct Incident *R, int *Rnum)
{
    char *u = getenv("USERPROFILE");
    char file[MAX];
    strcpy(file, u);
    strcat(file, "\\Downloads\\historical_dataimp.txt");

    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    while (*Rnum < MAX && fscanf(fp, "%d%s%s%d", &R[*Rnum].incident_id, R[*Rnum].inc_name, R[*Rnum].city, &R[*Rnum].dist) == 4)
    {
        (*Rnum)++;
    }

    fclose(fp);
}


void loadResourceData(struct Resource *r, int *Rnum)
{
    char *u = getenv("USERPROFILE");
    char file[MAX];
    strcpy(file, u);
    strcat(file, "\\Downloads\\resource.txt");

    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        printf("Error opening resources file.\n");
        return;
    }

    while (*Rnum < RMAX && fscanf(fp, "%d %s %d %d", &r[*Rnum].resource_id, r[*Rnum].name,
                                  &r[*Rnum].quantity, &r[*Rnum].urgency) == 4)
    {
        (*Rnum)++;
    }

    fclose(fp);
}


struct Queue *createQueue(int capacity)
{
    struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue));
    if (!queue)
    {
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->array = (int *)malloc(capacity * sizeof(int));
    if (!queue->array)
    {
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    return queue;
}


void enqueue(struct Queue *queue, int item)
{
    if (queue->size == queue->capacity)
    {
        printf("Queue is full\n");
        exit(EXIT_FAILURE);
    }

    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size++;
}


int dequeue(struct Queue *queue)
{
    if (queue->size == 0)
    {
        printf("Queue is empty\n");
        exit(EXIT_FAILURE);
    }

    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;

    return item;
}


bool isQueueEmpty(struct Queue *queue)
{
    return (queue->size == 0);
}


void bfs(struct Incident incidents[], int n, int start, int adjMatrix[][n])
{
    // Initialize visited array
    bool visited[n];
    for (int i = 0; i < n; i++)
    {
        visited[i] = false;
    }


    struct Queue *queue = createQueue(n);


    visited[start] = true;
    enqueue(queue, start);

    while (!isQueueEmpty(queue))
    {

        int current = dequeue(queue);
        struct Incident currentIncident = incidents[current];
        printf("%d %s %s %d\n", currentIncident.incident_id, currentIncident.inc_name, currentIncident.city, currentIncident.dist);

        for (int i = 0; i < n; i++)
        {
            if (!visited[i] && adjMatrix[current][i] == 1)
            {
                visited[i] = true;
                enqueue(queue, i);
            }
        }
    }

    free(queue->array);
    free(queue);
}


void loadShelterData(struct Shelter *s, int *Snum)
{
    char *u = getenv("USERPROFILE");
    char file[MAX];
    strcpy(file, u);
    strcat(file, "\\Downloads\\shelters.txt");

    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        printf("Error opening shelters file.\n");
        return;
    }

    while (*Snum < SMAX && fscanf(fp, "%s%d", s[*Snum].location, &s[*Snum].safescore) == 2)
    {
        (*Snum)++;
    }

    fclose(fp);
}

struct hash_entry {
    int num;
    char name[MAX_NAME_LENGTH];
};

struct HashTable {
    struct hash_entry* table[HASH_SIZE];
};

int hashFunction(int key) {
    return key % HASH_SIZE;
}

int secondHashFunction(int key) {
    return SECOND_HASH_CONSTANT - (key % SECOND_HASH_CONSTANT);
}

int doubleHashFunction(int key, int i) {
    return (hashFunction(key) + i * secondHashFunction(key)) % HASH_SIZE;
}

void insertRecord(struct HashTable* ht, struct hash_entry* record) {
    int index = hashFunction(record->num);
    int step = secondHashFunction(record->num);

    int i = 0;
    while (ht->table[index]!= NULL) {
        index = doubleHashFunction(record->num, i);
        i++;
    }

    // Allocate space for the name and copy the data
    ht->table[index] = malloc(sizeof(struct hash_entry));
    ht->table[index]->num = record->num;
    strncpy(ht->table[index]->name, record->name, MAX_NAME_LENGTH);
}

void displayHashTable(const struct HashTable* ht) {
    printf("Displaying the historical data of the incidents occurred:\n");
    for (int i = 0; i < HASH_SIZE; ++i) {
        if (ht->table[i] != NULL) {
            printf("INCIDENT ID: %d\n", ht->table[i]->num);
            printf("INCIDENT NAME: %s\n", ht->table[i]->name);
            printf("------------------------------\n");
        }
    }
}

struct hash_entry* searchRecord(const struct HashTable* ht, int key) {
    int index = hashFunction(key);
    int step = secondHashFunction(key);

    int i = 0;
    while (ht->table[index] != NULL) {
        if (ht->table[index]->num == key) {
            return ht->table[index];
        }
        index = doubleHashFunction(key, i);
        i++;
    }

    // Record not found
    return NULL;
}

int main()
{
    welcome();
    int choice;
    struct stack stk;
    stk.top = -1;

    struct Incident R[MAX];
    int Rnum = 0;

    struct Resource r[RMAX];
    int RnumResources = 0;

    struct Shelter s[SMAX];
    int Snum = 0;

    loadIncidentData(R, &Rnum);
    loadResourceData(r, &RnumResources);
    loadShelterData(s, &Snum);

    struct BST rBST;
      char rname[50];
      char searchLocation[30];
      int result;

    while(1)
    {
        printf("\nENTER THE CHOICE\n");
        printf("1-TO REPRESENT THE HISTORICAL DATA\n");
        printf("2-ALLOCTE RESOURES TO INCIDENTS\n");
        printf("3-SORT BY RESOURCE URGENCY(Merge sort)\n");
        printf("4-SORT BY INCIDENT NAME(Quick sort)\n");
        printf("5-SORT SAFETY PALCES BY THEIR SAFETY SCORE(Heap sort)\n");
        printf("6-SEARCH RESOUCE BY THEIR NAME(Kmp)\n");
        printf("7-DO U WANT TO KNOW YOUR RECENT CHOICE?\n");
        printf("8-SEARCH THE SAFER PLACE(Rabin karp)\n");
        printf("9-BSF BETWEEN THE INCIDENTS\n");
        printf("10-DO U WANNA GIVE ANY FEEDBACK?\n");
        printf("0-EXIT\n");

        scanf("%d",&choice);
             if(full(stk))
            {
                printf("STACK OVERFLOW\n");
            }
            else if(choice!=7)
                push(&stk,choice);
        switch(choice)
        {
        case 1:
            {
    struct hash ht;

        for (int i = 0; i < HSIZE; ++i) {
         ht.R[i] = NULL;
        }

    char *u = getenv("USERPROFILE");
    char file[MAX];
    strcpy(file, u);
    strcat(file, "\\Downloads\\historical_dataimp.txt");

    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    struct Incident R[MAX];
    int Rnum = 0;

    while (Rnum < MAX && fscanf(fp, "%d%s%s%d", &R[Rnum].incident_id, R[Rnum].inc_name, R[Rnum].city, &R[Rnum].dist) == 4)
    {
        inserthash(&ht, &R[Rnum]);
        Rnum++;
    }

    fclose(fp);
    displayhash(&ht);
    printf("TOTAL NUMBER OF RECORDS: %d\n", Rnum);

    int search_id;
    printf("Enter incident_id to search: ");
    scanf("%d", &search_id);

    struct Incident* result = search(&ht, search_id);
    if (result != NULL)
    {
        printf("Incident found:\n");
        printf("INCIDENT ID: %d\n", result->incident_id);
        printf("INCIDENT NAME: %s\n", result->inc_name);
        printf("CITY NAME: %s\n", result->city);
        printf("DISTANCE: %d\n", result->dist);
    }
    else
    {
        printf("Incident not found.\n");
    }
    }
    break;
        case 2:
    initBST(&rBST, r, RnumResources);
    alloctr(&R,RnumResources, Rnum, &rBST);
            break;
            case 3:
    mergeSort(r, 0, RnumResources - 1);
    printf("\nSorted Resources:\n");
    display(r, RnumResources);
            break;
            case 4:

    // Sort by city
    quicksort(R, 0, Rnum - 1);

    // Print sorted array
    for (int i = 0; i < Rnum; i++)
    {
        printf("%d %s %s %d\n", R[i].incident_id, R[i].inc_name,
              R[i].city, R[i].dist);
    }
                break;
            case 5:

    printf("Original Shelters:\n");
    for (int i = 0; i < Snum; i++)
        {
        printf("%s - Safety Score: %d\n", s[i].location, s[i].safescore);
    }
    heapSort(s, Snum);
    printf("\nShelters after Heap Sort:\n");
    for (int i = 0; i < Snum; i++)
    {
        printf("%s - Safety Score: %d\n", s[i].location, s[i].safescore);
    }
                break;
            case 6:
    printf("ENTER THE RESOURCE TO BE SEARCHED\n");
    scanf("%s",rname);

    struct Resource* result = searchr(r, RnumResources, rname);

    if (result != NULL) {
        printf("Resource found:\n");
        printf("ID: %d\nName: %s\nQuantity: %d\nUrgency: %d\n",
               result->resource_id, result->name, result->quantity, result->urgency);
    }
    else

    {
        printf("Resource not found.\n");
    }
                break;
            case 7:
                {
                    while(stk.top!=-1)
                    {
                        int data=pop(&stk);
                            struct HashTable htable;
    memset(&htable, 0, sizeof(struct HashTable)); // Initialize the hash table

    char* username = getenv("USERPROFILE");
    char filepath[MAX];
    strcpy(filepath, username);
    strcat(filepath, "\\Downloads\\choices.txt");

    FILE* fp = fopen(filepath, "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    struct hash_entry Records[MAX];
    int Rnum = 0;

    while (Rnum < MAX && fscanf(fp, "%d ", &Records[Rnum].num) == 1) {
        // Use fgets to read the character name with spaces
        fgets(Records[Rnum].name, MAX_NAME_LENGTH, fp);

        // Remove the newline character from the end
        Records[Rnum].name[strcspn(Records[Rnum].name, "\n")] = '\0';

        insertRecord(&htable, &Records[Rnum]);
        Rnum++;
    }

    fclose(fp);
    //displayHashTable(&ht);
   // printf("TOTAL NUMBER OF RECORDS: %d\n", Rnum);

    // Example of searching for a record
   // int searchKey;
    //printf("Enter incident number to search: ");
    //scanf("%d", &searchKey);

    struct hash_entry* foundRecord = searchRecord(&htable,data );

    if (foundRecord != NULL) {
        printf("Recent choices:\n");
        printf("Case: %d\n", foundRecord->num);
        printf("Functionality: %s\n", foundRecord->name);
    } else {
        printf("Record not found for incident number %d.\n",data);
    }
                    }
                }
                break;
            case 10:
        feedback();
        printf("THANK YOU SO MUCH FOR YOUR FEEDBACK\n");
    exit(0);
                break;
                case 8:
    printf("Enter the location to search: ");
    scanf("%s", searchLocation);

    result = searchl(s, Snum, searchLocation);

    if (result != -1)
        {
        printf("Shelter found at index %d\n", result);
    }
    else
     {
        printf("Shelter not found\n");
        printf("Do u want to add safer shelter to the file\n");
        char u[10];
        scanf("%s",u);
        if(strcmp(u,"YES")==0)
        {
        FILE *fp1 = fopen(u, "a");  // Open the file in append mode
        if (fp1 == NULL) {
            printf("Error opening shelters file.\n");
            return 1;
        }

        struct Shelter nw;

       printf("Enter the new shelter location: ");
    scanf("%s", nw.location);

        printf("Enter the safety score: ");
        scanf("%d", &nw.safescore);

        fprintf(fp1, "%s %d\n", nw.location, nw.safescore);

        fclose(fp1);
        }
    }
break;
case 9:{
    struct Incident Records[MAX];
    char *username = getenv("USERPROFILE");
    char filepath[MAX];
    strcpy(filepath, username);
    strcat(filepath, "\\Downloads\\historical_dataimp.txt");

    FILE *fp = fopen(filepath, "r");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    int Rnum = 0;

    while (Rnum < MAX && fscanf(fp, "%d%s%s%d", &Records[Rnum].incident_id, Records[Rnum].inc_name, Records[Rnum].city, &Records[Rnum].dist) == 4)
    {
        Rnum++;
    }

    fclose(fp);

    // Provided adjacency matrix
    int adjMatrix[10][10] = {
        {0, 1, 0, 1, 0, 1, 0, 0, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0, 0, 0},
        {0, 1, 0, 0, 0, 1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 1, 0, 0, 0, 1, 0, 0, 1, 0},
        {1, 0, 1, 0, 1, 0, 0, 0, 0, 1},
        {0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0, 1, 0, 1, 0},
        {0, 0, 0, 0, 1, 0, 0, 1, 0, 0},
        {1, 0, 0, 0, 0, 1, 0, 0, 0, 0}};

    // Copy data from Records to incidents
    struct Incident incidents[Rnum];
    for (int i = 0; i < Rnum; i++)
    {
        incidents[i] = Records[i];
    }


    int start;
    printf("Enter the index of the initial node for BFS: ");
    scanf("%d", &start);

    if (start < 0 || start >= Rnum)
    {
        printf("Invalid index. Exiting.\n");
        return 1;
    }

    printf("BFS starting from incident %d:\n", start);
    bfs(incidents, Rnum, start, adjMatrix);


}
break;
        case 0:
            printf("THANK YOU\n");
            exit(0);
        }
    }
}
