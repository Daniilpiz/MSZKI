#include <iostream>
#include <string>
#include <locale>
#include <cstdlib>
#include <limits>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

using namespace std;

/**
 * @brief Default alphabet used if the user does not provide one.
 * @details Includes Russian Cyrillic letters (both cases), digits, and punctuation marks.
 */
const wstring DEFAULT_ALPHABET = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя0123456789.,!?-;:() ";

/**
 * @brief Converts a standard ANSI string (char*) to a wide string (wstring).
 * @details Uses CP_ACP (Active Code Page) for native Windows console compatibility.
 *
 * @param str The standard string to convert.
 * @return The converted wide string.
 */
wstring s2ws(const string& str) {
    if (str.empty()) return wstring();

    int size_needed = MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), NULL, 0);
    wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);

    return wstrTo;
}

/**
 * @brief Encrypts the given text using a shift cipher.
 * @details Calculates the new character position using the formula: Y = (X + K) mod N.
 *
 * @param text The plain text to encrypt.
 * @param K The shift value (integer).
 * @param alphabet The alphabet string used for shifting.
 * @return The resulting encrypted cipher text.
 */
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

/**
 * @brief Decrypts the given text using a shift cipher.
 *
 * @param text The cipher text to decrypt.
 * @param K The shift value (integer) originally used for encryption.
 * @param alphabet The alphabet string used for shifting.
 * @return The resulting decrypted plain text.
 */
wstring Decrypt(const wstring& text, int K, const wstring& alphabet) {
    return Encrypt(text, -K, alphabet);
}

/**
 * @brief Main entry point of the program.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return 0 on successful execution.
 */
int main(int argc, char* argv[]) {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    //SetConsoleCP(1251);           
    //SetConsoleOutputCP(1251);     
    //setlocale(LC_ALL, "Russian"); 

    // Command line mode: program.exe [mode: 1-encrypt, 2-decrypt] [shift] [text] [alphabet (optional)]
    if (argc >= 4) {
        int mode = stoi(argv[1]);
        int shift = stoi(argv[2]);
        wstring text = s2ws(argv[3]);
        wstring alphabet = (argc >= 5) ? s2ws(argv[4]) : DEFAULT_ALPHABET;

        if (mode == 1) {
            wcout << L"Result: " << Encrypt(text, shift, alphabet) << endl;
        }
        else if (mode == 2) {
            wcout << L"Result: " << Decrypt(text, shift, alphabet) << endl;
        }

        wcout << L"\nНажмите Enter, чтобы выйти...";
        cin.get();
        return 0;
    }

    // Interactive mode
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