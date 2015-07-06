#ifndef __BASE64_H__
#define __BASE64_H__

/* Encode */
extern int b64GetEncodedSize(int srcLen);
extern int b64Encode(char *srcStr, int srcLen, char *dstStr);

/* Decode */
extern int b64Decode(char *srcStr, int srcLen, char *dstStr);

#ifdef	__BASE64_TEST__
/* Test */
extern void b64Test(void);
#endif	/* __BASE64_TEST__
*/
#endif /* __BASE64_H__ */
