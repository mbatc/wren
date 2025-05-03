#include "wren_ext.h"
#include "wren_vm.h"

// Ensures that [slot] is a valid index into the API's stack of slots.
static void validateApiSlot(WrenVM* vm, int slot)
{
  ASSERT(slot >= 0, "Slot cannot be negative.");
  ASSERT(slot < wrenGetSlotCount(vm), "Not that many slots.");
}

void wrenGetObjectClass(WrenVM* vm, int valueSlot, int classSlot)
{
  validateApiSlot(vm, valueSlot);
  validateApiSlot(vm, classSlot);

  ASSERT(IS_OBJ(vm->apiStack[valueSlot]), "Slot must hold an object");

  Obj * obj = AS_OBJ(vm->apiStack[valueSlot]);
  vm->apiStack[classSlot] = OBJ_VAL(obj->classObj);
}

void wrenGetClassSupertype(WrenVM* vm, int classSlot, int superSlot)
{
  validateApiSlot(vm, classSlot);
  validateApiSlot(vm, superSlot);
  
  ASSERT(IS_CLASS(vm->apiStack[classSlot]), "Slot must hold an class");

  ObjClass * obj = AS_CLASS(vm->apiStack[classSlot]);
  if (obj->superclass == NULL)
    wrenSetSlotNull(vm, superSlot);
  else
    vm->apiStack[classSlot] = OBJ_VAL(obj->superclass);
}

bool wrenGetClassHasMethod(WrenVM* vm, int classSlot, char const * methodName)
{
  validateApiSlot(vm, classSlot);

  ASSERT(IS_CLASS(vm->apiStack[classSlot]), "Slot must hold a class.");

  int symbol = wrenSymbolTableFind(&vm->methodNames, methodName, strlen(methodName));
  if (symbol == -1)
    return false;

  ObjClass * cls = AS_CLASS(vm->apiStack[classSlot]);

  Method method = cls->methods.data[symbol];
  return method.type != METHOD_PRIMITIVE && method.type != METHOD_NONE;
}

bool wrenGetClassFirstMethod(WrenVM* vm, int classSlot, int methodSlot)
{
  validateApiSlot(vm, classSlot);
  validateApiSlot(vm, methodSlot);

  wrenSetSlotDouble(vm, methodSlot, -1.0);

  return wrenGetClassNextMethod(vm, classSlot, methodSlot);
}

bool wrenGetClassNextMethod(WrenVM* vm, int classSlot, int methodSlot)
{
  validateApiSlot(vm, classSlot);
  validateApiSlot(vm, methodSlot);

  ASSERT(IS_CLASS(vm->apiStack[classSlot]), "Slot must hold a class.");
  ASSERT(IS_NUM(vm->apiStack[methodSlot]), "Slot must hold a number.");
  
  ObjClass * cls = AS_CLASS(vm->apiStack[classSlot]);
  int        i   = (int)AS_NUM(vm->apiStack[methodSlot]) + 1;

  for (; i < cls->methods.count; ++i)
  {
    Method *method = &cls->methods.data[i];
    if (method->type == METHOD_PRIMITIVE || method->type == METHOD_NONE)
      continue;

    wrenSetSlotDouble(vm, methodSlot, (double)i);
    return true;
  }

  wrenSetSlotDouble(vm, methodSlot, (double)-1);
  return false;
}

char const * wrenGetMethodSymbol(WrenVM * vm, int symbolSlot) {
  validateApiSlot(vm, symbolSlot);

  ASSERT(IS_NUM(vm->apiStack[symbolSlot]), "Slot must hold a number.");
  int i = (int)AS_NUM(vm->apiStack[symbolSlot]);
  ASSERT(i >= 0 && i < vm->methodNames.count, "Invalid symbol index in Slot");

  return vm->methodNames.data[i]->value;
}
