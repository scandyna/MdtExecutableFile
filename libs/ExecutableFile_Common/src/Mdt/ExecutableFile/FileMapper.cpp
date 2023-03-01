// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2021-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "FileMapper.h"
#include <cassert>

namespace Mdt{ namespace ExecutableFile{

FileMapper::FileMapper(QObject *parent)
 : QObject(parent)
{
}

ByteArraySpan FileMapper::mapIfRequired(QFileDevice & file, qint64 offset, qint64 size)
{
  assert( file.isOpen() );
  assert( offset >= 0 );
  assert( size > 0 );
  assert( file.size() >= (offset+size) );

  if( !needToRemap(offset, size) ){
    mSize = size;
    return makeSpan();
  }

  if( mCurrentMap != nullptr ){
    if( !file.unmap(mCurrentMap) ){
      const QString message = tr("could not unmap file '%1': %2")
                              .arg( file.fileName(), file.errorString() );
      throw FileOpenError(message);
    }
  }

  mCurrentMap = file.map(offset, size);
  if( mCurrentMap == nullptr ){
    const QString message = tr("could not map file '%1': %2")
                            .arg( file.fileName(), file.errorString() );
    throw FileOpenError(message);
  }
  mSize = size;
  mOffset = offset;

  return makeSpan();
}

void FileMapper::unmap(QFileDevice & file)
{
  mOffset = 0;
  mSize = 0;

  if( mCurrentMap != nullptr ){
    file.unmap(mCurrentMap);
    mCurrentMap = nullptr;
  }
}

bool FileMapper::needToRemap(qint64 offset, qint64 size) const noexcept
{
  assert( offset >= 0 );
  assert( size > 0 );

  if( offset != mOffset ){
    return true;
  }
  if( size > mSize ){
    return true;
  }

  return false;
}

ByteArraySpan FileMapper::makeSpan() const noexcept
{
  assert( mCurrentMap != nullptr );
  assert( mSize > 0 );

  ByteArraySpan map;

  map.data = mCurrentMap;
  map.size = mSize;

  return map;
}

}} // namespace Mdt{ namespace DeployUtils{
