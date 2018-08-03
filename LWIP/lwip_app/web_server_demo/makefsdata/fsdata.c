#include "fs.h"
#include "lwip/def.h"
#include "fsdata.h"


#define file_NULL (struct fsdata_file *) NULL


static const unsigned int dummy_align__index_shtml = 0;
static const unsigned char data__index_shtml[] = {
/* /index.shtml (13 chars) */
0x2f,0x69,0x6e,0x64,0x65,0x78,0x2e,0x73,0x68,0x74,0x6d,0x6c,0x00,0x00,0x00,0x00,

/* HTTP header */
/* "HTTP/1.0 200 OK
" (17 bytes) */
0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x30,0x20,0x32,0x30,0x30,0x20,0x4f,0x4b,0x0d,
0x0a,
/* "Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)
" (63 bytes) */
0x53,0x65,0x72,0x76,0x65,0x72,0x3a,0x20,0x6c,0x77,0x49,0x50,0x2f,0x31,0x2e,0x33,
0x2e,0x31,0x20,0x28,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x73,0x61,0x76,0x61,0x6e,
0x6e,0x61,0x68,0x2e,0x6e,0x6f,0x6e,0x67,0x6e,0x75,0x2e,0x6f,0x72,0x67,0x2f,0x70,
0x72,0x6f,0x6a,0x65,0x63,0x74,0x73,0x2f,0x6c,0x77,0x69,0x70,0x29,0x0d,0x0a,
/* "Content-type: text/html
Expires: Fri, 10 Apr 2008 14:00:00 GMT
Pragma: no-cache

" (85 bytes) */
0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x74,0x79,0x70,0x65,0x3a,0x20,0x74,0x65,
0x78,0x74,0x2f,0x68,0x74,0x6d,0x6c,0x0d,0x0a,0x45,0x78,0x70,0x69,0x72,0x65,0x73,
0x3a,0x20,0x46,0x72,0x69,0x2c,0x20,0x31,0x30,0x20,0x41,0x70,0x72,0x20,0x32,0x30,
0x30,0x38,0x20,0x31,0x34,0x3a,0x30,0x30,0x3a,0x30,0x30,0x20,0x47,0x4d,0x54,0x0d,
0x0a,0x50,0x72,0x61,0x67,0x6d,0x61,0x3a,0x20,0x6e,0x6f,0x2d,0x63,0x61,0x63,0x68,
0x65,0x0d,0x0a,0x0d,0x0a,
/* raw file data (884 bytes) */
0x3c,0x21,0x44,0x4f,0x43,0x54,0x59,0x50,0x45,0x20,0x68,0x74,0x6d,0x6c,0x20,0x50,
0x55,0x42,0x4c,0x49,0x43,0x20,0x22,0x2d,0x2f,0x2f,0x57,0x33,0x43,0x2f,0x2f,0x44,
0x54,0x44,0x20,0x58,0x48,0x54,0x4d,0x4c,0x20,0x31,0x2e,0x30,0x20,0x54,0x72,0x61,
0x6e,0x73,0x69,0x74,0x69,0x6f,0x6e,0x61,0x6c,0x2f,0x2f,0x45,0x4e,0x22,0x20,0x22,
0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x77,0x77,0x77,0x2e,0x77,0x33,0x2e,0x6f,0x72,
0x67,0x2f,0x54,0x52,0x2f,0x78,0x68,0x74,0x6d,0x6c,0x31,0x2f,0x44,0x54,0x44,0x2f,
0x78,0x68,0x74,0x6d,0x6c,0x31,0x2d,0x74,0x72,0x61,0x6e,0x73,0x69,0x74,0x69,0x6f,
0x6e,0x61,0x6c,0x2e,0x64,0x74,0x64,0x22,0x3e,0x0d,0x0a,0x3c,0x68,0x74,0x6d,0x6c,
0x20,0x78,0x6d,0x6c,0x6e,0x73,0x3d,0x22,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x77,
0x77,0x77,0x2e,0x77,0x33,0x2e,0x6f,0x72,0x67,0x2f,0x31,0x39,0x39,0x39,0x2f,0x78,
0x68,0x74,0x6d,0x6c,0x22,0x3e,0x0d,0x0a,0x3c,0x68,0x65,0x61,0x64,0x3e,0x0d,0x0a,
0x3c,0x6d,0x65,0x74,0x61,0x20,0x68,0x74,0x74,0x70,0x2d,0x65,0x71,0x75,0x69,0x76,
0x3d,0x22,0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x54,0x79,0x70,0x65,0x22,0x20,
0x63,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x3d,0x22,0x74,0x65,0x78,0x74,0x2f,0x68,0x74,
0x6d,0x6c,0x3b,0x20,0x63,0x68,0x61,0x72,0x73,0x65,0x74,0x3d,0x67,0x62,0x32,0x33,
0x31,0x32,0x22,0x20,0x2f,0x3e,0x0d,0x0a,0x3c,0x74,0x69,0x74,0x6c,0x65,0x3e,0x59,
0x75,0x6a,0x69,0x6e,0x63,0x68,0x65,0x6e,0x67,0x20,0x55,0x54,0x46,0x38,0x3c,0x2f,
0x74,0x69,0x74,0x6c,0x65,0x3e,0x0d,0x0a,0x3c,0x2f,0x68,0x65,0x61,0x64,0x3e,0x0d,
0x0a,0x3c,0x62,0x6f,0x64,0x79,0x3e,0x0d,0x0a,0x20,0x20,0x3c,0x70,0x3e,0x0d,0x0a,
0x20,0x20,0xb2,0xe2,0xca,0xd4,0xcf,0xd4,0xca,0xbe,0x0d,0x0a,0x20,0x20,0x3c,0x2f,
0x70,0x3e,0x0d,0x0a,0x0d,0x0a,0x20,0x20,0x3c,0x70,0x3e,0x0d,0x0a,0x20,0x20,0xb2,
0xe2,0xca,0xd4,0xb1,0xed,0xb5,0xa5,0x20,0x66,0x6f,0x72,0x6d,0x5f,0x62,0x75,0x74,
0x74,0x6f,0x6d,0x0d,0x0a,0x20,0x20,0x3c,0x66,0x6f,0x72,0x6d,0x20,0x6d,0x65,0x74,
0x68,0x6f,0x64,0x3d,0x22,0x67,0x65,0x74,0x22,0x20,0x61,0x63,0x74,0x69,0x6f,0x6e,
0x3d,0x22,0x2f,0x6c,0x65,0x64,0x73,0x2e,0x63,0x67,0x69,0x22,0x3e,0x0d,0x0a,0x20,
0x20,0x20,0x20,0x4c,0x45,0x44,0x31,0x3a,0x0d,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x3c,0x69,0x6e,0x70,0x75,0x74,0x20,0x74,0x79,0x70,0x65,0x3d,0x22,0x72,0x61,
0x64,0x69,0x6f,0x22,0x20,0x6e,0x61,0x6d,0x65,0x3d,0x22,0x4c,0x45,0x44,0x31,0x22,
0x20,0x76,0x61,0x6c,0x75,0x65,0x3d,0x22,0x4c,0x45,0x44,0x31,0x4f,0x4e,0x22,0x20,
0x69,0x64,0x3d,0x22,0x4c,0x45,0x44,0x31,0x5f,0x30,0x22,0x3e,0x4f,0x4e,0x0d,0x0a,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x69,0x6e,0x70,0x75,0x74,0x20,0x6e,0x61,
0x6d,0x65,0x3d,0x22,0x4c,0x45,0x44,0x31,0x22,0x20,0x74,0x79,0x70,0x65,0x3d,0x22,
0x72,0x61,0x64,0x69,0x6f,0x22,0x20,0x69,0x64,0x3d,0x22,0x4c,0x45,0x44,0x31,0x5f,
0x31,0x22,0x20,0x76,0x61,0x6c,0x75,0x65,0x3d,0x22,0x4c,0x45,0x44,0x31,0x4f,0x46,
0x46,0x22,0x3e,0x4f,0x46,0x46,0x3c,0x62,0x72,0x3e,0x0d,0x0a,0x20,0x20,0x20,0x3c,
0x62,0x72,0x3e,0x0d,0x0a,0x20,0x20,0x20,0x3c,0x69,0x6e,0x70,0x75,0x74,0x20,0x74,
0x79,0x70,0x65,0x3d,0x22,0x73,0x75,0x62,0x6d,0x69,0x74,0x22,0x20,0x76,0x61,0x6c,
0x75,0x65,0x3d,0x22,0x4c,0x45,0x44,0x22,0x3e,0x0d,0x0a,0x20,0x20,0x3c,0x2f,0x66,
0x6f,0x72,0x6d,0x3e,0x0d,0x0a,0x0d,0x0a,0x20,0x20,0x3c,0x66,0x6f,0x72,0x6d,0x20,
0x6d,0x65,0x74,0x68,0x6f,0x64,0x3d,0x22,0x67,0x65,0x74,0x22,0x20,0x61,0x63,0x74,
0x69,0x6f,0x6e,0x3d,0x22,0x2f,0x69,0x70,0x2e,0x63,0x67,0x69,0x22,0x3e,0x0d,0x0a,
0x20,0x20,0x20,0x20,0x3c,0x69,0x6e,0x70,0x75,0x74,0x20,0x74,0x79,0x70,0x65,0x3d,
0x22,0x74,0x65,0x78,0x74,0x22,0x20,0x6e,0x61,0x6d,0x65,0x3d,0x22,0x49,0x50,0x61,
0x64,0x64,0x72,0x22,0x3e,0x0d,0x0a,0x20,0x20,0x20,0x3c,0x62,0x72,0x3e,0x0d,0x0a,
0x20,0x20,0x20,0x3c,0x69,0x6e,0x70,0x75,0x74,0x20,0x74,0x79,0x70,0x65,0x3d,0x22,
0x73,0x75,0x62,0x6d,0x69,0x74,0x22,0x20,0x76,0x61,0x6c,0x75,0x65,0x3d,0x22,0x49,
0x50,0x22,0x3e,0x0d,0x0a,0x20,0x20,0x3c,0x2f,0x66,0x6f,0x72,0x6d,0x3e,0x0d,0x0a,
0x20,0x20,0x3c,0x2f,0x70,0x3e,0x0d,0x0a,0x0d,0x0a,0x20,0x20,0x3c,0x70,0x3e,0x0d,
0x0a,0x20,0x20,0xb2,0xe2,0xca,0xd4,0xcf,0xd4,0xca,0xbe,0xc4,0xda,0xb4,0xe6,0xd7,
0xd6,0xb7,0xfb,0x0d,0x0a,0x20,0x20,0x3c,0x21,0x2d,0x2d,0x23,0x30,0x2d,0x2d,0x3e,
0x0d,0x0a,0x20,0x20,0x3c,0x2f,0x70,0x3e,0x0d,0x0a,0x0d,0x0a,0x20,0x20,0x3c,0x70,
0x3e,0x0d,0x0a,0x20,0x20,0xb2,0xe2,0xca,0xd4,0xcf,0xd4,0xca,0xbe,0xca,0xe4,0xc8,
0xeb,0xba,0xf3,0xbb,0xd8,0xcf,0xd4,0xca,0xbe,0x0d,0x0a,0x20,0x20,0x3c,0x21,0x2d,
0x2d,0x23,0x31,0x2d,0x2d,0x3e,0x0d,0x0a,0x20,0x20,0x3c,0x2f,0x70,0x3e,0x0d,0x0a,
0x0d,0x0a,0x0d,0x0a,0x3c,0x2f,0x62,0x6f,0x64,0x79,0x3e,0x0d,0x0a,0x3c,0x2f,0x68,
0x74,0x6d,0x6c,0x3e,};



const struct fsdata_file file__index_shtml[] = { {
file_NULL,
data__index_shtml,
data__index_shtml + 16,
sizeof(data__index_shtml) - 16,
1,
}};

#define FS_ROOT file__index_shtml
#define FS_NUMFILES 1

