#pragma once

#include "cliSession.h"
#include "cliDisplay.h"

enum SelectionSubcommand {
    SSC_ALL,
    SSC_CLEAR,
    SSC_GROW,
    SSC_BY_LAYER,

    SSC_EMPTY,

    SSC_INVALID = -1,
};
SelectionSubcommand ResolveSelectionCommand (string argument);

// Returns true to get new command string.
// Returns false for exit.
bool    EvaluateSelectionCommandString (CliSession& session,
                                        CliDisplay& display);
void    SelectionMode                  (CliSession& session,
                                        CliDisplay& display);
