#pragma once
#include <fltKernel.h>
#include <dontuse.h>
#include <suppress.h>

NTSTATUS MiniUnload(FLT_FILTER_UNLOAD_FLAGS Flags);

// defined in registrations.c
extern FLT_REGISTRATION fltRegistration; 
extern FLT_OPERATION_REGISTRATION Callbacks[];


//defined in routines.c
FLT_PREOP_CALLBACK_STATUS MiniPreCreate(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionCallbacks);

FLT_PREOP_CALLBACK_STATUS MiniPreRead(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionCallbacks);

FLT_PREOP_CALLBACK_STATUS MiniPreWrite(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionCallbacks);