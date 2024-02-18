#pragma once

#include "includes.h"
#include "noodlecore.h"
#include "returnType.h"

using namespace Noodle;

/*  CliNoodleSession
    ----------------
    Taking user input and attatching it to program functionalities.
    The session holds the data needed to interact with the noodlecore.
    Some of this is redundant in the UI in a display format.
    But this is the source of truth.
*/
struct CliSession : public Noodle::Session
{
    CliSession (std::string workingdirectory);

    Noodle::ERROR result = Noodle::ERROR::NONE;

    Name currentLayer;

    std::stringstream userInputStream;

    void CleanLayerMask ();
    Names GetDoodlesFromStream ();
    int GetDoodleFromInput (Name& doodle);

    //  Argument Parsing
    //  ----------------
    int ParseCommandDoodleNewText   (int argc, char* argv[]);
    int ParseCommandDoodleDelete    (int argc, char* argv[]);
    int ParseCommandLink            (int argc, char* argv[]);
    int ParseCommandUnlink          (int argc, char* argv[]);
    int ParseCommandLayerNew        (int argc, char* argv[]);
    int ParseCommandLayerRemove     (int argc, char* argv[]);
    int ParseCommandRefresh         (int argc, char* argv[]);
};
