/*
CSC 385 project - Library Management System


User and media add, delete, edit, list currently work

Known bugs:
Current way media/user IDing is set up, deleting records will break the ID system

STUFF THAT STILL NEEDS TO BE DONE:

USER:
login
logout
checkin
checkout
get checked out list
search

MEDIA:

Checkin
Checkout
Search


MENUS:

*** Would be nice to have all menus be objects, currently functions.

INPUT VALIDATION: We will want at least basic validation for new user/media info (i.e. valid emails, passwords etc)


Login interface - prompt for username, password

Admin interface
User menu
	Search users by field X
Media menu
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
	string firstName;				// user's full name
	string lastName;				// user's last name
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
		firstName = "";
		lastName = "";
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
		firstName = "";
		lastName = "";
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
		cout << "Name: " << firstName << " " << lastName << endl;
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
	// number of users added so far, used to determined new user IDs
	vector<User> list;
	int userCounter;

	UserList::UserList()
	{
		userCounter = 0;
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
		return -1;		// no user found with matching ID
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
	}

	bool UserList::edit(int id, User editedUser)
	{
		int targetIndex = getUserIndex(id);
		list[targetIndex] = editedUser;
		return true;
	}

	

	void UserList::listAll()
	{
		User theUser;
		for (int i = 0; i < list.size(); i++)
		{
			theUser = list[i];
			theUser.displayInformation();
		}
	}

	vector<int> UserList::getAllUserIDs()
	{
		vector<int> userIDs;
		User theUser;

		for (int i = 0; i < list.size(); i++)
		{
			theUser = list[i];
			userIDs.push_back(theUser.userID);
		}
	}

};

// facilitates operations on the User objects in UserList objects
class UserHandler
{
public:
	UserList theUserList;

	// UserHandler constructor
	UserHandler::UserHandler()
	{
	}

	// get a user from the user list by their ID number
	User UserHandler::getUser(int id)
	{
		User theUser = theUserList.getUser(id);
		return theUser;
	}

	// add a new user to the user list
	int UserHandler::addUser(User theUser)
	{
		int editedUserID;
		editedUserID = theUserList.add(theUser);
		return editedUserID;
	}

	bool UserHandler::deleteUser(int id)
	{
		theUserList.remove(id);
		return true;
	}

	bool UserHandler::editUser(int id, User editedUser)
	{
		theUserList.edit(id, editedUser);
		return true;
	}

	void UserHandler::listAllUsers()
	{
		theUserList.listAll();
		/*User theUser;
		for (int i = 0; i < theUserList.getSize(); i++)
		{
			theUser = getUser(i);
			theUser.displayInformation();
		}*/
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
	int mediaCounter;

	MediaList::MediaList()
	{
		mediaCounter = 0;
	}

	// get media by ID
	Media MediaList::getMedia(int id)
	{
		Media theMedia;
		for (unsigned int i = 0; i < list.size(); i++)
		{
			theMedia = list[i];
			if (theMedia.mediaID == id)
			{
				return theMedia;
			}
		}
		return theMedia;		// no Media found with matching ID
	}

	// get index in current list of media with provided ID
	int MediaList::getMediaIndex(int id)
	{
		Media theMedia;
		unsigned int i;
		for (i = 0; i < list.size(); i++)
		{
			theMedia = list[i];
			if (theMedia.mediaID == id)
			{
				return i;
			}
		}
		return -1;		// no Media found with matching ID
	}

	// Add media
	int MediaList::add(Media theMedia)
	{
		theMedia.mediaID = mediaCounter; // media ID is the index media was added at in the list
		mediaCounter++;
		list.push_back(theMedia);
		return theMedia.mediaID;
	}

	bool MediaList::remove(int id)
	{
		int targetIndex = getMediaIndex(id);
		list.erase(list.begin() + targetIndex);
		return true;
	}

	bool MediaList::edit(int id, Media editedMedia)
	{
		int targetIndex = getMediaIndex(id);
		list[targetIndex] = editedMedia;
		return true;
	}

	

	void MediaList::listAll()
	{
		Media theMedia;
		for (int i = 0; i < list.size(); i++)
		{
			theMedia = list[i];
			theMedia.displayInformation();
		}
	}

	vector<int> MediaList::getAllMediaIDs()
	{
		vector<int> MediaIDs;
		Media theMedia;

		for (int i = 0; i < list.size(); i++)
		{
			theMedia = list[i];
			MediaIDs.push_back(theMedia.mediaID);
		}
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
	MediaList theMediaList;

	// MediaHandler constructor
	MediaHandler::MediaHandler()
	{
	}

	// get a media from the media list by their ID number
	Media MediaHandler::getMedia(int id)
	{
		Media theMedia = theMediaList.getMedia(id);
		return theMedia;
	}

	// add a new media to the media list
	int MediaHandler::addMedia(Media theMedia)
	{
		int newMediaID;
		newMediaID = theMediaList.add(theMedia);
		return newMediaID;
	}

	bool MediaHandler::deleteMedia(int id)
	{
		theMediaList.remove(id);
		return true;
	}

	bool MediaHandler::editMedia(int id, Media editedMedia)
	{
		theMediaList.edit(id, editedMedia);
		return true;
	}

	void MediaHandler::listAllMedia()
	{
		theMediaList.listAll();
		/*Media theMedia;
		for (int i = 0; i < theMediaList.getSize(); i++)
		{
			theMedia = getMedia(i);
			theMedia.displayInformation();
		}*/
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

	cout << "First Name: ";
	getline(cin, editedUser.firstName);

	cout << "Last Name: ";
	getline(cin, editedUser.lastName);

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
	char edit_command = 'z';
	User editedUser = theUser;

	while (edit_command != 'x')
	{
		editedUser.displayInformation();

		cout << "Select field to edit" << endl << endl;

		cout << "(1) User Type" << endl;
		cout << "(2) Username" << endl;
		cout << "(3) Password" << endl;
		cout << "(4) First Name" << endl;
		cout << "(5) Last Name" << endl;
		cout << "(6) Email" << endl;
		cout << "(7) Address" << endl;
		cout << "(8) Phone" << endl;
		cout << "(a) Edit all" << endl;
		cout << "(x) Finish editing" << endl;

		cin >> edit_command;
		cin.ignore(1, '\n');		// stop last cin from messing up future getline input by inserting a new line here

		switch (edit_command)
		{
		case 'a':
			editedUser = menu_user_add(theHandler);		// use user add menu to get all new information for this user ID
			// get userID, session ID etc from original user record
			editedUser.userID = theUser.userID;
			editedUser.sessionID = theUser.sessionID;
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
			cout << "First Name: ";
			getline(cin, editedUser.firstName);
			break;

		case '5':
			cout << "Last Name: ";
			getline(cin, editedUser.lastName);
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
			cout << "Invalid command!" << endl;
			break;
		}

		//editedUser.userID = theUser.userID;
		//editedUser.sessionID = theUser.sessionID;
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
	char edit_command = 'z';
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
			editedMedia = menu_media_add(theHandler);		// use the media add menu to get all new info for this media ID
			// get media ID etc from original media record
			editedMedia.mediaID = theMedia.mediaID;
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
	int menu_select = 1;
	char menu_command = 'z';

	User theUser;
	UserHandler theUserHandler;

	Media theMedia;
	MediaHandler theMediaHandler;

	User testUser;				// initialize with a user for testing
	testUser.userType = 'a';
	testUser.sessionID = 55324;
	testUser.username = "nfrogley";
	testUser.password = "password";
	testUser.firstName = "Nick";
	testUser.lastName = "Frogley";
	testUser.email = "nickfro@gmail.com";
	testUser.address = "111 Mansfield Hollow Rd, Mansfield Center, CT 06250";
	testUser.phone = "860 214 9523";

	theUserHandler.addUser(testUser);			// add first user

	Media testMedia;			// initialize with a media for testing
	testMedia.mediaType = 'b';
	testMedia.isbn = "0553386794";
	testMedia.title = "Game of Thrones";
	testMedia.author = "George R. R. Martin";
	testMedia.subject = "Fantasy";
	testMedia.copies = 5;

	theMediaHandler.addMedia(testMedia);	// add first media

	

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
			Media editedMedia;
			switch (menu_command)
			{
				case 'a':
					int newMediaID;
					theMedia = menu_media_add(theMediaHandler);
					newMediaID = theMediaHandler.addMedia(theMedia);
					cout << "Successfully added Media ID " << newMediaID << endl;
					break;
				case 'e':
					int editID;
					cout << endl << "Media ID to edit: ";
					do
					{
						cin >> editID;
						theMedia = theMediaHandler.getMedia(editID);
						if (theMedia.mediaID == -1)
						{
							cout << "Media ID not found!" << endl;
						}
					}
					while(theMedia.mediaID == -1);

					editedMedia = menu_media_edit(theMedia, theMediaHandler);
					theMediaHandler.editMedia(editID, editedMedia);
					cout << endl << "Successfully edited Media ID " << editID << endl;
					break;
				case 'd':
					int deleteID;
					cout << endl << "Media ID to delete: ";
					cin >> deleteID;
					theMediaHandler.deleteMedia(deleteID);
					cout << endl << "Successfully deleted Media ID " << deleteID << endl;
					break;
				case 'l':
					cout << "---------------------------------- MEDIA LIST" << endl;
					theMediaHandler.listAllMedia();
					break;

			}
		}

		else	// user menu
		{
			User editedUser;
			switch (menu_command)
			{
				case 'a':
					int newUserID;
					theUser = menu_user_add(theUserHandler);
					newUserID = theUserHandler.addUser(theUser);
					cout << "Successfully added user ID " << newUserID << endl;
					break;
				case 'e':
					int editID;
					cout << endl << "User ID to edit: ";
					do
					{
						cin >> editID;
						theUser = theUserHandler.getUser(editID);
						if (theUser.userID == -1)
						{
							cout << "User ID not found!" << endl;
						}
					}
					while(theUser.userID == -1);

					editedUser = menu_user_edit(theUser, theUserHandler);
					theUserHandler.editUser(editID, editedUser);
					cout << endl << "Successfully edited user ID " << editID << endl;
					break;
				case 'd':
					int deleteID;
					cout << endl << "User ID to delete: ";
					cin >> deleteID;
					theUserHandler.deleteUser(deleteID);
					cout << endl << "Successfully deleted user ID " << deleteID << endl;
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
