#include<iostream>
#include<cstdint>
#include<vector>
#include<bitset>
#define word bitset<32>
using namespace std;
class SHA_256 {
    private:
        uint32_t H[8];
        static constexpr uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};
        uint32_t rotr(uint32_t x, int n) {
            n%=32;
            return (x>>n)|(x<<(32-n));
        }
        uint32_t s0(uint32_t x) {
            return rotr(x, 7)^rotr(x, 18)^(x>>3);
        }
        uint32_t s1(uint32_t x) {
            return rotr(x, 17)^rotr(x, 19)^(x>>10);
        }
        uint32_t ch(uint32_t x, uint32_t y, uint32_t z) {
            return (x&y)^((~x)&z);
        }
        uint32_t maj(uint32_t x, uint32_t y, uint32_t z) {
            return (x&y)^(x&z)^(y&z);
        }
        uint32_t S0(uint32_t x) {
            return rotr(x, 2)^rotr(x, 13)^rotr(x, 22);
        }
        uint32_t S1(uint32_t x) {
            return rotr(x, 6)^rotr(x, 11)^rotr(x, 25);
        }
        vector<bitset<512>> padding(string s) {
            __uint128_t len=s.length(), bits=len*8;
            vector<bool> bit;
            for(int i = 0;i<len;i++)
                for(int j = 7;j>=0;j--)
                    bit.push_back(s[i]>>j&1);
            bit.push_back(true), bits++;
            while(bits%512!=448)
                bit.push_back(false), bits++;
            for(int i = 63;i>=0;i--)
                bit.push_back(((len*8)>>i)&1), bits++;
            vector<bitset<512>> ans;
            bitset<512> tmp;
            for(int i = 0;i<bits;i++) {
                if(i%512!=511)
                    tmp<<=1, tmp|=bit[i]?1:0;
                else
                    tmp<<=1, tmp|=bit[i]?1:0, ans.push_back(tmp), tmp.reset();
            }
            return ans;
        }
        vector<word> work(bitset<512> x) {
            vector<word> v(64);
            for(int i = 0;i<16;i++) {
                word w;
                for(int j = 0;j<32;j++)
                    w[31-j]=x[511-(i*32+j)];
                v[i]=w;
            }
            for(int i = 16;i<=63;i++) {
                uint32_t temp = s1(v[i-2].to_ulong()) + v[i-7].to_ulong() +
                                s0(v[i-15].to_ulong()) + v[i-16].to_ulong();
                v[i] = word(temp);
            }
            uint32_t a=H[0], b=H[1], c=H[2], d=H[3], e=H[4], f=H[5], g=H[6], h=H[7];
            for(int i = 0;i<64;i++) {
                uint32_t t1=h+S1(e)+ch(e, f, g)+K[i]+v[i].to_ulong(), t2=S0(a)+maj(a, b, c);
                h=g, g=f, f=e, e=d+t1, d=c, c=b, b=a, a=t1+t2;
            }
            H[0]+=a; H[1]+=b; H[2]+=c; H[3]+=d;
            H[4]+=e; H[5]+=f; H[6]+=g; H[7]+=h;
            return v;
        }
    public:
                void reset() {
            H[0]=0x6a09e667; H[1]=0xbb67ae85; H[2]=0x3c6ef372; H[3]=0xa54ff53a;
            H[4]=0x510e527f; H[5]=0x9b05688c; H[6]=0x1f83d9ab; H[7]=0x5be0cd19;
        }
    public:
        SHA_256() {
            reset();
        }
        string hash(string s) {
            reset();
            vector<bitset<512>> block=padding(s);
            for(int i = 0;i<block.size();i++) {
                work(block[i]);
            }
            string result;
            for(int i = 0;i<8;i++)
                for(int j = 0;j<8;j++)
                    result += "0123456789abcdef"[(H[i]>>(28-j*4))&0xf];
            return result;
        }
};