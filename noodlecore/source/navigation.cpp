#include "noodlecore.h"
#include "helpers.h"
#include "returnType.h"

namespace Noodle
{

// Navigation
// ----------

int
Dive   (const Mpg&  connectome,
        Names&      toDiveInto,
        Names&      layermask)
{
    // Update layermask. todo: make this a subset
    layermask = toDiveInto;
    toDiveInto.clear ();

    // Replace toDiveInto with the union of doodles contained in layers.
    for (Name toDive : layermask) {
        Names doodlesInLayer = connectome.DoodleList (toDive);
        toDiveInto.insert (doodlesInLayer.begin (), doodlesInLayer.end ());
    }

    return ERROR::NONE;
}

int
Explore (const Mpg& connectome,
         Names&     toExplore,
         Names&     layerList)
{
    for (Name doodle : toExplore) {
        for (Name layer : connectome.LayerList ()) {
            Names adjacencies = connectome.AdjacencyList (doodle, layer);
            toExplore.insert (adjacencies.begin (), adjacencies.end ());
            if (adjacencies.size ())
                layerList.insert (layer);
        }
    }
    return ERROR::NONE;
}

int
Jump   (const Mpg&  connectome,
        Names&      jumpDoodles,
        Names&      layermask)
{
/*  From specification alpha-1.0:
    -----------------------------
    The layermask becomes the set of displayed doodles.
    The new layermask is the result of layer auto of that set.
    Any selected doodles will continue being displayed.

    Modified behaviour to update:
    -----------------------------
    "Supersplore" the doodles and optimize.
*/
    Names toJumpOutOf = jumpDoodles;
    jumpDoodles.clear ();
    for (Name toJump : toJumpOutOf) {
        Names layersForDoodle = connectome.LayerList (toJump);
        jumpDoodles.insert (layersForDoodle.begin (), layersForDoodle.end ());
    }
    
    //todo: shorten jumpDoodles set based on toJumpOutOf
    //we only need minimal coverage

    toJumpOutOf = layermask;
    layermask.clear ();
    for (Name layer : toJumpOutOf) {
        Names newLayermask = connectome.LayerList (layer);
        if (newLayermask.size () == 0)
            continue;
        layermask.insert (newLayermask.begin (), newLayermask.end ());
    }

    //todo: shorten the layermask list for minimal coverage

    return ERROR::NONE;
}

//int Focus ();

int
Grow   (const Mpg&      connectome,
        Names&          toGrow,
        const Names&    layermask)
{
    for (Name doodle : toGrow) {
        for (Name layer : layermask) {
            //todo: fix the adjacency list function.
            for (Name adjacent : connectome.AdjacencyList(doodle, layer)) {
                toGrow.insert(adjacent);
            }
        }
    }
    return ERROR::NONE;
}

//int Hide ();

int
Shrink (const Mpg&      connectome,
        Names&          toShrink,
        const Names&    layermask)
{
    // calculate center
    Name center = CalculateCenter(connectome, toShrink, layermask);

    // breadth first traverse starting at center

    // remove the last traverselayer from toShrink

    return ERROR::NOT_IMPLEMENTED;
}

int 
Switch (Mpg&         connectome,
        Doodles&     doodles,
        Names&       displayed,
        Names&       layermask,
        const Names& toSwitchTo)
{
    ERROR result = ERROR::NONE;

    displayed.clear();
    for (Name layer : toSwitchTo) {
        if (connectome.LayerCheck (layer) == false)
            LayerNew (connectome, doodles, layer);
        Names doodlesInLayer = connectome.DoodleList (layer);
        displayed.insert (doodlesInLayer.begin (), doodlesInLayer.end ());
    }
    layermask.clear();
    layermask = toSwitchTo;

    return result;
}

int
Subsplore  (const Mpg&  connectome,
            Names&      toSubsplore,
            Names&      layermask)
{
    Names workingSploreSet, workingLayermask = layermask;
    bool hasSubsplored = false;
    for (Name doodle : toSubsplore) {
        if (connectome.LayerCheck (doodle) == false)
            continue;

        bool hasSubsploredCurrentDoodle = false;
        
        // Check connections with origin. Splore there if it exists.
        for (Name adjacent : connectome.AdjacencyList(origin, doodle)) {
            //todo: incorrect behaiovur!!
            //you should grow on the sublayer
            workingSploreSet.insert(adjacent);
            hasSubsplored = hasSubsploredCurrentDoodle = true;
        }
        if (hasSubsploredCurrentDoodle) {
            workingLayermask.insert(doodle);
            continue;
        }
        
        // No origin? Use the self doodle.
        for (Name adjacent : connectome.AdjacencyList(doodle, doodle)) {
            workingSploreSet.insert(adjacent);
            hasSubsplored = hasSubsploredCurrentDoodle = true;
        }
        if (hasSubsploredCurrentDoodle) {
            workingLayermask.insert(doodle);
            continue;
        }
        
        // No self doodle? Load the entire layer.
        for (Name subdoodle : connectome.DoodleList (doodle)) {
            workingSploreSet.insert(subdoodle);
            hasSubsplored = hasSubsploredCurrentDoodle = true;
        }
        if (hasSubsploredCurrentDoodle) {
            workingLayermask.insert(doodle);
            continue;
        }
    }   

    if (hasSubsplored) {
        // if there is no subsplore leave the lists unchanged?
        //todo: decide if this is what we want
        //it might make more sense to do a check at the UI level
        toSubsplore = workingSploreSet;
        layermask = workingLayermask;
    } 

    return ERROR::NONE;
}

//int ToggleView ();

//int ToggleRotationDirection ();

}   // namespace Noodle
