#include "utilities.h"
#include "helpers.h"
#include "selection.h"

#include <iostream>

namespace Utilities
{

int
Clean  (CliSession& session,
        CliDisplay& display)
{
    return Noodle::ERROR::NOT_IMPLEMENTED;
}

int
FilesystemAuto (CliSession& session,
                CliDisplay& display)
{
    return Noodle::ERROR::NOT_IMPLEMENTED;
}

int
Help   (CliSession& session,
        CliDisplay& display)
{
    return Noodle::ERROR::NOT_IMPLEMENTED;
}

int
MetaAuto   (CliSession& session,
            CliDisplay& display)
{
    return Noodle::ERROR::NOT_IMPLEMENTED;
}

int
Orphaned   (CliSession& session,
            CliDisplay& display)
{
    return Noodle::ERROR::NOT_IMPLEMENTED;
}

int
Quit (CliSession& session,
      CliDisplay& display)
{
    //todo: check for unsaved changes before prompt
    //todo: also reconsider this behaviour
    std::cout << "Save changes before quitting? [y]/n: ";
    string userInput;
    std::cin >> userInput;
    if (userInput == "" || userInput == "Y" || userInput == "y")
        Save (session, display);
    return ERROR::NONE;
}

struct CliRefresher : public Refresher
{
private:

    CliSession* session;

public:

    CliRefresher (CliSession* session) : session (session), Refresher ()
    {
        return;
    }

    void HandleMissing      ()
    {
        if (missing.size () == 0)
            return;
        std::cout << "\rThe following doodles have missing files:\n";
        for (string m : missing)
            std::cout << m << " ";
        std::cout << "\n"
            "1 Do nothing.\n"
            "2 Remove file references but keep the doodles.\n"
            "3 Delete doodles with missing files.\n"
            "How do you want to handle missing files? Does nothing by default. [1]/2/3 ";
        string uin;
        std::getline(std::cin, uin);
        if (uin == "2") {
            for (auto m : missing) {
                session->doodles.at (m).abstract = true;
                session->doodles.at (m).path = ".noodle/"+m;
            }
        } else if (uin == "3") {
            Names allLayers = session->connectome.LayerList ();
            Noodle::Remove (session->connectome,
                            session->doodles,
                            &missing,
                            &allLayers);
        }
    }
    void HandleModified     ()
    {
        if (modified.size () == 0)
            return;
        std::cout << "\nThe following doodles have been modified:\n";
        for (string m : modified)
            std::cout << m << " ";
        std::cout << "\n";
    }
    void HandleMoved        ()
    {
        if (moved.size () == 0)
            return;
        std::cout << "\nThe following doodles have updated paths:\n";
        for (auto m : moved)
            std::cout << m.first << " -> " << m.second << "\n";
    }
    void HandleUntracked    ()
    {
        if (untracked.size () == 0)
            return;
        std::cout << "\nThe following files were added as untracked doodles:\n";
        for (auto u : untracked)
            std::cout << u.first << " @ " << u.second << "\n";
    }
};

int
Refresh (CliSession& session)
{
    CliRefresher refresher (&session);
    return Noodle::Refresh (session.connectome,
                            session.doodles,
                            session.workingdirectory,
                            session.timeOfLastRefresh,
                            session.timeOfLastRefresh,
                            refresher);
}

int
Save (CliSession& session,
      CliDisplay& display)
{
    std::cout << "\rSaving Noodle...  ";

    Noodle::WriteNodeCache (&session.doodles,
                            session.workingdirectory,
                            session.timeOfLastRefresh);
    Noodle::WriteConnectome (&session.connectome,
                            session.workingdirectory);

    std::cout << "\r                \rNoodle Saved\n";
    display.displayOnNextInteractiveCommand = false;
    return ERROR::NONE;
}

int
Select (CliSession& session,
        CliDisplay& display)
{
    string argument;
    getline (session.userInputStream, argument, ' ');

    if (argument.empty ()) {
        SelectionMode (session, display);
        return ERROR::NONE;
    }

    // reset input
    string restOfLine;
    getline (session.userInputStream, restOfLine);
    session.userInputStream = std::stringstream (argument + " " + restOfLine);
    EvaluateSelectionCommandString (session, display);

    return Noodle::ERROR::NONE;
}

int
Untracked (CliSession& session,
            CliDisplay& display)
{
    return Noodle::ERROR::NOT_IMPLEMENTED;
}

}   // namespace Utilities
