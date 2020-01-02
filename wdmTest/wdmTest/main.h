#pragma once


#define NT_DEVICE_NAME L"\\Device\\WDMHello"
#define DOS_DEVICE_NAME L"\\DosDevices\\WDMHello"


/*
	Driver Entry
*/
NTSTATUS DriverEntry(
	PDRIVER_OBJECT pDriverObject,
	PUNICODE_STRING pRegstryPath
);

void DriverUnload(
	PDRIVER_OBJECT pDriverObject
);

NTSTATUS
CreateDispatch(
	PDEVICE_OBJECT pDeviceObject,
	PIRP pIrp
);
NTSTATUS
CloseDispatch(
	PDEVICE_OBJECT pDeviceObject,
	PIRP pIrp
);
NTSTATUS
IoControlDispatch(
	PDEVICE_OBJECT pDeviceObject,
	PIRP pIrp
);