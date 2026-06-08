#include<iostream>
#include<vector>
#include<string>
#include<random>
#define vi vector<int>
#define vvi vector<vi >
#define mig multi_in_GF256
#define enMC4x4 enMixColumns4x4
#define deMC4x4 deMixColumns4x4
using namespace std;
class AES {
private:
	// ========== 常量定义 ==========
	unsigned char S_Box[16][16] = {
		{0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
		{0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
		{0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
		{0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
		{0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
		{0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
		{0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
		{0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
		{0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
		{0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
		{0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
		{0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
		{0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
		{0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
		{0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
		{0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}
	};
	int enMixColumns4x4[4][4] = {
		{0x02, 0x03, 0x01, 0x01},
		{0x01, 0x02, 0x03, 0x01},
		{0x01, 0x01, 0x02, 0x03},
		{0x03, 0x01, 0x01, 0x02}
	};
	int deMixColumns4x4[4][4] = {
		{0x0e, 0x0b, 0x0d, 0x09},
		{0x09, 0x0e, 0x0b, 0x0d},
		{0x0d, 0x09, 0x0e, 0x0b},
		{0x0b, 0x0d, 0x09, 0x0e}
	};

	// ========== 密钥相关 ==========
	unsigned char key[32];
	unsigned char roundKeys[15][16];

	// 生成随机密钥
	void gen_key() {
		chrono::system_clock::time_point now = chrono::system_clock::now();
		unsigned long long seed = chrono::duration_cast<chrono::nanoseconds>(now.time_since_epoch()).count();
		mt19937_64 mt(seed);
		uniform_int_distribution<> dist(0, 255);
		for(int i = 0; i < 32; i++) {
			key[i] = static_cast<unsigned char>(dist(mt));
		}
		key_expansion();
	}

	// RotWord: 循环左移1字节 [a0, a1, a2, a3] -> [a1, a2, a3, a0]
	void rot_word(unsigned char w[4]) {
		unsigned char tmp = w[0];
		w[0] = w[1]; w[1] = w[2]; w[2] = w[3]; w[3] = tmp;
	}

	// SubWord: 对4字节分别查S_Box
	void sub_word(unsigned char w[4]) {
		for(int i = 0; i < 4; i++) {
			int l = w[i] >> 4, r = w[i] & 15;
			w[i] = S_Box[l][r];
		}
	}

	// Rcon[j] = x^(j-1) 在 GF(2^8) 下，j从1开始
	unsigned char rcon_val(int j) {
		unsigned char val = 1;
		for(int k = 1; k < j; k++) {
			unsigned char carry = val & 0x80;
			val <<= 1;
			if(carry) val ^= 0x1b;
		}
		return val;
	}

	void key_expansion() {
		unsigned char w[60][4];
		for(int i = 0; i < 8; i++) {
			for(int j = 0; j < 4; j++) {
				w[i][j] = key[i * 4 + j];
			}
		}
		for(int i = 8; i < 60; i++) {
			unsigned char tmp[4];
			for(int j = 0; j < 4; j++) tmp[j] = w[i - 1][j];
			if(i % 8 == 0) {
				rot_word(tmp);
				sub_word(tmp);
				unsigned char rc = rcon_val(i / 8);
				tmp[0] ^= rc;
			} else if(i % 8 == 4) {
				sub_word(tmp);
			}
			for(int j = 0; j < 4; j++) {
				w[i][j] = w[i - 8][j] ^ tmp[j];
			}
		}
		for(int r = 0; r < 15; r++) {
			for(int j = 0; j < 4; j++) {
				for(int k = 0; k < 4; k++) {
					roundKeys[r][j * 4 + k] = w[r * 4 + j][k];
				}
			}
		}
	}

	// ========== GF(2^8) 乘法 ==========
	unsigned char multi_in_GF256(unsigned char a, unsigned char b) {
		unsigned char res = 0;
		while(b) {
			if(b & 1) res ^= a;
			unsigned char carry = a & 0x80;
			a <<= 1;
			if(carry) a ^= 0x1b;
			b >>= 1;
		}
		return res;
	}

	// ========== 数据格式转换 ==========
	vvi char_to_piece(const char* str) {
		vvi ans(4, vector<int>(4, 0));
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				ans[i][j] = static_cast<unsigned char>(str[i * 4 + j]);
			}
		}
		return ans;
	}

	string piece_to_str(vvi piece) {
		string ans;
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				ans.push_back(static_cast<unsigned char>(piece[i][j]));
			}
		}
		return ans;
	}

	vi piece_to_vector(vvi piece) {
		vector<int> ans;
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				ans.push_back(piece[i][j]);
			}
		}
		return ans;
	}

	// ========== 加密核心步骤 ==========
	// SubBytes: 每个字节查S_Box
	vvi enSubBytes(const vvi str) {
		vvi ans(4, vi(4, 0));
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				int l = static_cast<unsigned char>(str[i][j]) >> 4;
				int r = static_cast<unsigned char>(str[i][j]) & 15;
				ans[i][j] = (int)S_Box[l][r];
			}
		}
		return ans;
	}

	// ShiftRows: 行循环左移（第0行0格, 第1行1格, 第2行2格, 第3行3格）
	vvi enShiftRows(const vvi str) {
		vvi ans = str;
		// 第1行: 左移1格
		int tmp1 = ans[1][0];
		ans[1][0] = ans[1][1];
		ans[1][1] = ans[1][2];
		ans[1][2] = ans[1][3];
		ans[1][3] = tmp1;
		// 第2行: 左移2格
		tmp1 = ans[2][0];
		int tmp2 = ans[2][1];
		ans[2][0] = ans[2][2];
		ans[2][1] = ans[2][3];
		ans[2][2] = tmp1;
		ans[2][3] = tmp2;
		// 第3行: 左移3格
		tmp1 = ans[3][0], tmp2 = ans[3][1];
		int tmp3 = ans[3][2];
		ans[3][0] = ans[3][3];
		ans[3][1] = tmp1;
		ans[3][2] = tmp2;
		ans[3][3] = tmp3;
		return ans;
	}

	// MixColumns: 每列与固定矩阵相乘（GF(2^8)）
	vvi enMixColumns(const vvi str) {
		vvi ans(4, vi(4, 0));
		for(int c = 0; c < 4; c++) {
			ans[0][c] = mig(str[0][c], enMC4x4[0][0]) ^ mig(str[1][c], enMC4x4[0][1])
				      ^ mig(str[2][c], enMC4x4[0][2]) ^ mig(str[3][c], enMC4x4[0][3]);
			ans[1][c] = mig(str[0][c], enMC4x4[1][0]) ^ mig(str[1][c], enMC4x4[1][1])
				      ^ mig(str[2][c], enMC4x4[1][2]) ^ mig(str[3][c], enMC4x4[1][3]);
			ans[2][c] = mig(str[0][c], enMC4x4[2][0]) ^ mig(str[1][c], enMC4x4[2][1])
				      ^ mig(str[2][c], enMC4x4[2][2]) ^ mig(str[3][c], enMC4x4[2][3]);
			ans[3][c] = mig(str[0][c], enMC4x4[3][0]) ^ mig(str[1][c], enMC4x4[3][1])
				      ^ mig(str[2][c], enMC4x4[3][2]) ^ mig(str[3][c], enMC4x4[3][3]);
		}
		return ans;
	}

	// AddRoundKey: state[行][列] 与 roundKey[列*4+行] 异或
	vvi add_round_key(const vvi state, int round) {
		vvi ans = state;
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				ans[i][j] ^= static_cast<unsigned char>(roundKeys[round][j * 4 + i]);
			}
		}
		return ans;
	}

	// ========== 解密核心步骤 ==========
	// InvSubBytes: 暴力查找S_Box恢复原始值
	vvi deSubBytes(const vvi str) {
		vvi ans(4, vi(4, 0));
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				for(int k = 0; k < 16; k++) {
					for(int l = 0; l < 16; l++) {
						if(str[i][j] == S_Box[k][l]) {
							ans[i][j] = (k << 4) + l;
						}
					}
				}
			}
		}
		return ans;
	}

	// InvShiftRows: 行循环右移（与加密反向）
	vvi deShiftRows(const vvi str) {
		vvi ans = str;
		// 第1行: 右移1格
		int tmp1 = ans[1][3];
		ans[1][3] = ans[1][2];
		ans[1][2] = ans[1][1];
		ans[1][1] = ans[1][0];
		ans[1][0] = tmp1;
		// 第2行: 右移2格
		tmp1 = ans[2][3];
		int tmp2 = ans[2][2];
		ans[2][3] = ans[2][1];
		ans[2][2] = ans[2][0];
		ans[2][1] = tmp1;
		ans[2][0] = tmp2;
		// 第3行: 右移3格
		tmp1 = ans[3][3], tmp2 = ans[3][2];
		int tmp3 = ans[3][1];
		ans[3][3] = ans[3][0];
		ans[3][2] = tmp1;
		ans[3][1] = tmp2;
		ans[3][0] = tmp3;
		return ans;
	}

	// InvMixColumns: 每列与逆矩阵相乘
	vvi deMixColumns(const vvi str) {
		vvi ans(4, vi(4, 0));
		for(int c = 0; c < 4; c++) {
			ans[0][c] = mig(str[0][c], deMC4x4[0][0]) ^ mig(str[1][c], deMC4x4[0][1])
				      ^ mig(str[2][c], deMC4x4[0][2]) ^ mig(str[3][c], deMC4x4[0][3]);
			ans[1][c] = mig(str[0][c], deMC4x4[1][0]) ^ mig(str[1][c], deMC4x4[1][1])
				      ^ mig(str[2][c], deMC4x4[1][2]) ^ mig(str[3][c], deMC4x4[1][3]);
			ans[2][c] = mig(str[0][c], deMC4x4[2][0]) ^ mig(str[1][c], deMC4x4[2][1])
				      ^ mig(str[2][c], deMC4x4[2][2]) ^ mig(str[3][c], deMC4x4[2][3]);
			ans[3][c] = mig(str[0][c], deMC4x4[3][0]) ^ mig(str[1][c], deMC4x4[3][1])
				      ^ mig(str[2][c], deMC4x4[3][2]) ^ mig(str[3][c], deMC4x4[3][3]);
		}
		return ans;
	}

	// ========== 静态工具函数 ==========
	static string base64_encode(const vi& data) {
		const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		string ans;
		int i = 0, n = data.size();
		while(i < n) {
			unsigned char a = data[i++];
			bool has_b = (i < n);
			unsigned char b = has_b ? data[i++] : 0;
			bool has_c = (i < n);
			unsigned char c = has_c ? data[i++] : 0;
			ans.push_back(table[a >> 2]);
			ans.push_back(table[((a & 3) << 4) | (b >> 4)]);
			ans.push_back(has_b ? table[((b & 15) << 2) | (c >> 6)] : '=');
			ans.push_back(has_c ? table[c & 63] : '=');
		}
		return ans;
	}

	static vi base64_decode(const string& str) {
		auto idx = [](unsigned char c) -> int {
			if(c >= 'A' && c <= 'Z') return c - 'A';
			if(c >= 'a' && c <= 'z') return c - 'a' + 26;
			if(c >= '0' && c <= '9') return c - '0' + 52;
			if(c == '+') return 62;
			if(c == '/') return 63;
			return -1;
		};
		vi ans;
		int i = 0, n = str.length();
		while(i < n && str[i] != '=') {
			int a = idx(str[i++]); if(a == -1) continue;
			int b = idx(str[i++]); if(b == -1) continue;
			ans.push_back((a << 2) | (b >> 4));
			if(i < n && str[i] != '=') {
				int c = idx(str[i++]); if(c == -1) continue;
				ans.push_back(((b & 15) << 4) | (c >> 2));
				if(i < n && str[i] != '=') {
					int d = idx(str[i++]); if(d == -1) continue;
					ans.push_back(((c & 3) << 6) | d);
				}
			}
		}
		return ans;
	}

public:
	// ========== 构造函数 ==========
	AES() {
		gen_key();
	}
	AES(const unsigned char* user_key) {
		set_key(user_key);
	}

	// ========== 密钥设置 ==========
	// set_key: 手动设置32字节密钥，重新计算轮密钥
	void set_key(const unsigned char* user_key) {
		for(int i = 0; i < 32; i++) {
			key[i] = user_key[i];
		}
		key_expansion();
	}

	// ========== 密钥导出 ==========
	// get_key_str: 返回当前密钥字符串（32字节）
	string get_key_str() {
		string ans;
		for(int i = 0; i < 32; i++) {
			ans.push_back(static_cast<char>(key[i]));
		}
		return ans;
	}

	// ========== 加密 ==========
	// encode: 使用当前密钥AES-256加密 → Base64输出（只含密文）
	string encode(const char* msg) {
		string str(msg);
		vi raw;
		int len = 16 - str.length() % 16;
		for(int i = 1; i <= len; i++) {
			str.push_back(static_cast<char>(len));
		}
		for(int i = 0; i < (int)str.length(); i += 16) {
			vvi state = char_to_piece(str.substr(i, 16).c_str());
			state = add_round_key(state, 0);
			for(int r = 1; r <= 13; r++) {
				state = enSubBytes(state);
				state = enShiftRows(state);
				state = enMixColumns(state);
				state = add_round_key(state, r);
			}
			state = enSubBytes(state);
			state = enShiftRows(state);
			state = add_round_key(state, 14);
			vi v = piece_to_vector(state);
			for(int j = 0; j < 16; j++) raw.push_back(v[j]);
		}
		return base64_encode(raw);
	}

	// ========== 解密 ==========
	// decode: Base64解码 → 使用当前密钥解密 → 去填充
	string decode(const string& b64) {
		vi msg = base64_decode(b64);
		string ans;
		for(int i = 0; i < (int)msg.size(); i += 16) {
			vvi state(4, vi(4, 0));
			for(int j = 0; j < 4; j++) {
				for(int k = 0; k < 4; k++) {
					state[j][k] = msg[j * 4 + k + i];
				}
			}
			state = add_round_key(state, 14);
			state = deShiftRows(state);
			state = deSubBytes(state);
			for(int r = 13; r >= 1; r--) {
				state = add_round_key(state, r);
				state = deMixColumns(state);
				state = deShiftRows(state);
				state = deSubBytes(state);
			}
			state = add_round_key(state, 0);
			string str = piece_to_str(state);
			ans += str;
		}
		int pad_len = static_cast<unsigned char>(ans[ans.length() - 1]);
		return ans.substr(0, ans.length() - pad_len);
	}
};
