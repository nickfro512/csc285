/*
CSC 385 project - Library Management System


User and media add and list currently work

Known bugs:
Current way media/user IDing is set up, deleting records will break the ID system

STUFF THAT STILL NEEDS TO BE DONE:

USER:
delete
login
logout
checkin
checkout
get checked out list
update
search

MEDIA:

Update
Checkin
Checkout
Delete
Search


MENUS:

*** Would be nice to have all menus be objects, currently functions.

INPUT VALIDATION: We will want at least basic validation for new user/media info (i.e. valid emails, passwords etc)


Login interface - prompt for username, password

Admin interface
User menu
Edit user
Search users by field X
Media menu
Edit media
Search media by field X


User interface
Search menu
Checkin/checkout menu


DATABASE: 	Need to get a proper database setup or at least something that will read a CSV file or something
so changes/additions are saved on program exit

*/

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <ctime>

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

	// password encryption would be nice to have:

	// encrypt user password
	string User::encryptPassword(string password)
	{
		string encryptedPassword = password;
		return encryptedPassword;
	}

	// decrypt user password
	string User::decryptPassword(string encryptedPassword)
	{
		string decryptedPassword = encryptedPassword;
		return decryptedPassword;
	}
};

// a list of User objects	-	stand in for an actual database for the time being
class UserList
{
public:
	int userCounter = 0;	// number of users added so far, used to determined new user IDs
	vector<User> list;

	UserList::UserList()
	{
	}

	// get user by ID
	User UserList::getUser(int id)
	{
		User theUser;
		for (unsigned int i = 0; i < list.size(); i++)
		{
			theUser = list[i];
			if (theUser.userID == id)
			{
				return theUser;
			}
		}
		return theUser;		// no user found with matching ID
	}

	// get index of user in current list by user ID
	int UserList::getUserIndex(int id)
	{
		User theUser;
		unsigned int i;
		for (i = 0; i < list.size(); i++)
		{
			theUser = list[i];
			if (theUser.userID == id)
			{
				return i;
			}
		}
		return i;		// no user found with matching ID
	}

	// Add user
	int UserList::add(User theUser)
	{
		theUser.userID = userCounter;
		userCounter++;
		list.push_back(theUser);
		return theUser.userID;
	}

	// !!!! This doesn't work yet
	bool UserList::remove(int id)
	{
		int targetIndex = getUserIndex(id);
		list.erase(list.begin() + targetIndex);
		return true;
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
		int editedUserID;
		editedUserID = theList.add(theUser);
		return editedUserID;
	}

	bool UserHandler::deleteUser(int id)
	{
		theList.remove(id);
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


// Media item in the Library Management System
class Media
{
	// all media variables are public so they can be retrieved and updated by the MediaHandler class
public:
	int mediaID;				// unique user ID for media record
	char mediaType;			// media type: 'b' = book, 'd' = dvd, 'm' = music
	string isbn;				// ISBN number (string so it can take dashes)
	string title;				// media title
	string author;				// media author/artist name
	string subject;				// media subject (history, scifi, etc.)
	int copies;					// number of copies available
	vector<time_t> due_dates;		// due dates for copies that have been checked out
	vector<int> checked_out_IDs;	// user IDs of users who have checked out a copy

	// constructor
	Media::Media()
	{
		// initialize everything blank while we're debugging
		mediaID = -1;
		mediaType = 'x';
		isbn = "";
		title = "";
		author = "";
		subject = "";
		copies = 0;

	}

	void Media::clear()		// empty out a media record
	{
		// initialize everything blank while we're debugging
		mediaID = -1;	// this will get assigned by MediaHandler::add
		mediaType = 'x';
		isbn = "";
		title = "";
		author = "";
		subject = "";
		copies = 0;
	}

	// print the media's record to the console
	void Media::displayInformation()
	{
		cout << "ID: " << mediaID << endl;
		cout << "Type: ";
		if (mediaType == 'b')
		{
			cout << "Book" << endl;
		}
		else if (mediaType == 'd')
		{
			cout << "DVD" << endl;
		}
		else
		{
			cout << "Music" << endl;
		}

		cout << "ISBN: " << isbn << endl;
		cout << "Title: " << title << endl;
		cout << "Author: " << author << endl;
		cout << "Subject: " << subject << endl;
		cout << "Copies: " << copies << endl;

		cout << "------------------" << endl;
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

class InterfaceHandler
{

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
	cin.ignore(1, '\n');		// stop last cin from messing up future getline input by inserting a new line here
	cout << endl;
	return command;
}

// allow admin to input a new user record field by field
// NOTE: will want to validate these inputs eventually
User menu_user_add(UserHandler theHandler)
{
	User editedUser;

	cout << "User type (a for admin, p for patron): ";
	cin >> editedUser.userType;
	cin.ignore(1, '\n');		// stop last cin from messing up future getline input by inserting a new line here

	cout << "Username: ";
	getline(cin, editedUser.username);

	cout << "Password: ";
	getline(cin, editedUser.password);

	cout << "Full Name: ";
	getline(cin, editedUser.fullName);

	cout << "Email: ";
	getline(cin, editedUser.email);

	cout << "Address: ";
	getline(cin, editedUser.address);

	cout << "Phone: ";
	getline(cin, editedUser.phone);

	return editedUser;
}

// Menu to allow admin to edit user records
User menu_user_edit(User theUser, UserHandler theHandler)
{
	char edit_command = 'x';
	User editedUser = theUser;

	while (edit_command != 'x')
	{
		editedUser.displayInformation();

		cout << "Select field to edit" << endl << endl;

		cout << "(1) User Type " << endl;
		cout << "(2) Username: " << endl;
		cout << "(3) Password: " << endl;
		cout << "(4) First Name: " << endl;
		cout << "(5) Last Name: " << endl;
		cout << "(6) Email: " << endl;
		cout << "(7) Address: " << endl;
		cout << "(8) Phone: " << endl;
		cout << "(a) Edit all" << endl;
		cout << "(x) Finish editing" << endl;

		cin >> edit_command;
		cin.ignore(1, '\n');		// stop last cin from messing up future getline input by inserting a new line here

		switch (edit_command)
		{
		case 'a':
			editedUser = menu_user_add(theHandler);
			break;

		case '1':
			cin >> editedUser.userType;
			cin.ignore(1, '\n');		// stop last cin from messing up future getline input by inserting a new line here
			break;

		case '2':
			cout << "Username: ";
			getline(cin, editedUser.username);
			break;

		case '3':
			cout << "Password: ";
			getline(cin, editedUser.password);
			break;

		case '4':
		case '5':
			cout << "Full Name: ";
			getline(cin, editedUser.fullName);
			break;

		case '6':
			cout << "Email: ";
			getline(cin, editedUser.email);
			break;

		case '7':
			cout << "Address: ";
			getline(cin, editedUser.address);
			break;

		case '8':
			cout << "Phone: ";
			getline(cin, editedUser.phone);
			break;

		default:
			break;
		}
	}


	return editedUser;



}

// allow admin to input a new media record field by field
// NOTE: will want to validate these inputs eventually
Media menu_media_add(MediaHandler theHandler)
{
	Media newMedia;

	cout << "Media type (b for book, d for DVD, m for music): ";
	cin >> newMedia.mediaType;
	cin.ignore(1, '\n');		// stop last cin from messing up future getline input by inserting a new line here

	cout << "ISBN number: ";
	getline(cin, newMedia.isbn);

	cout << "Title: ";
	getline(cin, newMedia.title);

	cout << "Author: ";
	getline(cin, newMedia.author);

	cout << "Subject: ";
	getline(cin, newMedia.subject);

	cout << "Copies: ";
	cin >> newMedia.copies;



	return newMedia;
}

Media menu_media_edit(Media theMedia, MediaHandler theHandler)
{
	char edit_command = 'x';
	Media editedMedia = theMedia;

	while (edit_command != 'x')
	{
		editedMedia.displayInformation();

		cout << "Select field to edit" << endl << endl;

		cout << "(1) Media Type " << endl;
		cout << "(2) ISBN number: " << endl;
		cout << "(3) Title: " << endl;
		cout << "(4) Author: " << endl;
		cout << "(5) Subject: " << endl;
		cout << "(6) Copies: " << endl;
		cout << "(a) Edit all" << endl;
		cout << "(x) Finish editing" << endl;

		cin >> edit_command;
		cin.ignore(1, '\n');		// stop last cin from messing up future getline input by inserting a new line here

		switch (edit_command)
		{
		case 'a':
			editedMedia = menu_media_add(theHandler);
			break;

		case '1':
			cin >> editedMedia.mediaType;
			cin.ignore(1, '\n');		// stop last cin from messing up future getline input by inserting a new line here
			break;

		case '2':
			cout << "Title: ";
			getline(cin, editedMedia.title);
			break;

		case '3':
			cout << "Author: ";
			getline(cin, editedMedia.author);
			break;

		case '4':
			cout << "Subject: ";
			getline(cin, editedMedia.subject);
			break;

		case '5':
			cout << "Copies: ";
			cin >> editedMedia.copies;
			break;

		default:
			break;
		}
	}
	return editedMedia;
}


	int main()
	{
		vector<int> testlist;
		testlist.push_back(4);
		testlist.push_back(5);
		testlist.push_back(6);
		testlist.push_back(7);
		for (int i = 0; i < testlist.size(); i++)
		{
			cout << testlist[i] << endl;
		}

		cout << "at 0: " << testlist.at(0) << endl;
		testlist.erase(testlist.begin() + 0);
		cout << "at 0: " << testlist.at(0) << endl;

		for (int i = 0; i < testlist.size(); i++)
		{
			cout << testlist[i] << endl;
		}
		
		//cout << testlist;
		cout << endl;
		return 0;


		int menu_select = 1;
		char menu_command = 'z';

		User theUser;
		UserHandler theUserHandler;

		Media theMedia;
		MediaHandler theMediaHandler;

		User user2;				// initialize with a user for testing
		user2.userType = 'a';
		user2.sessionID = 55324;
		user2.username = "nfrogley";
		user2.password = "password";
		user2.fullName = "Nick Frogley";
		user2.email = "nickfro@gmail.com";
		user2.address = "111 Mansfield Hollow Rd, Mansfield Center, CT 06250";
		user2.phone = "860 214 9523";

		theUserHandler.addUser(user2);			// add first user

		//theUser = theUserHandler.getUser(0);	// get the new user record
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
				switch (menu_command)
				{
				case 'a':
					int newMediaID;
					theMedia = menu_media_add(theMediaHandler);
					newMediaID = theMediaHandler.addMedia(theMedia);
					cout << "Successfully added Media ID " << newMediaID << endl;
					break;
				case 'l':
					cout << "---------------------------------- MEDIA LIST" << endl;
					theMediaHandler.listAllMedia();
					break;

				}
			}

			else	// user menu
			{
				switch (menu_command)
				{
				case 'a':
					int editedUserID;
					theUser = menu_user_add(theUserHandler);
					editedUserID = theUserHandler.addUser(theUser);
					cout << "Successfully added user ID " << editedUserID << endl;
					break;
				case 'd':			// !!!! This doesn't work yet
					int deleteID;
					cout << endl << "User ID to delete: ";
					cin >> deleteID;
					theUserHandler.deleteUser(deleteID);
					cout << endl << "Successfully deleted user ID " << deleteID;
					break;
				case 'l':
					cout << "---------------------------------- USER LIST" << endl;
					theUserHandler.listAllUsers();
					break;

				}
			}

		}


		return 0;
	}
