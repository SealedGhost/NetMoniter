#ifndef _CONFIG_H
#define _CONFIG_H

#define BOAT_LIST_SIZE_MAX 400
#define TIMESTAMP          100

#define MSG_QUEUE_TABNUM   20



#define MYDEBUG(format,...) printf("FILE: "__FILE__", LINE: %d: "format"\r\n", __LINE__, ##__VA_ARGS__)

#endif
