/*
Janani Pandurangan
jananipandurangan05@gmail.com
Lab 7 - Fill Rates 
*/

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

#define MAX 100 //the value which the arrays will be initialized with prior to reading data

//declare all the functions that you will be using here i.e. getData()
int getData(ifstream& inFile, string crnNumber[], string className[], int currEnroll[], int maxEnroll[], int numWL[]);
void output(ofstream& outFile, string crnNumber[], string className[], int currEnroll[], int maxEnroll[], int numWL[], double fillRates[], int count);
void getFillRates(int currEnroll[], int maxEnroll[], double fillRates[], int num);
void minFillRate(ofstream& outFile, string crnNumber[], string className[], int currEnroll[], int maxEnroll[], int numWL[], double fillRates[], int num);
int searchClassCRN(string crnUser, string crnNumber[], int num);
void selectSort(string crnNumber[], string className[], int currEnroll[], int maxEnroll[], int numWL[], double fillRates[], int num);


int main()
{
	ifstream inFile;
	ofstream outFile; 

	string crnNumber[MAX]; 
	string className[MAX];
	int currEnroll[MAX];
	int maxEnroll[MAX];
	int numWL[MAX];
	double fillRates[MAX];  

	double avgFillRate;
	int number;
	int classLocation;

	

	//checks whether file was found and returns an error message if it was not found
	inFile.open("Lab 7 FillRates.txt");
	if (inFile.fail())
	{
		cout << "No such file!" << endl;
		exit(100);
	}

	outFile.open("Lab7_Output.txt");

	number = getData(inFile, crnNumber, className, currEnroll, maxEnroll, numWL);
	


	//this portion focuses on getFillRates() and displays the average fill rate to the screen 
	getFillRates(currEnroll, maxEnroll, fillRates, number);
	output(outFile, crnNumber, className, currEnroll, maxEnroll, numWL, fillRates, number);
	double sum = 0.0; //needed to calculate the sum in the for loop 
	
	for (int i = 0; i < number; ++i)  
	{
		sum = sum + fillRates[i];
	}

	avgFillRate = sum / number;
	cout << fixed << setprecision(1) << "The average fill rate is: " << avgFillRate << "%" << endl;

	//gets user input about the minimum fill rate 
	minFillRate(outFile, crnNumber, className, currEnroll, maxEnroll, numWL, fillRates, number);

	//goes to searchClassCRN and finds the class according to userCRN
	string userCRN;
	cout << "Enter the CRN of the class: ";
	cin >> userCRN;

	classLocation = searchClassCRN(userCRN, crnNumber, number);

	if  (classLocation == -1)
		cout << "\nNO SUCH CLASS!" << endl;
	else
	{
		cout << "\n" << setw(10) << "Class Name" << setw(18) << "Fill Rate%" << endl;
		cout << className[classLocation] << setw(12) << fillRates[classLocation] << endl;
	}


	//goes to selectSort() and arrays get sorted according to their fill rates; later goes to output()
	selectSort(crnNumber, className, currEnroll, maxEnroll, numWL, fillRates, number);
	output(outFile, crnNumber, className, currEnroll, maxEnroll, numWL, fillRates, number);


	//Name, Email, and Title of Lab will be outputted to the screen and to the file 
	 cout << "\n\n";
	 cout << "\nProgrammed By: Janani Pandurangan" << endl;
	 cout << "Email: jananipandurangan05@gmail.com" << endl;
	 cout << "Title: Lab 7 - Fill Rates" << endl;

}

int getData(ifstream& inFile, string crnNumber[], string className[], int currEnroll[], int maxEnroll[], int numWL[])
{
	/*Pre: fileInput - a reference to the ifstream object 
	       crnNumber[] - array of the Course Registration Numbers of the classes
		   className[] - array of the name of the CIS class 
		   currEnroll[] - array of the amount of students currently enrolled 
		   maxEnroll[] - array of the maximum amount of students that can be enrolled
		   numWL[] - array of the amount of students on the waitlist
	  Post: counter - the number of classes offered in total
	  Purpose: the gather data from the file and return the amount of classes available
	*/

	int counter = 0; //counts the amount of classes and stores data in its corresponding index

	while (counter < MAX && !inFile.eof())
	{
		inFile >> crnNumber[counter];

		inFile.ignore(1); //to ignore whitespace between the CRN number and the Class Name 
		getline(inFile, className[counter]);

		inFile >> currEnroll[counter] >> maxEnroll[counter] >> numWL[counter];

		++counter;
	}

	return counter;
}

void getFillRates(int currEnroll[], int maxEnroll[], double fillRates[], int num)
{
	/* Pre: currEnroll[] - array for the number of student currently enrolled 
	        maxEnroll[] - array for the maximum number of students that can be enrolled
			fillRates[] - array for the fill rates of the classes 
			num - the number of classes offered in total
	   Post: nothing
	   Purpose: to calculate the fill rate of each class 
	*/

	for (int i = 0; i < num; ++i)
	{
		fillRates[i] = (static_cast<double>(currEnroll[i]) / maxEnroll[i]) * 100;
	}

}


void minFillRate(ofstream& outFile, string crnNumber[], string className[], int currEnroll[], int maxEnroll[], int numWL[], double fillRates[], int num)
{
	/*Pre: outFile - a reference to the ofstream object 
	       crnNumber - array for the course registration numbers of the classes
		   className - array for the names of the classes 
		   currEnroll - array for the amount of student enrolled in a class currently
		   maxEnroll - array for the maximum amount of students that can be enrolled 
		   numWL - array for number of students on waitlist 
	       fillRates[] - array for the fill rates of the classes 
		   numClasses - the number of classes in total
		   minimumFR - the minimum fill rate the user inputs 
		   noMatchCount - to count all the classes higher than the fill rate so that the error message can print
	  Post: nothing
	  Purpose: to find and sort classes below the minimum fill rate 
	*/

	double minimumFR;
	int noMatchCount; 

	cout << "Enter the minimum fill rate: ";
	cin >> minimumFR;

	minimumFR = static_cast<double>(minimumFR); //to convert to double just incase user enters an int
	
	noMatchCount = 0;

	outFile << "\n\n"; 

	for (int i = 0; i < num; ++i)
	{
		if (i==0)
		{
			outFile << "Classes with Fill Rates below " << minimumFR << "%" << endl;
			outFile << setw(2) << "CRN" << setw(18) << "Course" << setw(17) << "Current" << setw(15) << "Maximum"
				<< setw(15) << "Number on" << setw(12) << "Fill" << endl;
			outFile << setw(39) << "Enrollment" << setw(16) << "Enrollment" << setw(13) << "Wait-list" << setw(13) << "Rate %" << endl;
		}

		if (fillRates[i] < minimumFR)
		{
			
			
			outFile << fixed << setprecision(1);
			outFile << crnNumber[i] << setw(20) << className[i] << setw(10) << currEnroll[i] << setw(15)
				<< maxEnroll[i] << setw(15) << numWL[i] << setw(15) << fillRates[i] << endl;
		}
		else
		{
			++noMatchCount;
		}
	}

	if (noMatchCount == num)
		outFile << "\n\nNo classes with fill rate below " << minimumFR << "%" << endl;


}


int searchClassCRN(string crnUser, string crnNumber[], int num)
{
	/*Pre: crnUser - CRN entered by the user 
		   crnNumber[] - array of the CRN numbers of the classes 
	       num - the number of classes in total
	  Post: location - indicates the location of the CRN or -1 if the class does not exist
	  Purpose: to find the class using the CRN the user typed 
	*/

	int location = -1;

	for (int i = 0; i < num; ++i)
	{
		if (crnNumber[i] == crnUser)
		{
			location = i;
		}
	}

	return location;

}




void selectSort(string crnNumber[], string className[], int currEnroll[], int maxEnroll[], int numWL[], double fillRates[], int num)
{
	/*Pre: crnNumber[] - array of the CRN numbers of the classes
		   className[] - array of the names of the classes
		   currEnroll[] - array of the number of students currently enrolled in each class
		   maxEnroll[] - array of the maximum number of students that can be enrolled
		   numWL[] - the number of students that are on the waitlist
	       fillRates[] - array of the fill rates of all the classes 
		   num - the total number of classes offered 
		   smallestFillRate - the temporary index of the smallest fill rate 
		   tempFR - the fill rate before swap
		   tempCRN - the CRN number before swap
		   tempClass -  the class name before swap
		   tempCE - the number of students enrolled before swap
		   tempME - the number of maximum students enrolled before swap
		   tempWL - the number of students waitlisted before swap
	  Post: nothing
	  Purpose: to sort the arrays from lowest to highest fill rates 
	
	*/
	int smallestFillRate;
	double tempFR;
	string tempCRN, tempClass;
	int tempCE, tempME, tempWL;

	for (int i = 0; i < num - 1; ++i)
	{
		smallestFillRate = i;
		for (int j = i + 1; j < num; ++j)
		{
			if (fillRates[j] < fillRates[smallestFillRate])
				smallestFillRate = j; 
		}

		tempFR = fillRates[i];
		fillRates[i] = fillRates[smallestFillRate];
		fillRates[smallestFillRate] = tempFR;

		tempCRN = crnNumber[i];
		crnNumber[i] = crnNumber[smallestFillRate];
		crnNumber[smallestFillRate] = tempCRN;

		tempClass = className[i];
		className[i] = className[smallestFillRate];
		className[smallestFillRate] = tempClass;

		tempCE =  currEnroll[i];
		currEnroll[i] = currEnroll[smallestFillRate];
		currEnroll[smallestFillRate] = tempCE;

		tempME = maxEnroll[i];
		maxEnroll[i] = maxEnroll[smallestFillRate];
		maxEnroll[smallestFillRate] = tempME;

		tempWL = numWL[i];
		numWL[i] = numWL[smallestFillRate];
		numWL[smallestFillRate] = tempWL;
	}

}




void output(ofstream& outFile, string crnNumber[], string className[], int currEnroll[], int maxEnroll[], int numWL[], double fillRates[], int count)
{
	/*Pre: outFile -  a reference to the ofstream object 
		   crnNumber[] - array of the course registration number of the classes
		   className[] - array of the name of the classes 
		   currEnroll[] - array of the number of students currently enrolled in each class
		   maxEnroll[] - array of the maximum number of students that can be in each class
		   numWL[] - array of the number of students on waitlist in each class
		   fillRates[] - array of the fill rates of each class
		   count - the number of classes offered
	 Post: nothing 
	 Purpose: to print the data out to the file
	*/

		outFile << fixed << setprecision(1);
		outFile << "\n\n\n";

		outFile << setw(2) << "CRN" << setw(18) << "Course" << setw(17) << "Current" << setw(15) << "Maximum"
			<< setw(15) << "Number on" << setw(12) << "Fill" << endl;
		outFile << setw(39) << "Enrollment" << setw(16) << "Enrollment" << setw(13) << "Wait-list" << setw(13) << "Rate %" << endl;

		for (int i = 0; i < count; ++i)
		{
			outFile << crnNumber[i] << setw(20) << className[i] << setw(10) << currEnroll[i] << setw(15)
				<< maxEnroll[i] << setw(15) << numWL[i] << setw(15) << fillRates[i] << endl;
		}
	
}


