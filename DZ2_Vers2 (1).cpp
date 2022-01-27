// DZ2_Vers2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <fstream>
#include <limits.h>
#include <string>
#include <vector>

// This programm is for 1 byte block size and 4 bits left shift
const unsigned int g_left_shift = 4;

void get_mode(std::string& mode)
{
    std::cout << "Please enter mode (encryption or decryption):" << std::endl;
    std::getline(std::cin, mode);
}

void get_file_name(std::string& file_name)
{
    std::cout << "Please enter file name:" << std::endl;
    std::getline(std::cin, file_name);
}

void get_message(std::string& message)
{
    std::cout << "Please enter your message to encrypt:" << std::endl;
    std::getline(std::cin, message);
}

void get_password(unsigned int* password)
{
    std::cout << "Please enter your numeric password:" << std::endl;
    std::cin >> *password;
}

int encrypt(const std::string& message, const unsigned int password, const std::string& file_name)
{
    std::ofstream file(file_name, std::ios::binary | std::ios::trunc);
    if (!file.is_open())
    {
        std::cout << "Cannot create output file." << std::endl;
        return -2;
    }
    std::vector<char> bytes(message.begin(), message.end());
    srand(password);
    for (int i = 0; i < bytes.size(); i++)
    {
        const unsigned int gamma = rand();
        const unsigned int byte_xored_with_gamma = bytes[i] ^ gamma;
        const char encrypted_block = (byte_xored_with_gamma << g_left_shift) | ((byte_xored_with_gamma & 0xFF) >> (CHAR_BIT - g_left_shift));
        file << encrypted_block;
    }
    file.close();
    std::cout << "Your encrypted message has been saved." << std::endl;
    return 0;
}

int encrypt(const std::string& file_name)
{
    std::string message;
    get_message(message);
    unsigned int password;
    get_password(&password);
    return encrypt(message, password, file_name);
}

int decrypt(const std::string& file_name, const unsigned int password)
{
    std::ifstream file(file_name, std::ios::binary | std::ios::in);
    if (!file.is_open())
    {
        std::cout << "Cannot open file \'" << file_name << "\'." << std::endl;
        return -3;
    }
    std::vector<char> encrypted_message((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    std::string message;
    srand(password);
    for (int i = 0; i < encrypted_message.size(); i++)
    {
        const unsigned int gamma = rand();
        const unsigned int encrypted_block = ((encrypted_message[i] & 0xFF) >> g_left_shift) | (encrypted_message[i] << (CHAR_BIT - g_left_shift));
        const char decrypted_byte = encrypted_block ^ gamma;
        message.push_back(decrypted_byte);
    }
    std::cout << message << std::endl;
    return 0;
}

int decrypt(const std::string& file_name)
{
    unsigned int password;
    get_password(&password);
    return decrypt(file_name, password);
}

int main(int argc, const char* argv[])
{
    std::string mode;
    std::string file_name;

    if (argc < 2)
    {
        get_mode(mode);
        get_file_name(file_name);
    }
    else
    {
        mode = argv[1];
        if (argc < 3)
        {
            get_file_name(file_name);
        }
        else
        {
            file_name = argv[2];
        }
    }
    if (mode == "encryption")
    {
        return encrypt(file_name);
    }
    if (mode == "decryption")
    {
        return decrypt(file_name);
    }
    std::cout << "Invalid mode." << std::endl;
    return -1;
}

