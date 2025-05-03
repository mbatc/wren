#ifndef wren_ext_h
#define wren_ext_h

#include "wren.h"

// Set [classSlot] to the class of the value in [valueSlot]
WREN_API void wrenGetObjectClass(WrenVM * vm, int valueSlot, int classSlot);

// Set [superSlot] to the supertype of the class in [classSlot]
WREN_API void wrenGetClassSupertype(WrenVM * vm, int classSlot, int superSlot);

// Returns the number of entries in the map stored in [slot].
WREN_API bool wrenGetClassHasMethod(WrenVM * vm, int classSlot, char const * method);

// Find the first method associated with the class stored in [classSlot].
// Store the method in [methodSlot]
WREN_API bool wrenGetClassFirstMethod(WrenVM * vm, int classSlot, int methodSlot);

// Find the next method in the class stored in [classSlot].
// This iterates the classes method table starting from the symbol already defined in [methodSlot]
WREN_API bool wrenGetClassNextMethod(WrenVM * vm, int classSlot, int methodSlot);

// Get the name of the method stored in [methodSlot]
WREN_API char const * wrenGetMethodSymbol(WrenVM * vm, int methodSlot);

#endif
