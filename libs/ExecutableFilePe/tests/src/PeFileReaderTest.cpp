// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "catch2/catch.hpp"
#include "Catch2QString.h"
#include "Mdt/ExecutableFile/Pe/FileReader.h"

using namespace Mdt::ExecutableFile;

ByteArraySpan arraySpanFromArray(unsigned char * const array, qint64 size)
{
  assert(size > 0);

  ByteArraySpan span;
  span.data = array;
  span.size = size;

  return span;
}

Pe::CoffHeader makeValid64BitDllCoffHeader()
{
  Pe::CoffHeader header;

  header.machine = 0x8664;
  header.characteristics = 0x200e;
  header.sizeOfOptionalHeader = 112;

  return header;
}

Pe::CoffHeader makeValid32BitDllCoffHeader()
{
  Pe::CoffHeader header;

  header.machine = 0x14c;
  header.characteristics = 0x230e;
  header.sizeOfOptionalHeader = 112;

  return header;
}


TEST_CASE("DosHeader")
{
  using Pe::DosHeader;

  SECTION("default constructed")
  {
    DosHeader header;
    REQUIRE( header.isNull() );
    REQUIRE( !header.seemsValid() );
  }
}

TEST_CASE("extractDosHeader")
{
  using Pe::extractDosHeader;
  using Pe::DosHeader;

  uchar map[64] = {};
  DosHeader header;

  SECTION("somewhat valid DOS header")
  {
    map[0] = 'M';
    map[1] = 'Z';
    map[0x3C] = 125;
    map[0x3D] = 0;
    map[0x3E] = 0;
    map[0x3F] = 0;

    ByteArraySpan mapSpan = arraySpanFromArray( map, sizeof(map) );
    header = extractDosHeader(mapSpan);
    REQUIRE( header.peSignatureOffset == 125 );
    REQUIRE( header.seemsValid() );
  }
}

TEST_CASE("containsPeSignature")
{
  using Pe::containsPeSignature;
  using Pe::extractDosHeader;
  using Pe::DosHeader;

  DosHeader dosHeader;
  uchar map[200] = {};
  map[0x3C] = 100;  // PE signature offset

  SECTION("with PE signature")
  {
    map[100] = 'P';
    map[101] = 'E';
    map[102] = 0;
    map[103] = 0;

    ByteArraySpan mapSpan = arraySpanFromArray( map, sizeof(map) );
    dosHeader = extractDosHeader(mapSpan);
    REQUIRE( containsPeSignature(mapSpan, dosHeader) );
  }

  SECTION("no PE signature")
  {
    map[100] = 1;
    map[101] = 2;
    map[102] = 3;
    map[103] = 4;

    ByteArraySpan mapSpan = arraySpanFromArray( map, sizeof(map) );
    dosHeader = extractDosHeader(mapSpan);
    REQUIRE( !containsPeSignature(mapSpan, dosHeader) );
  }

  SECTION("wrong PE signature")
  {
    map[100] = 'P';
    map[101] = 'E';
    map[102] = 0;
    map[103] = 1;

    ByteArraySpan mapSpan = arraySpanFromArray( map, sizeof(map) );
    dosHeader = extractDosHeader(mapSpan);
    REQUIRE( !containsPeSignature(mapSpan, dosHeader) );
  }
}

TEST_CASE("CoffHeader")
{
  using Pe::CoffHeader;
  using Pe::MachineType;

  CoffHeader header;

  SECTION("default constructed")
  {
    REQUIRE( header.machineType() == MachineType::Null );
    REQUIRE( !header.seemsValid() );
  }

  SECTION("Machine")
  {
    SECTION("Unknown")
    {
      header.machine = 0x0;
      REQUIRE( header.machineType() == MachineType::Unknown );
    }

    SECTION("AMD64 (x86-64)")
    {
      header.machine = 0x8664;
      REQUIRE( header.machineType() == MachineType::Amd64 );
    }

    SECTION("I386")
    {
      header.machine = 0x14c;
      REQUIRE( header.machineType() == MachineType::I386 );
    }
  }

  SECTION("Characteristics")
  {
    SECTION("Valid 32 bit DLL (release build example)")
    {
      header.characteristics = 0x230e;
      REQUIRE( header.isValidExecutableImage() );
      REQUIRE( header.is32BitWordMachine() );
      REQUIRE( header.isDll() );
      REQUIRE( header.isDebugStripped() );
    }

    SECTION("Valid 32 bit DLL (debug build example)")
    {
      header.characteristics = 0x2106;
      REQUIRE( header.isValidExecutableImage() );
      REQUIRE( header.is32BitWordMachine() );
      REQUIRE( header.isDll() );
      REQUIRE( !header.isDebugStripped() );
    }

    SECTION("Invalid image")
    {
      header.characteristics = 0x2301;
      REQUIRE( !header.isValidExecutableImage() );
    }
  }
}

TEST_CASE("CoffHeader_seemsValid")
{
  using Pe::CoffHeader;
  using Pe::MachineType;

  CoffHeader header;

  SECTION("default constructed")
  {
    REQUIRE( !header.seemsValid() );
  }

  SECTION("Valid 32-bit DLL")
  {
    header.machine = 0x14c;
    header.characteristics = 0x230e;
    header.sizeOfOptionalHeader = 112;
    REQUIRE( header.seemsValid() );
  }

  SECTION("Invalid")
  {
    header = makeValid32BitDllCoffHeader();
    REQUIRE( header.seemsValid() );

    SECTION("Unknown machine")
    {
      header.machine = 0;
      REQUIRE( !header.seemsValid() );
    }

    SECTION("Not a valid executable image")
    {
      header.characteristics = 0;
      REQUIRE( !header.seemsValid() );
    }

    SECTION("SizeOfOptionalHeader not set")
    {
      header.sizeOfOptionalHeader = 0;
      REQUIRE( !header.seemsValid() );
    }

    SECTION("SizeOfOptionalHeader to small")
    {
      header.sizeOfOptionalHeader = 50;
      REQUIRE( !header.seemsValid() );
    }
  }
}

TEST_CASE("coffHeaderFromArray")
{
  using Pe::coffHeaderFromArray;
  using Pe::CoffHeader;

  CoffHeader header;

  SECTION("x86_64 (little endian array format)")
  {
    unsigned char array[20] = {
      // Machine
      0x64,0x86, // 0x8664: x64
      // NumberOfSections
      0x23,0x01, // 0x0123
      // TimeDateStamp
      0x90,0x03,0x02,0x01, // 0x01020390
      // PointerToSymbolTable
      0x06,0x05,0x04,0x03,  // 0x03040506
      // NumberOfSymbols
      0x00,0x00,0x00,0x00,  // 0 (NumberOfSymbols is deprecated)
      // SizeOfOptionalHeader
      0x02,0x01, // 0x0102
      // Characteristics
      0x03,0x02 // 0x0203
    };

    ByteArraySpan arraySpan = arraySpanFromArray( array, sizeof(array) );
    header = coffHeaderFromArray(arraySpan);
    REQUIRE( header.machine == 0x8664 );
    REQUIRE( header.numberOfSections == 0x0123 );
    REQUIRE( header.timeDateStamp == 0x01020390 );
    REQUIRE( header.pointerToSymbolTable == 0x03040506 );
    REQUIRE( header.numberOfSymbols == 0 );
    REQUIRE( header.sizeOfOptionalHeader == 0x0102 );
    REQUIRE( header.characteristics == 0x0203 );
  }
}

TEST_CASE("CoffStringTableHandle")
{
  using Pe::CoffStringTableHandle;

  CoffStringTableHandle stringTable;

  SECTION("default constructed")
  {
    REQUIRE( stringTable.isNull() );
    REQUIRE( stringTable.isEmpty() );
  }

  SECTION("empty string table")
  {
    unsigned char array[4] = {1,2,3,4};
    stringTable.table = arraySpanFromArray( array, sizeof(array) );

    REQUIRE( !stringTable.isNull() );
    REQUIRE( stringTable.isEmpty() );
  }
}

TEST_CASE("ImageDataDirectory")
{
  using Pe::ImageDataDirectory;

  ImageDataDirectory directory;

  SECTION("fromUint64")
  {
    directory = ImageDataDirectory::fromUint64(0x1234567887654321);
    REQUIRE( directory.virtualAddress == 0x87654321 );
    REQUIRE( directory.size == 0x12345678 );
  }
}

TEST_CASE("OptionalHeader")
{
  using Pe::OptionalHeader;
  using Pe::MagicType;

  OptionalHeader header;

  SECTION("default constructed")
  {
    REQUIRE( header.magicType() == MagicType::Unknown );
  }

  SECTION("Magic type")
  {
    SECTION("PE32"){
      header.magic = 0x10b;
      REQUIRE( header.magicType() == MagicType::Pe32 );
    }

    SECTION("PE32+"){
      header.magic = 0x20b;
      REQUIRE( header.magicType() == MagicType::Pe32Plus );
    }

    SECTION("ROM image"){
      header.magic = 0x107;
      REQUIRE( header.magicType() == MagicType::RomImage );
    }

    SECTION("unknown")
    {
      header.magic = 0x1234;
      REQUIRE( header.magicType() == MagicType::Unknown );
    }
  }

  SECTION("import table")
  {
    SECTION("default constructed")
    {
      REQUIRE( !header.containsImportTable() );
    }

    SECTION("no import table")
    {
      header.numberOfRvaAndSizes = 1;
      REQUIRE( !header.containsImportTable() );
    }

    SECTION("has import table")
    {
      header.numberOfRvaAndSizes = 2;
      header.importTable = 125;
      REQUIRE( header.containsImportTable() );
    }
  }

  SECTION("delay import table")
  {
    SECTION("default constructed")
    {
      REQUIRE( !header.containsDelayImportTable() );
    }

    SECTION("no delay import table")
    {
      header.numberOfRvaAndSizes = 13;
      REQUIRE( !header.containsDelayImportTable() );
    }

    SECTION("has delay import table")
    {
      header.numberOfRvaAndSizes = 14;
      header.delayImportTable = 126;
      REQUIRE( header.containsDelayImportTable() );
    }
  }

  SECTION("Debug directory")
  {
    SECTION("default constructed")
    {
      REQUIRE( !header.containsDebugDirectory() );
    }

    SECTION("no debug directory")
    {
      header.numberOfRvaAndSizes = 6;
      REQUIRE( !header.containsDebugDirectory() );
    }

    SECTION("has debug directory")
    {
      header.numberOfRvaAndSizes = 7;
      header.debug = 1234;
      REQUIRE( header.containsDebugDirectory() );
    }
  }
}

TEST_CASE("optionalHeaderFromArray")
{
  using Pe::optionalHeaderFromArray;
  using Pe::OptionalHeader;
  using Pe::CoffHeader;

  OptionalHeader header;
  CoffHeader coffHeader;

  SECTION("32-bit image file")
  {
    coffHeader = makeValid32BitDllCoffHeader();
    coffHeader.sizeOfOptionalHeader = 208;

    unsigned char array[208] = {};
    // Magic: 0x010B, PE32
    array[0] = 0x0B;
    array[1] = 0x01;
    // NumberOfRvaAndSizes: 0x12345678
    array[92] = 0x78;
    array[93] = 0x56;
    array[94] = 0x34;
    array[95] = 0x12;
    // Import Table: 0x1234567812345678
    array[104] = 0x78;
    array[105] = 0x56;
    array[106] = 0x34;
    array[107] = 0x12;
    array[108] = 0x78;
    array[109] = 0x56;
    array[110] = 0x34;
    array[111] = 0x12;
    // Debug directory: 0x8765432187654321
    array[144] = 0x21;
    array[145] = 0x43;
    array[146] = 0x65;
    array[147] = 0x87;
    array[148] = 0x21;
    array[149] = 0x43;
    array[150] = 0x65;
    array[151] = 0x87;
    // Delay import table: 0x3456789012345678
    array[200] = 0x78;
    array[201] = 0x56;
    array[202] = 0x34;
    array[203] = 0x12;
    array[204] = 0x90;
    array[205] = 0x78;
    array[206] = 0x56;
    array[207] = 0x34;

    ByteArraySpan arraySpan = arraySpanFromArray( array, sizeof(array) );
    header = optionalHeaderFromArray(arraySpan, coffHeader);
    REQUIRE( header.magic == 0x10B );
    REQUIRE( header.numberOfRvaAndSizes == 0x12345678 );
    REQUIRE( header.importTable == 0x1234567812345678 );
    REQUIRE( header.debug == 0x8765432187654321 );
    REQUIRE( header.delayImportTable == 0x3456789012345678 );
  }

  SECTION("64-bit image file")
  {
    coffHeader = makeValid64BitDllCoffHeader();
    coffHeader.sizeOfOptionalHeader = 224;

    unsigned char array[224] = {};
    // Magic: 0x020B, PE32+
    array[0] = 0x0B;
    array[1] = 0x02;
    // NumberOfRvaAndSizes: 0x12345678
    array[108] = 0x78;
    array[109] = 0x56;
    array[110] = 0x34;
    array[111] = 0x12;
    // Import Table: 0x1234567812345678
    array[120] = 0x78;
    array[121] = 0x56;
    array[122] = 0x34;
    array[123] = 0x12;
    array[124] = 0x78;
    array[125] = 0x56;
    array[126] = 0x34;
    array[127] = 0x12;
    // Debug directory: 0x8765432187654321
    array[160] = 0x21;
    array[161] = 0x43;
    array[162] = 0x65;
    array[163] = 0x87;
    array[164] = 0x21;
    array[165] = 0x43;
    array[166] = 0x65;
    array[167] = 0x87;
    // Delay import table: 0x3456789012345678
    array[216] = 0x78;
    array[217] = 0x56;
    array[218] = 0x34;
    array[219] = 0x12;
    array[220] = 0x90;
    array[221] = 0x78;
    array[222] = 0x56;
    array[223] = 0x34;

    ByteArraySpan arraySpan = arraySpanFromArray( array, sizeof(array) );
    header = optionalHeaderFromArray(arraySpan, coffHeader);
    REQUIRE( header.magic == 0x20B );
    REQUIRE( header.numberOfRvaAndSizes == 0x12345678 );
    REQUIRE( header.importTable == 0x1234567812345678 );
    REQUIRE( header.debug == 0x8765432187654321 );
    REQUIRE( header.delayImportTable == 0x3456789012345678 );
  }
}

TEST_CASE("qStringFromUft8BoundedUnsignedCharArray")
{
  using Pe::qStringFromUft8BoundedUnsignedCharArray;

  ByteArraySpan arraySpan;

  SECTION("Section table format")
  {
    SECTION(".idata")
    {
      unsigned char array[8] = {
        '.','i','d','a','t','a',0,0
      };
      arraySpan = arraySpanFromArray( array, sizeof(array) );
      REQUIRE( qStringFromUft8BoundedUnsignedCharArray(arraySpan) == QLatin1String(".idata") );
    }

    SECTION("abcdefgh (no string terminating null)")
    {
      unsigned char array[8] = {
        'a','b','c','d','e','f','g','h'
      };
      arraySpan = arraySpanFromArray( array, sizeof(array) );
      REQUIRE( qStringFromUft8BoundedUnsignedCharArray(arraySpan) == QLatin1String("abcdefgh") );
    }
  }
}

TEST_CASE("getSectionHeaderName")
{
  using Pe::getSectionHeaderName;

  ByteArraySpan arraySpan;
  Pe::CoffStringTableHandle stringTable;

  SECTION("Short names (standard for image files)")
  {
    /*
     * We don't have to check every possible string length etc,
     * this is done by qStringFromUft8BoundedUnsignedCharArray()
     */
    SECTION(".idata")
    {
      unsigned char array[8] = {
        '.','i','d','a','t','a',0,0
      };
      arraySpan = arraySpanFromArray( array, sizeof(array) );
      REQUIRE( getSectionHeaderName(arraySpan, stringTable) == QLatin1String(".idata") );
    }
  }

  SECTION("Long names")
  {
    SECTION("string table is empty")
    {
      SECTION("/4")
      {
        unsigned char array[8] = {
          '/','4',0,0,0,0,0,0
        };
        arraySpan = arraySpanFromArray( array, sizeof(array) );
        REQUIRE( getSectionHeaderName(arraySpan, stringTable) == QLatin1String("/4") );
      }
    }

    SECTION("string table exists and is not empty")
    {
      unsigned char stringTableArray[29] = {
        0,0,0,0x1D, // size: 29
        '.','e','h','_','f','r','a','m','e',0,
        '.','d','e','b','u','g','_','a','r','a','n','g','e','s',0
      };
      stringTable.table = arraySpanFromArray( stringTableArray, sizeof(stringTableArray) );

      SECTION(".eh_frame")
      {
        unsigned char array[8] = {
          '/','4',0,0,0,0,0,0
        };
        arraySpan = arraySpanFromArray( array, sizeof(array) );
        REQUIRE( getSectionHeaderName(arraySpan, stringTable) == QLatin1String(".eh_frame") );
      }

      SECTION(".debug_aranges")
      {
        unsigned char array[8] = {
          '/','1','4',0,0,0,0,0
        };
        arraySpan = arraySpanFromArray( array, sizeof(array) );
        REQUIRE( getSectionHeaderName(arraySpan, stringTable) == QLatin1String(".debug_aranges") );
      }

    }
  }
}

TEST_CASE("SectionHeader_rvaIsInThisSection")
{
  using Pe::SectionHeader;

  SectionHeader header;

  header.name = QLatin1String(".idata");
  header.virtualSize = 10;
  header.virtualAddress = 10000;
  header.sizeOfRawData = 100;
  header.pointerToRawData = 1000;
  REQUIRE( header.seemsValid() );

  SECTION("RVA is in section")
  {
    REQUIRE( header.rvaIsInThisSection(10005) );
  }

  SECTION("RVA is at the beginning of the section")
  {
    REQUIRE( header.rvaIsInThisSection(10000) );
  }

  SECTION("RVA is at the end of the section")
  {
    REQUIRE( header.rvaIsInThisSection(10009) );
  }

  SECTION("RVA is just before the section")
  {
    REQUIRE( !header.rvaIsInThisSection(9999) );
  }

  SECTION("RVA is just after the section")
  {
    REQUIRE( !header.rvaIsInThisSection(10010) );
  }
}

TEST_CASE("SectionHeader")
{
  using Pe::SectionHeader;

  SectionHeader header;

  SECTION("default constructed")
  {
    REQUIRE( !header.seemsValid() );
  }

  SECTION("validity")
  {
    header.name = QLatin1String(".idata");
    header.virtualSize = 10;
    header.virtualAddress = 10000;
    header.sizeOfRawData = 100;
    header.pointerToRawData = 1000;
    REQUIRE( header.seemsValid() );

    SECTION("name with / (not valid for executable image)")
    {
      header.name = QLatin1String("/1234");
      REQUIRE( !header.seemsValid() );
    }

    SECTION("virtualAddress and pointerToRawData")
    {
      SECTION("valid: virtualAddress > pointerToRawData")
      {
        header.virtualAddress = 1000;
        header.pointerToRawData = 100;
        REQUIRE( header.seemsValid() );
      }

      SECTION("valid (limit): virtualAddress == pointerToRawData")
      {
        header.virtualAddress = 1000;
        header.pointerToRawData = 1000;
        REQUIRE( header.seemsValid() );
      }

      SECTION("invalid: virtualAddress < pointerToRawData")
      {
        header.virtualAddress = 100;
        header.pointerToRawData = 1000;
        REQUIRE( !header.seemsValid() );
      }
    }
  }

  SECTION("RVA and file offset")
  {
    header.name = QLatin1String(".idata");
    header.virtualSize = 10;
    header.virtualAddress = 10000;
    header.sizeOfRawData = 100;
    header.pointerToRawData = 1000;
    REQUIRE( header.seemsValid() );

    SECTION("valid RVA")
    {
      REQUIRE( header.rvaIsValid(10005) );
      REQUIRE( header.rvaToFileOffset(10005) == 1005 );
    }

    SECTION("valid RVA (limit case)")
    {
      REQUIRE( header.rvaIsValid(10000) );
      REQUIRE( header.rvaToFileOffset(10000) == 1000 );
    }

    SECTION("invalid RVA")
    {
      REQUIRE( !header.rvaIsValid(8000) );
    }
  }
}

TEST_CASE("sectionHeaderFromArray")
{
  using Pe::sectionHeaderFromArray;
  using Pe::CoffStringTableHandle;
  using Pe::SectionHeader;

  unsigned char array[40] = {};
  ByteArraySpan arraySpan;
  CoffStringTableHandle stringTable;
  SectionHeader header;

  SECTION(".idata")
  {
    // Name
    array[0] = '.';
    array[1] = 'i';
    array[2] = 'd';
    array[3] = 'a';
    array[4] = 't';
    array[5] = 'a';
    array[6] = 0;
    array[7] = 0;
    // VirtualSize: 0x12345678
    array[8] = 0x78;
    array[9] = 0x56;
    array[10] = 0x34;
    array[11] = 0x12;
    // VirtualAddress: 0x34567890
    array[12] = 0x90;
    array[13] = 0x78;
    array[14] = 0x56;
    array[15] = 0x34;
    // SizeOfRawData: 0x12345678
    array[16] = 0x78;
    array[17] = 0x56;
    array[18] = 0x34;
    array[19] = 0x12;
    // PointerToRawData: 0x87654321
    array[20] = 0x21;
    array[21] = 0x43;
    array[22] = 0x65;
    array[23] = 0x87;

    arraySpan = arraySpanFromArray( array, sizeof(array) );
    header = sectionHeaderFromArray(arraySpan, stringTable);
    REQUIRE( header.name == QLatin1String(".idata") );
    REQUIRE( header.virtualSize == 0x12345678 );
    REQUIRE( header.virtualAddress == 0x34567890 );
    REQUIRE( header.sizeOfRawData == 0x12345678 );
    REQUIRE( header.pointerToRawData == 0x87654321 );
  }
}

TEST_CASE("importDirectoryFromArray")
{
  using Pe::importDirectoryFromArray;
  using Pe::ImportDirectory;

  unsigned char array[20] = {};
  ByteArraySpan arraySpan;
  ImportDirectory directory;

  // Name RVA: 0x12345678
  array[12] = 0x78;
  array[13] = 0x56;
  array[14] = 0x34;
  array[15] = 0x12;

  arraySpan = arraySpanFromArray( array, sizeof(array) );
  directory = importDirectoryFromArray(arraySpan);
  REQUIRE( directory.nameRVA == 0x12345678 );
}

TEST_CASE("delayLoadDirectoryFromArray")
{
  using Pe::delayLoadDirectoryFromArray;
  using Pe::DelayLoadDirectory;

  unsigned char array[32] = {};
  ByteArraySpan arraySpan;
  DelayLoadDirectory directory;

  // Attributes: 0x34567890
  array[0] = 0x90;
  array[1] = 0x78;
  array[2] = 0x56;
  array[3] = 0x34;
  // Name RVA: 0x12345678
  array[4] = 0x78;
  array[5] = 0x56;
  array[6] = 0x34;
  array[7] = 0x12;

  arraySpan = arraySpanFromArray( array, sizeof(array) );
  directory = delayLoadDirectoryFromArray(arraySpan);
  REQUIRE( directory.attributes == 0x34567890 );
  REQUIRE( directory.nameRVA == 0x12345678 );
}
