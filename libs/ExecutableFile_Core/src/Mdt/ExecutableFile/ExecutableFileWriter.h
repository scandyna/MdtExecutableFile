// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_EXECUTABLE_FILE_WRITER_H
#define MDT_EXECUTABLE_FILE_EXECUTABLE_FILE_WRITER_H

#include "Mdt/ExecutableFile/FileOpenError.h"
#include "Mdt/ExecutableFile/ExecutableFileReadError.h"
#include "Mdt/ExecutableFile/ExecutableFileWriteError.h"
#include "Mdt/ExecutableFile/ExecutableFileIoEngine.h"
#include "Mdt/ExecutableFile/RPath.h"
#include "mdt_executablefilecore_export.h"
#include <QObject>
#include <QFileInfo>
#include <QStringList>

namespace Mdt{ namespace ExecutableFile{

  /*! \brief Writer to set some attributes to a executable file
   *
   * Here is a example to set the RPath to a executable
   * (if executable format supports RPath):
   * \code
   * // rpath will be $ORIGIN on Linux like systems
   * RPath rpath;
   * rpath.appendPath(".");
   *
   * ExecutableFileWriter writer;
   *
   * writer.openFile(targetLibrary);
   * writer.setRunPath(rpath);
   * writer.close();
   * \endcode
   *
   * \todo Update above doc once RPath class have been created
   */
  class MDT_EXECUTABLEFILECORE_EXPORT ExecutableFileWriter : public QObject
  {
    Q_OBJECT

   public:

    /*! \brief Construct a file writer
     */
    explicit ExecutableFileWriter(QObject *parent = nullptr);

    /*! \brief Close this file writer and free resources
     */
    ~ExecutableFileWriter() noexcept = default;

    /*! \brief Open a file
     *
     * \pre \a fileInfo must have a file path set
     * \pre this writer must not allready have a file open
     * \sa isOpen()
     * \sa close()
     * \exception FileOpenError
     */
    void openFile(const QFileInfo & fileInfo);

    /*! \brief Open a file for a expected platform
     *
     * \pre \a fileInfo must have a file path set
     * \pre \a platform must be valid
     * \pre this writer must not allready have a file open
     * \sa isOpen()
     * \sa close()
     * \exception FileOpenError
     */
    void openFile(const QFileInfo & fileInfo, const Platform & platform);

    /*! \brief Check if this writer has a open file
     *
     * \sa openFile()
     * \sa close()
     */
    bool isOpen() const noexcept;

    /*! \brief Close the file that was maybe open
     */
    void close();

    /*! \brief Check if this writer refers to a executable or a shared library
     *
     * \pre this writer must have a open file
     * \sa isOpen()
     * \exception ExecutableFileReadError
     * \sa ExecutableFileReader::isExecutableOrSharedLibrary()
     */
    bool isExecutableOrSharedLibrary();

    /*! \brief Get the run path for the file this writer refers to
     *
     * Will only return a result for executable formats that supports run path
     *
     * \pre this writer must have a open file which is a executable or a shared library
     * \sa isOpen()
     * \sa isExecutableOrSharedLibrary()
     * \exception ExecutableFileReadError
     */
    RPath getRunPath();

    /*! \brief Set the run path this writer refers to to \a rPath
     *
     * For executable that do not support RPath,
     * this method does nothing.
     *
     * \pre this writer must have a open file which is a executable or a shared library
     * \sa isOpen()
     * \sa isExecutableOrSharedLibrary()
     * \exception ExecutableFileWriteError
     */
    void setRunPath(const RPath & rPath);

   signals:

    void message(const QString & message) const;
    void verboseMessage(const QString & message) const;

   private:

    ExecutableFileIoEngine mEngine;
  };

}} // namespace Mdt{ namespace ExecutableFile{

#endif // #ifndef MDT_EXECUTABLE_FILE_EXECUTABLE_FILE_WRITER_H
