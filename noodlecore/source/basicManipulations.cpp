#include "noodlecore.h"
#include "helpers.h"
#include "returnType.h"

namespace Noodle
{

//   Basic
//   -----

//todo: implement default behaviours and user error checks

int
Delete (Mpg&     connectome,
        Doodles& doodles,
        Name     toDelete,
        string   workingdirectory)
{
    if (toDelete == origin || toDelete == meta || toDelete == base)
        return ERROR::UNKNOWN;

    // remove the file
    fs::remove (workingdirectory + doodles.at (toDelete).path);
    
    // remove external structure
    for (Name layer : connectome.LayerList ()) {
        if (layer == toDelete)
            continue;
        connectome.Unlink (toDelete, layer);
    }
    
    // preserve internal structure
    if (connectome.LayerCheck (toDelete)) {
        doodles.at (toDelete).abstract = true;
        //todo: calculate base before linking
        Link (connectome, toDelete, origin, base);
        return ERROR::NONE;
    }

    // remove from doodles
    doodles.erase (toDelete);
    return ERROR::NONE;
}

int
Link (Mpg& connectome,
      Name toLinkA,
      Name toLinkB,
      Name layer)
{
    if (connectome.DoodleCheck (toLinkA) == false)
        return ERROR::DOODLE_NOT_FOUND;
    if (connectome.DoodleCheck (toLinkB) == false)
        return ERROR::DOODLE_NOT_FOUND;

    return connectome.Link (toLinkA, toLinkB, layer);
}

int
New (Mpg&     connectome,
     Doodles& doodles,
     Name     newDoodle)
{
    if (CreateIfDoesntExist (doodles, newDoodle)) {
        Name autoBaseAdjacency = CalculateBaseLink (connectome, newDoodle);
        connectome.Link (autoBaseAdjacency, newDoodle, base);
    }
    return ERROR::NONE;
}

int
Rename (Mpg&     connectome,
        Doodles& doodles,
        Name     original,
        Name     newName,
        string   workingdirectory)
{
    // Check for valid input

    if ((doodles.count (original) == 0) ||
        doodles.count (newName)) {
            return ERROR::UNKNOWN;
    }

    Doodle data = doodles.at(original);
    
    // Update filename
    // !!! Assumes directory names and doodle names don't collide !!!
    std::stringstream pathStream (data.path);
    string next, newPath = "";
    getline (pathStream, next, '/'); // ignore leading '/'   todo: remove leading '/'
    for (;getline (pathStream, next, '/');) {
        if (next != original)
            newPath = newPath + "/" + next;
    }
    newPath += "/" + newName;
    if (data.abstract == false)
        fs::rename (workingdirectory + data.path, workingdirectory + newPath);

    // Update data
    data.name = newName;
    data.path = newPath;
    doodles.erase (original);
    doodles.emplace (newName, data);

    // Update connectome
    if (connectome.LayerCheck (original)) {
        // Recreate internal structure
        connectome.LayerAdd (newName);
        CopyInternalNewSelf (connectome, original, newName);
    }
    // Recreate external structure
    CopyExternal (connectome, original, newName);
    // Remove old doodle
    if (connectome.LayerCheck (original))
        connectome.LayerRemove (original);
    connectome.DoodleRemove(original);

    return ERROR::NONE;
}

int
Unlink (Mpg& connectome,
        Name toUnlinkA,
        Name toUnlinkB,
        Name layer)
{
    int result = ERROR::NONE;

    // Check for valid input
    if (!connectome.LayerCheck (layer)) {
        return ERROR::LAYER_NOT_FOUND;
    }
    if (!connectome.DoodleCheck (toUnlinkA, layer)
        || !connectome.DoodleCheck (toUnlinkB, layer)) {
            return ERROR::DOODLE_NOT_FOUND;
    }

    //todo: Check for illegal manipulations??
    //todo: autogen base
    
    // Perform the manipulation
    connectome.Unlink (toUnlinkA, toUnlinkB, layer);
    result = HandleUnlinked (connectome, toUnlinkA, layer);
    if (result != ERROR::NONE)
        return result;
    result = HandleUnlinked (connectome, toUnlinkB, layer);
    if (result != ERROR::NONE)
        return result;
    
    return ERROR::NONE;
}

}   // namespace Noodle
