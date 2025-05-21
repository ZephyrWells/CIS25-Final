#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <ctime>
using namespace std;


// INITIALIZING FUNCTIONS

void Write();
void Read(bool& secondTime);
bool fileExists(const string& filename);
string getDate();


int main()
{
	// INITIALIZING VARIABLES
	int writeOrRead;
	string title;
	bool quit = false;
	bool secondRead = false;
	
	// checking if the entries file exists and creating it if it doesn't
	if (!fileExists("entries.txt")) {
		ofstream outfile("entries.txt");
		outfile.close();
	}
	
	// while loop will repeat the switch case until the user chooses to quit
	while (!quit) {

		// getting user input
		cout << "What would you like to do?" << endl << endl;
		cout << "1. Write a journal entry" << endl << "2. Read a journal entry" << endl <<
			"3. Quit Journal program" << endl << endl << "Input choice now: ";
		cin >> writeOrRead;

		// making sure the user input is read correctly
		if (!cin >> writeOrRead) {
			// clearing the user input if it is outside of the bounds of the switch case
			for (int i = 0; i < to_string(writeOrRead).length(); i++) {
				cin.clear();
				cin.ignore();
			}
		}

		switch (writeOrRead)
		{
		case 1:
			Write();
			break;

		case 2:
			Read(secondRead);
			break;

		case 3:
			cout << endl << endl << "Thank you for using my program!" << endl << "Goodbye!" << endl 
				<< "----------------------------------------------------------------------------------------------------";
			quit = true; // while loop check will fail and the program will end
			break;

		default: cout << endl << "INVALID ANSWER" << endl << endl;
		}
	}

	return 0;
}


// FUNCTIONS

void Write()
{
	// INITIALIZING VARIABLES
	string title;
	string entry;
	ofstream entriesOut;

	// getting the title for the entry
	cout << endl << "What is the title of this entry?" << endl << endl;
	cin.ignore();
	getline(cin, title);
	
	// inputting the entry title into the entries file with today's date
	entriesOut.open("entries.txt", ios_base::app);
	entriesOut << title << " --- " << getDate() << endl;
	entriesOut.close();

	// creating a file with the title of the entry
	title += ".txt";
	ofstream outfile(title);

	// getting the entry and inputting it into the file
	cout << endl << "Start your entry now. Press enter when it is complete" << endl <<
		"----------------------------------------------------------------------------------------------------" << endl;
	getline(cin, entry);
	outfile << entry << endl;
	outfile.close();

	cout << "----------------------------------------------------------------------------------------------------"
		<< endl << "Entry saved!" << endl << endl;
}

void Read(bool& secondTime)
{
	// INITIALIZING VARIABLES
	string entryList;
	string wantedTitle;
	string entry;
	bool back = false;
	
	// while loop will repeat until the user chooses one of the files
	while (!back) {
		cout << endl << "Type the full name of the entry you want to read, or type \"BACK\" to return to the home page:" 
			<< endl << endl;

		// getting the names of all the entries from the entries file and listing them
		ifstream entryStream("entries.txt");
		while (getline(entryStream, entryList)) {
			cout << entryList << endl;
		}
		cout << endl;

		// only running the cin.ignore() command on the second time the Read function is called
		if (!secondTime) {
			cin.ignore();
		}
		getline(cin, wantedTitle);
		wantedTitle += ".txt";

		// getting the entry file if it exists and reading out the entry
		if (fileExists(wantedTitle)) {
			ifstream inputStream(wantedTitle);

			getline(inputStream, entry);

			cout << "----------------------------------------------------------------------------------------------------"
				<< endl << entry << endl
				<< "----------------------------------------------------------------------------------------------------" << endl
				<< "END OF ENTRY" << endl << endl << endl;
			secondTime = true; // next time the function is called the cin.ignore() command will run
		}
		else if (wantedTitle == "BACK.txt") {
			back = true; // while loop check will fail
			secondTime = true; // next time the function is called the cin.ignore() command will run
			cout << endl << endl;
		}
		else {
			// if the user chooses an entry which does not exist, will return to asking them to input an entry
			cout << endl << "No entry of that name exists, try again" << endl;
		}
	}
}

// checking to see if a file exists and returning true or false accordingly
bool fileExists(const std::string& filename) {
	struct stat buf;

	if (stat(filename.c_str(), &buf) != -1) 
	{
		return true;
	}
	return false;
}

string getDate() {
	// INITIALIZING VARIABLES
	time_t TSE = time(NULL); // TSE (Time Since Epoch) is a variable tracking the time in seconds since Jan 1 00:00:00
	int secPerYear = 31536000;
	int secPerDay = 86400;
	int currentYear;
	int currentMonth;
	int currentDay;
	int numLeapYears;
	bool isLeapYear = false;
	string date;

	// getting the current year and subtracting it from the TSE
	currentYear = 1970 + (TSE / secPerYear);
	TSE -= (currentYear - 1970) * secPerYear;

	// dealing with leap years
	numLeapYears = (currentYear - 1972) / 4;
	if (numLeapYears % 4 == 0) {
		numLeapYears--;
		isLeapYear = true;
	}
	TSE -= numLeapYears * secPerDay;
	if (isLeapYear) {
		TSE -= secPerDay;
	}

	// getting the current month and subtracting it from the TSE
	if (TSE <= 2678400) { currentMonth = 1; }
	else if (TSE <= 5097600) { currentMonth = 2; TSE -= 2678400; }
	else if (TSE <= 7776000) { currentMonth = 3; TSE -= 5097600; }
	else if (TSE <= 10368000) { currentMonth = 4; TSE -= 7776000; }
	else if (TSE <= 13046400) { currentMonth = 5; TSE -= 10368000; }
	else if (TSE <= 15638400) { currentMonth = 6; TSE -= 13046400; }
	else if (TSE <= 18316800) { currentMonth = 7; TSE -= 15638400; }
	else if (TSE <= 20995200) { currentMonth = 8; TSE -= 18316800; }
	else if (TSE <= 23587200) { currentMonth = 9; TSE -= 20995200; }
	else if (TSE <= 26265600) { currentMonth = 10; TSE -= 23587200; }
	else if (TSE <= 28857600) { currentMonth = 11; TSE -= 26265600; }
	else { currentMonth = 12; TSE -= 28857600; }


	if (isLeapYear && currentMonth <= 2) {
		TSE += secPerDay;
	}

	currentDay = TSE / secPerDay;
	if (currentMonth == 1 && currentDay == 32) {
		currentMonth = 2;
		currentDay = 31;
	}

	// returning the date as a string
	date = to_string(currentMonth) + '/' + to_string(currentDay) + '/' + to_string(currentYear);
	return date;
}