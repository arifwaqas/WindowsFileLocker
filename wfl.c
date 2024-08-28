#include "wfl.h"
#include "port.h"

PFLT_FILTER phFilter = NULL;
PFLT_PORT pPort = NULL;


// Minifilter unload function for minifilter
//
NTSTATUS MiniUnload(FLT_FILTER_UNLOAD_FLAGS Flags)
{
	KdPrint(("Driver unload called!!\r\n"));
	FltUnregisterFilter(phFilter);
	return STATUS_SUCCESS;
}



NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING RegistryPath)
{
	NTSTATUS status = STATUS_SUCCESS;

	status = FltRegisterFilter(pDriverObj, &fltRegistration, &phFilter);
	if (!NT_SUCCESS(status)) {
		KdPrint(("FltRegisterFilter Failed!!"));
		goto NO_FILTER_CLEANUP;
	}

	// initialize the filter port
	status = MinifltPortInitialize(phFilter);
	if (!NT_SUCCESS(status)) {
		KdPrint(("MinifltPortInitialize failed!!"));
		//goto FILTER_CLEANUP;
	}

	status = FltStartFiltering(phFilter);

FILTER_CLEANUP:
	if (!NT_SUCCESS(status)) {
		FltUnregisterFilter(phFilter);
		KdPrint(("FltUnregisterFilter Failed!!"));
	}

NO_FILTER_CLEANUP:
	return status;

}