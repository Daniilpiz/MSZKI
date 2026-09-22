#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <algorithm>
#include <limits>
#include <io.h>
#include <fcntl.h>

using namespace std;

/**
 * @brief Default English alphabet containing letters, digits, and punctuation marks.
 */
const wstring DEFAULT_ALPHABET = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,!?-;:() ";

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
            result += sub_alphabet[pos]; // Берем символ по той же позиции из таблицы замен
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
 * @brief Program entry point supporting CLI arguments and interactive mode.
 */
int wmain(int argc, wchar_t* argv[]) {
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stdin), _O_U8TEXT);
    _setmode(_fileno(stderr), _O_U8TEXT);

    string key_file = "sub_table_en.txt"; 

    if (argc >= 3) {
        int mode = _wtoi(argv[1]);
        wstring text = argv[2];

        wstring sub_table = GenerateOrLoadTable(DEFAULT_ALPHABET, key_file);

        if (mode == 1) {
            wcout << L"Result of Encrypt: " << Encrypt(text, DEFAULT_ALPHABET, sub_table) << endl;
        }
        else if (mode == 2) {
            wcout << L"Result of Decrypt: " << Decrypt(text, DEFAULT_ALPHABET, sub_table) << endl;
        }

        wcout << L"\nPress Enter to exit...";
        wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
        wcin.get();
        return 0;
    }

    wcout << L"--- Interactive Mode: Substitution Cipher ---\n";
    wcout << L"Enter text: ";
    wstring text;
    getline(wcin, text);

    wcout << L"Select mode (1 - Encrypt, 2 - Decrypt): ";
    int mode;
    wcin >> mode;

    wstring sub_table = GenerateOrLoadTable(DEFAULT_ALPHABET, key_file);

    if (mode == 1) {
        wcout << L"Ciphertext: " << Encrypt(text, DEFAULT_ALPHABET, sub_table) << endl;
    }
    else {
        wcout << L"Plaintext: " << Decrypt(text, DEFAULT_ALPHABET, sub_table) << endl;
    }

    wcout << L"\nPress Enter to exit...";
    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
    wcin.get();

    return 0;
}
