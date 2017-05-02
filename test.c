#include <stdio.h>

int main() {
	char buf[100];
	for (int i = 0; i < 10; i++){
	printf("%d\n", sprintf(buf, "%.2f", 1000.));
	printf("buf: ");
	//buf[0] = 'a';
	printf("%s\n", buf);}
	return 0;
}
