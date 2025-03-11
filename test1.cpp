#include <iostream>
#include <vector>

int main() {
    // Исходный вектор
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    // Вектор, в который будем добавлять элементы
    std::vector<int> targetVec = {10, 11, 12};

    // Проверка, что в исходном векторе достаточно элементов
    if (vec.size() >= 3) {

        // Вырезаем первые три элемента
        auto start = vec.begin();
        auto end = vec.begin() + 3;

        // Вставляем вырезанные элементы в начало целевого вектора в обратном порядке
        targetVec.insert(targetVec.begin(), std::make_reverse_iterator(end), std::make_reverse_iterator(start));

        // Удаляем вырезанные элементы из исходного вектора
        vec.erase(start, end);

        // Выводим результат
        std::cout << "Source vector after erasing: ";
        for (int elem : vec) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;

        std::cout << "Target vector after inserting: ";
        for (int elem : targetVec) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "Not enough elements in the source vector!" << std::endl;
    }

    return 0;
}