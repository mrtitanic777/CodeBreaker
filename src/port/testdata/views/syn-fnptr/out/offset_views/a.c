#include "port.h"
/* a */
char *mk(int16_t n)
{
	return 0;
}
int16_t run(char *(*fn)(), int16_t k)
{
	return (*fn)(k) != 0;
}
int main(void)
{
	run(mk, 3);
	return 0;
}
