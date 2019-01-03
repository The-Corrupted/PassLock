#include <iostream>
#include "passmanage.cpp"

int main(void) {
	//Login/Register User
	User user_data;
	char path;
	cls();
	std::cout<<"1.) Login\n2.) Register new user\n: ";
	while ( std::cin>>path ) {
		if ( path == '1' ) {
			break;
		} else if ( path == '2' ) {
			//Register new user
			cls();
			std::cout<<"Registering new user."<<std::endl;
			user_data.New_User();
			cls();
			std::cout<<"1.) Login\n2.) Register new user\n: ";
			continue;
		}
		std::cout<<"1.) Login\n2.) Register new user\n: ";

	}
	int attempts = 0;
	while ( attempts < 3 ) {
		bool unlocked = user_data.Set_User();
		if ( unlocked ) {
			cls();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::string welcome_message = "Welcome: " + user_data.Get_User() + "!";
			PrintUnderlinedText(welcome_message);
			break;
		} else {
			std::cout<<"Bad credentials."<<std::endl;
			attempts += 1;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			if ( attempts == 3 ) {
				std::cout<<"Too many login attempts made. Exiting program."<<std::endl;
				cls();
				return 0;
			}
		}
	}
	Menu main_menu(user_data);
	while (true) {
		main_menu.OptionsPrompt();
	}
	return 0;
}