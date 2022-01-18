#include <string.h>
#include "ac_search_linear.h"

// Driver program to test above
int main()
{
	char *arr[4];
	arr[0] = "he";
	arr[1] = "she";
	arr[2] = "hers";
	arr[3] = "his";
	char *text = "ahishers";
	int k = 4;
	int *searchState;
	int state = 0;
	searchState = &state;

	struct MatchingAutomaton *ma = getMatchingAutomaton(arr, k);

	printf("%s\n", text);
	searchWords(arr, k, text, strlen(text), ma, searchState);

	return 0;
}

