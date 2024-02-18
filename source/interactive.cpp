#include "interactive.h"

#include "basic.h"
#include "navigation.h"
#include "manipulations.h"

#include "utilities.h"

#include "layer.h"
#include "selection.h"

#include "definitions.h"

CliDisplay* CLI_DISPLAY;

const char* INTERACTIVE_COMMANDS[] = {

    // navigation
    
    "dive",         "explore",      "focus",
    "grow",         "hide",         "jump",
    "shrink",       "switch",       "subsplore",
    "supersplore",  "toggleview",   "togglerotation",
    
    // basic
    
    "delete",       "link",         "new",
    "rename",       "unlink",
    
    // manipulations
    
    "chain",        "clone",        "sprout",
    "subchain",     "superchain",   "phase",
    "project",

    "flatten",      "merge",        "pluck",
    "swapfile",     "swaplinks",    "swapname",
    "transfer",

    "dissolve",     "isolate",      "purge",
    "remove",

    // utilities

    "baseauto",     "clean",        "export",
    "fsauto",       "import",       "init",
    "layer",        "quit",         "refresh",
    "save",         "select",       "help",

    // extra

    "text",         "folder",       "display",
};

const char* SELECT_SUBCOMMANDS[] = {

};


/* Generator function for command completion.  STATE lets us know whether
   to start from scratch; without any state (i.e. STATE == 0), then we
   start at the top of the list. */
char*
CommandGenerator   (const char* text, 
                    int         state)
{
    static int list_index, len;
    const char *name;

    /* If this is a new word to complete, initialize now.  This includes
        saving the length of TEXT for efficiency, and initializing the index
        variable to 0. */
    if (!state)
    {
        list_index = 0;
        len = strlen (text);
    }

    /* Return the next name which partially matches from the command list. */
    while (name = INTERACTIVE_COMMANDS[list_index])
    {
        list_index++;
        if (list_index >= InteractiveCommand::INVALID) {
            break;
        }
        if (strncmp (name, text, len) == 0) {
            char *r = (char*) malloc (strlen (name) + 1);
            strcpy (r, name);
            return (r);
        }
    }

    /* If no names matched, then return NULL. */
    return ((char *)NULL);
}

char*
DoodleGenerator    (const char* text,
                    int         state)
{
    static int len;
    static unordered_map<string,DisplayDoodle>::iterator nextMatch;
    const char *name;

    /*  If this is a new word to complete, initialize now.  This includes
        saving the length of TEXT for efficiency, and initializing the index
        variable to 0. */
    if (!state)
    {
        nextMatch = CLI_DISPLAY->displayed.begin ();
        len = strlen (text);
    }

    while (++nextMatch != CLI_DISPLAY->displayed.end ()) {
        name = nextMatch->first.c_str ();
        if (strncmp (name, text, len) == 0) {
            char *r = (char*) malloc (strlen (name) + 1);
            strcpy (r, name);
            return (r);
        }
    }

    /* If no names matched, then return NULL. */
    return ((char *)NULL);
}

char*
SubcummandGenerator    (const char* text,
                        int         state)
{
    
}

char**
ReadlineAutocomplete (const char *text, int start, int end)
{
    char **matches;
    matches = (char **)NULL;
    //todo: detect subcommands
    if (start == 0) {
        matches = rl_completion_matches (text, CommandGenerator);
    } else {
        matches = rl_completion_matches (text, DoodleGenerator);
    }
    return (matches);
}

int
ReadlineInit ()
{
    //todo: what is this??
    /* Allow conditional parsing of the ~/.inputrc file. */
    rl_readline_name = "Noodle";
    /* Tell the completer that we want a crack first. */
    rl_attempted_completion_function = ReadlineAutocomplete;

    return ERROR::NONE;
}

InteractiveCommand
ResolveInteractiveCommand (string userInput)
{
    // Grouping and ordering of commands follows the documentation.

    // Navigation

    if (userInput == "dive-into")   return InteractiveCommand::DIVE;
    if (userInput == "dive-in")     return InteractiveCommand::DIVE;
    if (userInput == "into")        return InteractiveCommand::DIVE;
    if (userInput == "in")          return InteractiveCommand::DIVE;
    
    if (userInput == "collapse")    return InteractiveCommand::COLLAPSE;

    if (userInput == "expand")      return InteractiveCommand::EXPAND;

    if (userInput == "explore")     return InteractiveCommand::EXPLORE;
    if (userInput == "xpl")         return InteractiveCommand::EXPLORE;

    if (userInput == "f")           return InteractiveCommand::FOCUS;
    if (userInput == "focus")       return InteractiveCommand::FOCUS;

    if (userInput == "g")           return InteractiveCommand::GROW;
    if (userInput == "grow")        return InteractiveCommand::GROW;

    if (userInput == "h")           return InteractiveCommand::HIDE;
    if (userInput == "hide")        return InteractiveCommand::HIDE;

    if (userInput == "jump-out")    return InteractiveCommand::JUMP;
    if (userInput == "jump")        return InteractiveCommand::JUMP;
    if (userInput == "out")         return InteractiveCommand::JUMP;
    
    if (userInput == "shrink")      return InteractiveCommand::SHRINK;

    if (userInput == "sbs")         return InteractiveCommand::SUBSPLORE;
    if (userInput == "subsplore")   return InteractiveCommand::SUBSPLORE;

    if (userInput == "supersplore") return InteractiveCommand::SUPERSPLORE;
    
    if (userInput == "switch")      return InteractiveCommand::SWITCH;

    if (userInput == "r")           return InteractiveCommand::TOGGLE_ROTATION;
    if (userInput == "rotate")      return InteractiveCommand::TOGGLE_ROTATION;

    if (userInput == "v")           return InteractiveCommand::TOGGLE_VIEW;
    if (userInput == "view")        return InteractiveCommand::TOGGLE_VIEW;
    if (userInput == "toggle-view") return InteractiveCommand::TOGGLE_VIEW;

    // Basic Manipulations

    if (userInput == "del")         return InteractiveCommand::DELETE;
    if (userInput == "delete")      return InteractiveCommand::DELETE;

    if (userInput == "l")           return InteractiveCommand::LINK;
    if (userInput == "link")        return InteractiveCommand::LINK;

    if (userInput == "n")           return InteractiveCommand::NEW;
    if (userInput == "new")         return InteractiveCommand::NEW;

    if (userInput == "rn")          return InteractiveCommand::RENAME;
    if (userInput == "rename")      return InteractiveCommand::RENAME;

    if (userInput == "u")           return InteractiveCommand::UNLINK;
    if (userInput == "ul")          return InteractiveCommand::UNLINK;
    if (userInput == "unlink")      return InteractiveCommand::UNLINK;

    // Manipulations

    if (userInput == "c")           return InteractiveCommand::CHAIN;
    if (userInput == "chain")       return InteractiveCommand::CHAIN;

    if (userInput == "clone")       return InteractiveCommand::CLONE;
   
    if (userInput == "spt")         return InteractiveCommand::SPROUT;
    if (userInput == "sprt")        return InteractiveCommand::SPROUT;
    if (userInput == "sprout")      return InteractiveCommand::SPROUT;
    if (userInput == "branch")      return InteractiveCommand::SPROUT;

    if (userInput == "subchain")    return InteractiveCommand::SUBCHAIN;

    if (userInput == "subsprout")   return InteractiveCommand::SUBSPROUT;

    if (userInput == "superchain")  return InteractiveCommand::SUPERCHAIN;

    if (userInput == "phase")       return InteractiveCommand::PHASE;

    if (userInput == "cp")          return InteractiveCommand::PROJECT;
    if (userInput == "copy")        return InteractiveCommand::PROJECT;
    if (userInput == "project")     return InteractiveCommand::PROJECT;

    if (userInput == "mv")          return InteractiveCommand::FLATTEN;
    if (userInput == "move")        return InteractiveCommand::FLATTEN;
    if (userInput == "flat")        return InteractiveCommand::FLATTEN;
    if (userInput == "flatten")     return InteractiveCommand::FLATTEN;

    if (userInput == "merge")       return InteractiveCommand::MERGE;

    if (userInput == "pluck")       return InteractiveCommand::PLUCK;

    if (userInput == "swap-file")   return InteractiveCommand::SWAPFILE;

    if (userInput == "swap-links")  return InteractiveCommand::SWAPLINKS;

    if (userInput == "swap-name")   return InteractiveCommand::SWAPNAME;

    if (userInput == "transfer")    return InteractiveCommand::TRANSFER;

    if (userInput == "dissolve")    return InteractiveCommand::DISSOLVE;

    if (userInput == "isolate")     return InteractiveCommand::ISOLATE;

    if (userInput == "purge")       return InteractiveCommand::PURGE;

    if (userInput == "rm")          return InteractiveCommand::REMOVE;
    if (userInput == "remove")      return InteractiveCommand::REMOVE;

    // Utilities

    if (userInput == "clean")       return InteractiveCommand::CLEAN;

    if (userInput == "export")      return InteractiveCommand::EXPORT;

    if (userInput == "help")        return InteractiveCommand::HELP;

    if (userInput == "import")      return InteractiveCommand::IMPORT;

    if (userInput == "layer")       return InteractiveCommand::LAYER;
    if (userInput == "mask")        return InteractiveCommand::LAYER;
    if (userInput == "layermask")   return InteractiveCommand::LAYER;

    if (userInput == "quit")        return InteractiveCommand::QUIT;
    if (userInput == "q")           return InteractiveCommand::QUIT;

    if (userInput == "refresh")     return InteractiveCommand::REFRESH;

    if (userInput == "save")        return InteractiveCommand::SAVE;

    if (userInput == "s")           return InteractiveCommand::SELECT;
    if (userInput == "select")      return InteractiveCommand::SELECT;

    // Extra

    if (userInput == "t")           return InteractiveCommand::TEXT;
    if (userInput == "text")        return InteractiveCommand::TEXT;

    if (userInput == "d")           return InteractiveCommand::DISPLAY;
    if (userInput == "display")     return InteractiveCommand::DISPLAY;

    if (userInput == "folder")      return InteractiveCommand::FOLDER;

    if (userInput.empty ())         return InteractiveCommand::ROTATE;


    return InteractiveCommand::INVALID;
}

void
DoodleNewText  (CliSession& session,
                CliDisplay& display)
{
    Name        textDoodle;
    Name        layer;
    getline (session.userInputStream, textDoodle, ' ');
    if (textDoodle.empty ())
        return;
    if (session.connectome.DoodleCheck (textDoodle) == false) {
        Doodle data = {
            0, // hash
            false,  // abstract
            false,  // meta
            false,  // visited
            "/" + textDoodle, // relative to working directory and includes filename
            textDoodle, // same as filename
        };
        session.doodles.emplace (textDoodle, data);
        session.connectome.Link (textDoodle, origin, layer);

        std::ofstream newDoodleFile;
        newDoodleFile.open(session.workingdirectory + data.path);
        newDoodleFile.close();
        std::cout << textDoodle << " created\n";
    } else if (session.doodles.at (textDoodle).abstract == false) {
        std::cout << "editing " << textDoodle << "\n";
    } else {
        std::cout << "creating file " << textDoodle << "\n";
        session.doodles.at (textDoodle).abstract = false;
        session.doodles.at (textDoodle).path = "/" + textDoodle;
    }

    //todo: Use editor in config file.
    //      Only use default editor if there is none configured.
    std::string textEditorCommand = 
        "nvim \"" + session.workingdirectory + session.doodles.at (textDoodle).path + "\"";
    system(textEditorCommand.c_str());

    display.displayOnNextInteractiveCommand = true;
}

void
HandleInteractiveInput (CliSession& session, CliDisplay& display)
{
    string nextArgument;
    std::getline (session.userInputStream, nextArgument, ' ');
    switch (ResolveInteractiveCommand (nextArgument)) {
    // navigation
    case InteractiveCommand::COLLAPSE:          Collapse (session, display); break;
    case InteractiveCommand::DIVE:              Dive (session, display); break;
    case InteractiveCommand::EXPAND:            Expand (session, display); break;
    case InteractiveCommand::EXPLORE:           Explore (session, display); break;
    case InteractiveCommand::FOCUS:             Focus (session, display);  break;
    case InteractiveCommand::GROW:              Grow (session, display); break;
    case InteractiveCommand::HIDE:              Hide (session, display); break;
    case InteractiveCommand::JUMP:              Jump (session, display); break;
    case InteractiveCommand::SHRINK:            Shrink(session, display); break;
    case InteractiveCommand::SWITCH:            Switch (session, display); break;
    case InteractiveCommand::SUBSPLORE:         Subsplore(session, display); break;
    case InteractiveCommand::TOGGLE_VIEW:       display.ToggleView (); break;
    case InteractiveCommand::TOGGLE_ROTATION:   display.ToggleRotationDirection (session); break;
    // manipulations
    case InteractiveCommand::CHAIN:             Chain (session, display); break;
    case InteractiveCommand::SPROUT:            Sprout (session, display); break;
    case InteractiveCommand::PLUCK:             Pluck (session, display); break;
    case InteractiveCommand::MERGE:             Merge (session, display); break;
    case InteractiveCommand::FLATTEN:           Flatten (session, display); break;
    case InteractiveCommand::PROJECT:           Project (session, display); break;
    // basic
    case InteractiveCommand::DELETE:            Delete (session, display); break;
    case InteractiveCommand::LINK:              Link (session); break;
    case InteractiveCommand::NEW:               New (session, display); break;
    case InteractiveCommand::RENAME:            Rename (session, display); break;
    case InteractiveCommand::REMOVE:            Remove (session, display); break;
    case InteractiveCommand::UNLINK:            Unlink (session); break;
    // utilities
    case InteractiveCommand::SELECT:            Utilities::Select (session, display); break;
    case InteractiveCommand::LAYER:             Layer (session, display); break;
    case InteractiveCommand::CLEAN:             Utilities::Clean (session, display); break;
    case InteractiveCommand::REFRESH:
        display.displayOnNextInteractiveCommand = false;
        Utilities::Refresh (session);
        break;
    case InteractiveCommand::SAVE:              Utilities::Save (session, display); break;
    case InteractiveCommand::QUIT:              Utilities::Quit (session, display); break;
    case InteractiveCommand::HELP:              Utilities::Help (session, display); break;
    // misc
    case InteractiveCommand::TEXT:              DoodleNewText (session, display); break;
    case InteractiveCommand::FOLDER:            break;
    case InteractiveCommand::DISPLAY:           break;

    case InteractiveCommand::ROTATE:            display.Rotate (); break;

    default:
        //invalid command
        break;
    }
}

int
Interactive (CliSession& session)
{
    int result = ERROR::NONE;

    //todo: check if initialized

    string nodeCacheVersion;
    result = LoadNodeCache (session.doodles,
                            session.workingdirectory,
                            session.timeOfLastRefresh,
                            nodeCacheVersion);
    if (result != ERROR::NONE || nodeCacheVersion != NODE_CACHE_VERSION) {
        perror ("Error: Unable to load node cache.\n");
        return ERROR::UNKNOWN;
    }
    result = Noodle::LoadConnectome    (session.connectome,
                                        session.workingdirectory);
    if (session.result != Noodle::ERROR::NONE) {
        perror ("Error: Unable to load connectome.\n");
        return ERROR::UNKNOWN;
    }

    cout << "\n"
        "    ─  ─ ── ──── N O O D L E ──── ── ─  ─    \n"
         << "\033[38;5;251m" <<
        "     GPLv3 open source and distributable.\n"
        "   Copyright (C) Francisco Garcia-Gonzalez\n"
        "       " << PROJECT_SITE_URL << "\n\n";

    cout << "Refreshing...";
    session.result = Noodle::ERROR(Utilities::Refresh (session));
    if (session.result != Noodle::ERROR::NONE) {
        perror ("Error: Unable to refresh.\n");
        return ERROR::UNKNOWN;
    }
    cout
        << "\r             \rWorking directory refreshed.\n\n";

    CliDisplay display (session);
    CLI_DISPLAY = &display;

    ReadlineInit ();

    char* userInput, shell_prompt[256];
    rl_bind_key ('\t', rl_complete);
    while (true) {
        if (display.displayOnNextInteractiveCommand)
            display.GeometerDisplay (session);
        snprintf (shell_prompt, sizeof (shell_prompt), display.prompt.c_str ());
        userInput = readline (shell_prompt);
        string userInputString = std::string (userInput);
        if (userInputString.empty () == false)
            add_history (userInputString.c_str ());
        session.userInputStream = std::stringstream (userInputString);
        HandleInteractiveInput (session, display);
        if (userInputString == "quit" || userInputString == "q")
            break;
        free (userInput);
    }

    return EXIT_SUCCESS;
}
