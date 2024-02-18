#include "cliSession.h"

CliSession::CliSession (string workingdirectory) : Session (workingdirectory)
{

}

void
CliSession::CleanLayerMask ()
{
    Names layerList = connectome.LayerList ();
    for (auto l = layermask.begin (); l != layermask.end ();) {
        if (layerList.count (*l) == 0) {
            l = layermask.erase (l);
        } else {
            l++;
        }
    }
}

Names
CliSession::GetDoodlesFromStream ()
{
    Noodle::Names fromStream;
    string nextArgument;
    while (getline (userInputStream, nextArgument, ' ')){
        if (doodles.count (nextArgument))
            fromStream.emplace (nextArgument);
    }
    fromStream.erase ("");
    return fromStream;
}

int
CliSession::GetDoodleFromInput (Name& doodle)
{
    if (!getline (userInputStream, doodle, ' '))
        return ERROR::UNKNOWN;
    if (doodles.count (doodle) == 0)
        return ERROR::DOODLE_NOT_FOUND;
    return ERROR::NONE;
}
