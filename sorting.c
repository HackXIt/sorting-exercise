/* SORTING PROJECT.c
 *   by Nikolaus Rieder
 *
 * Created:
 *   1/23/2021, 8:14:48 PM
 * Last edited:
 *   1/27/2021, 11:39:30 AM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Program which sorts an integer Array in three different ways
 *   (Mergesort, Quicksort, Shake-sort) and measures the performance of
 *   each sorting algorithm.
**/

/* NOTE Sorting-Requirements
This program requires three sorting functions:
 - Mergesort
 - Quicksort
 - Shakesort

I also need a seperate function to call each individual sort and time it.
We'll call that test_sort()
That function will require to use a function-pointer to operate.
For this to properly work, each sorting function needs the same parameters.
*/

/* NOTE Execution-Requirements
In this program I'm supposed to generate random numbers between -32.786 and 32.767.
Initialization of that should be done in a seperate function: init_random_array()
"Use pointers in an appropriate way"
The sorting should be done with three differently sized arrays: 3000, 12000, 24000
I also need an array (actually matrix) to store the timing results.
*/

/*--- COMMON LIBRARIES ---*/
#include <stdio.h>   // Come on.. which program doesn't need this library?
#include <stdbool.h> // just to handle return of certain functions more easily
#include <stdlib.h>  // random number functions
#include <time.h>    // To measure execution time of the program & sorting-functions.

/*--- CUSTOM LIBRARIES ---*/

/*--- MACROS ---*/
// #define DEBUG
// #define SIZE_A 3000
// #define SIZE_B 12000
// #define SIZE_C 24000
#define ALGORITHMS 3
#define ARRAYS 3

short *init_random_array(short size);
void clear_arrays(short *arr1, short *arr2, short *arr3);
void print_divider(FILE *f_stream);
void print_array(short *arr, short size, FILE *f_stream);

/*--- Quick-Sort ---*/
void QuickSort(short *arr, short start, short end);
short Partition(short *arr, short start, short end);
void swap(short *a, short *b);

/* ---- MERGE-SORT ---- */
void MergeSort(short *arr, short start, short end);
void merge(short *arr, short start, short mid, short end);

/* ---- SHAKER-SORT ---- */
// Or also called: Bidirectional Bubble-Sort or BiDi-Bubble-Sort
void ShakerSort(short *arr, short start, short end);

double call_Sorting(void (*func)(short *a, short s, short e), short *arr, short start, short end);

int main()
{
    // short *arr1 = init_random_array(SIZE_A);
    // short *arr2 = init_random_array(SIZE_B);
    // short *arr3 = init_random_array(SIZE_C);

    // QuickSort(arr1, 0, (SIZE_A - 1));
    // QuickSort(arr2, 0, (SIZE_B - 1));
    // QuickSort(arr3, 0, (SIZE_C - 1));

    // MergeSort(arr1, 0, (SIZE_A - 1));
    // MergeSort(arr2, 0, (SIZE_B - 1));
    // MergeSort(arr3, 0, (SIZE_C - 1));

    // ShakerSort(arr1, 0, (SIZE_A - 1));
    // ShakerSort(arr2, 0, (SIZE_B - 1));
    // ShakerSort(arr3, 0, (SIZE_C - 1));
    printf("A more detailed insight on the execution can be found in the created 'execution-statistic.txt'-File\n");
    FILE *f_statistics = fopen("execution-statistic.txt", "w");

    short *arrays[ARRAYS];
    short array_sizes[ARRAYS] = {3000, 12000, 24000};
    // short sizes = sizeof(array_sizes) / sizeof(array_sizes[0]);
    for (short i = 0; i < ARRAYS; i++)
    {
        arrays[i] = init_random_array(array_sizes[i]);
    }

    void (*funcs[ALGORITHMS])(short *a, short s, short e) = {QuickSort,
                                                             MergeSort,
                                                             ShakerSort};

    char *algorithms[ALGORITHMS] = {"QuickSort", "MergeSort", "ShakerSort"};

    double execution_times[ARRAYS][ALGORITHMS];

    /* Test sorting Algorithms with different Array-Sizes */
    for (short i = 0; i < ARRAYS; i++)
    {
        for (short j = 0; j < ALGORITHMS; j++)
        {
            // printf("\nSorting Array %hi (%hi) with Algorithm %hi (%s)\n", (i + 1), array_sizes[i], (j + 1), algorithms[j]);
            short arr_cpy[array_sizes[i]]; // Copy original array, to have equal conditions for each algorithm
            for (short k = 0; k < array_sizes[i]; k++)
            {
                arr_cpy[k] = arrays[i][k];
            }
            printf("Testing execution of %s with Array %hi (%hi)...\n", algorithms[j], (i + 1), array_sizes[i]);
            execution_times[i][j] = call_Sorting(funcs[j], (short *)&arr_cpy, 0, (array_sizes[i] - 1));
            // printf("Array was sorted in %f seconds\n", execution_times[i][j]);
        }
    }

    /* Output arrays in pretty format to statistics file */
    // Due to Array-Size, the output for CLI is commented
    for (short i = 0; i < ARRAYS; i++)
    {
        // printf("\nUnsorted Array %hi (%hi):\n", (i + 1), array_sizes[i]);
        fprintf(f_statistics, "Unsorted Array %hi (%hi):\n", (i + 1), array_sizes[i]);
        // print_array(arrays[i], array_sizes[i], NULL);
        print_array(arrays[i], array_sizes[i], f_statistics);
        QuickSort(arrays[i], 0, (array_sizes[i] - 1)); // Need to sort each original array once for printing. (QuickSort is fastest, so used here)
        // printf("\nSorted Array %hi (%hi):\n", (i + 1), array_sizes[i]);
        fprintf(f_statistics, "Sorted Array %hi (%hi):\n", (i + 1), array_sizes[i]);
        // print_array(arrays[i], array_sizes[i], NULL);
        print_array(arrays[i], array_sizes[i], f_statistics);
    }
    printf("\nExecution statistics of sorting algorithms:\n");
    fprintf(f_statistics, "\nExecution statistics of sorting algorithms:\n");
    printf("ALGORITHM\t|\tSIZE\t|\tTIME\t\t|\tSIZE\t|\tTIME\t\t|\tSIZE\t|\tTIME\t\t|\n");
    fprintf(f_statistics, "ALGORITHM\t|\tTIME\t\t|\tSIZE\t|\tTIME\t\t|\tSIZE\t|\tTIME\t\t|\n");
    /* Output Execution-Statistics to output & statistics file */
    for (short i = 0; i < ALGORITHMS; i++)
    {
        printf("%s\t|", algorithms[i]);
        fprintf(f_statistics, "%s\t|", algorithms[i]);
        for (short j = 0; j < ARRAYS; j++)
        {
            printf("\t%hi\t|\t%f\t|", array_sizes[j], execution_times[i][j]);
            fprintf(f_statistics, "\t%hi\t|\t%f\t|", array_sizes[j], execution_times[i][j]);
        }
        printf("\n");
        fprintf(f_statistics, "\n");
    }
    clear_arrays(arrays[0], arrays[1], arrays[2]);
    return EXIT_SUCCESS;
}

short *init_random_array(short size)
{
    short *newArray = calloc(size, sizeof(short)); // Range of 65536 is the size of short
    if (newArray == NULL)
    {
        fprintf(stderr, "Couldn't allocate memory for random-integer-array.\n");
        exit(EXIT_FAILURE);
    }
    srand(time(NULL));
    printf("Creating new array of size %u...\n", size);
    for (short i = 0; i < size; i++)
    {
        newArray[i] = (rand() % 65536) + (-32786); // Random number between -32786 and 32787
    }
#ifdef DEBUG
    print_array(newArray, size, NULL);
#endif
    return newArray;
}

void print_array(short *arr, short size, FILE *f_stream)
{
    if (f_stream == NULL)
    {
        print_divider(NULL);
        for (short i = 0; i < size; i++)
        {
#ifdef DEBUG
            if ((i % 5) == 0)
            {
                printf("\n[%5zu] = %6d", i, arr[i]);
            }
            else
            {
                printf("\t[%5zu] = %6d", i, arr[i]);
            }
#endif
#ifndef DEBUG
            if ((i % 15) == 0)
            {
                printf("\n\t%6d", arr[i]);
            }
            else
            {
                printf("\t%6d", arr[i]);
            }
#endif
        }
        print_divider(NULL);
    }
    else
    {
        print_divider(f_stream);
        for (short i = 0; i < size; i++)
        {
#ifdef DEBUG
            if ((i % 5) == 0)
            {
                fprintf(f_stream, "\n[%5zu] = %6d", i, arr[i]);
            }
            else
            {
                fprintf(f_stream, "\t[%5zu] = %6d", i, arr[i]);
            }
#endif
#ifndef DEBUG
            if ((i % 15) == 0)
            {
                fprintf(f_stream, "\n\t%6d", arr[i]);
            }
            else
            {
                fprintf(f_stream, "\t%6d", arr[i]);
            }
#endif
        }
        print_divider(f_stream);
    }
}

void print_divider(FILE *f_stream)
{
    if (f_stream == NULL)
    {
        printf("\n|");
        for (short i = 0; i < 130; i++)
        {
            printf("-");
        }
        printf("|\n");
    }
    else
    {
        fprintf(f_stream, "\n|");
        for (short i = 0; i < 130; i++)
        {
            fprintf(f_stream, "-");
        }
        fprintf(f_stream, "|\n");
    }
}

double call_Sorting(void (*func)(short *a, short s, short e), short *arr, short start, short end)
{
    clock_t start_time;
    clock_t end_time;
    start_time = clock();
    (*func)(arr, start, end);
    end_time = clock();
    return (double)(end_time - start_time) / CLOCKS_PER_SEC;
}

void clear_arrays(short *arr1, short *arr2, short *arr3)
{
    if (arr1 != NULL)
    {
        free(arr1);
    }
    if (arr2 != NULL)
    {
        free(arr2);
    }
    if (arr3 != NULL)
    {
        free(arr3);
    }
}

/* ---- QUICK-SORT ---- */
// NOTE Helpful Resource from programiz.com
// https://www.programiz.com/dsa/quick-sort
void QuickSort(short *arr, short start, short end)
{
    if (start < end)
    {
        short p = Partition(arr, start, end);
        QuickSort(arr, start, (p - 1));
        QuickSort(arr, (p + 1), end);
    }
}
short Partition(short *arr, short start, short end)
{
    short pivot = arr[end];
    short pIndex = (start - 1);
    for (short i = start; i < end; i++)
    {
        if (arr[i] < pivot)
        {
            pIndex++;
            swap(&arr[pIndex], &arr[i]);
        }
    }
    swap(&arr[pIndex + 1], &arr[end]);
    return (pIndex + 1);
}
void swap(short *a, short *b)
{
    short tmp = *a;
    *a = *b;
    *b = tmp;
}

/* ---- MERGE-SORT ---- */
// NOTE Helpful Resource from programiz.com
// https://www.programiz.com/dsa/merge-sort
void MergeSort(short *arr, short start, short end)
{
    if (start < end)
    {
        short mid = (start + end) / 2;
        MergeSort(arr, start, mid);
        MergeSort(arr, (mid + 1), end);
        merge(arr, start, mid, end);
    }
}
void merge(short *arr, short start, short mid, short end)
{
    short index_L = start;
    short index_R = (mid + 1);
    short index_temp = start;
    short temp[end + 1]; // FIXME This is wasting space and not the correct way to implement the algorithm
    // But I didn't have the time to implement this properly and currently it works (but sadly wastes local storage)
    // Correct implementation would be to use left & right sub-arrays, with proportional size
    while (index_L <= mid && index_R <= end)
    {
        if (arr[index_L] <= arr[index_R])
        {
            temp[index_temp] = arr[index_L];
            index_L++;
            index_temp++;
        }
        else
        {
            temp[index_temp] = arr[index_R];
            index_R++;
            index_temp++;
        }
    }
    while (index_L <= mid)
    {
        temp[index_temp] = arr[index_L];
        index_L++;
        index_temp++;
    }
    while (index_R <= end)
    {
        temp[index_temp] = arr[index_R];
        index_R++;
        index_temp++;
    }
    for (short i = start; i <= end; i++)
    {
        arr[i] = temp[i];
    }
}

/* ---- SHAKER-SORT ---- */
// NOTE Helpful Resource from programiz.com / Wikipedia
// https://www.programiz.com/dsa/bubble-sort  -- Bubble-Sort is foundation for bidirectional bubble sort
// https://en.wikipedia.org/wiki/Cocktail_shaker_sort
void ShakerSort(short *arr, short start, short end)
{
    bool swapped = true;
    while (swapped)
    {
        swapped = false; // Reset for first-stage

        // First stage, going from left to right
        for (short i = start; i < end; i++)
        {
            if (arr[i] > arr[i + 1])
            {
                swap(&arr[i], &arr[i + 1]);
                swapped = true;
            }
        }

        if (!swapped)
        {
            break;
        }
        end--;

        swapped = false; // Reset for second-stage

        // Second stage, going from right to left
        for (short i = end - 1; i >= start; i--)
        {
            if (arr[i] > arr[i + 1])
            {
                swap(&arr[i], &arr[i + 1]);
                swapped = true;
            }
        }

        start++;
    }
}