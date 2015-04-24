/*
CSC 385 project - Library Management System


User and media add, delete, edit, list currently work. User and Media are read in from user_data.txt and
media_data.txt now. Changes are saved on program exit by writing to those files. Basically that's going to be
our standin for the actual database that's in the specs.


Known bugs:
Entering text for copies when adding/editing media breaks program
Checkin/checkout is buggy in various ways

STUFF THAT STILL NEEDS TO BE DONE:

USER:
checkin/checkout stuff - Nick is working on this
login
logout
search
password encryption - Haven't Tested 100%, but test cases and adding a new user worked as intended


MEDIA:
Search
checkin/checkout stuff - Nick is working on this


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


*/

//#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

// filenames for user and media data
const string userDataFile = "user_data.txt";
const string mediaDataFile = "media_data.txt";

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
	vector<int> checkedOutMediaIds;	// array with ID's of media items user has checked out of the system

	// constructor
	User()
	{
		// initialize everything blank while we're debugging
		userID = -1;	// this will get assigned by add
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

	void clear()		// empty out a user record
	{
		// initialize everything blank while we're debugging
		userID = -1;	// this will get assigned by add
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
	void displayInformation()
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
		cout << "Password: " << passwordEncryptDecrypt(password) << endl;
		cout << "Name: " << firstName << " " << lastName << endl;
		cout << "Email: " << email << endl;
		cout << "Address: " << address << endl;
		cout << "Phone number: " << phone << endl;

		cout << "------------------" << endl;
	}

	void displayCheckedOutInformation()
	{
		if (checkedOutMediaIds.size() > 0)
		{
			cout << "Checked out media: " << endl;
			for (unsigned int i = 0; i < checkedOutMediaIds.size(); i++)
			{
				cout << "Media " << checkedOutMediaIds[i] << endl;
			}
			cout << "---------------" << endl;
		}
	}

	int login()
	{
		return 0;
	}

	int logout()
	{
		return 0;
	}

	// when user first creates a password, password = passwordEncryptDecrypt(password), that will encrypt it
	// the password will then be stored in an encrypted state, then call passwordEncryptDecrypt(password) to
	// display the dcrypted password without changing the password within the 'database'
	string passwordEncryptDecrypt(string passwordIn) {
		char key[3] = { 'K', 'C', 'Q' }; //Any chars will work
		string passwordOut = passwordIn;

		for (int i = 0; i < passwordIn.size(); i++)
			passwordOut[i] = passwordIn[i] ^ key[i % (sizeof(key) / sizeof(char))];

		return passwordOut;
	}
};

// a list of User objects	-	stand in for an actual database for the time being
class UserList
{
public:
	// number of users added so far, used to determined new user IDs
	vector<User> list;
	int userIdCounter;

	UserList()
	{
		userIdCounter = 0;
	}



	// get user by ID
	User getUser(int id)
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
	int getUserIndex(int id)
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
	int add(User theUser)
	{
		theUser.userID = userIdCounter;		 // user is assigned newest available ID based on count of users added so far (userCounter)
		userIdCounter++;
		list.push_back(theUser);
		return theUser.userID;
	}

	// Remove user
	bool remove(int id)
	{
		int targetIndex = getUserIndex(id);
		list.erase(list.begin() + targetIndex);
		return true;
	}

	// Edit a user record (replaces old record with editedUser record)
	bool edit(int id, User editedUser)
	{
		int targetIndex = getUserIndex(id);
		list[targetIndex] = editedUser;
		return true;
	}

	bool checkIn(int mediaID, int userID)
	{
		return false;
	}

	bool checkOut(int mediaID, int userID)
	{
		User theUser = getUser(userID);
		theUser.checkedOutMediaIds.push_back(mediaID);
		edit(userID, theUser);
		cout << "\n\nCheck out succeeded on user record ID " << userID << endl;
		return true;
	}

	// display list of all users
	void listAll()
	{
		User theUser;
		for (int i = 0; i < list.size(); i++)
		{
			theUser = list[i];
			theUser.displayInformation();
			theUser.displayCheckedOutInformation();
		}
	}

	// get a list of all the user IDs currently in the user list
	vector<int> getAllUserIDs()
	{
		vector<int> userIDs;
		User theUser;

		for (int i = 0; i < list.size(); i++)
		{
			theUser = list[i];
			userIDs.push_back(theUser.userID);
		}
		return userIDs;
	}

	// populate the User list by reading users in from a text file
	bool readListFromFile(string filename)
	{
		string line;
		string firstLine;

		vector<User> newList;				// new list of users to read in to
		User theUser;

		ifstream theFile(filename);			// attempt to open file

		if (theFile.is_open())
		{
			if (getline(theFile, line))
			{
				userIdCounter = atoi(line.c_str());	// convert string to int for user ID counter value (gets used to assign new user IDs)

				while (getline(theFile, firstLine))		// get the first line of the next user record if we're not at end of file
				{
					// read a User record in from the file

					theUser.userID = atoi(firstLine.c_str());		// convert string to int for user ID value

					getline(theFile, line);
					theUser.userType = line.at(0);					// get first character of string for userType value

					getline(theFile, line);
					theUser.sessionID = atoi(line.c_str());		// convert string to int for session ID value

					getline(theFile, line);
					theUser.username = line;

					getline(theFile, line);
					string password = line;
					theUser.password = theUser.passwordEncryptDecrypt(password);

					getline(theFile, line);
					theUser.firstName = line;

					getline(theFile, line);
					theUser.lastName = line;

					getline(theFile, line);
					theUser.email = line;

					getline(theFile, line);
					theUser.address = line;

					getline(theFile, line);
					theUser.phone = line;

					/*
					if(getline(theFile, line))
					{
					if (line.length() > 0)
					{
					while(line.at(0) != 'x' && line != "")
					{
					theUser.checkedOutMediaIds.push_back(atoi(line.c_str()));
					getline(theFile, line);
					}
					}
					}
					*/

					newList.push_back(theUser);			// add this user to new user list

				}
			}
			list = newList;					// replace current list with the new list

			cout << "Successfully read in " << list.size() << " users from file " << filename << endl;

			theFile.close();				// close file and return true (file read success)
			return true;
		}

		else				// failed to open file, so display error and return false (file read failure)
		{
			cout << "Couldn't open file " << filename << endl;
			return false;
		}
	}

	// write the current user list to a text file
	bool writeListToFile(string filename)
	{
		User theUser;

		ofstream theFile(filename);			// open file for writing

		if (theFile.is_open())
		{
			theFile << userIdCounter << endl;

			// iterate through all users and write them to file
			for (int i = 0; i < list.size(); i++)
			{
				theUser = list[i];					// get user

				//	write all user information to file
				theFile << theUser.userID << endl;
				theFile << theUser.userType << endl;
				theFile << theUser.sessionID << endl;
				theFile << theUser.username << endl;
				theFile << theUser.password << endl;
				theFile << theUser.firstName << endl;
				theFile << theUser.lastName << endl;
				theFile << theUser.email << endl;
				theFile << theUser.address << endl;
				theFile << theUser.phone << endl;

				/*
				for(unsigned int j = 0; j < theUser.checkedOutMediaIds.size(); j++)
				{
				theFile << theUser.checkedOutMediaIds[j] << endl;
				}
				theFile << "x" << endl;
				*/
				cout << "Successfully wrote user at index " << i << " to " << filename << endl;
			}

			theFile.close();				// close file and return true (file read success)
			return true;
		}

		else				// failed to open file, so display error and return false (file read failure)
		{
			cout << "Couldn't open file " << filename << endl;
			return false;
		}
	}

};

// facilitates operations on the User objects in UserList objects
class UserHandler
{
public:
	UserList theUserList;

	// UserHandler constructor
	UserHandler()
	{
	}

	// read user records into the user list from userDataFile (text file specified at top of program)
	bool readUsers()
	{
		return theUserList.readListFromFile(userDataFile);
	}

	// write current user records out to userDataFile (text file specified at the top of the program)
	bool writeUsers()
	{
		return theUserList.writeListToFile(userDataFile);
	}

	// get a user from the user list by their ID number
	User getUser(int id)
	{
		User theUser = theUserList.getUser(id);
		return theUser;
	}

	// add a new user to the user list
	int addUser(User theUser)
	{
		int newUserID;
		newUserID = theUserList.add(theUser);
		return newUserID;
	}

	bool deleteUser(int id)
	{
		theUserList.remove(id);
		return true;
	}

	bool editUser(int id, User editedUser)
	{
		theUserList.edit(id, editedUser);
		return true;
	}

	bool checkInMedia(int mediaID, int userID)
	{
		return false;
	}

	bool checkOutMedia(int mediaID, int userID)
	{
		theUserList.checkOut(mediaID, userID);
		return true;
	}

	void listAllUsers()
	{
		theUserList.listAll();
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
	vector<time_t> dueDates;		// due dates for copies that have been checked out
	vector<int> checkedOutUserIds;	// user IDs of users who have checked out a copy

	// constructor
	Media()
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

	void clear()		// empty out a media record
	{
		// initialize everything blank while we're debugging
		mediaID = -1;	// this will get assigned by add
		mediaType = 'x';
		isbn = "";
		title = "";
		author = "";
		subject = "";
		copies = 0;
	}

	// print the media's record to the console
	void displayInformation()
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

	void displayCheckedOutInformation()
	{
		if (checkedOutUserIds.size() > 0)
		{
			cout << "Checked out by: " << endl;
			for (unsigned int i = 0; i < checkedOutUserIds.size(); i++)
			{
				cout << "User " << checkedOutUserIds[i] << endl;
			}
			cout << "---------------" << endl;
		}
	}

};

// a list of Media objects	-	stand in for an actual database for the time being
class MediaList
{
public:
	vector<Media> list;
	int mediaIdCounter;

	MediaList()
	{
		mediaIdCounter = 0;
	}

	// get media by ID
	Media getMedia(int id)
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
	int getMediaIndex(int id)
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
	int add(Media theMedia)
	{
		theMedia.mediaID = mediaIdCounter; // media is assigned newest available ID based on count of Media added so far (mediaCounter)
		mediaIdCounter++;
		list.push_back(theMedia);
		return theMedia.mediaID;
	}

	bool remove(int id)
	{
		int targetIndex = getMediaIndex(id);
		list.erase(list.begin() + targetIndex);
		return true;
	}

	bool edit(int id, Media editedMedia)
	{
		int targetIndex = getMediaIndex(id);
		//editedMedia.displayCheckedOutInformation();
		list[targetIndex] = editedMedia;
		cout << endl << "NEW LIST" << endl;
		list[targetIndex].displayCheckedOutInformation();
		return true;
	}





	vector<int> getAllMediaIDs()
	{
		vector<int> MediaIDs;
		Media theMedia;

		for (int i = 0; i < list.size(); i++)
		{
			theMedia = list[i];
			MediaIDs.push_back(theMedia.mediaID);
		}
		return MediaIDs;
	}

	int getSize()
	{
		return list.size();
	}

	bool checkIn(int mediaID, int userID)
	{
		return false;
	}

	bool checkOut(int mediaID, int userID)
	{
		Media theMedia = getMedia(mediaID);
		if (theMedia.copies > 0)
		{
			theMedia.checkedOutUserIds.push_back(userID);
			theMedia.copies--;
			edit(mediaID, theMedia);
			cout << "\n\nCheck out succeeded, " << theMedia.copies << " copies left\n\n";
			return true;
		}
		else
		{
			cout << "\n\nCheck out failed, no copies left\n\n";
			return false;
		}
	}

	bool readListFromFile(string filename)
	{
		string line;
		string firstLine;

		vector<Media> newList;				// new list of media to read in to
		Media theMedia;

		ifstream theFile(filename);			// attempt to open file

		if (theFile.is_open())
		{
			if (getline(theFile, line))
			{
				mediaIdCounter = atoi(line.c_str());	// convert string to int for media ID counter value

				while (getline(theFile, firstLine))		// get the first line of the next media record if we're not at end of file
				{
					// read a Media record in from the file

					theMedia.mediaID = atoi(firstLine.c_str());		// convert string to int for media ID value

					getline(theFile, line);
					theMedia.mediaType = line.at(0);					// get first character of string for mediaType value

					getline(theFile, line);
					theMedia.isbn = line;

					getline(theFile, line);
					theMedia.title = line;

					getline(theFile, line);
					theMedia.author = line;

					getline(theFile, line);
					theMedia.subject = line;

					getline(theFile, line);
					theMedia.copies = atoi(line.c_str());

					/*
					if(getline(theFile, line))
					{
					if (line.length() > 0)
					{
					while(line.at(0) != 'x' && line != "")
					{
					theMedia.checkedOutUserIds.push_back(atoi(line.c_str()));
					getline(theFile, line);
					}
					}
					}*/


					/*
					if (line.at(0) != 'x')
					{

					while(line.at(0) != 'x')
					{
					theMedia.checkedOutUserIds.push_back(atoi(line.c_str()));
					getline(theFile, line);
					}

					}*/

					newList.push_back(theMedia);			// add this media to new media list
				}
			}

			list = newList;					// replace current list with the new list

			cout << "Successfully read in " << list.size() << " media from file " << filename << endl;

			theFile.close();				// close file and return true (file read success)
			return true;
		}

		else				// failed to open file, so display error and return false (file read failure)
		{
			cout << "Couldn't open file " << filename << endl;
			return false;
		}
	}

	// write the current media list to a text file
	bool writeListToFile(string filename)
	{
		Media theMedia;

		ofstream theFile(filename);			// open file for writing

		if (theFile.is_open())
		{
			theFile << mediaIdCounter << endl;

			// iterate through all medias and write them to file
			for (int i = 0; i < list.size(); i++)
			{
				theMedia = list[i];					// get media

				//	write all media information to file
				theFile << theMedia.mediaID << endl;
				theFile << theMedia.mediaType << endl;
				theFile << theMedia.isbn << endl;
				theFile << theMedia.title << endl;
				theFile << theMedia.author << endl;
				theFile << theMedia.subject << endl;
				theFile << theMedia.copies << endl;

				/*
				for(unsigned int j = 0; j < theMedia.checkedOutUserIds.size(); j++)
				{
				theFile << theMedia.checkedOutUserIds[j] << endl;
				}
				theFile << "x" << endl;
				*/

				cout << "Successfully wrote media at index " << i << " to " << filename << endl;
			}

			theFile.close();				// close file and return true (file read success)
			return true;
		}

		else				// failed to open file, so display error and return false (file read failure)
		{
			cout << "Couldn't open file " << filename << endl;
			return false;
		}
	}

	void listAll()
	{
		for (int i = 0; i < list.size(); i++)
		{
			list[i].displayInformation();
			list[i].displayCheckedOutInformation();
		}
	}

};

// facilitates operations on the Media objects in MediaList objects
class MediaHandler
{
public:
	MediaList theMediaList;

	// MediaHandler constructor
	MediaHandler()
	{
	}

	// read media records into the media list from mediaDataFile (text file specified at top of program)
	bool readMedia()
	{
		return theMediaList.readListFromFile(mediaDataFile);
	}

	// write current media records out to mediaDataFile (text file specified at the top of the program)
	bool writeMedia()
	{
		return theMediaList.writeListToFile(mediaDataFile);
	}


	// get a media from the media list by their ID number
	Media getMedia(int id)
	{
		Media theMedia = theMediaList.getMedia(id);
		return theMedia;
	}

	// add a new media to the media list
	int addMedia(Media theMedia)
	{
		int newMediaID;
		newMediaID = theMediaList.add(theMedia);
		return newMediaID;
	}

	bool deleteMedia(int id)
	{
		theMediaList.remove(id);
		return true;
	}

	bool editMedia(int id, Media editedMedia)
	{
		theMediaList.edit(id, editedMedia);
		return true;
	}

	void listAllMedia()
	{
		theMediaList.listAll();
	}

	bool checkInMedia(int mediaID, int userID)
	{
		theMediaList.checkIn(mediaID, userID);
		return true;
	}

	bool checkOutMedia(int mediaID, int userID)
	{
		theMediaList.checkOut(mediaID, userID);
		return true;
	}

};

class InterfaceHandler
{

};

// print the menu and get a command from user
char menu_select_get(int menu_type)
{
	char command = 'x';
	User theUser;
	switch (menu_type)
	{
	case 0:
		cout << "MEDIA MENU: " << endl;
		cout << "(a) Add media item" << endl;
		cout << "(d) Delete media item" << endl;
		cout << "(e) Edit media item" << endl;
		cout << "(l) List media items" << endl;
		cout << "(c) Check out media item" << endl;
		break;
	case 1:
		cout << "USER MENU: " << endl;
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
User menu_user_add()
{
	User editedUser;

	cout << "User type (a for admin, p for patron): ";
	cin >> editedUser.userType;
	cin.ignore(1, '\n');		// stop last cin from messing up future getline input by inserting a new line here

	cout << "Username: ";
	getline(cin, editedUser.username);

	cout << "Password: ";
	string password;
	getline(cin, password);
	editedUser.password = editedUser.passwordEncryptDecrypt(password);

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
User menu_user_edit(User theUser)
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
			editedUser = menu_user_add();		// use user add menu to get all new information for this user ID
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
			//cout << "Invalid command!" << endl;
			break;
		}

		//editedUser.userID = theUser.userID;
		//editedUser.sessionID = theUser.sessionID;
	}


	return editedUser;



}

// allow admin to input a new media record field by field
// NOTE: will want to validate these inputs eventually
Media menu_media_add()
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

Media menu_media_edit(Media theMedia)
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
			editedMedia = menu_media_add();		// use the media add menu to get all new info for this media ID
			// get media ID etc from original media record
			editedMedia.mediaID = theMedia.mediaID;
			break;

		case '1':
			cin >> editedMedia.mediaType;
			cin.ignore(1, '\n');		// stop last cin from messing up future getline input by inserting a new line here
			break;

		case '2':
			cout << "ISBN: ";
			getline(cin, editedMedia.isbn);
			break;


		case '3':
			cout << "Title: ";
			getline(cin, editedMedia.title);
			break;

		case '4':

			cout << "Author: ";
			getline(cin, editedMedia.author);
			break;

		case '5':
			cout << "Subject: ";
			getline(cin, editedMedia.subject);
			break;

		case '6':
			cout << "Copies: ";
			cin >> editedMedia.copies;
			break;

		default:
			break;
		}
	}
	return editedMedia;
}

vector<int> menuMediaSearch(MediaHandler theMediaHandler)
{
	vector<int> resultsList;
	char search_command = 'z';
	string search_term = "";
	char search_term_char = 'z';


	cout << "Select field to search by" << endl << endl;

	cout << "(1) Media Type " << endl;
	cout << "(2) ISBN number: " << endl;
	cout << "(3) Title: " << endl;
	cout << "(4) Author: " << endl;
	cout << "(5) Subject: " << endl;


	cin >> search_command;
	cin.ignore(1, '\n');		// stop last cin from messing up future getline input by inserting a new line here

	switch (search_command)
	{
	case '1':
		cin >> search_term_char;
		cin.ignore(1, '\n');		// stop last cin from messing up future getline input by inserting a new line here
		break;

	case '2':
		cout << "ISBN: ";
		getline(cin, search_term);
		break;

	case '3':
		cout << "Title: ";
		getline(cin, search_term);
		break;

	case '4':
		cout << "Author: ";
		getline(cin, search_term);
		break;

	case '5':
		cout << "Subject: ";
		getline(cin, search_term);
		break;

	default:
		break;
	}

	return resultsList;
}



Media menuMediaCheckOut(MediaHandler& theMediaHandler, UserHandler& theUserHandler)
{


	int userID;
	int mediaID;

	theMediaHandler.listAllMedia();
	cout << "Media ID to check out: ";
	cin >> mediaID;
	theUserHandler.listAllUsers();
	cout << "User ID checking this out: ";
	cin >> userID;
	cin.ignore(1, '\n');		// stop last cin from messing up future getline input by inserting a new line here

	Media editedMedia = theMediaHandler.getMedia(mediaID);
	theMediaHandler.checkOutMedia(mediaID, userID);
	theUserHandler.checkOutMedia(mediaID, userID);

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
	testUser.password = testUser.passwordEncryptDecrypt("password");
	testUser.firstName = "Nick";
	testUser.lastName = "Frogley";
	testUser.email = "nickfro@gmail.com";
	testUser.address = "111 Mansfield Hollow Rd, Mansfield Center, CT 06250";
	testUser.phone = "860 214 9523";

	//theUserHandler.addUser(testUser);			// add first user
	//theUserHandler.writeUsers();
	//	return 0;


	theUserHandler.readUsers();


	Media testMedia;			// initialize with a media for testing
	testMedia.mediaType = 'b';
	testMedia.isbn = "0553386794";
	testMedia.title = "Game of Thrones";
	testMedia.author = "George R. R. Martin";
	testMedia.subject = "Fantasy";
	testMedia.copies = 5;

	//theMediaHandler.addMedia(testMedia);	// add first media

	theMediaHandler.readMedia();



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
			vector <int> mediaReuslts;

			switch (menu_command)
			{
			case 'a':
				int newMediaID;
				theMedia = menu_media_add();
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
				} while (theMedia.mediaID == -1);

				editedMedia = menu_media_edit(theMedia);
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
			case 'c':
				editedMedia = menuMediaCheckOut(theMediaHandler, theUserHandler);
				//editedMedia.displayInformation();
				break;
			case 's':
				mediaReuslts = menuMediaSearch(theMediaHandler);
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
				theUser = menu_user_add();
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
				} while (theUser.userID == -1);

				editedUser = menu_user_edit(theUser);
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

	theUserHandler.writeUsers();
	theMediaHandler.writeMedia();

	return 0;
}
