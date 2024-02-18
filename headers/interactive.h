/*  Noodle CLI Interactive
    ----------------------
    This file contains definitions for the command line interface app.
    The functionality here creates an interactive UX much like GDB.
    Because the CLI is meant to closely follow the core,
    most of the CLI is a thin wrapper.
    The rest is some plumbing and boilerplate CLI stuff
    i.e. command parsing autocomplete etc.
*/

#pragma once
#include <iostream>
#include <sstream>
#include "noodlecore.h"
#include "returnType.h"
#include <vector>
#include <readline/readline.h>
#include <readline/history.h>

#include "cliSession.h"

using namespace Noodle;
using std::cout, std::cin, std::string, std::vector;

int  ReadlineInit ();

int Interactive (CliSession& session);

enum InteractiveCommand {

    // navigation
    DIVE,           COLLAPSE,       EXPAND,
    EXPLORE,        FOCUS,
    GROW,           HIDE,           JUMP,
    SHRINK,         SWITCH,         SUBSPLORE,
    SUPERSPLORE,    TOGGLE_VIEW,    TOGGLE_ROTATION,

    // basic

    DELETE,         LINK,           NEW,
    RENAME,         UNLINK,

    // manipulations

    CHAIN,          CLONE,          SPROUT,
    SUBCHAIN,       SUBSPROUT,      SUPERCHAIN,
    PHASE,          PROJECT,
    
    FLATTEN,        MERGE,          PLUCK,
    SWAPFILE,       SWAPLINKS,      SWAPNAME,
    TRANSFER,

    DISSOLVE,       ISOLATE,        PURGE,
    REMOVE,

    // utilities

    BASEAUTO,       CLEAN,          EXPORT,
    FILESYSTEMAUTO, IMPORT,         INIT,
    LAYER,          QUIT,           REFRESH,
    SAVE,           SELECT,         HELP,

    // bonus (not in the specification, but useful for development)

    TEXT,           FOLDER,         DISPLAY,
    ROTATE,

    // invalid

    INVALID,
};
InteractiveCommand ResolveInteractiveCommand (string userInput);
