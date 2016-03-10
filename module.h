#ifndef __MODULE_H
#define __MODULE_H

void
sslinfo_print_to_client(const char* const);

void
sslinfo_server_connected(SERVER_REC*);

void
sslinfo_init_signals(void);

void
sslinfo_deinit_signals(void);

void
sslinfo_init(void);

void
sslinfo_deinit(void);

#ifdef IRSSI_ABI_VERSION
void
sslinfo_abicheck(int *);
#endif

#endif
