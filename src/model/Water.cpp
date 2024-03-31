/*======================================================================================================================
 * model/Water.cpp is part of Brewken, and is copyright the following authors 2009-2024:
 *   • Brian Rower <brian.rower@gmail.com>
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
#include "model/Water.h"

#include "database/ObjectStoreWrapper.h"
#include "model/NamedParameterBundle.h"
#include "model/Recipe.h"

QString const Water::LocalisedName = tr("Water");

EnumStringMapping const Water::typeStringMapping {
   {Water::Type::Base  , "base"  },
   {Water::Type::Target, "target"},
};

EnumStringMapping const Water::typeDisplayNames {
   {Water::Type::Base  , tr("Base"  )},
   {Water::Type::Target, tr("Target")},
};

EnumStringMapping const Water::ionStringMapping {
   {Water::Ion::Ca  , "Ca"  },
   {Water::Ion::Cl  , "Cl"  },
   {Water::Ion::HCO3, "HCO3"},
   {Water::Ion::Mg  , "Mg"  },
   {Water::Ion::Na  , "Na"  },
   {Water::Ion::SO4 , "SO4" },
};

// Not sure there is really anything to translate here!
// 2023-06-01: MY: I tried HCO₃ and SO₄ as display names, but the unicode subscript numbers seemed somewhat too small in
//                 the fonts I use.  Nonetheless, I am open to persuasion on this if others feel strongly.
EnumStringMapping const Water::ionDisplayNames {
   {Water::Ion::Ca  , tr("Ca  ")},
   {Water::Ion::Cl  , tr("Cl  ")},
   {Water::Ion::HCO3, tr("HCO3")},
   {Water::Ion::Mg  , tr("Mg  ")},
   {Water::Ion::Na  , tr("Na  ")},
   {Water::Ion::SO4 , tr("SO4 ")},
};

bool Water::isEqualTo(NamedEntity const & other) const {
   // Base class (NamedEntity) will have ensured this cast is valid
   Water const & rhs = static_cast<Water const &>(other);
   // Base class will already have ensured names are equal
   bool const outlinesAreEqual{
      // "Outline" fields: In BeerJSON, all these fields are in the FermentableBase type
      this->m_calcium_ppm      == rhs.m_calcium_ppm      &&
      this->m_bicarbonate_ppm  == rhs.m_bicarbonate_ppm  &&
      this->m_carbonate_ppm    == rhs.m_carbonate_ppm    &&
      this->m_potassium_ppm    == rhs.m_potassium_ppm    &&
      this->m_iron_ppm         == rhs.m_iron_ppm         &&
      this->m_nitrate_ppm      == rhs.m_nitrate_ppm      &&
      this->m_nitrite_ppm      == rhs.m_nitrite_ppm      &&
      this->m_flouride_ppm     == rhs.m_flouride_ppm     &&
      this->m_sulfate_ppm      == rhs.m_sulfate_ppm      &&
      this->m_chloride_ppm     == rhs.m_chloride_ppm     &&
      this->m_sodium_ppm       == rhs.m_sodium_ppm       &&
      this->m_magnesium_ppm    == rhs.m_magnesium_ppm
   };

   // If either object is an outline (see comment in model/OutlineableNamedEntity.h) then there is no point comparing
   // any more fields.  Note that an object will only be an outline whilst it is being read in from a BeerJSON file.
   if (this->m_outline || rhs.m_outline) {
      return outlinesAreEqual;
   }

   return (
      outlinesAreEqual &&
      // Remaining BeerJSON fields
      this->m_ph    == rhs.m_ph    &&
      this->m_notes == rhs.m_notes
   );
}

ObjectStore & Water::getObjectStoreTypedInstance() const {
   return ObjectStoreTyped<Water>::getInstance();
}

TypeLookup const Water::typeLookup {
   "Water",
   {
///      PROPERTY_TYPE_LOOKUP_ENTRY(PropertyNames::Water::amount          , Water::m_amount            , Measurement::PhysicalQuantity::Volume             ),
      PROPERTY_TYPE_LOOKUP_ENTRY(PropertyNames::Water::calcium_ppm     , Water::m_calcium_ppm       , Measurement::PhysicalQuantity::MassFractionOrConc),
      PROPERTY_TYPE_LOOKUP_ENTRY(PropertyNames::Water::bicarbonate_ppm , Water::m_bicarbonate_ppm   , Measurement::PhysicalQuantity::MassFractionOrConc),
      PROPERTY_TYPE_LOOKUP_ENTRY(PropertyNames::Water::sulfate_ppm     , Water::m_sulfate_ppm       , Measurement::PhysicalQuantity::MassFractionOrConc),
      PROPERTY_TYPE_LOOKUP_ENTRY(PropertyNames::Water::chloride_ppm    , Water::m_chloride_ppm      , Measurement::PhysicalQuantity::MassFractionOrConc),
      PROPERTY_TYPE_LOOKUP_ENTRY(PropertyNames::Water::sodium_ppm      , Water::m_sodium_ppm        , Measurement::PhysicalQuantity::MassFractionOrConc),
      PROPERTY_TYPE_LOOKUP_ENTRY(PropertyNames::Water::magnesium_ppm   , Water::m_magnesium_ppm     , Measurement::PhysicalQuantity::MassFractionOrConc),
      PROPERTY_TYPE_LOOKUP_ENTRY(PropertyNames::Water::ph              , Water::m_ph                , Measurement::PhysicalQuantity::Acidity           ),
      PROPERTY_TYPE_LOOKUP_ENTRY(PropertyNames::Water::alkalinity_ppm  , Water::m_alkalinity_ppm    , Measurement::PhysicalQuantity::MassFractionOrConc),
      PROPERTY_TYPE_LOOKUP_ENTRY(PropertyNames::Water::notes           , Water::m_notes             ,           NonPhysicalQuantity::String            ),
      PROPERTY_TYPE_LOOKUP_ENTRY(PropertyNames::Water::type            , Water::m_type              ,           NonPhysicalQuantity::Enum              ),
      PROPERTY_TYPE_LOOKUP_ENTRY(PropertyNames::Water::mashRo_pct      , Water::m_mashRo_pct        ,           NonPhysicalQuantity::Percentage        ),
      PROPERTY_TYPE_LOOKUP_ENTRY(PropertyNames::Water::spargeRo_pct    , Water::m_spargeRo_pct      ,           NonPhysicalQuantity::Percentage        ),
      PROPERTY_TYPE_LOOKUP_ENTRY(PropertyNames::Water::alkalinityAsHCO3, Water::m_alkalinity_as_hco3,           NonPhysicalQuantity::Bool              ),
      // ⮜⮜⮜ All below added for BeerJSON support ⮞⮞⮞
      PROPERTY_TYPE_LOOKUP_ENTRY(PropertyNames::Water::carbonate_ppm   , Water::m_carbonate_ppm     , Measurement::PhysicalQuantity::MassFractionOrConc),
      PROPERTY_TYPE_LOOKUP_ENTRY(PropertyNames::Water::potassium_ppm   , Water::m_potassium_ppm     , Measurement::PhysicalQuantity::MassFractionOrConc),
      PROPERTY_TYPE_LOOKUP_ENTRY(PropertyNames::Water::iron_ppm        , Water::m_iron_ppm          , Measurement::PhysicalQuantity::MassFractionOrConc),
      PROPERTY_TYPE_LOOKUP_ENTRY(PropertyNames::Water::nitrate_ppm     , Water::m_nitrate_ppm       , Measurement::PhysicalQuantity::MassFractionOrConc),
      PROPERTY_TYPE_LOOKUP_ENTRY(PropertyNames::Water::nitrite_ppm     , Water::m_nitrite_ppm       , Measurement::PhysicalQuantity::MassFractionOrConc),
      PROPERTY_TYPE_LOOKUP_ENTRY(PropertyNames::Water::flouride_ppm    , Water::m_flouride_ppm      , Measurement::PhysicalQuantity::MassFractionOrConc),
   },
   // Parent classes lookup
   {&OutlineableNamedEntity::typeLookup,
    std::addressof(FolderBase<Water>::typeLookup)}
};
static_assert(std::is_base_of<FolderBase<Water>, Water>::value);

Water::Water(QString name) :
   OutlineableNamedEntity{name},
   FolderBase<Water>{},
///   m_amount             {0.0         },
   m_calcium_ppm        {0.0         },
   m_bicarbonate_ppm    {0.0         },
   m_sulfate_ppm        {0.0         },
   m_chloride_ppm       {0.0         },
   m_sodium_ppm         {0.0         },
   m_magnesium_ppm      {0.0         },
   m_ph                 {0.0         },
   m_alkalinity_ppm     {0.0         },
   m_notes              {""          },
   m_type               {std::nullopt},
   m_mashRo_pct         {0.0         },
   m_spargeRo_pct       {0.0         },
   m_alkalinity_as_hco3 {true        },
   // ⮜⮜⮜ All below added for BeerJSON support ⮞⮞⮞
   m_carbonate_ppm      {std::nullopt},
   m_potassium_ppm      {std::nullopt},
   m_iron_ppm           {std::nullopt},
   m_nitrate_ppm        {std::nullopt},
   m_nitrite_ppm        {std::nullopt},
   m_flouride_ppm       {std::nullopt} {
   return;
}

Water::Water(NamedParameterBundle const & namedParameterBundle) :
   OutlineableNamedEntity{namedParameterBundle},
   FolderBase<Water>{namedParameterBundle},
///   SET_REGULAR_FROM_NPB (m_amount            , namedParameterBundle, PropertyNames::Water::amount          ),
   SET_REGULAR_FROM_NPB (m_calcium_ppm       , namedParameterBundle, PropertyNames::Water::calcium_ppm     ),
   SET_REGULAR_FROM_NPB (m_bicarbonate_ppm   , namedParameterBundle, PropertyNames::Water::bicarbonate_ppm ),
   SET_REGULAR_FROM_NPB (m_sulfate_ppm       , namedParameterBundle, PropertyNames::Water::sulfate_ppm     ),
   SET_REGULAR_FROM_NPB (m_chloride_ppm      , namedParameterBundle, PropertyNames::Water::chloride_ppm    ),
   SET_REGULAR_FROM_NPB (m_sodium_ppm        , namedParameterBundle, PropertyNames::Water::sodium_ppm      ),
   SET_REGULAR_FROM_NPB (m_magnesium_ppm     , namedParameterBundle, PropertyNames::Water::magnesium_ppm   ),
   SET_REGULAR_FROM_NPB (m_ph                , namedParameterBundle, PropertyNames::Water::ph              ),
   SET_REGULAR_FROM_NPB (m_alkalinity_ppm    , namedParameterBundle, PropertyNames::Water::alkalinity_ppm  ),
   SET_REGULAR_FROM_NPB (m_notes             , namedParameterBundle, PropertyNames::Water::notes           ),
   SET_OPT_ENUM_FROM_NPB(m_type , Water::Type, namedParameterBundle, PropertyNames::Water::type            ),
   SET_REGULAR_FROM_NPB (m_mashRo_pct        , namedParameterBundle, PropertyNames::Water::mashRo_pct      ),
   SET_REGULAR_FROM_NPB (m_spargeRo_pct      , namedParameterBundle, PropertyNames::Water::spargeRo_pct    ),
   SET_REGULAR_FROM_NPB (m_alkalinity_as_hco3, namedParameterBundle, PropertyNames::Water::alkalinityAsHCO3),
   // ⮜⮜⮜ All below added for BeerJSON support ⮞⮞⮞
   SET_REGULAR_FROM_NPB (m_carbonate_ppm     , namedParameterBundle, PropertyNames::Water::carbonate_ppm   ),
   SET_REGULAR_FROM_NPB (m_potassium_ppm     , namedParameterBundle, PropertyNames::Water::potassium_ppm   ),
   SET_REGULAR_FROM_NPB (m_iron_ppm          , namedParameterBundle, PropertyNames::Water::iron_ppm        ),
   SET_REGULAR_FROM_NPB (m_nitrate_ppm       , namedParameterBundle, PropertyNames::Water::nitrate_ppm     ),
   SET_REGULAR_FROM_NPB (m_nitrite_ppm       , namedParameterBundle, PropertyNames::Water::nitrite_ppm     ),
   SET_REGULAR_FROM_NPB (m_flouride_ppm      , namedParameterBundle, PropertyNames::Water::flouride_ppm    ) {
   return;
}

Water::Water(Water const& other) :
   OutlineableNamedEntity{other},
   FolderBase<Water>{other},
///   m_amount             {other.m_amount            },
   m_calcium_ppm        {other.m_calcium_ppm       },
   m_bicarbonate_ppm    {other.m_bicarbonate_ppm   },
   m_sulfate_ppm        {other.m_sulfate_ppm       },
   m_chloride_ppm       {other.m_chloride_ppm      },
   m_sodium_ppm         {other.m_sodium_ppm        },
   m_magnesium_ppm      {other.m_magnesium_ppm     },
   m_ph                 {other.m_ph                },
   m_alkalinity_ppm     {other.m_alkalinity_ppm    },
   m_notes              {other.m_notes             },
   m_type               {other.m_type              },
   m_mashRo_pct         {other.m_mashRo_pct        },
   m_spargeRo_pct       {other.m_spargeRo_pct      },
   m_alkalinity_as_hco3 {other.m_alkalinity_as_hco3},
   // ⮜⮜⮜ All below added for BeerJSON support ⮞⮞⮞
   m_carbonate_ppm      {other.m_carbonate_ppm     },
   m_potassium_ppm      {other.m_potassium_ppm     },
   m_iron_ppm           {other.m_iron_ppm          },
   m_nitrate_ppm        {other.m_nitrate_ppm       },
   m_nitrite_ppm        {other.m_nitrite_ppm       },
   m_flouride_ppm       {other.m_flouride_ppm      } {
   return;
}

void Water::swap(Water & other) noexcept {
   this->NamedEntity::swap(other);
///   std::swap(this->m_amount            , other.m_amount            );
   std::swap(this->m_calcium_ppm       , other.m_calcium_ppm       );
   std::swap(this->m_bicarbonate_ppm   , other.m_bicarbonate_ppm   );
   std::swap(this->m_sulfate_ppm       , other.m_sulfate_ppm       );
   std::swap(this->m_chloride_ppm      , other.m_chloride_ppm      );
   std::swap(this->m_sodium_ppm        , other.m_sodium_ppm        );
   std::swap(this->m_magnesium_ppm     , other.m_magnesium_ppm     );
   std::swap(this->m_ph                , other.m_ph                );
   std::swap(this->m_alkalinity_ppm    , other.m_alkalinity_ppm    );
   std::swap(this->m_notes             , other.m_notes             );
   std::swap(this->m_type              , other.m_type              );
   std::swap(this->m_mashRo_pct        , other.m_mashRo_pct        );
   std::swap(this->m_spargeRo_pct      , other.m_spargeRo_pct      );
   std::swap(this->m_alkalinity_as_hco3, other.m_alkalinity_as_hco3);
   // ⮜⮜⮜ All below added for BeerJSON support ⮞⮞⮞
   std::swap(this->m_carbonate_ppm     , other.m_carbonate_ppm     );
   std::swap(this->m_potassium_ppm     , other.m_potassium_ppm     );
   std::swap(this->m_iron_ppm          , other.m_iron_ppm          );
   std::swap(this->m_nitrate_ppm       , other.m_nitrate_ppm       );
   std::swap(this->m_nitrite_ppm       , other.m_nitrite_ppm       );
   std::swap(this->m_flouride_ppm      , other.m_flouride_ppm      );
   return;
}

Water::~Water() = default;
/*Water::~Water() {
   qDebug() <<
      Q_FUNC_INFO << "Deleting Water #" << this->key() << ":" << this->name() << "@" << static_cast<void *>(this);
   ObjectStoreWrapper::logDiagnostics<Water>();
   return;
}*/

Water & Water::operator=(Water other) {
   // Per https://en.wikibooks.org/wiki/More_C++_Idioms/Copy-and-swap and other places, the safest way to do assignment
   // is via the copy-and-swap idiom

   // I think it's a coding error if we're trying to assign to ourselves
   Q_ASSERT(this != &other);

   this->swap(other);

   // Using swap means we have bypassed all the magic of setAndNotify.  So we need to do a couple of things here:
   //   - if we are already stored in the DB then we need to update the data there
   //   - we need to issue the notifications for properties that changed as a result of the assignment
   if (this->key() > 0) {
      // We have to be careful not to create a new shared pointer for the object, but instead to get a copy of the one
      // held by the object store.
      qDebug() <<
         Q_FUNC_INFO << "After assignment, updating Water #" << this->key() << "(" << this->name() << ") @" <<
         static_cast<void *>(this) << "in DB";
      ObjectStoreWrapper::update(*this);
   }
///   if (this->m_amount             != other.m_amount            ) { this->propagatePropertyChange(PropertyNames::Water::amount          ); }
   if (this->m_calcium_ppm        != other.m_calcium_ppm       ) { this->propagatePropertyChange(PropertyNames::Water::calcium_ppm     ); }
   if (this->m_bicarbonate_ppm    != other.m_bicarbonate_ppm   ) { this->propagatePropertyChange(PropertyNames::Water::bicarbonate_ppm ); }
   if (this->m_sulfate_ppm        != other.m_sulfate_ppm       ) { this->propagatePropertyChange(PropertyNames::Water::sulfate_ppm     ); }
   if (this->m_chloride_ppm       != other.m_chloride_ppm      ) { this->propagatePropertyChange(PropertyNames::Water::chloride_ppm    ); }
   if (this->m_sodium_ppm         != other.m_sodium_ppm        ) { this->propagatePropertyChange(PropertyNames::Water::sodium_ppm      ); }
   if (this->m_magnesium_ppm      != other.m_magnesium_ppm     ) { this->propagatePropertyChange(PropertyNames::Water::magnesium_ppm   ); }
   if (this->m_ph                 != other.m_ph                ) { this->propagatePropertyChange(PropertyNames::Water::ph              ); }
   if (this->m_alkalinity_ppm     != other.m_alkalinity_ppm    ) { this->propagatePropertyChange(PropertyNames::Water::alkalinity_ppm  ); }
   if (this->m_notes              != other.m_notes             ) { this->propagatePropertyChange(PropertyNames::Water::notes           ); }
   if (this->m_type               != other.m_type              ) { this->propagatePropertyChange(PropertyNames::Water::type            ); }
   if (this->m_mashRo_pct         != other.m_mashRo_pct        ) { this->propagatePropertyChange(PropertyNames::Water::mashRo_pct      ); }
   if (this->m_spargeRo_pct       != other.m_spargeRo_pct      ) { this->propagatePropertyChange(PropertyNames::Water::spargeRo_pct    ); }
   if (this->m_alkalinity_as_hco3 != other.m_alkalinity_as_hco3) { this->propagatePropertyChange(PropertyNames::Water::alkalinityAsHCO3); }
   // ⮜⮜⮜ All below added for BeerJSON support ⮞⮞⮞
   if (this->m_carbonate_ppm      != other.m_carbonate_ppm     ) { this->propagatePropertyChange(PropertyNames::Water::carbonate_ppm); }
   if (this->m_potassium_ppm      != other.m_potassium_ppm     ) { this->propagatePropertyChange(PropertyNames::Water::potassium_ppm); }
   if (this->m_iron_ppm           != other.m_iron_ppm          ) { this->propagatePropertyChange(PropertyNames::Water::iron_ppm     ); }
   if (this->m_nitrate_ppm        != other.m_nitrate_ppm       ) { this->propagatePropertyChange(PropertyNames::Water::nitrate_ppm  ); }
   if (this->m_nitrite_ppm        != other.m_nitrite_ppm       ) { this->propagatePropertyChange(PropertyNames::Water::nitrite_ppm  ); }
   if (this->m_flouride_ppm       != other.m_flouride_ppm      ) { this->propagatePropertyChange(PropertyNames::Water::flouride_ppm ); }

   return *this;
}

//============================================= "GETTER" MEMBER FUNCTIONS ==============================================
///double                     Water::amount          () const { return                    m_amount            ; }
double                     Water::calcium_ppm     () const { return                    m_calcium_ppm       ; }
double                     Water::bicarbonate_ppm () const { return                    m_bicarbonate_ppm   ; }
double                     Water::sulfate_ppm     () const { return                    m_sulfate_ppm       ; }
double                     Water::chloride_ppm    () const { return                    m_chloride_ppm      ; }
double                     Water::sodium_ppm      () const { return                    m_sodium_ppm        ; }
double                     Water::magnesium_ppm   () const { return                    m_magnesium_ppm     ; }
double                     Water::ph              () const { return                    m_ph                ; }
double                     Water::alkalinity_ppm  () const { return                    m_alkalinity_ppm    ; }
QString                    Water::notes           () const { return                    m_notes             ; }
std::optional<Water::Type> Water::type            () const { return                    m_type              ; }
std::optional<int>         Water::typeAsInt       () const { return Optional::toOptInt(m_type)             ; }
double                     Water::mashRo_pct      () const { return                    m_mashRo_pct        ; }
double                     Water::spargeRo_pct    () const { return                    m_spargeRo_pct      ; }
bool                       Water::alkalinityAsHCO3() const { return                    m_alkalinity_as_hco3; }
// ⮜⮜⮜ All below added for BeerJSON support ⮞⮞⮞
std::optional<double>      Water::carbonate_ppm   () const { return                    m_carbonate_ppm; }
std::optional<double>      Water::potassium_ppm   () const { return                    m_potassium_ppm; }
std::optional<double>      Water::iron_ppm        () const { return                    m_iron_ppm     ; }
std::optional<double>      Water::nitrate_ppm     () const { return                    m_nitrate_ppm  ; }
std::optional<double>      Water::nitrite_ppm     () const { return                    m_nitrite_ppm  ; }
std::optional<double>      Water::flouride_ppm    () const { return                    m_flouride_ppm ; }

//============================================= "SETTER" MEMBER FUNCTIONS ==============================================
///void Water::setAmount          (double              const   val) { SET_AND_NOTIFY(PropertyNames::Water::amount          , m_amount            ,                            val ); return; }
void Water::setCalcium_ppm     (double              const   val) { SET_AND_NOTIFY(PropertyNames::Water::calcium_ppm     , m_calcium_ppm       , val); return; }
void Water::setBicarbonate_ppm (double              const   val) { SET_AND_NOTIFY(PropertyNames::Water::bicarbonate_ppm , m_bicarbonate_ppm   , val); return; }
void Water::setSulfate_ppm     (double              const   val) { SET_AND_NOTIFY(PropertyNames::Water::sulfate_ppm     , m_sulfate_ppm       , val); return; }
void Water::setChloride_ppm    (double              const   val) { SET_AND_NOTIFY(PropertyNames::Water::chloride_ppm    , m_chloride_ppm      , val); return; }
void Water::setSodium_ppm      (double              const   val) { SET_AND_NOTIFY(PropertyNames::Water::sodium_ppm      , m_sodium_ppm        , val); return; }
void Water::setMagnesium_ppm   (double              const   val) { SET_AND_NOTIFY(PropertyNames::Water::magnesium_ppm   , m_magnesium_ppm     , val); return; }
void Water::setPh              (double              const   val) { SET_AND_NOTIFY(PropertyNames::Water::ph              , m_ph                , val); return; }
void Water::setAlkalinity_ppm  (double              const   val) { SET_AND_NOTIFY(PropertyNames::Water::alkalinity_ppm  , m_alkalinity_ppm    , val); return; }
void Water::setNotes           (QString             const & val) { SET_AND_NOTIFY(PropertyNames::Water::notes           , m_notes             , val); return; }
void Water::setType            (std::optional<Type> const   val) { SET_AND_NOTIFY(PropertyNames::Water::type            , m_type              , val); return; }
void Water::setTypeAsInt       (std::optional<int>  const   val) { SET_AND_NOTIFY(PropertyNames::Water::type            , m_type              , Optional::fromOptInt<Type>(val)); return; }
void Water::setMashRo_pct      (double              const   val) { SET_AND_NOTIFY(PropertyNames::Water::mashRo_pct      , m_mashRo_pct        , val); return; }
void Water::setSpargeRo_pct    (double              const   val) { SET_AND_NOTIFY(PropertyNames::Water::spargeRo_pct    , m_spargeRo_pct      , val); return; }
void Water::setAlkalinityAsHCO3(bool                const   val) { SET_AND_NOTIFY(PropertyNames::Water::alkalinityAsHCO3, m_alkalinity_as_hco3, val); return; }
// ⮜⮜⮜ All below added for BeerJSON support ⮞⮞⮞
void Water::setCarbonate_ppm   (std::optional<double> const val) { SET_AND_NOTIFY(PropertyNames::Water::carbonate_ppm   , m_carbonate_ppm     , val); return; }
void Water::setPotassium_ppm   (std::optional<double> const val) { SET_AND_NOTIFY(PropertyNames::Water::potassium_ppm   , m_potassium_ppm     , val); return; }
void Water::setIron_ppm        (std::optional<double> const val) { SET_AND_NOTIFY(PropertyNames::Water::iron_ppm        , m_iron_ppm          , val); return; }
void Water::setNitrate_ppm     (std::optional<double> const val) { SET_AND_NOTIFY(PropertyNames::Water::nitrate_ppm     , m_nitrate_ppm       , val); return; }
void Water::setNitrite_ppm     (std::optional<double> const val) { SET_AND_NOTIFY(PropertyNames::Water::nitrite_ppm     , m_nitrite_ppm       , val); return; }
void Water::setFlouride_ppm    (std::optional<double> const val) { SET_AND_NOTIFY(PropertyNames::Water::flouride_ppm    , m_flouride_ppm      , val); return; }

double Water::ppm(Water::Ion const ion) const {
   switch (ion) {
      case Water::Ion::Ca:   return this->m_calcium_ppm;
      case Water::Ion::Cl:   return this->m_chloride_ppm;
      case Water::Ion::HCO3: return this->m_bicarbonate_ppm;
      case Water::Ion::Mg:   return this->m_magnesium_ppm;
      case Water::Ion::Na:   return this->m_sodium_ppm;
      case Water::Ion::SO4:  return this->m_sulfate_ppm;
      // No default case as we want the compiler to warn us if we missed one of the enum values above
   }

   // Should be unreachable
   return 0.0;
}

///Recipe * Water::getOwningRecipe() const {
///   return ObjectStoreWrapper::findFirstMatching<Recipe>( [this](Recipe * rec) {return rec->uses(*this);} );
///}

// Boilerplate code for FolderBase
FOLDER_BASE_COMMON_CODE(Water)
