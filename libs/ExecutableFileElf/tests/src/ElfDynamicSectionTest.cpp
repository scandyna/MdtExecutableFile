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
#include "TestUtils.h"
#include "ElfDynamicSectionTestCommon.h"
#include "Mdt/ExecutableFile/Elf/DynamicSection.h"
#include <QLatin1String>

#include <QDebug>

bool sectionContainsRunPathEntry(const DynamicSection & section)
{
  for(DynamicStruct s : section){
    if( s.tagType() == DynamicSectionTagType::Runpath ){
      return true;
    }
  }

  return false;
}


TEST_CASE("DynamicStruct")
{
  SECTION("default constructed")
  {
    DynamicStruct ds;
    REQUIRE( ds.tagType() == DynamicSectionTagType::Null );
  }

  SECTION("construct with a tag")
  {
    DynamicStruct ds(DynamicSectionTagType::Runpath);
    REQUIRE( ds.tagType() == DynamicSectionTagType::Runpath );
  }
}

TEST_CASE("isNull")
{
  DynamicSection section;

  SECTION("default constructed")
  {
    REQUIRE( section.isNull() );
  }

  SECTION("add a entry")
  {
    section.addEntry( makeNeededEntry() );
    REQUIRE( !section.isNull() );
  }
}

TEST_CASE("byteCount")
{
  using Mdt::ExecutableFile::Elf::Class;

  DynamicSection section;

  SECTION("default constructed")
  {
    SECTION("32-bit")
    {
      REQUIRE( section.byteCount(Class::Class32) == 0 );
    }

    SECTION("64-bit")
    {
      REQUIRE( section.byteCount(Class::Class64) == 0 );
    }
  }

  SECTION("1 entry")
  {
    section.addEntry( makeNullEntry() );

    SECTION("32-bit")
    {
      REQUIRE( section.byteCount(Class::Class32) == 8 );
    }

    SECTION("64-bit")
    {
      REQUIRE( section.byteCount(Class::Class64) == 16 );
    }
  }

  SECTION("2 entries")
  {
    section.addEntry( makeNullEntry() );
    section.addEntry( makeNeededEntry(5) );

    SECTION("32-bit")
    {
      REQUIRE( section.byteCount(Class::Class32) == 16 );
    }

    SECTION("64-bit")
    {
      REQUIRE( section.byteCount(Class::Class64) == 32 );
    }
  }
}

TEST_CASE("add_entry_clear")
{
  DynamicSection section;

  REQUIRE( !section.containsStringTableSizeEntry() );
  section.addEntry( makeStringTableSizeEntry(1) );
  REQUIRE( section.entriesCount() == 1 );
  REQUIRE( section.containsStringTableSizeEntry() );

  uchar stringTable[8] = {'\0','S','o','N','a','m','e','\0'};
  section.setStringTable( stringTableFromCharArray( stringTable, sizeof(stringTable) ) );
  REQUIRE( section.stringTable().byteCount() == 8 );

  section.clear();
  REQUIRE( section.entriesCount() == 0 );
  REQUIRE( section.stringTable().byteCount() == 1 );
  REQUIRE( section.isNull() );
}

TEST_CASE("gnuHashTableAddress")
{
  DynamicSection section;

  SECTION("initially the DT_GNU_HASH entry not exists")
  {
    REQUIRE( !section.containsGnuHashTableAddress() );
  }

  SECTION("DT_GNU_HASH exists - change and get it")
  {
    section.addEntry( makeGnuHashEntry(1234) );

    REQUIRE( section.containsGnuHashTableAddress() );
    REQUIRE( section.gnuHashTableAddress() == 1234 );

    section.setGnuHashTableAddress(524);
    REQUIRE( section.gnuHashTableAddress() == 524 );

    section.clear();
    REQUIRE( !section.containsGnuHashTableAddress() );
  }
}

TEST_CASE("stringTableAddress")
{
  DynamicSection section;

  SECTION("initially the DT_STRTAB entry not exists")
  {
    REQUIRE( !section.containsStringTableAddress() );
  }

  SECTION("DT_STRTAB exists - change and get it")
  {
    section.addEntry( makeStringTableAddressEntry(5568) );

    REQUIRE( section.containsStringTableAddress() );
    REQUIRE( section.stringTableAddress() == 5568 );

    section.setStringTableAddress(512);
    REQUIRE( section.stringTableAddress() == 512 );

    section.clear();
    REQUIRE( !section.containsStringTableAddress() );
  }

  /*
   * Bug from 17.11.2021
   */
  SECTION("adding DT_RUNPATH must not break string table size index")
  {
    section.addEntry( makeNullEntry() );
    section.addEntry( makeStringTableAddressEntry(123) );

    REQUIRE( section.containsStringTableAddress() );
    REQUIRE( !section.containsRunPathEntry() );

    SECTION("section not contains null entries at end")
    {
      section.addRunPathEntry( makeRunPathEntry() );
      REQUIRE( section.containsRunPathEntry() );

      REQUIRE( section.containsStringTableAddress() );
      REQUIRE( section.stringTableAddress() == 123 );
    }

    SECTION("section contains null entries at end")
    {
      section.addEntry( makeNullEntry() );
      // the DT_RUNPATH entry should be added before the ending DT_NULL entry
      section.addRunPathEntry( makeRunPathEntry() );
      REQUIRE( section.containsRunPathEntry() );

      REQUIRE( section.containsStringTableAddress() );
      REQUIRE( section.stringTableAddress() == 123 );
    }
  }

  SECTION("Removing DT_RUNPATH must not break string table size index")
  {
    uchar initialStringTable[6] = {
      '\0',
      '/','t','m','p','\0'
    };

    section.addEntry( makeNullEntry() );
    section.addEntry( makeRunPathEntry(1) );
    section.addEntry( makeStringTableSizeEntry(1) );
    section.addEntry( makeStringTableAddressEntry(156) );
    section.addEntry( makeNullEntry() );

    section.setStringTable( stringTableFromCharArray( initialStringTable, sizeof(initialStringTable) ) );

    REQUIRE( section.containsRunPathEntry() );
    REQUIRE( section.containsStringTableAddress() );
    REQUIRE( section.stringTableAddress() == 156 );

    section.removeRunPath();

    REQUIRE( !section.containsRunPathEntry() );
    REQUIRE( section.containsStringTableAddress() );
    REQUIRE( section.stringTableAddress() == 156 );
  }
}

TEST_CASE("containsRunPathEntry")
{
  DynamicSection section;

  SECTION("default constructed")
  {
    REQUIRE( !section.containsRunPathEntry() );
  }

  SECTION("with DT_RUNPATH entry")
  {
    section.addEntry( makeRunPathEntry(1) );
    REQUIRE( section.containsRunPathEntry() );
  }
}

TEST_CASE("getStringTableAddress")
{
  DynamicSection section;
  section.addEntry( makeStringTableAddressEntry(1000) );

  SECTION("get from existing entry")
  {
    REQUIRE( section.stringTableAddress() == 1000 );
  }
}

TEST_CASE("setStringTableAddress")
{
  DynamicSection section;
  section.addEntry( makeStringTableAddressEntry(1000) );

  section.setStringTableAddress(150);
  REQUIRE( section.stringTableAddress() == 150 );
}

TEST_CASE("getStringTableSize")
{
  DynamicSection section;
  section.addEntry( makeStringTableSizeEntry(20) );

  SECTION("get from existing entry")
  {
    REQUIRE( section.getStringTableSize() == 20 );
  }
}

TEST_CASE("setStringTable")
{
  DynamicSection section;
  uchar stringTableArray[8] = {'\0','S','o','N','a','m','e','\0'};
  const StringTable stringTable = stringTableFromCharArray( stringTableArray, sizeof(stringTableArray) );

  section.addEntry( makeStringTableAddressEntry(1000) );
  section.addEntry( makeStringTableSizeEntry(1) );

  section.setStringTable(stringTable);

  REQUIRE( section.stringTableAddress() == 1000 );
  REQUIRE( section.getStringTableSize() == 8 );
}

TEST_CASE("getSoName")
{
  DynamicSection section;
  section.addEntry( makeStringTableSizeEntry(1) );

  uchar stringTable[8] = {'\0','S','o','N','a','m','e','\0'};
  section.setStringTable( stringTableFromCharArray( stringTable, sizeof(stringTable) ) );

  SECTION("no DT_SONAME present")
  {
    REQUIRE( section.getSoName().isEmpty() );
  }

  SECTION("SoName")
  {
    section.addEntry( makeSoNameEntry(1) );
    REQUIRE( section.getSoName() == QLatin1String("SoName") );
  }
}

TEST_CASE("getNeededSharedLibraries")
{
  DynamicSection section;
  section.addEntry( makeStringTableSizeEntry(1) );

  uchar stringTable[17] = {
    '\0',
    'l','i','b','A','.','s','o','\0',
    'l','i','b','B','.','s','o','\0'
  };
  section.setStringTable( stringTableFromCharArray( stringTable, sizeof(stringTable) ) );

  SECTION("no DT_NEEDED present")
  {
    REQUIRE( section.getNeededSharedLibraries().isEmpty() );
  }

  SECTION("libA.so")
  {
    section.addEntry( makeNeededEntry(1) );
    REQUIRE( section.getNeededSharedLibraries() == qStringListFromUtf8Strings({"libA.so"}) );
  }

  SECTION("libA.so libB.so")
  {
    section.addEntry( makeNeededEntry(1) );
    section.addEntry( makeNeededEntry(9) );
    REQUIRE( section.getNeededSharedLibraries() == qStringListFromUtf8Strings({"libA.so","libB.so"}) );
  }
}

TEST_CASE("addRunPathEntry")
{
  DynamicSection section;
  DynamicStruct entry(DynamicSectionTagType::Runpath);

  SECTION("the dynamic section is initially empty")
  {
    section.addRunPathEntry(entry);

    REQUIRE( section.entriesCount() == 1 );
    REQUIRE( section.entryAt(0).tagType() == DynamicSectionTagType::Runpath );
  }

  SECTION("the dynamic section initially has only a null entry")
  {
    section.addEntry( makeNullEntry() );
    section.addRunPathEntry(entry);

    REQUIRE( section.entriesCount() == 2 );
    REQUIRE( section.entryAt(0).tagType() == DynamicSectionTagType::Runpath );
    REQUIRE( section.entryAt(1).isNull() );
  }

  SECTION("the dynamic section initially contains only a DT_NEEDED entry")
  {
    section.addEntry( makeNeededEntry() );
    section.addRunPathEntry(entry);

    REQUIRE( section.entriesCount() == 2 );
    REQUIRE( section.entryAt(0).tagType() == DynamicSectionTagType::Needed );
    REQUIRE( section.entryAt(1).tagType() == DynamicSectionTagType::Runpath );
  }
}

TEST_CASE("getRunPath")
{
  DynamicSection section;
  section.addEntry( makeStringTableSizeEntry(1) );

  uchar stringTable[13] = {
    '\0',
    '/','t','m','p',':',
    '/','p','a','t','h','2','\0'
  };
  section.setStringTable( stringTableFromCharArray( stringTable, sizeof(stringTable) ) );

  SECTION("no DT_RUNPATH present")
  {
    REQUIRE( section.getRunPath().isEmpty() );
  }

  SECTION("/tmp:/path2")
  {
    section.addEntry( makeRunPathEntry(1) );
    REQUIRE( section.getRunPath() == QLatin1String("/tmp:/path2") );
  }
}

TEST_CASE("removeRunPath")
{
  DynamicSection section;
  section.addEntry( makeStringTableSizeEntry(1) );

  SECTION("section does not have the DT_RUNPATH")
  {
    section.removeRunPath();
    REQUIRE( !sectionContainsRunPathEntry(section) );
    REQUIRE( section.stringTable().isEmpty() );
    REQUIRE( section.getStringTableSize() == 1 );
  }

  SECTION("section contains the DT_RUNPATH")
  {
    uchar initialStringTable[6] = {
      '\0',
      '/','t','m','p','\0'
    };
    section.setStringTable( stringTableFromCharArray( initialStringTable, sizeof(initialStringTable) ) );
    section.addEntry( makeRunPathEntry(1) );
    REQUIRE( sectionContainsRunPathEntry(section) );
    REQUIRE( section.getRunPath() == QLatin1String("/tmp") );

    section.removeRunPath();
    REQUIRE( !sectionContainsRunPathEntry(section) );
    REQUIRE( section.stringTable().isEmpty() );
    REQUIRE( section.getStringTableSize() == 1 );
  }

  SECTION("section contains the DT_RUNPATH and a DT_NEEDED")
  {
    uchar initialStringTable[14] = {
      '\0',
      '/','t','m','p','\0',
      'l','i','b','A','.','s','o','\0'
    };
    section.setStringTable( stringTableFromCharArray( initialStringTable, sizeof(initialStringTable) ) );
    section.addEntry( makeRunPathEntry(1) );
    section.addEntry( makeNeededEntry(6) );
    REQUIRE( sectionContainsRunPathEntry(section) );
    REQUIRE( section.getRunPath() == QLatin1String("/tmp") );
    REQUIRE( section.getNeededSharedLibraries() == qStringListFromUtf8Strings({"libA.so"}) );

    section.removeRunPath();
    REQUIRE( !sectionContainsRunPathEntry(section) );
    REQUIRE( section.stringTable().byteCount() == 1+7+1 );
    REQUIRE( section.getStringTableSize() == 1+7+1 );
    REQUIRE( section.getNeededSharedLibraries() == qStringListFromUtf8Strings({"libA.so"}) );
  }
}

TEST_CASE("setRunPath")
{
  DynamicSection section;
  section.addEntry( makeStringTableSizeEntry(1) );

  SECTION("there is initially no entry in the section (string table is also empty)")
  {
    section.setRunPath( QLatin1String("/path1:/path2") );
    REQUIRE( section.getRunPath() == QLatin1String("/path1:/path2") );
    REQUIRE( section.stringTable().byteCount() == 1+13+1 );
    REQUIRE( section.getStringTableSize() == 1+13+1 );
  }

  SECTION("there is initially only a DT_NEEDED entry")
  {
    uchar initialStringTable[9] = {
      '\0',
      'l','i','b','A','.','s','o','\0'
    };
    section.setStringTable( stringTableFromCharArray( initialStringTable, sizeof(initialStringTable) ) );
    section.addEntry( makeNeededEntry(1) );
    REQUIRE( section.getNeededSharedLibraries() == qStringListFromUtf8Strings({"libA.so"}) );
    REQUIRE( section.getRunPath().isEmpty() );

    section.setRunPath( QLatin1String("/path1:/path2") );
    REQUIRE( section.getNeededSharedLibraries() == qStringListFromUtf8Strings({"libA.so"}) );
    REQUIRE( section.getRunPath() == QLatin1String("/path1:/path2") );
  }

  SECTION("there is initially only the DT_RUNPATH entry (string table contains only a DT_RUNPATH string)")
  {
    uchar initialStringTable[6] = {
      '\0',
      '/','t','m','p','\0'
    };
    section.setStringTable( stringTableFromCharArray( initialStringTable, sizeof(initialStringTable) ) );
    section.addEntry( makeRunPathEntry(1) );
    REQUIRE( section.getRunPath() == QLatin1String("/tmp") );

    SECTION("set a RunPath")
    {
      section.setRunPath( QLatin1String("/path1:/path2") );
      REQUIRE( section.getRunPath() == QLatin1String("/path1:/path2") );
    }

    SECTION("set a empty RunPath")
    {
      REQUIRE( sectionContainsRunPathEntry(section) );
      section.setRunPath( QString() );
      REQUIRE( !sectionContainsRunPathEntry(section) );
      REQUIRE( section.stringTable().isEmpty() );
      REQUIRE( section.getRunPath().isEmpty() );
    }
  }

  SECTION("there is initially a DT_NEEDED entry and the DT_RUNPATH (string table contains related strings)")
  {
    uchar initialStringTable[14] = {
      '\0',
      'l','i','b','A','.','s','o','\0',
      '/','t','m','p','\0'
    };
    section.setStringTable( stringTableFromCharArray( initialStringTable, sizeof(initialStringTable) ) );

    SECTION("add DT_NEEDED then DT_RUNPATH (indexes: 1,9 -> ascending)")
    {
      section.addEntry( makeNeededEntry(1) );
      section.addEntry( makeRunPathEntry(9) );
      REQUIRE( section.getNeededSharedLibraries() == qStringListFromUtf8Strings({"libA.so"}) );
      REQUIRE( section.getRunPath() == QLatin1String("/tmp") );

      section.setRunPath( QLatin1String("/path1:/path2") );
      REQUIRE( section.getRunPath() == QLatin1String("/path1:/path2") );
      REQUIRE( section.getNeededSharedLibraries() == qStringListFromUtf8Strings({"libA.so"}) );
    }

    SECTION("add DT_RUNPATH then DT_NEEDED (indexes: 9,1)")
    {
      section.addEntry( makeRunPathEntry(9) );
      section.addEntry( makeNeededEntry(1) );
      REQUIRE( section.getNeededSharedLibraries() == qStringListFromUtf8Strings({"libA.so"}) );
      REQUIRE( section.getRunPath() == QLatin1String("/tmp") );

      section.setRunPath( QLatin1String("/path1:/path2") );
      REQUIRE( section.getRunPath() == QLatin1String("/path1:/path2") );
      REQUIRE( section.getNeededSharedLibraries() == qStringListFromUtf8Strings({"libA.so"}) );
    }
  }

  SECTION("there is initially the DT_RUNPATH entry, then a DT_NEEDED entry (string table contains related strings)")
  {
    uchar initialStringTable[14] = {
      '\0',
      '/','t','m','p','\0',
      'l','i','b','A','.','s','o','\0'
    };
    section.setStringTable( stringTableFromCharArray( initialStringTable, sizeof(initialStringTable) ) );

    SECTION("add DT_RUNPATH then DT_NEEDED (indexes: 1,6 -> ascending)")
    {
      section.addEntry( makeRunPathEntry(1) );
      section.addEntry( makeNeededEntry(6) );
      REQUIRE( section.getRunPath() == QLatin1String("/tmp") );
      REQUIRE( section.getNeededSharedLibraries() == qStringListFromUtf8Strings({"libA.so"}) );

      SECTION("set a shorter RunPath")
      {
        section.setRunPath( QLatin1String("/a") );
        REQUIRE( section.getRunPath() == QLatin1String("/a") );
        REQUIRE( section.getNeededSharedLibraries() == qStringListFromUtf8Strings({"libA.so"}) );
      }

      SECTION("set a longer RunPath")
      {
        section.setRunPath( QLatin1String("/path1:/path2") );
        REQUIRE( section.getRunPath() == QLatin1String("/path1:/path2") );
        REQUIRE( section.getNeededSharedLibraries() == qStringListFromUtf8Strings({"libA.so"}) );
      }
    }

    SECTION("add DT_NEEDED then DT_RUNPATH (indexes: 6,1)")
    {
      section.addEntry( makeNeededEntry(6) );
      section.addEntry( makeRunPathEntry(1) );
      REQUIRE( section.getRunPath() == QLatin1String("/tmp") );
      REQUIRE( section.getNeededSharedLibraries() == qStringListFromUtf8Strings({"libA.so"}) );

      SECTION("set a shorter RunPath")
      {
        section.setRunPath( QLatin1String("/a") );
        REQUIRE( section.getRunPath() == QLatin1String("/a") );
        REQUIRE( section.getNeededSharedLibraries() == qStringListFromUtf8Strings({"libA.so"}) );
      }

      SECTION("set a longer RunPath")
      {
        section.setRunPath( QLatin1String("/path1:/path2") );
        REQUIRE( section.getRunPath() == QLatin1String("/path1:/path2") );
        REQUIRE( section.getNeededSharedLibraries() == qStringListFromUtf8Strings({"libA.so"}) );
      }
    }
  }
}
