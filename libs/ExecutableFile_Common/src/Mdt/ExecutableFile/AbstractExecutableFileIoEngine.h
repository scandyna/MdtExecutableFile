// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_ABSTRACT_EXECUTABLE_FILE_IO_ENGINE_H
#define MDT_EXECUTABLE_FILE_ABSTRACT_EXECUTABLE_FILE_IO_ENGINE_H

#include "Mdt/ExecutableFile/FileOpenError.h"
#include "Mdt/ExecutableFile/ExecutableFileReadError.h"
#include "Mdt/ExecutableFile/ExecutableFileWriteError.h"
#include "Mdt/ExecutableFile/ExecutableFileOpenMode.h"
#include "Mdt/ExecutableFile/Platform.h"
#include "Mdt/ExecutableFile/RPath.h"
#include "Mdt/ExecutableFile/FileMapper.h"
#include "Mdt/ExecutableFile/ByteArraySpan.h"
#include "mdt_executablefile_common_export.h"
#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QStringList>


namespace Mdt{ namespace ExecutableFile{

  /*! \brief Interface to a minimal executable file I/O engine
   */
  class MDT_EXECUTABLEFILE_COMMON_EXPORT AbstractExecutableFileIoEngine : public QObject
  {
    Q_OBJECT

   public:

    /*! \brief Construct a file I/O engine
     */
    explicit AbstractExecutableFileIoEngine(QObject *parent = nullptr);

    /*! \brief Check if this I/O engine supports given platform
     */
    bool supportsPlatform(const Platform & platform) const noexcept
    {
      return doSupportsPlatform(platform);
    }

    /*! \brief Open a file
     *
     * This method does not check if \a fileInfo refers to a executable file of any format.
     *
     * \pre \a fileInfo must have a file path set
     * \pre this engine must not allready have a file open
     * \sa isOpen()
     * \sa close()
     * \exception FileOpenError
     */
    void openFile(const QFileInfo & fileInfo, ExecutableFileOpenMode mode);

    /*! \brief Check if this engine has a open file
     *
     * \sa openFile()
     * \sa close()
     */
    bool isOpen() const noexcept
    {
      return mFile.isOpen();
    }

    /*! \brief Close the file that was maybe open
     */
    void close();

    /*! \brief Check if this engine refers to a ELF file (Linux)
     *
     * \pre this engine must have a open file
     * \sa isOpen()
     * \exception ExecutableFileReadError
     * \note static library archive (libSomeLib.a) are not supported
     */
    bool isElfFile();

    /*! \brief Check if this engine refers to a PE image file (Windows)
     *
     * \pre this engine must have a open file
     * \sa isOpen()
     * \exception ExecutableFileReadError
     * \note static library archive (libSomeLib.a) are not supported
     */
    bool isPeImageFile();

    /*! \brief Get the platorm of the file this engine refers to
     *
     * \pre this engine must have a file open
     * \sa isOpen()
     * \exception ExecutableFileReadError
     */
    Platform getFilePlatform();

    /*! \brief Check if this engine refers to a executable or a shared library
     *
     * \pre this engine must have a open file
     * \sa isOpen()
     * \exception ExecutableFileReadError
     */
    bool isExecutableOrSharedLibrary();

    /*! \brief Check if this engine refers to a executable that contains debug symbols
     *
     * \pre this engine must have a open file which is a executable or a shared library
     * \sa isOpen()
     * \sa isExecutableOrSharedLibrary()
     * \exception ExecutableFileReadError
     */
    bool containsDebugSymbols();

    /*! \brief Get a list of needed shared libraries the file this engine refers to
     *
     * \pre this engine must have a open file which is a executable or a shared library
     * \sa isOpen()
     * \sa isExecutableOrSharedLibrary()
     * \exception ExecutableFileReadError
     */
    QStringList getNeededSharedLibraries();

    /*! \brief Get the run path for the file this engine refers to
     *
     * Will only return a result for executable formats that supports run path
     *
     * \pre this engine must have a open file which is a executable or a shared library
     * \sa isOpen()
     * \sa isExecutableOrSharedLibrary()
     * \exception ExecutableFileReadError
     */
    RPath getRunPath();

    /*! \brief Set the run path this engine refers to to \a rPath
     *
     * For executable formats that do not support rpath,
     * this method does nothing.
     *
     * \pre this engine must have a open file which is a executable or a shared library
     * \sa isOpen()
     * \sa isExecutableOrSharedLibrary()
     * \exception ExecutableFileWriteError
     */
    void setRunPath(const RPath & rPath);

   signals:

    void message(const QString & message) const;
    void verboseMessage(const QString & message) const;

   protected:

    /*! \brief Get the size of the file
     *
     * \pre this engine must have a open file
     * \sa isOpen()
     */
    qint64 fileSize() const noexcept;

    /*! \brief Resize current file
     *
     * \pre this engine must have a open file
     * \sa isOpen()
     * \pre \a size must be > 0
     * \exception ExecutableFileWriteError
     */
    void resizeFile(qint64 size);

    /*! \brief Get the name of the file
     *
     * \pre this engine must have a open file
     * \sa isOpen()
     */
    QString fileName() const noexcept;

    /*! \brief Map the file into memory
     *
     * \pre this engine must have a open file
     * \sa isOpen()
     * \pre \a offset must be >= 0
     * \pre \a size must be > 0
     * \pre \a file size must be at least \a offset + \a size
     * \exception FileOpenError
     */
    ByteArraySpan mapIfRequired(qint64 offset, qint64 size);

   private:

    virtual void newFileOpen(const QString & fileName) = 0;
    virtual void fileClosed() = 0;

    virtual bool doSupportsPlatform(const Platform & platform) const noexcept = 0;

    virtual bool doIsElfFile()
    {
      return false;
    }

    virtual bool doIsPeImageFile()
    {
      return false;
    }

    virtual Platform doGetFilePlatform() = 0;
    virtual bool doIsExecutableOrSharedLibrary() = 0;
    virtual bool doContainsDebugSymbols() = 0;
    virtual QStringList doGetNeededSharedLibraries() = 0;

    virtual RPath doGetRunPath()
    {
      return RPath();
    }

    virtual void doSetRunPath(const RPath & rPath);

    static
    QIODevice::OpenMode qIoDeviceOpenModeFromOpenMode(ExecutableFileOpenMode mode) noexcept;

    FileMapper mFileMapper;
    QFile mFile;
  };

}} // namespace Mdt{ namespace ExecutableFile{

#endif // #ifndef MDT_EXECUTABLE_FILE_ABSTRACT_EXECUTABLE_FILE_IO_ENGINE_H
