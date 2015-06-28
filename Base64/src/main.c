#include <stdio.h>
#include <stdlib.h>

#include "base64.h"

int main(void)
{
	char dst[15];
	char src[] = "ABCDEFG";
	int len;

	memset(dst, 0, 15);
	len = 0;

	/* [TEST] encode */
	encode_base64(src, 7, dst, &len);
	printf("RESULT:%s (SIZE:%d)\n", dst, len);

	return EXIT_SUCCESS;
}
