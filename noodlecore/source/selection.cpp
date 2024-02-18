#include "noodlecore.h"
#include "returnType.h"

namespace Noodle
{

//   Selection
//   ---------
//int All ();
//int Clear ();
//int Expand ();
int
SelectByLayer  (const Mpg*  connectome,
                Name        layerToSelect,
                Names&      resultingSelection)
{
    resultingSelection = connectome->DoodleList(layerToSelect);
    return ERROR::NONE;
}

}   // namespace Noodle