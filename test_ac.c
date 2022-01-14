#include "ac_search.h"

// Driver program to test above
int main()
{
	char *arr[4];
	arr[0] = "he";
	arr[1] = "she";
	arr[2] = "hers";
	arr[3] = "his";
	char *text = "ahishers";
	int k = sizeof(arr)/sizeof(arr[0]);

	printf("%s\n", text);
	searchWords(arr, k, text);

	return 0;
}

