#include "passmanage.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>



bool Encryption::Encrypt(std::string account, std::string password, char direction, int shift_level) {

	return false;
}

bool Encryption::Encrypt(std::string username, std::string password) {
	//Randomly generate shift and direction for the username and password encryption
	const char shift_pos;
	for ( const auto& v : )
	return false;
}

std::string Encryption::Decrypt(std::string text, std::string code) {
	char 	
	return "FAIL";
}

bool User::Set_User_Map(void) {
	std::string file = "/.Users.txt";
	const char *homedir;
	if ((homedir = getenv("HOME")) == NULL ) {
		homedir = getpwuid(getuid())->pw_dir;
	}
	std::string home(homedir);
	std::string ffile = home + file;
	std::ifstream users_file(ffile, std::ifstream::in);
	if ( !users_file ) {
		std::cout<<"This file does not exist. Have any users been registered?"<<std::endl;
		return false;		
	}
	while (!users_file.eof()) {
		std::string registered_user;
		std::string user_password;
		std::string code;
		users_file>>registered_user;
		users_file>>user_password;
		users_file>>code;
		std::vector<std::string> user_info = {registered_user, user_password, code};
		user_map.push_back(user_info);
	}
	return true;
}

bool User::Set_User(void) {
	std::cout<<"Username: ";
	std::string user;
	std::string password;
	std::cin>>user;
	// Check users file
	bool map_filled = Set_User_Map();
	if (!map_filled) {
		return map_filled;                   //Failed
	}
	bool found_flag = false;
	std::vector<std::string> f_user;
	for ( const auto& v : user_map ) {
		if ( enc.Decrypt(v[0], v[2]) == user ) {
			found_flag = true;
			f_user = v;
			break;
		}
	}
	if ( found_flag == true ) {
		std::cout<<"Password: ";
		std::cin>>password;
		//Check for password
		if ( password == enc.Decrypt(f_user[1], f_user[2]) ) {
			c_user = enc.Decrypt(f_user[0]);
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}

std::string User::Get_User(void) const {
	return c_user;
}

bool User::New_User(void) const {
	std::string new_user;
	std::string password;
	std::string password_check;
	while ( true ) {
		std::cout<<"Enter a username to use.\n: ";
		std::cin>>new_user;
		break;
		//Check file for availibility.
	}
	while ( true ) {
		std::cout<<"Enter password for account.\n: ";
		std::cin>>password;
		std::cout<<"Confirm password.\n: ";
		std::cin>>password_check;
		if ( password == password_check ) {
			//write to file
			std::cout<<"User created."<<std::endl;
			break;
		} else {
			std::cout<<"Passwords didn't match."<<std::endl;
			continue;
		}
	}
}

// void Construct_User_File(void) {
// 	return void;
// }