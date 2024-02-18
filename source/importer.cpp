#include "importer.h"
#include <filesystem>

Importer::ImportOption Importer::ParseArgument (std::string argument)
{
    if (argument == "--type")   return ImportOptionType;
    if (argument == "--layers") return ImportOptionLayers;
    if (argument == "--name")   return ImportOptionName;
    if (argument == "--from")   return ImportOptionSource;

    return ImportOptionInvalid;
}

bool Importer::hasImportType ()
{
    return type != ImportTypeInvalid;
}

bool Importer::hasSourcePath ()
{
    return fs::exists (sourcePath);
}

bool Importer::isReadyToImport ()
{
    return
        hasImportType ()
        && layersToImport.size () > 0
        && hasSourcePath ();

}

int Importer::ParseImportType (std::string argument)
{
    if (argument == "noodle")   return ImportTypeNoodle;
    if (argument == "graphml")  return ImportTypeGraphml;
    if (argument == "obsidian") return ImportTypeObsidian;

    return ImportTypeInvalid;
}

int Import (int argc, char** argv)
{
    // Get parameters from user input.
    bool gettingLayers = false;
    Importer importer;
    for (int i = 1; i < argc; i++) {
        Importer::ImportOption option = importer.ParseArgument (std::string (argv[i]));
        switch (option) {
        case Importer::ImportOptionType:
            if (importer.type == ImportTypeInvalid) {
                std::cout << "Import type already set.\n";
                return -1;
            }
            importer.ParseImportType (std::string (argv[i+1]));
            i++;
            continue;
        case Importer::ImportOptionName:
            if (importer.name.empty () == false) {
                std::cout << "Import name already set.\n";
                return -1;
            }
            importer.name = std::string (argv[i+1]);
            i++;
            break;
        case Importer::ImportOptionSource:
            if (importer.sourcePath.empty () == false) {
                std::cout << "Import source already set.\n";
                return -1;
            }
            importer.sourcePath = std::string (argv[i+1]);
            i++;
            break;
        case Importer::ImportOptionLayers:
            gettingLayers = true;
            break;
        default:
            if (gettingLayers) {
                importer.layersToImport.insert (std::string (argv[i]));
            }
            else {
                std::cout << "Invalid import arguments.\n";
                return -1;
            }
        }
    }
    if (importer.isReadyToImport () == false) {
        std::cout << "Invalid import arguments.\n";
        return -1;
    }

    // Load current noodle
    time_t timeOfLastRefresh;
    string nodeCacheVersion, workingdirectory = fs::current_path ();
    Noodle::Session session (workingdirectory);
    LoadNodeCache  (session.doodles,
                    workingdirectory,
                    timeOfLastRefresh,
                    nodeCacheVersion);
    LoadConnectome (session.connectome,
                    workingdirectory);

    // Import
    return Noodle::Import  (session.connectome,
                            session.doodles,
                            importer.type,
                            importer.sourcePath,
                            &importer.layersToImport,
                            importer.name);
}
