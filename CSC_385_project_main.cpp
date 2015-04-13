/*
CSC 385 project - Library Management System


User add and list currently work for the most part. Set up media classes but haven't tested yet.

Known bugs: 
When adding a user, using spaces in the input will make it skip a field. 
Current way media/user IDing is set up, deleting record will break the ID system. Will fix soon.


STUFF THAT STILL NEEDS TO BE DONE:

USER:
delete
login
logout
checkin
checkout
get checked out list
update

VALIDATION: We will also want at least basic validation for new user info (i.e. valid emails, passwords etc)

MEDIA:
Create		
Update	
Checkin
Checkout
Delete	
Search

MENUS:

Log in menu

User Interface
	Edit user
Media interface
	New media
	Edit media



DATABASE: 	Need to get a proper database setup or at least something that will read a CSV file or something
			so changes/additions are saved on program exit

*/

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Library system users
class User
{
	// all user variables are public so they can be retrieved and updated by the UserHandler class
	public:
		int userID;						// unique user ID number
		int sessionID;					// session ID for logged in user
		char userType;					// user type - 'a' is admin, 'p' is patron, 'x' is inactive
		string username;				// username for their login
		string password;				// password for their login
		string fullName;				// user's full name
		string email;					// user's email
		string address;					// user's address
		string phone;					// user's phone number
		vector<int> checkedOutMedia;	// array with ID's of media items user has checked out of the system

	// constructor
	User::User()
	{
		// initialize everything blank while we're debugging
		userID = -1;	// this will get assigned by UserHandler::add
		userType = 'x';	
		sessionID = 0;
		username = "";	
		password = "";
		fullName = "";
		email = "";		
		address = "";				
		phone = "";
	}

	void User::clear()		// empty out a user record
	{
		// initialize everything blank while we're debugging
		userID = -1;	// this will get assigned by UserHandler::add
		userType = 'a';	
		sessionID = 0;
		username = "";	
		password = "";
		fullName = "";
		email = "";		
		address = "";				
		phone = "";
	}

	// print the user's record to the console
	void User::displayInformation()
	{
		cout << "ID: " << userID << endl;
		cout << "Type: ";
		if (userType == 'a')
		{
			cout << "Administrator" << endl;
		}
		else
		{
			cout << "Patron" << endl;
		}

		cout << "Username: " << username << endl;
		cout << "Password: " << decryptPassword(password) << endl;
		cout << "Name: " << fullName << endl;
		cout << "Email: " << email << endl;
		cout << "Address: " << address << endl;
		cout << "Phone number: " << phone << endl;

		cout << "------------------" << endl;
	}

	int User::login()
	{
	}

	int User::logout()
	{
	}

	// encrypt user password
	string User::encryptPassword(string password)
	{
		string encryptedPassword = "sfdfsdd";
		return encryptedPassword;
	}

	// decrypt user password
	string User::decryptPassword(string encryptedPassword)
	{
		string decryptedPassword = "password";
		return decryptedPassword;
	}
};

// a list of User objects	-	stand in for an actual database for the time being
class UserList
{
	public:
		vector<User> list;

	UserList::UserList()
	{
	}
	
	// get user by ID
	User UserList::getUser(int id)
	{
		return list[id];
	}

	// Add user
	int UserList::add(User theUser)
	{
		theUser.userID = list.size(); // user ID is the index user was added at in the list
		list.push_back(theUser);
		return theUser.userID;
	}

	// !!!! This doesn't work yet
	bool UserList::remove(int id)
	{
		list.erase(list.begin() + id);
		/*User theUser;
		theUser = getUser(id);
		theUser.userType = 'x';
		return true;*/ 
	}

	int UserList::getSize()
	{
		return list.size();
	}

};

// facilitates operations on the User objects in UserList objects
class UserHandler
{
	public:
		UserList theList;

	// UserHandler constructor
	UserHandler::UserHandler()
	{
	}

	// get a user from the user list by their ID number
	User UserHandler::getUser(int id)
	{
		User theUser = theList.getUser(id);
		return theUser;
	}

	// add a new user to the user list
	int UserHandler::addUser(User theUser)
	{
		int newUserID;
		newUserID = theList.add(theUser);
		return newUserID;
	}

	bool UserHandler::deleteUser(int id)
	{
		//theList.remove(id);
		return true;
	}

	void UserHandler::listAllUsers()
	{
		User theUser;
		for (int i = 0; i < theList.getSize(); i++)
		{
			theUser = getUser(i);
			theUser.displayInformation();
		}
	}
};


// !!!!!!! MEDIA CLASSES ARE UNTESTED

// Media item in the Library Management System
class Media
{
	// all media variables are public so they can be retrieved and updated by the MediaHandler class
	public:
		int mediaID;				// unique user ID for media record
		char media_type;			// media type: 'b' = book, 'd' = dvd, 'm' = music
		string isbn;				// ISBN number (string so it can take dashes)
		string title;				// media title
		string author;				// media author/artist name
		string subject;				// media subject (history, scifi, etc.)
		vector<date> due_dates;		// due dates for copies that have been checked out
		vector<int> checked_out_IDs;	// user IDs of users who have checked out a copy

	// constructor
	Media::Media()
	{
		// initialize everything blank while we're debugging
		mediaID = -1;
		media_type = 'x';
		isbn = "";
		title = "";
		author = "";
		subject = "";

	}

	void Media::clear()		// empty out a media record
	{
		// initialize everything blank while we're debugging
		mediaID = -1;	// this will get assigned by MediaHandler::add
		media_type = 'x';
		isbn = "";
		title = "";
		author = "";
		subject = "";
		
	}

	// print the media's record to the console
	void Media::displayInformation()
	{
	}

};

// a list of Media objects	-	stand in for an actual database for the time being
class MediaList
{
	public:
		vector<Media> list;

	MediaList::MediaList()
	{
	}
	
	// get media by ID
	Media MediaList::getMedia(int id)
	{
		return list[id];
	}

	// Add media
	int MediaList::add(Media theMedia)
	{
		theMedia.mediaID = list.size(); // media ID is the index media was added at in the list
		list.push_back(theMedia);
		return theMedia.mediaID;
	}

	// !!!! This doesn't work yet
	bool MediaList::remove(int id)
	{
		list.erase(list.begin() + id);
		/*Media theMedia;
		theMedia = getMedia(id);
		theMedia.mediaType = 'x';
		return true;*/ 
	}

	int MediaList::getSize()
	{
		return list.size();
	}

};

// facilitates operations on the Media objects in MediaList objects
class MediaHandler
{
	public:
		MediaList theList;

	// MediaHandler constructor
	MediaHandler::MediaHandler()
	{
	}

	// get a media from the media list by their ID number
	Media MediaHandler::getMedia(int id)
	{
		Media theMedia = theList.getMedia(id);
		return theMedia;
	}

	// add a new media to the media list
	int MediaHandler::addMedia(Media theMedia)
	{
		int newMediaID;
		newMediaID = theList.add(theMedia);
		return newMediaID;
	}

	bool MediaHandler::deleteMedia(int id)
	{
		//theList.remove(id);
		return true;
	}

	void MediaHandler::listAllMedia()
	{
		Media theMedia;
		for (int i = 0; i < theList.getSize(); i++)
		{
			theMedia = getMedia(i);
			theMedia.displayInformation();
		}
	}

	bool MediaHandler::checkIn()
	{
	}

	bool MediaHandler::checkOut()
	{
	}

};



// print the menu and get a command from user
char menu_select_get(int menu_type)
{
	cout << "MENU: " << endl;
	char command = 'x';
	User theUser;
	switch (menu_type)
	{	
		case 0:
			cout << "(a) Add media item" << endl;
			cout << "(d) Delete media item" << endl;
			cout << "(e) Edit media item" << endl;
			cout << "(l) List media items" << endl;
			break;
		case 1:
			cout << "(a) Add user" << endl;
			cout << "(d) Delete user" << endl;
			cout << "(e) Edit user" << endl;
			cout << "(l) List users" << endl;
			break;
	}
	cout << "(t) Toggle Media/User menu" << endl;
	cout << "(x) exit" << endl;
	cin >> command;
	return command;
}

// allow admin to input a new user record field by field
// NOTE: will want to validate these inputs eventually
User menu_user_add(UserHandler theHandler)
{
	User newUser;

	cout << "User type (a for admin, p for patron): ";
	cin >> newUser.userType;

	cout <<  "Username: ";
	cin >> newUser.username;

	cout <<  "Password: ";
	cin >> newUser.password;

	cout <<  "Full Name: ";
	cin >> newUser.fullName;

	cout <<  "Email: ";
	cin >> newUser.email;

	cout <<  "Address: ";
	cin >> newUser.address;

	cout <<  "Phone: ";
	cin >> newUser.phone;
	
	return newUser;
}

int main()
{
	int menu_select = 1;
	char menu_command = 'z';
	
	User theUser;
	UserHandler theHandler;

	User user2;				// initialize with a user for testing
	user2.userType = 'a';	
	user2.sessionID = 55324;
	user2.username = "nfrogley";	
	user2.password = "password";	
	user2.fullName = "Nick Frogley";		
	user2.email = "nickfro@gmail.com";		
	user2.address = "111 Mansfield Hollow Rd, Mansfield Center, CT 06250";				
	user2.phone = "860 214 9523";
	
	theHandler.addUser(user2);			// add first user
	
	//theUser = theHandler.getUser(0);	// get the new user record
	// theUser.displayInformation();		// print record

	while (menu_command != 'x')
	{
		menu_command = menu_select_get(menu_select);
		
		if (menu_command == 't')		// toggle media/user menu
		{
			if (menu_select == 0)
			{
				menu_select = 1;
			}
			else
			{
				menu_select = 0;
			}
		}

		if (menu_select == 0)	// media menu
		{
			switch(menu_command)
			{
				
			}
		}

		else	// user menu
		{
			switch(menu_command)
			{
				case 'a':
					int newUserID;
					theUser = menu_user_add(theHandler);
					newUserID = theHandler.addUser(theUser);
					cout << "Successfully added user ID " << newUserID << endl;
					break;
				case 'd':			// !!!! This doesn't work yet
					int deleteID;
					cout << endl << "User ID to delete: ";
					cin >> deleteID;
					theHandler.deleteUser(deleteID);
					cout << endl << "Successfully deleted user ID " << deleteID;
					break;
				case 'l':
					cout << "---------------------------------- USER LIST" << endl;
					theHandler.listAllUsers();
					break;	

			}
		}
			
	}


	return 0;
}
