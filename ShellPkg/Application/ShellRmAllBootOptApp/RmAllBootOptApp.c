/** @file
  This is a simple shell application

  Copyright (c) 2008 - 2010, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>

/**
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS            Status;
  UINTN                 Length = 0;
  UINT16                Count = 0;
  UINT16                *Buffer;
  CHAR16                VariableName[12];

  Buffer = NULL;

  Print(L"\nDelete all entries in boot option list\n");

  Buffer = AllocateZeroPool(Length+(4*sizeof(UINT16)));
  Status = gRT->GetVariable(
    (CHAR16*)L"BootOrder",
    (EFI_GUID*)&gEfiGlobalVariableGuid,
    NULL,
    &Length,
    Buffer);

  Print(L"Status: %r\n", Status);
  if (Status == EFI_BUFFER_TOO_SMALL) {
    Buffer = AllocateZeroPool(Length+(4*sizeof(UINT16)));
    if (Buffer != NULL) {
      Status = gRT->GetVariable(
        (CHAR16*)L"BootOrder",
        (EFI_GUID*)&gEfiGlobalVariableGuid,
        NULL,
        &Length,
        Buffer);
      Print(L"Status: %r\n", Status);
    } else {
      return EFI_OUT_OF_RESOURCES;
    }
  }

  Count = (UINT16) (Length / sizeof(Buffer[0]));

  Print(L"Count: %d\n", Count);

  for (INT8 Index = Count; Index >= 0; Index--) {
     Print(L"Remove boot option: %d\n", Index);
     UnicodeSPrint(VariableName, sizeof(VariableName), L"%s%04x", L"Boot", Index);
     Status = gRT->SetVariable(
       VariableName,
       (EFI_GUID*)&gEfiGlobalVariableGuid,
       EFI_VARIABLE_NON_VOLATILE|EFI_VARIABLE_BOOTSERVICE_ACCESS|EFI_VARIABLE_RUNTIME_ACCESS,
       0,
       NULL);
   }

  FreePool(Buffer);
  return EFI_SUCCESS;
}
