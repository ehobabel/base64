#include <stdio.h>
#include <stdlib.h>

#define TBL_SIZE 64

static const char cnvTbl[TBL_SIZE] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int b64GetEncodedSize(int srcLen)
{
	int encLen;

	/* check param */
	if (0 >= srcLen)
	{
#ifdef __DEBUG__
		printf("[ERR] param error.\n");
#endif /* __DEBUG__ */
		return -1;	/* FAIL */
	}

	/* initialize */
	encLen = 0;

	/* calculate */
	encLen = ((srcLen * 8) + 5) / 6;
	encLen = (encLen + 3) / 4;
	encLen *= 4;

	/* SUCCESS */
	return encLen;
}

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

int b64Decode(char *srcStr, int srcLen, char *dstStr)
{
	int remain;
	int decLen;
	int i, j;
	char tmp[4];

	/* check params */
	if ((NULL == srcStr) || (0 >= srcLen) || (NULL == dstStr))
	{
#ifdef __DEBUG__
		printf("[ERR] param error.\n");
#endif /* __DEBUG__ */
		return -1;	/* FAIL */
	}
	if (0 != (srcLen%4))
	{
#ifdef __DEBUG__
		printf("[ERR] invalid length.\n");
#endif /* __DEBUG__ */
		return -2;
	}

	/* initialize */
	remain = srcLen;
	decLen = 0;

	/* decode */
	while (0 < remain)
	{
		for (i=0; i<4; i++)
		{
			tmp[i] = 0;

			if ('=' != *srcStr)
			{
				for (j=0; j<TBL_SIZE; j++)
				{
					if (cnvTbl[j] == *srcStr)
					{
						tmp[i] = j;
						break;
					}
				}
			}
			else
			{
				if (2 < remain)
				{
#ifdef __DEBUG__
					printf("[ERR] invalid encoded strings.\n");
#endif /* __DEBUG__ */
					return -3;
				}

				remain = 0;
				break;
			}

			srcStr++;
			remain--;
		}

		switch (i)
		{
		case 4:
			*dstStr = ((tmp[0] << 2) & 0xFC) | ((tmp[1] >> 4) & 0x3);
			dstStr++;
			*dstStr = ((tmp[1] << 4) & 0xF0) | ((tmp[2] >> 2) & 0xF);
			dstStr++;
			*dstStr = ((tmp[2] << 6) & 0xC0) | (tmp[3] & 0x3F);
			dstStr++;
			decLen += 3;
			break;
		case 3:
			*dstStr = ((tmp[0] << 2) & 0xFC) | ((tmp[1] >> 4) & 0x3);
			dstStr++;
			*dstStr = ((tmp[1] << 4) & 0xF0) | ((tmp[2] >> 2) & 0xF);
			dstStr++;
			decLen += 2;
			break;
		case 2:
			*dstStr = ((tmp[0] << 2) & 0xFC) | ((tmp[1] >> 4) & 0x3);
			dstStr++;
			decLen += 1;
			break;
		default:
#ifdef __DEBUG__
			printf("[ERR] unexpected... \n");
#endif /* __DEBUG__ */
			return -4;
		}
	}

	/* SUCCESS */
	return decLen;
}

#ifdef	__BASE64_TEST__
void b64Test(void)
{
	char enc[50], dec[50];
	int len;

	/* TEST-1 */
	memset(enc, 0, 50);
	memset(dec, 0, 50);
	len = b64Encode("A", 1, enc);
	printf("[01-E]%s (%d)\n", enc, len);
	len = b64Decode(enc, len, dec);
	printf("[01-D]%s (%d)\n", dec, len);

	/* TEST-2 */
	memset(enc, 0, 50);
	memset(dec, 0, 50);
	len = b64Encode("AB", 2, enc);
	printf("[02-E]%s (%d)\n", enc, len);
	len = b64Decode(enc, len, dec);
	printf("[02-D]%s (%d)\n", dec, len);

	/* TEST-3 */
	memset(enc, 0, 50);
	memset(dec, 0, 50);
	len = b64Encode("ABC", 3, enc);
	printf("[03-E]%s (%d)\n", enc, len);
	len = b64Decode(enc, len, dec);
	printf("[03-D]%s (%d)\n", dec, len);

	/* TEST-4 */
	memset(enc, 0, 50);
	memset(dec, 0, 50);
	len = b64Encode("ABCD", 4, enc);
	printf("[04-E]%s (%d)\n", enc, len);
	len = b64Decode(enc, len, dec);
	printf("[04-D]%s (%d)\n", dec, len);

	/* TEST-5 */
	memset(enc, 0, 50);
	memset(dec, 0, 50);
	len = b64Encode("ABCDE", 5, enc);
	printf("[05-E]%s (%d)\n", enc, len);
	len = b64Decode(enc, len, dec);
	printf("[05-D]%s (%d)\n", dec, len);

	/* TEST-6 */
	memset(enc, 0, 50);
	memset(dec, 0, 50);
	len = b64Encode("ABCDEF", 6, enc);
	printf("[06-E]%s (%d)\n", enc, len);
	len = b64Decode(enc, len, dec);
	printf("[06-D]%s (%d)\n", dec, len);

	/* TEST-7 */
	memset(enc, 0, 50);
	memset(dec, 0, 50);
	len = b64Encode("ABCDEFG", 7, enc);
	printf("[07-E]%s (%d)\n", enc, len);
	len = b64Decode(enc, len, dec);
	printf("[07-D]%s (%d)\n", dec, len);

	return;
}
#endif	/* __BASE64_TEST__ */
