#include "navigation.h"

//   Navigation
//   ----------

int
Dive   (CliSession& session,
        CliDisplay& display)
{
    int result;
    Names doodlesToDive = display.Displayed ();
    result = Noodle::Dive  (session.connectome,
                            doodlesToDive,
                            session.layermask);
    display.Clear ();
    display.Display (doodlesToDive);
    return result;
}

int
Collapse (CliSession& session,
          CliDisplay&  display)
{
    Names toCollapse = session.GetDoodlesFromStream ();
    if (toCollapse.size () == 0)
        return ERROR::UNKNOWN;
    // reduce layermask
    Names reducedLayermask = session.layermask;
    for (Name layer : toCollapse) {
         reducedLayermask.erase (layer);
    }
    // compile initial list of doodles to Hide
    Names displayed = display.Displayed ();
    Names toHide;
    for (Name layer : toCollapse) {
        for (Name doodle : displayed) {
            if (session.connectome.DoodleCheck (doodle, layer))
                toHide.insert (doodle);
        }
    }
    // reduce the list based on adjacencies under the reduced layermask
    Names toHideReduced = toHide;
    for (Name layer : reducedLayermask) {
        for (Name doodle : toHide) {
            Names adjacencies = session.connectome.AdjacencyList (doodle, layer);
            for (Name adjacency : adjacencies) {
                if (displayed.count (adjacency) > 0
                    && toHide.count (adjacency) == 0) {
                        toHideReduced.erase (doodle);
                        break;
                }
            }
        }
    }
    display.Hide (toHideReduced);
    session.layermask = reducedLayermask;
    return ERROR::NONE;
}

int 
Expand (CliSession& session,
        CliDisplay& display)
{
    Names toExpand = session.GetDoodlesFromStream ();
    for (Name doodle : toExpand) {
        if (session.connectome.LayerCheck (doodle) == false)
            continue;
        session.layermask.insert (doodle);
        display.Display (session.connectome.DoodleList (doodle));
    }
    return ERROR::NONE;
}

int
Explore    (CliSession& session,
            CliDisplay& display)
{
    int result;

    // userInputStream specifies doodles
    Names toExplore = session.GetDoodlesFromStream ();
    if (toExplore.size () > 0) {
        result = Noodle::Explore   (session.connectome,
                                    toExplore,
                                    session.layermask);
        display.Display (toExplore);
        return result;
    }

    // selection specifies doodles
    if (session.selection.size () > 0) {
        result = Noodle::Explore   (session.connectome,
                                    session.selection,
                                    session.layermask);
        // The selection is always displayed anyway.
        return result;
    }

    // Explore all displayed doodles by default.
    toExplore = display.Displayed ();
    result = Noodle::Explore   (session.connectome,
                                toExplore,
                                session.layermask);
    display.Display (toExplore);
    return result;
}

int
Jump   (CliSession& session,
        CliDisplay& display)
{
    int result;
    Names toJump = display.Displayed ();
    result = Noodle::Jump (session.connectome, toJump, session.layermask);
    display.Clear ();
    display.Display (toJump);
    return result;
}

int
Focus  (CliSession& session,
        CliDisplay& display)
{
    // userInputStream specifies doodles
    Names toFocus = session.GetDoodlesFromStream ();
    if (toFocus.size () > 0) {
        display.Clear ();
        display.Display (toFocus);
    }

    // use selection
    else if (session.selection.size () > 0) {
        display.Clear ();
        display.Display (session.selection);
    }

    // no doodles specified

    return ERROR::NONE;
}

int
Grow   (CliSession& session,
        CliDisplay& display)
{
    int result;

    // userInputStream specifies doodles
    Names toGrow = session.GetDoodlesFromStream ();
    if (toGrow.size () > 0) {
        result = Noodle::Grow  (session.connectome,
                                toGrow,
                                session.layermask);
    }

    // selection specifies doodles
    else if (session.selection.size () > 0) {
        result = Noodle::Grow  (session.connectome,
                                session.selection,
                                session.layermask);
    }

    else {
        toGrow = display.Displayed ();
        result = Noodle::Grow  (session.connectome,
                                toGrow,
                                session.layermask);

    }

    display.Display (toGrow);

    return result;
}

int
Hide   (CliSession& session,
        CliDisplay& display)
{
    Names toHide = session.GetDoodlesFromStream ();
    if (toHide.size () == 0) {
        toHide = session.selection;
        session.selection.clear ();
    }
    display.Hide (toHide);
    return ERROR::NONE;
}

int
Shrink (CliSession& session,
        CliDisplay& display)
{
    int result;
    Names toShrink = display.Displayed ();
    result = Noodle::Shrink    (session.connectome,
                                toShrink,
                                session.layermask);
    return result;
}

int
Switch (CliSession& session,
        CliDisplay& display)
{
    int result;
    Names toSwitchTo = session.GetDoodlesFromStream ();
    if (toSwitchTo.size () == 0)
        return ERROR::UNKNOWN;
    Names displayed = display.Displayed ();
    result = Noodle::Switch    (session.connectome,
                                session.doodles,
                                displayed,
                                session.layermask,
                                toSwitchTo);
    session.currentLayer = *toSwitchTo.begin ();
    display.Clear ();
    display.Display (displayed);
    return result;
}

int
Subsplore  (CliSession& session,
            CliDisplay& display)
{
    Names toSubsplore = session.GetDoodlesFromStream ();
    if (toSubsplore.size () == 0)
        toSubsplore = session.selection;
    session.selection;
    if (toSubsplore.size () == 0)
        toSubsplore = display.Displayed ();
    Noodle::Subsplore  (session.connectome,
                        toSubsplore,
                        session.layermask);
    display.Display (toSubsplore);
    return ERROR::NONE;
}
