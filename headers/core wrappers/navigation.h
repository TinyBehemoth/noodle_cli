#pragma once

#include "cliSession.h"
#include "cliDisplay.h"

int Dive                       (CliSession& session,
                                CliDisplay& display);

int Collapse                    (CliSession& session,
                                 CliDisplay& display);

int Expand                      (CliSession& session,
                                 CliDisplay& display);

int Explore                    (CliSession& session,
                                CliDisplay& display);

int Jump                       (CliSession& session,
                                CliDisplay& display);

int Focus                      (CliSession& session,
                                CliDisplay& display);

int Grow                       (CliSession& session,
                                CliDisplay& display);

int Hide                       (CliSession& session,
                                CliDisplay& display);

int Shrink                     (CliSession& session,
                                CliDisplay& display);

int Switch                     (CliSession& session,
                                CliDisplay& display);

int Subsplore                  (CliSession& session,
                                CliDisplay& display);
