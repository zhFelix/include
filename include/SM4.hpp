#ifndef SM4_HPP
#define SM4_HPP

#include<iostream>
#include<vector>
#include<string>
#include<random>
#define vi vector<int>
#define vvi vector<vi >
using namespace std;

class SM4 {
private:
    // SM4 S盒
    static const unsigned char S[16][16];
    // 系统参数 FK
    static const unsigned int FK[4];
    // 固定参数 CK (32个)
    static const unsigned int CK[32];
    
    unsigned int key[4];           // 128位密钥 (4个32位字)
    unsigned int roundKeys[32];    // 32轮轮密钥
    
    // 32位循环左移
    unsigned int rotl(unsigned int x, int n) {
        return (x << n) | (x >> (32 - n));
    }
    
    // 非线性变换 τ: 4个字节分别查S盒
    unsigned int tau(unsigned int A) {
        unsigned char a[4];
        a[0] = (A >> 24) & 0xFF;
        a[1] = (A >> 16) & 0xFF;
        a[2] = (A >> 8) & 0xFF;
        a[3] = A & 0xFF;
        unsigned int B = 0;
        for(int i=0;i<4;i++) {
            int l = a[i] >> 4, r = a[i] & 0xF;
            B = (B << 8) | S[l][r];
        }
        return B;
    }
    
    // 线性变换 L (用于加密轮函数)
    unsigned int L(unsigned int B) {
        return B ^ rotl(B, 2) ^ rotl(B, 10) ^ rotl(B, 18) ^ rotl(B, 24);
    }
    
    // 线性变换 L' (用于密钥扩展)
    unsigned int L_prime(unsigned int B) {
        return B ^ rotl(B, 13) ^ rotl(B, 23);
    }
    
    // T 变换 = L(τ(...))
    unsigned int T(unsigned int X) {
        return L(tau(X));
    }
    
    // T' 变换 = L'(τ(...))
    unsigned int T_prime(unsigned int X) {
        return L_prime(tau(X));
    }
    
    // 生成随机密钥
    void gen_key() {
        chrono::system_clock::time_point now = chrono::system_clock::now();
        unsigned long long seed = chrono::duration_cast<chrono::nanoseconds>(now.time_since_epoch()).count();
        mt19937_64 mt(seed);
        uniform_int_distribution<> dist(0, 255);
        unsigned char k[16];
        for(int i=0;i<16;i++) {
            k[i] = static_cast<unsigned char>(dist(mt));
        }
        for(int i=0;i<4;i++) {
            key[i] = (k[i*4] << 24) | (k[i*4+1] << 16) | (k[i*4+2] << 8) | k[i*4+3];
        }
        key_expansion();
    }
    
    void key_expansion() {
        unsigned int K[36];
        // K[0..3] = MK ^ FK
        for(int i=0;i<4;i++) {
            K[i] = key[i] ^ FK[i];
        }
        for(int i=0;i<32;i++) {
            K[i+4] = K[i] ^ T_prime(K[i+1] ^ K[i+2] ^ K[i+3] ^ CK[i]);
            roundKeys[i] = K[i+4];
        }
    }
    
    // 将字符串(16字节)转为4个32位字 (大端)
    void bytes_to_words(const unsigned char* data, unsigned int w[4]) {
        for(int i=0;i<4;i++) {
            w[i] = ((unsigned int)data[i*4] << 24) |
                   ((unsigned int)data[i*4+1] << 16) |
                   ((unsigned int)data[i*4+2] << 8) |
                   (unsigned int)data[i*4+3];
        }
    }
    
    // 将4个32位字转为16字节字符串 (大端)
    string words_to_str(unsigned int w[4]) {
        string ans;
        for(int i=0;i<4;i++) {
            ans.push_back(static_cast<char>((w[i] >> 24) & 0xFF));
            ans.push_back(static_cast<char>((w[i] >> 16) & 0xFF));
            ans.push_back(static_cast<char>((w[i] >> 8) & 0xFF));
            ans.push_back(static_cast<char>(w[i] & 0xFF));
        }
        return ans;
    }
    
public:
    SM4() {
        gen_key();
    }
    
    SM4(const unsigned char* user_key) {
        for(int i=0;i<4;i++) {
            key[i] = ((unsigned int)user_key[i*4] << 24) |
                     ((unsigned int)user_key[i*4+1] << 16) |
                     ((unsigned int)user_key[i*4+2] << 8) |
                     (unsigned int)user_key[i*4+3];
        }
        key_expansion();
    }
    
    // 加密一个块 (16字节)
    void encrypt_block(const unsigned char* in, unsigned char* out) {
        unsigned int X[36];
        bytes_to_words(in, X);
        for(int i=0;i<32;i++) {
            X[i+4] = X[i] ^ T(X[i+1] ^ X[i+2] ^ X[i+3] ^ roundKeys[i]);
        }
        // 反序输出 X[35], X[34], X[33], X[32]
        for(int i=0;i<4;i++) {
            unsigned int val = X[35-i];
            out[i*4] = (val >> 24) & 0xFF;
            out[i*4+1] = (val >> 16) & 0xFF;
            out[i*4+2] = (val >> 8) & 0xFF;
            out[i*4+3] = val & 0xFF;
        }
    }
    
    // 解密一个块 (16字节)
    void decrypt_block(const unsigned char* in, unsigned char* out) {
        unsigned int X[36];
        bytes_to_words(in, X);
        for(int i=0;i<32;i++) {
            // 解密用逆序轮密钥
            X[i+4] = X[i] ^ T(X[i+1] ^ X[i+2] ^ X[i+3] ^ roundKeys[31-i]);
        }
        for(int i=0;i<4;i++) {
            unsigned int val = X[35-i];
            out[i*4] = (val >> 24) & 0xFF;
            out[i*4+1] = (val >> 16) & 0xFF;
            out[i*4+2] = (val >> 8) & 0xFF;
            out[i*4+3] = val & 0xFF;
        }
    }
    
    string encode(const char* msg) {
        gen_key();  // 每次加密重新生成密钥
        string str(msg);
        vi raw;
        // PKCS#7 填充
        int pad_len = 16 - str.length() % 16;
        for(int i=1;i<=pad_len;i++) {
            str.push_back(static_cast<char>(pad_len));
        }
        for(size_t i=0;i<str.length();i+=16) {
            unsigned char in[16], out[16];
            for(int j=0;j<16;j++) {
                in[j] = static_cast<unsigned char>(str[i+j]);
            }
            encrypt_block(in, out);
            for(int j=0;j<16;j++) {
                raw.push_back(out[j]);
            }
        }
        // 密钥(16字节)拼在密文前
        vi combined;
        for(int i=0;i<4;i++) {
            combined.push_back((key[i] >> 24) & 0xFF);
            combined.push_back((key[i] >> 16) & 0xFF);
            combined.push_back((key[i] >> 8) & 0xFF);
            combined.push_back(key[i] & 0xFF);
        }
        for(int x : raw) combined.push_back(x);
        return base64_encode(combined);
    }
    
    string decode(const string& b64) {
        vi all = base64_decode(b64);
        // 前16字节是密钥
        unsigned char saved_key[16];
        for(int i=0;i<16;i++) saved_key[i] = static_cast<unsigned char>(all[i]);
        // 后部是密文
        vi msg;
        for(size_t i=16;i<all.size();i++) msg.push_back(all[i]);
        string ans;
        for(size_t i=0;i<msg.size();i+=16) {
            unsigned char in[16], out[16];
            for(int j=0;j<16;j++) in[j] = static_cast<unsigned char>(msg[i+j]);
            // 用提取的密钥构造临时SM4对象来解密
            SM4 tmp(saved_key);
            tmp.decrypt_block(in, out);
            for(int j=0;j<16;j++) {
                ans.push_back(static_cast<char>(out[j]));
            }
        }
        int pad_len = static_cast<unsigned char>(ans[ans.length()-1]);
        return ans.substr(0, ans.length() - pad_len);
    }
    
    // ---- Base64 工具 (与 AES 相同) ----
    static string base64_encode(const vi& data) {
        const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        string ans;
        int i=0, n=data.size();
        while(i<n) {
            unsigned char a = data[i++];
            bool has_b = (i < n);
            unsigned char b = has_b ? data[i++] : 0;
            bool has_c = (i < n);
            unsigned char c = has_c ? data[i++] : 0;
            ans.push_back(table[a>>2]);
            ans.push_back(table[((a&3)<<4)|(b>>4)]);
            ans.push_back(has_b ? table[((b&15)<<2)|(c>>6)] : '=');
            ans.push_back(has_c ? table[c&63] : '=');
        }
        return ans;
    }
    
    static vi base64_decode(const string& str) {
        auto idx = [](unsigned char c) -> int {
            if(c>='A'&&c<='Z') return c-'A';
            if(c>='a'&&c<='z') return c-'a'+26;
            if(c>='0'&&c<='9') return c-'0'+52;
            if(c=='+') return 62;
            if(c=='/') return 63;
            return -1;
        };
        vi ans;
        int i=0, n=str.length();
        while(i<n && str[i]!='=') {
            int a = idx(str[i++]); if(a==-1) continue;
            int b = idx(str[i++]); if(b==-1) continue;
            ans.push_back((a<<2)|(b>>4));
            if(i<n && str[i]!='=') {
                int c = idx(str[i++]); if(c==-1) continue;
                ans.push_back(((b&15)<<4)|(c>>2));
                if(i<n && str[i]!='=') {
                    int d = idx(str[i++]); if(d==-1) continue;
                    ans.push_back(((c&3)<<6)|d);
                }
            }
        }
        return ans;
    }
};

// SM4 S盒
const unsigned char SM4::S[16][16] = {
    {0xd6,0x90,0xe9,0xfe,0xcc,0xe1,0x3d,0xb7,0x16,0xb6,0x14,0xc2,0x28,0xfb,0x2c,0x05},
    {0x2b,0x67,0x9a,0x76,0x2a,0xbe,0x04,0xc3,0xaa,0x44,0x13,0x26,0x49,0x86,0x06,0x99},
    {0x9c,0x42,0x50,0xf4,0x91,0xef,0x98,0x7a,0x33,0x54,0x0b,0x43,0xed,0xcf,0xac,0x62},
    {0xe4,0xb3,0x1c,0xa9,0xc9,0x08,0xe8,0x95,0x80,0xdf,0x94,0xfa,0x75,0x8f,0x3f,0xa6},
    {0x47,0x07,0xa7,0xfc,0xf3,0x73,0x17,0xba,0x83,0x59,0x3c,0x19,0xe6,0x85,0x4f,0xa8},
    {0x68,0x6b,0x81,0xb2,0x71,0x64,0xda,0x8b,0xf8,0xeb,0x0f,0x4b,0x70,0x56,0x9d,0x35},
    {0x1e,0x24,0x0e,0x5e,0x63,0x58,0xd1,0xa2,0x25,0x22,0x7c,0x3b,0x01,0x21,0x78,0x87},
    {0xd4,0x00,0x46,0x57,0x9f,0xd3,0x27,0x52,0x4c,0x36,0x02,0xe7,0xa0,0xc4,0xc8,0x9e},
    {0xea,0xbf,0x8a,0xd2,0x40,0xc7,0x38,0xb5,0xa3,0xf7,0xf2,0xce,0xf9,0x61,0x15,0xa1},
    {0xe0,0xae,0x5d,0xa4,0x9b,0x34,0x1a,0x55,0xad,0x93,0x32,0x30,0xf5,0x8c,0xb1,0xe3},
    {0x1d,0xf6,0xe2,0x2e,0x82,0x66,0xca,0x60,0xc0,0x29,0x23,0xab,0x0d,0x53,0x4e,0x6f},
    {0xd5,0xdb,0x37,0x45,0xde,0xfd,0x8e,0x2f,0x03,0xff,0x6a,0x72,0x6d,0x6c,0x5b,0x51},
    {0x8d,0x1b,0xaf,0x92,0xbb,0xdd,0xbc,0x7f,0x11,0xd9,0x5c,0x41,0x1f,0x10,0x5a,0xd8},
    {0x0a,0xc1,0x31,0x88,0xa5,0xcd,0x7b,0xbd,0x2d,0x74,0xd0,0x12,0xb8,0xe5,0xb4,0xb0},
    {0x89,0x69,0x97,0x4a,0x0c,0x96,0x77,0x7e,0x65,0xb9,0xf1,0x09,0xc5,0x6e,0xc6,0x84},
    {0x18,0xf0,0x7d,0xec,0x3a,0xdc,0x4d,0x20,0x79,0xee,0x5f,0x3e,0xd7,0xcb,0x39,0x48}
};

// 系统参数 FK
const unsigned int SM4::FK[4] = {
    0xa3b1bac6, 0x56aa3350, 0x677d9197, 0xb27022dc
};

// 固定参数 CK
const unsigned int SM4::CK[32] = {
    0x00070e15, 0x1c232a31, 0x383f464d, 0x545b6269,
    0x70777e85, 0x8c939aa1, 0xa8afb6bd, 0xc4cbd2d9,
    0xe0e7eef5, 0xfc030a11, 0x181f262d, 0x343b4249,
    0x50575e65, 0x6c737a81, 0x888f969d, 0xa4abb2b9,
    0xc0c7ced5, 0xdce3eaf1, 0xf8ff060d, 0x141b2229,
    0x30373e45, 0x4c535a61, 0x686f767d, 0x848b9299,
    0xa0a7aeb5, 0xbcc3cad1, 0xd8dfe6ed, 0xf4fb0209,
    0x10171e25, 0x2c333a41, 0x484f565d, 0x646b7279
};

#endif