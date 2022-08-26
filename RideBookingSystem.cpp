#include<iostream>
#include<fstream>
#include<conio.h>
#include<string>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<windows.h>
using namespace std;

//Constants
const int RPOINTS = 9;
const int DISTS = 6;

//A structure of destinations
struct Destinations{
	string district, ridePoints[RPOINTS];
}locations[DISTS];

//A structure of driver and driver's car information
struct Driver{
	string name, licensePlate, carName, color;
}drivers[50];

//A structure of user information
struct User{
	bool hasRide;
	int users, ride, hours[3], minutes[3], drvrs[3], prices[3];
	string userName, email, password, district, location, phone;
}users[100];

//User Defined Functions' Prototype
void welcome();
void header(string);
void footer();
void bookRide();
void displayRides(int, int);
void dispBooked();
void genRandomTime(int);
void genRandomPrice(int);
void updateData();
void clearFiles();
void setConsole();
int getNoOfRides();
int getRandomDriver();
bool readData();
bool saveUser(string, string, string, int, int, string);
bool signIn(int);
bool delAccount();

//Global Variables
bool run, logged, isNear = false;
char heart = 3; //3 is an ascii for heart
int price, noOfDrivers = 0, rideChoice, user;

//Main Body
int main(){
	// setConsole(); //Does not work on vs code
	string choice;
	srand(time(0)); //Initializing a random 'seed' for each time rand() function is called
	
	system("Color F0"); //F is the hexa value for white and 0 for black
	welcome();
	// clearFiles(); //Function to clear user data
	users[0].users = 0; //Initializing number of users to zero
	run = readData(); //Reading existing data (returns false if read unsuccessful)
	
	for(int i = 0; i < users[0].users; i++){
		users[i].hasRide = false;
	}

	while(true){
		header("SignUp Menu");
		cout<<"\n\t\t Please select what you want to do:";
		cout<<"\n\n\t\t 1. Login\n\t\t 2. Register: ";
		cin>>choice;
		if(!isdigit(choice[0])){
			cout<<"\n\t\t Invalid Input!, press any key...";
			getch();
		}
		else if(stoi(choice) > 2 || stoi(choice) < 1){
			cout<<"\n\t\t Invalid Input!, press any key...";
			getch();
		}
		else{
			break;
		}
	}

	logged = signIn(stoi(choice));
	
	if(!logged){
		cout<<"\n\t\t You must login before using this app. Exiting...";
		sleep(1.5);
	}

	while(run and logged){
		
		header("HOME MENU");
		cout<<"\t\t Welcome "<<users[user].userName<<"!";
		cout<<"\n\n\t\t Please specify what you want to do below:"<<endl;
		cout<<"\t\t 1. Book a Ride\n\t\t 2. Show Booked Rides\n\t\t 3. Login To Another Account\n\t\t 4. Create New Account\n\t\t 5. Delete Current Account\n\t\t 6. Quit : ";
		cin>>choice;

		if(!isdigit(choice[0])){
			cout<<"\n\t\t Invalid Input!, Please enter an integer value for choice...";
			getch();
			continue;
		}

		switch(stoi(choice)){
			case 1:
				bookRide();
				break;
			case 2:
				dispBooked();
				break;
			case 3:
				cout<<"\n\t\t Logging out...";
				sleep(1.5);
				logged = signIn(1);
				if(!logged){
					cout<<"\n\t\t You must login before using this app. Exiting...";
					sleep(1.5);
				}
				break;
			case 4:
				cout<<"\n\t\t Logging out...";
				sleep(1.5);
				logged = signIn(2);
				if(!logged){
					cout<<"\n\t\t You must login before using this app. Exiting...";
					sleep(1.5);
				}
				break;
			case 5:
				if(delAccount()){
					while(true){
						header("SignUp Menu");
						cout<<"\n\t\t Please select what you want to do:";
						cout<<"\n\n\t\t 1. Login\n\t\t 2. Register: ";
						cin>>choice;
						if(!isdigit(choice[0])){
							cout<<"\n\t\t Invalid Input!, press any key...";
							getch();
						}
						else if(stoi(choice) > 2 || stoi(choice) < 1){
							cout<<"\n\t\t Invalid Input!, press any key...";
							getch();
						}
						else{
							break;
						}
					}
					logged = signIn(stoi(choice));
					if(!logged){
						cout<<"\n\t\t You must login before using this app. Exiting...";
						sleep(1.5);
					}
				}
				break;
			case 6:
				cout<<"\n\n\t\t Happy Riding! "<<heart;
				run = false;
				logged = false;
				break;
			default:
				cout<<"\n\t\t Invalid Choice!, Please enter from the given list!, press enter to continue...";
				getch();
		}
	}
	
	footer();
	
	return 0;
}

/***********************User Defined Function Bodies**************************/

void dispBooked(){
	if(!users[user].hasRide){
		cout<<"\n\n\t\t There is no booked ride yet, press any key...";
		getch();
		return;
	}
	bool arrived = false;
	time_t currTime = time(0);
	tm lclTime = *localtime(&currTime);
	int minLeft = users[user].minutes[users[user].ride] - lclTime.tm_min;
	
	while(!arrived){
		header("Booked Ride");
		displayRides(users[user].ride, users[user].drvrs[users[user].ride]);
		if(minLeft < 0)
			minLeft = 0;
		cout<<"\n\t\t Your driver will arrive in "<<minLeft<<" minutes!";
		if(minLeft == 0)
			break;
		sleep(5);
		minLeft -= 3;
	}
	users[user].hasRide = false;
	cout<<"\n\n\t\t Your driver has arrived at your location!.\n\t\t You will soon receive a call at: "<<users[user].phone<<".\n\t\t Please guide them to your bus stop when they call. Happy Riding "<<heart;
	cout<<"\n\t\t Press any key to continue...";
	getch();
}

bool signIn(int choice){
	string email, password, cnfrmPass;
	bool correct = false;
	
	RELOG:
	correct = false;
	if(choice == 1){
		header("Login Menu");
		cout<<"\n\t\t Please enter your email: ";
		cin.clear(); cin.sync();
		getline(cin, email);

		bool isDomain = false;
		for(int i = 0; i < email.length(); i++){
			if(email[i] == '@'){
				isDomain = true;
				break;
			}
		}
		//Adding domain automatically if not present
		if(!isDomain){ 
			email += "@gmail.com";
		}

		for(int i = 0; i < users[0].users; i++){
			if(email == users[i].email){
				user = i;
				while(!correct){
					cout<<"\t\t Please enter your password: ";
					while(true){
						char ch = getch();
						if(ch == 8){
							cout<<"\b \b";
							password += '\b';
						}
						if(ch >= 32 && ch <= 126){ //space, special characters, alphabets and numerals
							password += ch;
							cout<<"*";
						}
						if(ch == '\r'){
							password += '\0';
							break;
						}
					}
					for(int i = 0; i < password.length(); i++){
						if(password[i] == users[user].password[i]){
							correct = true;
						}
						else{
							correct = false;
							break;
						}
					}
					if(!correct){
						cout<<"\n\t\t Incorrect Password!, please Re-Enter...";
						password = "\0";
						getch();
					}
				}
				break;
			}
		}
		if(!correct){
			char ch;
			cout<<"\n\t\t The email entered is not registered to this app. Please register before login.\n\t\t Do you want to re-enter? (y/n): ";
			cin>>ch;
			if(ch == 'y' || ch == 'Y'){
				goto RELOG;
			}
			else{
				cout<<"\n\t\t Do you want to register? (y/n): ";
				cin>>ch;
				if(ch == 'y' || ch == 'Y'){
					choice = 2;
				}
				else{
					return false;
				}
			}
		}
		else{
			cout<<"\n\t\t Logged In Successfully!, Press any key to continue...";
			getch();
			return true;
		}
	}
	if(choice == 2){
		string usrName, phone, choice;
		int dist, loc;
		bool correct = false;
		
		header("Registration Menu");
		
		cout<<"\n\t\t Please enter your username: ";
		cin.clear(); cin.sync(); //Clearing cin in order to use a new input method
		getline(cin, usrName);
		
		for(int i = 0; i < users[0].users; i++){
			if(usrName == users[i].userName){
				cout<<"\n\t\t The user name is already taken please enter a different one: ";
				getline(cin, usrName);
				i = -1;
			}
			else{
				break;
			}
		}
		
		cout<<"\n\t\t Please enter your email: ";
		getline(cin, email);

		bool isDomain = false;
		for(int i = 0; i < email.length(); i++){
			if(email[i] == '@'){
				isDomain = true;
				break;
			}
		}
		//Adding domain automatically if not present
		if(!isDomain){ 
			email += "@gmail.com";
		}

		for(int i = 0; i < users[0].users; i++){
			if(email == users[i].email){
				cout<<"\n\t\t The email already exists please enter a different one: ";
				getline(cin, email);
				for(int i = 0; i < email.length(); i++){
					if(email[i] == '@'){
						isDomain = true;
						break;
					}
				}
				//Adding domain automatically if not present
				if(!isDomain){ 
					email += "@gmail.com";
				}
				i = -1;
			}
			else{
				break;
			}
		}
		
		while(true){
			cout<<"\t\t Please enter your password: ";
			while(true){
				char ch = getch();
				if(ch == 8){
					cout<<"\b \b";
					password += '\b';
				}
				if(ch >= 32 && ch <= 126){ //space, special characters, alphabets and numerals
					password += ch;
					cout<<"*";
				}
				if(ch == '\r'){
					password += '\0';
					break;
				}
			}
			cout<<"\n\t\t Please re-enter the same password: ";
			while(true){
				char ch = getch();
				if(ch == 8){
					cout<<"\b \b";
					cnfrmPass += '\b';
				}
				if(ch >= 32 && ch <= 126){ //space, special characters, alphabets and numerals
					cnfrmPass += ch;
					cout<<"*";
				}
				if(ch == '\r'){
					cnfrmPass += '\0';
					break;
				}
			}
			if(cnfrmPass == password){
				break;
			}
			else{
				cout<<"\t\t The passwords do not match! Please re-enter!\n";
				password = "\0";
				cnfrmPass = "\0";
			}
		}
		
		cout<<"\n\n\t\t Please select your district:\n";
		while(true){
			for(int i = 0; i < DISTS; i++){
				cout<<"\n\t\t "<<i+1<<" "<<locations[i].district;
			}
			cout<<"\n\t\t Choice: ";
			cin>>choice;
			if(!isdigit(choice[0])){
				cout<<"\n\t\t Invalid Input!, Please enter an integer value for choice...";
				getch();
			}
			else if(stoi(choice) < 1 || stoi(choice) > DISTS){
				cout<<"\n\t\t Invalid Input!, Please enter from the given choices!";
			}
			else{
				dist = stoi(choice);
				break;
			}
		}
		
		cout<<"\n\t\t Please select your location:";
		while(true){
			int points = 0;
			for(int i = 0; locations[dist-1].ridePoints[i] != "\0"; i++){
				cout<<"\n\t\t "<<i+1<<" "<<locations[dist-1].ridePoints[i];
				points++;
			}
			cout<<"\n\t\t Choice: ";
			cin>>choice;
			if(!isdigit(choice[0])){
				cout<<"\n\t\t Invalid Input!, Please enter an integer value for choice...";
				getch();
			}
			else if(stoi(choice) < 1 || stoi(choice) > points){
				cout<<"\n\t\t Invalid Input!, Please enter from the given choices...";
				getch();
			}
			else{
				loc = stoi(choice);
				break;
			}
		}
		
		while(true){
			bool isCorrect = true;
			cout<<"\n\t\t Please enter your phone number (Necessary for the driver to contact you): ";
			cin>>phone;

			for(int i = 0; i < phone.length(); i++){
				if(!isdigit(phone[i])){
					isCorrect = false;
					break;
				}
			}

			if(!isCorrect){
				cout<<"\n\t\t Invalid Phone Number!, Please Re-Enter...";
				getch();
			}
			else{
				break;
			}
		}
		
		user = users[0].users;
		users[0].users++;

		//Assigning registration details to struct
		users[user].userName = usrName;
		users[user].email = email;
		users[user].password = password;
		users[user].district = locations[dist-1].district;
		users[user].location = locations[dist-1].ridePoints[loc-1];
		users[user].phone = phone;
		
		if(saveUser(usrName, email, password, dist-1, loc-1, phone)){
			users[user].hasRide = false;
			cout<<"\n\t\t Registered Successfully!, Press any key to continue...";
			getch();
			return true;
		}
		else{
			cout<<"\n\t\t There is a problem registering you to the app, try again later. Press any key to continue...";
			getch();
			return false;
		}
	}
}

bool saveUser(string usrName, string email, string password, int dist, int loc, string phone){
	ofstream nameFile, emlFile, pswdFile, distFile, locFile, phneFile;
	
	//ios_base:: defines how the file should be opened, 'app' refers to appending
	nameFile.open("Database/User Info/username.txt", ios_base::app);
	emlFile.open("Database/User Info/email.txt", ios_base::app);
	pswdFile.open("Database/User Info/password.txt", ios_base::app);
	distFile.open("Database/User Info/district.txt", ios_base::app);
	locFile.open("Database/User Info/location.txt", ios_base::app);
	phneFile.open("Database/User Info/phone.txt", ios_base::app);
	
	if(nameFile.is_open() and emlFile.is_open() and pswdFile.is_open() and distFile.is_open() and locFile.is_open() and phneFile.is_open()){
		nameFile<<usrName<<endl; //Writing to the file using the insertion operator
		nameFile.close();
		emlFile<<email<<endl;
		emlFile.close();
		pswdFile<<password<<endl;
		pswdFile.close();
		distFile<<locations[dist].district<<endl;
		distFile.close();
		locFile<<locations[dist].ridePoints[loc]<<endl;
		locFile.close();
		phneFile<<phone<<endl;
		phneFile.close();
		
		return true;
	}
	else{
		return false;
	}
}

void updateData(){
	ofstream nameFile, emlFile, pswdFile, distFile, locFile, phneFile;
	
	nameFile.open("Database/User Info/username.txt");
	emlFile.open("Database/User Info/email.txt");
	pswdFile.open("Database/User Info/password.txt");
	distFile.open("Database/User Info/district.txt");
	locFile.open("Database/User Info/location.txt");
	phneFile.open("Database/User Info/phone.txt");
	
	if(nameFile.is_open() and emlFile.is_open() and pswdFile.is_open() and distFile.is_open() and locFile.is_open() and phneFile.is_open()){
		for(int i = 0; i < users[0].users; i++){
			nameFile<<users[i].userName;
			emlFile<<users[i].email;
			pswdFile<<users[i].password;
			distFile<<users[i].district;
			locFile<<users[i].location;
			phneFile<<users[i].phone;
			if(i < users[0].users-1){
				nameFile<<endl;
				emlFile<<endl;
				pswdFile<<endl;
				distFile<<endl;
				locFile<<endl;
				phneFile<<endl;
			}
		}
		nameFile.close();
		emlFile.close();
		pswdFile.close();
		distFile.close();
		locFile.close();
		phneFile.close();
	}
}

bool delAccount(){
	char ch;
	header("Delete Account");
	cout<<"\t\t Are you sure you want to delete this account? (y/n): ";
	cin>>ch;
	if(ch == 'y' || ch == 'Y'){
		for(int i = user; i < users[0].users; i++){
			if(i < users[0].users - 1){
				users[i].userName = users[i+1].userName;
				users[i].email = users[i+1].email;
				users[i].password = users[i+1].password;
				users[i].district = users[i+1].district;
				users[i].location = users[i+1].location;
				users[i].phone = users[i+1].phone;
				users[i].hasRide = users[i+1].hasRide;
			}
			else if(i == users[0].users - 1){
				users[i].userName = "\0";
				users[i].email = "\0";
				users[i].password = "\0";
				users[i].district = "\0";
				users[i].location = "\0";
				users[i].phone = "\0";
				users[i].hasRide = false;
				break;
			}
		}
		users[0].users--;
		updateData();
		cout<<"\n\t\t Deleted Account!, Press any key to enter login menu...";
		getch();
		return true;
	}
	else{
		return false;
	}
}

void bookRide(){

	if(users[user].hasRide){
		cout<<"\n\t\t You already have a booked ride! Press any key...";
		getch();
		return;
	}

	bool choosing = true;
	string choice;
	int distChoice, pointChoice, points;
	
	while(choosing){
		header("BOOK A RIDE MENU");
		cout<<"\t\t Please choose the district of your location from below:";
		while(true){
			for(int i = 0; i < DISTS; i++){
				cout<<"\n\t\t "<<i+1<<". "<<locations[i].district;
			}
			cout<<"\n\t\t Choice: ";
			cin>>choice;

			if(!isdigit(choice[0])){
				cout<<"\n\t\t Invalid Input, Please enter an integer value for choice...\n";
			}
			else if(stoi(choice) < 1 || stoi(choice) > DISTS){
				cout<<"\n\t\t Invalid Input, Please enter from the given options...\n";
			}
			else{
				distChoice = stoi(choice);
				break;
			}
		}

		distChoice--;
		
		for(int i = 0; i < DISTS; i++){
			if(users[user].district == locations[distChoice].district){
				price = 60;
				isNear = true;
			}
		}
		
		if(!isNear){
			price = 200;
		}
		
		cout<<"\n\t\t Please select your pickup point from the following:\n";
		while(true){
			points = 0;
			for(int i = 0; locations[distChoice].ridePoints[i] != "\0"; i++){
				cout<<"\t\t "<<i+1<<". "<<locations[distChoice].ridePoints[i]<<endl;
				points++;
			}
			cout<<"\t\t Choice: ";
			cin>>choice;
			
			if(!isdigit(choice[0])){
				cout<<"\n\t\t Invalid Input, Please enter an integer value for choice...\n";
			}
			else if(stoi(choice) < 1 || stoi(choice) > points){
				cout<<"\n\t\t Invalid Input, Please enter from the given options...\n";
			}
			else{
				pointChoice = stoi(choice);
				break;
			}
		}
		
		pointChoice--;
		
		if(locations[distChoice].ridePoints[pointChoice] == users[user].location){
			cout<<"\n\t\t Invalid Choice!\n\t\t Please Re-Enter... (YOU CANNOT TRAVEL TO YOUR OWN LOCATION!)";
			getch();
			continue;
		}
		
		choosing = false;
	}
	
	header("Choose a Ride");
	
	int rides = getNoOfRides();
	if(rides == 0){
		cout<<"\n\t\t Sorry, no rides are leaving for your destination currently...";
		cout<<"\n\t\t Press enter to return to home menu...";
	}
	else{
		cout<<"\n\t\t Available rides that will leave for your destination at the mentioned time:\n";
		while(true){
			for(int i = 0; i < rides; i++){
				int drvr = getRandomDriver();
				genRandomTime(i);
				genRandomPrice(i);
				displayRides(i, drvr);
				users[user].drvrs[i] = drvr;
			}
			cout<<"\n\t\t Choose your ride: ";
			cin>>choice;

			if(!isdigit(choice[0])){
				cout<<"\n\t\t Invalid Input, Please enter an integer value for choice...\n";
			}
			else if(stoi(choice) < 1 || stoi(choice) > rides){
				cout<<"\n\t\t Invalid Input, Please enter from the given options...\n";
			}
			else{
				rideChoice = stoi(choice);
				break;
			}
		}

		choosing = true;
		char ch;
		while(choosing){
			if(rideChoice <= rides){
				cout<<"\n\t\t Are you sure you want to ride with "<<drivers[users[user].drvrs[rideChoice-1]].name<<"? (y/n):";
				cin>>ch;
				if(ch == 'y' || ch == 'Y')
					choosing = false;
				else{
					header("Choose a Ride");
					cout<<"\n\t\t Available rides that will leave for your destination at the mentioned time:\n";
					while(true){
						for(int i = 0; i < rides; i++){
							displayRides(i, users[user].drvrs[i]);
						}
						cout<<"\n\t\t Choose your ride: ";
						cin>>choice;
						if(!isdigit(choice[0])){
							cout<<"\n\t\t Invalid Input, Please enter an integer value for choice...\n";
						}
						else{
							rideChoice = stoi(choice);
							break;
						}
					}
				}
			}
			else{
				cout<<"\n\t\t Incorrect Choice!, please enter from the given list, press any key...";
				getch();
				header("Choose a Ride");
				cout<<"\n\t\t Available rides that will leave for your destination at the mentioned time:\n";
				while(true){
					for(int i = 0; i < rides; i++){
						displayRides(i, users[user].drvrs[i]);
					}
					cout<<"\n\t\t Choose your ride: ";
					cin>>choice;
					if(!isdigit(choice[0])){
						cout<<"\n\t\t Invalid Input, Please enter an integer value for choice...\n";
					}
					else{
						rideChoice = stoi(choice);
						break;
					}
				}
			}
		}
		
		rideChoice--;
		
		users[user].hasRide = true; //This user has a ride
		users[user].ride = rideChoice;
		
		cout<<"\n\t\t Thankyou for choosing Carify!, please reach the nearby bus stop before the given time.\n\t\t Your driver will call you on reaching your location, please guide them with your bus point.\n\t\t Happy Riding "<<heart<<endl;
		cout<<"\n\t\t Press any key to return...";
	}
	getch();
}

void displayRides(int num, int drvr){
	cout<<"\n\n\t\t ("<<num+1<<") ---------------------------------------------------";
	cout<<"\n\t\t     | Driver Name: "<<drivers[drvr].name;
	cout<<"\n\t\t     | Car: "<<drivers[drvr].carName;
	cout<<"\n\t\t     | Color: "<<drivers[drvr].color;
	cout<<"\n\t\t     | License Plate: "<<drivers[drvr].licensePlate;
	cout<<"\n\t\t     | Price (Fixed): Rs."<<users[user].prices[num];
	if(users[user].minutes[num] < 10)
		cout<<"\n\t\t     | Pickup Time: "<<users[user].hours[num]<<":0"<<users[user].minutes[num]<<" P.M";
	else
		cout<<"\n\t\t     | Pickup Time: "<<users[user].hours[num]<<":"<<users[user].minutes[num]<<" P.M";
	cout<<"\n\t\t     ---------------------------------------------------";
}

int getNoOfRides(){
	return (rand() % 4 + 0);
}

int getRandomDriver(){
	return (rand() % (noOfDrivers-2) + 1);
}

void genRandomPrice(int index){
	if(isNear){
		price += rand() % 71 + 20; //Generate a random number between 20 and 90
	}
	else{
		price += rand() % 151 + 150; //Generate a random number between 150 and 300
	}
	users[user].prices[index] = price;
}

void genRandomTime(int i){
	time_t currTime = time(0); //Getting current time
	tm lclTime = *localtime(&currTime); //Converting it to local time	
	users[user].hours[i] = lclTime.tm_hour; //Getting current hour
	if(users[user].hours[i] > 12){
		users[user].hours[i] -= 12; //Converting from 24 hour format to 12 hour format
	}
	int arrTime = rand() % 11 + 5; //Generates a random number between 5 and 15
	users[user].minutes[i] = lclTime.tm_min + arrTime; //Getting current minutes and adding driver's arrival time
}

bool readData(){
	ifstream distFile, pntFile; //Destination files
	ifstream drvrFile, licFile, carFile, colFile; //Driver Files
	ifstream nameFile, emlFile, pswdFile, phneFile, usrDistFile, usrLocFile; //User Files
	int readIndex = 0, pointIndex = 0;
	string readPoint;
	
	distFile.open("Database/Destinations/districts.txt");
	pntFile.open("Database/Destinations/ridepoints.txt");
	drvrFile.open("Database/Driver Info/drivers.txt");
	licFile.open("Database/Driver Info/licenseplates.txt");
	carFile.open("Database/Driver Info/cars.txt");
	colFile.open("Database/Driver Info/carcolors.txt");
	nameFile.open("Database/User Info/username.txt");
	emlFile.open("Database/User Info/email.txt");
	pswdFile.open("Database/User Info/password.txt");
	usrDistFile.open("Database/User Info/district.txt");
	usrLocFile.open("Database/User Info/location.txt");
	phneFile.open("Database/User Info/phone.txt");
	
	if(distFile.is_open() and pntFile.is_open() and drvrFile.is_open() and licFile.is_open() and carFile.is_open() and colFile.is_open() and nameFile.is_open() and emlFile.is_open() and pswdFile.is_open() and phneFile.is_open() and usrDistFile.is_open() and usrLocFile.is_open()){
		
		while(distFile){
			getline(distFile, locations[readIndex++].district);
		}
		distFile.close();
		
		readIndex = 0;
		while(pntFile){
			getline(pntFile, readPoint);
			if(readPoint == "*"){
				readIndex++;
				pointIndex = 0;
			}
			else{
				locations[readIndex].ridePoints[pointIndex++] = readPoint;
			}
		}
		locations[readIndex].ridePoints[--pointIndex] = "\0";
		pntFile.close();
		
		readIndex = 0;
		while(drvrFile){
			getline(drvrFile, drivers[readIndex++].name);
			noOfDrivers++;
		}
		drvrFile.close();
		
		readIndex = 0;
		while(licFile){
			getline(licFile, drivers[readIndex++].licensePlate);
		}
		licFile.close();
		
		readIndex = 0;
		while(carFile){
			getline(carFile, drivers[readIndex++].carName);
		}
		carFile.close();
		
		readIndex = 0;
		while(colFile){
			getline(colFile, drivers[readIndex++].color);
		}
		colFile.close();
		
		readIndex = 0;
		while(nameFile){
			getline(nameFile, users[readIndex++].userName);
		}
		nameFile.close();
		
		users[0].users = readIndex - 1; //Assigning number of users to the struct to know availability of data
		
		readIndex = 0;
		while(emlFile){
			getline(emlFile, users[readIndex++].email);
		}
		emlFile.close();
		
		readIndex = 0;
		while(pswdFile){
			getline(pswdFile, users[readIndex++].password);
		}
		pswdFile.close();
		
		readIndex = 0;
		while(phneFile){
			getline(phneFile, users[readIndex++].phone);
		}
		phneFile.close();
		
		readIndex = 0;
		while(usrDistFile){
			getline(usrDistFile, users[readIndex++].district);
		}
		usrDistFile.close();
		
		readIndex = 0;
		while(usrLocFile){
			getline(usrLocFile, users[readIndex++].location);
		}
		usrDistFile.close();
		
		return true;
	}
	else{
		cout<<"\n Unfortunately, the data is not able to be read\n Sorry for the inconvenience!";
		return false;
	}
}

void welcome(){
	system("cls");
	cout<<"\n\n\t\t************************************************************\n";
	cout<<"\t\t************************************************************\n";
	cout<<"\t\t************************************************************\n";
	cout<<"\t\t**                                                        **\n";
	cout<<"\t\t**   <----------------- \"C++\" PROJECT ---------------->   **\n";
	cout<<"\t\t**   <---------------- BY: SYED DAWOOD --------------->   **\n";
	cout<<"\t\t**                                                        **\n";
	cout<<"\t\t**                                                        **\n";
	cout<<"\t\t**   << PROGRAM NAME: 'Carify - A Ride Booking System'>>  **\n";
	cout<<"\t\t**                                                        **\n";
	cout<<"\t\t************************************************************\n";
	cout<<"\t\t************************************************************\n";
	cout<<"\t\t************************************************************\n\n\n\n\n\n\n";
	cout<<"\t\t Press any key to continue...";  getch();
}

void header(string title){
	system("cls");
	cout<<"\n\n\t\t";
	for(int i = 0; i < 75; i++){
		cout<<"*";
	}
	cout<<"\n\t\t***********   <----------------"<<title<<"---------------->   ***********\n\t\t";
	for(int i = 0; i < 75; i++){
		cout<<"*";
	}
	cout<<"\n\n\n";
}

void footer(){
	cout<<"\n\n\n\n\n\t\t************************************************************\n";
	cout<<"\t\t**   <---------------- END OF PROGRAM ---------------->   **\n";
	cout<<"\t\t************************************************************\n";
}

void setConsole(){
	HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r); //stores the console's current dimensions

	SetWindowPos(console, 0, 250, 100, 900, 500, SWP_SHOWWINDOW);
}

//A self convenient UDF made to clear files to start from fresh
void clearFiles(){
	ofstream nameFile, emlFile, pswdFile, distFile, locFile, phneFile;
	
	nameFile.open("Database/User Info/username.txt");
	emlFile.open("Database/User Info/email.txt");
	pswdFile.open("Database/User Info/password.txt");
	distFile.open("Database/User Info/district.txt");
	locFile.open("Database/User Info/location.txt");
	phneFile.open("Database/User Info/phone.txt");

	nameFile.clear();
	emlFile.clear();
	pswdFile.clear();
	distFile.clear();
	locFile.clear();
	phneFile.clear();

	nameFile.close();
	emlFile.close();
	pswdFile.close();
	distFile.close();
	locFile.close();
	phneFile.close();
}
