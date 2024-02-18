#pragma once

#include "cliSession.h"
#include "cliDisplay.h"

int Chain      (CliSession& session,
                CliDisplay& display);

int Clone      (CliSession& session,
                CliDisplay& display);

int Sprout     (CliSession& session,
                CliDisplay& display);

int Subchain   (CliSession& session,
                CliDisplay& display);

int Subsprout  (CliSession& session,
                CliDisplay& display);

int Superchain (CliSession& session,
                CliDisplay& display);

int Phase      (CliSession& session,
                CliDisplay& display);

int Project    (CliSession& session,
                CliDisplay& display);

int Flatten    (CliSession& session,
                CliDisplay& display);

int Merge      (CliSession& session,
                CliDisplay& display);

int Pluck      (CliSession& session,
                CliDisplay& display);

int SwapFile   (CliSession& session,
                CliDisplay& display);

int SwapLinks  (CliSession& session,
                CliDisplay& display);

int SwapName   (CliSession& session,
                CliDisplay& display);

int Transfer   (CliSession& session,
                CliDisplay& display);

int Dissolve   (CliSession& session,
                CliDisplay& display);

int Isolate    (CliSession& session,
                CliDisplay& display);

int Purge      (CliSession& session,
                CliDisplay& display);

int Remove     (CliSession& session,
                CliDisplay& display);

int Clean      (CliSession& session,
                CliDisplay& display);
