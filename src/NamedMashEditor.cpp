/*======================================================================================================================
 * NamedMashEditor.cpp is part of Brewken, and is copyright the following authors 2009-2023:
 *   • Brian Rower <brian.rower@gmail.com>
 *   • Daniel Moreno <danielm5@users.noreply.github.com>
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
#include "NamedMashEditor.h"

#include <QDebug>
#include <QWidget>

#include "database/ObjectStoreWrapper.h"
#include "measurement/Unit.h"
#include "model/Equipment.h"
#include "model/Mash.h"
#include "model/Recipe.h"


NamedMashEditor::NamedMashEditor(QWidget* parent, MashStepEditor* editor, bool singleMashEditor) :
   QDialog{parent},
   mashObs{nullptr} {
   setupUi(this);

   if (singleMashEditor) {
      for (int i = 0; i < horizontalLayout_mashs->count(); ++i) {
         QWidget* w = horizontalLayout_mashs->itemAt(i)->widget();
         if (w) {
            w->setVisible(false);
         }
      }
      // pushButton_new->setVisible(false);
   }

   //! Create the list model and assign it to the combo box
   mashListModel = new MashListModel(mashComboBox);
   mashComboBox->setModel( mashListModel );

   //! Create the table model (and may St. Stevens take pity)
   mashStepTableModel = new MashStepTableModel(mashStepTableWidget);
   mashStepTableWidget->setItemDelegate(new MashStepItemDelegate());
   mashStepTableWidget->setModel(mashStepTableModel);

   //! Preserve the step editor
   mashStepEditor = editor;

   //! And do some fun stuff with the equipment
   equipListModel = new EquipmentListModel(equipmentComboBox);
   equipmentComboBox->setModel(equipListModel);
   connect(equipmentComboBox, SIGNAL(activated(const QString&)), this, SLOT(fromEquipment(const QString&)));

   // ok and cancel buttons
   connect(pushButton_save,           &QAbstractButton::clicked, this, &NamedMashEditor::saveAndClose );
   connect(pushButton_cancel,         &QAbstractButton::clicked, this, &NamedMashEditor::closeEditor );
   // new mash step, delete mash step, move mash step up and down
   connect(pushButton_addMashStep,    &QAbstractButton::clicked, this, &NamedMashEditor::addMashStep);
   connect(pushButton_removeMashStep, &QAbstractButton::clicked, this, &NamedMashEditor::removeMashStep);
   connect(pushButton_mashUp,         &QAbstractButton::clicked, this, &NamedMashEditor::moveMashStepUp);
   connect(pushButton_mashDown,       &QAbstractButton::clicked, this, &NamedMashEditor::moveMashStepDown);
   // finally, the combo box and the remove mash button
   connect(mashComboBox, SIGNAL(activated(const QString&)), this, SLOT(mashSelected(const QString&)));
   connect(pushButton_remove, &QAbstractButton::clicked, this, &NamedMashEditor::removeMash);

   this->setMash(mashListModel->at(mashComboBox->currentIndex()));
   return;
}

void NamedMashEditor::showEditor() {
   this->showChanges();
   this->setVisible(true);
   return;
}

void NamedMashEditor::closeEditor() {
   this->setVisible(false);
   return;
}

void NamedMashEditor::saveAndClose() {
   if (this->mashObs == nullptr) {
      return;
   }

   qDebug() << Q_FUNC_INFO << "Saving mash (#" << this->mashObs->key() << ")";

   // using toCanonical aon the spargePh is something of a cheat, but the btLineEdit
   // class will do the right thing. That is how a plan comes together.

   this->mashObs->setEquipAdjust(true); // BeerXML won't like me, but it's just stupid not to adjust for the equipment when you're able.
   this->mashObs->setName(lineEdit_name->text());
   this->mashObs->setGrainTemp_c(lineEdit_grainTemp->toCanonical().quantity());
   this->mashObs->setSpargeTemp_c(lineEdit_spargeTemp->toCanonical().quantity());
   this->mashObs->setPh(lineEdit_spargePh->toCanonical().quantity());
   this->mashObs->setTunTemp_c(lineEdit_tunTemp->toCanonical().quantity());
   this->mashObs->setTunWeight_kg(lineEdit_tunMass->toCanonical().quantity());
   this->mashObs->setTunSpecificHeat_calGC(lineEdit_tunSpHeat->toCanonical().quantity());

   this->mashObs->setNotes( textEdit_notes->toPlainText() );
   return;
}

void NamedMashEditor::setMash(Mash* mash) {
   if( mashObs ) {
      disconnect( mashObs, 0, this, 0 );
   }

   mashObs = mash;
   mashStepTableModel->setMash(mashObs);

   if (mashObs) {
      connect( mashObs, &NamedEntity::changed, this, &NamedMashEditor::changed );
      showChanges();
   }
}

void NamedMashEditor::changed(QMetaProperty prop, QVariant /*val*/) {
   if (sender() == mashObs) {
      showChanges(&prop);
   }
   return;
}

void NamedMashEditor::showChanges(QMetaProperty* prop) {
   bool updateAll = false;
   QString propName;

   if (this->mashObs == nullptr) {
      this->clear();
      return;
   }

   if (prop == nullptr) {
      updateAll = true;
   } else {
      propName = prop->name();
   }
   qDebug() << Q_FUNC_INFO << "Updating" << (updateAll ? "all" : "property") << propName;

   if( propName == PropertyNames::NamedEntity::name || updateAll ) {
      lineEdit_name->setText(mashObs->name());
      if( ! updateAll )
         return;
   }
   if( propName == PropertyNames::Mash::grainTemp_c || updateAll ) {
      lineEdit_grainTemp->setText(mashObs);
      if( ! updateAll )
         return;
   }
   if( propName == PropertyNames::Mash::spargeTemp_c || updateAll ) {
      lineEdit_spargeTemp->setText(mashObs);
      if( ! updateAll )
         return;
   }
   if( propName == PropertyNames::Mash::ph || updateAll ) {
      lineEdit_spargePh->setText(mashObs);
      if( ! updateAll )
         return;
   }
   if( propName == PropertyNames::Mash::tunTemp_c || updateAll ) {
      lineEdit_tunTemp->setText(mashObs);
      if( ! updateAll )
         return;
   }
   if( propName == PropertyNames::Mash::tunWeight_kg || updateAll ) {
      lineEdit_tunMass->setText(mashObs);
      if( ! updateAll )
         return;
   }
   if( propName == PropertyNames::Mash::tunSpecificHeat_calGC || updateAll ) {
      lineEdit_tunSpHeat->setText(mashObs);
      if( ! updateAll )
         return;
   }
   if( propName == PropertyNames::Mash::notes || updateAll ) {
      textEdit_notes->setPlainText(mashObs->notes());
      if( ! updateAll )
         return;
   }
}

void NamedMashEditor::clear() {
   lineEdit_name->setText(QString(""));
   lineEdit_grainTemp->setText(QString(""));
   lineEdit_spargeTemp->setText(QString(""));
   lineEdit_spargePh->setText(QString(""));
   lineEdit_tunTemp->setText(QString(""));
   lineEdit_tunMass->setText(QString(""));
   lineEdit_tunSpHeat->setText(QString(""));

   textEdit_notes->setPlainText("");
   return;
}

void NamedMashEditor::addMashStep() {
   if (!this->mashObs) {
      return;
   }

   // The call to Mash::addMashStep() will also store the MashStep in the ObjectStore / DB
   auto step = std::make_shared<MashStep>();
   this->mashObs->addMashStep(step);
   mashStepEditor->setMashStep(step);
   mashStepEditor->setVisible(true);
   return;
}

bool NamedMashEditor::justOne(QModelIndexList selected) {
   int size = selected.size();
   if ( ! size ) {
      return false;
   }

   int row = selected[0].row();
   for (int ii = 1; ii < size; ++ii) {
      if (selected[ii].row() != row) {
         return false;
      }
   }
   return true;
}

void NamedMashEditor::removeMashStep() {
   if ( ! mashObs ) {
      return;
   }

   QModelIndexList selected = mashStepTableWidget->selectionModel()->selectedIndexes();
   if ( !justOne(selected) ) {
      return;
   }

   auto step = mashStepTableModel->getRow(selected[0].row());
   this->mashObs->removeMashStep(step);
   return;
}

void NamedMashEditor::moveMashStepUp()
{
   QModelIndexList selected = mashStepTableWidget->selectionModel()->selectedIndexes();
   if (selected.isEmpty())
   {  //nothing selected
      return;
   }

   int row = selected[0].row();

   if ( ! justOne(selected) || row < 1)
      return;

   mashStepTableModel->moveStepUp(row);
   return;
}

void NamedMashEditor::moveMashStepDown()
{
   QModelIndexList selected = mashStepTableWidget->selectionModel()->selectedIndexes();
   if (selected.isEmpty())
   {  //nothing selected
      return;
   }

   int row = selected[0].row();

   if ( ! justOne(selected) || row >= mashStepTableModel->rowCount()-1 )
      return;

   mashStepTableModel->moveStepDown(row);
   return;
}

void NamedMashEditor::mashSelected([[maybe_unused]] QString const & name) {
   Mash* selected = mashListModel->at(mashComboBox->currentIndex());
   if (selected && selected != mashObs)
      setMash(selected);
}

void NamedMashEditor::fromEquipment([[maybe_unused]] QString const & name) {
   if( mashObs == 0 )
      return;
   Equipment* selected = equipListModel->at(equipmentComboBox->currentIndex());

   if ( selected )
   {
      lineEdit_tunMass->setText(selected);
      lineEdit_tunSpHeat->setText(selected);
   }
}

void NamedMashEditor::removeMash() {
   if (!this->mashObs) {
      return;
   }

   int newMash = this->mashComboBox->currentIndex() - 1;

   // I *think* we want to disconnect the mash first?
   disconnect(this->mashObs, 0, this, 0);
   // Delete the mashsteps
   // .:TBD:. Mash should be responsible for deleting its steps.  This is already correctly handled for hard delete, but
   // not for soft delete.
   for (auto step : this->mashObs->mashSteps()) {
      ObjectStoreWrapper::softDelete(*step);
   }
   // Delete the mash itself
   ObjectStoreWrapper::softDelete(*this->mashObs);

   this->setMash(this->mashListModel->at(newMash));
   return;
}
