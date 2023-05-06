// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_CORE_PE_FILE_IO_ENGINE_H
#define MDT_EXECUTABLE_FILE_CORE_PE_FILE_IO_ENGINE_H

#include "Mdt/ExecutableFile/AbstractExecutableFileIoEngine.h"
#include <memory>

namespace Mdt{ namespace ExecutableFile{

  namespace Pe{

    class FileReader;

  } // namespace Pe{

  /*! \brief Minimal PE file I/O engine
   *
   * This PE reader and writer can just read/write a minimal set of informations
   * required for deployemnt of applications.
   *
   * For other purposes, other tools
   * like objdump or readpe should be considered.
   *
   * \sa ExecutableFileReader
   * \sa ExecutableFileWriter
   */
  class PeFileIoEngine : public AbstractExecutableFileIoEngine
  {
    Q_OBJECT

   public:

    /*! \brief Construct a file I/O engine
     */
    explicit PeFileIoEngine(QObject *parent = nullptr);

    ~PeFileIoEngine() noexcept;

  private:

    void newFileOpen(const QString & fileName) override;
    void fileClosed() override;
    bool doSupportsPlatform(const Platform & platform) const noexcept override;
    bool doIsPeImageFile() override;
    Platform doGetFilePlatform() override;
    bool doIsExecutableOrSharedLibrary() override;
    bool doContainsDebugSymbols() override;
    QStringList doGetNeededSharedLibraries() override;

    bool tryExtractDosCoffAndOptionalHeader();

    std::unique_ptr<Pe::FileReader> mImpl;
  };

}} // namespace Mdt{ namespace ExecutableFile{

#endif // #ifndef MDT_EXECUTABLE_FILE_CORE_PE_FILE_IO_ENGINE_H
