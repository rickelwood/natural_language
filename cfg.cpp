/*

Implements the following CFG:

Nonterminals:   N = {A, B, C}
Terminals:      T = {a, b, c}

Productions
A -> Bc
B -> Cb | l
C -> Ba | l

Where the terminals a, b, and c expand into the substrings:
a = “he said”,  b = “she said”,  c = “you lied”

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
#include <time.h>
#pragma warning( disable : 4996)
using namespace std;

// Strings
#define SIZE 10000
char w[SIZE] = "A";     // our initial string of N's and T's
char buf[SIZE] = "";    // a buffer for string processing

// Productions
char P[][5][20] = {
	"Bc", "", "", "", "",	// A -> Bc
	"Cb", "l", "", "", "",	// B -> Cb|l
	"Ba", "l", "",  "", "",		// C -> Ba
};

// Terminal Expansions
char tTable[][20] = {
	"he said ",			//a
	"she said ",		//b
	"you lie.",			//c
};

// Gets the cardinality for a row of the Productions table
int Cardinality(int row)
{
	int col = 0;

	// look for an empty string
	while (P[row][col][0])
		col++;

	return col;
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
void expand()
{
	int i = 0;  buf[0] = 0;
	while (w[i])
	{
		int index = w[i] - 'a';
		strcat(buf, tTable[index]);
		i++;
	}

	// copy back to w
	strcpy(w, buf);
}

/*
Implements an iterative CFG processor driven by global tables.
Can process any CFG that can be put in these global tables.

Some lines in the inner loop in main have been hobbled,
to force you to think about what is going on - you have to fix them.
Once you get it working, you should not have to modify this processor,
you only need to modify the global tables to process other grammars.
*/
void main(void)
{
	// seed random number generator
	srand((unsigned)time(0));

	cout << "w = " << w << endl;  // print status

	// process all of w repeatedly until all nonterminals are gone
	while (gotNs())
	{
		// scan w and replace nonterminals with random productions
		int i = 0; int j = 0; buf[0] = 0;
		while (w[i])                                  // for each w[i] in w
		{
			// Nonterminals
			if (w[i] >= 'A' && w[i] <= 'Z')
			{
				// calculate row index into Productions table
				// get cardinality of this row of productions
				rand % C// generate a random column index for this row of productions: rand() % C
				// print status
				strcat(buf, P[row][col]);           // copy production to buf
			}
			// Terminals
			else                                    // else w[i] is a terminal
			{
				char lilbuf[10] = { w[i], 0 };      // put this terminal into a lil buffer
				strcat(buf, lilbuf);                // copy terminal to buf
			}
			i++;                                    // next char in w
		}

		// copy back to w
		strcpy(w, buf);
		cout << endl << "w = " << w << endl;    // print status
	}

	expand();
	cout << "w = " << w;
}
