#include "noodlecore.h"
#include "returnType.h"
#include "helpers.h"

namespace Noodle
{

int
Baseauto   (Mpg&     connectome,
            Doodles& doodles,
            string   workingdirectory)
{
    BaseFromFilesystem (connectome, doodles, workingdirectory);

    return ERROR::NONE;
}


int
Metaauto   (Mpg& connectome)
{
    // Doodles which are not included in layers
    Names highestLayers = connectome.LayerList ();
    highestLayers.erase (base);
    highestLayers.erase (meta);
    for (Name connectomeLayer : connectome.LayerList ()) {
        if (connectomeLayer == base)
            continue;
        if (connectomeLayer == meta)
            continue;

        Names higher = connectome.LayerList (connectomeLayer);
        higher.erase (base);
        higher.erase (meta);
        if (higher.size ())
            highestLayers.erase (connectomeLayer);
    }
    
    // Make no changes if empty.
    if (highestLayers.size ())
        return ERROR::UNKNOWN;

    // clears current meta layer
    connectome.LayerRemove (meta);
    connectome.LayerAdd (meta);

    if (highestLayers.size () > 1) {
        
        for (Name highLayer : highestLayers) {
            connectome.Link (meta, highLayer, meta);
        }
        return ERROR::NONE;
    }

    Name highestLayer = *highestLayers.begin (); 
    CopyInternal (connectome, highestLayer, meta);
    return ERROR::NONE;
}

int
Clean  (Mpg&        connectome,
        Doodles&    doodles,
        string      workingdirectory)
{
    // remove empty layers
    for (Name layer : connectome.LayerList ()) {
        if (layer == meta || layer == base)
            continue;
        if (connectome.DoodleList (layer).size () == 0)
            connectome.LayerRemove (layer);
    }
    //todo: remove temporary and extra layers from manipulations
    //date imports??

    // remove untracked
    Names toPurge;
    toPurge.insert (untracked);
    Purge (connectome, doodles, toPurge, workingdirectory);

    //autosave
    return ERROR::NONE;
}

int
ExportToNoodle (const Mpg&      connectome,
                const Doodles&  doodles,
                string          workingdirectory,
                string          destinationPath,
                const Names&    toExport,
                const Names&    layermask)
{
    int result;

    // Prepare directory
    result = InitializeDirectory (destinationPath);
    if (result != ERROR::NONE) {
        return ERROR::UNKNOWN;
    }
    Doodles exportedDoodles;
    time_t temp; string version;
    LoadNodeCache (exportedDoodles, destinationPath, temp, version);

    // Create partial node-cache
    for (Name doodle : toExport) {
        Doodle data = doodles.at (doodle);
        exportedDoodles.emplace(doodle, data);
    }
    time_t now = std::chrono::system_clock::to_time_t (std::chrono::system_clock::now ());
    WriteNodeCache (doodles, destinationPath, now);

    // Create partial connectome
    Mpg exportedMpg;
    for (Name layer : layermask) {
        for (Name doodle : toExport) {
            Names adjacencyList = connectome.AdjacencyList (doodle, layer);
            for (Name adjacency : adjacencyList) {
                if (toExport.count (adjacency))
                    exportedMpg.Link (doodle, adjacency, layer);
            }
        }
    }
    WriteConnectome (exportedMpg, destinationPath);

    // Copy doodle files
    for (Name doodle : toExport) {
        if (doodles.at (doodle).abstract)
            continue;
        string newDirectory = DirectoryFromFilePath (doodles.at (doodle).path);
        fs::create_directories (destinationPath + newDirectory);
        fs::copy (
            workingdirectory + doodles.at (doodle).path,
            destinationPath + doodles.at (doodle).path);
    }

    return ERROR::NONE;
}

int
ExportToGraphml ()
{
    return ERROR::NOT_IMPLEMENTED;
}

int
Export (const Mpg&      connectome,
        const Doodles&  doodles,
        ExportType      type,
        string          workingdirectory,
        string          destinationPath,
        const Names&    toExport,
        const Names&    layermask)
{
    switch (type) {
    case ExportType::ExportTypeNoodle:
        return ExportToNoodle  (connectome,
                                doodles,
                                workingdirectory,
                                destinationPath,
                                toExport,
                                layermask);
    case ExportType::ExportTypeGraphml:
        return ExportToGraphml ();
    default:
        // bad export type
        return ERROR::BAD_PARAMETER;
    }
}

int
Filesystemauto ()
{
    //todo: use an algorythm to regenerate base and impose that in the filesystem
    return ERROR::NOT_IMPLEMENTED;
}

int
ImportFromNoodle   (Mpg&            connectome,
                    Doodles&        doodles,
                    string          fromPath,
                    const Names&    layerList,
                    Name            importName)
{
    // Check for import prefix
    //todo: specify prefix system a little better.
    //it might make sense to integrate this with nested noodles

    // Prepare subdirectory for import

    // Import

    return ERROR::NOT_IMPLEMENTED;
}

int
ImportFromGraphml  (Mpg&            connectome,
                    Doodles&        doodles,
                    string          fromPath,
                    const Names&    layerList,
                    Name            importName)
{
    return ERROR::NOT_IMPLEMENTED;
}

int
ImportFromObsidian (Mpg&            connectome,
                    Doodles&        doodles,
                    string          fromPath,
                    const Names&    layerList,
                    Name            importName)
{
    /*

    These are old notes. Look at the doc.

    We need to recursively move through the obsidian vault.
    We'll need to parse the files one at a time.


    Import Obsidian Vault
    ---------------------
    void ObsidianImport() {
        importLayer = MPG. CreateLayer("obsidianImport");

        // Load up MPG nodes
        for each file in obsidian-vault {
            DoodleNew(file.relativePath);
            // file name is basically the id.
        }
        // Parse files to get connectome
        for each file in obsidian-vault {
            for each line in file {
                if contains("[[SOME NAME]]") {
                    // this handles mentioned but uncreated nodes
                    Cursor toLink = MPG.AddIfNotExists(SOME NAME);
                    MPG.Link(MPG.FindByName(file.name), toLink, importLayer);
                }
            }
        }
    }

    An import creates it's own layer.
    You can manually subsequently merge the layers but that's outside the scope of the import.

    */

    return ERROR::NOT_IMPLEMENTED;
}

int
Import (Mpg&         connectome,
        Doodles&     doodles,
        ImportType   type,
        string       fromPath,
        const Names& layerList,
        Name         importName)
{
    switch (type) {
    case ImportType::ImportTypeNoodle:
        return ImportFromNoodle    (connectome,
                                    doodles,
                                    fromPath,
                                    layerList,
                                    importName);
    case ImportType::ImportTypeGraphml:
        return ImportFromGraphml   (connectome,
                                    doodles,
                                    fromPath,
                                    layerList,
                                    importName);
    case ImportType::ImportTypeObsidian:
        return ImportFromObsidian  (connectome,
                                    doodles,
                                    fromPath,
                                    layerList,
                                    importName);
    default:
        // bad import type
        return ERROR::BAD_PARAMETER;
    }
}

bool
DirectoryIsInitialized (string path)
{
    //todo: is there a better way for this?
    //I'd like some kind of real validation/verification
    //maybe we should check the node cache version?
    //todo: sanity check
    return fs::exists (path + "/.noodle");
}

int
InitializeDirectory (string path)
{
    int result = ERROR::NONE;

    string noodledirectory = path + "/.noodle/";

    if (DirectoryIsInitialized (path)) {
        perror ("Directory already initialized.");
        return ERROR::UNKNOWN;
    }

    // Create hidden .noodle directory
    fs::create_directory (noodledirectory.c_str ());
    fs::create_directory ((noodledirectory + "/connectome").c_str ());
    fs::create_directory ((noodledirectory + "/geometer").c_str ());

    // Node Cache
    Doodles doodles;
    doodles[origin] = {
        0, 
        true,   // abstract
        false,  // meta
        false,  // visited,
        "/.noodle/origin",
        origin
    };
    doodles[base]   = {
        0,
        true,   // abstract
        true,   // meta
        false,  // visited,
        "/.noodle/base",
        base
    };
    doodles[meta]   = {
        0, 
        true,   // abstract
        true,   // meta
        false,  // visited,
        "/.noodle/meta",
        meta
    };

    //todo: generate base layer?

    time_t now = std::chrono::system_clock::to_time_t (std::chrono::system_clock::now ());
    result = WriteNodeCache (doodles, path, now);

    return result;
}

//int Layer ();
//int Quit ();

void
Refresher::MovedUntrackedModified  (const Doodles&  doodles,
                                    string          workingdirectory,
                                    time_t          timeOfLastRefresh)
{
    for (const auto& dirEntry : fs::recursive_directory_iterator (workingdirectory)) {
        string path = ConvertToRelativePath (workingdirectory, dirEntry.path ());
        string fileName = FileNameFromPath (path);
        //todo: skip hidden directories e.g.: .noodle/
        if (IsHidden (workingdirectory, dirEntry.path ())
        || fs::is_directory (dirEntry.path ())) {
            continue;
        }
        auto inCache = doodles.find (fileName);
        if (inCache == doodles.end ()) {
            // the file is untracked
            untracked.emplace (fileName, path);
            continue;
        }
        if (inCache->second.path != path) {
            // the doodle file is moved or duplicate
            moved.emplace (fileName, path);
            continue;
        }
        struct stat result;
        if (stat (dirEntry.path ().c_str (), &result) != 0)
            return;
        time_t modificationTime = result.st_mtime;
        if (modificationTime < timeOfLastRefresh)
            continue;
        // the doodle file is modified
        modified.emplace (fileName);
    }
}

void
Refresher::Missing (const Doodles&  doodles,
                    string          workingdirectory,
                    time_t          timeOfLastRefresh)
{
    Names  missingOrMoved;
    for (auto namedDoodle : doodles) {
        if (namedDoodle.second.abstract == false
        && fs::exists (workingdirectory + namedDoodle.second.path) == false) {
            missingOrMoved.emplace (namedDoodle.first);
        }
    }
    missing = missingOrMoved;
    // Doodle is missing if it wasn't moved.
    //todo: maybe we can use a cleaner syntax
    for (auto movedDoodle : moved) {
        missing.erase (movedDoodle.first);
    }
}

void
Refresher::DetermineDoodleLists    (const Doodles&  doodles,
                                    string          workingdirectory,
                                    time_t          timeOfLastRefresh)
{
    MovedUntrackedModified (doodles,
                            workingdirectory,
                            timeOfLastRefresh);
    Missing    (doodles,
                workingdirectory,
                timeOfLastRefresh);
}

int
Refresh    (Mpg&        connectome,
            Doodles&    doodles,
            string      workingdirectory,
            time_t      timeOfLastRefresh,
            time_t&     newRefreshTime,
            Refresher&  refresher)
{
    //todo: handle conflicting filenames.
    //we already turn abstract doodles into regular ones if we find a file for it.

    refresher.DetermineDoodleLists (doodles, workingdirectory, timeOfLastRefresh);
    
    // Handle Doodles
    for (auto m : refresher.moved) {
        doodles.at (m.first).path = m.second;
        doodles.at (m.first).abstract = false;
        if (doodles.at (m.first).abstract) {
            //todo: maybe make a list of converted ABSTRACT->nonABSTRACT doodles
            //converted.push_back(m.first); 
        }
    }
    //todo: reconsider untracked auto-layer as a feature
    if (refresher.untracked.size () > 0
    && connectome.LayerCheck (untracked) == false) {
        //todo: more error checks
        LayerNew (connectome, doodles, untracked);
    }
    for (auto u : refresher.untracked) {
        //todo: use hashed content to detect doodles which have been both moved and renamed.
        Doodle newData = {
        /*nameHash*/    0,
                        false,  // abstract
                        false,  // meta
                        false,  // visited,
        /*path*/        u.second,
        /*name*/        u.first
        };
        doodles.emplace (u.first, newData);
        connectome.Link (u.first, origin, untracked);
    }

    newRefreshTime = std::chrono::system_clock::to_time_t (std::chrono::system_clock::now ());

    // Application layer may want to do additional things with these doodles.
    refresher.HandleMissing ();
    refresher.HandleModified ();
    refresher.HandleMoved ();
    refresher.HandleUntracked ();

    return ERROR::NONE;    //todo: true result    
}

void
ParseNodeCacheHeader   (std::ifstream&  nodeCache,
                        string&         version,
                        time_t&         timeOfLastRefresh)
{
    std::string headerLine;
    std::getline (nodeCache,headerLine);
    std::stringstream headerStream (headerLine);
    getline (headerStream, version, ',');
    headerStream >> timeOfLastRefresh;
}

int
Save   (const Mpg&      connectome,
        const Doodles&  doodles,
        string          workingdirectory)
{
    int result;

    // Get the last refresh time.
    time_t timeOfLastRefresh;
    string nodeCacheVersion;
    std::ifstream nodeCacheFile;
    nodeCacheFile.open (workingdirectory + "/.noodle/");
    ParseNodeCacheHeader (nodeCacheFile, nodeCacheVersion, timeOfLastRefresh);
    nodeCacheFile.close();

    result = WriteNodeCache (doodles, workingdirectory, timeOfLastRefresh);
    if (result != ERROR::NONE) {
        return result;
    }
    result = WriteConnectome (connectome, workingdirectory);
    if (result != ERROR::NONE) {
        return result;
    }

    return ERROR::NONE;
}

//int Select ();

int
WriteNodeCache (const Doodles&  doodles,
                string          workingdirectory,
                time_t          timeOfLastRefresh)
{
    /*  Node Cache Example (tabular form):
        ----------------------------------
        20231020                        79230
        origin                          1
        base                            1
        meta                            1
        /philosophy/existentialism.md   0
        /politics/secularity.md         0
        /morality/love.md               0
    */

    //todo: copy existing cache to a temp file in case write fails

    std::ofstream nodeCache;
    nodeCache.open (workingdirectory + "/.noodle/node-cache");

    nodeCache
        << NODE_CACHE_VERSION << "," 
        << timeOfLastRefresh << "\n";

    for (auto doodle : doodles) {
        Doodle data = doodle.second;
        int state =
            data.abstract  * (0b1 << 0) + 
            data.layer     * (0b1 << 1) + 
            data.directory * (0b1 << 2);
        nodeCache
            << data.path << ","
            << state << "\n";
    }
    nodeCache.close ();

    return ERROR::NONE;
}

int
WriteConnectomeLayer   (const Mpg&  connectome,
                        string      workingdirectory,
                        Name        layer)
{
    string layerPath = workingdirectory + "/.noodle/connectome/" + layer;
    std::ofstream layerConnectome;
    layerConnectome.open (layerPath);
    for (Name node : connectome.DoodleList (layer)) {
        layerConnectome << node;
        Names adjacencyList = connectome.AdjacencyList (node, layer);
        for (Name adjacentNode : adjacencyList) {
            layerConnectome << "," << adjacentNode;
        }
        layerConnectome << "\n";
    }
    layerConnectome.close ();
    return ERROR::NONE;
}

int
WriteConnectome    (const Mpg&  connectome,
                    string      workingdirectory)
{
    int result;
    //todo: use temp files instead of deleting right away
    for (const auto & connectomeFile :
    fs::directory_iterator (workingdirectory + "/.noodle/connectome")) {
        fs::remove (connectomeFile.path ());
    }
    for (Name layer : connectome.LayerList ()) {
        result = WriteConnectomeLayer (connectome, workingdirectory, layer);
        if (result != ERROR::NONE)
            return ERROR::UNKNOWN;
    }
    return ERROR::NONE;
}

int
LoadNodeCache  (Doodles&    doodles,
                string      workingdirectory,
                time_t&     timeOfLastRefresh,
                string&     nodeCacheVersion)
{
    if (DirectoryIsInitialized (workingdirectory) == false)
        return ERROR::UNKNOWN;

    std::ifstream nodeTableFile;
    nodeTableFile.open (workingdirectory + "/.noodle/node-cache");

    ParseNodeCacheHeader (nodeTableFile, nodeCacheVersion, timeOfLastRefresh);

    Doodle toLoad = {
        /*nameHash*/    0,
                        false,
                        false,
                        false,
        /*path*/        "",
        /*name*/        ""
    };
    for (string line; std::getline (nodeTableFile,line);) {
        int state;
        std::stringstream lineStream (line);
        getline (lineStream, toLoad.path, ',');
        lineStream >> state;
        // convert state integer into boolean values
        toLoad.abstract  = state & (0b1 << 0);
        toLoad.layer     = state & (0b1 << 1);
        toLoad.directory = state & (0b1 << 2);
        toLoad.name      = FileNameFromPath (toLoad.path);
        doodles.emplace (toLoad.name, toLoad);
    }
    
    nodeTableFile.close ();
    return ERROR::NONE;
}

int
LoadConnectomeFileLine (Mpg&    connectome,
                        Name    layerName,
                        string  line)
{
    int result;
    std::stringstream lineStream (line);
    Name doodleName;
    Name firstNameInLine;
    while (getline(lineStream, doodleName, ',')) {
        if (firstNameInLine.empty ()) {
            firstNameInLine = doodleName;
            continue;
        } 
        result = connectome.LinkSingly (firstNameInLine, doodleName, layerName);
        if (result != ERROR::NONE)
            return result;
    }
    return ERROR::NONE;
}

int
LoadConnectomeLayer    (Mpg&    connectome,
                        string  layerFilePath)
{
    int result;

    // Get layer name
    // warning: this doesn't work if layerFilePath has '/' on the end
    std::stringstream pathStream (layerFilePath);
    Name layerName;
    while (std::getline (pathStream, layerName, '/'));

    // Add layer
    if (connectome.LayerCheck (layerName))
        return ERROR::UNKNOWN;
    connectome.LayerAdd (layerName);

    // Parse layer file
    std::ifstream layerFile;
    layerFile.open (layerFilePath);
    std::string line;
    while (std::getline (layerFile, line)) {
        result = LoadConnectomeFileLine (connectome, layerName, line);
        if (result != ERROR::NONE)
            return result;
    }
    layerFile.close ();

    return ERROR::NONE;
}

int
LoadConnectome (Mpg&    connectome,
                string  workingdirectory)
{
    if (DirectoryIsInitialized (workingdirectory) == false)
        return ERROR::UNKNOWN;

    for (const auto & connectomeFile : 
    fs::directory_iterator (workingdirectory + "/.noodle/connectome")) {
        LoadConnectomeLayer    (connectome,
                                connectomeFile.path ());
    }

    return ERROR::NONE;
}

}   // namespace Noodle
