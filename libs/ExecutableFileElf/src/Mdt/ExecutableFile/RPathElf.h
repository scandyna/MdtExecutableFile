// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_EXECUTABLE_FILE_RPATH_ELF_H
#define MDT_EXECUTABLE_FILE_RPATH_ELF_H

#include "Mdt/ExecutableFile/RPath.h"
#include "Mdt/ExecutableFile/RPathFormatError.h"
// #include "mdt_deployutilscore_export.h"
#include <QString>
#include <QObject>

namespace Mdt{ namespace ExecutableFile{

  /*! \brief Helper class to convert RPath from and to ELF RPATH string
   *
   * \sa https://man7.org/linux/man-pages/man8/ld.so.8.html
   */
  class /*MDT_DEPLOYUTILSCORE_EXPORT*/ RPathElf : public QObject
  {
    Q_OBJECT

   public:

    /*! \brief Get a rpath entry from \a path
     *
     * \pre \a path must not be empty
     * \exception RPathFormatError
     */
    static
    RPathEntry rPathEntryFromString(const QString & path);

    /*! \brief Get a rpath from \a rpathString
     *
     * \exception RPathFormatError
     */
    static
    RPath rPathFromString(const QString & rpathString);

    /*! \brief Get the string version of \a rpathEntry
     */
    static
    QString rPathEntryToString(const RPathEntry & rpathEntry) noexcept;

    /*! \brief Get the string version of \a rpath
     */
    static
    QString rPathToString(const RPath & rpath) noexcept;
  };

}} // namespace Mdt{ namespace ExecutableFile{

#endif // #ifndef MDT_EXECUTABLE_FILE_RPATH_ELF_H
