/*
 * irssi module.
 */

#include <openssl/ssl.h>
#include <openssl/crypto.h>

#include <string.h>

#include "irssi_includes.h"
#include "module.h"

// XXX: this is defined in irssi's network-openssl.c ...
typedef struct
{
	GIOChannel pad;
	gint fd;
	GIOChannel *giochan;
	SSL *ssl;
	SSL_CTX *ctx;
	unsigned int verify:1;
	SERVER_REC *server;
	int port;
} GIOSSLChannel;

//! print a message to the client.
/*!
 * @param char* msg
 *
 * @return void
 */
void
print_to_client(const char* const msg) {
  if (!msg || strlen(msg) == 0) {
    return;
  }
  printtext(NULL, NULL, MSGLEVEL_CLIENTERROR, "sslinfo: %s", msg);
}

//! callback for a server connected event.
/*!
 * @param SERVER_REC* server
 *
 * @return void
 *
 * we retrieve cipher information and show it to the client.
 */
void
server_connected(SERVER_REC* server) {
  // sanity check that we have the required structs.
  if (!server) {
    print_to_client("server not set");
    return;
  }
  if (!server->connrec) {
    print_to_client("connection record not set");
    return;
  }
  if (!server->handle) {
    print_to_client("server handle not set");
    return;
  }
  if (!server->tag) {
    print_to_client("server tag not set");
    return;
  }

  // we only do anything here if we have an ssl connection.
  if (!server->connrec->use_ssl) {
    printtext(NULL, NULL, MSGLEVEL_CLIENTERROR, "Connected to server [%s]"
      " without SSL/TLS.", server->tag);
    return;
  }

  // connrec has a member connect_handle which is a GIOChannel,
  // but actually is a GIOSSLChannel (see network-openssl.c).
  GIOSSLChannel *ssl_channel = 
    (GIOSSLChannel*) server->handle->handle;
  if (!ssl_channel->ssl) {
    print_to_client("ssl struct not found");
    return;
  }

  // get the ssl cipher name.
  const SSL_CIPHER* ssl_cipher = SSL_get_current_cipher(ssl_channel->ssl);
  if (!ssl_cipher) {
    print_to_client("failed to find ssl cipher");
    return;
  }

  // NOTE: there is also SSL_CIPHER_description() which gives additional
  //   details, but does not look as nice, and the details do not seem
  //   to give much more that is useful.
  //   example:
  //   cipher name: DHE-RSA-AES256-GCM-SHA384
  //   cipher description: DHE-RSA-AES256-GCM-SHA384 TLSv1.2 Kx=DH       Au=RSA      Enc=AESGCM(256) Mac=AEAD
  // for more information, refer to
  // http://www.openssl.org/docs/ssl/ssl.html
  const char* const cipher_name = SSL_CIPHER_get_name(ssl_cipher);
  int cipher_bits = SSL_CIPHER_get_bits(ssl_cipher, NULL);
  char* protocol_version = SSL_CIPHER_get_version(ssl_cipher);
  printtext(NULL, NULL, MSGLEVEL_CLIENTERROR, "Connected to server [%s] using"
    " cipher [%s] (%d bits) (SSL/TLS protocol version %s).",
    server->tag, cipher_name, cipher_bits, protocol_version);
}

//! set up signals we listen for.
/*!
 * @return void
 */
void
init_signals(void) {
  signal_add("server connected", server_connected);
}

//! clean up signals we are listening for.
/*!
 * @return void
 */
void
deinit_signals(void) {
  // clear signals for the whole module.
  signals_remove_module(MODULE_NAME);
}

//! set up the module.
/*!
 * @return void
 */
void
sslinfo_init(void) {
  module_register(MODULE_NAME, "core");
  init_signals();
  print_to_client("sslinfo loaded");
}

//! clean up the module.
/*!
 * @return void
 */
void
sslinfo_deinit(void) {
  deinit_signals();
}
