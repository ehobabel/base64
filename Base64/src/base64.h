#ifndef __BASE64_H__
#define __BASE64_H__

extern int b64GetEncodedSize(int srcLen);
extern int b64Encode(char *srcStr, int srcLen, char *dstStr);

#endif /* __BASE64_H__ */
