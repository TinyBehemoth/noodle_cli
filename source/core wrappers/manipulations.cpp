#include "manipulations.h"
#include <iostream>

int
Chain (CliSession& session,
        CliDisplay& display)
{
    Name nextArgument;
    vector<Name> toChain;
    //todo: use iterable base class instead
    Names toChainForDisplay;
    while (getline (session.userInputStream, nextArgument, ' ')) {
        toChain.push_back (nextArgument);
        toChainForDisplay.insert (nextArgument);
    }
    display.Display (toChainForDisplay);
    return Noodle::Chain (session.connectome, session.doodles, toChain, session.currentLayer);
}

int
Clone (CliSession& session,
        CliDisplay& display)
{
    return Noodle::ERROR::NOT_IMPLEMENTED;
}

int
Sprout (CliSession& session,
        CliDisplay& display)
{
    Name root;
    Name nextArgument;
    Names toSprout;
    if (!getline (session.userInputStream, root, ' '))
        return ERROR::UNKNOWN;
    //todo: error check on root. might already happen in the corelib. just make sure
    while (getline (session.userInputStream, nextArgument, ' ')) {
        toSprout.insert (nextArgument);
    }
    display.Display (toSprout);
    return Noodle::Sprout (session.connectome, session.doodles, root, toSprout, session.currentLayer);
}

int
Subchain (CliSession& session,
            CliDisplay& display)
{
    return Noodle::ERROR::NOT_IMPLEMENTED;
}

int
Subsprout (CliSession& session,
            CliDisplay& display)
{
    return Noodle::ERROR::NOT_IMPLEMENTED;
}

int
Superchain (CliSession& session,
            CliDisplay& display)
{
    return Noodle::ERROR::NOT_IMPLEMENTED;
}

int
Phase (CliSession& session,
            CliDisplay& display)
{
    return Noodle::ERROR::NOT_IMPLEMENTED;
}

int
Project (CliSession& session,
         CliDisplay& display)
{
    if (session.selection.size () == 0)
        return ERROR::UNKNOWN;
    // select target layer (can be included in layerMask)
    Name targetLayer;
    if (!getline (session.userInputStream, targetLayer, ' '))
        return ERROR::UNKNOWN;
    if (session.connectome.LayerCheck (targetLayer) == false) {
        std::cout << targetLayer << " is now a layer\n";
    }
    session.layermask.insert (targetLayer);
    return Project (session.connectome,
                    session.doodles,
                    session.selection,
                    session.layermask,
                    targetLayer);
}

int
Flatten (CliSession& session,
            CliDisplay& display)
{
    if (session.selection.size () == 0)
        return ERROR::UNKNOWN;
    Name targetLayer;
    if (!getline (session.userInputStream, targetLayer, ' '))
        return ERROR::UNKNOWN;
    if (session.connectome.LayerCheck (targetLayer) == false)
        std::cout << targetLayer << "is now a layer\n";
    session.layermask.insert (targetLayer);
    return Noodle::Flatten (session.connectome,
                            session.doodles,
                            session.selection,
                            session.layermask,
                            targetLayer);
}

int
Merge (CliSession& session,
            CliDisplay& display)
{
    Name target;
    if (!getline (session.userInputStream, target, ' '))
        return ERROR::UNKNOWN;
    Names toMerge = session.GetDoodlesFromStream ();
    if (toMerge.size () == 0) {
        string confirm;
        std::cout << "You are about to merge all doodles in the selection. Continue? y/n: ";
        std::cin >> confirm;
        if (confirm != "y")
            return ERROR::UNKNOWN;
        toMerge = session.selection;
    }
    display.Hide (toMerge);
    //todo: cleanup
    Names toDisplay;
    toDisplay.insert (target);
    display.Display (toDisplay);
    return Noodle::Merge (session.connectome,
                          session.doodles,
                          toMerge,
                          target,
                          session.workingdirectory);
}

int
Pluck (CliSession& session,
            CliDisplay& display)
{
    Name toPluck, target;
    int result = session.GetDoodleFromInput (toPluck);
    if (result != ERROR::NONE)
        return result;
    result = session.GetDoodleFromInput (target);
    if (result != ERROR::NONE)
        return result;
    /*result = Pluck (session.connectome,
                        toPluck,
                        target,
                        &session.layermask);
    Names toDisplay;
    toDisplay.insert (toPluck);
    toDisplay.insert (target);
    display.Display (toDisplay);
    return result;*/
    return ERROR::NOT_IMPLEMENTED;
}

int
SwapFile (CliSession& session,
            CliDisplay& display)
{
    Name toSwapA, toSwapB;
    int resultA = session.GetDoodleFromInput (toSwapA);
    int resultB = session.GetDoodleFromInput (toSwapB);
    if (resultA != ERROR::NONE || resultB != ERROR::NONE)
        return ERROR::UNKNOWN;
    //return Noodle::Swapfile (session.doodles,
    //                         toSwapA,
    //                         toSwapB);
    return ERROR::NOT_IMPLEMENTED;
}

int
SwapLinks (CliSession& session,
            CliDisplay& display)
{
    Name toSwapA, toSwapB;
    int resultA = session.GetDoodleFromInput (toSwapA);
    int resultB = session.GetDoodleFromInput (toSwapB);
    if (resultA != ERROR::NONE || resultB != ERROR::NONE)
        return ERROR::UNKNOWN;
    //return Noodle::Swaplinks (session.connectome,
    //                          toSwapA,
    //                          toSwapB);
    return ERROR::NOT_IMPLEMENTED;
}

int
SwapName (CliSession& session,
          CliDisplay& display)
{
    Name toSwapA, toSwapB;
    int resultA = session.GetDoodleFromInput (toSwapA);
    int resultB = session.GetDoodleFromInput (toSwapB);
    if (resultA != ERROR::NONE || resultB != ERROR::NONE)
        return ERROR::UNKNOWN;
    //return Noodle::Swapname (session.connectome,
    //                         session.doodles,
    //                         toSwapA,
    //                         toSwapB);
    return ERROR::NOT_IMPLEMENTED;
}

int
Transfer (CliSession& session,
            CliDisplay& display)
{
    Names toTransfer = session.selection;
    if (toTransfer.size () == 0)
        return ERROR::UNKNOWN;
    return Noodle::Transfer (session.connectome,
                             session.doodles,
                             toTransfer,
                             session.layermask);
}

int
Dissolve (CliSession& session,
          CliDisplay& display)
{
    Names toDissolve = session.GetDoodlesFromStream ();
    return Noodle::Dissolve (session.connectome,
                             session.doodles,
                             toDissolve);
}

int
Isolate (CliSession& session,
            CliDisplay& display)
{
    Names toIsolate = session.selection;
    return Noodle::Isolate (session.connectome,
                            toIsolate,
                            session.layermask);
}

int
Purge (CliSession& session,
        CliDisplay& display)
{
    Names toPurge = session.GetDoodlesFromStream ();
    if (toPurge.size () == 0)
        return ERROR::UNKNOWN;
    for (Name doodle : toPurge) {
        session.layermask.erase (doodle);
    }
    return Noodle::Purge (session.connectome,
                          session.doodles,
                          toPurge,
                          session.workingdirectory);
}

int
Remove (CliSession& session,
        CliDisplay& display)
{
    Names toRemove = session.GetDoodlesFromStream ();
    if (toRemove.size () == 0)
        return ERROR::UNKNOWN;
    int result = Noodle::Remove (session.connectome,
                           session.doodles,
                           toRemove,
                           session.layermask);
    display.Hide (toRemove);
    return result;
}
