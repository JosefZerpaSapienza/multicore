// Adapted from: 
// https://www.geeksforgeeks.org/aho-corasick-algorithm-pattern-searching/
//
// C program for string matching with Aho Corasick algorithm
#include <string.h>
#include "queue.h"

// Max number of states in the matching machine.
// Should be equal to the sum of the length of all keywords. TODO: Check (efficiency improvement?)
#define MAXS 500 

// Maximum number of characters in input alphabet
#define MAXC 26

// Matching Automaton
struct MatchingAutomaton {
  int out[MAXS];
  int failure[MAXS];
  int gotos[MAXS][MAXC];
};

// Get matching automaton.
// Allocates and builds a matching automaton for the
// given array of patterns.
// arr - array of words. The index of each keyword is important:
//		 "out[state] & (1 << i)" is > 0 if we just found word[i]
//		 in the text.
// k - the number of words in the array.
struct MatchingAutomaton *getMatchingAutomaton(char **arr, int k)
{
	// Allocate matching automaton
	struct MatchingAutomaton *ma = malloc(sizeof(struct MatchingAutomaton));
	// Fill out function with 0
	memset(ma->out, 0, MAXS); 
	// Fill failure functions with -1
	memset(ma->failure, -1, MAXS);
	// Fill goto functions with -1
	memset(ma->gotos, -1, MAXS * MAXC);

	// Initially, we just have the 0 state
	int states = 1;

	// Construct values for goto function, i.e., fill g[][]
	// This is same as building a Trie for arr[]
	for (int i = 0; i < k; ++i)
	{
		// const
		char *word = arr[i];
		int currentState = 0;

		// Insert all characters of current word in arr[]
		for (int j = 0; j < strlen(word); ++j)
		{
			int ch = word[j] - 'a';

			// Allocate a new node (create a new state) if a
			// node for ch doesn't exist.
			if (ma->gotos[currentState][ch] == -1)
				ma->gotos[currentState][ch] = states++;

			currentState = ma->gotos[currentState][ch];
		}

		// Add current word in output function
		ma->out[currentState] |= (1 << i);
	}

	// For all characters which don't have an edge from
	// root (or state 0) in Trie, add a goto edge to state
	// 0 itself
	for (int ch = 0; ch < MAXC; ++ch)
		if (ma->gotos[0][ch] == -1)
			ma->gotos[0][ch] = 0;

	// Failure function is computed in breadth first order
	// using a queue
	struct Queue *q = createQueue(MAXC);

	// Iterate over every possible input
	for (int ch = 0; ch < MAXC; ++ch)
	{
		// All nodes of depth 1 have failure function value
		// as 0. For example, in above diagram we move to 0
		// from states 1 and 3.
		if (ma->gotos[0][ch] != 0)
		{
			ma->failure[ma->gotos[0][ch]] = 0;
			enqueue(q, ma->gotos[0][ch]);
		}
	}

	// Now queue has states 1 and 3
	while (size(q))
	{
		// Remove the front state from queue
		int state = dequeue(q);

		// For the removed state, find failure function for
		// all those characters for which goto function is
		// not defined.
		for (int ch = 0; ch <= MAXC; ++ch)
		{
			// If goto function is defined for character 'ch'
			// and 'state'
			if (ma->gotos[state][ch] != -1)
			{
				// Find failure state of removed state
				int failure = ma->failure[state];

				// Find the deepest node labeled by proper
				// suffix of string from root to current
				// state.
				while (ma->gotos[failure][ch] == -1)
					failure = ma->failure[failure];

				failure = ma->gotos[failure][ch];
				ma->failure[ma->gotos[state][ch]] = failure;

				// Merge output values
				ma->out[ma->gotos[state][ch]] |= ma->out[failure];

				// Insert the next level node (of Trie) in Queue
				enqueue(q, ma->gotos[state][ch]);
			}
		}
	}

	return ma;
}

// Returns the next state the machine will transition to using goto
// and failure functions.
// currentState - The current state of the machine. Must be between
//			 0 and the number of states - 1, inclusive.
// nextInput - The next character that enters into the machine.
int findNextState(struct MatchingAutomaton *ma, int currentState, char nextInput)
{
	int answer = currentState;
	int ch = nextInput - 'a';

	// If goto is not defined, use failure function
	while (ma->gotos[answer][ch] == -1)
		answer = ma->failure[answer];

	return ma->gotos[answer][ch];
}

// This function finds all occurrences of all array words
// in text.
// As first search set currentState to 0
// Return -1 when no word is found, the index otherwise.
int ACsearch(char **arr, int k, char *text, int len,
		struct MatchingAutomaton *ma, int *currentState, char **word)
{
	// Traverse the text through the built machine to find
	// all occurrences of words in arr[]
	for (int i = 0; i < len; ++i)
	{
		*currentState = findNextState(ma, *currentState, text[i]);

		// If match not found, move to next state
		if (ma->out[*currentState] == 0)
			continue;

		// Match found, print all matching words of arr[]
		// using output function.
		for (int j = 0; j < k; ++j)
		{
			if (ma->out[*currentState] & (1 << j))
			{
				// printf("Found word %s at %ld.\n", arr[j], i - strlen(arr[j]) + 1);
				*word = arr[j];
				return i - strlen(arr[j]) + 1;
			}
		}
	}

	return -1;
}
