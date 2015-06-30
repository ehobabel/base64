#include <stdio.h>
#include <stdlib.h>

static const char cnvTbl[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int b64Encode(char *srcStr, int srcLen, char *dstStr)
{
	int remain;
	int encLen;
	int i;
	char tmp[4];

	/* check params */
	if ((NULL == srcStr) || (0 >= srcLen) || (NULL == dstStr))
	{
#ifdef __DEBUG__
		printf("[ERR] param error.\n");
#endif /* __DEBUG__ */
		return -1;	/* FAIL */
	}

	/* initialize */
	remain = srcLen;
	encLen = 0;
	i = 0;

	/* encode */
	while (0 < remain)
	{
		if (3 <= remain)
		{
			tmp[0] = ((srcStr[3*i + 0] >> 2) & 0x3F);
			tmp[1] = (((srcStr[3*i + 0] << 4) & 0x30) | ((srcStr[3*i + 1] >> 4) & 0xF));
			tmp[2] = (((srcStr[3*i + 1] << 2) & 0x3C) | ((srcStr[3*i + 2] >> 6) & 0x3));
			tmp[3] = (srcStr[3*i + 2] & 0x3F);
			dstStr[4*i + 0] = cnvTbl[tmp[0]];
			dstStr[4*i + 1] = cnvTbl[tmp[1]];
			dstStr[4*i + 2] = cnvTbl[tmp[2]];
			dstStr[4*i + 3] = cnvTbl[tmp[3]];
			remain -= 3;
		}
		else if (2 == remain)
		{
			tmp[0] = ((srcStr[3*i + 0] >> 2) & 0x3F);
			tmp[1] = (((srcStr[3*i + 0] << 4) & 0x30) | ((srcStr[3*i + 1] >> 4) & 0xF));
			tmp[2] = ((srcStr[3*i + 1] << 2) & 0x3C);
			dstStr[4*i + 0] = cnvTbl[tmp[0]];
			dstStr[4*i + 1] = cnvTbl[tmp[1]];
			dstStr[4*i + 2] = cnvTbl[tmp[2]];
			dstStr[4*i + 3] = '=';
			remain -= 2;
		}
		else	/* 1 == remain */
		{
			tmp[0] = ((srcStr[3*i + 0] >> 2) & 0x3F);
			tmp[1] = ((srcStr[3*i + 0] << 4) & 0x30);
			dstStr[4*i + 0] = cnvTbl[tmp[0]];
			dstStr[4*i + 1] = cnvTbl[tmp[1]];
			dstStr[4*i + 2] = '=';
			dstStr[4*i + 3] = '=';
			remain -= 1;
		}

		encLen += 4;
		i++;
	}

	/* SUCCESS */
	return encLen;
}
