// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef TEST_FILE_UTILS_H
#define TEST_FILE_UTILS_H

#include <QString>
#include <QStringList>
#include <QTemporaryDir>
#include <QProcessEnvironment>
#include <QFile>

/*
 * Make a absolute path that retruns the correct result
 * on Linux and also on Windows.
 * As example: /tmp/file.txt will become C:/tmp/file.txt on Windows
 * See https://gitlab.com/scandyna/mdtdeployutils/-/jobs/1934576156
 */
QString makeAbsolutePath(const std::string & path);

QString makePath(const QString & rootPath, const char * const subPath);

QString makePath(const QTemporaryDir & dir, const char * const subPath);

bool createDirectoryFromPath(const QString & path);

bool createDirectoryFromPath(const QTemporaryDir & dir, const char * const subPath);

bool isExistingDirectory(const QString & path);

bool fileExists(const QString & filePath);

bool writeTextFileUtf8(QFile & file, const QString & content);

bool createTextFileUtf8(const QString & filePath , const QString & content);

QString readTextFileUtf8(const QString & filePath);

bool copyFile(const QString & source, const QString & destination);

bool hasExePermissions(QFile::Permissions permissions) noexcept;

void setExePermissions(QFile::Permissions & permissions) noexcept;

bool setFileExePermissionsIfRequired(const QString & filePath);

bool runExecutable( const QString & executableFilePath,
                    const QStringList & arguments = QStringList(),
                    const QProcessEnvironment & env = QProcessEnvironment::systemEnvironment() );

#endif // #ifndef TEST_FILE_UTILS_H
