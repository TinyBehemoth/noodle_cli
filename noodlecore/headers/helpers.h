#pragma once
#include "includes.h"
#include "doodle.h"
#include "multiplexGraph.h"

namespace Noodle
{

/*  Returns true on doodle creation. False otherwise */
bool    CreateIfDoesntExist    (Doodles&        doodles,
                                Name            doodle);

Name    CalculateCenter        (const Mpg&      connectome,
                                const Names&    doodles,
                                const Names&    layermask);

/*  Calculates a doodle to link to on the meta layer.
    This is an autobehaviour.
*/
//todo: maybe we deprecate this becuase of how we're doing meta now
Name    CalculateMetaLink      (Mpg&            connectome,
                                Name            metaDoodle);

Name    CalculateBaseLink      (Mpg&            connectome,
                                Name            doodle);
void    BaseFromFilesystem     (Mpg&            connectome,
                                Doodles&        doodles,
                                string          workingdirectory);


int     HandleUnlinked         (Mpg&            connectome,
                                Name            toUnlink,
                                Name            layer);

void    CopyInternal           (Mpg&            connectome,
                                Name            from,
                                Name            to);
void    CopyInternalNewSelf    (Mpg&            connectome,
                                Name            from,
                                Name            to);

void    CopyExternal           (Mpg&            connectome,
                                Name            from,
                                Name            to);


string  FileNameFromPath       (string          path);

string  ConvertToRelativePath  (string          workingdirectory,
                                string          fullPath);

string  DirectoryFromFilePath  (string          path);

bool    IsHidden               (string          workingdirectory,
                                string          path);

}   // namespace Noodle
