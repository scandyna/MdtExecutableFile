// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_EXECUTABLE_FILE_IO_ENGINE_H
#define MDT_EXECUTABLE_FILE_EXECUTABLE_FILE_IO_ENGINE_H

#include "Mdt/ExecutableFile/FileOpenError.h"
#include "Mdt/ExecutableFile/ExecutableFileReadError.h"
#include "Mdt/ExecutableFile/ExecutableFileOpenMode.h"
#include "Mdt/ExecutableFile/Platform.h"
#include "mdt_executablefilecore_export.h"
#include <QObject>
#include <QFileInfo>
#include <memory>

namespace Mdt{ namespace ExecutableFile{

  class ExecutableFileIoEngineImplementationInterface;

  /*! \internal Helper class to instanciate a executable file I/O engine
   */
  class MDT_EXECUTABLEFILECORE_EXPORT ExecutableFileIoEngine : public QObject
  {
    Q_OBJECT

   public:

    /*! \brief Construct a file I/O engine
     */
    explicit ExecutableFileIoEngine(QObject *parent = nullptr);

    /*! \brief Close this file engine and free resources
     */
    ~ExecutableFileIoEngine() noexcept;

    /*! \brief Open a file
     *
     * \pre \a fileInfo must have a file path set
     * \pre this engine must not allready have a file open
     * \sa isOpen()
     * \sa close()
     * \exception FileOpenError
     */
    void openFile(const QFileInfo & fileInfo, ExecutableFileOpenMode mode);

    /*! \brief Open a file for a expected platform
     *
     * \pre \a fileInfo must have a file path set
     * \pre \a platform must be valid
     * \pre this engine must not allready have a file open
     * \sa isOpen()
     * \sa close()
     * \exception FileOpenError
     */
    void openFile(const QFileInfo & fileInfo, ExecutableFileOpenMode mode, const Platform & platform);

    /*! \brief Check if this engine has a open file
     *
     * \sa openFile()
     * \sa close()
     */
    bool isOpen() const noexcept;

    /*! \brief Close the file that was maybe open
     */
    void close();

    /*! \brief Get the platorm of the file this engine refers to
     *
     * \pre this engine must have a file open
     * \sa isOpen()
     * \exception ExecutableFileReadError
     */
    Platform getFilePlatform();

    /*! \brief Access the engine
     *
     * \pre this engine must have a file open
     * \sa isOpen()
     */
    std::unique_ptr<ExecutableFileIoEngineImplementationInterface> & engine() noexcept
    {
      assert( isOpen() );

      return mIoEngine;
    }

   signals:

    void message(const QString & message) const;
    void verboseMessage(const QString & message) const;

   private:

    void instanciateEngine(ExecutableFileFormat format) noexcept;

    std::unique_ptr<ExecutableFileIoEngineImplementationInterface> mIoEngine;
  };

}} // namespace Mdt{ namespace ExecutableFile{

#endif // #ifndef MDT_EXECUTABLE_FILE_EXECUTABLE_FILE_IO_ENGINE_H
