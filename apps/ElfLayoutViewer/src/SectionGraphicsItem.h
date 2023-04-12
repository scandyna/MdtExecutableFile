// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef SECTION_GRAPHICS_ITEM_H
#define SECTION_GRAPHICS_ITEM_H

#include <QGraphicsItemGroup>
#include <QString>

#include <cstdint>
#include <string>

/*! \brief
 *
 * \todo give a correct name, make correct API !!
 */
struct SecSegGraphicsItemData
{
  uint64_t offset = 0;
  uint64_t size = 100;
  std::string name;
};

/*! \brief
 */
class SectionGraphicsItemData
{
 public:

  void setOffset();

  void setSize();

  static
  SectionGraphicsItemData fromSectionHeader();
};

/*! \brief Represents a ELF section as a Qt graphics item
 */
class SectionGraphicsItem : public QGraphicsItemGroup
{
 public:

  /*! \brief Constructor
   */
  explicit SectionGraphicsItem(SecSegGraphicsItemData data, QGraphicsItem *parent = nullptr);

 private:

  void createLeftLabel(const QString & text) noexcept;
  void createCenterLabel(const QString & text) noexcept;
  void createRightLabel(const QString & text) noexcept;
};


/*! \brief
 *
 * \todo give a correct name, make correct API !
 *
 * \a X is responsible of adding sections and segments to the layout scene.
 */
class X
{
 public:

  void addSection();

  void addSegment();
};

#endif // #ifndef SECTION_GRAPHICS_ITEM_H
