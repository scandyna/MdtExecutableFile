// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "HeaderTableGraphicsItemMapId.h"
#include "Numeric.h"
#include <QMetaType>
#include <cassert>

QVariant HeaderTableGraphicsItemMapId::toQVariant() const noexcept
{
  return QVariant( qulonglong_from_size_t(mValue) );
}

bool HeaderTableGraphicsItemMapId::isValidQVariant(const QVariant & value) noexcept
{
  const auto type = static_cast<QMetaType::Type>( value.type() );
  if(type != QMetaType::ULongLong){
    return false;
  }

  if( value.isNull() ){
    return false;
  }

  return true;
}

HeaderTableGraphicsItemMapId
HeaderTableGraphicsItemMapId::fromQVariant(const QVariant & value) noexcept
{
  assert( isValidQVariant(value) );
  assert( value.canConvert<qulonglong>() );

  return HeaderTableGraphicsItemMapId( size_t_from_qulonglong( value.toULongLong() ) );
}
