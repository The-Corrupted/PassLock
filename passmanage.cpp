#include "passmanage.h"
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <ios>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <random>
#include <iterator>


// ----------------------------Global functions-------------------------------------

void cls( void ) {
	for(int x=0; x<60; x++) {
		std::cout<<std::endl;
	}
}

void PrintUnderlinedText( std::string text ) {
	std::cout<<text<<std::endl;
	for ( char& c : text ) {
		std::cout<<"-";
	}
	std::cout<<std::endl;
}

//-----------------------------Class Definitions---------------------------------


int Encryption::Generate_Random_Number( void ) const {
	int range = char_map.size() - 1;
	static std::random_device rd;
	static std::mt19937 rng( rd() );
	static std::uniform_int_distribution<int> uid( 1,range );
	return uid(rng);
}

char Encryption::Generate_Random_Direction( void ) const {
	static std::random_device rd;
	static std::mt19937 rng( rd() );
	static std::uniform_int_distribution<int> uid(0,1);
	int gen_number = uid(rng);
	if ( gen_number ) {
		return 'k';                          //Left
	}
	return 'e';                              //Right
}

std::string Encryption::Encrypt( const std::string& text, const char& direction, const int& shift ) {
	std::string encrypted_text = "";
	if ( direction == 'e' ) {
		for(int x=0; x<text.length();x++) {
			const char c_char = text[x];
			for ( int y=0; y<char_map.size();y++ ) {
				if ( char_map[y] == c_char ) {
					if ( y - shift < 0 ) {
						int negated = ( y - shift ) * -1;
						encrypted_text.push_back( char_map[char_map.size() - negated] );
						break;
					}
					encrypted_text.push_back( char_map[y-shift] );
					break;
				}
			}
		}
	}
	else {
		for(int x=0; x<text.length();x++) {
			const char c_char = text[x];
			for ( int y=0; y<char_map.size();y++) {
				if ( char_map[y] == c_char ) {
					if ( y + shift > char_map.size()-1 ) {
						int wrap_pos =  y + shift - ( char_map.size() );
						// std::cout<<"Wrap_pos: "<<wrap_pos<<std::endl;
						encrypted_text.push_back( char_map[wrap_pos] );
						break;
					}
					encrypted_text.push_back( char_map[y+shift] );
					break;
				}
			}
		}
	}
	return encrypted_text;
}

bool Encryption::WriteEncryptedAccount( std::string account, std::string password, std::string save_file, std::string username="" ) {
	std::cout<<"\n\nAccount: "<<account<<"\nPassword: "<<password<<"\nUsername: "<<username<<std::endl;
	int shift = Generate_Random_Number();
	char direction = Generate_Random_Direction();
	std::string enc_account = Encrypt ( account, direction, shift );
	std::string enc_password = Encrypt ( password, direction, shift );
	std::ofstream a_file( save_file, std::ios_base::app | std::ios_base::out );
	if ( username != "" || username != " " || username != "\n" ) {
		std::string enc_usermail = "";
		enc_usermail = Encrypt( username, direction, shift );
		if ( a_file.is_open() ) {
			std::cout<<"Saving account."<<std::endl;
			a_file<<enc_account<<" "<<enc_usermail<<" "<<enc_password<<" "<<direction<<shift<<'\n';
			if ( a_file.fail() ) {
				std::cout<<"Failed to write to file."<<std::endl;
				a_file.close();
				return false;
			}
			a_file.close();
			return true;
		}
		std::cout<<"Failed to open file."<<std::endl;
		return false;
	}
	if ( a_file.is_open() ) {
		std::cout<<"Saving account."<<std::endl;
		a_file<<enc_account<<" "<<enc_password<<" "<<direction<<shift<<'\n';
		if ( a_file.fail() ) {
			std::cout<<"Failed to write to file."<<std::endl;
			a_file.close();
			return true;
		}
		std::cout<<"Failed to open file."<<std::endl;
		return false;
	}
	std::cout<<"Failed to open file."<<std::endl;
	return false;
}

bool Encryption::WriteEncryptedUser( std::string username, std::string password, std::string save_file ) {
	//Randomly generate shift and direction for the username and password encryption
	int shift = Generate_Random_Number();
	char direction = Generate_Random_Direction();
	std::cout<<"Shift: "<<shift<<"\nDirection: "<<direction<<std::endl;
	//Encrypt text
	std::string enc_user_name = Encrypt( username, direction, shift );
	std::string enc_password = Encrypt( password, direction, shift );
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

std::string Encryption::Decrypt( std::string text, std::string code ) {
	const char direction = code[0];
	const int shift = std::stoi( code.substr( 1, code.length() ));
	//Start decryption loop
	std::string decrypted_text = "";
//	std::cout<<"Direction: "<<direction<<"\nShift: "<<shift<<std::endl;
	if ( direction == 'e') {
		for( int x=0; x<text.length();x++ ) {
			//std::cout<<"Decrypted_Text: "<<decrypted_text<<std::endl;
			const char c_char = text[x];
			for ( int y=0; y<char_map.size();y++ ) {
				if ( char_map[y] == c_char ) {
					if ( y + shift > char_map.size()-1 ) {
						int wrap_pos =  y + shift - ( char_map.size() );
						// std::cout<<"Wrap_pos: "<<wrap_pos<<std::endl;
						decrypted_text.push_back( char_map[wrap_pos] );
						break;
					}
					decrypted_text.push_back( char_map[y+shift] );
					break;
				}
			}
		}
	}
	else {
//		std::cout<<"Decrypting: "<<text<<std::endl;
		for( int x=0; x<text.length();x++ ) {
			//std::cout<<"Decrypted_Text: "<<decrypted_text<<std::endl;
			const char c_char = text[x];
			for ( int y=0; y<char_map.size();y++ ) {
				if ( char_map[y] == c_char ) {
					if ( y - shift < 0 ) {
						int negated = ( y - shift ) * -1;
						decrypted_text.push_back( char_map[char_map.size() - negated] );
						break;
					}
					decrypted_text.push_back( char_map[y-shift] );
					break;
				}
			}
		}
	}
//	std::cout<<decrypted_text<<std::endl;
	return decrypted_text;
}

bool User::Set_User_Map( void ) {
	//Set home directory.
	bool set = Set_User_File();
	if ( !set ) {
		std::cout<<"User file didn't set.";
		return false;
	}
	std::ifstream users_file(user_file, std::ifstream::in);
	if ( !users_file ) {
		std::cout<<"This file does not exist. Have any users been registered?"<<std::endl;
		return false;
	}
	while ( !users_file.eof() ) {
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

bool User::Set_User( void ) {
	std::cout<<"Username: ";
	std::string user;
	std::string password;
	std::cin>>user;
	// Check users file
	bool map_filled = Set_User_Map();        //Move to init and
	                                         //special refresh function.
	if ( !map_filled ) {
		return map_filled;                   //Failed
	}
	bool found_flag = false;
	std::vector<std::string> f_user;
	// std::cout<<user_map.size()<<std::endl;
	for ( const auto& v : user_map ) {
		// std::cout<<"User: "<<v[0]<<"\nCode: "<<v[2]<<std::endl;
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
		if ( password == enc.Decrypt( f_user[1], f_user[2] ) ) {
			c_user = enc.Decrypt( f_user[0], f_user[2] );
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}

std::string User::Get_User( void ) const {
	return c_user;
}

bool User::New_User( void ) {
	bool file_set = Set_User_File();     //Move to init and special refresh
                                         //function
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
			bool encrypt_user = enc.WriteEncryptedUser( new_user, password, user_file );
			if ( ! encrypt_user ) {
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
	if ((homedir = getenv( "HOME" )) == NULL ) {
		homedir = getpwuid(getuid())->pw_dir;
	}
	const std::string home(homedir);
	const std::string ffile = home + file;
	user_file = ffile;
	return true;
}

Menu::Menu(User user) {
	c_user = user.Get_User();
	SetAccountFile();
}

void Menu::OptionsPrompt(void) {
	char choice;
	std::cout<<"Select an option:"<<std::endl;
	std::cout<<"1.) Add Account and Password"<<std::endl;
	std::cout<<"2.) Retrieve Account Password"<<std::endl;
	std::cout<<"3.) Remove Account and Password"<<std::endl;
	std::cout<<": ";
	std::cin>>choice;
	std::string r_account;
	switch(choice) {
		case '1':
			cls();
			AddAccountPasswordCombination();
			break;
		case '2': 
			cls();
			std::cout<<"Account: ";
			std::cin>>r_account;
			DisplayRetrieved(RetrieveAccount(r_account));
			break;
		case '3':
			cls();
			std::cout<<"Removing saved account."<<std::endl;
			break;
		default:
			std::cout<<"Invalid option selected."<<std::endl;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
	}
	cls();
}

bool Menu::AddAccountPasswordCombination( void ) {
	std::string account;
	std::string password;
	std::string optional_username;
	std::cout<<"Account: ";
	//Ignore any newline in the buffer from any previous std::cin>>'s
	std::cin.ignore(1, '\n');
	std::getline( std::cin, account );
	std::cout<<"(Optional) Username/Email: ";
	std::getline( std::cin, optional_username );
	std::cout<<"Password: ";
	std::getline( std::cin, password );
	enc.WriteEncryptedAccount( account, password, account_file, optional_username );
}

bool Menu::SetAccountFile( void ) {
	const std::string file = "/.Accounts" + c_user + ".txt";
	const char *homedir;
	if ((homedir = getenv( "HOME" )) == NULL ) {
		homedir = getpwuid(getuid())->pw_dir;
	}
	const std::string home(homedir);
	const std::string afile = home + file;
	account_file = afile;
	return true;
}	

std::vector<std::string> Menu::RetrieveAccount( std::string req_account ) {
	std::vector<std::string> matched_details;
	std::ifstream accounts_file(account_file, std::ifstream::in);
	if (! accounts_file ) {
		std::cout<<"Couldn't open the accounts file. Does it exist?"<<std::endl;
		return matched_details;
	}
	std::string line;
	while(getline(accounts_file, line)) {
		std::stringstream ss(line);
		std::istream_iterator<std::string> begin(ss);
		std::istream_iterator<std::string> end;
		std::vector<std::string> linestrings(begin, end);
		std::string code = linestrings[linestrings.size() - 1];
		std::string user = enc.Decrypt(linestrings[0], code);
		if ( user == req_account ) {
			if ( linestrings.size() - 1 == 3 ) {      //Check if account present.
				matched_details = {user, enc.Decrypt(linestrings[1], code),
										  enc.Decrypt(linestrings[2], code)};
				break;
			} else {
				matched_details = {user, enc.Decrypt(linestrings[1], code)};
				break;
			}
		}
	}
	return matched_details;
}

void Menu::DisplayRetrieved( std::vector<std::string> aquired_details ) {
	if ( aquired_details.size() <= 1 ) {
		cls();
		std::cout<<"***Couldn't find the requested account***"<<std::endl;
		std::cout<<"-------Press any button to continue-------"<<std::endl;
		std::cin.ignore(1, '\n');
		int ch = std::cin.get();
	} else if ( aquired_details.size() == 2 ) {
		cls();
		std::cout<<"***Account Details***\n\n"<<std::endl;
		std::cout<<"Account: "<<aquired_details[0]<<std::endl;
		std::cout<<"Password: "<<aquired_details[1]<<std::endl;
		std::cout<<"\n\n-------Press any button to continue-------"<<std::endl;
		std::cin.ignore(1, '\n');
		int ch = std::cin.get();
		cls();
	} else {
		cls();
		std::cout<<"***Account Details***\n\n"<<std::endl;
		std::cout<<"Account: "<<aquired_details[0]<<std::endl;
		std::cout<<"Username/Email: "<<aquired_details[1]<<std::endl;
		std::cout<<"Password: "<<aquired_details[2]<<std::endl;
		std::cout<<"\n\n-------Press any button to continue-------"<<std::endl;
		std::cin.ignore(1, '\n');
		int ch = std::cin.get();
		cls();
	}
}

bool Menu::DeleteAccountPasswordCombination( void ) {

}

