/*======================================================================================================================
 * editors/FermentableEditor.h is part of Brewken, and is copyright the following authors 2009-2023:
 *   • Brian Rower <brian.rower@gmail.com>
 *   • Jeff Bailey <skydvr38@verizon.net>
 *   • Matt Young <mfsy@yahoo.com>
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
#ifndef EDITORS_FERMENTABLEEDITOR_H
#define EDITORS_FERMENTABLEEDITOR_H
#pragma once

#include "ui_fermentableEditor.h"

#include <QDialog>
#include <QMetaProperty>
#include <QString>

#include "editors/EditorBase.h"

// Forward declarations.
class Fermentable;

/*!
 * \class FermentableEditor
 *
 * \brief View/controller class for creating and editing Fermentables.
 */
class FermentableEditor : public QDialog, private Ui::fermentableEditor, public EditorBase<Fermentable, FermentableEditor> {
   Q_OBJECT

public:
   FermentableEditor(QWidget *parent=nullptr);
   virtual ~FermentableEditor();

   void writeFieldsToEditItem();
   void writeLateFieldsToEditItem();
   void readFieldsFromEditItem(std::optional<QString> propName);

public slots:
   // Standard editor slots
   void save();
   void clearAndClose();
   void changed(QMetaProperty, QVariant);
   void clickedNew();
   // Extra slots
   void setIsWeight(bool state);
};

#endif
