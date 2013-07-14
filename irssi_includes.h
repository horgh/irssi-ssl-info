#ifndef __IRSSI_INCLUDES_H
#define __IRSSI_INCLUDES_H

// this is to fix a bug apparently.
// see github.com/shabble/irssi-scripts test module.
// another way to silence this seems to be to pass in
// define HAVE_CONFIG_H..
#define UOFF_T_LONG_LONG 1

#include <common.h>

// MODULE_NAME is used in Irssi. some of the below includes need it.
#define MODULE_NAME "sslinfo"

#include <core/levels.h>
#include <core/net-sendbuffer.h>
#include <fe-common/core/printtext.h>
#include <irc/core/irc.h>
#include <irc/core/irc-servers.h>

#endif
