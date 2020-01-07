#pragma once


#define NT_DEVICE_NAME L"\\Device\\WDMHello"
#define DOS_DEVICE_NAME L"\\DosDevices\\WDMHello"
#define ALLOC_TAG 'WMn2'

#define IOCTL_MY_CTL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x4000, METHOD_BUFFERED , FILE_ANY_ACCESS)


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