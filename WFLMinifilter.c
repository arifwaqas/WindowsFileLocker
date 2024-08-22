#include <fltKernel.h>
#include <dontuse.h>
#include <suppress.h>

PFLT_FILTER pFltHandle = NULL;

FLT_PREOP_CALLBACK_STATUS MiniPreCreate(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionCallbacks)
{
	PFLT_FILE_NAME_INFORMATION Information;
	NTSTATUS status = STATUS_SUCCESS;

	WCHAR Name[250] = { 0 };

	status = FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &Information);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("MiniPreCreate Failed!!\r\n"));
		goto cleanup;
	}

	status = FltParseFileNameInformation(Information);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("FltParseFileNameInformation Failed!!\r\n"));
		goto cleanup;
	}

	if (Information->Name.Length < 250)
	{
		RtlCopyMemory(Name, Information->Name.Buffer, Information->Name.Length);
		KdPrint(("Preop callback success!! for FileName : %ws \r\n", Name));
	}
	else
	{
		KdPrint(("File name exceeds maximum Length!!"));
	}

	FltReleaseFileNameInformation(Information);

cleanup:
	return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

FLT_PREOP_CALLBACK_STATUS MiniPreRead(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionCallbacks)
{
	KdPrint(("MiniPreRead called!!"));

	return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

FLT_PREOP_CALLBACK_STATUS MiniPreWrite(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionCallbacks)
{
	KdPrint(("MiniPreRead called!!"));

	return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

// Callbacks list for operation registration in the minifilter
//
FLT_OPERATION_REGISTRATION Callbacks[] = {
	{IRP_MJ_CREATE, 0, MiniPreCreate, NULL},
	{IRP_MJ_READ, 0, MiniPreRead, NULL},
	{IRP_MJ_WRITE, 0, MiniPreWrite, NULL},
	{IRP_MJ_OPERATION_END}
};


// Minifilter unload function for filter registration
//
NTSTATUS MiniUnload(FLT_FILTER_UNLOAD_FLAGS Flags)
{
	KdPrint(("Driver unload called!!\r\n"));
	FltUnregisterFilter(pFltHandle);
	return STATUS_SUCCESS;
}


const FLT_REGISTRATION fltRegistration = {
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