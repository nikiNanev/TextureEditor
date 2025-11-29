#include "Storage.h"

#include "Modules/Textures/Loader.h"

typedef struct command
{
    void undo(Loader *loader)
    {
        // Undo check
        if (current >= 0 && current <= vtextures.size())
        {

        }
    }

    void redo(Loader *loader)
    {
        if (current >= 0 && current <= vtextures.size())
        {
            
        }
    }
};
