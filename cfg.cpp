/*
//////////////////////////// Added 12-06-15 //////////////////////////////
Rick Elwood
CISP 440
HW #16
Fall 2015
cfg.cpp

Added two data types production and terminal and created an array of them 
to iterate through 5 of them. Four of them provided and one of my own 
creation. 

I changed l for ~ to stand for lambda. This makes it easier to check for
and allows for an extra spot. I also added 0's at the end to make it
easier to scan through like a null terminator. 

print_production(production P) now accepts a production. expand(int cfg_i)
now takes an int so it knows where it is in the array of terminals. The 
same is true for Cardinality(int row, int i), it takes an extra int to 
keep track of the array of productions.

in main() there is a loop to loop through all 5 of the productions and 
terminals.
//////////////////////////////////////////////////////////////////////////

Implements the following CFG:

Nonterminals:   N = {A, B, C}
Terminals:      T = {a, b, c}

Productions
A -> Bc
B -> Cb | l
C -> Ba | l

Where the terminals a, b, and c expand into the substrings:
a = �he said�,  b = �she said�,  c = �you lied�d

Example Series of Productions:
w = A
A->Bc
w = Bc
B->Cb
w = Cbc
C->Ba
w = Babc
B->Cb
w = Cbabc
C->l
w = lbabc
w = she said he said she said you lie.

Dan Ross
Oct 2014

*/
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#pragma warning( disable : 4996)
using namespace std;

// Strings
#define SIZE 10000
char w[SIZE] = "A";					// our initial string of N's and T's
char buf[SIZE] = "";				// a buffer for string processing
typedef char production[10][5][20]; //custom type: production
typedef char terminal[27][20];		//custom type: terminal

// Array of Productions 
production P[5] = { {
	"Bc", "", "", "", "",	// A -> Bc
	"Cb", "~", "", "", "",	// B -> Cb|l
	"Ba", "~", "",  "", "",	// C -> Ba
	0, 0, 0,  0, 0,			// null
},
{// Productions 2
	"aBCoDF", "", "", "", "",	// A -> 
	"Bb", "Bc", "Bd", "~", "",	// B -> 
	"e", "f", "g",  "", "",		// C -> 
	"E", "EE", "EEE",  "~", "",	// D -> 
	"h", "i", "j",  "", "",		// E -> 
	"k", "m", "n",  "", "",		// F -> 
	0, 0, 0,  0, 0,				// null
},
{// Productions 3
	"aBCDE", "", "", "", "",	// A -> 
	"bc", "bd", "cb", "cd", "",	// B -> 
	"e", "f", "g",  "", "",		// C -> 
	"h", "i", "j",  "", "",		// D -> 
	"k", "m", "~",  "", "",		// E -> 
	0, 0, 0,  0, 0,				// null
},
{// Productions 4
	"Bmbnaomap", "", "", "", "",// A -> 
	"Cmcnb", "~", "", "", "",	// B -> 
	"Dmdnc", "~", "",  "", "",	// C -> 
	"Emend", "~", "",  "", "",	// D -> 
	"Fmfne", "~", "",  "", "",	// E -> 
	"mgnf", "~", "",  "", "",	// F -> 
	0, 0, 0,  0, 0,				// null
}, 
{// Rick Elwoods custom Grammar (production 5)
	"aBBCDEtFG", "aBCDEtFG", "aBBBGtFCDE", "",	"", // A -> 
	"b", "c", "d", "e", "",		// B -> 
	"g", "h", "i",  "j", "",	// C -> 
	"k", "l", "m",  "n", "",	// D -> 
	"p", "q", "r",  "s", "",	// E -> 
	"u", "v", "w",  "x", "",	// F -> 
	"y", "z", "o",  "", "",		// G -> 
	0, 0, 0,  0, 0,				// null
} };

// Array of Terminal Expansions
terminal tTable[5] = { {
	"he said ",			//a
	"she said ",		//b
	"you lie.", 0},		//c
{ //Terminal Expansion 2
	"My ",				//a
	"brothers ",		//b
	"uncle's ",			//c
	"sister's ",		//d
	"boss ",			//e
	"somalier ",		//f
	"hairdresser ",		//g
	"world famous ",	//h
	"excellent ",		//i
	"hairy ",			//j
	"rockstar ",		//k
	"~",				//l
	"noodle head ",		//m
	"astronaut ",		//n
	"is a ", 0},		//o	
{ //Terminal Expansion 3
	"the ",				//a
	"brown ",			//b
	"lazy ",			//c
	"fat ",				//d
	"cow ",				//e
	"fish ",			//f
	"liger ",			//g
	"ran ",				//h
	"swam ",			//i
	"jumped ",			//j
	"high ",			//k
	"~",				//l
	"fast ", 0},		//m
{ //Terminal Expansion 4
	"fly ",					//a
	"spider ",				//b
	"mouse ",				//c
	"cat ",					//d
	"dog ",					//e
	"liger ",				//f
	"Trex ",				//g
	"1",					//h
	"1",					//i
	"1",					//j
	"1",					//k
	"~",					//l
	"she swallowed the ",	//m
	"to catch the ",		//n
	"I don't know why ",	//o
	"perhaps she'll die ",	//p		
	0, },
	{ //Terminal Expansion 5 (Rick Elwood's custom grammar)
	"My ",					//a
	"friend's ",			//b
	"brother's ",			//c
	"cousin's ",			//d
	"mother's ",			//e
	"father's ",			//f
	"cousin ",				//g
	"friend ",				//h
	"mother ",				//i
	"father ",				//j
	"Roseanne ",			//k
	"Dan ",					//l
	"Stan ",				//m
	"Dianne ",				//n
	"pecan ",				//o
	"Quillman ",			//p	
	"Moran ",				//q	
	"McMahon ",				//r	
	"Duran ",				//s	
	"had a ",				//t	
	"cyan ",				//u	
	"overran ",				//v	
	"bland ",				//w	
	"tan ",					//x	
	"catamaran ",			//y	
	"minivan ",				//z	
	0, }
};
// Gets the cardinality for a row of the Productions table
int Cardinality(int row, int i)
{
	int col = 0;

	// look for an empty string
	while (P[i][row][col][0])
		col++;

	return col;
}
// Prints the productions
int print_production(production P) {
	int i = 0, j;
	while (P[i][0][0]) {
		cout << char(i + 'A') << " -> " << P[i][0];
		j = 1;
		while (P[i][j][0]) {
			cout << " | " << P[i][j];	
			j++;
		}
		cout << endl;
		i++;
	}
	cout << endl;
	return 0;
}
// Returns true if there are Nonterminals in w
bool gotNs()
{
	for (int i = 0; w[i]; i++)
		if (w[i] >= 'A' && w[i] <= 'Z')
			return true;
	return false;
}
/*
Introduces verbosity
Replaces terminals with big strings for humans to read and ponder
*/
void expand(int cfg_i)
{
	int i = 0;  buf[0] = 0;
	while (w[i]){
		if (w[i] != '~'){
			int index = w[i] - 'a';
			strcat(buf, tTable[cfg_i][index]);
		}
		i++;
	}
	strcpy(w, buf);  // copy back to w
}
/*
Implements an iterative CFG processor driven by global tables.
Can process any CFG that can be put in these global tables.

main() has been fixed and code added to iterate through the different
grammars and to output the steps and final output neatly. 
*/
int main(void)
{
    // seed random number generator
    srand((unsigned)time(0));
    int row_num, pos_col, rand_col;

    // Main loop to loop through the 5 language generators
	for (int cfg_i = 0; cfg_i < 5; cfg_i++){

        //Print productions
            cout << "Productions:" << endl;
        print_production(P[cfg_i]);

        //Print Terminals
        cout << "Where the terminals ";
        int t = 0;
        while (tTable[cfg_i][t][0]) {
            if (tTable[cfg_i][t][0] != '~' && tTable[cfg_i][t][0] != '1')
                cout << char(t + 'a')  << ", ";
            t++;
        }
        //Print the substrings
        cout << "expand to the substrings: " << endl;
        t = 0;
        while (tTable[cfg_i][t][0]) {
            if (tTable[cfg_i][t][0] != '~' && tTable[cfg_i][t][0] != '1')
                cout << char(t + 'a') << " = \"" << tTable[cfg_i][t] << "\", ";
            t++;
        }
        //Print 3 of each random productions
        cout << endl << endl;
            for (int i = 0; i < 3; i++){
                strcpy(w,"A");
                strcpy(buf,"");
                cout << "Random Production:" << endl;
                cout << "w = " << w << endl;  // print status

                // Process all of w repeatedly until all nonterminals are gone
                while (gotNs())
                {
                    // Scan w and replace nonterminals with random productions
                    i = 0; buf[0] = 0;
                    while (w[i])  // for each w[i] in w
                    {
                        // Nonterminals
                        if (w[i] >= 'A' && w[i] <= 'Z')
                        {
                            row_num = w[i] - 'A';	// calculate row index into Productions table
                            pos_col = Cardinality(row_num, cfg_i);  // get cardinality of this row of productions
                            rand_col = rand() % pos_col;  // generate a random column index for this row of productions
                            cout << "  " << w[i] << " -> " << P[cfg_i][row_num][rand_col];
                            strcat(buf, P[cfg_i][row_num][rand_col]);  // copy production to buf
                        }
						// Terminals
                        else{                               // else w[i] is a terminal
                            char lilbuf[10] = { w[i], 0 };  // put this terminal into a lil buffer
                            strcat(buf, lilbuf);            // copy terminal to buf
                        }
                        i++;                                // next char in w
                    }
                    // copy back to w
                    strcpy(w, buf);
                    cout << endl << "w = " << w << endl;    // print status
                }

                expand(cfg_i);
                cout << "w = " << w << endl << endl;		// Print the string
            }
        cout << endl;
    }
    getchar();
    return 0;
}
