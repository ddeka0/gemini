#include "utils.hpp"

const unsigned int MOD = 1e9 + 7;
unsigned int sha256(const std::string str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    unsigned int res = 0;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
        res = (res + (int)hash[i])%MOD; 
    }
    return res % MOD;
}

bool inrange(unsigned int c,unsigned int a,unsigned int b) {
	// TODO implement inrange in proper directory and file
	a = a % SIZE;
	b = b % SIZE;
	c = c % SIZE;
	if(a < b) {
		return (a <= c) and (c < b);
	}else {
		return (a <= c) or (c < b);
	}
}