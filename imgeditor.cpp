/*
	imgeditor.cpp

	author: Alexander Patel
	date:   1/24/2014

	Image editing program for CSCI 262, Spring 2014, Assignment 1. 
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// Global constants 
static const int EFFECT_COUNT = 10;

// Use these!  E.g., 
//	if (selections[FLATTEN_BLUE]) { // call flatten_blue() here }
static const int DO_NOTHING      = 0;
static const int GRAYSCALE       = 1;
static const int FLIP_HORIZONTAL = 2;
static const int NEGATE_RED      = 3;
static const int NEGATE_GREEN    = 4;
static const int NEGATE_BLUE     = 5;
static const int FLATTEN_RED     = 6;
static const int FLATTEN_GREEN   = 7;
static const int FLATTEN_BLUE    = 8;
static const int EXTREME_CONTRAST= 9;

static const string MENU_STRINGS[] = {
	"do nothing",
	"convert to grayscale",
	"flip horizontally",
	"negate red",
	"negate green",
	"negate blue",
	"flatten red",
	"flatten green",
	"flatten blue",
	"extreme contrast"
};

// Function Prototypes

void get_user_selections(bool* selections);

void grayScale( int red[], int green[], int blue[], int fileLen );
			
void flipHorizontal( int rEdits[], int gEdits[], int bEdits[], int fileLen, int cols, int rows );
			
void negateRed( int red[], int fileLen );
			
void negateGreen( int green[], int fileLen );
			
void negateBlue( int blue[], int fileLen);
	
void flattenRed( int red[], int fileLen );

void flattenGreen( int green[], int fileLen );

void flattenBlue( int blue[], int fileLen );

void extremeContrast( int red[], int green[], int blue[], int fileLen );


	


int main() 
{
	// user interaction to obtain input filename
	cout << "Portable Pixmap (PPM) Image Editor!" << endl << endl;
	cout << "Enter name of image file: ";
	string infile;
	getline(cin, infile);
	
	// open input filestream
	ifstream fin;
	fin.open(infile);
	if (fin.fail()) {
		cerr << "Error opening input file '"; 
		cerr << infile << "', exiting!"  << endl;
		// Uncomment if running from within Visual Studio and
		// you want to see the error message.
		 system("pause");
		return -1;
	}

	// read magic 
	string magic;
	fin >> magic;
	if (magic != "P3") {
		cerr << "Input file '" << infile;
		cerr << "' appears to not be a PPM, exiting!" << endl;
		fin.close();
		// Uncomment if running from within Visual Studio and
		// you want to see the error message.
		 system("pause");
		return -1;
	}

	// TODO: read in columns/rows/depth values; if using a statically
	// sized buffer, ensure # of columns won't exceed room in buffer;
	// declare buffer (if static) or allocate memory for buffer or 2-D
	// array if you choose to go that way
	
	int rows, cols, maxColor; // variables to store the ppm header data
	fin >> cols >> rows >> maxColor; // read the ppm values from the image file

	
	int *rVals; // red dynamic variable
	int *gVals; // green dynamic variable
	int *bVals; // blue dynamic variable
	int fileLen = rows * cols; // length of the file
	rVals = new int[fileLen]; // red dynamic memory
	gVals = new int[fileLen]; // green dynamic memory
	bVals = new int[fileLen]; // blue dynamic memory
	

	// for loop to read in the values for rgb ppm 
	for( int i = 0; i < fileLen; i++ )
	{
		fin >> rVals[i] >> gVals[i] >> bVals[i];
	}
	


	//// TODO: get output filename and open output file

	cout << "Enter output file name: ";
	string outfile; 
	getline(cin, outfile);
	ofstream out; // name of output file
	out.open(outfile);
	if (out.fail()) {
		cerr << "Error opening input file '"; 
		cerr << outfile << "', exiting!"  << endl;
		// Uncomment if running from within Visual Studio and
		// you want to see the error message.
		system("pause");
		return -1;
	}	

	// TODO: output magic, columns, rows, depth values (don't forget
	// whitespace and/or endlines!)

	out << magic << endl; // write the magic value to the output file
	out << cols << " " << rows << endl; // write the # of cols and rows to the file
	out << maxColor << endl; // write the max color to the file

		
	
	// display menu and get user selections
	bool selections[EFFECT_COUNT];
	get_user_selections(selections);

	// TODO:
	// If using a single row buffer, read in each row here, applying
	// effects and writing row to output.
	// If using a 2-D array, read in entire image here, then apply
	// effects and write entire image to output.

	// more dynamic memory to store the values for the rgb from the input file
	int *rEdits;
	int *gEdits;
	int *bEdits;
	rEdits = new int[fileLen];
	gEdits = new int[fileLen];
	bEdits = new int[fileLen];

	// set the orginal values to the new values
	for( int i = 0; i < fileLen; i++ )
	{
		rEdits[i] = rVals[i];
		gEdits[i] = gVals[i];
		bEdits[i] = bVals[i];		
	}

	// free memory of the old values for rgb
	delete[] rVals;
	delete[] gVals;
	delete[] bVals;

	




	// image effects options

	if(selections[DO_NOTHING])
	{
		return 0;
	}

	if (selections[GRAYSCALE])
	{
		grayScale( rEdits, gEdits, bEdits, fileLen );
	}

	if (selections[FLIP_HORIZONTAL])
	{
		flipHorizontal( rEdits, bEdits, bEdits, fileLen, cols, rows );
	}

	if (selections[NEGATE_RED])
	{
		negateRed( rEdits, fileLen );
	}
	if (selections[NEGATE_GREEN])
	{
		negateGreen( gEdits, fileLen );
	}

	if (selections[NEGATE_BLUE])
	{
		negateBlue( bEdits, fileLen );
	}

	if (selections[FLATTEN_RED])
	{
		flattenRed( rEdits, fileLen );
	}

	if (selections[FLATTEN_GREEN])
	{
		flattenGreen( gEdits, fileLen );
	}	

	if (selections[EXTREME_CONTRAST])
	{
		extremeContrast( rEdits, gEdits, bEdits, fileLen );
	}
	
	if (selections[FLATTEN_BLUE])
	{
		flattenBlue( bEdits, fileLen );
	}


	// for loop that writes the edited rgb values to the output file
	for( int i = 0; i < fileLen; i++ )
	{
		out << rEdits[i] << " " << gEdits[i] << " " << bEdits[i] << " ";
	}
	
	// free memory of the arrays containing the rgb values
	delete[] rEdits;
	delete[] gEdits;
	delete[] bEdits;
		

	 // TODO: close input and output files	

	fin.close();
	out.close();

	// Uncomment if desired to use under Visual Studio
	system("pause");
			
	return 0;
}

// functions
void get_user_selections(bool* selections) {
	// initialize selections
	for (int i = 0; i < EFFECT_COUNT; i++) selections[i] = false;

	// display menu
	cout << endl;
	cout << "Here are your choices:" << endl;
	int columns = 80;
	for (int i = 0; i < EFFECT_COUNT; i++) {
		// approximate length of menu item assuming 2 digit numbers
		int len = MENU_STRINGS[i].length() + 6;
		columns -= len;
		if (columns < 0) {
			cout << endl;
			columns = 80 - len;
		}
		cout << "[" << i << "] " << MENU_STRINGS[i] << " ";
	}
	cout << endl;
	cout << "Please enter your selections as a comma separated list, e.g., '5,6,1':" << endl;
	
	// get user response and extract numbers from it
	string response;
	getline(cin, response);
	
	// actually allows pretty much any characters between integers
	istringstream sin(response);
	while (sin) {
		while ((sin.peek() < '0' || sin.peek() > '9') && sin.peek() != EOF) sin.ignore();
		if (sin.peek() == EOF) break;
		int n;
		sin >> n;
		if (n < EFFECT_COUNT) {
			selections[n] = true;
		} else {
			cout << "Invalid selection " << n << " ignored!" << endl;
		}
	}
}



// Changes just the red color numbers into their "negative"
void negateRed( int rEdits[], int fileLen )
{
	for( int i = 0; i < fileLen; i++ )
	{
		rEdits[i] = 255 - rEdits[i];
	}
}

// Changes just the green color numbers into their "negative"
void negateGreen( int gEdits[], int fileLen )
{
	for( int i = 0; i < fileLen; i++ )
	{
		gEdits[i] = 255 - gEdits[i];
	}
}

// Changes just the blue color numbers into their "negative"
void negateBlue( int bEdits[], int fileLen )
{
	for( int i = 0; i < fileLen; i++ )
	{
		bEdits[i] = 255 - bEdits[i];
	}
}


// Flips the picture horizontally
void flipHorizontal( int rEdits[], int gEdits[], int bEdits[], int fileLen, int cols, int rows )
{
	int tempRed;
	int tempGreen;
	int tempBlue;
	for( int i = 0; i < rows; i++ )
	{
		for( int j = 0; j < cols; j++ )
		{
			rEdits[j + i] = rEdits[cols - j - 1 + i];
			gEdits[j + i] = gEdits[cols - j - 1 + i];
			bEdits[j + i] = bEdits[cols - j - 1 + i];
		}
	}
}

// Changes the picture into a gray scale image
void grayScale( int rEdits[], int gEdits[], int bEdits[], int fileLen )
{
	int avg = 0;
	int total = 0;
	for( int i = 0; i < fileLen; i++ )
	{
		total = ( rEdits[i] + gEdits[i] + bEdits[i] );
		avg = total / 3;
		rEdits[i] = avg;
		gEdits[i] = avg;
		bEdits[i] = avg;
	}

}

// Sets all red values to zero
void flattenRed( int rEdits[], int fileLen )
{
	for( int i = 0; i < fileLen; i++ )
	{
		rEdits[i] = 0;
	}
}

// Sets all green values to zero
void flattenGreen( int gEdits[], int fileLen )
{
	for( int i = 0; i < fileLen; i++ )
	{
		gEdits[i] = 0;
	}
}


// Sets all blue values to zero
void flatternBlue( int bEdits[], int fileLen )
{
	for( int i = 0; i < fileLen; i++ )
	{
		bEdits[i] = 0;
	}
}


/*For this effect, change each color value to either the
* highest color number possible or to 0. This change is 
* based on whether it is greater than the midpoint of the
* color range, or less. If it is greater than half of the 
* color depth, replace it with the colordepth. If it is 
* less, replace it with zero. 
*/ 
void extremeContrast( int rEdits[], int gEdits[], int bEdits[], int fileLen )
{
	// Red contrast
	for( int i = 0; i < fileLen; i++ )
	{
		if( rEdits[i] > (255 / 2) )
		{
			rEdits[i] = 255;
		}
		else
		{
			rEdits[i] = 0;
		}
	}
	// Green constrast
	for( int i = 0; i < fileLen; i++ )
	{
		if( gEdits[i] > (255 / 2) )
		{
			gEdits[i] = 255;
		}
		else
		{
			gEdits[i] = 0;
		}
	}
	// Blue contrast
	for( int i = 0; i < fileLen; i++ )
	{
		if( bEdits[i] > (255 / 2) )
		{
			bEdits[i] = 255;
		}
		else
		{
			bEdits[i] = 0;
		}
	}
}
