#include "cliDisplay.h"
#include "utilities.h"
#include <iostream>
#include "colours.h"

void
DisplayADoodle (Name            doodle, 
                const Doodles&  doodles)
{
    Doodle data = doodles.at (doodle);
    if (data.abstract) {
        std::cout << "[" << data.name << "]";
    } else {
        std::cout << data.name;
    }
}

unsigned short
UiLayermask::ColourIdFromLayer (Name layer)
{
    auto id = layerColours.find (layer);
    if (id == layerColours.end ())
        return 0;
    return id->second;
}

UiLayermask::UiLayermask ()
{
    for (unsigned short i = 1; i < Colours::NUM_COLOURS; i++) {
         unusedColours.insert (i);
    }
}

//todo: fix this
/*  If layer is removed, return iterator to next layermask item. */
unordered_map<Name,unsigned short>::iterator
UiLayermask::RemoveLayer (Name layer) {
    unusedColours.insert (layerColours.at (layer));
    return layerColours.erase (layerColours.find (layer));
}

void
UiLayermask::ToggleLayer (Name layer)
{
    auto layermaskItem = layerColours.find (layer);
    if (layermaskItem == layerColours.end ()) {
        // add the layer to the UI layermask
        if (unusedColours.size () == 0)
            layerColours.emplace (layer, 0);
        unsigned short newColour = unusedColours.size () ? *unusedColours.begin () : 0;
        unusedColours.erase (newColour);
        layerColours.emplace (layer, newColour);
    } else {
        RemoveLayer (layer);
    }
}

void
UiLayermask::ToggleLayers (Names layers)
{
    for (Name layer : layers) {
        ToggleLayer (layer);
    }
}

void
UiLayermask::Update (Names layermask)
{
    for (auto item = layerColours.begin (); item != layerColours.end ();) {
        if (layermask.count (item->first) == 0)
            item = RemoveLayer (item->first);
        else
            item++;
    }
    for (Name layer : layermask) {
        if (layerColours.count (layer) == 0)
            ToggleLayer (layer);
    }
}

void
UiLayermask::Display (CliSession& session)
{
    // print
    for (auto& item : layerColours) {
        if (item.first == session.currentLayer) {
            std::cout << "**" << Colours::List[item.second];
            DisplayADoodle (item.first, session.doodles);
            std::cout << Colours::NO_FORMAT << "** "; 
        } else {
            std::cout << Colours::List[item.second];
            DisplayADoodle (item.first, session.doodles);
            std::cout << Colours::NO_FORMAT << " ";
        }
    }
}

CliDisplay::CliDisplay (CliSession& session)
{
    prompt = string(Colours::C_GREY78) + "<noodle>" + string(Colours::NO_FORMAT);
    Names initialLayer = session.connectome.LayerList ();
    Names onDisplay;
    if ( initialLayer.size () <= 5) {
        // generate default
        LayerNew (session.connectome, session.doodles, "default");
        initialLayer.emplace ("default");
        session.currentLayer = "default";
    } else {
        initialLayer.clear ();
        session.currentLayer = "meta";
    }
    initialLayer.emplace ("meta");
    Noodle::Switch (session.connectome,
                    session.doodles,
                    onDisplay,
                    session.layermask,
                    initialLayer);
    Clear ();
    Display (onDisplay);
    currentView = View::FLAT;
    displayOnNextInteractiveCommand = true;
}

bool
CliDisplay::IsVisited (Name doodle) const
{
    return displayed.at (doodle).visited;
}

Names
CliDisplay::Displayed () const
{
    Names result;
    for (auto namedDisplayDoodles : displayed) {
        result.emplace (namedDisplayDoodles.first);
    }
    return result;
}

bool
CliDisplay::IsDisplayed (Name doodle) const
{
    return displayed.count (doodle);
}

void
CliDisplay::Hide (Names toHide)
{
    for (Name doodle : toHide) {
        displayed.erase (doodle);
    }
    displayOnNextInteractiveCommand = true;
}

void
CliDisplay::Clear ()
{
    displayed.clear ();
}

bool
CliDisplay::IsSelected (const CliSession*   session,
                        Name                doodleToCheck) const
{
    return session->selection.count (doodleToCheck);
}


void
CliDisplay::Display    (Names           toDisplay)
{
    for (Name doodle : toDisplay) {
        displayed.emplace (doodle, DisplayDoodle ());
    }
    displayOnNextInteractiveCommand = true;
}

void
CliDisplay::DisplayFlatView (const CliSession* session) const
{
    for (auto doodle : displayed) {
        std::cout << "   ";
        DisplayADoodle (doodle.first, session->doodles);
        if (IsSelected (session, doodle.first))
            std::cout << " * ";
        std::cout << "\n";
    }
}

void
CliDisplay::UpdateDisplayData (CliSession& session)
{
    // reset
    for (auto namedDisplayDoodle = displayed.begin ();
        namedDisplayDoodle != displayed.end ();) {
        
        if (session.doodles.find (namedDisplayDoodle->first) == session.doodles.end ()) {
            namedDisplayDoodle = displayed.erase (namedDisplayDoodle);
        } else {
            namedDisplayDoodle->second.selected = false;
            namedDisplayDoodle++;
        }
    }
    // update selection
    Display (session.selection);
    for (Name selected : session.selection) {
        displayed. at (selected).selected = true;
    }
}

void
CliDisplay::ColourPrint (string toPrint)
{
    if (coloursActive)
        std::cout << Colours::C_GREY78;
    std::cout << toPrint;
    if (coloursActive)
        std::cout << Colours::NO_FORMAT;
}

//string
//Colourize (string toColourize)
//{
//    if (coloursActive)
//}

void
CliDisplay::GeometerDisplay (CliSession& session)
{
    ColourPrint (session.workingdirectory + "\n");

    UpdateDisplayData (session);

    //todo: display disconnected doodles in displayed list
    switch(currentView) {
        case View::SIMPLE_TREE:
            DisplaySimpleTree (&session);
            break;
        case View::FLAT:
            DisplayFlatView (&session);
            displayOnNextInteractiveCommand = false;
            break;
        default:
            std::cout << "Invalid noodle view.\n";
            break;
    }
    // layermask
    ColourPrint ("layermask: ");
    session.CleanLayerMask ();
    layermask.Update (session.layermask);
    // ensure we always have a current layer
    if (session.layermask.count (session.currentLayer) == 0)
        session.currentLayer = * session.layermask.begin ();
    layermask.Display (session);
    if (session.connectome.LayerList ().size () > session.layermask.size ()) {
        std::cout << "...";
    }
    std::cout << "\n";
}

void
CliDisplay::BuildSimpleTreeRecursive   (const CliSession* session,
                                        vector<string>* current,
                                        vector<string>* next,
                                        unsigned int    depth)
{
    if (current->size () == 0) {
        return;
    }
    next->clear ();
    for (auto c : *current) {
        auto cAsDisplayed = displayed.find (c);
        if (cAsDisplayed->second.visited) {
            continue;
        }
        for (auto l : session->layermask) {
            for (auto a : session->connectome.AdjacencyList (c, l)) {
                if (IsDisplayed (a) == false)
                    continue;
                next->push_back (a);
                if (displayed.at (a).layerTrace.empty ())
                    displayed.at (a).layerTrace = l;
            }
        }
        cAsDisplayed->second.visited = true;
        cAsDisplayed->second.displayOffset = depth;
    }
    maxDisplayOffset = depth;
    BuildSimpleTreeRecursive (session, next, current, depth + 1);
}

void
CliDisplay::UpdatePrintStates ()
{

    bool aboveRoot = true;
    Name belowBranch = "";
    Name lowestBranch = "";
    
    for (auto& treeItem : displayed) { 
        // handles islands
        if (treeItem.first != rotationIndex && treeItem.second.displayOffset == 0) {
            treeItem.second.displayOffset = maxDisplayOffset + 2;
            treeItem.second.layerTrace = "";
        }

        // pivot on root
        if (treeItem.first == rotationIndex) {
            aboveRoot = false;
            lowestBranch = treeItem.first;
        }
        treeItem.second.aboveRoot = aboveRoot;
        
        // set belowBranch above root
        if (aboveRoot && belowBranch != "")
            treeItem.second.belowBranch = belowBranch;
        if (aboveRoot && treeItem.second.displayOffset == 1) {
            // create new branch from above the root
            belowBranch = lowestBranch = displayed.at (treeItem.first).layerTrace;
        }
        // set belowBranch below root
        if (aboveRoot == false && treeItem.second.displayOffset == 1) {
            // create new branch from below the root
            for (auto bb = ++(displayed.find (lowestBranch));
                 bb != displayed.find (treeItem.first) && bb != displayed.end ();
                 bb++) {
                    bb->second.belowBranch = displayed.at (treeItem.first).layerTrace;
            }
            lowestBranch = treeItem.first;
        }
    }
}

void
CliDisplay::ResetPrintStates ()
{
    for (auto& d : displayed) {
        d.second.aboveRoot = true;
        d.second.belowBranch = "";
        d.second.layerTrace = "";
    }
}

void
CliDisplay::DisplaySimpleTree (const CliSession* session)
{
    if (displayed.size () == 0) {
        return;
    }
    if (rotationIndex.empty () || !IsDisplayed (rotationIndex)) {
        rotationIndex = displayed.begin ()->first;
    }
    // unvisit displayed.
    for (auto& d : displayed) {
        d.second.visited = false;
        d.second.displayOffset = 0;
    }
    // spread from rotationIndex
    vector<string> buffer1, buffer2;
    buffer1.clear (); buffer2.clear ();
    buffer1.push_back (rotationIndex);
    ResetPrintStates ();
    BuildSimpleTreeRecursive (session, &buffer1, &buffer2, 0);
    UpdatePrintStates ();
    for (auto& d : displayed) {
        // Print TreeItem
        // todo: add colour
        unsigned short colour = layermask.ColourIdFromLayer (d.second.layerTrace);
        if (d.second.displayOffset == 0) {
            // root
            std::cout << " ⦿ ";
        } else if (d.second.displayOffset == 1) {
            std::cout << Colours::List[colour]; 
            // draw branch
            if (d.second.aboveRoot)
                std::cout << " ╭──○ ";
            else
                std::cout << " ╰──○ ";
            std::cout << Colours::NO_FORMAT;
        } else {
            // non branches
            if (d.second.belowBranch != "") {
                unsigned short branchColour = layermask.ColourIdFromLayer (d.second.belowBranch);
                std::cout << Colours::List[branchColour] << " │ " << Colours::NO_FORMAT;
            } else {
                std::cout << "   ";
            }
            for (int i = 0; i < d.second.displayOffset - 1; i++) {
                std::cout << "   ";
            }
            std::cout << Colours::List[colour] << " ○ " << Colours::NO_FORMAT;
        }
        
        DisplayADoodle (d.first, session->doodles);
        if (IsSelected (session, d.first)) {
            std::cout << " * ";
        }
        std::cout << "\n";
    }
}

void
CliDisplay::Rotate() {
    if (displayed.size () == 0)
        return;
    if (rotateForward) {
        RotateForward();
    } else {
        RotateBack();
    }
}

void
CliDisplay::RotateForward() {
    if (currentView != View::SIMPLE_TREE) {
        return;
    }
    auto d = displayed.find(rotationIndex);
    d++;
    if (d == displayed.end()) {
        d = displayed.begin();
    }
    rotationIndex = d->first;
}

void
CliDisplay::RotateBack() {
    if (currentView != View::SIMPLE_TREE) {
        return;
    }
    auto d=displayed.begin();
    if (d==displayed.end()) {
        return;
    }
    auto dd=d;
    ++dd;
    while (dd!=displayed.end() && dd->first!=rotationIndex) {
        ++d;
        ++dd;
    }
    rotationIndex = d->first;
}

int
CliDisplay::ToggleView ()
{
    currentView = CliDisplay::View ((int(currentView)+1) % NUMBER_OF_VIEWS);
    displayOnNextInteractiveCommand = 
        currentView == CliDisplay::View::SIMPLE_TREE
        || currentView == CliDisplay::View::FLAT;
    return ERROR::NONE;
}

int
CliDisplay::ToggleRotationDirection (CliSession& session)
{
    // Technically spec compliant.
    // This implements an extra feature where you can
    // directly rotate to a specified doodle.
    string doodleName;
    if (!getline (session.userInputStream, doodleName, ' ')) {
        rotateForward = !rotateForward;
        Rotate();
    } else if (IsDisplayed(doodleName)) {
        rotationIndex = doodleName;
    }
    return ERROR::NONE;
}
