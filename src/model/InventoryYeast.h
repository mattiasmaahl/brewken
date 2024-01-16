/*======================================================================================================================
 * model/InventoryHop.h is part of Brewken, and is copyright the following authors 2023:
 *   • Matt Young <mfsy@yahoo.com>
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
#ifndef MODEL_INVENTORYHOP_H
#define MODEL_INVENTORYHOP_H
#pragma once

#include <QObject>
#include <QString>

#include "model/Hop.h"
#include "model/Inventory.h"
#include "model/IngredientAmount.h"

/**
 * \brief Inventory of \c Hop
 */
class InventoryHop : public Inventory, public IngredientAmount<InventoryHop, Hop> {
   Q_OBJECT

   INGREDIENT_AMOUNT_DECL(InventoryHop, Hop)

public:
   /**
    * \brief See comment in model/NamedEntity.h
    */
   static QString const LocalisedName;

   /**
    * \brief Mapping of names to types for the Qt properties of this class.  See \c NamedEntity::typeLookup for more
    *        info.
    */
   static TypeLookup const typeLookup;

   InventoryHop();
   InventoryHop(NamedParameterBundle const & namedParameterBundle);
   InventoryHop(InventoryHop const & other);

   virtual ~InventoryHop();

public:
   virtual char const * getIngredientClass() const;
   Hop * hop() const ;

protected:
   virtual bool isEqualTo(NamedEntity const & other) const;
   virtual ObjectStore & getObjectStoreTypedInstance() const;

};

#endif
