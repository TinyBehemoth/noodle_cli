#include "layer.h"
#include "helpers.h"

enum LayerSubcommand {
    LscAll,
    LscAuto,
    LscCurrent,
    LscFocus,
    LscFull,
    LscHide,
    LscNew,
    LscDissolve,
    LscSet,

    LscInvalid = -1,
};

LayerSubcommand ResolveLayerSubcommand (string argument)
{
    if (argument == "all")      return LayerSubcommand::LscAll;
    if (argument == "a")        return LayerSubcommand::LscAll;
    if (argument == "auto")     return LayerSubcommand::LscAuto;
    if (argument == "current")  return LayerSubcommand::LscCurrent;
    if (argument == "c")        return LayerSubcommand::LscCurrent;
    if (argument == "focus")    return LayerSubcommand::LscFocus;
    if (argument == "f")        return LayerSubcommand::LscFocus;
    if (argument == "dissolve") return LayerSubcommand::LscDissolve;
    if (argument == "full")     return LayerSubcommand::LscFull;
    if (argument == "hide")     return LayerSubcommand::LscHide;
    if (argument == "new")      return LayerSubcommand::LscNew;
    if (argument == "set")      return LayerSubcommand::LscSet;

    return LayerSubcommand::LscInvalid;
}

int
ToggleOrCreate (string first, CliSession& session)
{
    Names toToggleOrCreate = session.GetDoodlesFromStream ();
    toToggleOrCreate.insert (first);
    Name candidateForCurrentLayer;

    for (Name doodle : toToggleOrCreate) {
        if (session.layermask.count (doodle)) {
            session.layermask.erase (doodle);
        }
        else {
            session.layermask.insert (doodle);
            Noodle::LayerNew (session.connectome, session.doodles, doodle);
            candidateForCurrentLayer = doodle;
        }
    }

    session.currentLayer = candidateForCurrentLayer;

    return ERROR::NONE;
}

int
LayerCurrent (CliSession& session, CliDisplay& display)
{
    Name currentLayer;
    getline (session.userInputStream, currentLayer);
    if (session.connectome.LayerCheck (currentLayer) == false)
        return ERROR::UNKNOWN;
    session.currentLayer = currentLayer;
    return ERROR::NONE;
}

int
LayerFocus (CliSession& session, CliDisplay& display)
{
    Names toFocus = session.GetDoodlesFromStream ();
    if (toFocus.size () == 0) {
        toFocus = session.selection;
    }
    if (toFocus.size () == 0) {
        toFocus.insert (session.currentLayer);
    }
    session.layermask.clear ();
    for (Name layer : toFocus) {
        if (session.connectome.LayerCheck (layer))
            session.layermask.insert (layer);
    }
    return ERROR::NONE;
}

int
LayerAuto (CliSession& session, CliDisplay& display)
{
    Names displayed = display.Displayed ();
    int result = Noodle::LayerAuto (&session.connectome,
                                    &displayed,
                                    session.layermask);
    return result;
}

int
LayerDissolve (CliSession& session, CliDisplay& display)
{
    Names toDissolve = session.GetDoodlesFromStream ();
    return Noodle::Dissolve (session.connectome, 
                            session.doodles,
                            &toDissolve);
}

int
LayerFull (CliSession& session, CliDisplay& display)
{
    Names displayed = display.Displayed ();
    return Noodle::LayerFull   (&session.connectome,
                                &displayed,
                                session.layermask);
}

int
LayerHide (CliSession& session, CliDisplay& display)
{
    Names toHide = session.GetDoodlesFromStream ();
    for (Name doodle : toHide) {
        session.layermask.erase (doodle);
    }
    return ERROR::NONE;
}

int
Layer  (CliSession& session,
        CliDisplay& display)
{
    string argument;
    getline (session.userInputStream, argument, ' ');
    switch (ResolveLayerSubcommand (argument)) {
    case LayerSubcommand::LscAll:
        return Noodle::LayerAll (&session.connectome, session.layermask);
    case LayerSubcommand::LscAuto:
        return LayerAuto (session, display);
    case LayerSubcommand::LscCurrent:
        return LayerCurrent (session, display);
    case LayerSubcommand::LscFocus:
        return LayerFocus (session, display);
    case LayerSubcommand::LscDissolve:
        return LayerDissolve (session, display);
    case LayerSubcommand::LscFull:
        return LayerFull (session, display);
    case LayerSubcommand::LscHide:
        return LayerHide (session, display);
    case LayerSubcommand::LscNew:
        break;
    case LayerSubcommand::LscSet:
        session.layermask = session.GetDoodlesFromStream ();
        //todo: handle currentlayer
        break;
    default:
        ToggleOrCreate (argument, session);
        break;
    }

    return ERROR::NONE;
}
