// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef HEADER_TABLE_GRAPHICS_ITEM_MAP_H
#define HEADER_TABLE_GRAPHICS_ITEM_MAP_H

#include "HeaderTableGraphicsItemMapId.h"
#include "SectionGraphicsItem.h"
#include <vector>

/*! \brief Mapping between header tables and layout view items
 *
 * When the user selects a section in the section header table
 * or a segment in the program header table,
 * we want to highlight it in the layout view.
 *
 * To achieve this, we have to find the graphics item in the layout view
 * that corresponds to the selected item in the header table view.
 *
 * Each time a new item is added to the map, a ID is returned.
 * This ID is simply the index in the map
 * (internally, the map is implemented as a vector).
 *
 * The returned ID has to be stored in the corresponding table model as user data.
 *
 *
 * \section Rationale Rationale
 *
 * Here are some ideas  explored to solve the problem.
 *
 * Should we index by name ?
 * This will not work for segments, because they don't have names,
 * but types. It is common, as example, to have many LOAD segments.
 * (Also, this could maybe be a bit slow).
 *
 * Should we use model indexes ?
 * This seems complex and confusing when using sort proxy models.
 *
 * Should we index by offset ?
 * - does not work for segments (many ones could start from the same offset)
 * - we want to be able to display corrupted files (having f.ex. 2 sections starting from same offset
 */
class HeaderTableGraphicsItemMap
{
 public:

  /*! \brief Register given item
   *
   * \pre \a item must be a valid pointer
   */
  HeaderTableGraphicsItemMapId registerItem(LayoutGraphicsItem *item) noexcept;

  /*! \brief Chek if given id exists in this map
   */
  bool containsId(HeaderTableGraphicsItemMapId id) const noexcept;

  /*! \brief Get the item for given id
   *
   * \pre id must exist in this map
   * \sa containsId()
   */
  LayoutGraphicsItem *itemForId(HeaderTableGraphicsItemMapId id) const noexcept;

 private:

  std::vector<LayoutGraphicsItem*> mList;
};

#endif // #ifndef HEADER_TABLE_GRAPHICS_ITEM_MAP_H
