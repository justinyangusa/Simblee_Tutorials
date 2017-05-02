#include <stdio.h>

int main() {
	char buf[100];
	printf("%d\n", sprintf(buf, "%.2f", 50.05));
	print("buf: ");
	//buf[0] = 'a';
	Serial.println(buf);
	return 0;
}
