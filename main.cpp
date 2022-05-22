//https://www.matburo.ru/mart_sub.php?p=art_lp_215
//https://math.semestr.ru/simplex/integer.php
#include <iostream>
#include "rationalnum/RationalNum.h"
#include <vector>
#include <fstream>
#include <cmath>

//#define DEBUG

std::string path = "/Users/salimovbogdan/CLionProjects/2semextra/input/input.txt";

// найти дробную часть числа
RationalNum fraction(RationalNum a) {
    RationalNum b = (int) floor(a.getNumerator() / a.getDenominator());
    return ((a - b) < 0 ? a - b + 1 : a - b);
}

// дробное ли число
bool isFrac(RationalNum a) {
    if (a == RationalNum(0)) return false;
    if (fraction(a) != RationalNum(0)) return true;
    return false;
}

// есть ли
bool isColFrac(std::vector<std::vector<RationalNum>> &arr) {
    for (int i = 1; i < arr.size() - 1; ++i) {
        if (isFrac(arr[i][arr[0].size() - 1])) {
            return true;
        }
    }
    return false;
}

// вывести матрицу
void print_m(std::vector<std::vector<RationalNum>> &arr) {
    std::cout << "------------------------------------" << std::endl;
    for (int i = 0; i < arr.size(); ++i) {
        for (int j = 0; j < arr[0].size(); ++j) {
            std::cout << arr[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << "------------------------------------" << std::endl;
}

// поиск столбца с минимальным элементом в последней строке
int find_min_col(std::vector<std::vector<RationalNum>> &arr) {
    int index;
    RationalNum temp(1100000);
    for (int i = 1; i < arr.size() + arr[0].size() - 2; ++i) {
        if (arr[arr.size() - 1][i] < temp) {
            temp = arr[arr.size() - 1][i];
            index = i;
        }
    }
    return index;
}

// найти элемент с наибольшей дробной частью в последнем столбце
int find_max_row_gomory(std::vector<std::vector<RationalNum>> &arr) {
    int index;
    RationalNum temp(1100000);
    for (int i = 1; i < arr.size() - 1; ++i) {
        if (fraction(arr[i][arr[0].size() - 1]) < temp && arr[i][arr[0].size() - 1] > RationalNum(0)) {
            temp = fraction(arr[i][arr[0].size() - 1]);
            index = i;
        }
    }
    return index;
}

// поиск строки с минимальным результатом(>0) деления правого столбца на столбец с минимальным элементом
int find_min_row(std::vector<std::vector<RationalNum>> &arr, int min_col_index) {
    int index;
    RationalNum temp(1100000);
    for (int i = 1; i < arr.size() - 1; ++i) {
        if (arr[i][arr[1].size() - 1] / arr[i][min_col_index] < temp &&
            arr[i][arr[1].size() - 1] / arr[i][min_col_index] > 0) {
            temp = arr[i][arr[1].size() - 1] / arr[i][min_col_index];
            index = i;
        }
    }
    return index;
}

// ищем минимальный элемент, получившийся в результате
// деления последней строки на предпоследнюю строку, принадлежащую новой переменной
// (при делении на ноль пропускаем)
int find_min_col_gomory(std::vector<std::vector<RationalNum>> &arr) {
    int index;
    RationalNum temp(1100000);
    for (int i = 1; i < arr[0].size() - 2; ++i) {
        if (arr[arr.size() - 2][i] != RationalNum(0) &&
            arr[arr.size() - 1][i] / arr[arr.size() - 2][i] < temp) {
            temp = arr[arr.size() - 1][i] / arr[arr.size() - 2][i];
            index = i;
        }
    }
    return index;
}

// добавление пустого ряда для базисной переменной
void add_row(std::vector<std::vector<RationalNum>> &arr) {
    arr.emplace_back(arr[0].size());
    std::swap(arr[arr.size() - 1], arr[arr.size() - 2]);
}

// добавление пустого столбца для базисной переменной
void add_col(std::vector<std::vector<RationalNum>> &arr) {
    for (int i = 0; i < arr.size(); ++i) {
        arr[i].push_back(RationalNum(0));
        std::swap(arr[i][arr[0].size() - 1], arr[i][arr[0].size() - 2]);
    }
}


// проверка остались ли еще отрицательные элементы в последней строке
bool check(std::vector<std::vector<RationalNum>> &arr) {
    for (auto &i: arr[arr.size() - 1]) {
        if (i < 0) return true;
    }
    return false;
}

// вычитание строк с целью обнулить столбец
void subtract_row(std::vector<std::vector<RationalNum>> &arr, int min_row_index, int min_col_index) {
    RationalNum ttt = arr[min_row_index][min_col_index];
    for (int j = 1; j < arr[0].size(); ++j) {
        arr[min_row_index][j] = arr[min_row_index][j] / ttt;
    }
    for (int i = 1; i < arr.size(); ++i) {
        RationalNum temp = arr[i][min_col_index];
        if (i != min_row_index) {
            for (int j = 1; j < arr[0].size(); ++j) {
                arr[i][j] -= arr[min_row_index][j] * temp;
            }
        }
    }


}

int main() {
    ////НАЧАЛО ЧТЕНИЯ ДАННЫХ
    std::ifstream fin;
    fin.open(path);
//    if (fin.is_open()) std::cout <q< "Opened" << std::endl;
    int n, m;
    fin >> n >> m;
    std::vector<std::vector<RationalNum>> arr(m + 2);
    for (int i = 0; i < n + m + 1; ++i) {
        arr[0].push_back(RationalNum(i));
    }

    for (int i = 1; i < m + 1; ++i) {
        arr[i].push_back(RationalNum(i + n));
        for (int j = 1; j < n + 1; ++j) {
            std::string temp;
            fin >> temp;
            RationalNum temp1;
            temp1.read(temp, temp1);
            arr[i].push_back(temp1);
        }
        for (int j = 1; j < m + 1; ++j) {
            if (i == j) arr[i].push_back(RationalNum(1));
            else arr[i].push_back(RationalNum(0));
        }
    }
    arr[0].push_back(RationalNum(0));
    for (int i = 1; i < m + 1; ++i) {
        std::string temp;
        fin >> temp;
        RationalNum temp1;
        temp1.read(temp, temp1);
        arr[i].push_back(temp1);
    }
    arr[m + 1].push_back(RationalNum(0));
    for (int i = 1; i < n + 1; ++i) {
        std::string temp;
        fin >> temp;
        RationalNum temp1;
        temp1.read(temp, temp1);
        arr[m + 1].push_back(RationalNum(-temp1));
    }
    for (int i = n + 1; i < n + m + 2; ++i) {
        arr[m + 1].push_back(RationalNum(0));
    }
#ifdef DEBUG
    print_m(arr);
#endif
////КОНЕЦ ЧТЕНИЯ ДАННЫХ


//// ПРИМЕР ТАБЛИЦЫ:
//// 0	1	2	3	4	5	6	7	8	0
//// 6	1	2	3	5	6	1	0	0	102
//// 7	8	2	30	4	3	0	1	0	90
//// 8	2	1	23	9	4	0	0	1	30
//// 0	-2	-3	-1	-49	-4	0	0	0	0
//// первая строка свободные и базисные переменные
//// первый столбец базисные перемнные
//// крайний правый столбец - ограничения (<=b)
//// последняя строка - вектор с (c*x -> max)
//// ПРИ НЕОБХОДИМОСТИ МОЖНО ПРОПИСАТЬ СВЕРХУ #define DEBUG

//// СИМПЛЕКС
    while (check(arr)) { // проверка: остались ли еще отрицательные элементы в последней строке
        int min_col_index = find_min_col(arr); // столбец с минимальных элементом в последней строке
        int min_row_index = find_min_row(arr, min_col_index); // строка с минимальным результатом(>0) деления
        // крайнего правого столбца на столбец с минимальным элементом в последней строке
        arr[min_row_index][0] = min_col_index; // замена номера переменной в левом столбце
        subtract_row(arr, min_row_index, min_col_index); // вычитание строк, зануление элементов
        // под минимальным столбцом
#ifdef DEBUG
        print_m(arr);
#endif
    }

    //// ГОМОРИ

    while (isColFrac(arr)) {
        // Добавление новой базисной переменной
        add_row(arr);
        add_col(arr);
        arr[0][arr[0].size() - 2] = RationalNum((int) arr[0].size() - 2);
        // ищем элемент с наибольшей дробной частью в последнем столбце
        int max_row_index = find_max_row_gomory(arr);
        // заполняем строку с новой переменной и меняем знаки в последней строке
        for (int i = 1; i < arr[0].size(); ++i) {
            arr[arr.size() - 2][i] = -fraction(arr[max_row_index][i]);
            arr[arr.size() - 1][i] = -arr[arr.size() - 1][i];
        }
        // ставим единицу на пересечении строки и столбца новой переменной
        arr[arr.size() - 2][arr[0].size() - 2] = 1;

        // ищем минимальный элемент, получившийся в результате
        // деления последней строки на предпоследнюю строку, принадлежащую новой переменной
        // (при делении на ноль пропускаем)
        int min_col_index = find_min_col_gomory(arr);

        // заменяем в первом столбце новую переменную на базисную
        arr[arr.size() - 2][0] = arr[0][min_col_index];
        // Обнуляем столбец по Гауссу
        subtract_row(arr, arr.size() - 2, min_col_index);
#ifdef DEBUG
        print_m(arr);
#endif
    }

    //// ВЫВОД РЕЗУЛЬТАТА И ВЕКТОРА Х
    std::cout << "Result:" << ((arr[arr.size() - 1][arr[0].size() - 1] > RationalNum(0)) ?
                               arr[arr.size() - 1][arr[0].size() - 1] :
                               -1 * arr[arr.size() - 1][arr[0].size() - 1]) << std::endl;
    std::vector<RationalNum> x(n);
    for (int i = 1; i < arr.size() - 1; ++i) {
        if (arr[i][0] <= n) {
            x[arr[i][0].getNumerator() - 1] = arr[i][arr[0].size() - 1];
        }
    }
    std::cout << "X:(" << x[0];
    for (int i = 1; i < x.size(); ++i) {
        std::cout << ", " << x[i];
    }
    std::cout << ')' << std::endl;
}