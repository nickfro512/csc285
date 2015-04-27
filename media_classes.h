#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

// text file to get media records from
const string MEDIA_DATA_FILE = "media_data.txt";

// time periods coverted to seconds to use when altering due dates
const long SECONDS_IN_A_DAY = (60 * 60 * 24);
const long SECONDS_IN_A_WEEK = (SECONDS_IN_A_DAY * 7); 
const long SECONDS_IN_THIRTY_DAYS = (SECONDS_IN_A_DAY * 30); 

const long CHECKOUT_PERIOD_IN_SECONDS = SECONDS_IN_THIRTY_DAYS;		// sets how long a media item gets checked out for
const long RENEWAL_PERIOD_IN_SECONDS = SECONDS_IN_A_WEEK;			// sets how long a media item can be renewed for by user after initial checkout

class UserHandler;

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
    
	// display information about who has checked out this media item and when it's due
    void displayCheckedOutInformation()
   	{
		time_t rawTime;		// need these time objects to get the renewal dates and display them
		tm *readableTime;

		time_t currentTime = time(0);	// get current time to check if anything's overdue

        if (checkedOutUserIds.size() > 0)
        {
            cout << "Checked out by: " << endl;
            for (unsigned int i = 0; i < checkedOutUserIds.size(); i++)
            {
				rawTime = dueDates[i];
				readableTime = localtime(&rawTime);
                cout << "User " << checkedOutUserIds[i] << "\t\t" << "Due: " << asctime(readableTime);
				if (currentTime >= dueDates[i])
				{
					cout << " ! OVERDUE";
				}
				cout << endl;
            }
            cout << "---------------" << endl;
        }

		//delete readableTime; ptrdel
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
    
	// check out a media item for a user - stores user ID and a due date in the media record
    bool checkOut(int mediaID, int userID)
    {
		UserHandler theUserHandler;			// user handler for checking if user has alraedy checked something out

        Media theMedia = getMedia(mediaID);

        if (theMedia.copies > 0)
        {
			time_t dueDate = time(0) + SECONDS_IN_THIRTY_DAYS;		// set due date to 30 days after checkout
			
			// set due time to midnight on due date
			tm *dueDateTm = localtime(&dueDate);
			dueDateTm->tm_hour = 0;
			dueDateTm->tm_min = 0;
			dueDateTm->tm_sec = 0;
			dueDate = mktime(dueDateTm);
			//delete dueDateTm;	//ptrdel

            theMedia.checkedOutUserIds.push_back(userID);
			theMedia.dueDates.push_back(dueDate);
			theMedia.copies--;

            edit(mediaID, theMedia);								// update the media record with new checkout ID and due date
			
            cout << "\n\nCheck out succeeded, " << theMedia.copies << " copies left\n\n";
            return true;
        }
        else
        {
            cout << "\n\nCheck out failed, no copies left\n\n";
            return false;
        }
    }

	// check in a media item for a user - remove user id from checked out id list and their due date from due date list
	bool checkIn(int mediaID, int userID)
    {
		Media theMedia = getMedia(mediaID);
		for (unsigned int i = 0; i < theMedia.checkedOutUserIds.size(); i++)
		{
			if (theMedia.checkedOutUserIds[i] == userID)
			{
				theMedia.checkedOutUserIds.erase(theMedia.checkedOutUserIds.begin() + i);	// delete this user ID in the checked out list
				theMedia.dueDates.erase(theMedia.dueDates.begin() + i);						// delete due date for this user ID in the due date list
				theMedia.copies++;
				edit(mediaID, theMedia);	// update the media record
				cout << "Check in succeeded for User ID " << userID << " on mediaID " << mediaID << endl;
				return true;
			}
		}
		cout << "Checkin failed! Couldn't find user in checked out ID list" << endl;
		return false;
    }

	// renew a media item (add another week to the due date)
	bool renew(int mediaID, int userID)
    {
		Media theMedia = getMedia(mediaID);
		for (unsigned int i = 0; i < theMedia.checkedOutUserIds.size(); i++)
		{
			if (theMedia.checkedOutUserIds[i] == userID)
			{
				theMedia.dueDates[i] += SECONDS_IN_A_WEEK;						// delete due date for this user ID in the due date list
				edit(mediaID, theMedia);	// update the media record
				cout << "Renewal succeeded" << endl;
				return true;
			}
		}
		cout << "Renewal failed! Couldn't find user in checked out ID list" << endl;
		return false;
    }
    
    bool readListFromFile(string filename)
    {
        string line;
        string firstLine;
        
        vector<Media> newList;				// new list of media to read in to
        
        
        ifstream theFile(filename);			// attempt to open file
        
        if (theFile.is_open())
        {
            if (getline(theFile, line))
            {
                mediaIdCounter = atoi(line.c_str());	// convert string to int for media ID counter value
                
                while(getline(theFile, firstLine))		// get the first line of the next media record if we're not at end of file
                {
                    // read a Media record in from the file
					Media theMedia;		// create a fresh media record for this record in the file

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
                    
                    
                    if(getline(theFile, line))
                    {
						if (line.length() > 0)
						{
							while(line.at(0) != 'x' && line != "")
							{
								//cout << "line is:  " << line;
								theMedia.checkedOutUserIds.push_back(atoi(line.c_str()));
								cout << " *** added: " << atoi(line.c_str()) << endl;
								
								time_t dueDate = time(0);
								tm *dueDateTm = localtime(&dueDate);
								
								// read in due date
		
								getline(theFile, line);
								dueDateTm->tm_year = atoi(line.c_str());

								getline(theFile, line);
								dueDateTm->tm_mon = atoi(line.c_str());

								getline(theFile, line);
								dueDateTm->tm_mday = atoi(line.c_str());

								// set due time to midnight on due date
								dueDateTm->tm_hour = 0;
								dueDateTm->tm_min = 0;
								dueDateTm->tm_sec = 0;
								
								dueDate = mktime(dueDateTm);
								//delete dueDateTm;	//ptrdel

								theMedia.dueDates.push_back(dueDate);
								cout << "added due date: " << asctime(dueDateTm) << endl;

								getline(theFile, line);
								
							}
						}
						cout << "finished media checkout I/O for media ID " << theMedia.mediaID << endl;
                    }
					else
					{
						cout << endl << "no line!";
					}
                    
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
            
            // iterate through all media and write them to file
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
                
                // write out checked out media information (checked out user ID and raw unix time due date for each record)
                for(unsigned int j = 0; j < theMedia.checkedOutUserIds.size(); j++)
                {
					theFile << theMedia.checkedOutUserIds[j] << endl;	// write user ID

					tm *dueDate = localtime(&theMedia.dueDates[j]);
					theFile << dueDate->tm_year << endl;				// write due date year
					theFile << dueDate->tm_mon << endl;					// write due date month
					theFile << dueDate->tm_mday << endl;				// write due date day
					//delete dueDate; ptrdel
                }
                theFile << "x" << endl;		// write x to indicate end of data for this record
                                
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
    
    // read media records into the media list from MEDIA_DATA_FILE (text file specified at top of program)
    bool readMedia()
    {
        return theMediaList.readListFromFile(MEDIA_DATA_FILE);
    }
    
    // write current media records out to MEDIA_DATA_FILE (text file specified at the top of the program)
    bool writeMedia()
    {
        return theMediaList.writeListToFile(MEDIA_DATA_FILE);
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
