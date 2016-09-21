/*
 * irssi module.
 */

#include <openssl/ssl.h>
#include <openssl/crypto.h>

#include <string.h>

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
sslinfo_print_to_client(const char* const msg) {
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
sslinfo_server_connected(const SERVER_REC * const server) {
  // sanity check that we have the required structs.
  if (!server) {
    sslinfo_print_to_client("server not set");
    return;
  }
  if (!server->connrec) {
    sslinfo_print_to_client("connection record not set");
    return;
  }
  if (!server->handle) {
    sslinfo_print_to_client("server handle not set");
    return;
  }
  if (!server->tag) {
    sslinfo_print_to_client("server tag not set");
    return;
  }

  // If we're not connected using TLS then say that. And that's all.
  if (!server->connrec->use_ssl) {
    printtext(NULL, NULL, MSGLEVEL_CLIENTERROR, "Connected to server [%s]"
      " without SSL/TLS.", server->tag);
    return;
  }

  // connrec has a member connect_handle which is a GIOChannel,
  // but actually is a GIOSSLChannel (see network-openssl.c).
  const GIOSSLChannel * const ssl_channel =
    (GIOSSLChannel*) server->handle->handle;
  if (!ssl_channel->ssl) {
    sslinfo_print_to_client("ssl struct not found");
    return;
  }

  const SSL_CIPHER * const ssl_cipher = SSL_get_current_cipher(
      ssl_channel->ssl);
  if (!ssl_cipher) {
    sslinfo_print_to_client("failed to find ssl cipher");
    return;
  }

  const int cipher_bits = SSL_CIPHER_get_bits(ssl_cipher, NULL);

  // Get SSL/TLS version. We use this instead of SSL_CIPHER_get_version() as
  // SSL_CIPHER_* indicates the protocol version to which the cipher belongs
  // (where it was defined in the spec) rather than the current TLS version.
  const char * const protocol_version = SSL_get_version(ssl_channel->ssl);

  // Get key exchange, cipher, and MAC.
  // NOTE: There is also SSL_CIPHER_description() which gives additional
  //   details, but does not look as nice, and the details do not seem
  //   to give much more that is useful. It writes a buffer that looks like
  //   this:
  //   DHE-RSA-AES256-GCM-SHA384 TLSv1.2 Kx=DH       Au=RSA      Enc=AESGCM(256) Mac=AEAD
  //   Whereas SSL_CIPHER_get_name() returns a string that looks like this:
  //   DHE-RSA-AES256-GCM-SHA384
  // For more information, refer to http://www.openssl.org/docs/ssl/ssl.html
  const char * const cipher_name = SSL_CIPHER_get_name(ssl_cipher);

  printtext(NULL, NULL, MSGLEVEL_CLIENTERROR, "Connected to server [%s] using"
    " [%s] (%d cipher bits) (SSL/TLS protocol version %s)",
    server->tag, cipher_name, cipher_bits, protocol_version);
}

//! set up signals we listen for.
/*!
 * @return void
 */
void
sslinfo_init_signals(void) {
  signal_add("server connected", sslinfo_server_connected);
  sslinfo_print_to_client("sslinfo signals added");
}

//! clean up signals we are listening for.
/*!
 * @return void
 */
void
sslinfo_deinit_signals(void) {
  // clear signals for the whole module.
  signals_remove_module(MODULE_NAME);
  sslinfo_print_to_client("sslinfo signals cleared");
}

//! set up the module.
/*!
 * @return void
 */
void
sslinfo_init(void) {
  sslinfo_init_signals();
  sslinfo_print_to_client("sslinfo loaded");
  module_register(MODULE_NAME, "core");
}

//! clean up the module.
/*!
 * @return void
 */
void
sslinfo_deinit(void) {
  sslinfo_deinit_signals();
  sslinfo_print_to_client("sslinfo unloaded");
}

#ifdef IRSSI_ABI_VERSION
/*
 * Irssi abi check
 */
void
sslinfo_abicheck(int * version) {
  *version = IRSSI_ABI_VERSION;
}
#endif
