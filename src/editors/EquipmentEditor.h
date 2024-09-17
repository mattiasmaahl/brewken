/*======================================================================================================================
 * editors/EquipmentEditor.h is part of Brewken, and is copyright the following authors 2009-2024:
 *   • David Grundberg <individ@acc.umu.se>
 *   • Jeff Bailey <skydvr38@verizon.net>
 *   • Matt Young <mfsy@yahoo.com>
 *   • Mike Evans <mikee@saxicola.co.uk>
 *   • Mik Firestone <mikfire@gmail.com>
 *   • Philip Greggory Lee <rocketman768@gmail.com>
 *
 * Brewken is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Brewken is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 =====================================================================================================================*/
#ifndef EDITORS_EQUIPMENTEDITOR_H
#define EDITORS_EQUIPMENTEDITOR_H
#pragma once

#include <QMetaProperty>
#include <QVariant>

#include "ui_equipmentEditor.h"

#include "editors/EditorBase.h"
#include "model/Equipment.h"

/*!
 * \class EquipmentEditor
 *
 * \brief View/controller class for creating and editing \c Equipment records.
 *
 *        See comment on EditorBase::connectSignalsAndSlots for why we need to have \c public, not \c private
 *        inheritance from the Ui base.
 */
class EquipmentEditor : public QDialog, public Ui::equipmentEditor, public EditorBase<EquipmentEditor, Equipment> {
   Q_OBJECT

   EDITOR_COMMON_DECL(Equipment)

public slots:
   void hideOrShowOptionalVessels();
   void updateCalcBoilVolume();
   void resetAbsorption();
   void updateDefaultEquipment();

public:
   bool validateBeforeSave();

private:
   double calcBatchSize();

};

#endif
