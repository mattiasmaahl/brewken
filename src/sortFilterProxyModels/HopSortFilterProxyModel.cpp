/*======================================================================================================================
 * sortFilterProxyModels/HopSortFilterProxyModel.cpp is part of Brewken, and is copyright the following authors
 * 2009-2023:
 *   • Daniel Pettersson <pettson81@gmail.com>
 *   • Mattias Måhl <mattias@kejsarsten.com>
 *   • Matt Young <mfsy@yahoo.com>
 *   • Mik Firestone <mikfire@gmail.com>
 *   • Philip Greggory Lee <rocketman768@gmail.com>
 *   • Samuel Östling <MrOstling@gmail.com>
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
#include "sortFilterProxyModels/HopSortFilterProxyModel.h"

#include "measurement/Measurement.h"
#include "measurement/PhysicalQuantity.h"

bool HopSortFilterProxyModel::isLessThan(HopTableModel::ColumnIndex const columnIndex,
                                         QVariant const & leftItem,
                                         QVariant const & rightItem) const {
   switch (columnIndex) {
      case HopTableModel::ColumnIndex::Name:
      case HopTableModel::ColumnIndex::Form:
      case HopTableModel::ColumnIndex::Year:
      // TODO Work out how to reinstate inventory
//      case HopTableModel::ColumnIndex::IsWeight:
         return leftItem.toString() < rightItem.toString();

      case HopTableModel::ColumnIndex::Alpha:
         return Measurement::extractRawFromString<double>( leftItem.toString()) <
                Measurement::extractRawFromString<double>(rightItem.toString());

      // TODO Work out how to reinstate inventory
//      case HopTableModel::ColumnIndex::Inventory:
//         return Measurement::qStringToSI( leftItem.toString(), Measurement::PhysicalQuantity::Mass) <
//                Measurement::qStringToSI(rightItem.toString(), Measurement::PhysicalQuantity::Mass);

      // No default case as we want the compiler to warn us if we missed one
   }

   // Should be unreachable
   Q_ASSERT(false);
   return true;
}

// Insert the boiler-plate stuff that we cannot do in SortFilterProxyModelBase
SORT_FILTER_PROXY_MODEL_COMMON_CODE(Hop)
