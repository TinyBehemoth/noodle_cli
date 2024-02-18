#pragma once
#include "cliSession.h"
#include "cliDisplay.h"

namespace Utilities
{
int Clean          (CliSession& session,
                    CliDisplay& display);

int Export         (CliSession& session,
                    CliDisplay& display);

int FilesystemAuto (CliSession& session,
                    CliDisplay& display);

int Help           (CliSession& session,
                    CliDisplay& display);

int Import         (CliSession& session,
                    CliDisplay& display);

int MetaAuto       (CliSession& session,
                    CliDisplay& display);

int Orphaned       (CliSession& session,
                    CliDisplay& display);

int Quit           (CliSession& session,
                    CliDisplay& display);

int Refresh        (CliSession& session);

int Save           (CliSession& session,
                    CliDisplay& display);

int Select         (CliSession& session,
                    CliDisplay& display);

int Untracked      (CliSession& session,
                    CliDisplay& display);

}   // namespace Utilities
