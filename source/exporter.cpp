#include "exporter.h"
#include "interactive.h"


Exporter::ExportOption
Exporter::ParseArgument (std::string argument)
{

    if (argument == "--type")           return ExportOptionType;
    if (argument == "--layers")         return ExportOptionLayers;
    if (argument == "--destination")    return ExportOptionDestination;

    return ExportOptionInvalid;
}


int
Export (int argc,
        char** argv)
{
    Exporter exporter;

    //todo: parse commandline arguments

    CliSession session (fs::current_path ());

    return Noodle::Export (session.connectome,
                           session.doodles,
                           exporter.type,
                           session.workingdirectory,
                           exporter.destinationPath,
                           session.selection,
                           session.layermask);
}
