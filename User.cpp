#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Encrypt.cpp"

bool User::Set_User_Map(void) {
	std::ifstream users_file("~/.Users.txt", std::ifstream::in);
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
}

bool User::Set_User(void) {
	std::cout<<"Username: ";
	std::string user;
	std::string password;
	std::cin>>user;
	// Check users file
	map_filled = Set_User_Map();
	if (!map_filled) {
		return map_filled;                   //Failed
	}
	bool found_flag = false;
	std::vector<std::string> f_user;
	for ( const auto& v : user_map ) {
		if ( v[0] == user ) {
			found_flag = true;
			f_user = v;
			break;
		}
	}
	if ( found_flag == true ) {
		std::cout<<"Password: ";
		std::cin>>password;
		//Check for password
		if ( password == f_user[1] ) {
			c_user = f_user[0];
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