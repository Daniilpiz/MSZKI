#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <algorithm>
#include <limits>
#include <io.h>
#include <fcntl.h>
#include <cstdlib>
#include <windows.h>

using namespace std;

/**
 * @brief Default English alphabet containing letters, digits, and punctuation marks.
 */
const wstring DEFAULT_ALPHABET = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,!?-;:() ";

/**
 * @brief Russian alphabet with letters and punctuation.
 */
const wstring RUSSIAN_ALPHABET = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя .,!?-;:()";

/**
 * @brief Digits only (0-9).
 */
const wstring DIGITS_ALPHABET = L"0123456789";

/**
 * @brief Special characters and punctuation.
 */
const wstring SPECIAL_ALPHABET = L"!@#$%^&*()-_=+[]{}|;:',.<>?/~`";

/**
 * @brief Loads existing substitution key table from file or generates and saves a new one.
 *
 * @param alphabet The original ordered alphabet.
 * @param filename File path to save/load key table.
 * @return Randomized substitution alphabet string.
 */
wstring GenerateOrLoadTable(const wstring& alphabet, const string& filename) {
    wifstream in(filename);
    in.imbue(locale(".UTF-8"));
    wstring sub_alphabet;

    if (in.is_open()) {
        getline(in, sub_alphabet);
        in.close();
        if (sub_alphabet.length() == alphabet.length()) {
            return sub_alphabet;
        }
    }

    sub_alphabet = alphabet;
    random_device rd;
    mt19937 g(rd());
    shuffle(sub_alphabet.begin(), sub_alphabet.end(), g);

    wofstream out(filename);
    out.imbue(locale(".UTF-8"));
    if (out.is_open()) {
        out << sub_alphabet;
        out.close();
    }
    return sub_alphabet;
}

/**
 * @brief Encrypts input text using a randomized substitution table.
 *
 * @param text The plain text to encrypt.
 * @param alphabet Original ordered alphabet.
 * @param sub_alphabet Randomized substitution alphabet.
 * @return Encrypted cipher text.
 */
wstring Encrypt(const wstring& text, const wstring& alphabet, const wstring& sub_alphabet) {
    wstring result = L"";
    for (wchar_t c : text) {
        size_t pos = alphabet.find(c);
        if (pos != wstring::npos) {
            result += sub_alphabet[pos];
        }
        else {
            result += c;
        }
    }
    return result;
}

/**
 * @brief Decrypts cipher text using substitution table mapping.
 *
 * @param text The cipher text to decrypt.
 * @param alphabet Original ordered alphabet.
 * @param sub_alphabet Randomized substitution alphabet.
 * @return Decrypted plain text.
 */
wstring Decrypt(const wstring& text, const wstring& alphabet, const wstring& sub_alphabet) {
    return Encrypt(text, sub_alphabet, alphabet);
}

/**
 * @brief Displays available alphabet options and returns selected alphabet.
 *
 * @return Selected alphabet string.
 */
wstring SelectAlphabet() {
    wcout << L"\n--- Select Alphabet ---\n";
    wcout << L"1. English (default, A-Z a-z 0-9 and punctuation)\n";
    wcout << L"2. Russian (А-Я а-я and punctuation)\n";
    wcout << L"3. Digits only (0-9)\n";
    wcout << L"4. Special characters (!@#$%^&*...)\n";
    wcout << L"5. Custom (enter your own)\n";
    wcout << L"Select option (1-5): ";
    
    int choice;
    wcin >> choice;
    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
    
    switch (choice) {
        case 1:
            wcout << L"Selected: English alphabet\n";
            return DEFAULT_ALPHABET;
        case 2:
            wcout << L"Selected: Russian alphabet\n";
            return RUSSIAN_ALPHABET;
        case 3:
            wcout << L"Selected: Digits only\n";
            return DIGITS_ALPHABET;
        case 4:
            wcout << L"Selected: Special characters\n";
            return SPECIAL_ALPHABET;
        case 5: {
            wcout << L"Enter custom alphabet: ";
            wstring customAlphabet;
            getline(wcin, customAlphabet);
            if (customAlphabet.empty()) {
                wcout << L"Empty alphabet, using default.\n";
                return DEFAULT_ALPHABET;
            }
            wcout << L"Selected: Custom alphabet (" << customAlphabet.length() << L" characters)\n";
            return customAlphabet;
        }
        default:
            wcout << L"Invalid choice, using English alphabet.\n";
            return DEFAULT_ALPHABET;
    }
}

/**
 * @brief Program entry point supporting CLI arguments and interactive mode.
 * 
 * CLI Usage:
 *   L1_2.exe <mode> <text> [alphabet_type]
 * Where:
 *   mode: 1 = Encrypt, 2 = Decrypt
 *   text: text to process
 *   alphabet_type: 1=English(default), 2=Russian, 3=Digits, 4=Special, 5=Custom:customtext
 * 
 * Examples:
 *   L1_2.exe 1 "hello" 1
 *   L1_2.exe 1 "привет" 2
 *   L1_2.exe 1 "test" 5:ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
 */
int wmain(int argc, wchar_t* argv[]) {
    // Setup Windows console for UTF-8
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stdin), _O_U8TEXT);
    _setmode(_fileno(stderr), _O_U8TEXT);

    string key_file = "sub_table_en.txt";

    if (argc >= 3) {
        int mode = _wtoi(argv[1]);
        wstring text = argv[2];
        wstring alphabet = DEFAULT_ALPHABET;

        if (argc >= 4) {
            wstring alphaArg = argv[3];
            if (alphaArg[0] == L'1') {
                alphabet = DEFAULT_ALPHABET;
                key_file = "sub_table_en.txt";
            }
            else if (alphaArg[0] == L'2') {
                alphabet = RUSSIAN_ALPHABET;
                key_file = "sub_table_ru.txt";
            }
            else if (alphaArg[0] == L'3') {
                alphabet = DIGITS_ALPHABET;
                key_file = "sub_table_digits.txt";
            }
            else if (alphaArg[0] == L'4') {
                alphabet = SPECIAL_ALPHABET;
                key_file = "sub_table_special.txt";
            }
            else if (alphaArg[0] == L'5' && alphaArg.length() > 2 && alphaArg[1] == L':') {
                alphabet = alphaArg.substr(2);
                key_file = "sub_table_custom.txt";
            }
        }

        wstring sub_table = GenerateOrLoadTable(alphabet, key_file);

        if (mode == 1) {
            wcout << L"Result: " << Encrypt(text, alphabet, sub_table) << endl;
        }
        else if (mode == 2) {
            wcout << L"Result: " << Decrypt(text, alphabet, sub_table) << endl;
        }

        wcout << L"\nPress Enter to exit...";
        wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
        wcin.get();
        return 0;
    }

    wcout << L"--- Interactive Mode: Substitution Cipher ---\n";
    
    wcout << L"Select mode (1 - Encrypt, 2 - Decrypt): ";
    int mode;
    wcin >> mode;
    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');

    wstring alphabet = SelectAlphabet();
    
    // Generate key file name based on alphabet type
    if (alphabet == DEFAULT_ALPHABET) {
        key_file = "sub_table_en.txt";
    } else if (alphabet == RUSSIAN_ALPHABET) {
        key_file = "sub_table_ru.txt";
    } else if (alphabet == DIGITS_ALPHABET) {
        key_file = "sub_table_digits.txt";
    } else if (alphabet == SPECIAL_ALPHABET) {
        key_file = "sub_table_special.txt";
    } else {
        key_file = "sub_table_custom.txt";
    }

    wstring sub_table = GenerateOrLoadTable(alphabet, key_file);

    wcout << L"Enter text: ";
    wstring text;
    getline(wcin, text);

    if (mode == 1) {
        wcout << L"\nCiphertext: " << Encrypt(text, alphabet, sub_table) << endl;
    }
    else {
        wcout << L"\nPlaintext: " << Decrypt(text, alphabet, sub_table) << endl;
    }

    wcout << L"\nPress Enter to exit...";
    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
    wcin.get();

    return 0;
}
