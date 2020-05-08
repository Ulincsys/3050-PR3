#include "functions.h"

int main() {
	int f,s,g,u,d;
	cmdIn = fopen(CMD_FILE, "r");

	puts("Enter the number of floors, the starting floor, the intended floor, "
			"move size up, move size down:");
	fscanf(((cmdIn) ? cmdIn : stdin), "%d %d %d %d %d",&f,&s,&g,&u,&d);
	printf("%d floors, starting at %d, trying to reach %d, going up %d and down %d\n", f, s, g, u, d);


	// Just for fun I decided to implement both a Tree based approach, and an iterative approach.
	printf("Result with search using tree: ");
	if(!searchWithTree(f, s, g, u, d)) {
		printf("Use the stairs.\n");
	}
	printf("Result of search without tree: ");
	if(!searchDirect(f, s, g, u, d)) {
		printf("Use the stairs.\n");
	}

	if(cmdIn) {
		fclose(cmdIn);
	}
	return 0;
}
