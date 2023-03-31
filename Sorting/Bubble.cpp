#include <iostream>

void swap(int* a, int* b);
void BubbleSort(int* Arr, int size);
int main() 
{
    int array[] = { 3, 5, 6, 3, 2, 5 };

    BubbleSort(array, 6);

    for(int x : array) {
        std::cout << x << " ";
    }
    return 0;
}

void BubbleSort(int* Arr, int size) {
    for (int x=0; x < size; x++) 
    {
        for(int y=size-1; y > x; y--) // y > x so that y-1 > -1
        {
            if(Arr[y] < Arr[y-1])
                swap(&Arr[y], &Arr[y-1]);
        }
    }
}
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}