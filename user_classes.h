#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

// text file to get user records from
const string USER_DATA_FILE = "user_data.txt";

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
        cout << "Password: " << password << endl;
        cout << "Name: " << firstName << " " << lastName << endl;
        cout << "Email: " << email << endl;
        cout << "Address: " << address << endl;
        cout << "Phone number: " << phone << endl;
        
        cout << "------------------" << endl;
    }
    
    // when user first creates a password, password = passwordEncryptDecrypt(password), that will encrypt it
    // the password will then be stored in an encrypted state, then call passwordEncryptDecrypt(password) to
    // display the dcrypted password without changing the password within the 'database'
    string passwordEncryptDecrypt(string passwordIn)
    {
        char key[3] = { 'K', 'C', 'Q' }; //Any chars will work
        string passwordOut = passwordIn;
        for (int i = 0; i < passwordIn.size(); i++)
            passwordOut[i] = passwordIn[i] ^ key[i % (sizeof(key) / sizeof(char))];
        return passwordOut;
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
    
	// check out a media item on a user record - add the media ID to the user's checked out media ID list
    bool checkIn(int mediaID, int userID)
    {
        User theUser = getUser(userID);
		for (unsigned int i = 0; i < theUser.checkedOutMediaIds.size(); i++)
		{
			if (theUser.checkedOutMediaIds[i] == mediaID)
			{
				theUser.checkedOutMediaIds.erase(theUser.checkedOutMediaIds.begin() + i);	// delete this media ID in the checked out list
				edit(userID, theUser);	// update the user record
				cout << "User Record Check in succeeded for User ID " << userID << " on mediaID " << mediaID << endl;
				return true;
			}
		}
		cout << "Renewal failed! Couldn't find media in checked out ID list" << endl;
		return false;
    }
    
	// check out a media item on a user record - add the media ID to the user's checked out media ID list
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
        
        ifstream theFile(filename);			// attempt to open file
        
        if (theFile.is_open())
        {
            if (getline(theFile, line))
            {
                userIdCounter = atoi(line.c_str());	// convert string to int for user ID counter value (gets used to assign new user IDs)
                
                while(getline(theFile, firstLine))		// get the first line of the next user record if we're not at end of file
                {
                    // read a User record in from the file

					User theUser;		// create a fresh user record for this record in the file
                    
                    theUser.userID = atoi(firstLine.c_str());		// convert string to int for user ID value
                    
                    getline(theFile, line);
                    theUser.userType = line.at(0);					// get first character of string for userType value
                    
                    getline(theFile, line);
                    theUser.sessionID = atoi(line.c_str());		// convert string to int for session ID value
                    
                    getline(theFile, line);
                    theUser.username = line;
                    
                    getline(theFile, line);
                    theUser.password = line;
                    theUser.password = theUser.passwordEncryptDecrypt(theUser.password);
                    
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
                    
                    if(getline(theFile, line))
                    {
						if (line.length() > 0)
						{
							while(line.at(0) != 'x' && line != "")
							{
								cout << "line is:  " << line;
								theUser.checkedOutMediaIds.push_back(atoi(line.c_str()));
								cout << " *** added: " << atoi(line.c_str()) << endl;
								getline(theFile, line);
							}
						}
						cout << "finished user checkout I/O for user ID " << theUser.userID << endl;
                    }
					else
					{
						cout << endl << "no line!";
					}
                    
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
                theFile << theUser.passwordEncryptDecrypt(theUser.password) << endl;
                //theFile << theUser.password << endl;
				theFile << theUser.firstName << endl;
                theFile << theUser.lastName<< endl;
                theFile << theUser.email << endl;
                theFile << theUser.address << endl;
                theFile << theUser.phone << endl;
                
                
                for(unsigned int j = 0; j < theUser.checkedOutMediaIds.size(); j++)
                {
					theFile << theUser.checkedOutMediaIds[j] << endl;
                }
                theFile << "x" << endl;
                 
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
    
    // read user records into the user list from USER_DATA_FILE (text file specified at top of program)
    bool readUsers()
    {
        return theUserList.readListFromFile(USER_DATA_FILE);
    }
    
    // write current user records out to USER_DATA_FILE (text file specified at the top of the program)
    bool writeUsers()
    {
        return theUserList.writeListToFile(USER_DATA_FILE);
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
        return theUserList.checkIn(mediaID, userID);
    }
    
    bool checkOutMedia(int mediaID, int userID)
    {
        return theUserList.checkOut(mediaID, userID);
    }
    
    void listAllUsers()
    {
		theUserList.listAll();
    }
};

