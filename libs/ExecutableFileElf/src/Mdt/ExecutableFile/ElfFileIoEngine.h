// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_CORE_ELF_FILE_IO_ENGINE_H
#define MDT_EXECUTABLE_FILE_CORE_ELF_FILE_IO_ENGINE_H

#include "Mdt/ExecutableFile/AbstractExecutableFileIoEngine.h"
#include "Mdt/ExecutableFile/FileOpenError.h"
#include "Mdt/ExecutableFile/ExecutableFileReadError.h"
#include "Mdt/ExecutableFile/RPath.h"
#include "Mdt/ExecutableFile/Elf/FileIoEngine.h"
#include <QObject>
#include <QString>
#include <QStringList>


namespace Mdt{ namespace ExecutableFile{

  /*! \brief Minimal ELF file I/O engine
   *
   * This ELF reader and writer can just read/write a minimal set of informations
   * required for deployement of applications.
   *
   * For other purposes, other tools
   * like objdump or readelf should be considered.
   *
   * \sa ExecutableFileReader
   * \sa ExecutableFileWriter
   */
  class ElfFileIoEngine : public AbstractExecutableFileIoEngine
  {
    Q_OBJECT

   public:

    /*! \brief Construct a file I/O engine
     */
    explicit ElfFileIoEngine(QObject *parent = nullptr);

    /*! \brief Get the shared object name (SONAME) of the file this engine refers to
     *
     * \pre this engine must have a open file which is a executable or a shared library
     * \sa isOpen()
     * \sa isExecutableOrSharedLibrary()
     * \exception ExecutableFileReadError
     */
    QString getSoName();

   private:

    void newFileOpen(const QString & fileName) override;
    void fileClosed() override;
    bool doSupportsPlatform(const Platform & platform) const noexcept override;
    bool doIsElfFile() override;
    Platform doGetFilePlatform() override;
    bool doIsExecutableOrSharedLibrary() override;
    bool doContainsDebugSymbols() override;
    QStringList doGetNeededSharedLibraries() override;
    RPath doGetRunPath() override;
    void doSetRunPath(const RPath & rPath) override;

    Elf::FileIoEngine mImpl;
  };

}} // namespace Mdt{ namespace ExecutableFile{

#endif // #ifndef MDT_EXECUTABLE_FILE_CORE_ELF_FILE_IO_ENGINE_H
