#include <openssl/evp.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// To compile use g++ day4.cpp -lssl -lcrypto -o day4.exe


// Function to calculate md5 hash, in hex, of a string
std::string md5(const std::string& data) {

    // Using EVP interface of openssl library for MD5 hash calculation
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) throw std::runtime_error("EVP_MD_CTX_new failed");

    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len = 0;

    if (EVP_DigestInit_ex2(ctx, EVP_md5(), nullptr) != 1)
        throw std::runtime_error("EVP_DigestInit_ex2 failed");

    if (EVP_DigestUpdate(ctx, data.data(), data.size()) != 1)
        throw std::runtime_error("EVP_DigestUpdate failed");

    if (EVP_DigestFinal_ex(ctx, digest, &digest_len) != 1)
        throw std::runtime_error("EVP_DigestFinal_ex failed");

    EVP_MD_CTX_free(ctx);

    // Converting to Hex
    std::ostringstream ss;
    ss << std::hex << std::setfill('0');
    for (unsigned int i = 0; i < digest_len; i++)
        ss << std::setw(2) << (int)digest[i];

    return ss.str();
}

int main() {
    std::string input = "bgvyzdsv";
    
    int posnum = 1;

    std::string testKey; 
    std::string md5_digest;

    // Brute force calculates the md5 hash using md5 function with a random positive number until one that satisfies the
    // needed leading number of zeroes is found
    while(true) {
        testKey = input + std::to_string(posnum);
        md5_digest = md5(testKey);
        if(md5_digest.substr(0, 6) == "000000") {
            std::cout << "Found Secret Positive Number: " << posnum;
            break;
        }
        posnum++;
    }

    return 0;
}