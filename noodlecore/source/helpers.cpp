#include "helpers.h"
#include "noodlecore.h"
#include "returnType.h"

namespace Noodle
{

bool
CreateIfDoesntExist    (Doodles&    doodles,
                        Name        toCreate)
{
    if (doodles.find (toCreate) != doodles.end ())
        return false;
    Doodle data = {
        0,                      // nameHash
        true,                   // abstract
        false,                  // meta
        false,                  // visited
        ".noodle/" + toCreate,  // path
        toCreate                // name
    };
    doodles.emplace (toCreate, data);
    return true;
}

Name
CalculateCenter    (const Mpg&      connectome,
                    const Names&    doodles,
                    const Names&    layermask)
{
    //todo: implement
    return *(doodles.begin ());
}

Name
CalculateMetaLink  (Mpg&    connectome,
                    Name    metaDoodle)
{
    //todo: find longest inclusion chain back to origin?
    return origin;
}

Name
CalculateBaseLink (Mpg& connectome,
                   Name doodle)
{
    auto multiList = connectome.AdjacencyMultiList (doodle);
    multiList.erase (base);
    multiList.erase (meta);

    // Calculate scores.
    unordered_map<Name,int> adjacencyScores;
    unordered_map<Name,int> layerScores;
    for (auto namedAdjacencyList : multiList) {
        layerScores.emplace (namedAdjacencyList.first, namedAdjacencyList.second.size ());
        for (Name adjacency : namedAdjacencyList.second) {
            if (adjacencyScores.count (adjacency))
                adjacencyScores.at (adjacency)++;
            else
                adjacencyScores.emplace (adjacency, 1);
        }
    }
     
    // Try adjacency scores first.
    int maxScore = 0;
    Name autoBaseAdjacency = origin;
    for (auto score : adjacencyScores) {
        if (score.second > maxScore) {
            autoBaseAdjacency = score.first;
            maxScore = score.second;
        }
    }
    if (maxScore > 1)
        return autoBaseAdjacency;
    
    // Use layer inclusion if scores are too low.
    maxScore = 0;
    autoBaseAdjacency = origin;
    for (auto score : layerScores) {
        if (score.second > maxScore) {
            autoBaseAdjacency = score.first;
            autoBaseAdjacency = score.second;
        }
    }

    return autoBaseAdjacency;
}

/*  Generates a new base layer based on the filesystem.
 *  The filesystem tree is used as scaffold.
 *  The workingdirectory is the origin.
 *  Abstract doodles are added subsequently.
*/
void
BaseFromFilesystem (Mpg&     connectome,
                    Doodles& doodles,
                    string   workingdirectory)
{
    // Refresh

    // Dissolve base layer.
    connectome.LayerRemove (base);
    
    // Recreate fs structure.
    Name currentSubdir = workingdirectory;
    for (const auto& fsElement : fs::directory_iterator (workingdirectory)) {
        Name current = FileNameFromPath (fsElement.path ());
        if (IsHidden (workingdirectory, fsElement.path ()))
            continue;

        connectome.Link (current, currentSubdir, base);

        if (fsElement.is_directory ()) {
            currentSubdir = current;
            CreateIfDoesntExist (doodles, current);
        }
    }

    // Add non-directory, abstract doodles.
    // These were missed in the fs iteration.
    for (auto namedDoodle : doodles) {
        if (namedDoodle.second.abstract == false)
            continue;
        if (connectome.DoodleCheck(namedDoodle.first, base))
            continue;
        Name autoBaseAdjacency = CalculateBaseLink (connectome, namedDoodle.first);
        connectome.Link (namedDoodle.first, autoBaseAdjacency, base);
    }
}

int
HandleUnlinked (Mpg& connectome,
                Name toUnlink,
                Name unlinkLayer)
{
    // Check unlinked meta-doodle
    //todo: reconsider this and other meta auto-behaviours
    int degree = connectome.AdjacencyList (toUnlink, unlinkLayer).size ();
    if (unlinkLayer == "meta"
        && degree == 0) {
            // Link on the meta layer
            Name metaLink = CalculateMetaLink (connectome, toUnlink);
            connectome.Link (metaLink, toUnlink, "meta");
            return ERROR::NONE;
    }

    // Check for orphaned doodle
    degree = 0;
    for (Name layer : connectome.LayerList ()) {
        degree += connectome.AdjacencyList (toUnlink, layer).size ();
    }
    if (degree == 0) {
        connectome.Link (toUnlink, "origin", "base");
    }

    return ERROR::NONE;
}

//todo: move this to the Mpg class
void
CopyInternal   (Mpg& connectome,
                Name from,
                Name to)
{
    for (Name doodle : connectome.DoodleList (from)) {
        for (Name adjacency : connectome.AdjacencyList (from, doodle)) {
            connectome.Link (doodle, adjacency, to);
        }
    }
}

void
CopyInternalNewSelf (Mpg& connectome,
                     Name from,
                     Name to)
{
    for (Name internalDoodle : connectome.DoodleList (from)) {
        Names adjacencyList = connectome.AdjacencyList (internalDoodle, from);
        if (internalDoodle == from)
            internalDoodle = to;
        for (Name adjacency : adjacencyList) {
            connectome.Link (internalDoodle, adjacency, to);
        }
    }
}

void
CopyExternal   (Mpg& connectome,
                Name from,
                Name to)
{
    //todo: is it faster to use the multilist? or just LayerList
    auto adjacencyMultiList = connectome.AdjacencyMultiList (from);
    for (auto& namedAdjacencyList : adjacencyMultiList) {
        for (Name adjacency : namedAdjacencyList.second) {
            connectome.Link (to, adjacency, namedAdjacencyList.first);
        }
    }
}

string
FileNameFromPath (string path)
{
    string fileName;
    std::stringstream pathStream (path);
    while (std::getline (pathStream,fileName,'/'));
    return fileName;
}

string
DirectoryFromFilePath (string path)
{
    string directoryPath = "";
    string builder, previousBuilder;
    std::stringstream pathStream (path);
    std::getline (pathStream, builder, '/');
    previousBuilder = builder;
    while (std::getline (pathStream, builder, '/')) {
        directoryPath = directoryPath + "/" + previousBuilder;
        previousBuilder = builder;
    }
    return directoryPath + "/";
}


string
ConvertToRelativePath  (string workingdirectory,
                        string fullPath)
{
    // find noodle's root folder (working directory)
    std::stringstream workingdirTailStream(workingdirectory);
    string workingdirTail;
    while (getline(workingdirTailStream, workingdirTail, '/'));
    string next;
    std::stringstream pathStream(fullPath);
    while (getline(pathStream, next, '/') && next != workingdirTail);

    // build the relative path
    string relativePath = "";
    while (getline(pathStream, next, '/')) {
        relativePath += "/" + next;
    }
    return relativePath;    
}

bool
IsHidden   (string workingdirectory,
            string dirPath)
{
    std::stringstream pathStream (ConvertToRelativePath (workingdirectory, dirPath));
    string dirOrFileName;
    while (std::getline (pathStream, dirOrFileName, '/')) {
        if (!dirOrFileName.empty () && dirOrFileName.c_str ()[0] == '.') {
            return true;
        }
    }
    return false;
}


}   // namespace Noodle
