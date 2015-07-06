#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base64.h"

int main(void)
{
	char *dst;
	char src[] = "ABCDEFG";
	int len;

	len = b64GetEncodedSize(strlen(src));
	printf("ENCODED SIZE=%d (byte)\n", len);

	if (0 >= len)
	{
		printf("[ERR] invalid size\n");
		return -1;
	}

	dst = malloc(len+1);
	if (NULL == dst)
	{
		printf("[ERR] cannot allocate memory\n");
		return -1;
	}

	memset(dst, 0, len+1);

	/* encode */
	len = b64Encode(src, strlen(src), dst);
	if (0 < len)
	{
		printf("RESULT:%s (SIZE:%d)\n", dst, len);
	}
	else
	{
		printf("[ERR] failed: (err code=%d)\n", len);
	}

	memset(src, 0, strlen(src));
	printf("check: %s\n", src);

	/* decode */
	len = b64Decode(dst, len, src);
	if (0 < len)
	{
		printf("RESULT:%s (SIZE:%d)\n", src, len);
	}
	else
	{
		printf("[ERR] failed: (err code=%d)\n", len);
	}

	free(dst);

	return EXIT_SUCCESS;
}
