#include <iostream>
#include <cassert>

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void InsertionSort(int* Arr, int size) 
{
    for(int i=1; i<size; i++)
    {
        int current_elem = Arr[i];
        for(int j = i-1; j >= 0; j--) 
        {
            if(Arr[j] > current_elem) 
            {
                swap(&Arr[j], &Arr[i]); 
                break;
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    int array[] = { 12, 5, 11, 15, 51, 211, 310 };
    InsertionSort(array, 7);
    for(int x: array)
        std::cout << x << " ";


    return 0;
}
