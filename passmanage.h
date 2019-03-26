#ifndef _passmanage_h
#define _passmanage_h

#include <vector>
#include <string>

class Encryption {
	private:
		const std::vector<char> char_map = {
										   'A', 'B', 'C', 'D', 'E', 
										   'F', 'G', 'H', 'I', 'J',
										   'K', 'L', 'M', 'N', 'O', 
										   'P', 'Q', 'R', 'S', 'T', 
										   'U', 'V', 'W', 'X', 'Y',
										   'Z', 'a', 'b', 'c', 'd',
										   'e', 'f', 'g', 'h', 'i',
										   'j', 'k', 'l', 'm', 'n', 
										   'o', 'p', 'q', 'r', 's', 
										   't', 'u', 'v', 'w', 'x', 
										   'y', 'z', '1', '2', '3',
										   '4', '5', '6', '7', '8', 
										   '9', '0', '.', '(', ')', 
										   '&', '@', '!', '#', '$',
										   '%', '^', '_', '-', '=',
										   '\'', '\"', '~', '{', '}',
										   '[', ']'
									       };

		std::string Encrypt( const std::string&, const char&, const int& );
		int Generate_Random_Number( void ) const;
		char Generate_Random_Direction( void ) const;
	public:
		bool WriteEncryptedAccount( std::string, std::string, std::string, std::string );
		bool WriteEncryptedUser( std::string, std::string, std::string );
		bool RemoveEncryptedAccount( std::string, std::string );
		std::string Decrypt( std::string, std::string );
};

class User {
	private:
		std::vector<std::vector<std::string>> user_map = {};
		std::string c_user;
		std::string user_file;
		Encryption enc;
		bool Set_User_Map( void );
		bool Set_User_File( void );
		bool Set_Accounts_File( void );
	public:
		bool Set_User( void );
		void Construct_User_File( void );
		std::string Get_User( void ) const;
		bool New_User( void );
};

class Menu {
	private:
		bool AddAccountPasswordCombination( void );
		bool ListAccounts( void );
		bool RemoveAccount( void );
		std::vector<std::string> RetrieveAccount( std::string );
		void DisplayRetrieved(std::vector<std::string>);
		bool DeleteAccountPasswordCombination( void );
		bool SetAccountFile( void );
		std::string c_user;
		std::string account_file;
		Encryption enc;

	public:
		Menu( User );
		void OptionsPrompt( void );
};

#endif