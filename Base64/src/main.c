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
	len = b64Encode(src, 7, dst);
	if (0 < len)
	{
		printf("RESULT:%s (SIZE:%d)\n", dst, len);
	}
	else
	{
		printf("FAILED: err code=%d\n", len);
	}

	return EXIT_SUCCESS;
}
