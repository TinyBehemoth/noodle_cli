#include <iostream>
#include "noodlecore.h"

using namespace Noodle;

struct Importer {

    enum ImportOption {
        ImportOptionType,
        ImportOptionLayers,
        ImportOptionName,
        ImportOptionSource,
        ImportOptionInvalid = -1,
    };
    ImportOption ParseArgument (std::string argument);

    ImportType type = ImportTypeInvalid;
    Names layersToImport;
    string sourcePath;
    string name;

    bool hasImportType ();

    bool hasSourcePath ();

    bool isReadyToImport ();

    int  ParseImportType (std::string typeString);
};

int Import (int argc, char** argv);