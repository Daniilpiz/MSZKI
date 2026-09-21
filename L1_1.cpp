/**
 * @file task1_shift.cpp
 * @brief Implementation of a simple shift cipher (Caesar cipher).
 */

#include <iostream>
#include <string>
#include <limits>
#include <io.h>
#include <fcntl.h>

using namespace std;

const wstring DEFAULT_ALPHABET = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя0123456789.,!?-;:() ";

wstring Encrypt(const wstring& text, int K, const wstring& alphabet) {
    wstring result = L"";
    int N = (int)alphabet.length();
    for (wchar_t c : text) {
        size_t pos = alphabet.find(c);
        if (pos != wstring::npos) {
            result += alphabet[(pos + K % N + N) % N];
        }
        else {
            result += c;
        }
    }
    return result;
}

wstring Decrypt(const wstring& text, int K, const wstring& alphabet) {
    return Encrypt(text, -K, alphabet);
}

// Заменяем main на wmain для нативной работы с юникод-аргументами (wchar_t) в Windows
int wmain(int argc, wchar_t* argv[]) {
    // Жёстко переводим потоки ввода/вывода консоли в режим UTF-16
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);

    // Запуск через консоль: program.exe [режим] [сдвиг] [текст] [алфавит]
    if (argc >= 4) {
        int mode = _wtoi(argv[1]);
        int shift = _wtoi(argv[2]);
        wstring text = argv[3];
        wstring alphabet = (argc >= 5) ? argv[4] : DEFAULT_ALPHABET;

        if (mode == 1) {
            wcout << L"Result: " << Encrypt(text, shift, alphabet) << endl;
        }
        else if (mode == 2) {
            wcout << L"Result: " << Decrypt(text, shift, alphabet) << endl;
        }
        return 0;
    }

    // Интерактивный режим
    wcout << L"--- Interactive Mode: Shift Cipher ---\n";
    wcout << L"Enter text: ";
    wstring text;
    getline(wcin, text);

    wcout << L"Select mode (1 - Encrypt, 2 - Decrypt): ";
    int mode;
    wcin >> mode;

    wcout << L"Enter shift (integer): ";
    int shift;
    wcin >> shift;

    if (mode == 1) {
        wcout << L"Ciphertext: " << Encrypt(text, shift, DEFAULT_ALPHABET) << endl;
    }
    else {
        wcout << L"Plaintext: " << Decrypt(text, shift, DEFAULT_ALPHABET) << endl;
    }

    wcout << L"\nНажмите Enter, чтобы выйти...";
    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
    wcin.get();

    return 0;
}