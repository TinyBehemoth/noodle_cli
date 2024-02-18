#include "basic.h"
#include "helpers.h"

int
Delete (CliSession& session, CliDisplay& display)
{
    int result;

    // userInputStream determines doodles to delete
    Names toDelete = session.GetDoodlesFromStream ();
    for (Name doodle : toDelete) {
        result =  Noodle::Delete (
            session.connectome,
            session.doodles,
            doodle,
            session.workingdirectory
        );
    }
    display.Hide (toDelete);

    //todo: weird return statement
    if (toDelete.size() > 0) {
        return result;
    }

    return ERROR::UNKNOWN;
}

int
Link (CliSession& session)
{
    Name first, second;
    getline (session.userInputStream, first, ' ');
    getline (session.userInputStream, second, ' ');
    if (first.empty () || second.empty ())
        return ERROR::UNKNOWN;
    if (session.doodles.count (first) == 0
        || session.doodles.count (second) == 0) {
            return ERROR::DOODLE_NOT_FOUND;
    }
    //todo: handle current layer.
    return Link (session.connectome, first, second, session.currentLayer);
}

int
New (CliSession& session, CliDisplay& display)
{
    Name newDoodle;
    if (!getline (session.userInputStream, newDoodle, ' '))
        return ERROR::UNKNOWN;
    return Noodle::New (session.connectome,
                        session.doodles,
                        newDoodle); 
}

int
Rename (CliSession& session,
        CliDisplay& display)
{
    int result;
    Name original, newName;
    if (!getline (session.userInputStream, original, ' '))
        return ERROR::UNKNOWN;
    if (!getline (session.userInputStream, newName, ' '))
        return ERROR::UNKNOWN;
    
    result = Rename (session.connectome,
                     session.doodles,
                     original,
                     newName,
                     session.workingdirectory);
    if (result != ERROR::NONE)
        return result;

    if (session.layermask.count (original)) {
        session.layermask.erase (original);
        session.layermask.insert (newName);
    }
    if (session.currentLayer == original)
        session.currentLayer = newName;

    //todo: this is disgusting, probably just overload the display methods.
    Names asList;
    asList.insert (original);
    display.Hide (asList);
    asList.clear ();
    asList.insert (newName);
    display.Display (asList);

    return result;
}

int
Unlink (CliSession& session)
{
    Name first, second;
    if (!getline (session.userInputStream, first, ' '))
        return ERROR::UNKNOWN;
    if (!getline (session.userInputStream, second, ' '))
        return ERROR::UNKNOWN;
    //todo: handle the currentLayer in a better way
    if (session.layermask.count (session.currentLayer) == 0)
        return ERROR::UNKNOWN;
    int result = ERROR::UNKNOWN;
    for (Name layer : session.layermask) {
        int localResult = Unlink (session.connectome,
                                  first,
                                  second,
                                  layer);
        if (localResult == ERROR::NONE)
            result = ERROR::NONE;
    }
    return result;
}
