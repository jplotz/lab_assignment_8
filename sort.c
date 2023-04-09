#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

// heap helper functions
int parent(int idx)
{
    if (idx == 0)
        return 0;
    else if (idx % 2 == 0)
        return idx/2 - 1;
    return idx/2;
}

int leftChild(int idx)
{
    return idx*2 + 1;
}

int rightChild(int idx)
{
    return idx*2 + 2;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;

}

// this is the max-heapify function
void buildMaxHeap(int heap[], int n)
{
    int max = heap[0];
    int maxIdx = 0;
    int isMaxHeap = 0;

    for (int i = 1; i < n; i++) {
        if (heap[i] > max) {
            max = heap[i];
            maxIdx = i;
        }
    }

    // first step: move biggest element to front
    swap(&heap[0], &heap[maxIdx]);

    // remaining steps: swap if parent is smaller than child node
    while (!isMaxHeap) {
        isMaxHeap = 1;
        for (int i = 1; i < n; i++) {
            if (heap[parent(i)] < heap[i]) {
                isMaxHeap = 0;
                swap(&heap[parent(i)], &heap[i]);
            }
        }
    }
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
    buildMaxHeap(arr, n);

    for (int i = 0; i < n; i++) {
        swap(&arr[0], &arr[n-i-1]);
        buildMaxHeap(arr, n-i-1);
    }


}

void merge(int *finalarray, int finalarraylen, int *arr1, int len1, int *arr2, int len2)
{
    int pos1 = 0;
    int pos2 = 0;
    int smallestpos, smallestval;
    for (int i = 0; i < finalarraylen; i++)
    {
        if (pos1 >= len1)
        {
            finalarray[i] = arr2[pos2++];
        }
        else if (pos2 >= len2)
        {
            finalarray[i] = arr1[pos1++];
        }
        else if (arr1[pos1] <= arr2[pos2])
        {
            smallestval = arr1[pos1];
            smallestpos = pos1;
            pos1++;
            finalarray[i] = smallestval;
        }
        else if (arr2[pos2] <= arr1[pos1])
        {
            smallestval = arr2[pos2];
            smallestpos = pos2;
            pos2++;
            finalarray[i] = smallestval;
        }
    }
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
    if (l == r) // only one element
        return;

    int mid = (l + r) / 2;

    int leftLen = mid-l+1;
    int *leftSide = malloc(sizeof(int) * leftLen);
    extraMemoryAllocated += sizeof(int) * leftLen;
    if (leftSide == NULL) {
        printf("malloc error\n");
        return;
    }
    int rightLen = r-mid;
    int *rightSide = malloc(sizeof(int) * rightLen);
    if (rightSide == NULL) {
        printf("malloc error\n");
        return;
    }
    extraMemoryAllocated += sizeof(int) * rightLen;
    memcpy(leftSide, pData, leftLen*sizeof(int));
    memcpy(rightSide, pData+mid+1, rightLen*sizeof(int));

    mergeSort(leftSide, 0, leftLen-1);
    mergeSort(rightSide, 0, rightLen-1);
    merge(pData, r+1, leftSide, leftLen, rightSide, rightLen);
    free(leftSide);
    free(rightSide);
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
// NOTE: I changed this function to only print out
// valid array indexes on inputs with less than 200 elements
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100 && i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz && i>0;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}
