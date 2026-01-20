#include <openssl/evp.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <map>

// To compile use g++ day5.cpp -lssl -lcrypto -o day5.exe


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
    std::string input = "wtnhxymk";
    
    int posnum{1}; // Incrementing, random number
    std::string password{""}; // Holds the characters of the building password

    std::string testKey; // the input string + positive random number
    std::string md5_digest; // Holds the md5 hash of the testKey


    std::map<size_t, char> passwordPt2Map{}; // For Part 2 this maps a password character index to the character

    // Brute force calculates the md5 hash using md5 function with input string + random positive number until we find all the digits
    // of the password(s)
    while(passwordPt2Map.size() < 8) {
        testKey = input + std::to_string(posnum);
        md5_digest = md5(testKey);
        // Looking for a hash that has 5 leading 0's
        if(md5_digest.substr(0, 5) == "00000") {
            if(password.size() < 8) { // Part 1 ends before Part 2, so this prevents adding extra character's to part 1's password
                password.push_back(md5_digest[5]);
            }
            // Part 2: Checking if the character at the 5th index is a digit that is a valid position in our Part 2 password and 
            // has a position value not seen before.
            if(std::isdigit(md5_digest[5])){
                size_t pos{size_t(md5_digest[5] - '0')};
                if(passwordPt2Map.count(pos) == 0 && pos < 8) {
                    // If the criteria are met the map adds the 6th index character as the value to a key of the 5th index value
                    passwordPt2Map[pos] = md5_digest[6];
                }
            }
        }
        posnum++;
    }

    std::cout << "The password is: " << password << std::endl; // Part 1 Output

    // Part 2 Output
    std::cout << "[Part 2] The password is: ";
    for (size_t i = 0; i < 8; ++i) {
        std::cout << passwordPt2Map.at(i);
    }
    return 0;
}