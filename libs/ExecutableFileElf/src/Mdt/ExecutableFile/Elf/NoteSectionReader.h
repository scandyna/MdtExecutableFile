// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ELF_NOTE_SECTION_READER_H
#define MDT_EXECUTABLE_FILE_ELF_NOTE_SECTION_READER_H

#include "FileReader.h"

#include "Mdt/ExecutableFile/Elf/SectionHeader.h"
#include "Mdt/ExecutableFile/Elf/SectionHeaderTable.h"

#include "Exceptions.h"
#include "Mdt/DeployUtils/Impl/ExecutableFileReaderUtils.h"

#include "Mdt/ExecutableFile/Elf/NoteSection.h"
#include "Mdt/ExecutableFile/Elf/NoteSectionTable.h"
#include "Mdt/ExecutableFile/Elf/FileHeader.h"
#include "Mdt/ExecutableFile/Elf/Algorithm.h"
#include "Mdt/ExecutableFile/ByteArraySpan.h"

// #include "mdt_deployutilscore_export.h"
#include <QObject>
#include <cstdint>
#include <cassert>

namespace Mdt{ namespace ExecutableFile{ namespace Elf{

  /*! \internal
   */
  class /*MDT_DEPLOYUTILSCORE_EXPORT*/ NoteSectionReader : public QObject
  {
    Q_OBJECT

   public:

    /*! \internal
     *
     * \exception NoteSectionReadError
     */
    static
    NoteSection noteSectionFromArray(const ByteArraySpan & array, const Ident & ident)
    {
      assert( !array.isNull() );
      assert( ident.isValid() );
      assert( array.size >= NoteSection::minimumByteBount() );

      NoteSection section;

      const int64_t nameSize = static_cast<int64_t>( getWord(array.data, ident.dataFormat) );
      if(nameSize == 0){
        const QString msg = tr("name size is 0");
        throw NoteSectionReadError(msg);
      }
      if( nameSize > NoteSection::maximumNameSize(array.size) ){
        const QString msg = tr("name size %1 is to large").arg(nameSize);
        throw NoteSectionReadError(msg);
      }
      assert(nameSize > 0);
      assert(nameSize < array.size);

      section.descriptionSize = getWord(array.subSpan(4, 4).data, ident.dataFormat);
      section.type = getWord(array.subSpan(8, 4).data, ident.dataFormat);

      section.name = stringFromBoundedUnsignedCharArray( array.subSpan(12, nameSize) );

      const int64_t descriptionStart = 12 + static_cast<int64_t>( findAlignedSize(static_cast<uint64_t>(nameSize), 4) );
      const int64_t descriptionEnd = descriptionStart + static_cast<int64_t>(section.descriptionSize);

      if(descriptionEnd > array.size){
        const QString msg = tr("section name size and/or description size is to large");
        throw NoteSectionReadError(msg);
      }

      for(int64_t offset = descriptionStart; offset < descriptionEnd; offset += 4){
        section.description.push_back( getWord(array.subSpan(offset, 4).data, ident.dataFormat) );
      }

      return section;
    }

    /*! \internal
     *
     * \exception NoteSectionReadError
     */
    static
    NoteSection extractNoteSection(const ByteArraySpan & map, const FileHeader & fileHeader, const SectionHeader & sectionHeader)
    {
      assert( !map.isNull() );
      assert( fileHeader.seemsValid() );
      assert( map.size >= sectionHeader.minimumSizeToReadSection() );
      assert( isNoteSectionHeader(sectionHeader) );

      const int64_t offset = static_cast<int64_t>(sectionHeader.offset);
      const int64_t size = static_cast<int64_t>(sectionHeader.size);

      try{
        return noteSectionFromArray(map.subSpan(offset, size), fileHeader.ident);
      }catch(const NoteSectionReadError & error){
        const QString msg = tr("note section %1 is corrupted: %2")
                            .arg( QString::fromStdString(sectionHeader.name), error.whatQString() );
        throw NoteSectionReadError(msg);
      }
    }

    /*! \internal
     *
     * \exception NoteSectionReadError
     */
    static
    NoteSectionTable extractNoteSectionTable(const ByteArraySpan & map, const FileHeader & fileHeader,
                                            const std::vector<SectionHeader> & sectionHeaderTable)
    {
      assert( !map.isNull() );
      assert( fileHeader.seemsValid() );
      assert( map.size >= fileHeader.minimumSizeToReadAllSectionHeaders() );

      NoteSectionTable table;

      for(const SectionHeader & header : sectionHeaderTable){
        if( isNoteSectionHeader(header) ){
          table.addSectionFromFile( header, extractNoteSection(map, fileHeader, header) );
        }
      }

      return table;
    }
  };

}}} // namespace Mdt{ namespace ExecutableFile{ namespace Elf{

#endif // #ifndef MDT_EXECUTABLE_FILE_ELF_NOTE_SECTION_READER_H
