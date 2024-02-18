#include "noodlecore.h"
#include "helpers.h"
#include "returnType.h"

namespace Noodle
{

//   Layer
//   -----

int
LayerAll   (const Mpg&      connectome,
            Names&          listOfAllLayers)
{
    listOfAllLayers = connectome.LayerList ();
    return ERROR::NONE;
}

int
LayerAuto  (const Mpg&      connectome,
            const Names&    doodlesToCover,
            Names&          resultingLayerList)
{
    //todo:
    // flat metagraph
    // breadthfirst traverse
    // terminate when we've covered the doodles
    return ERROR::NOT_IMPLEMENTED;
}

//int Current ();

//int Focus ();

int
LayerFull  (const Mpg&      connectome,
            const Names&    displayed,
            Names&          layermask)
{
    layermask.clear ();
    for (Name doodle : displayed) {
        Names layersForDoodle = connectome.LayerList (doodle); 
        layermask.insert (layersForDoodle.begin (), layersForDoodle.end());
    }
    return ERROR::NONE;
}

//int Hide ();

int
LayerNew   (Mpg&        connectome,
            Doodles&    doodles,
            Name        newLayer)
{
    if (connectome.LayerCheck (newLayer))
        return ERROR::UNKNOWN;  // layer already exists

    connectome.LayerAdd (newLayer);

    // meta doodle
    if (doodles.find (newLayer) == doodles.end ()) {
        Doodle data = {
            0,
            true,   // abstract
            true,   // layer
            false,  // directory
            "/.noodle/" + newLayer,
            newLayer
        };
        doodles.emplace (newLayer, data);
    } else {
        doodles.at (newLayer).layer = true;
    }
    
    Metaauto (connectome);

    return ERROR::NONE;
}

int
Dissolve   (Mpg&        connectome,
            Doodles&    doodles,
            Name        layerToRemove)
{
    Names doodlesToHandle = connectome.DoodleList (layerToRemove);

    connectome.LayerRemove (layerToRemove);

    // meta doodle
    Names externalStructure = connectome.LayerList (layerToRemove);
    //todo: make sure the base and meta autobehaviours match up with this.
    bool isOnlyMeta = externalStructure.size () <= 2
        && externalStructure.count (base) 
        && externalStructure.count (meta)
        && doodles.at (layerToRemove).abstract;
    
    if (isOnlyMeta)
        connectome.DoodleRemove (layerToRemove);

    // cleanup
    for (Name toHandle : doodlesToHandle) {
        HandleUnlinked (connectome, toHandle, layerToRemove);
    }
    return ERROR::NONE;
}

int
Dissolve   (Mpg&            connectome,
            Doodles&        doodles,
            const Names&    toDissolve)
{
    int result;
    for (Name doodle : toDissolve) {
        result  = Dissolve (connectome, doodles, doodle);
        if (result != ERROR::NONE)
            return result;
    }
    return result;
}

}   // namespace Noodle
