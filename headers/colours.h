#pragma once
namespace Colours {
    
    // Reserved UI Colours
    const char C_GREY78     [] = "\033[38;5;251m";
    // Layer Colours
    const char NO_FORMAT    [] = "\033[0m";
    const char C_DEEPPINK2  [] = "\033[38;5;197m";
    const char C_PALEGREEN1 [] = "\033[38;5;121m";
    const char C_MEDIUMSPRINGGREEN [] = "\033[38;5;49m";
    const char C_DARKORANGE [] = "\033[38;5;208m";
    const char C_YELLOW1    [] = "\033[38;5;226m";
    const char C_PURPLE     [] = "\033[38;5;129m";
    const char C_MEDIUMORCHID1 [] = "\033[38;5;207m";
    const char C_DEEPSKYBLUE1 [] = "\033[38;5;39m";

    const unsigned short NUM_COLOURS = 7;
    const char* List [] = {
        NO_FORMAT,

        C_DEEPPINK2,
        C_PALEGREEN1,
        C_DEEPSKYBLUE1,
        C_DARKORANGE,
        C_YELLOW1,
    
        C_PURPLE,
        C_MEDIUMORCHID1,
    };
}
