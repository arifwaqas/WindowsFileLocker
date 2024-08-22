#include "wflminifilter.h"

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