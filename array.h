#ifndef FINAL_ARRAY_H
#define FINAL_ARRAY_H

#include <iostream>

void arr() {
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int* ptr = arr;
    ptr += 3;
    std::cout << "4-й элемент массива: " << *ptr << std::endl;
}

#endif //FINAL_ARRAY_H
