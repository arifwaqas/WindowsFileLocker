#include "wfl.h"

#define MAX_PATH 1024

// Routine helpers
static NTSTATUS GetParsedFileNameInfo(
	PFLT_CALLBACK_DATA pData,
	WCHAR* parentDir
)
{
	NTSTATUS status = STATUS_SUCCESS;
	WCHAR dir[MAX_PATH] = { 0 };
	PFLT_FILE_NAME_INFORMATION pFileNameInfo = NULL;

	status = FltGetFileNameInformation(pData, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &pFileNameInfo);
	if (NT_SUCCESS(status))
	{
		status = FltParseFileNameInformation(pFileNameInfo);
		if (NT_SUCCESS(status))
		{
			if (pFileNameInfo->Name.Length < MAX_PATH && pFileNameInfo->Name.Length >= 0)
			{
				RtlCopyMemory(dir, pFileNameInfo->Name.Buffer, pFileNameInfo->Name.Length);
				KdPrint(("dir : % ws \r\n", dir));
			}
			else
			{
				KdPrint(("File name exceeds maximum Length!!\n"));
			}

			// Get parent dir
			if (pFileNameInfo->ParentDir.Length < MAX_PATH && pFileNameInfo->ParentDir.Length >= 0)
			{
				RtlCopyMemory(parentDir, pFileNameInfo->ParentDir.Buffer, pFileNameInfo->ParentDir.Length);
				KdPrint(("parentDir : %ws \r\n", parentDir));
			}
			else {
				KdPrint(("Parent dir exceeds maximum Length!!\n"));
			}
		}
		else
		{
			KdPrint(("FltParseFileNameInformation Failed!!\r\n"));
		}
	}
	else
	{
		KdPrint(("MiniPreCreate Failed!!\r\n"));
	}


	if (pFileNameInfo) {
		FltReleaseFileNameInformation(pFileNameInfo);
	}

	return status;
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

	WCHAR parentDir[MAX_PATH] = { 0 };

	NTSTATUS status = GetParsedFileNameInfo(pData, parentDir);

	KdPrint(("Object Access allowed!! for parent: %ws\n", parentDir));

OK:
	return FLT_PREOP_SUCCESS_NO_CALLBACK;
}



FLT_PREOP_CALLBACK_STATUS MiniPreRead(PFLT_CALLBACK_DATA pData, PCFLT_RELATED_OBJECTS pFltObjects, PVOID* ppCompletionCallbacks)
{
	KdPrint(("MiniPreRead called!!"));

	UNREFERENCED_PARAMETER(ppCompletionCallbacks);
	UNREFERENCED_PARAMETER(pFltObjects);

	WCHAR parentDir[MAX_PATH] = { 0 };

	NTSTATUS status = GetParsedFileNameInfo(pData, parentDir);

	KdPrint(("Object Access allowed!!"));

OK:
	return FLT_PREOP_SUCCESS_NO_CALLBACK;
}



FLT_PREOP_CALLBACK_STATUS MiniPreWrite(PFLT_CALLBACK_DATA pData, PCFLT_RELATED_OBJECTS pFltObjects, PVOID* ppCompletionCallbacks)
{
	KdPrint(("MiniPreWrite called!!"));

	UNREFERENCED_PARAMETER(ppCompletionCallbacks);
	UNREFERENCED_PARAMETER(pFltObjects);
// TODO : Implement
	KdPrint(("Object Access allowed!!"));

OK:
	return FLT_PREOP_SUCCESS_NO_CALLBACK;

//FAIL:
//	pData->IoStatus.Status = STATUS_ACCESS_DENIED;
//	return FLT_PREOP_COMPLETE;
}