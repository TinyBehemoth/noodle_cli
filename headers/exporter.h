#include <iostream>
#include "noodlecore.h"

struct Exporter {

    enum ExportOption {
        ExportOptionType,
        ExportOptionLayers,
        ExportOptionDestination,
        ExportOptionInvalid = -1
    };
    ExportOption ParseArgument (std::string argument);

    Noodle::ExportType type = Noodle::ExportTypeInvalid;
    Noodle::Names layersToExport;
    std::string destinationPath;

    bool hasExportType ();

    bool hasDestionationType ();

    bool isReadyToExport ();
};

int Export (int argc, char** argv);