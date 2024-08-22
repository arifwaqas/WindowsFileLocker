#include "wflminifilter.h"

PFLT_FILTER pFltHandle = NULL;


// Minifilter unload function for minifilter
//
NTSTATUS MiniUnload(FLT_FILTER_UNLOAD_FLAGS Flags)
{
	KdPrint(("Driver unload called!!\r\n"));
	FltUnregisterFilter(pFltHandle);
	return STATUS_SUCCESS;
}



NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING RegistryPath)
{
	NTSTATUS status = STATUS_SUCCESS;

	status = FltRegisterFilter(pDriverObj, &fltRegistration, &pFltHandle);

	if (!NT_SUCCESS(status)) {
		KdPrint(("FltRegisterFilter Failed!!"));
		goto cleanup;
	}


	status = FltStartFiltering(pFltHandle);
	if (!NT_SUCCESS(status)) {
		FltUnregisterFilter(pFltHandle);
		KdPrint(("FltUnregisterFilter Failed!!"));
		goto cleanup;
	}

cleanup:
	return status;

}