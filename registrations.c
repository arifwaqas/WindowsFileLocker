#include "wflminifilter.h"

// Callbacks list for operation registration in the minifilter
//
FLT_OPERATION_REGISTRATION Callbacks[] = {
	{IRP_MJ_CREATE, 0, MiniPreCreate, NULL},
	{IRP_MJ_READ, 0, MiniPreRead, NULL},
	{IRP_MJ_WRITE, 0, MiniPreWrite, NULL},
	{IRP_MJ_OPERATION_END}
};

FLT_REGISTRATION fltRegistration = {
	sizeof(FLT_REGISTRATION),
	FLT_REGISTRATION_VERSION,
	0,
	NULL,
	Callbacks,
	MiniUnload,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};