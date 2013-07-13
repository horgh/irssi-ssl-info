#ifndef __MODULE_H
#define __MODULE_H

void
print_to_client(const char* const);

void
server_connected(SERVER_REC*);

void
init_signals(void);

void
deinit_signals(void);

void
sslinfo_init(void);

void
sslinfo_deinit(void);

#endif
