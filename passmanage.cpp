#include "passmanage.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ios>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <random>


int Encryption::Generate_Random_Number(void) const {
	int range = char_map.size() - 1;
	static std::random_device rd;
	static std::mt19937 rng(rd());
	static std::uniform_int_distribution<int> uid(1,range);
	return uid(rng);
}

char Encryption::Generate_Random_Direction(void) const {
	static std::random_device rd;
	static std::mt19937 rng(rd());
	static std::uniform_int_distribution<int> uid(0,1);
	int gen_number = uid(rng);
	if ( gen_number ) {
		return 'k';                          //Left
	}
	return 'e';                              //Right
}

std::string Encryption::Encrypt(const std::string& text, const char& direction, const int& shift) {
	std::string encrypted_text = "";
	if ( direction == 'e' ) {
		for(int x=0; x<text.length();x++) {
			//std::cout<<"Decrypted_Text: "<<decrypted_text<<std::endl;
			const char c_char = text[x];
			for ( int y=0; y<char_map.size();y++) {
				if ( char_map[y] == c_char ) {
					if ( y - shift < 0 ) {
						int negated = ( y - shift ) * -1;
						encrypted_text.push_back(char_map[char_map.size() - negated]);
						break;
					}
					encrypted_text.push_back(char_map[y-shift]);
					break;
				}
			}
		}
	}
	else {
		for(int x=0; x<text.length();x++) {
			//std::cout<<"Decrypted_Text: "<<decrypted_text<<std::endl;
			const char c_char = text[x];
			for ( int y=0; y<char_map.size();y++) {
				if ( char_map[y] == c_char ) {
					if ( y - shift < 0 ) {
						int negated = ( y - shift ) * -1;
						encrypted_text.push_back(char_map[char_map.size() - negated]);
						break;
					}
					encrypted_text.push_back(char_map[y-shift]);
					break;
				}
			}
		}
	}
	return encrypted_text;
}

bool Encryption::WriteEncryptedAccount(std::string account, std::string password, std::string save_file) {

	return false;
}

bool Encryption::WriteEncryptedUser(std::string username, std::string password, std::string save_file) {
	//Randomly generate shift and direction for the username and password encryption
	int shift = Generate_Random_Number();
	char direction = Generate_Random_Direction();
	std::cout<<"Shift: " <<shift<<"\nDirection: "<<direction<<std::endl;
	//Encrypt text
	std::string enc_user_name = Encrypt(username, direction, shift);
	std::string enc_password = Encrypt(password, direction, shift);
	//Write to file.
	std::cout<<"User file: "<<save_file<<std::endl;
	std::ofstream u_file(save_file, std::ios_base::app | std::ios_base::out);
	if ( u_file.is_open() ) {
		std::cout<<"Saving user."<<std::endl;
		u_file<<enc_user_name<<" "<<enc_password<<" "<<direction<<shift<<'\n';
		if ( u_file.fail() ) {
			std::cout<<"Failed to write user to file."<<std::endl;
			u_file.close();
			return false;
		}
		u_file.close();
		return true;
	}
	std::cout<<"Failed to open file."<<std::endl;
	return false;
}

std::string Encryption::Decrypt(std::string text, std::string code) {
	const char direction = code[0];
	const int shift = std::stoi(code.substr(1, code.length()));
	//Start decryption loop
	std::string decrypted_text = "";
	if ( direction == 'e') {
		for(int x=0; x<text.length();x++) {
			//std::cout<<"Decrypted_Text: "<<decrypted_text<<std::endl;
			const char c_char = text[x];
			for ( int y=0; y<char_map.size();y++) {
				if ( char_map[y] == c_char ) {
					if ( y + shift > char_map.size()-1 ) {
						int wrap_pos =  y + shift - ( char_map.size() );
						decrypted_text.push_back(char_map[wrap_pos]);
						break;
					}
					decrypted_text.push_back(char_map[y+shift]);
					break;
				}
			}
		}
	}
	else {
		for(int x=0; x<text.length();x++) {
			//std::cout<<"Decrypted_Text: "<<decrypted_text<<std::endl;
			const char c_char = text[x];
			for ( int y=0; y<char_map.size();y++) {
				if ( char_map[y] == c_char ) {
					if ( y - shift < 0 ) {
						int negated = ( y - shift ) * -1;
						decrypted_text.push_back(char_map[char_map.size() - negated]);
						break;
					}
					decrypted_text.push_back(char_map[y-shift]);
					break;
				}
			}
		}
	}
//	std::cout<<decrypted_text<<std::endl;
	return decrypted_text;
}

bool User::Set_User_Map(void) {
	//Set home directory.
	bool set = Set_User_File();
	if (!set) {
		std::cout<<"User file didn't set.";
		return false;
	}
	std::ifstream users_file(user_file, std::ifstream::in);
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
		if ( registered_user != "" && user_password != "" && code != "" ) {
			std::vector<std::string> user_info = {registered_user, user_password, code};
			user_map.push_back(user_info);
		}
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
	std::cout<<user_map.size()<<std::endl;
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
			c_user = enc.Decrypt(f_user[0], f_user[2]);
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

bool User::New_User(void) {
	bool file_set = Set_User_File();
	if (! file_set ) {
		std::cout<<"Failed to set user map."<<std::endl;
		return false;
	} 
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
			bool encrypt_user = enc.WriteEncryptedUser(new_user, password, user_file);
			if (! encrypt_user ) {
				std::cout<<"Failed to create user."<<std::endl;
				return false;
			}
			std::cout<<"User created."<<std::endl;
			break;
		} else {
			std::cout<<"Passwords didn't match."<<std::endl;
			continue;
		}
	}
	return true;
}

bool User::Set_User_File(void) {
	const std::string file = "/.Users.txt";
	const char *homedir;
	if ((homedir = getenv("HOME")) == NULL ) {
		homedir = getpwuid(getuid())->pw_dir;
	}
	const std::string home(homedir);
	const std::string ffile = home + file;
	user_file = ffile;
	return true;
}