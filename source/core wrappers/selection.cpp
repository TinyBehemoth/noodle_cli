#include "selection.h"
#include <readline/readline.h>
#include <readline/history.h>

SelectionSubcommand
ResolveSelectionCommand (string argument)
{
    if (argument == "all")      return SelectionSubcommand::SSC_ALL;
    if (argument == "a")        return SelectionSubcommand::SSC_ALL;
    if (argument == "layer")    return SelectionSubcommand::SSC_BY_LAYER;
    if (argument == "clear")    return SelectionSubcommand::SSC_CLEAR;
    if (argument == "c")        return SelectionSubcommand::SSC_CLEAR;
    if (argument == "grow")     return SelectionSubcommand::SSC_GROW;
    if (argument == "g")        return SelectionSubcommand::SSC_GROW;
    if (argument == "")         return SelectionSubcommand::SSC_EMPTY;

    return SelectionSubcommand::SSC_INVALID;
}

void
SelectionGrow  (CliSession& session,
                CliDisplay& display)
{
    Names displayed = display.Displayed ();

    Noodle::Grow   (&session.connectome,
                    session.selection,
                    &session.layermask);

    for (Name doodle : session.selection) {
        if (displayed.count (doodle) == 0)
            session.selection.erase (doodle);
    }
}

// Returns true to get new command string.
// Returns false for exit.
bool
EvaluateSelectionCommandString (CliSession& session,
                                CliDisplay& display)
{
    string argument;
    bool isFirstArg = true;
    while (getline (session.userInputStream, argument, ' ')) {
        isFirstArg = false;
        switch (ResolveSelectionCommand (argument)) {
        case SelectionSubcommand::SSC_ALL:
            session.selection = display.Displayed ();
            break;
        case SelectionSubcommand::SSC_BY_LAYER:
            break;
        case SelectionSubcommand::SSC_CLEAR:
            session.selection.clear ();
            break;
        case SelectionSubcommand::SSC_GROW:
            SelectionGrow (session, display);
            break;
        default:
            // toggle selection status
            if (session.selection.count (argument)) {
                session.selection.erase (argument);
            } else if (session.doodles.count (argument)) {
                session.selection.insert (argument);
            }
            break;
        }
    }
    display.displayOnNextInteractiveCommand = true;
    return !isFirstArg;
}

void
SelectionMode  (CliSession& session,
                CliDisplay& display)
{
    //display.prompt = "\033[36m<noodle:select>\033[39;49m ";
    display.prompt = "\033[38;5;222m<noodle:select>\033[39;49m ";
    char* userInput;
    // Command Loop
    do {
        display.GeometerDisplay (session);
        userInput = readline (display.prompt.c_str ());
        session.userInputStream = std::stringstream (userInput);
    } while (EvaluateSelectionCommandString (session, display));
    display.prompt = "\033[38;5;251m<noodle>\033[39;49m ";
}
