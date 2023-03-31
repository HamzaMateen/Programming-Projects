#include <iostream>
typedef int index;

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

index minimum(int* Arr, int left, int size)
{
    int smallest = Arr[left];
    int smallest_index = left;

    for(int i=left; i<size; i++)
    {
        if(Arr[i] < smallest) 
        {
            smallest = Arr[i];
            smallest_index = i;
        } 
    }
    return smallest_index;
}

void SelectionSort(int* Arr, int size)
{
    // select the minimum as smallest 
    for (int o = 0; o < size; o++) 
    {
        for(int i=o; i < size; i++) 
        {
            int min_index = minimum(Arr, o, size);
            swap(&Arr[o], &Arr[min_index]);
        }
    }
}

int main(int argc, char const *argv[])
{
    int array[] = {-11, 3, 5, 2, 0, 0, -1, -53 };

    SelectionSort(array, 8);

    for(int x: array)
        std::cout << x << " ";

    return 0;
}
