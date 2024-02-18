#include "multiplexGraph.h"
#include "returnType.h"
//#include "noodle.h" // Defines ERROR enum for return statements.

namespace Noodle
{

// Manipulations

int
Mpg::LayerAdd (Name newLayer)
{
    layers.emplace (newLayer, Graph ());
    return ERROR::NONE;
}

int
Mpg::LayerRemove (Name layerToRemove)
{
    layers.erase (layerToRemove);
    return ERROR::NONE;
}

int
Mpg::DoodleRemove (Name doodleToRemove)
{
    for (auto namedGraph = layers.begin ();
        namedGraph != layers.end ();)
    {
        if (namedGraph->first == doodleToRemove) {
            namedGraph = layers.erase (namedGraph);
            continue;
        }
        namedGraph->second.DoodleRemove (doodleToRemove);
        namedGraph++;
    }
    return ERROR::NONE;
}

int
Mpg::Link  (Name toLinkA,
            Name toLinkB,
            Name layer)
{
    if (!layers.at (layer).DoodleCheck (toLinkA))
        layers.at (layer).DoodleAdd (toLinkA);
    if (!layers.at (layer).DoodleCheck (toLinkB))
        layers.at (layer).DoodleAdd (toLinkB);
    layers.at (layer).Link (toLinkA, toLinkB);
    layers.at (layer).Link (toLinkB, toLinkA);
    return ERROR::NONE;
}

int
Mpg::LinkSingly    (Name toLinkFrom,
                    Name toLinkTo,
                    Name layer)
{
    if (!layers.at (layer).DoodleCheck (toLinkFrom))
        layers.at (layer).DoodleAdd (toLinkFrom);
    if (!layers.at (layer).DoodleCheck (toLinkTo))
        layers.at (layer).DoodleAdd (toLinkTo);
    return layers.at (layer).Link (toLinkFrom, toLinkTo);
}

int
Mpg::Unlink    (Name toUnlinkA,
                Name toUnlinkB,
                Name layer)
{
    auto namedGraph = layers.find (layer);
    if (namedGraph == layers.end ())
        return ERROR::LAYER_NOT_FOUND;
    return namedGraph->second.Unlink (toUnlinkA, toUnlinkB);
}

int
Mpg::Unlink    (Name toUnlink,
                Name layer)
{
    return layers.at (layer).Unlink (toUnlink);
}

// Information

bool
Mpg::DoodleCheck (Name doodle) const
{
    return DoodleList ().count (doodle);
}

bool
Mpg::DoodleCheck (Name doodle, Name layer) const
{
    return DoodleList (layer).count (doodle);
}

bool
Mpg::LayerCheck (Name layer) const
{
    return LayerList ().count (layer);
}

Names
Mpg::LayerList () const
{
    Names layerList;
    for (auto layer : layers) {
        layerList.emplace (layer.first);
    }
    return layerList;
}

Names
Mpg::LayerList (Name doodle) const
{
    Names layerList;
    for (auto layer : layers) {
        if (layer.second.DoodleCheck (doodle))
            layerList.emplace (layer.first);
    }
    return layerList;
}

Names
Mpg::DoodleList () const
{
    Names doodleList;
    for (auto layer : layers) {
        Names doodlesInLayer = layer.second.DoodleList ();
        doodleList.insert (doodlesInLayer.begin (), doodlesInLayer.end ());
    }
    return doodleList;
}

Names
Mpg::DoodleList (Name layer) const
{
    return layers.at (layer).DoodleList ();
}

Names
Mpg::AdjacencyList (Name doodle,
                    Name layer) const
{
    auto muxLayer = layers.find (layer);
    if (muxLayer == layers.end ())
        return Names();
    return muxLayer->second.AdjacencyList (doodle);
}

unordered_map<Name, Names>
Mpg::AdjacencyMultiList (Name doodle) const
{
    unordered_map<Name, Names> result;
    for (auto& namedLayer : layers) {
        result.emplace (namedLayer.first,
                        AdjacencyList (doodle, namedLayer.first));
    }
    return result;
}

int
Graph::DoodleAdd (Name toAdd)
{
    adjacencyLists.emplace (toAdd, Names());
    return ERROR::NONE;
}

int
Graph::DoodleRemove (Name toRemove)
{
    adjacencyLists.erase (toRemove);
    Unlink (toRemove);
    return ERROR::NONE;
}

int
Graph::Link    (Name toLinkFrom,
                Name toLinkTo)
{
    adjacencyLists.at (toLinkFrom).emplace (toLinkTo);
    return ERROR::NONE;
}

int
Graph::Unlink  (Name toUnlinkA,
                Name toUnlinkB)
{
    adjacencyLists.at (toUnlinkA).erase (toUnlinkB);
    adjacencyLists.at (toUnlinkB).erase (toUnlinkA);
    if (adjacencyLists.at (toUnlinkA).size () == 0)
        adjacencyLists.erase (toUnlinkA);
    if (adjacencyLists.at (toUnlinkB).size () == 0)
        adjacencyLists.erase (toUnlinkB);
    return ERROR::NONE;
}

int
Graph::Unlink (Name toUnlink)
{
    for (auto node : adjacencyLists) {
        node.second.erase (toUnlink);
    }
    return ERROR::NONE;
}

bool
Graph::DoodleCheck (Name doodle) const
{
    return adjacencyLists.count (doodle);
}

Names
Graph::DoodleList () const
{
    Names doodleList;
    for (auto node : adjacencyLists) {
        doodleList.emplace (node.first);
    }
    return doodleList;
}

Names
Graph::AdjacencyList   (Name doodle) const
{
    if (DoodleCheck (doodle) == false)
        return Names();
    return adjacencyLists.at (doodle);
}

}   // namespace Noodle
