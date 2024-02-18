/*  Noodlecore
    ----------
    This file specifies how noodle-based programs access noodle functionality.

    Formatting is based on gnome programming guidelines.
    https://developer.gnome.org/documentation/guidelines/programming/coding-style.html
*/

#pragma once
#include "includes.h"
#include "doodle.h"
#include "multiplexGraph.h"

/*  Versioning
    ----------
*/
const char NOODLECORE_VERSION[] = "2023.1.0";
const char NODE_CACHE_VERSION[] = "1.0";

namespace Noodle
{

/*  Automatic Doodles
    -----------------
*/
const char origin[] =       "origin";
const char base[] =         "base";
const char meta[] =         "meta";
//const char untracked[] =    "untracked";

/*  Session
    -------
    This structure describes a UI making use of a noodle directory.
    A UI app may choose to extend this structure or implement its own.
    But it should give you an idea of what is needed to use noodle functionality.
*/
struct Session
{
    Session (string workingdirectorypath) : 
        workingdirectory (workingdirectorypath)
    {
        return;
    }
    
    Names   selection;  // Passed to  commands.
    Names   layermask;  // Used to manipulate the displayed structure.

    Mpg     connectome; // All connections between doodles.
    Doodles doodles;    // Contains all doodles.

    //bool    readonly;           // Restricted from writing to drive?
    string  workingdirectory;   // Where are we in the system?
	time_t  timeOfLastRefresh;  // Updates on refresh.
    string  nodeCacheVersion;   // For determining compatibility
};

/*  Notes on Commands
    -----------------

    Commands take in some session information plus some additional parameters.
    They return an error code.
    ERROR::NONE is returned on success.

    Some commands are commented out because they were pasted directly from the cli
    documentation and it doesn't make sense to implement them here for one reason
    or another.

    Parameters are ordered according to:
      - Do I need the connectome?
      - Do I need doodles?
      - What am I changing?
      - What else do I need?
*/


//  Navigation
//  ----------

int Dive       (const Mpg*      connectome,
                Names&          toDiveInto,
                Names&          layermask);
int Explore    (const Mpg*      connectome,
                Names&          toExplore,
                Names&          layerList);
//int Focus ();
int Grow       (const Mpg*      connectome,
                Names&          toGrow,
                const Names*    layermask);
//int Hide ();
int Jump       (const Mpg*      connectome,
                Names&          toJumpOutOf,
                Names&          layermask);
int Shrink     (const Mpg*      connectome, 
                Names&          toShrink,
                const Names*    layermask);
int Switch     (Mpg&            connectome,
                Doodles&        doodles,
                Names&          displayed,
                Names&          layermask,
                const Names*    toSwitchTo);
int Subsplore  (const Mpg*      connectome,
                Names&          toSubsplore,
                Names&          layermask);
//int ToggleView ();
//int ToggleRotationDirection ();


//  Basic
//  -----

int Delete (Mpg&        connectome,
            Doodles&    doodles,
            Name        toDelete,
            string      workingdirectory);
int Link   (Mpg&        connectome,
            Name        toLinkA,
            Name        toLinkB,
            Name        layer);
int New    (Mpg&        connectome,
            Doodles&    doodles,
            Name        newDoodle);
int Rename (Mpg&        connectome,
            Doodles&    doodles,
            Name        original,
            Name        newName,
            string      workingdirectory);
int Unlink (Mpg&        connectome,
            Name        toUnlinkA,
            Name        toUnlinkB,
            Name        layer);


//  Manipulations
//  -------------

int Chain      (Mpg&            connectome,
                Doodles&        doodles,
                vector<Name>    listOfNames,
                Name            layer);
//int Clone ();
int Sprout     (Mpg&            connectome,
                Doodles&        doodles,
                Name            sproutRoot,
                const Names*    toSprout,
                Name            layer);
int Subchain   (Mpg&            connectome,
                Doodles&        doodles,
                const Names*    toChain);
int Subsprout  (Mpg&            connectome,
                Doodles&        doodles,
                Name            sproutRoot,
                const Names*    toSprout);
int Superchain (Mpg&            connectome,
                Doodles&        doodles,
                const Names*    toChain);
struct PhaseOptions
{
    bool fullStructure = false;
    bool internalStructureOnly = false;
    bool mergeOnCollision = false;
    
    bool noCloneLayers = false;
    bool noFile = false;
    bool stealFile = false;
};
int Phase      (Mpg&            connectome,
                Doodles&        doodles,
                const Names*    toPhase,
                PhaseOptions    options);
int Project    (Mpg&            connectome,
                Doodles&        doodles,
                const Names*    toProject,
                const Names*    sourceLayers,
                Name            targetLayer);
int Flatten    (Mpg&            connectome,
                Doodles&        doodles,
                const Names*    toProject,
                const Names*    sourceLayers,
                Name            targetLayer);
int Merge      (Mpg&            connectome,
                Doodles&        doodles,
                const Names*    toMerge,
                Name            result,
                string          workingdirectory);
int Pluck      (Mpg&            connectome,
                Name            toPluck,
                Name            target,
                const Names*    layermask);
int Swapfile   (Doodles&        doodles,
                Name            toSwapA,
                Name            toSwapB);
int Swaplinks  (Mpg&            connectome,
                Name            toSwapA,
                Name            toSwapB);
int Swapname   (Mpg&            connectome,
                Doodles&        doodles,
                Name            toSwapA,
                Name            toSwapB);
int Transfer   (Mpg&            connectome,
                Doodles&        doodles,
                const Names*    toTransfer,
                const Names*    layermask);

int Dissolve   (Mpg&            connectome,
                Doodles&        doodles,
                const Names*    toDissolve);
int Isolate    (Mpg&            connectome,
                const Names*    toIsolate,
                const Names*    layermask);
int Purge      (Mpg&            connectome,
                Doodles&        doodles,
                const Names*    toPurge,
                string          workingdirectory);
int Remove     (Mpg&            connectome,
                Doodles&        doodles,
                const Names*    toRemove,
                const Names*    layermask);     //todo: consider edge case
//int Clean ();


//  Layer
//  -----
int LayerAll   (const Mpg*      connectome,
                Names&          listOfAllLayers);
int LayerAuto  (const Mpg*      connectome,
                const Names*    doodlesToCover,
                Names&          resultingLayerList);
//int Current ();
//int Focus ();
int LayerFull  (const Mpg*      connectome,
                const Names*    displayed,
                Names&          layermask);
//int Hide ();
int LayerNew   (Mpg&            connectome,
                Doodles&        doodles,
                Name            newLayer);
//int Dissolve ();


//  Selection
//  ---------
//int All ();
//int Clear ();
//int Expand ();
int SelectByLayer  (const Mpg*  connectome,
                    Name        layerToSelect,
                    Names&      resultingSelection);

//  Auto
//  ----

int Baseauto                   (Mpg&            connectome,
                                const Doodles*  doodles);
int Filesystemauto             (Mpg&            connectome);
int Metaauto                   (Mpg&            connectome,
                                const Doodles*  doodles);

//  Utilities
//  ---------

int Clean                      (Mpg&            connectome,
                                Doodles&        doodles,
                                string          workingdirectory);
enum ExportType
{
    ExportTypeNoodle,
    ExportTypeGraphml,
    ExportTypeInvalid = -1
};
int Export                     (const Mpg*      connectome,
                                const Doodles*  doodles,
                                ExportType      type,
                                string          workingdirectory,
                                string          destinationPath,
                                const Names*    toExport,
                                const Names*    layermask);
enum ImportType
{
    ImportTypeNoodle,
    ImportTypeGraphml,
    ImportTypeObsidian,
    ImportTypeInvalid = -1
};
int Import                     (Mpg&            connectome,
                                Doodles&        doodles,
                                ImportType      type,
                                string          fromPath,
                                const Names*    layerList,
                                Name            importName);
int InitializeDirectory        (string          path);
//int Layer (); 
//int Quit ();
struct Refresher
{
    unordered_map<string,string> moved;
    unordered_map<string,string> untracked;
    Names missing;
    Names modified;
    
    // For the application layer to implement:
    virtual void HandleMissing      () = 0;
    virtual void HandleModified     () = 0;
    virtual void HandleMoved        () = 0;
    virtual void HandleUntracked    () = 0;

    void DetermineDoodleLists      (const Doodles*  doodles,
                                    string          workindirectory,
                                    time_t          timeOfLastRefresh);
private:
    void MovedUntrackedModified    (const Doodles*  doodles,
                                    string          workingdirectory,
                                    time_t          timeOfLastRefresh);
    void Missing                   (const Doodles*  doodles,
                                    string          workingdirectory,
                                    time_t          timeOfLastRefresh);
};

int Refresh                    (Mpg&            connectome,
                                Doodles&        doodles,
                                string          workingdirectory,
                                time_t      timeOfLastRefresh,
                                time_t&         newRefreshTime,
                                Refresher&      resultHandlers);
int Save                       (const Mpg*      connectome,
                                const Doodles*  doodles,
                                string          workingdirectory);
//int Select ();
int WriteNodeCache             (const Doodles*  doodles,
                                string          workingdirectory,
                                time_t          timeOfLastRefresh);
int WriteConnectome            (const Mpg*      connectome,
                                string          workingdirectory);
int LoadNodeCache              (Doodles&        doodles,
                                string          workingdirectory,
                                time_t&         timeOfLastRefresh,
                                string&         nodeCacheVersion);
int LoadConnectome             (Mpg&            connectome,
                                string          workingdirectory);

}   // namespace Noodle
