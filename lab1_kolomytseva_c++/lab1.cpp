#include <Windows.h>
#include <iostream>
#include <locale>
using namespace std;



// ФУНКЦИИ

// Функция ввода данных
void inputData(int** matrix, int rows, int cols) {
    cout << "\nВведите элементы матрицы (" << rows << "x" << cols << "):\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << "matrix[" << i << "][" << j << "] = ";
            cin >> matrix[i][j];
        }
    }
}

// Функция обработки (поиск и обмен строк)
void processMatrix(int** matrix, int rows, int cols, int& minRow, int& maxRow) {
    int minElement = matrix[0][0];
    int maxElement = matrix[0][0];
    minRow = 0;
    maxRow = 0;
    
    // Поиск минимального и максимального элементов
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] < minElement) {
                minElement = matrix[i][j];
                minRow = i;
            }
            if (matrix[i][j] > maxElement) {
                maxElement = matrix[i][j];
                maxRow = i;
            }
        }
    }
    
    cout << "\nМинимальный элемент: " << minElement << " (строка " << minRow << ")";
    cout << "\nМаксимальный элемент: " << maxElement << " (строка " << maxRow << ")";
    
    // Обмен строк местами
    if (minRow != maxRow) {
        int* temp = matrix[minRow];
        matrix[minRow] = matrix[maxRow];
        matrix[maxRow] = temp;
        cout << "\nСтроки " << minRow << " и " << maxRow << " поменяны местами!";
    }
}

// Функция вывода данных
void outputMatrix(int** matrix, int rows, int cols) {
    cout << "\n\nРезультат:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
}

// Функция освобождения памяти
void freeMemory(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

// ГЛАВНАЯ ФУНКЦИЯ

int main() {
    SetConsoleOutputCP(65001);  
    
    int rows, cols;
    
    // Ввод размерности матрицы
    cout << "Введите количество строк: ";
    cin >> rows;
    cout << "Введите количество столбцов: ";
    cin >> cols;
    
    // Выделение памяти в куче
    int** matrix = new int*[rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
    }
    
    // Вызов функций
    inputData(matrix, rows, cols);
    
    cout << "\nИсходная матрица:";
    outputMatrix(matrix, rows, cols);
    
    int minRow, maxRow;
    processMatrix(matrix, rows, cols, minRow, maxRow);
    
    outputMatrix(matrix, rows, cols);
    
    // Освобождение памяти
    freeMemory(matrix, rows);
    
    cout << "\nПамять освобождена. Программа завершена.";
    return 0;
}