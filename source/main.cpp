#include <iostream>
#include <filesystem>

#include "noodlecore.h"
#include "returnType.h"

#include "interactive.h"
#include "utilities.h"
#include "importer.h"
#include "exporter.h"

#include "definitions.h"

enum ShellCommand {
    ShellCommandInvalid = -1,
    ShellCommandHelp,
    ShellCommandVersion,
    ShellCommandInitialize,
    ShellCommandInteractive,
    ShellCommandImport,
    ShellCommandExport,
};

ShellCommand ResolveShellCommand (string argument)
{
    if (argument == "--help")       return ShellCommandHelp;
    if (argument == "--version")    return ShellCommandVersion;
    if (argument == "--intarctive") return ShellCommandInteractive;
    if (argument == "-i")           return ShellCommandInteractive;

    if (argument == "init")         return ShellCommandInitialize;
    if (argument == "initialize")   return ShellCommandInitialize;
    if (argument == "import")       return ShellCommandImport;
    if (argument == "export")       return ShellCommandExport;

    return ShellCommandInvalid;
}

int main (int argc, char* argv[])
{
    CliSession session (fs::current_path ());

    if (argc == 1) {
        return Interactive (session);
    }

    ShellCommand command = ResolveShellCommand (std::string(argv[1]));
    switch (command) {
    case ShellCommandVersion:
        std::cout
            << "noodle cli version " << CLI_VERSION << '\n'
            << "noodlecore         " << "---" << '\n'
            << "node cache         " << NODE_CACHE_VERSION << '\n'
            << PROJECT_SITE_URL << '\n';
        break;
    case ShellCommandInitialize:
        return InitializeDirectory (session.workingdirectory);
    case ShellCommandInteractive:
        return Interactive (session);
    case ShellCommandImport:
        return Import (argc, argv);
    case ShellCommandExport:
        return Export (argc, argv);
    default:
        std::cout << "Invalid noodle arguments.\n";
        return -1;
    }
}