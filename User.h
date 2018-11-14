#ifndef _User_h
#define _User_h

#include <vector>
#include <string>
#include "Encrypt.h"

class User {
	private:
		std::vector<std::vector<std::string>> user_map = {{"Donovan", "abcd", "k10"}};
		std::string c_user;
		std::string user_file;
		Encryption enc;
		bool Set_User_Map(void);

	public:
		bool Set_User(void);
		void Construct_User_File(void);
		std::string Get_User(void) const;
		bool New_User(void) const;
};

#endif