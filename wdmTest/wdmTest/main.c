
#include <wdm.h>

#include "main.h"

NTSTATUS DriverEntry(
	PDRIVER_OBJECT pDriverObject,
	PUNICODE_STRING pRegstryPath
)
{
	UNREFERENCED_PARAMETER(pDriverObject);
	UNREFERENCED_PARAMETER(pRegstryPath);
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	UNICODE_STRING deviceNTName;
	UNICODE_STRING deviceDOSName;
	PDEVICE_OBJECT pDeviceObject = NULL;
	RtlInitUnicodeString(&deviceNTName, NT_DEVICE_NAME);
	RtlInitUnicodeString(&deviceDOSName, DOS_DEVICE_NAME);
	
	//create Device Object 
	status = IoCreateDevice(
		pDriverObject, 
		0, 
		&deviceNTName, 
		FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN, 
		0, 
		&pDeviceObject);


	if (!NT_SUCCESS(status))
	{

		KdPrint(("DriverEntry fail to IoCreateDevice.. <==\n"));
		goto Exit;

	}
	KdPrint(("DriverEntry success to IoCreateDevice.. <==\n"));
	//Create Symblic Link
	status = IoCreateSymbolicLink(&deviceDOSName, &deviceNTName);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("DriverEntry fail to IoCreateSymbolicLink.. <==\n"));
		if (pDeviceObject)
		{

			IoDeleteDevice(pDeviceObject);
			pDeviceObject = NULL;
			KdPrint(("DriverEntry IoDeleteDevice.. <==\n"));
		}
		goto Exit;
	}
	KdPrint(("DriverEntry success to IoCreateSymbolicLink.. <==\n"));

	// Set Buffered I/O
	pDeviceObject->Flags|= DO_BUFFERED_IO;

	pDriverObject->DriverUnload = DriverUnload;
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = CreateDispatch;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = CloseDispatch;
	pDriverObject->MajorFunction[IRP_MJ_READ] = IoControlDispatch;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = IoControlDispatch;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoControlDispatch;

	KdPrint(("DriverEntry success<==\n"));

Exit:



	return status;
}
void DriverUnload(
	PDRIVER_OBJECT pDriverObject
)
{
	UNICODE_STRING deviceDOSName;
	UNREFERENCED_PARAMETER(pDriverObject);
	KdPrint(("DriverUnload <==\n"));
	RtlInitUnicodeString(&deviceDOSName, DOS_DEVICE_NAME);
	IoDeleteSymbolicLink(&deviceDOSName);
	KdPrint(("DriverUnload IoDeleteSymbolicLink <==\n"));

	if (pDriverObject->DeviceObject)
	{
		IoDeleteDevice(pDriverObject->DeviceObject);
		KdPrint(("DriverUnload IoDeleteDevice <==\n"));
	}

	return;
}


NTSTATUS
IoControlDispatch(
	PDEVICE_OBJECT pDeviceObject,
	PIRP pIrp
)
{
	UNREFERENCED_PARAMETER(pDeviceObject);
	UNREFERENCED_PARAMETER(pIrp);

	PWCHAR buf = NULL;
	PIO_STACK_LOCATION  irpSp;
	pIrp->IoStatus.Information = 0;
	irpSp = IoGetCurrentIrpStackLocation(pIrp);
	
	/*
		How to read buffer from user application on IRP_MJ_WRITE?  
		BUFFERED I/O
	*/
	if (irpSp->MajorFunction == IRP_MJ_WRITE)
	{	
		if (irpSp->Parameters.Write.Length)
		{
			buf = ExAllocatePoolWithTag(NonPagedPool, irpSp->Parameters.Write.Length, ALLOC_TAG);
			if (buf)
			{
				//copy buffer
				RtlCopyMemory(buf, pIrp->AssociatedIrp.SystemBuffer, irpSp->Parameters.Write.Length);
				//print
				KdPrint(("IoControlDispatch IRP_MJ_WRITE MajorFunction:%x Length:%lu %S<==\n", irpSp->MajorFunction, irpSp->Parameters.Write.Length, buf));
				ExFreePool(buf);
			}
			//return written bytes
			pIrp->IoStatus.Information = irpSp->Parameters.Write.Length;
		}
	}

	KdPrint(("IoControlDispatch  %x <==\n", irpSp->MajorFunction));

	pIrp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

NTSTATUS
CreateDispatch(
	PDEVICE_OBJECT pDeviceObject,
	PIRP pIrp
)
{
	UNREFERENCED_PARAMETER(pDeviceObject);
	UNREFERENCED_PARAMETER(pIrp);
	KdPrint(("CreateDispatch <==\n"));
	PIO_STACK_LOCATION  irpSp;

	irpSp = IoGetCurrentIrpStackLocation(pIrp);

	if (irpSp->MajorFunction == IRP_MJ_CREATE)
	{
		KdPrint(("CreateDispatch IRP_MJ_CREATE <==\n"));
	}

	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

NTSTATUS
CloseDispatch(
	PDEVICE_OBJECT pDeviceObject,
	PIRP pIrp
)
{
	UNREFERENCED_PARAMETER(pDeviceObject);
	UNREFERENCED_PARAMETER(pIrp);
	KdPrint(("CloseDispatch <==\n"));
	PIO_STACK_LOCATION  irpSp;// Pointer to current stack location

	irpSp = IoGetCurrentIrpStackLocation(pIrp);

	if (irpSp->MajorFunction == IRP_MJ_CLOSE)
	{
		KdPrint(("CloseDispatch IRP_MJ_CLOSE <==\n"));
	}

	
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	

	return STATUS_SUCCESS;
}