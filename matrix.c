#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 20

int matA[MAX][MAX]; 
int matB[MAX][MAX]; 

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX]; 

typedef struct {
    int row;
    int col;
} MatrixCell;

void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the sum of the
// values at the coordinates of matA and matB.
void* computeSum(void* args) { // pass in the number of the ith thread
    MatrixCell* cell = (MatrixCell*)args;
    int row = cell->row;
    int col = cell->col;
    matSumResult[row][col] = matA[row][col] + matB[row][col];
    free(cell);  // Free the memory allocated for the cell
    return NULL;
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the difference of the
// values at the coordinates of matA and matB.
void* computeDiff(void* args) { // pass in the number of the ith thread
    MatrixCell* cell = (MatrixCell*)args;
    int row = cell->row;
    int col = cell->col;
    matDiffResult[row][col] = matA[row][col] - matB[row][col];
    free(cell);  // Free the memory allocated for the cell
    return NULL;
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the inner product
// of matA and matB.
void* computeProduct(void* args) { // pass in the number of the ith thread
    MatrixCell* cell = (MatrixCell*)args;
    int row = cell->row;
    int col = cell->col;
    matProductResult[row][col] = 0;
    for (int k = 0; k < MAX; k++) {
        matProductResult[row][col] += matA[row][k] * matB[k][col];
    }
    free(cell);  // Free the memory allocated for the cell
    return NULL;
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main(int argc, char *argv[]) {
    srand(time(0));  // Do Not Remove. Just ignore and continue below.
    
    // 0. Get the matrix size from the command line and assign it to MAX
    int size = atoi(argv[1]);
    
    // 1. Fill the matrices (matA and matB) with random values.
    fillMatrix(matA);
    fillMatrix(matB);
    
    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    
    // 3. Create pthread_t objects for our threads.
    pthread_t sumThreads[MAX][MAX];
    pthread_t diffThreads[MAX][MAX];
    pthread_t productThreads[MAX][MAX];

    
    // 4. Create a thread for each cell of each matrix operation.
    // 
    // You'll need to pass in the coordinates of the cell you want the thread
    // to compute.
    // 
    // One way to do this is to malloc memory for the thread number i, populate the coordinates
    // into that space, and pass that address to the thread. The thread will use that number to calcuate 
    // its portion of the matrix. The thread will then have to free that space when it's done with what's in that memory.
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // Sum threads
            MatrixCell* cell = malloc(sizeof(MatrixCell));
            cell->row = i;
            cell->col = j;
            pthread_create(&sumThreads[i][j], NULL, computeSum, (void*)cell);

            // Difference threads
            cell = malloc(sizeof(MatrixCell));
            cell->row = i;
            cell->col = j;
            pthread_create(&diffThreads[i][j], NULL, computeDiff, (void*)cell);

            // Product threads
            cell = malloc(sizeof(MatrixCell));
            cell->row = i;
            cell->col = j;
            pthread_create(&productThreads[i][j], NULL, computeProduct, (void*)cell);
        }
    }
    
    // 5. Wait for all threads to finish.
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            pthread_join(sumThreads[i][j], NULL);
            pthread_join(diffThreads[i][j], NULL);
            pthread_join(productThreads[i][j], NULL);
        }
    }
    
    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    return 0;
  
}