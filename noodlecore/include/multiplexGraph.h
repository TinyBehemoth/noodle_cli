#pragma once
#include "includes.h"

namespace Noodle
{

/*  Graph
    -----
    A simple undirected graph represented with adjacency lists.    
*/
class Graph
{
public:

    int     DoodleAdd      (Name toAdd);
    int     DoodleRemove   (Name toRemove);
    int     Link           (Name toLinkFrom,
                            Name toLinkTo);
    int     Unlink         (Name toUnlinkA,
                            Name toUnlinkB);
    int     Unlink         (Name toUnlink);

    bool    DoodleCheck    (Name doodle)    const;
    Names   DoodleList     ()               const;
    Names   AdjacencyList  (Name doodle)    const;

private:

    unordered_map<Name,Names> adjacencyLists;
};

/*  Multiplex Graph
    ---------------
    This is used to represent noodle's connectome.
    (How the doodles are connected)
    It's a structure of layers where each layer is a graph.

    This class implements basic mechanics and a bit of noodle related logic.
    No user error checks. No doodle data.
*/
class Mpg
{
public:

// Manipulations

    int LayerAdd       (Name newLayer);
    int LayerRemove    (Name layerToRemove);
    int	DoodleRemove   (Name doodleToRemove);
    
    int Link       (Name toLinkA,
                    Name toLinkB,
                    Name layer);
    int LinkSingly (Name toLinkFrom,
                    Name toLinkTo,
                    Name layer);
    int Unlink     (Name toUnlinkA,
                    Name toUnlinkB,
                    Name layer);
    int Unlink     (Name toUnlink,
                    Name layer);
    
// Information

    bool    DoodleCheck   (Name doodle) const;
    bool    DoodleCheck   (Name doodle,
                           Name layer)  const;
    bool    LayerCheck    (Name layer)  const;

    Names   LayerList     ()            const;
    Names   LayerList     (Name doodle) const;
    Names   DoodleList    ()            const;
    Names   DoodleList    (Name layer)  const;
    Names   AdjacencyList (Name doodle,
                           Name layer)  const;
    unordered_map<Name, Names> AdjacencyMultiList (Name doodle) const;

private:

    unordered_map<Name,Graph> layers;
};

}   // namespace Noodle
