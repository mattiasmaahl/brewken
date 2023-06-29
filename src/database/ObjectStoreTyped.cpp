/*======================================================================================================================
 * database/ObjectStoreTyped.cpp is part of Brewken, and is copyright the following authors 2021-2023:
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
#include "database/ObjectStoreTyped.h"

#include  <mutex> // for std::once_flag

#include "database/DbTransaction.h"
#include "model/BrewNote.h"
#include "model/Equipment.h"
#include "model/Fermentable.h"
#include "model/Hop.h"
#include "model/Instruction.h"
#include "model/Inventory.h"
#include "model/Mash.h"
#include "model/MashStep.h"
#include "model/Misc.h"
#include "model/Recipe.h"
#include "model/Salt.h"
#include "model/Style.h"
#include "model/Water.h"
#include "model/Yeast.h"


namespace {

   //
   // By the magic of templated variables and template specialisation, we have below all the constructor parameters for
   // each type of ObjectStoreTyped.
   //
   // The only wrinkle here is that the order of definitions matters, eg the definition of PRIMARY_TABLE<BrewNote>
   // needs to appear after that of PRIMARY_TABLE<Recipe>, as the address of the latter is used in the former (to show
   // foreign key references).  However, as long as we don't want circular foreign key references in the database,
   // there should always be an order that works!
   //
   template<class NE> ObjectStore::TableDefinition const PRIMARY_TABLE;
   template<class NE> ObjectStore::JunctionTableDefinitions const JUNCTION_TABLES;

   //
   // NOTE: Unlike C++, SQL is generally case-insensitive, so we have slightly different naming conventions.
   //       Specifically, we use snake_case rather than camelCase for field and table names.  By convention, we also
   //       use upper case for SQL keywords and lower case for everything else.  This is in pursuit of making SQL
   //       slightly more readable.
   //

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Database field mappings for Equipment
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   template<> ObjectStore::TableDefinition const PRIMARY_TABLE<Equipment> {
      "equipment",
      {
         {ObjectStore::FieldType::Int   , "id"                            , PropertyNames::NamedEntity::key                      },
         {ObjectStore::FieldType::String, "name"                          , PropertyNames::NamedEntity::name                     },
         {ObjectStore::FieldType::Bool  , "display"                       , PropertyNames::NamedEntity::display                  },
         {ObjectStore::FieldType::Bool  , "deleted"                       , PropertyNames::NamedEntity::deleted                  },
         {ObjectStore::FieldType::String, "folder"                        , PropertyNames::NamedEntity::folder                   },
         {ObjectStore::FieldType::Double, "fermenter_batch_size_l"        , PropertyNames::Equipment::fermenterBatchSize_l       },
         {ObjectStore::FieldType::Double, "boiling_point"                 , PropertyNames::Equipment::boilingPoint_c             },
         {ObjectStore::FieldType::Double, "kettle_boil_size_l"            , PropertyNames::Equipment::kettleBoilSize_l           },
         {ObjectStore::FieldType::Double, "boil_time"                     , PropertyNames::Equipment::boilTime_min               },
         {ObjectStore::FieldType::Bool  , "calc_boil_volume"              , PropertyNames::Equipment::calcBoilVolume             },
         {ObjectStore::FieldType::Double, "kettle_evaporation_per_hour_l" , PropertyNames::Equipment::kettleEvaporationPerHour_l },
         {ObjectStore::FieldType::Double, "evap_rate"                     , PropertyNames::Equipment::evapRate_pctHr             },
         {ObjectStore::FieldType::Double, "mash_tun_grain_absorption_lkg" , PropertyNames::Equipment::mashTunGrainAbsorption_LKg },
         {ObjectStore::FieldType::Double, "hop_utilization"               , PropertyNames::Equipment::hopUtilization_pct         },
         {ObjectStore::FieldType::Double, "lauter_tun_deadspace_loss_l"   , PropertyNames::Equipment::lauterTunDeadspaceLoss_l   },
         {ObjectStore::FieldType::String, "kettle_notes"                  , PropertyNames::Equipment::kettleNotes                },
         {ObjectStore::FieldType::Double, "top_up_kettle"                 , PropertyNames::Equipment::topUpKettle_l              },
         {ObjectStore::FieldType::Double, "top_up_water"                  , PropertyNames::Equipment::topUpWater_l               },
         {ObjectStore::FieldType::Double, "kettle_trub_chiller_loss_l"    , PropertyNames::Equipment::kettleTrubChillerLoss_l    },
         {ObjectStore::FieldType::Double, "mash_tun_specific_heat_calgc"  , PropertyNames::Equipment::mashTunSpecificHeat_calGC  },
         {ObjectStore::FieldType::Double, "mash_tun_volume_l"             , PropertyNames::Equipment::mashTunVolume_l            },
         {ObjectStore::FieldType::Double, "mash_tun_weight_kg"            , PropertyNames::Equipment::mashTunWeight_kg           },
         // ⮜⮜⮜ All below added for BeerJSON support ⮞⮞⮞
         {ObjectStore::FieldType::String, "hlt_type"                      , PropertyNames::Equipment::hltType                    },
         {ObjectStore::FieldType::String, "mash_tun_type"                 , PropertyNames::Equipment::mashTunType                },
         {ObjectStore::FieldType::String, "lauter_tun_type"               , PropertyNames::Equipment::lauterTunType              },
         {ObjectStore::FieldType::String, "kettle_type"                   , PropertyNames::Equipment::kettleType                 },
         {ObjectStore::FieldType::String, "fermenter_type"                , PropertyNames::Equipment::fermenterType              },
         {ObjectStore::FieldType::String, "agingvessel_type"              , PropertyNames::Equipment::agingVesselType            },
         {ObjectStore::FieldType::String, "packaging_vessel_type"         , PropertyNames::Equipment::packagingVesselType        },
         {ObjectStore::FieldType::Double, "hlt_volume_l"                  , PropertyNames::Equipment::hltVolume_l                },
         {ObjectStore::FieldType::Double, "lauter_tun_volume_l"           , PropertyNames::Equipment::lauterTunVolume_l          },
         {ObjectStore::FieldType::Double, "aging_vessel_volume_l"         , PropertyNames::Equipment::agingVesselVolume_l        },
         {ObjectStore::FieldType::Double, "packaging_vessel_volume_l"     , PropertyNames::Equipment::packagingVesselVolume_l    },
         {ObjectStore::FieldType::Double, "hlt_loss_l"                    , PropertyNames::Equipment::hltLoss_l                  },
         {ObjectStore::FieldType::Double, "mash_tun_loss_l"               , PropertyNames::Equipment::mashTunLoss_l              },
         {ObjectStore::FieldType::Double, "fermenter_loss_l"              , PropertyNames::Equipment::fermenterLoss_l            },
         {ObjectStore::FieldType::Double, "aging_vessel_loss_l"           , PropertyNames::Equipment::agingVesselLoss_l          },
         {ObjectStore::FieldType::Double, "packaging_vessel_loss_l"       , PropertyNames::Equipment::packagingVesselLoss_l      },
         {ObjectStore::FieldType::Double, "kettle_outflow_per_minute_l"   , PropertyNames::Equipment::kettleOutflowPerMinute_l   },
         {ObjectStore::FieldType::Double, "hlt_weight_kg"                 , PropertyNames::Equipment::hltWeight_kg               },
         {ObjectStore::FieldType::Double, "lauter_tun_weight_kg"          , PropertyNames::Equipment::lauterTunWeight_kg         },
         {ObjectStore::FieldType::Double, "kettle_weight_kg"              , PropertyNames::Equipment::kettleWeight_kg            },
         {ObjectStore::FieldType::Double, "hlt_specific_heat_calgc"       , PropertyNames::Equipment::hltSpecificHeat_calGC      },
         {ObjectStore::FieldType::Double, "lauter_tun_specific_heat_calgc", PropertyNames::Equipment::lauterTunSpecificHeat_calGC},
         {ObjectStore::FieldType::Double, "kettle_specific_heat_calgc"    , PropertyNames::Equipment::kettleSpecificHeat_calGC   },
         {ObjectStore::FieldType::String, "hlt_notes"                     , PropertyNames::Equipment::hltNotes                   },
         {ObjectStore::FieldType::String, "mash_tun_notes"                , PropertyNames::Equipment::mashTunNotes               },
         {ObjectStore::FieldType::String, "lauter_tun_notes"              , PropertyNames::Equipment::lauterTunNotes             },
         {ObjectStore::FieldType::String, "fermenter_notes"               , PropertyNames::Equipment::fermenterNotes             },
         {ObjectStore::FieldType::String, "aging_vessel_notes"            , PropertyNames::Equipment::agingVesselNotes           },
         {ObjectStore::FieldType::String, "packaging_vessel_notes"        , PropertyNames::Equipment::packagingVesselNotes       },
      }
   };
   template<> ObjectStore::JunctionTableDefinitions const JUNCTION_TABLES<Equipment> {
      // NamedEntity objects store their parents not their children, so this view of the junction table is from the child's point of view
      {
         "equipment_children",
         {
            {ObjectStore::FieldType::Int, "id"                                                                                  },
            {ObjectStore::FieldType::Int, "child_id",  PropertyNames::NamedEntity::key,       nullptr, &PRIMARY_TABLE<Equipment>},
            {ObjectStore::FieldType::Int, "parent_id", PropertyNames::NamedEntity::parentKey, nullptr, &PRIMARY_TABLE<Equipment>},
         },
         ObjectStore::MAX_ONE_ENTRY
      }
   };

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Database field mappings for InventoryFermentable
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   template<> ObjectStore::TableDefinition const PRIMARY_TABLE<InventoryFermentable> {
      "fermentable_in_inventory",
      {
         {ObjectStore::FieldType::Int,    "id",               PropertyNames::Inventory::id},
         {ObjectStore::FieldType::Double, "amount",           PropertyNames::Inventory::amount}
      }
   };
   template<> ObjectStore::JunctionTableDefinitions const JUNCTION_TABLES<InventoryFermentable> {};

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Database field mappings for Fermentable
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   template<> ObjectStore::TableDefinition const PRIMARY_TABLE<Fermentable> {
      "fermentable",
      {
         {ObjectStore::FieldType::Int   , "id"                            , PropertyNames::NamedEntity::key                      },
         {ObjectStore::FieldType::String, "name"                          , PropertyNames::NamedEntity::name                     },
         {ObjectStore::FieldType::Bool  , "deleted"                       , PropertyNames::NamedEntity::deleted                  },
         {ObjectStore::FieldType::Bool  , "display"                       , PropertyNames::NamedEntity::display                  },
         {ObjectStore::FieldType::String, "folder"                        , PropertyNames::NamedEntity::folder                   },
         {ObjectStore::FieldType::Int   , "inventory_id"                  , PropertyNames::NamedEntityWithInventory::inventoryId , nullptr, &PRIMARY_TABLE<InventoryFermentable>},
         {ObjectStore::FieldType::Bool  , "add_after_boil"                , PropertyNames::Fermentable::addAfterBoil             },
         {ObjectStore::FieldType::Double, "amount"                        , PropertyNames::Fermentable::amount                   },
         {ObjectStore::FieldType::Bool  , "amount_is_weight"              , PropertyNames::Fermentable::amountIsWeight           }, // ⮜⮜⮜ Added for BeerJSON support ⮞⮞⮞
         {ObjectStore::FieldType::Double, "coarse_fine_diff"              , PropertyNames::Fermentable::coarseFineDiff_pct       },
         {ObjectStore::FieldType::Double, "color"                         , PropertyNames::Fermentable::color_srm                },
         {ObjectStore::FieldType::Double, "diastatic_power"               , PropertyNames::Fermentable::diastaticPower_lintner   },
         {ObjectStore::FieldType::Enum  , "ftype"                         , PropertyNames::Fermentable::type                     , &Fermentable::typeStringMapping},
         {ObjectStore::FieldType::Bool  , "is_mashed"                     , PropertyNames::Fermentable::isMashed                 },
         {ObjectStore::FieldType::Double, "ibu_gal_per_lb"                , PropertyNames::Fermentable::ibuGalPerLb              },
         {ObjectStore::FieldType::Double, "max_in_batch"                  , PropertyNames::Fermentable::maxInBatch_pct           },
         {ObjectStore::FieldType::Double, "moisture"                      , PropertyNames::Fermentable::moisture_pct             },
         {ObjectStore::FieldType::String, "notes"                         , PropertyNames::Fermentable::notes                    },
         {ObjectStore::FieldType::String, "origin"                        , PropertyNames::Fermentable::origin                   },
         {ObjectStore::FieldType::String, "supplier"                      , PropertyNames::Fermentable::supplier                 },
         {ObjectStore::FieldType::Double, "protein"                       , PropertyNames::Fermentable::protein_pct              },
         {ObjectStore::FieldType::Bool  , "recommend_mash"                , PropertyNames::Fermentable::recommendMash            },
         {ObjectStore::FieldType::Double, "yield"                         , PropertyNames::Fermentable::yield_pct                },
         // ⮜⮜⮜ All below added for BeerJSON support ⮞⮞⮞
         {ObjectStore::FieldType::Enum  , "grain_group"                   , PropertyNames::Fermentable::grainGroup               , &Fermentable::grainGroupStringMapping},
         {ObjectStore::FieldType::String, "producer"                      , PropertyNames::Fermentable::producer                 },
         {ObjectStore::FieldType::String, "productid"                     , PropertyNames::Fermentable::productId                },
         {ObjectStore::FieldType::Double, "fine_grind_yield_pct"          , PropertyNames::Fermentable::fineGrindYield_pct       },
         {ObjectStore::FieldType::Double, "coarse_grind_yield_pct"        , PropertyNames::Fermentable::coarseGrindYield_pct     },
         {ObjectStore::FieldType::Double, "potential_yield_sg"            , PropertyNames::Fermentable::potentialYield_sg        },
         {ObjectStore::FieldType::Double, "alpha_amylase_dext_units"      , PropertyNames::Fermentable::alphaAmylase_dextUnits   },
         {ObjectStore::FieldType::Double, "kolbach_index_pct"             , PropertyNames::Fermentable::kolbachIndex_pct         },
         {ObjectStore::FieldType::Double, "hardness_prp_glassy_pct"       , PropertyNames::Fermentable::hardnessPrpGlassy_pct    },
         {ObjectStore::FieldType::Double, "hardness_prp_half_pct"         , PropertyNames::Fermentable::hardnessPrpHalf_pct      },
         {ObjectStore::FieldType::Double, "hardness_prp_mealy_pct"        , PropertyNames::Fermentable::hardnessPrpMealy_pct     },
         {ObjectStore::FieldType::Double, "kernel_size_prp_plump_pct"     , PropertyNames::Fermentable::kernelSizePrpPlump_pct   },
         {ObjectStore::FieldType::Double, "kernel_size_prp_thin_pct"      , PropertyNames::Fermentable::kernelSizePrpThin_pct    },
         {ObjectStore::FieldType::Double, "friability_pct"                , PropertyNames::Fermentable::friability_pct           },
         {ObjectStore::FieldType::Double, "di_ph"                         , PropertyNames::Fermentable::di_ph                    },
         {ObjectStore::FieldType::Double, "viscosity_cp"                  , PropertyNames::Fermentable::viscosity_cP             },
         {ObjectStore::FieldType::Double, "dmsp"                          , PropertyNames::Fermentable::dmsP                     },
         {ObjectStore::FieldType::Bool  , "dmsp_is_mass_per_volume"       , PropertyNames::Fermentable::dmsPIsMassPerVolume      },
         {ObjectStore::FieldType::Double, "fan"                           , PropertyNames::Fermentable::fan                      },
         {ObjectStore::FieldType::Bool  , "fan_is_mass_per_volume"        , PropertyNames::Fermentable::fanIsMassPerVolume       },
         {ObjectStore::FieldType::Double, "fermentability_pct"            , PropertyNames::Fermentable::fermentability_pct       },
         {ObjectStore::FieldType::Double, "beta_glucan"                   , PropertyNames::Fermentable::betaGlucan               },
         {ObjectStore::FieldType::Bool  , "beta_glucan_is_mass_per_volume", PropertyNames::Fermentable::betaGlucanIsMassPerVolume},
      }
   };
   template<> ObjectStore::JunctionTableDefinitions const JUNCTION_TABLES<Fermentable> {
      {
         "fermentable_children",
         {
            {ObjectStore::FieldType::Int, "id"                                                                                    },
            {ObjectStore::FieldType::Int, "child_id",  PropertyNames::NamedEntity::key,       nullptr, &PRIMARY_TABLE<Fermentable>},
            {ObjectStore::FieldType::Int, "parent_id", PropertyNames::NamedEntity::parentKey, nullptr, &PRIMARY_TABLE<Fermentable>},
         },
         ObjectStore::MAX_ONE_ENTRY
      }
   };

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Database field mappings for InventoryHop
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   template<> ObjectStore::TableDefinition const PRIMARY_TABLE<InventoryHop> {
      "hop_in_inventory",
      {
         {ObjectStore::FieldType::Int,    "id",               PropertyNames::Inventory::id    },
         {ObjectStore::FieldType::Double, "amount",           PropertyNames::Inventory::amount},
      }
   };
   template<> ObjectStore::JunctionTableDefinitions const JUNCTION_TABLES<InventoryHop> {};

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Database field mappings for Hop
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   template<> ObjectStore::TableDefinition const PRIMARY_TABLE<Hop> {
      "hop",
      {
         {ObjectStore::FieldType::Int   , "id"                   , PropertyNames::NamedEntity::key                     },
         {ObjectStore::FieldType::String, "name"                 , PropertyNames::NamedEntity::name                    },
         {ObjectStore::FieldType::Bool  , "display"              , PropertyNames::NamedEntity::display                 },
         {ObjectStore::FieldType::Bool  , "deleted"              , PropertyNames::NamedEntity::deleted                 },
         {ObjectStore::FieldType::String, "folder"               , PropertyNames::NamedEntity::folder                  },
         {ObjectStore::FieldType::Int   , "inventory_id"         , PropertyNames::NamedEntityWithInventory::inventoryId, nullptr,           &PRIMARY_TABLE<InventoryHop>},
         {ObjectStore::FieldType::Double, "alpha"                , PropertyNames::Hop::alpha_pct                       },
         {ObjectStore::FieldType::Double, "amount"               , PropertyNames::Hop::amount_kg                       },
         {ObjectStore::FieldType::Double, "beta"                 , PropertyNames::Hop::beta_pct                        },
         {ObjectStore::FieldType::Double, "caryophyllene"        , PropertyNames::Hop::caryophyllene_pct               },
         {ObjectStore::FieldType::Double, "cohumulone"           , PropertyNames::Hop::cohumulone_pct                  },
         {ObjectStore::FieldType::Enum  , "form"                 , PropertyNames::Hop::form                            , &Hop::formStringMapping},
         {ObjectStore::FieldType::Double, "hsi"                  , PropertyNames::Hop::hsi_pct                         },
         {ObjectStore::FieldType::Double, "humulene"             , PropertyNames::Hop::humulene_pct                    },
         {ObjectStore::FieldType::Double, "myrcene"              , PropertyNames::Hop::myrcene_pct                     },
         {ObjectStore::FieldType::String, "notes"                , PropertyNames::Hop::notes                           },
         {ObjectStore::FieldType::String, "origin"               , PropertyNames::Hop::origin                          },
         {ObjectStore::FieldType::String, "substitutes"          , PropertyNames::Hop::substitutes                     },
         {ObjectStore::FieldType::Double, "time"                 , PropertyNames::Hop::time_min                        },
         {ObjectStore::FieldType::Enum  , "htype"                , PropertyNames::Hop::type                            , &Hop::typeStringMapping},
         {ObjectStore::FieldType::Enum  , "use"                  , PropertyNames::Hop::use                             , &Hop::useStringMapping},
         // ⮜⮜⮜ All below added for BeerJSON support ⮞⮞⮞
         {ObjectStore::FieldType::String, "producer"             , PropertyNames::Hop::producer                        },
         {ObjectStore::FieldType::String, "product_id"           , PropertyNames::Hop::product_id                      },
         {ObjectStore::FieldType::String, "year"                 , PropertyNames::Hop::year                            },
         {ObjectStore::FieldType::Double, "total_oil_ml_per_100g", PropertyNames::Hop::total_oil_ml_per_100g           },
         {ObjectStore::FieldType::Double, "farnesene_pct"        , PropertyNames::Hop::farnesene_pct                   },
         {ObjectStore::FieldType::Double, "geraniol_pct"         , PropertyNames::Hop::geraniol_pct                    },
         {ObjectStore::FieldType::Double, "b_pinene_pct"         , PropertyNames::Hop::b_pinene_pct                    },
         {ObjectStore::FieldType::Double, "linalool_pct"         , PropertyNames::Hop::linalool_pct                    },
         {ObjectStore::FieldType::Double, "limonene_pct"         , PropertyNames::Hop::limonene_pct                    },
         {ObjectStore::FieldType::Double, "nerol_pct"            , PropertyNames::Hop::nerol_pct                       },
         {ObjectStore::FieldType::Double, "pinene_pct"           , PropertyNames::Hop::pinene_pct                      },
         {ObjectStore::FieldType::Double, "polyphenols_pct"      , PropertyNames::Hop::polyphenols_pct                 },
         {ObjectStore::FieldType::Double, "xanthohumol_pct"      , PropertyNames::Hop::xanthohumol_pct                 },
      }
   };
   template<> ObjectStore::JunctionTableDefinitions const JUNCTION_TABLES<Hop> {
      {
         "hop_children",
         {
            {ObjectStore::FieldType::Int, "id"                                                                            },
            {ObjectStore::FieldType::Int, "child_id",  PropertyNames::NamedEntity::key,       nullptr, &PRIMARY_TABLE<Hop>},
            {ObjectStore::FieldType::Int, "parent_id", PropertyNames::NamedEntity::parentKey, nullptr, &PRIMARY_TABLE<Hop>},
         },
         ObjectStore::MAX_ONE_ENTRY
      }
   };

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Database field mappings for Instruction
   // NB: instructions aren't displayed in trees, and get no folder
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   template<> ObjectStore::TableDefinition const PRIMARY_TABLE<Instruction> {
      "instruction",
      {
         {ObjectStore::FieldType::Int   , "id"        , PropertyNames::NamedEntity::key       },
         {ObjectStore::FieldType::String, "name"      , PropertyNames::NamedEntity::name      },
         {ObjectStore::FieldType::Bool  , "display"   , PropertyNames::NamedEntity::display   },
         {ObjectStore::FieldType::Bool  , "deleted"   , PropertyNames::NamedEntity::deleted   },
         {ObjectStore::FieldType::String, "directions", PropertyNames::Instruction::directions},
         {ObjectStore::FieldType::Bool  , "hasTimer"  , PropertyNames::Instruction::hasTimer  },
         {ObjectStore::FieldType::String, "timervalue", PropertyNames::Instruction::timerValue},
         {ObjectStore::FieldType::Bool  , "completed" , PropertyNames::Instruction::completed },
         {ObjectStore::FieldType::Double, "interval"  , PropertyNames::Instruction::interval  },
      }
   };
   // Instructions don't have children
   template<> ObjectStore::JunctionTableDefinitions const JUNCTION_TABLES<Instruction> {};

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Database field mappings for Mash
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   template<> ObjectStore::TableDefinition const PRIMARY_TABLE<Mash> {
      "mash",
      {
         {ObjectStore::FieldType::Int   , "id"               , PropertyNames::NamedEntity::key           },
         {ObjectStore::FieldType::String, "name"             , PropertyNames::NamedEntity::name          },
         {ObjectStore::FieldType::Bool  , "deleted"          , PropertyNames::NamedEntity::deleted       },
         {ObjectStore::FieldType::Bool  , "display"          , PropertyNames::NamedEntity::display       },
         {ObjectStore::FieldType::String, "folder"           , PropertyNames::NamedEntity::folder        },
         {ObjectStore::FieldType::Bool  , "equip_adjust"     , PropertyNames::Mash::equipAdjust          },
         {ObjectStore::FieldType::Double, "grain_temp"       , PropertyNames::Mash::grainTemp_c          },
         {ObjectStore::FieldType::String, "notes"            , PropertyNames::Mash::notes                },
         {ObjectStore::FieldType::Double, "ph"               , PropertyNames::Mash::ph                   },
         {ObjectStore::FieldType::Double, "sparge_temp"      , PropertyNames::Mash::spargeTemp_c         },
         {ObjectStore::FieldType::Double, "tun_specific_heat", PropertyNames::Mash::mashTunSpecificHeat_calGC},
         {ObjectStore::FieldType::Double, "tun_temp"         , PropertyNames::Mash::tunTemp_c            },
         {ObjectStore::FieldType::Double, "tun_weight"       , PropertyNames::Mash::mashTunWeight_kg         },
      }
   };
   // Mashes don't have children, and the link with their MashSteps is stored in the MashStep (as between Recipe and BrewNotes)
   template<> ObjectStore::JunctionTableDefinitions const JUNCTION_TABLES<Mash> {};

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Database field mappings for MashStep
   // NB: MashSteps don't get folders, because they don't separate from their Mash
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   template<> ObjectStore::TableDefinition const PRIMARY_TABLE<MashStep> {
      "mashstep",
      {
         {ObjectStore::FieldType::Int   , "id"              , PropertyNames::NamedEntity::key           },
         {ObjectStore::FieldType::String, "name"            , PropertyNames::NamedEntity::name          },
         {ObjectStore::FieldType::Bool  , "deleted"         , PropertyNames::NamedEntity::deleted       },
         {ObjectStore::FieldType::Bool  , "display"         , PropertyNames::NamedEntity::display       },
         // NB: MashSteps don't have folders, as each one is owned by a Mash
//         {ObjectStore::FieldType::Double, "decoction_amount", PropertyNames::MashStep::decoctionAmount_l},
         {ObjectStore::FieldType::Double, "end_temp"        , PropertyNames::MashStep::endTemp_c        },
//         {ObjectStore::FieldType::Double, "infuse_amount"   , PropertyNames::MashStep::infuseAmount_l   },
         {ObjectStore::FieldType::Double, "infuse_temp"     , PropertyNames::MashStep::infuseTemp_c     },
         {ObjectStore::FieldType::Int   , "mash_id"         , PropertyNames::MashStep::mashId           , nullptr, &PRIMARY_TABLE<Mash>},
         {ObjectStore::FieldType::Enum  , "mstype"          , PropertyNames::MashStep::type             , &MashStep::typeStringMapping},
         {ObjectStore::FieldType::Double, "ramp_time"       , PropertyNames::MashStep::rampTime_min     },
         {ObjectStore::FieldType::Int   , "step_number"     , PropertyNames::MashStep::stepNumber       },
         {ObjectStore::FieldType::Double, "step_temp"       , PropertyNames::MashStep::stepTemp_c       },
         {ObjectStore::FieldType::Double, "step_time"       , PropertyNames::MashStep::stepTime_min     },
         // Now we support BeerJSON, amount_l unifies and replaces infuseAmount_l and decoctionAmount_l
         // See comment in model/MashStep.h for more info
         {ObjectStore::FieldType::Double, "amount_l"        , PropertyNames::MashStep::amount_l         },

      }
   };
   // MashSteps don't have children
   template<> ObjectStore::JunctionTableDefinitions const JUNCTION_TABLES<MashStep> {};

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Database field mappings for InventoryMisc
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   template<> ObjectStore::TableDefinition const PRIMARY_TABLE<InventoryMisc> {
      "misc_in_inventory",
      {
         {ObjectStore::FieldType::Int,    "id",               PropertyNames::Inventory::id    },
         {ObjectStore::FieldType::Double, "amount",           PropertyNames::Inventory::amount},
      }
   };
   template<> ObjectStore::JunctionTableDefinitions const JUNCTION_TABLES<InventoryMisc> {};

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Database field mappings for Misc
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   template<> ObjectStore::TableDefinition const PRIMARY_TABLE<Misc> {
      "misc",
      {
         {ObjectStore::FieldType::Int   , "id"              , PropertyNames::NamedEntity::key                     },
         {ObjectStore::FieldType::String, "name"            , PropertyNames::NamedEntity::name                    },
         {ObjectStore::FieldType::Bool  , "deleted"         , PropertyNames::NamedEntity::deleted                 },
         {ObjectStore::FieldType::Bool  , "display"         , PropertyNames::NamedEntity::display                 },
         {ObjectStore::FieldType::String, "folder"          , PropertyNames::NamedEntity::folder                  },
         {ObjectStore::FieldType::Int   , "inventory_id"    , PropertyNames::NamedEntityWithInventory::inventoryId, nullptr,         &PRIMARY_TABLE<InventoryMisc>},
         {ObjectStore::FieldType::Enum  , "mtype"           , PropertyNames::Misc::type                           , &Misc::typeStringMapping},
         {ObjectStore::FieldType::Enum  , "use"             , PropertyNames::Misc::use                            , &Misc::useStringMapping },
         {ObjectStore::FieldType::Double, "time"            , PropertyNames::Misc::time_min                       },
         {ObjectStore::FieldType::Double, "amount"          , PropertyNames::Misc::amount                         },
         {ObjectStore::FieldType::Bool  , "amount_is_weight", PropertyNames::Misc::amountIsWeight                 },
         {ObjectStore::FieldType::String, "use_for"         , PropertyNames::Misc::useFor                         },
         {ObjectStore::FieldType::String, "notes"           , PropertyNames::Misc::notes                          },
         // ⮜⮜⮜ All below added for BeerJSON support ⮞⮞⮞
         {ObjectStore::FieldType::String, "producer"        , PropertyNames::Misc::producer                       },
         {ObjectStore::FieldType::String, "product_id"      , PropertyNames::Misc::productId                      },
      }
   };
   template<> ObjectStore::JunctionTableDefinitions const JUNCTION_TABLES<Misc> {
      {
         "misc_children",
         {
            {ObjectStore::FieldType::Int, "id"                                                                             },
            {ObjectStore::FieldType::Int, "child_id",  PropertyNames::NamedEntity::key,       nullptr, &PRIMARY_TABLE<Misc>},
            {ObjectStore::FieldType::Int, "parent_id", PropertyNames::NamedEntity::parentKey, nullptr, &PRIMARY_TABLE<Misc>},
         },
         ObjectStore::MAX_ONE_ENTRY
      }
   };

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Database field mappings for Salt
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   template<> ObjectStore::TableDefinition const PRIMARY_TABLE<Salt> {
      "salt",
      {
         {ObjectStore::FieldType::Int   , "id"              , PropertyNames::NamedEntity::key     },
         {ObjectStore::FieldType::String, "name"            , PropertyNames::NamedEntity::name    },
         {ObjectStore::FieldType::Bool  , "deleted"         , PropertyNames::NamedEntity::deleted },
         {ObjectStore::FieldType::Bool  , "display"         , PropertyNames::NamedEntity::display },
         {ObjectStore::FieldType::String, "folder"          , PropertyNames::NamedEntity::folder  },
         {ObjectStore::FieldType::Int   , "addTo"           , PropertyNames::Salt::whenToAdd      }, // TODO: Really an Enum.  Would be less fragile to store this as text than a number.  Also, column name...
         {ObjectStore::FieldType::Double, "amount"          , PropertyNames::Salt::amount         },
         {ObjectStore::FieldType::Bool  , "amount_is_weight", PropertyNames::Salt::amountIsWeight },
         {ObjectStore::FieldType::Bool  , "is_acid"         , PropertyNames::Salt::isAcid         },
         {ObjectStore::FieldType::Double, "percent_acid"    , PropertyNames::Salt::percentAcid    },
         {ObjectStore::FieldType::Int   , "stype"           , PropertyNames::Salt::type           },  // TODO: Really an Enum.  Would be less fragile to store this as text than a number
      }
   };
   // Salts don't have children
   template<> ObjectStore::JunctionTableDefinitions const JUNCTION_TABLES<Salt> {};

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Database field mappings for Style
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   template<> ObjectStore::TableDefinition const PRIMARY_TABLE<Style> {
      "style",
      {
         {ObjectStore::FieldType::Int   , "id"                , PropertyNames::NamedEntity::key        },
         {ObjectStore::FieldType::String, "name"              , PropertyNames::NamedEntity::name       },
         {ObjectStore::FieldType::Bool  , "display"           , PropertyNames::NamedEntity::display    },
         {ObjectStore::FieldType::Bool  , "deleted"           , PropertyNames::NamedEntity::deleted    },
         {ObjectStore::FieldType::String, "folder"            , PropertyNames::NamedEntity::folder     },
         {ObjectStore::FieldType::Double, "abv_max"           , PropertyNames::Style::abvMax_pct       },
         {ObjectStore::FieldType::Double, "abv_min"           , PropertyNames::Style::abvMin_pct       },
         {ObjectStore::FieldType::Double, "carb_max"          , PropertyNames::Style::carbMax_vol      },
         {ObjectStore::FieldType::Double, "carb_min"          , PropertyNames::Style::carbMin_vol      },
         {ObjectStore::FieldType::String, "category"          , PropertyNames::Style::category         },
         {ObjectStore::FieldType::String, "category_number"   , PropertyNames::Style::categoryNumber   },
         {ObjectStore::FieldType::Double, "color_max"         , PropertyNames::Style::colorMax_srm     },
         {ObjectStore::FieldType::Double, "color_min"         , PropertyNames::Style::colorMin_srm     },
         {ObjectStore::FieldType::String, "examples"          , PropertyNames::Style::examples         },
         {ObjectStore::FieldType::Double, "fg_max"            , PropertyNames::Style::fgMax            },
         {ObjectStore::FieldType::Double, "fg_min"            , PropertyNames::Style::fgMin            },
         {ObjectStore::FieldType::Double, "ibu_max"           , PropertyNames::Style::ibuMax           },
         {ObjectStore::FieldType::Double, "ibu_min"           , PropertyNames::Style::ibuMin           },
         {ObjectStore::FieldType::String, "ingredients"       , PropertyNames::Style::ingredients      },
         {ObjectStore::FieldType::String, "notes"             , PropertyNames::Style::notes            },
         {ObjectStore::FieldType::Double, "og_max"            , PropertyNames::Style::ogMax            },
         {ObjectStore::FieldType::Double, "og_min"            , PropertyNames::Style::ogMin            },
///         {ObjectStore::FieldType::String, "profile"           , PropertyNames::Style::profile          },
         {ObjectStore::FieldType::String, "style_guide"       , PropertyNames::Style::styleGuide       },
         {ObjectStore::FieldType::String, "style_letter"      , PropertyNames::Style::styleLetter      },
         {ObjectStore::FieldType::Enum  , "stype"             , PropertyNames::Style::type             , &Style::typeStringMapping},
         // ⮜⮜⮜ All below added for BeerJSON support ⮞⮞⮞
         {ObjectStore::FieldType::String, "aroma"             , PropertyNames::Style::aroma            },
         {ObjectStore::FieldType::String, "appearance"        , PropertyNames::Style::appearance       },
         {ObjectStore::FieldType::String, "flavor"            , PropertyNames::Style::flavor           },
         {ObjectStore::FieldType::String, "mouthfeel"         , PropertyNames::Style::mouthfeel        },
         {ObjectStore::FieldType::String, "overall_impression", PropertyNames::Style::overallImpression},
      }
   };
   template<> ObjectStore::JunctionTableDefinitions const JUNCTION_TABLES<Style> {
      {
         "style_children",
         {
            {ObjectStore::FieldType::Int, "id"                                                                              },
            {ObjectStore::FieldType::Int, "child_id",  PropertyNames::NamedEntity::key,       nullptr, &PRIMARY_TABLE<Style>},
            {ObjectStore::FieldType::Int, "parent_id", PropertyNames::NamedEntity::parentKey, nullptr, &PRIMARY_TABLE<Style>},
         },
         ObjectStore::MAX_ONE_ENTRY
      }
   };

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Database field mappings for Water
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   template<> ObjectStore::TableDefinition const PRIMARY_TABLE<Water> {
      "water",
      {
         {ObjectStore::FieldType::Int   , "id"         , PropertyNames::NamedEntity::key       },
         {ObjectStore::FieldType::String, "name"       , PropertyNames::NamedEntity::name      },
         {ObjectStore::FieldType::Bool  , "display"    , PropertyNames::NamedEntity::display   },
         {ObjectStore::FieldType::Bool  , "deleted"    , PropertyNames::NamedEntity::deleted   },
         {ObjectStore::FieldType::String, "folder"     , PropertyNames::NamedEntity::folder    },
         {ObjectStore::FieldType::String, "notes"      , PropertyNames::Water::notes           },
         {ObjectStore::FieldType::Double, "amount"     , PropertyNames::Water::amount          },
         {ObjectStore::FieldType::Double, "calcium"    , PropertyNames::Water::calcium_ppm     },
         {ObjectStore::FieldType::Double, "bicarbonate", PropertyNames::Water::bicarbonate_ppm },
         {ObjectStore::FieldType::Double, "sulfate"    , PropertyNames::Water::sulfate_ppm     },
         {ObjectStore::FieldType::Double, "sodium"     , PropertyNames::Water::sodium_ppm      },
         {ObjectStore::FieldType::Double, "chloride"   , PropertyNames::Water::chloride_ppm    },
         {ObjectStore::FieldType::Double, "magnesium"  , PropertyNames::Water::magnesium_ppm   },
         {ObjectStore::FieldType::Double, "ph"         , PropertyNames::Water::ph              },
         {ObjectStore::FieldType::Double, "alkalinity" , PropertyNames::Water::alkalinity_ppm  },
         {ObjectStore::FieldType::Int   , "wtype"      , PropertyNames::Water::type            }, // TODO: Would be less fragile to store this as text than a number
         {ObjectStore::FieldType::Double, "mash_ro"    , PropertyNames::Water::mashRo_pct      },
         {ObjectStore::FieldType::Double, "sparge_ro"  , PropertyNames::Water::spargeRo_pct    },
         {ObjectStore::FieldType::Bool  , "as_hco3"    , PropertyNames::Water::alkalinityAsHCO3},
      }
   };
   template<> ObjectStore::JunctionTableDefinitions const JUNCTION_TABLES<Water> {
      {
         "water_children",
         {
            {ObjectStore::FieldType::Int, "id"                                                                              },
            {ObjectStore::FieldType::Int, "child_id",  PropertyNames::NamedEntity::key,       nullptr, &PRIMARY_TABLE<Water>},
            {ObjectStore::FieldType::Int, "parent_id", PropertyNames::NamedEntity::parentKey, nullptr, &PRIMARY_TABLE<Water>},
         },
         ObjectStore::MAX_ONE_ENTRY
      }
   };

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Database field mappings for InventoryYeast
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   template<> ObjectStore::TableDefinition const PRIMARY_TABLE<InventoryYeast> {
      "yeast_in_inventory",
      {
         {ObjectStore::FieldType::Int,    "id",               PropertyNames::Inventory::id},
         // Yeast inventory amount is called quanta, which I find hard to understand
         {ObjectStore::FieldType::Double, "quanta",           PropertyNames::Inventory::amount}
      }
   };
   template<> ObjectStore::JunctionTableDefinitions const JUNCTION_TABLES<InventoryYeast> {};

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Database field mappings for Yeast
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   template<> ObjectStore::TableDefinition const PRIMARY_TABLE<Yeast> {
      "yeast",
      {
         {ObjectStore::FieldType::Int   , "id"                          , PropertyNames::NamedEntity::key                },
         {ObjectStore::FieldType::String, "name"                        , PropertyNames::NamedEntity::name               },
         {ObjectStore::FieldType::Bool  , "display"                     , PropertyNames::NamedEntity::display            },
         {ObjectStore::FieldType::Bool  , "deleted"                     , PropertyNames::NamedEntity::deleted            },
         {ObjectStore::FieldType::String, "folder"                      , PropertyNames::NamedEntity::folder             },
         {ObjectStore::FieldType::Int   , "inventory_id"                , PropertyNames::NamedEntityWithInventory::inventoryId, nullptr,                     &PRIMARY_TABLE<InventoryYeast>},
         {ObjectStore::FieldType::Bool  , "add_to_secondary"            , PropertyNames::Yeast::addToSecondary           },
         {ObjectStore::FieldType::Bool  , "amount_is_weight"            , PropertyNames::Yeast::amountIsWeight           },
         {ObjectStore::FieldType::Double, "amount"                      , PropertyNames::Yeast::amount                   },
         {ObjectStore::FieldType::Double, "attenuation"                 , PropertyNames::Yeast::attenuation_pct          },
         {ObjectStore::FieldType::Double, "max_temperature"             , PropertyNames::Yeast::maxTemperature_c         },
         {ObjectStore::FieldType::Double, "min_temperature"             , PropertyNames::Yeast::minTemperature_c         },
         {ObjectStore::FieldType::Enum  , "flocculation"                , PropertyNames::Yeast::flocculation             , &Yeast::flocculationStringMapping},
         {ObjectStore::FieldType::Enum  , "form"                        , PropertyNames::Yeast::form                     , &Yeast::formStringMapping        },
         {ObjectStore::FieldType::Enum  , "ytype"                       , PropertyNames::Yeast::type                     , &Yeast::typeStringMapping        },
         {ObjectStore::FieldType::Int   , "max_reuse"                   , PropertyNames::Yeast::maxReuse                 },
         {ObjectStore::FieldType::Int   , "times_cultured"              , PropertyNames::Yeast::timesCultured            },
         {ObjectStore::FieldType::String, "best_for"                    , PropertyNames::Yeast::bestFor                  },
         {ObjectStore::FieldType::String, "laboratory"                  , PropertyNames::Yeast::laboratory               },
         {ObjectStore::FieldType::String, "notes"                       , PropertyNames::Yeast::notes                    },
         {ObjectStore::FieldType::String, "product_id"                  , PropertyNames::Yeast::productID                }, // Manufacturer's product ID, so, unlike other blah_id fields, not a foreign key!
         // ⮜⮜⮜ All below added for BeerJSON support ⮞⮞⮞
         {ObjectStore::FieldType::Double, "alcohol_tolerance_pct"       , PropertyNames::Yeast::alcoholTolerance_pct     },
         {ObjectStore::FieldType::Double, "attenuation_min_pct"         , PropertyNames::Yeast::attenuationMin_pct       },
         {ObjectStore::FieldType::Double, "attenuation_max_pct"         , PropertyNames::Yeast::attenuationMax_pct       },
         {ObjectStore::FieldType::Bool  , "phenolic_off_flavor_positive", PropertyNames::Yeast::phenolicOffFlavorPositive},
         {ObjectStore::FieldType::Bool  , "glucoamylase_positive"       , PropertyNames::Yeast::glucoamylasePositive     },
         {ObjectStore::FieldType::Bool  , "killer_producing_k1_toxin"   , PropertyNames::Yeast::killerProducingK1Toxin   },
         {ObjectStore::FieldType::Bool  , "killer_producing_k2_toxin"   , PropertyNames::Yeast::killerProducingK2Toxin   },
         {ObjectStore::FieldType::Bool  , "killer_producing_k28_toxin"  , PropertyNames::Yeast::killerProducingK28Toxin  },
         {ObjectStore::FieldType::Bool  , "killer_producing_klus_toxin" , PropertyNames::Yeast::killerProducingKlusToxin },
         {ObjectStore::FieldType::Bool  , "killer_neutral"              , PropertyNames::Yeast::killerNeutral            },
      }
   };
   template<> ObjectStore::JunctionTableDefinitions const JUNCTION_TABLES<Yeast> {
      {
         "yeast_children",
         {
            {ObjectStore::FieldType::Int, "id"                                                                              },
            {ObjectStore::FieldType::Int, "child_id",  PropertyNames::NamedEntity::key,       nullptr, &PRIMARY_TABLE<Yeast>},
            {ObjectStore::FieldType::Int, "parent_id", PropertyNames::NamedEntity::parentKey, nullptr, &PRIMARY_TABLE<Yeast>},
         },
         ObjectStore::MAX_ONE_ENTRY
      }
   };

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Database field mappings for Recipe
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   EnumStringMapping const RECIPE_STEP_TYPE_ENUM {
      {Recipe::Type::Extract    , "Extract"     },
      {Recipe::Type::PartialMash, "Partial Mash"},
      {Recipe::Type::AllGrain   , "All Grain"   },
   };
   template<> ObjectStore::TableDefinition const PRIMARY_TABLE<Recipe> {
      "recipe",
      {
         {ObjectStore::FieldType::Int   , "id"                 , PropertyNames::NamedEntity::key          },
         {ObjectStore::FieldType::String, "name"               , PropertyNames::NamedEntity::name         },
         {ObjectStore::FieldType::Bool  , "deleted"            , PropertyNames::NamedEntity::deleted      },
         {ObjectStore::FieldType::Bool  , "display"            , PropertyNames::NamedEntity::display      },
         {ObjectStore::FieldType::String, "folder"             , PropertyNames::NamedEntity::folder       },
         {ObjectStore::FieldType::Double, "age"                , PropertyNames::Recipe::age_days          },
         {ObjectStore::FieldType::Double, "age_temp"           , PropertyNames::Recipe::ageTemp_c         },
         {ObjectStore::FieldType::String, "assistant_brewer"   , PropertyNames::Recipe::asstBrewer        },
         {ObjectStore::FieldType::Double, "batch_size"         , PropertyNames::Recipe::batchSize_l       },
         {ObjectStore::FieldType::Double, "boil_size"          , PropertyNames::Recipe::boilSize_l        },
         {ObjectStore::FieldType::Double, "boil_time"          , PropertyNames::Recipe::boilTime_min      },
         {ObjectStore::FieldType::String, "brewer"             , PropertyNames::Recipe::brewer            },
         {ObjectStore::FieldType::Double, "carb_volume"        , PropertyNames::Recipe::carbonation_vols  },
         {ObjectStore::FieldType::Double, "carbonationtemp_c"  , PropertyNames::Recipe::carbonationTemp_c },
         {ObjectStore::FieldType::Date  , "date"               , PropertyNames::Recipe::date              },
         {ObjectStore::FieldType::Double, "efficiency"         , PropertyNames::Recipe::efficiency_pct    },
         {ObjectStore::FieldType::Int   , "equipment_id"       , PropertyNames::Recipe::equipmentId       , nullptr,                &PRIMARY_TABLE<Equipment>},
         {ObjectStore::FieldType::UInt  , "fermentation_stages", PropertyNames::Recipe::fermentationStages},
         {ObjectStore::FieldType::Double, "fg"                 , PropertyNames::Recipe::fg                },
         {ObjectStore::FieldType::Bool  , "forced_carb"        , PropertyNames::Recipe::forcedCarbonation },
         {ObjectStore::FieldType::Double, "keg_priming_factor" , PropertyNames::Recipe::kegPrimingFactor  },
         {ObjectStore::FieldType::Int   , "mash_id"            , PropertyNames::Recipe::mashId            , nullptr,                &PRIMARY_TABLE<Mash>},
         {ObjectStore::FieldType::String, "notes"              , PropertyNames::Recipe::notes             },
         {ObjectStore::FieldType::Double, "og"                 , PropertyNames::Recipe::og                },
         {ObjectStore::FieldType::Double, "primary_age"        , PropertyNames::Recipe::primaryAge_days   },
         {ObjectStore::FieldType::Double, "primary_temp"       , PropertyNames::Recipe::primaryTemp_c     },
         {ObjectStore::FieldType::Double, "priming_sugar_equiv", PropertyNames::Recipe::primingSugarEquiv },
         {ObjectStore::FieldType::String, "priming_sugar_name" , PropertyNames::Recipe::primingSugarName  },
         {ObjectStore::FieldType::Double, "secondary_age"      , PropertyNames::Recipe::secondaryAge_days },
         {ObjectStore::FieldType::Double, "secondary_temp"     , PropertyNames::Recipe::secondaryTemp_c   },
         {ObjectStore::FieldType::Int   , "style_id"           , PropertyNames::Recipe::styleId           , nullptr,                &PRIMARY_TABLE<Style>},
         {ObjectStore::FieldType::String, "taste_notes"        , PropertyNames::Recipe::tasteNotes        },
         {ObjectStore::FieldType::Double, "taste_rating"       , PropertyNames::Recipe::tasteRating       },
         {ObjectStore::FieldType::Double, "tertiary_age"       , PropertyNames::Recipe::tertiaryAge_days  },
         {ObjectStore::FieldType::Double, "tertiary_temp"      , PropertyNames::Recipe::tertiaryTemp_c    },
         {ObjectStore::FieldType::Enum  , "type"               , PropertyNames::Recipe::type              , &RECIPE_STEP_TYPE_ENUM},
         {ObjectStore::FieldType::Int   , "ancestor_id"        , PropertyNames::Recipe::ancestorId        , nullptr,                &PRIMARY_TABLE<Recipe>},
         {ObjectStore::FieldType::Bool  , "locked"             , PropertyNames::Recipe::locked            },
      }
   };
   template<> ObjectStore::JunctionTableDefinitions const JUNCTION_TABLES<Recipe> {
      // .:TODO:. BrewNote table stores its recipe ID, so there isn't a brewnote junction table
      // There is a lot of boiler-plate here, and we could have gone for a much more compact representation of junction
      // tables, but this keeps the definition format relatively closely aligned with that of primary tables.
      {
         "fermentable_in_recipe",
         {
            {ObjectStore::FieldType::Int, "id"                                                                                            },
            {ObjectStore::FieldType::Int, "recipe_id",         PropertyNames::NamedEntity::key,       nullptr, &PRIMARY_TABLE<Recipe>     },
            {ObjectStore::FieldType::Int, "fermentable_id",    PropertyNames::Recipe::fermentableIds, nullptr, &PRIMARY_TABLE<Fermentable>},
         }
      },
      {
         "hop_in_recipe",
         {
            {ObjectStore::FieldType::Int, "id"                                                                                            },
            {ObjectStore::FieldType::Int, "recipe_id",         PropertyNames::NamedEntity::key,       nullptr, &PRIMARY_TABLE<Recipe>     },
            {ObjectStore::FieldType::Int, "hop_id",            PropertyNames::Recipe::hopIds,         nullptr, &PRIMARY_TABLE<Hop>        },
         }
      },
      {
         "instruction_in_recipe",
         {
            {ObjectStore::FieldType::Int, "id"                                                                                            },
            {ObjectStore::FieldType::Int, "recipe_id",         PropertyNames::NamedEntity::key,       nullptr, &PRIMARY_TABLE<Recipe>     },
            {ObjectStore::FieldType::Int, "instruction_id",    PropertyNames::Recipe::instructionIds, nullptr, &PRIMARY_TABLE<Instruction>},
            {ObjectStore::FieldType::Int, "instruction_number"                                                                            },
         }
      },
      {
         "misc_in_recipe",
         {
            {ObjectStore::FieldType::Int, "id"                                                                                            },
            {ObjectStore::FieldType::Int, "recipe_id",         PropertyNames::NamedEntity::key,       nullptr, &PRIMARY_TABLE<Recipe>     },
            {ObjectStore::FieldType::Int, "misc_id",           PropertyNames::Recipe::miscIds,        nullptr, &PRIMARY_TABLE<Misc>       },
         }
      },
      {
         "salt_in_recipe",
         {
            {ObjectStore::FieldType::Int, "id"                                                                                            },
            {ObjectStore::FieldType::Int, "recipe_id",         PropertyNames::NamedEntity::key,       nullptr, &PRIMARY_TABLE<Recipe>     },
            {ObjectStore::FieldType::Int, "salt_id",           PropertyNames::Recipe::saltIds,        nullptr, &PRIMARY_TABLE<Salt>       },
         }
      },
      {
         "water_in_recipe",
         {
            {ObjectStore::FieldType::Int, "id"                                                                                            },
            {ObjectStore::FieldType::Int, "recipe_id",         PropertyNames::NamedEntity::key,       nullptr, &PRIMARY_TABLE<Recipe>     },
            {ObjectStore::FieldType::Int, "water_id",          PropertyNames::Recipe::waterIds,       nullptr, &PRIMARY_TABLE<Water>      },
         }
      },
      {
         "yeast_in_recipe",
         {
            {ObjectStore::FieldType::Int, "id"                                                                                            },
            {ObjectStore::FieldType::Int, "recipe_id",         PropertyNames::NamedEntity::key,       nullptr, &PRIMARY_TABLE<Recipe>     },
            {ObjectStore::FieldType::Int, "yeast_id",          PropertyNames::Recipe::yeastIds,       nullptr, &PRIMARY_TABLE<Yeast>      },
         }
      },
   };

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Database field mappings for BrewNote
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   template<> ObjectStore::TableDefinition const PRIMARY_TABLE<BrewNote> {
      "brewnote",
      {
         {ObjectStore::FieldType::Int   , "id"                     , PropertyNames::NamedEntity::key           },
         // NB: BrewNotes don't have names in DB
         {ObjectStore::FieldType::Bool  , "display"                , PropertyNames::NamedEntity::display       },
         {ObjectStore::FieldType::Bool  , "deleted"                , PropertyNames::NamedEntity::deleted       },
         {ObjectStore::FieldType::String, "folder"                 , PropertyNames::NamedEntity::folder        },
         {ObjectStore::FieldType::Double, "abv"                    , PropertyNames::BrewNote::abv              },
         {ObjectStore::FieldType::Double, "attenuation"            , PropertyNames::BrewNote::attenuation      },
         {ObjectStore::FieldType::Double, "boil_off"               , PropertyNames::BrewNote::boilOff_l        },
         {ObjectStore::FieldType::Date  , "brewdate"               , PropertyNames::BrewNote::brewDate         },
         {ObjectStore::FieldType::Double, "brewhouse_eff"          , PropertyNames::BrewNote::brewhouseEff_pct },
         {ObjectStore::FieldType::Double, "eff_into_bk"            , PropertyNames::BrewNote::effIntoBK_pct    },
         {ObjectStore::FieldType::Date  , "fermentdate"            , PropertyNames::BrewNote::fermentDate      },
         {ObjectStore::FieldType::Double, "fg"                     , PropertyNames::BrewNote::fg               },
         {ObjectStore::FieldType::Double, "final_volume"           , PropertyNames::BrewNote::finalVolume_l    },
         // NB: BrewNotes don't have folders, as each one is owned by a Recipe
         {ObjectStore::FieldType::Double, "mash_final_temp"        , PropertyNames::BrewNote::mashFinTemp_c    },
         {ObjectStore::FieldType::String, "notes"                  , PropertyNames::BrewNote::notes            },
         {ObjectStore::FieldType::Double, "og"                     , PropertyNames::BrewNote::og               },
         {ObjectStore::FieldType::Double, "pitch_temp"             , PropertyNames::BrewNote::pitchTemp_c      },
         {ObjectStore::FieldType::Double, "post_boil_volume"       , PropertyNames::BrewNote::postBoilVolume_l },
         {ObjectStore::FieldType::Double, "projected_abv"          , PropertyNames::BrewNote::projABV_pct      },
         {ObjectStore::FieldType::Double, "projected_atten"        , PropertyNames::BrewNote::projAtten        },
         {ObjectStore::FieldType::Double, "projected_boil_grav"    , PropertyNames::BrewNote::projBoilGrav     },
         {ObjectStore::FieldType::Double, "projected_eff"          , PropertyNames::BrewNote::projEff_pct      },
         {ObjectStore::FieldType::Double, "projected_ferm_points"  , PropertyNames::BrewNote::projFermPoints   },
         {ObjectStore::FieldType::Double, "projected_fg"           , PropertyNames::BrewNote::projFg           },
         {ObjectStore::FieldType::Double, "projected_mash_fin_temp", PropertyNames::BrewNote::projMashFinTemp_c},
         {ObjectStore::FieldType::Double, "projected_og"           , PropertyNames::BrewNote::projOg           },
         {ObjectStore::FieldType::Double, "projected_points"       , PropertyNames::BrewNote::projPoints       },
         {ObjectStore::FieldType::Double, "projected_strike_temp"  , PropertyNames::BrewNote::projStrikeTemp_c },
         {ObjectStore::FieldType::Double, "projected_vol_into_bk"  , PropertyNames::BrewNote::projVolIntoBK_l  },
         {ObjectStore::FieldType::Double, "projected_vol_into_ferm", PropertyNames::BrewNote::projVolIntoFerm_l},
         {ObjectStore::FieldType::Double, "sg"                     , PropertyNames::BrewNote::sg               },
         {ObjectStore::FieldType::Double, "strike_temp"            , PropertyNames::BrewNote::strikeTemp_c     },
         {ObjectStore::FieldType::Double, "volume_into_bk"         , PropertyNames::BrewNote::volumeIntoBK_l   },
         {ObjectStore::FieldType::Double, "volume_into_fermenter"  , PropertyNames::BrewNote::volumeIntoFerm_l },
         {ObjectStore::FieldType::Int   , "recipe_id"              , PropertyNames::BrewNote::recipeId         , nullptr, &PRIMARY_TABLE<Recipe>},
      }
   };
   // BrewNotes don't have children
   template<> ObjectStore::JunctionTableDefinitions const JUNCTION_TABLES<BrewNote> {};


   //
   // This should give us all the singleton instances
   //
   template<class NE> ObjectStoreTyped<NE> ostSingleton{NE::typeLookup, PRIMARY_TABLE<NE>, JUNCTION_TABLES<NE>};

}


template<class NE>
ObjectStoreTyped<NE> & ObjectStoreTyped<NE>::getInstance() {
   // C++11 provides a thread-safe way to ensure singleton.loadAll() is called exactly once
   //
   // NB: It's easier to just pass in nullptr to ObjectStoreTyped<NE>::loadAll than to do all the magic casting to
   //     allow std::call_once to invoke it with the default parameter (which is nullptr).
   static std::once_flag initFlag;
   std::call_once(initFlag, &ObjectStoreTyped<NE>::loadAll, &ostSingleton<NE>, nullptr);

   return ostSingleton<NE>;
}

// We have to make sure that each version of the above function gets instantiated
template ObjectStoreTyped<BrewNote> &             ObjectStoreTyped<BrewNote            >::getInstance();
template ObjectStoreTyped<Equipment> &            ObjectStoreTyped<Equipment           >::getInstance();
template ObjectStoreTyped<Fermentable> &          ObjectStoreTyped<Fermentable         >::getInstance();
template ObjectStoreTyped<Hop> &                  ObjectStoreTyped<Hop                 >::getInstance();
template ObjectStoreTyped<Instruction> &          ObjectStoreTyped<Instruction         >::getInstance();
template ObjectStoreTyped<InventoryFermentable> & ObjectStoreTyped<InventoryFermentable>::getInstance();
template ObjectStoreTyped<InventoryHop> &         ObjectStoreTyped<InventoryHop        >::getInstance();
template ObjectStoreTyped<InventoryMisc> &        ObjectStoreTyped<InventoryMisc       >::getInstance();
template ObjectStoreTyped<InventoryYeast> &       ObjectStoreTyped<InventoryYeast      >::getInstance();
template ObjectStoreTyped<Mash> &                 ObjectStoreTyped<Mash                >::getInstance();
template ObjectStoreTyped<MashStep> &             ObjectStoreTyped<MashStep            >::getInstance();
template ObjectStoreTyped<Misc> &                 ObjectStoreTyped<Misc                >::getInstance();
template ObjectStoreTyped<Recipe> &               ObjectStoreTyped<Recipe              >::getInstance();
template ObjectStoreTyped<Salt> &                 ObjectStoreTyped<Salt                >::getInstance();
template ObjectStoreTyped<Style> &                ObjectStoreTyped<Style               >::getInstance();
template ObjectStoreTyped<Water> &                ObjectStoreTyped<Water               >::getInstance();
template ObjectStoreTyped<Yeast> &                ObjectStoreTyped<Yeast               >::getInstance();

namespace {
   QVector<ObjectStore const *> AllObjectStores {
      &ostSingleton<BrewNote            >,
      &ostSingleton<Equipment           >,
      &ostSingleton<Fermentable         >,
      &ostSingleton<Hop                 >,
      &ostSingleton<Instruction         >,
      &ostSingleton<InventoryFermentable>,
      &ostSingleton<InventoryHop        >,
      &ostSingleton<InventoryMisc       >,
      &ostSingleton<InventoryYeast      >,
      &ostSingleton<Mash                >,
      &ostSingleton<MashStep            >,
      &ostSingleton<Misc                >,
      &ostSingleton<Recipe              >,
      &ostSingleton<Salt                >,
      &ostSingleton<Style               >,
      &ostSingleton<Water               >,
      &ostSingleton<Yeast               >,
   };
}

bool CreateAllDatabaseTables(Database & database, QSqlDatabase & connection) {
   qDebug() << Q_FUNC_INFO;
   for (auto ii : AllObjectStores) {
      if (!ii->createTables(database, connection)) {
         return false;
      }
   }
   for (auto jj : AllObjectStores) {
      if (!jj->addTableConstraints(database, connection)) {
         return false;
      }
   }
   return true;
}

bool WriteAllObjectStoresToNewDb(Database & newDatabase, QSqlDatabase & connectionNew) {
   //
   // Start transaction
   // By the magic of RAII, this will abort if we exit this function (including by throwing an exception) without
   // having called dbTransaction.commit().  (It will also turn foreign keys back on either way -- whether the
   // transaction is committed or rolled back.)
   //
   DbTransaction dbTransaction{newDatabase, connectionNew, DbTransaction::DISABLE_FOREIGN_KEYS};

   for (ObjectStore const * objectStore : AllObjectStores) {
      if (!objectStore->writeAllToNewDb(newDatabase, connectionNew)) {
         return false;
      }
   }

   dbTransaction.commit();
   return true;
}
