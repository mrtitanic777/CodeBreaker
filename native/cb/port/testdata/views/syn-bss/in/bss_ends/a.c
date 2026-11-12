#include "port.h"
char g_1000[10];
int16_t g_100a;
int16_t f(void)
{
	char *p;
	p = (char *)&g_100a;
	return 0;
}
