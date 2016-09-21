# Summary
This is an Irssi module that gives additional information about the SSL/TLS
connection to a server upon connection. Primarily I am interested in showing
information about the cipher, hash functions, and TLS version in use.


# What does this module show?
  * Key exchange, cipher, and MAC.
    * For example, e.g. ECDHE-RSA-AES256-GCM-SHA384
      tells us key exchange ECDHE-RSA, cipher AES256 GCM, and MAC SHA384.
  * Cipher bit count
  * SSL/TLS version


# Building requirements
  * You need glib2.0 development files. In Debian/Ubuntu these are in the
    `libglib2.0-dev` package.
  * You need Irssi development files. In Debian/Ubuntu these are in the
    `irssi-dev` package.


# TODO
  * Can we say explicitly if the algorithm provides forward secrecy?
  * Can we say if the connection is secure or not? Some ciphers are judged to
    not be.
  * Can we say whether compression is in use? It's now recommended to not use as
    insecure.
  * What else is useful to show?
