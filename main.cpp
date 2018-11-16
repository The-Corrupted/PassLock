#include <iostream>
#include "passmanage.cpp"


int main(void) {
	//Login/Register User
	User user_data;
	char path;
	std::cout<<"1.) Login\n2.) Register new user\n: ";
	while ( std::cin>>path ) {
		if ( path == '1' ) {
			break;
		} else if ( path == '2' ) {
			//Register new user
			std::cout<<"Registering new user."<<std::endl;
			user_data.New_User();
			std::cout<<"1.) Login\n2.) Register new user\n: ";
			continue;
		}
		std::cout<<"1.) Login\n2.) Register new user\n: ";

	}
	int attempts = 0;
	while ( attempts < 3 ) {
		bool unlocked = user_data.Set_User();
		if ( unlocked ) {
			std::cout<<"Welcome: " + user_data.Get_User()<<std::endl;
			break;
		} else {
			std::cout<<"Bad credentials."<<std::endl;
			attempts += 1;
			if ( attempts == 3 ) {
				std::cout<<"Too many login attempts made. Exiting program."<<std::endl;
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