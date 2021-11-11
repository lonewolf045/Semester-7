#include <stdio.h>
#include <stdlib.h>

/* header files included **/
int min(int a, int b) {
	if(a < b) {
		return(a);
	} else {
		return(b);
	}
}

int abs(int a, int b) {
	if(a < b) {
		return(b-a);
	} else {
		return(a - b);
	}
}

int main() {
	int i,j,k,l,m,n,o;
	o = (i + j)/2;
	m = 0;
	while(m < 10) {
		m = m - 1;
	}
	if(abs(o,m) == 0) {
		i = j;
	}
	n = min(o,m);
	return(0);
}