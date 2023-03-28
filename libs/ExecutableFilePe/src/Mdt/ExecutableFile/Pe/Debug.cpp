// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "Debug.h"
#include <QLatin1String>

namespace Mdt{ namespace ExecutableFile{ namespace Pe{

QString qStringFromBool(bool b)
{
  if(b){
    return QLatin1String("true");
  }

  return QLatin1String("false");
}

QString toDebugString(const DosHeader & header)
{
  QString str = QLatin1String("PE signature offset: 0x") + QString::number(header.peSignatureOffset, 16);
  str += QLatin1String(" (") + QString::number(header.peSignatureOffset) + QLatin1String(")");

  return str;
}

QString toDebugString(MachineType type)
{
  switch(type){
    case MachineType::Unknown:
      return QLatin1String("Unknown (the content of this field is assumed to be applicable to any machine type)");
    case MachineType::Amd64:
      return QLatin1String("AMD64 (x64, x86-64)");
    case MachineType::I386:
      return QLatin1String("I386 (x86)");
    case MachineType::NotHandled:
      return QLatin1String("NotHandled");
    case MachineType::Null:
      return QLatin1String("Null");
  }

  return QString();
}

QString toDebugString(const CoffHeader & header)
{
  QString str = QLatin1String("Machine type: 0x") + QString::number(header.machine, 16)
                + QLatin1String(" (") + toDebugString( header.machineType() ) + QLatin1String(")");

  str += QLatin1String("\nstring table offset: 0x") + QString::number( header.coffStringTableOffset(), 16 ) ;

  str += QLatin1String("\nsize of optional header: ") + QString::number(header.sizeOfOptionalHeader)
       + QLatin1String(" (0x") + QString::number(header.sizeOfOptionalHeader, 16) + QLatin1String(")");

  str += QLatin1String("\ncharacteristics: 0x") + QString::number(header.characteristics, 16);
  str += QLatin1String("\n is valid executable image (IMAGE_FILE_EXECUTABLE_IMAGE): ") + qStringFromBool( header.isValidExecutableImage() );
  str += QLatin1String("\n is 32-bit word architecture (IMAGE_FILE_32BIT_MACHINE): ") + qStringFromBool( header.is32BitWordMachine() );
  str += QLatin1String("\n is valid a DLL (IMAGE_FILE_DLL): ") + qStringFromBool( header.isDll() );

  return str;
}

QString toDebugString(MagicType type)
{
  switch(type){
    case MagicType::Unknown:
      return QLatin1String("Unknown");
    case MagicType::Pe32:
      return QLatin1String("PE32, 32-bit executable");
    case MagicType::Pe32Plus:
      return QLatin1String("PE32+, 64-bit executable");
    case MagicType::RomImage:
      return QLatin1String("ROM image");
  }

  return QString();
}

QString toDebugString(const OptionalHeader & header)
{
  QString str = QLatin1String("magic: ") + toDebugString( header.magicType() );

  str += QLatin1String("\nnumber of RVA and sizes: ") + QString::number(header.numberOfRvaAndSizes);
  if( header.containsImportTable() ){
    const ImageDataDirectory directory = header.importTableDirectory();
    str += QLatin1String("\ncontains the import table: address: 0x") + QString::number(directory.virtualAddress, 16)
        +  QLatin1String(", size: ") + QString::number(directory.size);
  }
  if( header.containsDelayImportTable() ){
    const ImageDataDirectory directory = header.delayImportTableDirectory();
    str += QLatin1String("\ncontains the delay import table: address: 0x") + QString::number(directory.virtualAddress, 16)
        +  QLatin1String(", size: ") + QString::number(directory.size);
  }else{
    str += QLatin1String("\ndoes not contain a delay import table");
  }

  return str;
}

QString toDebugString(const SectionHeader & header)
{
  QString str = QLatin1String("section header ") + header.name;
  str += QLatin1String("\n virtual size: ") + QString::number(header.virtualSize);
  str += QLatin1String("\n virtual address: 0x") + QString::number(header.virtualAddress, 16);
  str += QLatin1String("\n file pointer to raw data: 0x") + QString::number(header.pointerToRawData, 16);
  str += QLatin1String("\n size: ") + QString::number(header.sizeOfRawData);

  return str;
}

QString toDebugString(const ImportDirectory & directory, const QString & leftPad)
{
  QString str = leftPad + QLatin1String("DLL name RVA: 0x") + QString::number(directory.nameRVA, 16);

  return str;
}

QString toDebugString(const ImportDirectoryTable & directoryTable)
{
  QString str = QLatin1String("import directory table:");

  for(const auto & directory : directoryTable){
    str += QLatin1String("\n") + toDebugString(directory, QLatin1String("  "));
  }

  return str;
}

QString toDebugString(const DelayLoadDirectory & directory, const QString & leftPad)
{
  QString str = leftPad + QLatin1String("Attributes: 0x") + QString::number(directory.attributes, 16);
  str += QLatin1Char('\n') + leftPad + QLatin1String("DLL name RVA: 0x") + QString::number(directory.nameRVA, 16);

  return str;
}

QString toDebugString(const DelayLoadTable & table)
{
  QString str = QLatin1String("delay load table:");

  for(const auto & directory : table){
    str += QLatin1String("\n") + toDebugString(directory, QLatin1String("  "));
  }

  return str;
}

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Pe{
