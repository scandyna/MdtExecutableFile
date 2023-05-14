// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtExecutableFile
 ** C++ library to help reading and partially editing some binary files like ELF and Pe.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef LAYOUT_VIEW_GRAPHICS_SCENE_H
#define LAYOUT_VIEW_GRAPHICS_SCENE_H

#include "SectionGraphicsItem.h"
#include "SegmentGraphicsItem.h"
#include "LayoutViewSegmentLayout.h"
#include "Mdt/ExecutableFile/Elf/SectionHeader.h"
#include "Mdt/ExecutableFile/Elf/ProgramHeader.h"
#include <QGraphicsScene>


/*! \brief Layout view graphics scene
 *
 * Handle the layout view graphics scene.
 *
 * Handle position of each item in the scene
 *
 * \todo should probably be renamed CreateLayoutViewGraphicsScene
 * It should not hold the scene, but reference it.
 * The lifetime of this class is only to create the scene,
 * then it can disapear.
 */
class LayoutViewGraphicsScene
{
 public:

  /*! \brief Create and add an section item for given header
   */
  SectionGraphicsItem *addSection(const Mdt::ExecutableFile::Elf::SectionHeader & header) noexcept;

  /*! \brief Get the height of the sections area
   */
  qreal sectionsAreaHeight() const noexcept
  {
    return mSectionsAreaHeight;
  }

  /*! \brief Add given segment to this scene
   *
   * \note Segments should only be added after all sections have been added.
   * This is because the x position of the segments is determined by the sections.
   * (dynamic change is not supported).
   *
   * \sa sectionsAreaHeight()
   */
  SegmentGraphicsItem *addSegment(const Mdt::ExecutableFile::Elf::ProgramHeader & header) noexcept;

  /*! \brief Clear this scene
   */
  void clear() noexcept;

  /*! \brief Returns a pointer to the scene
   *
   * \warning The returned pointer is only valid for the lifetime of this scene
   */
  QGraphicsScene *scene() noexcept
  {
    return &mScene;
  }

 private:

  void updateSectionsAreaHeight(const SectionGraphicsItem & item) noexcept;

  qreal mSectionsAreaHeight = 0.0;
  LayoutViewSegmentLayout mSegmentLayout;
  QGraphicsScene mScene;
};

#endif // #ifndef LAYOUT_VIEW_GRAPHICS_SCENE_H
