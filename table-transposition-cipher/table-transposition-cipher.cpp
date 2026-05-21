#include <iostream>
#include <windows.h>
#include <string>
#include <cctype>
#include <algorithm>
#include <vector>
#include <sstream>

using namespace std;

void Menu() {
    cout << "1. Шифрование" << endl;
    cout << "2. Расшифрование" << endl << endl;
    cout << "Выберите опцию (1-2): ";
}

//Проверка ключа
bool isValidKey(const string& key) {
    if (key.empty() || key.length() > 9) {
        cout << "Ключ должен содержать от 1 до 9 неповторяющихся цифр (без 0)!" << endl;
        return false;
    }
    for (char c : key) {
        if (!isdigit(c) || c == '0') {
            cout << "Только цифры от 1 до 9!" << endl;
            return false;
        }
    }
    for (size_t i = 0; i < key.length(); i++)
        for (size_t j = i + 1; j < key.length(); j++)
            if (key[i] == key[j]) {
                cout << "Цифры не должны повторяться!" << endl;
                return false;
            }
    return true;
}

//Сортировка ключа по возрастанию
string sortKey(const string& key) {
    string sorted = key;
    sort(sorted.begin(), sorted.end());
    return sorted;
}

//Шифрование
string encrypt(const string& text, const string& key) {
    string result;
    int cols = key.length();
    string sortedKey = sortKey(key);

    for (char sortedDigit : sortedKey) {
        size_t col = key.find(sortedDigit);
        for (size_t i = col; i < text.length(); i += cols)
            result += text[i];
    }
    return result;
}

//Расшифрование
string decrypt(const string& text, const string& key) {
    int cols = key.length();
    int rows = text.length() / cols;
    string sortedKey = sortKey(key);

    //Таблица
    vector<vector<char>> table(rows, vector<char>(cols, ' '));
    int pos = 0;
    for (char sortedDigit : sortedKey) {
        size_t col = key.find(sortedDigit);
        for (int row = 0; row < rows; row++) {
            table[row][col] = text[pos++];
        }
    }
    string result;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            result += table[row][col];
        }
    }

    //Удаление лишних символов
    result.erase(remove(result.begin(), result.end(), '_'), result.end());
    result.erase(remove(result.begin(), result.end(), '*'), result.end());
    return result;
}

//Добавление символов до кратности длине ключа
string padText(string text, const string& key) {
    if (text.length() % key.length() != 0)
        text.append(key.length() - text.length() % key.length(), '_');
    return text;
}

//Вывод таблицы
void printTable(const string& text, const string& key) {
    cout << endl << "Таблица:" << endl << endl;

    for (char c : key) cout << c << " ";
    cout << endl;

    for (size_t i = 0; i < text.length(); i++) {
        cout << text[i] << " ";
        if ((i + 1) % key.length() == 0) cout << endl;
    }
    cout << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Menu();
    string choice;
    getline(cin, choice);

    if (choice != "1" && choice != "2") {
        cout << endl << "Надо попасть либо по 1, либо по 2 :(" << endl;
        return 0;
    }

    cout << "------------------------------------------------------------------------------" << endl;

    string text, key, block;
    cout << "Введите текст: ";
    getline(cin, text);

    cout << "Введите ключ (неповторяющимися цифрами без 0): ";
    getline(cin, key);
    if (!isValidKey(key)) return 0;

    cout << "Номера блокированных ячеек (с 1 до " << text.length() << ") через пробелы: ";
    getline(cin, block);

    //Обработка блокированных ячеек
    vector<int> indexblock;
    istringstream iss(block);
    int index;
    while (iss >> index) {
        if (index < 1 || index > static_cast<int>(text.length())) {
            cout << "Числа не должны быть вне диапазона!" << endl;
            return 0;
        }
        indexblock.push_back(index);
    }

    //Вставка * в блокированные ячейки
    for (int i = indexblock.size() - 1; i >= 0; i--)
        text.insert(indexblock[i] - 1, "*");

    cout << "------------------------------------------------------------------------------" << endl;
    if (choice == "1") {
        text = padText(text, key);
        printTable(text, key);
        string encrypted = encrypt(text, key);
        cout << endl << "Зашифрованный текст: " << encrypted << endl;
    }
    else {
        string decrypted = decrypt(text, key);
        cout << endl << "Расшифрованный текст: " << decrypted << endl;
    }
    return 0;
}