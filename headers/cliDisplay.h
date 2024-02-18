#pragma once

#include "cliSession.h"

using namespace Noodle;

/*  A layermask UI element.
 *  Note: All inputs are assumed to be good. There is no error checking here.*/
struct UiLayermask
{
public:
    UiLayermask ();
private:
    unordered_map<Name,unsigned short>  layerColours;
    unordered_set<unsigned short>       unusedColours;
public:
    void ToggleLayer (Name layer);
    void ToggleLayers (Names layers);
    unordered_map<Name,unsigned short>::iterator RemoveLayer (Name layer);
    void Print ();
    unsigned short ColourIdFromLayer (Name layer);
    void Update (Names layermask);
    void Display (CliSession& session);
};

struct DisplayDoodle
{
    // used for display traversals
    bool visited = false;
    //todo: try to remove this
    bool selected = false;
    // for building the tree view
    bool aboveRoot = false;
    Name belowBranch;
    unsigned short displayOffset = -1;
    // for colours and perhaps other things
    Name layerTrace;
};

/*  CliDisplay
    ----------    
    This structure contains information and functionality for rendering a basic
    view of noodle in the cli.
*/
struct CliDisplay
{
    CliDisplay (CliSession& session);

    bool displayOnNextInteractiveCommand = false;
    // his will have to be covered by the Geometer interface.
    // The resulting sequence in the hashmap::iterator is the implicit geometer.
    unordered_map<Name,DisplayDoodle> displayed;

    UiLayermask layermask;

    string prompt;

    const unsigned short NUMBER_OF_VIEWS = 2;
    enum View { FLAT, SIMPLE_TREE };
    View currentView;

    //todo: move colour stuff to a more sensible place.
    bool coloursActive = true;
    void ColourPrint (string toPrint);

    //todo: implement UiMuxGraphTree
    bool rotateForward = true;
    Name rotationIndex;
    unsigned int maxDisplayOffset;
    void Rotate                    ();
    
    //todo: implement geometer architecture in the core
    void GeometerDisplay           (CliSession&         session);
    
    //todo: move this
    void DisplayDoodlesOnLayer     (const Mpg*          connectome,
                                    Name                layer);
    void Display                   (Names               toDisplay);

    void Hide                      (Names               toHide);
    void Clear                     ();


    Names Displayed                () const;
    bool  IsDisplayed              (Name                doodle) const;

    int   ToggleView               ();
    int   ToggleRotationDirection  (CliSession&         session);

private:

    // Simple Tree Display Mode
    void DisplaySimpleTree         (const CliSession*   session);
    void BuildSimpleTreeRecursive  (const CliSession*   session,
                                    vector<string>*     current,
                                    vector<string>*     next,
                                    unsigned int        depth);
    void UpdatePrintStates         ();
    void ResetPrintStates          ();
    void RotateForward             ();
    void RotateBack                ();

    // display utilities
    void UpdateDisplayData         (CliSession&         session);
    
    // Flat List Display Mode
    void DisplayFlatView           (const CliSession*   session) const;

    //todo: move these to the DisplayDoodle class
    bool IsSelected                (const CliSession*   session,
                                    Name                doodleToCheck) const;
    bool IsVisited                 (Name                doodle) const;
};
