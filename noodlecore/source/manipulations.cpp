#include "noodlecore.h"
#include "helpers.h"
#include "returnType.h"

namespace Noodle
{

//   Manipulations
//   -------------
int
Chain  (Mpg&            connectome,
        Doodles&        doodles,
        vector<Name>    listOfNames,
        Name            layer)
{
    Name previous;
    for (Name toChain : listOfNames) {
        if (previous.empty ()) {
            previous = toChain;
            CreateIfDoesntExist (doodles, previous);   
            continue;
        }
        CreateIfDoesntExist (doodles, toChain);
        connectome.Link (previous, toChain, layer);
        previous = toChain;
    }
    return ERROR::NONE;
}
//int Clone ();

int
Sprout     (Mpg&            connectome,
            Doodles&        doodles,
            Name            sproutRoot,
            const Names&    toSprout,
            Name            layer)
{
    CreateIfDoesntExist (doodles, sproutRoot);
    for (Name sproutling : toSprout) {
        CreateIfDoesntExist (doodles, sproutling);
        connectome.Link (sproutRoot, sproutling, layer);
    }
    return ERROR::NONE;
}

int
Subchain   (Mpg&            connectome,
            Doodles&        doodles,
            const Names&    toChain)
{
    Name previous;
    for (Name next : toChain) {
        if (previous.empty ()) {
            previous = next;
            CreateIfDoesntExist (doodles, previous);   
            continue;
        }
        CreateIfDoesntExist (doodles, next);
        connectome.Link (previous, next, previous);
        previous = next;
    }
    return ERROR::NONE;
}

int
Subsprout  (Mpg&            connectome,
            Doodles&        doodles,
            Name            sproutRoot,
            const Names&    toSprout)
{
    CreateIfDoesntExist (doodles, sproutRoot);
    for (Name sproutling : toSprout) {
        CreateIfDoesntExist (doodles, sproutling);
        connectome.Link (sproutRoot, sproutling, sproutRoot);
    }
    return ERROR::NONE;
}

int
Superchain (Mpg&            connectome,
            Doodles&        doodles,
            const Names&    toChain)
{
    Name previous;
    for (Name current : toChain) {
        if (previous.empty ()) {
            previous = current;
            CreateIfDoesntExist (doodles, previous);   
            continue;
        }
        CreateIfDoesntExist (doodles, current);
        connectome.Link (previous, current, current);
        previous = current;
    }
    return ERROR::NONE;
}

int
Phase      (Mpg&            connectome,
            Doodles&        doodles,
            const Names&    toPhase,
            PhaseOptions    options)
{
    string prefix = "phase";
    //todo: check for lingering phase doodles.

    //todo: implement options

    for (Name doodle : toPhase) {
        string phaseClone = prefix + "." + doodle;
        //todo: !!! wrong we should abort on collision
        CreateIfDoesntExist (doodles, phaseClone);
        //todo: !!! wrong again, internal links are not copied
        CopyExternal (connectome, doodle, phaseClone);
        if (doodles.at (doodle).abstract == false) {
            //todo: copy file
        }
    }
    return ERROR::NOT_IMPLEMENTED;
}

int
Project    (Mpg&            connectome,
            Doodles&        doodles,
            const Names&    toProject,
            const Names&    sourceLayers,
            Name            targetLayer)
{
    //todo: double check what layercheck does. I might be using it wrong.
    if (connectome.LayerCheck (targetLayer) == false)
        LayerNew (connectome, doodles, targetLayer);

    for (Name doodle : toProject) {
    for (Name layer : sourceLayers) {
        // skip the target layer
        if (layer == targetLayer || connectome.DoodleCheck (doodle, layer) == false) {
            continue;
        }
        Names adjacencyList = connectome.AdjacencyList (layer, doodle);
        for (Name adjacency : adjacencyList) {
            if (toProject.count (adjacency) == 0)
                continue;
            // don't worry about double links bc we unlink as we go.
            //todo: saftey and error checks?
            connectome.Unlink (doodle, adjacency, targetLayer);
        } 
    }   // doodle iteration
    }   // layer iteration
    return ERROR::NONE;
}

int
Flatten    (Mpg&            connectome,
            Doodles&        doodles,
            const Names&    toProject,
            const Names&    sourceLayers,
            Name            targetLayer)
{
    if (connectome.LayerCheck (targetLayer) == false)
        LayerNew (connectome, doodles, targetLayer);

    for (Name doodle : toProject) {
    for (Name layer : sourceLayers) {
        // skip the targetLayer
        if (layer == targetLayer)
            continue;
        Names adjacencyList = connectome.AdjacencyList (doodle, layer);
        for (Name adjacency : adjacencyList) {
            // only recreate links between selected doodles
            if (toProject.count (adjacency) == 0)
                continue;
            Unlink (connectome, doodle, adjacency, layer);
            Link (connectome, doodle, adjacency, targetLayer);
            }
        }
    }

    return ERROR::NONE;
}

//todo: perhaps the order of the merged items matters.
int
Merge (Mpg&         connectome,
       Doodles&     doodles,
       const Names& toMerge,
       Name         mergedDoodle,
       string       workingdirectory)
{
    bool hasMarkdown = false;
    bool abstractOnly = true;

    // file checks
    for (Name doodle : toMerge) {

        // check for bad input
        if (doodle == origin || doodle == meta || doodle == base)
            return ERROR::BAD_PARAMETER;
        auto data = doodles.find (doodle);
        if (data == doodles.end ())
            return ERROR::DOODLE_NOT_FOUND;

        // we can always merge abstract doodles
        if (data->second.abstract)
            continue;
        else
            abstractOnly = false;

        // we can only merge text files
        string fileExtension;
        std::stringstream pathStream (data->second.path);
        while (getline (pathStream, fileExtension, '.'));
        if (fileExtension.empty() == false
        //todo: use list of text file extensions.
        && fileExtension != "md"
        && fileExtension != "txt") {
            return ERROR::UNKNOWN;
        }
        // markdown format is prioritized for now
        //todo: reconsider this behaviour
        hasMarkdown |= fileExtension == "md";
    }

    //todo: detect collisions?

    // mergedDoodle
    CreateIfDoesntExist (doodles, mergedDoodle);

    // merge links
    for (Name doodle : toMerge) {
        CopyExternal (connectome, doodle, mergedDoodle);
    }

    // merge layers
    connectome.LayerAdd (mergedDoodle);

    for (Name doodle : toMerge) {
        if (connectome.LayerCheck (doodle))
            CopyInternalNewSelf (connectome, doodle, mergedDoodle);
    }

    if (abstractOnly == false) {
        // merge files
        std::ofstream mergedFile;
        if (doodles.at (mergedDoodle).abstract)
            mergedFile.open (workingdirectory + "/" + mergedDoodle, std::ios_base::app);
        else
            mergedFile.open (workingdirectory + doodles.at (mergedDoodle).path, std::ios_base::app);
        mergedFile << "\n\n";
        for (Name doodle : toMerge) {
            if (hasMarkdown) {
                //todo: check heading 1 titles
                //todo: add titles for the markdown
                //todo: other
            }
            std::ifstream fileMerge (workingdirectory + "/" + doodles.at (doodle).path);
            mergedFile << fileMerge.rdbuf() << "\n\n";
        }
        mergedFile.close ();
    }
    //todo: safety checks

    // cleanup
    Purge (connectome, doodles, toMerge, workingdirectory);

    return ERROR::NONE;
}

int
Pluck      (Mpg&            connectome,
            Name            toPluck,
            Name            target,
            Name            placeLayer,
            const Names&    layermask)
{
    Names toIsolate;
    toIsolate.insert (toPluck);
    Isolate (connectome, toIsolate, layermask);
    connectome.Link (toPluck, target, placeLayer);
    return ERROR::NONE;
}

int
Swapfile   (Doodles&        doodles,
            Name            toSwapA,
            Name            toSwapB,
            string          workingdirectory)
{
    string tempName = ".noodletemp";
    string pathA = workingdirectory + DirectoryFromFilePath (doodles.at (toSwapA).path);
    string pathB = workingdirectory + DirectoryFromFilePath (doodles.at (toSwapB).path);

    // rename first to temp
    fs::rename (pathA + toSwapA, pathA + tempName);
    // rename second to first
    fs::rename (pathB + toSwapB, pathB + toSwapA);
    // rename temp to second
    fs::rename (pathA + tempName, pathA + toSwapB);

    return ERROR::NONE;
}

int
Swaplinks  (Mpg&            connectome,
            Name            toSwapA,
            Name            toSwapB)
{
    string tempName = ".noodletemp";

    // copy external swapA to temp
    CopyExternal (connectome, toSwapA, tempName);
    // clear swapA
    //todo: this is probably inefficient
    for (Name layer : connectome.LayerList (toSwapA)) {
        connectome.Unlink (toSwapA, layer);
    }
    // copy external swapB to swapA
    CopyExternal (connectome, toSwapB, toSwapA);
    // clear swapB
    //todo: again inefficient
    for (Name layer : connectome.LayerList (toSwapB)) {
        connectome.Unlink (toSwapB, layer);
    }
    // copy external temp to swapB
    CopyExternal (connectome, tempName, toSwapB);

    //todo: cleanup
    //Delete (connectome, doodles, tempName);

    return ERROR::NONE;
}

int
Swapname   (Mpg&        connectome,
            Doodles&    doodles,
            Name        toSwapA,
            Name        toSwapB,
            string      workingdirectory)
{
    string tempName = ".noodletemp";
    // rename A to temp
    Rename (connectome, doodles, toSwapA, tempName, workingdirectory);
    // rename B to A
    Rename (connectome, doodles, toSwapB, toSwapA, workingdirectory);
    // rename temp to B
    Rename (connectome, doodles, tempName, toSwapB, workingdirectory);
    return ERROR::NONE;
}

int
Transfer   (Mpg&            connectome,
            Doodles&        doodles,
            const Names&    toTransfer,
            const Names&    layermask)
{
    return ERROR::NOT_IMPLEMENTED;
}

int
Isolate    (Mpg&            connectome,
            const Names&    toIsolate,
            const Names&    layermask)
{
    for (Name doodle : toIsolate) {
    for (Name layer : layermask) {
        Names adjacencyList = connectome.AdjacencyList (layer, doodle);
        for (Name adjacency : adjacencyList) {
            if (toIsolate.count (adjacency))
                continue;
            connectome.Unlink (doodle, adjacency, layer);
        }
    }
    }
    return ERROR::NONE;
}

int
Purge (Mpg&         connectome,
       Doodles&     doodles,
       const Names& toPurge,
       string       workingdirectory)
{
    Dissolve (connectome, doodles, toPurge);
    for (Name doodle : toPurge) {
        Delete (connectome, doodles, doodle, workingdirectory);
    }
    return ERROR::NONE; //todo: real result
}

int
Remove     (Mpg&            connectome,
            Doodles&        doodles,
            const Names&    toRemove,
            const Names&    layermask)
{
    // doodles external structure
    for (Name doodle : toRemove) {
    for (Name layer : layermask) {
        connectome.Unlink (doodle, layer);
    }
    }

    for (Name doodle : toRemove) {
        if (connectome.LayerList (doodle).size ())
            continue;
        // orphaned doodles
        bool abstract = doodles.at (doodle).abstract;
        bool isLayer = connectome.LayerCheck (doodle); 
        if (abstract && (isLayer == false))
            doodles.erase (doodle);
        else
            //todo: recalculate meta and base instead
            connectome.Link (doodle, origin, base);
    }
    return ERROR::NONE;
}

//int Clean ();

}   // namespace Noodle
