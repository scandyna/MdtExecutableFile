// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef TEST_BINARIES_UTILS_H
#define TEST_BINARIES_UTILS_H

#include <QString>
#include <QStringList>

/*! \internal Get the absolute path to the Qt5Core library
 */
QString qt5CoreFilePath();

/*! \internal Get the file name of the Qt5Core library
 */
QString qt5CoreFileName();

/*! \internal Return true if \a libraries contains Qt5Core shared library
 */
bool containsQt5Core(const QStringList & libraries);

/*! \internal Get the absolute path to the test shared library
 */
QString testSharedLibraryFilePath();

/*! \internal Get the file name of the test shared library
 */
QString testSharedLibraryFileName();

/*! \internal Return true if \a libraries contains the test shared library
 *
 * \sa testSharedLibraryFileName()
 */
bool containsTestSharedLibrary(const QStringList & libraries);

/*! \internal Get the absolute path to the test executable
 */
QString testExecutableFilePath();

/*! \internal Get the absolute path to the test static library
 */
QString testStaticLibraryFilePath();

#endif // #ifndef TEST_BINARIES_UTILS_H
