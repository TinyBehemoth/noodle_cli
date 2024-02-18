#pragma once
#include "cliSession.h"
#include "cliDisplay.h"


int Delete (CliSession& session,
            CliDisplay& display);

int Link   (CliSession& session);

int New    (CliSession& session,
            CliDisplay& display);

int Rename (CliSession& session,
            CliDisplay& display);

int Unlink (CliSession& session);
