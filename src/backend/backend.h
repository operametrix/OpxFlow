#ifndef __BACKEND_H__
#define __BACKEND_H__

void backend_add_subscriber(int fd);
int backend_get_subscriber(int index);
int backend_get_subscribers_size(void);

#endif