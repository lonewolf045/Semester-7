#include <stdio.h>
#include <stdlib.h>

/* header files included **/
void func() {
	printf("Hello\n");
}

int main() {
	int i;
	char k;
	for(int i = 0; i < 10; i++) {
		printf("%c\n", 'a'+i);
		func();
	} //loop ends */
	return 0;
}