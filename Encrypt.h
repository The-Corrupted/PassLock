#ifndef _Encrypt_h
#define _Encrypt_h

class Encryption {
	private:
		std::vector<char> char_map = {	   'A', 'B', 'C', 'D', 'E', 'F',
										   'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
										   'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
										   'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
										   'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
										   'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
										   '1', '2','3','4','5','6','7','8','9','0','.',
										   '(',')','&','@','!','#','$','%','^','_','-','=',
										   '\'', '\"','~','{','}','[',']'};
	public:
		bool Encrypt(std::string, std::string, char, int);
		bool Encrypt(std::string, std::string);
		std::string Decrypt(std::string, std::string);
};

#endif