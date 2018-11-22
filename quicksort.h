//
// Created by Andrew Gonzalez on 11/20/18.
//

#ifndef QUICKSORT_H
#define QUICKSORT_H

void quicksort(int sortMe[], int leftIndex, int rightIndex, int (*partitionFunc)(int*,int,int));
int lomutoPartition(int partArr[], int leftIndex, int rightIndex);
int hoarePartition(int partArr[], int leftIndex, int rightIndex);
void swap(int arr[], int currentIndex, int swapIndex);

#endif //QUICKSORT_H
