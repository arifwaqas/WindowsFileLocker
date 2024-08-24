#include "wflminifilter.h"

#define MAX_PATH 1024

// Routine helpers
static NTSTATUS GetParsedFileNameInfo(
	PFLT_CALLBACK_DATA pData,
	PFLT_FILE_NAME_INFORMATION pFileNameInfo
)
{
	NTSTATUS status = STATUS_SUCCESS;

	status = FltGetFileNameInformation(pData, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &pFileNameInfo);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("MiniPreCreate Failed!!\r\n"));
		goto FAIL;
	}

	status = FltParseFileNameInformation(pFileNameInfo);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("FltParseFileNameInformation Failed!!\r\n"));
		goto FAIL;
	}

	goto OK;

FAIL:
	if (pFileNameInfo != NULL) {
		FltReleaseFileNameInformation(pFileNameInfo);
	}
OK:
	return status;
}

BOOL GetAccessVerdict(
	PFLT_CALLBACK_DATA pData
) 
{
	PFLT_FILE_NAME_INFORMATION pFileNameInfo = NULL;

	KdPrint(("Preop callback success!!"));

	NTSTATUS status = GetParsedFileNameInfo(pData, pFileNameInfo);

	if (!NT_SUCCESS(status) || pFileNameInfo == NULL) {
		KdPrint(("GetParsedFileNameInfo failed or pFileInfo NULL!!"));
		goto FAIL;
	}

	WCHAR dir[MAX_PATH] = { 0 };
	WCHAR parentDir[MAX_PATH] = { 0 };

	if (pFileNameInfo->Name.Length < MAX_PATH && pFileNameInfo->Name.Length >= 0)
	{
		RtlCopyMemory(dir, pFileNameInfo->Name.Buffer, pFileNameInfo->Name.Length);
		KdPrint(("dir : % ws \r\n", dir));

		if (pFileNameInfo->ParentDir.Length < MAX_PATH && pFileNameInfo->ParentDir.Length >= 0)
		{
			RtlCopyMemory(parentDir, pFileNameInfo->ParentDir.Buffer, pFileNameInfo->ParentDir.Length);
			KdPrint(("parentDir : %ws \r\n", parentDir));
		}
		else {
			KdPrint(("Parent dir exceeds maximum Length!!"));
		}
	}
	else
	{
		KdPrint(("File name exceeds maximum Length!!"));
	}

	FltReleaseFileNameInformation(pFileNameInfo);

	return 1;

FAIL:
	return 0;
}


// Actual routine definitions

FLT_PREOP_CALLBACK_STATUS MiniPreCreate(
	PFLT_CALLBACK_DATA pData, 
	PCFLT_RELATED_OBJECTS pFltObjects, 
	PVOID* ppCompletionCallbacks
)
{
	KdPrint(("MiniPreCreate called!!"));

	UNREFERENCED_PARAMETER(ppCompletionCallbacks);
	UNREFERENCED_PARAMETER(pFltObjects);

	if (GetAccessVerdict(pData) == 0) {
		KdPrint(("Object Access denied!!"));
		goto FAIL;
	}

	KdPrint(("Object Access allowed!!"));

OK:
	return FLT_PREOP_SUCCESS_NO_CALLBACK;

FAIL:
	pData->IoStatus.Status = STATUS_ACCESS_DENIED;
	return FLT_PREOP_COMPLETE;
}



FLT_PREOP_CALLBACK_STATUS MiniPreRead(PFLT_CALLBACK_DATA pData, PCFLT_RELATED_OBJECTS pFltObjects, PVOID* ppCompletionCallbacks)
{
	KdPrint(("MiniPreRead called!!"));

	UNREFERENCED_PARAMETER(ppCompletionCallbacks);
	UNREFERENCED_PARAMETER(pFltObjects);

	if (GetAccessVerdict(pData) == 0) {
		KdPrint(("Object Access denied!!"));
		goto FAIL;
	}

	KdPrint(("Object Access allowed!!"));

OK:
	return FLT_PREOP_SUCCESS_NO_CALLBACK;

FAIL:
	pData->IoStatus.Status = STATUS_ACCESS_DENIED;
	return FLT_PREOP_COMPLETE;
}



FLT_PREOP_CALLBACK_STATUS MiniPreWrite(PFLT_CALLBACK_DATA pData, PCFLT_RELATED_OBJECTS pFltObjects, PVOID* ppCompletionCallbacks)
{
	KdPrint(("MiniPreWrite called!!"));

	UNREFERENCED_PARAMETER(ppCompletionCallbacks);
	UNREFERENCED_PARAMETER(pFltObjects);

	if (GetAccessVerdict(pData) == 0) {
		KdPrint(("Object Access denied!!"));
		goto FAIL;
	}

	KdPrint(("Object Access allowed!!"));

OK:
	return FLT_PREOP_SUCCESS_NO_CALLBACK;

FAIL:
	pData->IoStatus.Status = STATUS_ACCESS_DENIED;
	return FLT_PREOP_COMPLETE;
}