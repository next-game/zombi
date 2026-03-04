class CfgPatches
{
    class InediaInfectedAI
    {
        requiredAddons[] = {"DZ_Data", "DZ_Characters", "DZ_Scripts"};
    };
};

class CfgMods
{
    class InediaInfectedAI
    {
        type = "mod";
        name = "InediaInfectedAI";
        author = "Yauhen Saroka yauhen.saroka@gmail.com";
        authorID = 76561198047426689;
        defines[] = {INEDIA_INFECTEDAI};
        dependencies[] = {"Game", "World", "Mission"};

        class defs
        {
            class imageSets {
                files[] = {"InediaInfectedAI/gui/imagesets/inediainfectedai_slots.imageset"};
            };

            class gameScriptModule
            {
                value = "";
                files[] = {"InediaInfectedAI/scripts/3_Game"};
            };

            class worldScriptModule
            {
                value = "";
                files[] = {"InediaInfectedAI/scripts/4_World"};
            };

            class missionScriptModule {
                value = "";
                files[] = {"InediaInfectedAI/scripts/5_mission"};
            };
        };
    };
};

class cfgLiquidDefinitions
{
    class Vodka
    {
        class InediaPain
        {
            ShowTips=1;

            IsPainkillerLightItem=1;
            PainkillerLightItemDurationSeconds=120;
        };
    };
};

class CfgVehicles
{
    class Inventory_Base;
    class Container_Base;
    class Edible_Base;
    class Clothing;

    class InediaPain_FirstAidCase_ColorBase: Container_Base
    {
        scope=2;
        displayName="$STR_inediainfectedai_firstaid_case_display_name";
        descriptionShort="$STR_inediainfectedai_firstaid_case_description_short";
        model="\dz\gear\containers\Protector_Case.p3d";
        rotationFlags=17;
        weight=980;
        itemSize[]={4,4};
        itemsCargoSize[]={4,2};
        canBeDigged=1;
        isMeleeWeapon=1;
        allowOwnedCargoManipulation=1;
        randomQuantity=2;
        repairableWithKits[]={8};
        repairCosts[]={25};

        hiddenSelections[]={"zbytek"};
        hiddenSelectionsMaterials[]={"DZ\gear\containers\data\Protector_Case.rvmat"};
        hiddenSelectionsTextures[]={"InediaInfectedAI\gear\medical\data\inediapain_firstaid_case_white_co.paa"};

        attachments[]+=
        {
            "InediaPain_Pills1",
            "InediaPain_Pills2",
            "InediaPain_Pills3",
            "InediaPain_Pills4",
            "InediaPain_Pills5",
            "InediaPain_Pills6",
            "InediaPain_Pills7",
            "InediaPain_Pills8",
            "InediaPain_SurgicalKits1",
            "InediaPain_SurgicalKits2",
            "InediaPain_Splints1",
            "InediaPain_BloodSupplies1",
            "InediaPain_BloodSupplies2",
            "InediaPain_BloodSupplies3",
            "Rags",
            "InediaPain_Disinfectants1",
            "InediaPain_Bandages1",
            "InediaPain_Bandages2",
            "InediaPain_Bandages3",
            "InediaPain_Bandages4",
            "InediaPain_Injectors1",
            "InediaPain_Injectors2",
            "InediaPain_Injectors3",
            "InediaPain_Injectors4"
        };

        inventorySlot[]+=
        {
            "Belt_Left"
        };

        class DamageSystem {
            class GlobalHealth {
                class Health {
                    hitpoints=750;
                    healthLevels[]={{1,{"DZ\gear\containers\data\Protector_Case.rvmat"}},{0.7,{"DZ\gear\containers\data\Protector_Case.rvmat"}},{0.5,{"DZ\gear\containers\data\Protector_Case_damage.rvmat"}},{0.3,{"DZ\gear\containers\data\Protector_Case_damage.rvmat"}},{0,{"DZ\gear\containers\data\Protector_Case_destruct.rvmat"}}};
                };
            };
        };

        soundImpactType="plastic";
        class AnimEvents
        {
            class SoundWeapon
            {
                class pickUpItem
                {
                    soundSet="smallprotectorcase_pickup_SoundSet";
                    id=797;
                };
                class drop
                {
                    soundset="smallprotectorcase_drop_SoundSet";
                    id=898;
                };
            };
        };

        class InventorySlotsOffsets
        {
            class Belt_Left
            {
                position[]={0,-0.05,0}; // Position offset: X (left/right), Y (up/down), Z (forward/backward)
                orientation[]={0,180,180}; // Rotation: X (tilt forward/backward), Y (rotate left/right), Z (tilt sideways)
            };
        };
    };

    class InediaPain_FirstAidCase_White: InediaPain_FirstAidCase_ColorBase
    {
        hiddenSelectionsTextures[]={"InediaInfectedAI\gear\medical\data\inediapain_firstaid_case_white_co.paa"};
    }

    class InediaPain_FirstAidCase_Black: InediaPain_FirstAidCase_ColorBase
    {
        hiddenSelectionsTextures[]={"InediaInfectedAI\gear\medical\data\inediapain_firstaid_case_black_co.paa"};
    }

    class InediaPain_FirstAidCase_Camouflage: InediaPain_FirstAidCase_ColorBase
    {
        hiddenSelectionsTextures[]={"InediaInfectedAI\gear\medical\data\inediapain_firstaid_case_camouflage_co.paa"};
    }

    class InediaPain_FirstAidKit_ColorBase: Container_Base
    {
        scope=2;
        displayName="$STR_inediainfectedai_firstaid_kit_display_name";
        descriptionShort="$STR_inediainfectedai_firstaid_kit_description_short";
        model="\dz\gear\containers\FirstAidKit.p3d";
        rotationFlags=17;
        weight=140;
        itemSize[]={3,3};
        itemsCargoSize[]={3,2};
        allowOwnedCargoManipulation=1;
        randomQuantity=2;
        canBeDigged=1;
        repairableWithKits[]={2};
        repairCosts[]={25};
        isMeleeWeapon=1;

        hiddenSelections[]={"zbytek"};
        hiddenSelectionsMaterials[]={"DZ\gear\containers\data\FirsAidKit.rvmat"};
        hiddenSelectionsTextures[]={"InediaInfectedAI\gear\medical\data\inediapain_firstaid_kit_white_co.paa"};

        attachments[]+=
        {
            "InediaPain_Pills1",
            "InediaPain_Pills2",
            "InediaPain_Pills3",
            "InediaPain_Bandages1",
            "InediaPain_Injectors1",
            "InediaPain_SurgicalKits1",
            "InediaPain_BloodSupplies1",
            "InediaPain_Disinfectants1"
        };

        inventorySlot[]+=
        {
            "Belt_Left"
        };

        class DamageSystem {
            class GlobalHealth {
                class Health {
                    hitpoints=80;
                    healthLevels[]={{1,{"DZ\gear\containers\data\FirsAidKit.rvmat"}},{0.69999999,{"DZ\gear\containers\data\FirsAidKit.rvmat"}},{0.5,{"DZ\gear\containers\data\FirsAidKit_damage.rvmat"}},{0.30000001,{"DZ\gear\containers\data\FirsAidKit_damage.rvmat"}},{0,{"DZ\gear\containers\data\FirsAidKit_destruct.rvmat"}}};
                };
            };
        };

        soundImpactType="textile";
        class AnimEvents
        {
            class SoundWeapon
            {
                class pickUpItem_Light
                {
                    soundSet="pickUpCourierBag_Light_SoundSet";
                    id=796;
                };
                class pickUpItem
                {
                    soundSet="pickUpCourierBag_SoundSet";
                    id=797;
                };
            };
        };

        class InventorySlotsOffsets
        {
            class Belt_Left
            {
                position[]={0,-0.05,0}; // Position offset: X (left/right), Y (up/down), Z (forward/backward)
                orientation[]={0,180,180}; // Rotation: X (tilt forward/backward), Y (rotate left/right), Z (tilt sideways)
            };
        };
    };

    class InediaPain_FirstAidKit_White: InediaPain_FirstAidKit_ColorBase
    {
        hiddenSelectionsTextures[]={"InediaInfectedAI\gear\medical\data\inediapain_firstaid_kit_white_co.paa"};
    }

    class InediaPain_FirstAidKit_Black: InediaPain_FirstAidKit_ColorBase
    {
        hiddenSelectionsTextures[]={"InediaInfectedAI\gear\medical\data\inediapain_firstaid_kit_black_co.paa"};
    }

    class InediaPain_FirstAidKit_Camouflage: InediaPain_FirstAidKit_ColorBase
    {
        hiddenSelectionsTextures[]={"InediaInfectedAI\gear\medical\data\inediapain_firstaid_kit_camouflage_co.paa"};
    }

    class InediaPain_Tablet1Base: Edible_Base
    {
        scope=2;
        model="\dz\gear\medical\painkillers2.p3d";
        debug_ItemCategory=7;
        rotationFlags=17;
        itemSize[]={1,2};
        canBeSplit=1;
        varQuantityInit=12;
        varQuantityMin=0;
        varQuantityMax=12;
        varQuantityDestroyOnMin=1;
        weight=20;
        weightPerQuantityUnit=2;
        stackedUnit="pills";
        quantityBar=0;
        varTemperatureMin=0;
        varTemperatureMax=0;
        absorbency=0.69999999;
        hiddenSelections[]={"zbytek"};
        hiddenSelectionsMaterials[]={"dz\gear\medical\data\painkiller2.rvmat"};

        inventorySlot[]+=
        {
            "InediaPain_Pills1",
            "InediaPain_Pills2",
            "InediaPain_Pills3",
            "InediaPain_Pills4",
            "InediaPain_Pills5",
            "InediaPain_Pills6",
            "InediaPain_Pills7",
            "InediaPain_Pills8"
        };

        class DamageSystem
        {
            class GlobalHealth
            {
                class Health
                {
                    hitpoints=30;
                    healthLevels[]={{1,{"DZ\gear\medical\data\painkiller2.rvmat"}},{0.69999999,{"DZ\gear\medical\data\painkiller2.rvmat"}},{0.5,{"DZ\gear\medical\data\painkiller2_damage.rvmat"}},{0.30000001,{"DZ\gear\medical\data\painkiller2_damage.rvmat"}},{0,{"DZ\gear\medical\data\painkiller2_destruct.rvmat"}}};
                };
            };
        };

        class AnimEvents { class SoundWeapon { class Tablets_open { soundSet="Tablets_open_SoundSet"; id=201; }; class Tablets_catch { soundSet="Tablets_catch_SoundSet"; id=202; }; class Tablets_close { soundSet="Tablets_close_SoundSet"; id=203; }; class pickup { soundSet="purificationtablets_pickup_SoundSet"; id=797; }; class drop { soundset="purificationtablets_drop_SoundSet"; id=898; }; }; };
    };

    class InediaPain_Tablet2Base: Edible_Base
    {
        scope=2;
        model="\dz\gear\medical\charcoal_tablets.p3d";
        debug_ItemCategory=7;
        rotationFlags=17;
        canBeSplit=1;
        varQuantityInit=12;
        varQuantityMin=0;
        varQuantityMax=12;
        varQuantityDestroyOnMin=1;
        varTemperatureMin=0;
        varTemperatureMax=0;
        itemSize[]={1,2};
        weight=20;
        weightPerQuantityUnit=2;
        stackedUnit="pills";
        quantityBar=0;
        absorbency=0.89999998;
        hiddenSelections[]={"zbytek"};
        hiddenSelectionsMaterials[]={"dz\gear\medical\data\charcoal_tablets.rvmat"};

        inventorySlot[]+=
        {
            "InediaPain_Pills1",
            "InediaPain_Pills2",
            "InediaPain_Pills3",
            "InediaPain_Pills4",
            "InediaPain_Pills5",
            "InediaPain_Pills6",
            "InediaPain_Pills7",
            "InediaPain_Pills8"
        };

        class DamageSystem
        {
            class GlobalHealth
            {
                class Health
                {
                    hitpoints=30;
                    healthLevels[] = {{1,{"DZ\gear\medical\data\charcoal_tablets.rvmat"}},{0.69999999,{"DZ\gear\medical\data\charcoal_tablets.rvmat"}},{0.5,{"DZ\gear\medical\data\charcoal_tablets_damage.rvmat"}},{0.30000001,{"DZ\gear\medical\data\charcoal_tablets_damage.rvmat"}},{0,{"DZ\gear\medical\data\charcoal_tablets_destruct.rvmat"}}};
                };
            };
        };

        class AnimEvents { class SoundWeapon { class Tablets_open { soundSet="Tablets_open_SoundSet"; id=201; }; class Tablets_catch { soundSet="Tablets_catch_SoundSet"; id=202; }; class Tablets_close { soundSet="Tablets_close_SoundSet"; id=203; }; class pickup { soundSet="purificationtablets_pickup_SoundSet"; id=797; }; class drop { soundset="purificationtablets_drop_SoundSet"; id=898; }; }; };
    }

    class Morphine: Inventory_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_Injectors1",
            "InediaPain_Injectors2",
            "InediaPain_Injectors3",
            "InediaPain_Injectors4"
        };

        class InediaPain
        {
            ShowTips=1;

            IsPainkillerStrongItem=1;
            PainkillerStrongItemDurationSeconds=90;

            IsHealthRegenItem=1;
            HealthRegenItemDurationSeconds=90;
            HealthRegenItemRegenMultiplier=20;
        }
    }

    class Epinephrine: Inventory_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_Injectors1",
            "InediaPain_Injectors2",
            "InediaPain_Injectors3",
            "InediaPain_Injectors4"
        };

        class InediaPain
        {
            ShowTips=1;

            IsPainkillerLightItem=1;
            PainkillerLightItemDurationSeconds=60;
        }
    }

    class AntiChemInjector: Inventory_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_Injectors1",
            "InediaPain_Injectors2",
            "InediaPain_Injectors3",
            "InediaPain_Injectors4"
        };
    }

    class PainkillerTablets: Edible_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_Pills1",
            "InediaPain_Pills2",
            "InediaPain_Pills3",
            "InediaPain_Pills4",
            "InediaPain_Pills5",
            "InediaPain_Pills6",
            "InediaPain_Pills7",
            "InediaPain_Pills8"
        };

        class InediaPain
        {
            ShowTips=1;

            IsPainkillerLightItem=1;
            PainkillerLightItemDurationSeconds=300;

            IsHealthRegenItem=1;
            HealthRegenItemDurationSeconds=300;
            HealthRegenItemRegenMultiplier=2;
        }
    }

    class CharcoalTablets: Edible_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_Pills1",
            "InediaPain_Pills2",
            "InediaPain_Pills3",
            "InediaPain_Pills4",
            "InediaPain_Pills5",
            "InediaPain_Pills6",
            "InediaPain_Pills7",
            "InediaPain_Pills8"
        };
    }

    class TetracyclineAntibiotics: Edible_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_Pills1",
            "InediaPain_Pills2",
            "InediaPain_Pills3",
            "InediaPain_Pills4",
            "InediaPain_Pills5",
            "InediaPain_Pills6",
            "InediaPain_Pills7",
            "InediaPain_Pills8"
        };
    }

    class ChelatingTablets: Edible_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_Pills1",
            "InediaPain_Pills2",
            "InediaPain_Pills3",
            "InediaPain_Pills4",
            "InediaPain_Pills5",
            "InediaPain_Pills6",
            "InediaPain_Pills7",
            "InediaPain_Pills8"
        };
    }

    class VitaminBottle: Edible_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_Pills1",
            "InediaPain_Pills2",
            "InediaPain_Pills3",
            "InediaPain_Pills4",
            "InediaPain_Pills5",
            "InediaPain_Pills6",
            "InediaPain_Pills7",
            "InediaPain_Pills8"
        };
    }

    class PurificationTablets: Edible_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_Pills1",
            "InediaPain_Pills2",
            "InediaPain_Pills3",
            "InediaPain_Pills4",
            "InediaPain_Pills5",
            "InediaPain_Pills6",
            "InediaPain_Pills7",
            "InediaPain_Pills8"
        };
    }

    class Splint: Inventory_Base
    {
        class InediaPain
        {
            ShowTips=1;

            IsFractureStabilizationItem=1;
            FractureStabilizationItemAllowedLimbs[]={"arms", "legs"};
            FractureStabilizationItemShockReductionMultiplier = 1.5;
            FractureStabilizationItemRegenerationMultiplier = 1.5;
            FractureStabilizationItemActionTimeSeconds=10;
            FractureStabilizationItemActionText="#STR_inediainfectedai_stabilize_break";
            FractureStabilizationItemActionAnimationId=59; // 59 - craft, 71 - sew, 58 - bandage, see DayZPlayerConstants.CMD_ACTIONFB_* constants
            FractureStabilizationItemQuantityPerUse=1;
        }
    }

    class BandageDressing: Inventory_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_Bandages1",
            "InediaPain_Bandages2",
            "InediaPain_Bandages3",
            "InediaPain_Bandages4"
        };

        class InediaPain
        {
            ShowTips=1;

            IsDeepWoundBandageItem=1;
            DeepWoundBandageItemAllowedLimbs[]={"head", "arms", "legs", "torso"};
            DeepWoundBandageItemEffectiveDurationSeconds = 600;
            DeepWoundBandageItemRegenerationMultiplier = 1.5;
            DeepWoundBandageItemActionTimeSeconds=5;
            DeepWoundBandageItemActionText="#STR_inediainfectedai_bandage_wound";
            DeepWoundBandageItemActionAnimationId=58; // 59 - craft, 71 - sew, 58 - bandage, see DayZPlayerConstants.CMD_ACTIONFB_* constants
            DeepWoundBandageItemQuantityPerUse=1;
        }
    }

    class Rag: Inventory_Base
    {
        class InediaPain
        {
            ShowTips=1;

            IsDeepWoundBandageItem=1;
            DeepWoundBandageItemAllowedLimbs[]={"head", "arms", "legs", "torso"};
            DeepWoundBandageItemEffectiveDurationSeconds = 300;
            DeepWoundBandageItemRegenerationMultiplier = 1;
            DeepWoundBandageItemActionTimeSeconds=10;
            DeepWoundBandageItemActionText="#STR_inediainfectedai_bandage_wound";
            DeepWoundBandageItemActionAnimationId=58; // 59 - craft, 71 - sew, 58 - bandage, see DayZPlayerConstants.CMD_ACTIONFB_* constants
            DeepWoundBandageItemQuantityPerUse=1;
        }
    }

    class Bandana_ColorBase: Clothing
    {
        class InediaPain
        {
            ShowTips=1;

            IsDeepWoundBandageItem=1;
            DeepWoundBandageItemAllowedLimbs[]={"head", "arms", "legs", "torso"};
            DeepWoundBandageItemEffectiveDurationSeconds = 300;
            DeepWoundBandageItemRegenerationMultiplier = 1;
            DeepWoundBandageItemActionTimeSeconds=10;
            DeepWoundBandageItemActionText="#STR_inediainfectedai_bandage_wound";
            DeepWoundBandageItemActionAnimationId=58; // 59 - craft, 71 - sew, 58 - bandage, see DayZPlayerConstants.CMD_ACTIONFB_* constants
            DeepWoundBandageItemQuantityPerUse=1;
        }
    }

    class SewingKit: Inventory_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_SurgicalKits1",
            "InediaPain_SurgicalKits2"
        };

        class InediaPain
        {
            ShowTips=1;

            IsDeepWoundClosureItem=1;
            DeepWoundClosureItemAllowedLimbs[]={"head", "arms", "legs", "torso"};
            DeepWoundClosureItemRequiredPainkillerLevel=1;
            DeepWoundClosureItemActionTimeSeconds=10;
            DeepWoundClosureItemActionText="#STR_inediainfectedai_wound_closure_tool_close_wound";
            DeepWoundClosureItemActionAnimationId=71; // 59 - craft, 71 - sew, 58 - bandage, see DayZPlayerConstants.CMD_ACTIONFB_* constants
            DeepWoundClosureItemQuantityPerUse=20;
            DeepWoundClosureItemSuccessChancePercent=30;
            DeepWoundClosureItemAssistSuccessChancePercent=40;
            DeepWoundClosureItemProcedureBloodLossMl=150;
        }
    }

    class SteakKnife: Inventory_Base
    {
        class InediaPain
        {
            ShowTips=1;

            IsExtractBulletItem=1;
            ExtractBulletItemAllowedLimbs[]={"head", "arms", "legs", "torso"};
            ExtractBulletItemRequiredPainkillerLevel=2;
            ExtractBulletItemActionTimeSeconds=10;
            ExtractBulletItemActionText="#STR_inediainfectedai_bullet_extraction_tool_extract_bullet";
            ExtractBulletItemActionAnimationId=71; // 59 - craft, 71 - sew, 58 - bandage, see DayZPlayerConstants.CMD_ACTIONFB_* constants
            ExtractBulletItemQuantityPerUse=0;
            ExtractBulletItemSuccessChancePercent=20;
            ExtractBulletItemAssistSuccessChancePercent=30;
            ExtractBulletItemProcedureBloodLossMl=300;
        }
    }

    class KitchenKnife: Inventory_Base
    {
        class InediaPain
        {
            ShowTips=1;

            IsExtractBulletItem=1;
            ExtractBulletItemAllowedLimbs[]={"head", "arms", "legs", "torso"};
            ExtractBulletItemRequiredPainkillerLevel=2;
            ExtractBulletItemActionTimeSeconds=10;
            ExtractBulletItemActionText="#STR_inediainfectedai_bullet_extraction_tool_extract_bullet";
            ExtractBulletItemActionAnimationId=71; // 59 - craft, 71 - sew, 58 - bandage, see DayZPlayerConstants.CMD_ACTIONFB_* constants
            ExtractBulletItemQuantityPerUse=0;
            ExtractBulletItemSuccessChancePercent=20;
            ExtractBulletItemAssistSuccessChancePercent=30;
            ExtractBulletItemProcedureBloodLossMl=300;
        }
    }

    class CombatKnife: Inventory_Base
    {
        class InediaPain
        {
            ShowTips=1;

            IsExtractBulletItem=1;
            ExtractBulletItemAllowedLimbs[]={"head", "arms", "legs", "torso"};
            ExtractBulletItemRequiredPainkillerLevel=2;
            ExtractBulletItemActionTimeSeconds=10;
            ExtractBulletItemActionText="#STR_inediainfectedai_bullet_extraction_tool_extract_bullet";
            ExtractBulletItemActionAnimationId=71; // 59 - craft, 71 - sew, 58 - bandage, see DayZPlayerConstants.CMD_ACTIONFB_* constants
            ExtractBulletItemQuantityPerUse=0;
            ExtractBulletItemSuccessChancePercent=20;
            ExtractBulletItemAssistSuccessChancePercent=30;
            ExtractBulletItemProcedureBloodLossMl=300;
        }
    }

    class HuntingKnife: Inventory_Base
    {
        class InediaPain
        {
            ShowTips=1;

            IsExtractBulletItem=1;
            ExtractBulletItemAllowedLimbs[]={"head", "arms", "legs", "torso"};
            ExtractBulletItemRequiredPainkillerLevel=2;
            ExtractBulletItemActionTimeSeconds=10;
            ExtractBulletItemActionText="#STR_inediainfectedai_bullet_extraction_tool_extract_bullet";
            ExtractBulletItemActionAnimationId=71; // 59 - craft, 71 - sew, 58 - bandage, see DayZPlayerConstants.CMD_ACTIONFB_* constants
            ExtractBulletItemQuantityPerUse=0;
            ExtractBulletItemSuccessChancePercent=20;
            ExtractBulletItemAssistSuccessChancePercent=30;
            ExtractBulletItemProcedureBloodLossMl=300;
        }
    }

    class FangeKnife: Inventory_Base
    {
        class InediaPain
        {
            ShowTips=1;

            IsExtractBulletItem=1;
            ExtractBulletItemAllowedLimbs[]={"head", "arms", "legs", "torso"};
            ExtractBulletItemRequiredPainkillerLevel=2;
            ExtractBulletItemActionTimeSeconds=10;
            ExtractBulletItemActionText="#STR_inediainfectedai_bullet_extraction_tool_extract_bullet";
            ExtractBulletItemActionAnimationId=71; // 59 - craft, 71 - sew, 58 - bandage, see DayZPlayerConstants.CMD_ACTIONFB_* constants
            ExtractBulletItemQuantityPerUse=0;
            ExtractBulletItemSuccessChancePercent=20;
            ExtractBulletItemAssistSuccessChancePercent=30;
            ExtractBulletItemProcedureBloodLossMl=300;
        }
    }

    class KukriKnife: Inventory_Base
    {
        class InediaPain
        {
            ShowTips=1;

            IsExtractBulletItem=1;
            ExtractBulletItemAllowedLimbs[]={"head", "arms", "legs", "torso"};
            ExtractBulletItemRequiredPainkillerLevel=2;
            ExtractBulletItemActionTimeSeconds=10;
            ExtractBulletItemActionText="#STR_inediainfectedai_bullet_extraction_tool_extract_bullet";
            ExtractBulletItemActionAnimationId=71; // 59 - craft, 71 - sew, 58 - bandage, see DayZPlayerConstants.CMD_ACTIONFB_* constants
            ExtractBulletItemQuantityPerUse=0;
            ExtractBulletItemSuccessChancePercent=20;
            ExtractBulletItemAssistSuccessChancePercent=30;
            ExtractBulletItemProcedureBloodLossMl=300;
        }
    }

    class AK_Bayonet: Inventory_Base
    {
        class InediaPain
        {
            ShowTips=1;

            IsExtractBulletItem=1;
            ExtractBulletItemAllowedLimbs[]={"head", "arms", "legs", "torso"};
            ExtractBulletItemRequiredPainkillerLevel=2;
            ExtractBulletItemActionTimeSeconds=10;
            ExtractBulletItemActionText="#STR_inediainfectedai_bullet_extraction_tool_extract_bullet";
            ExtractBulletItemActionAnimationId=71; // 59 - craft, 71 - sew, 58 - bandage, see DayZPlayerConstants.CMD_ACTIONFB_* constants
            ExtractBulletItemQuantityPerUse=0;
            ExtractBulletItemSuccessChancePercent=20;
            ExtractBulletItemAssistSuccessChancePercent=30;
            ExtractBulletItemProcedureBloodLossMl=300;
        }
    }

    class M9A1_Bayonet: Inventory_Base
    {
        class InediaPain
        {
            ShowTips=1;

            IsExtractBulletItem=1;
            ExtractBulletItemAllowedLimbs[]={"head", "arms", "legs", "torso"};
            ExtractBulletItemRequiredPainkillerLevel=2;
            ExtractBulletItemActionTimeSeconds=10;
            ExtractBulletItemActionText="#STR_inediainfectedai_bullet_extraction_tool_extract_bullet";
            ExtractBulletItemActionAnimationId=71; // 59 - craft, 71 - sew, 58 - bandage, see DayZPlayerConstants.CMD_ACTIONFB_* constants
            ExtractBulletItemQuantityPerUse=0;
            ExtractBulletItemSuccessChancePercent=20;
            ExtractBulletItemAssistSuccessChancePercent=30;
            ExtractBulletItemProcedureBloodLossMl=300;
        }
    }

    class Mosin_Bayonet: Inventory_Base
    {
        class InediaPain
        {
            ShowTips=1;

            IsExtractBulletItem=1;
            ExtractBulletItemAllowedLimbs[]={"head", "arms", "legs", "torso"};
            ExtractBulletItemRequiredPainkillerLevel=2;
            ExtractBulletItemActionTimeSeconds=10;
            ExtractBulletItemActionText="#STR_inediainfectedai_bullet_extraction_tool_extract_bullet";
            ExtractBulletItemActionAnimationId=71; // 59 - craft, 71 - sew, 58 - bandage, see DayZPlayerConstants.CMD_ACTIONFB_* constants
            ExtractBulletItemQuantityPerUse=0;
            ExtractBulletItemSuccessChancePercent=20;
            ExtractBulletItemAssistSuccessChancePercent=30;
            ExtractBulletItemProcedureBloodLossMl=300;
        }
    }

    class SKS_Bayonet: Inventory_Base
    {
        class InediaPain
        {
            ShowTips=1;

            IsExtractBulletItem=1;
            ExtractBulletItemAllowedLimbs[]={"head", "arms", "legs", "torso"};
            ExtractBulletItemRequiredPainkillerLevel=2;
            ExtractBulletItemActionTimeSeconds=10;
            ExtractBulletItemActionText="#STR_inediainfectedai_bullet_extraction_tool_extract_bullet";
            ExtractBulletItemActionAnimationId=71; // 59 - craft, 71 - sew, 58 - bandage, see DayZPlayerConstants.CMD_ACTIONFB_* constants
            ExtractBulletItemQuantityPerUse=0;
            ExtractBulletItemSuccessChancePercent=20;
            ExtractBulletItemAssistSuccessChancePercent=30;
            ExtractBulletItemProcedureBloodLossMl=300;
        }
    }

    class InediaPain_HemostaticDressing: Inventory_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_Bandages1",
            "InediaPain_Bandages2",
            "InediaPain_Bandages3",
            "InediaPain_Bandages4"
        };

        class InediaPain
        {
            ShowTips=1;

            IsDeepWoundBandageItem=1;
            DeepWoundBandageItemAllowedLimbs[]={"head", "arms", "legs", "torso"};
            DeepWoundBandageItemEffectiveDurationSeconds = 3600;
            DeepWoundBandageItemRegenerationMultiplier = 3;
            DeepWoundBandageItemActionTimeSeconds=5;
            DeepWoundBandageItemActionText="#STR_inediainfectedai_bandage_wound";
            DeepWoundBandageItemActionAnimationId=58; // 59 - craft, 71 - sew, 58 - bandage, see DayZPlayerConstants.CMD_ACTIONFB_* constants
            DeepWoundBandageItemQuantityPerUse=1;

            IsCutClosureItem=1;
            CutClosureItemActionTimeSeconds=3;
            CutClosureItemActionText="#STR_inediainfectedai_remove_scratch";
            CutClosureItemActionAnimationId=58;
            CutClosureItemQuantityPerUse=1;
        }

        scope=2;
        displayName="$STR_inediainfectedai_hemostatic_dressing_display_name";
        descriptionShort="$STR_inediainfectedai_hemostatic_dressing_description_short";
        model="\dz\gear\medical\StartKit.p3d";
        debug_ItemCategory=7;
        itemSize[]={1,2};
        rotationFlags=17;
        weight=100;
        isMeleeWeapon=1;
        varCleannessInit=1;
        soundImpactType="plastic";
        hiddenSelections[]={"zbytek"};
        hiddenSelectionsMaterials[]={"DZ\gear\medical\data\start_kit.rvmat"};
        hiddenSelectionsTextures[]={"InediaInfectedAI\gear\medical\data\inediapain_hemostatic_dressing_co.paa"};

        class DamageSystem
        {
            class GlobalHealth
            {
                class Health
                {
                    hitpoints=20;
                    healthLevels[]={{1,{"DZ\gear\medical\data\start_kit.rvmat"}},{0.69999999,{"DZ\gear\medical\data\start_kit.rvmat"}},{0.5,{"DZ\gear\medical\data\start_kit_damage.rvmat"}},{0.30000001,{"DZ\gear\medical\data\start_kit_damage.rvmat"}},{0,{"DZ\gear\medical\data\start_kit_destruct.rvmat"}}};
                };
            };
        };
        class AnimEvents { class SoundWeapon { class Bandage_loop1 { soundSet="Bandage_loop_SoundSet"; id=201; }; class Bandage_loop2 { soundSet="Bandage_loop_SoundSet"; id=202; }; class Bandage_loop3 { soundSet="Bandage_loop_SoundSet"; id=203; }; class Bandage_start { soundSet="Bandage_start_SoundSet"; id=204; }; class Bandage_end { soundSet="Bandage_end_SoundSet"; id=205; }; }; };
    }

    class InediaPain_Tramadol: InediaPain_Tablet2Base
    {
        class InediaPain
        {
            ShowTips=1;

            IsPainkillerMediumItem=1;
            PainkillerMediumItemDurationSeconds=180;

            IsHealthRegenItem=1;
            HealthRegenItemDurationSeconds=180;
            HealthRegenItemRegenMultiplier=4;
        }

        displayName="$STR_inediainfectedai_tramadol_display_name";
        descriptionShort="$STR_inediainfectedai_tramadol_description_short";
        varQuantityInit=10;
        varQuantityMin=0;
        varQuantityMax=10;
        weight=20;
        weightPerQuantityUnit=0.05;

        hiddenSelectionsTextures[]={"InediaInfectedAI\gear\medical\data\inediapain_tramadol_co.paa"};
    }

    class InediaPain_Calcium : InediaPain_Tablet1Base
    {
        class InediaPain
        {
            ShowTips=1;

            IsBoneRegenItem=1;
            BoneRegenItemDurationSeconds=300;
            BoneRegenItemRegenMultiplier=2;
        }

        displayName="$STR_inediainfectedai_calcium_display_name";
        descriptionShort="$STR_inediainfectedai_calcium_description_short";
        model="\dz\gear\medical\painkillers2.p3d";
        varQuantityInit=10;
        varQuantityMin=0;
        varQuantityMax=10;
        weight=20;
        weightPerQuantityUnit=0.5;

        hiddenSelectionsTextures[]={"InediaInfectedAI\gear\medical\data\inediapain_calcium_co.paa"};
    };
    class InediaInfectedAI_Calcium: InediaPain_Calcium {}; // for backward compatibility

    class InediaPain_Sorbifer: InediaPain_Tablet2Base
    {
        class InediaPain
        {
            ShowTips=1;

            IsBloodRegenItem=1;
            BloodRegenItemDurationSeconds=300;
            BloodRegenItemRegenMultiplier=5;
        }

        displayName="$STR_inediainfectedai_sorbifer_display_name";
        descriptionShort="$STR_inediainfectedai_sorbifer_description_short";
        varQuantityInit=10;
        varQuantityMin=0;
        varQuantityMax=10;
        weight=20;
        weightPerQuantityUnit=0.3;

        hiddenSelectionsTextures[]={"InediaInfectedAI\gear\medical\data\inediapain_sorbifer_co.paa"};
    }

    class InediaPain_Tranexam: InediaPain_Tablet2Base
    {
        class InediaPain
        {
            ShowTips=1;

            IsDeepWoundRegenItem=1;
            DeepWoundRegenItemDurationSeconds=300;
            DeepWoundRegenItemRegenMultiplier=2;

            IsBloodClottingItem=1;
            BloodClottingItemDurationSeconds=300;
            BloodClottingItemClottingMultiplier=2;
        }

        displayName="$STR_inediainfectedai_tranexam_display_name";
        descriptionShort="$STR_inediainfectedai_tranexam_description_short";
        varQuantityInit=10;
        varQuantityMin=0;
        varQuantityMax=10;
        weight=20;
        weightPerQuantityUnit=0.5;

        hiddenSelectionsTextures[]={"InediaInfectedAI\gear\medical\data\inediapain_tranexam_co.paa"};
    }
    class InediaPain_Vikasol: InediaPain_Tranexam {}; // for backward compatibility

    class InediaPain_Metoclopramide: InediaPain_Tablet2Base
    {
        class InediaPain
        {
            ShowTips=1;

            IsVomitReliefItem=1;
            VomitReliefItemDurationSeconds=300;
        }

        displayName="$STR_inediainfectedai_metoclopramide_display_name";
        descriptionShort="$STR_inediainfectedai_metoclopramide_description_short";
        varQuantityInit=10;
        varQuantityMin=0;
        varQuantityMax=10;
        weight=20;
        weightPerQuantityUnit=0.01;

        hiddenSelectionsTextures[]={"InediaInfectedAI\gear\medical\data\inediapain_metoclopramide_co.paa"};
    }

    class InediaPain_SurgicalKit : Inventory_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_SurgicalKits1",
            "InediaPain_SurgicalKits2"
        };

        class InediaPain
        {
            ShowTips=1;

            IsDeepWoundClosureItem=1;
            DeepWoundClosureItemAllowedLimbs[]={"head", "arms", "legs", "torso"};
            DeepWoundClosureItemRequiredPainkillerLevel=1;
            DeepWoundClosureItemActionTimeSeconds=10;
            DeepWoundClosureItemActionText="#STR_inediainfectedai_wound_closure_tool_close_wound";
            DeepWoundClosureItemActionAnimationId=71; // 59 - craft, 71 - sew, 58 - bandage, see DayZPlayerConstants.CMD_ACTIONFB_* constants
            DeepWoundClosureItemQuantityPerUse=20;
            DeepWoundClosureItemSuccessChancePercent=90;
            DeepWoundClosureItemAssistSuccessChancePercent=100;
            DeepWoundClosureItemProcedureBloodLossMl=50;

            IsExtractBulletItem=1;
            ExtractBulletItemAllowedLimbs[]={"head", "arms", "legs", "torso"};
            ExtractBulletItemRequiredPainkillerLevel=2;
            ExtractBulletItemActionTimeSeconds=10;
            ExtractBulletItemActionText="#STR_inediainfectedai_bullet_extraction_tool_extract_bullet";
            ExtractBulletItemActionAnimationId=71; // 59 - craft, 71 - sew, 58 - bandage, see DayZPlayerConstants.CMD_ACTIONFB_* constants
            ExtractBulletItemQuantityPerUse=20;
            ExtractBulletItemSuccessChancePercent=70;
            ExtractBulletItemAssistSuccessChancePercent=80;
            ExtractBulletItemProcedureBloodLossMl=100;

            IsInternalBleedingStoppingItem=1;
            InternalBleedingStoppingItemRequiredPainkillerLevel=3;
            InternalBleedingStoppingItemActionTimeSeconds=10;
            InternalBleedingStoppingItemActionText="#STR_inediainfectedai_internal_bleeding_treatment_tool_stop_internal_bleeding";
            InternalBleedingStoppingItemActionAnimationId=71; // 59 - craft, 71 - sew, 58 - bandage, see DayZPlayerConstants.CMD_ACTIONFB_* constants
            InternalBleedingStoppingItemQuantityPerUse=20;
            InternalBleedingStoppingItemSuccessChancePercent=30;
            InternalBleedingStoppingItemAssistSuccessChancePercent=40;
            InternalBleedingStoppingItemProcedureBloodLossMl=500;

            IsCutClosureItem=1;
            CutClosureItemActionTimeSeconds=3;
            CutClosureItemActionText="#STR_inediainfectedai_remove_scratch";
            CutClosureItemActionAnimationId=71;
            CutClosureItemQuantityPerUse=5;
        }

        scope=2;
        displayName="$STR_inediainfectedai_surgicalkit_display_name";
        descriptionShort="$STR_inediainfectedai_surgicalkit_description_short";
        model="\dz\gear\tools\sewing_kit.p3d";
        debug_ItemCategory=2;
        animClass="Knife";
        rotationFlags=17;
        stackedUnit="percentage";
        quantityBar=1;
        varQuantityInit=100;
        varQuantityMin=0;
        varQuantityMax=100;
        varCleannessInit=1;
        itemSize[]={1,2};
        weight=100;
        weightPerQuantityUnit=5;
        fragility=0.0099999998;
        repairKitType=0;

        hiddenSelections[]={"zbytek"};
        hiddenSelectionsTextures[]={"InediaInfectedAI\gear\medical\data\inediapain_surgicalkit_co.paa"};
        hiddenSelectionsMaterials[]={"DZ\gear\tools\data\sewing_kit.rvmat"};

        class DamageSystem
        {
            class GlobalHealth
            {
                class Health
                {
                    hitpoints=50;
                    healthLevels[]={{1,{"DZ\gear\tools\data\sewing_kit.rvmat"}},{0.69999999,{"DZ\gear\tools\data\sewing_kit.rvmat"}},{0.5,{"DZ\gear\tools\data\sewing_kit_damage.rvmat"}},{0.30000001,{"DZ\gear\tools\data\sewing_kit_damage.rvmat"}},{0,{"DZ\gear\tools\data\sewing_kit_destruct.rvmat"}}};
                };
            };
        };
        soundImpactType="metal";
        class AnimEvents { class SoundWeapon { class Stitch { soundSet="StitchUpSelf_SoundSet"; id=201; }; class pickup { soundSet="sewingkit_pickup_SoundSet"; id=797; }; class drop { soundset="sewingkit_drop_SoundSet"; id=898; }; }; };
    };

    class InediaPain_SplintKit : Inventory_Base
    {
        inventorySlot[]+=
        {
            "Belt_Left",
            "InediaPain_Splints1"
        };

        class InediaPain
        {
            ShowTips=1;

            IsFractureStabilizationItem=1;
            FractureStabilizationItemAllowedLimbs[]={"head", "arms", "legs", "torso"};
            FractureStabilizationItemShockReductionMultiplier = 3;
            FractureStabilizationItemRegenerationMultiplier = 3;
            FractureStabilizationItemActionTimeSeconds=10;
            FractureStabilizationItemActionText="#STR_inediainfectedai_stabilize_break";
            FractureStabilizationItemActionAnimationId=59; // 59 - craft, 71 - sew, 58 - bandage, see DayZPlayerConstants.CMD_ACTIONFB_* constants
            FractureStabilizationItemQuantityPerUse=20;
        }

        scope=2;
        displayName="$STR_inediainfectedai_splintkit_display_name";
        descriptionShort="$STR_inediainfectedai_splintkit_description_short";
        model="\dz\gear\containers\FirstAidKit.p3d";
        debug_ItemCategory=2;
        animClass="Knife";
        rotationFlags=17;
        stackedUnit="percentage";
        quantityBar=1;
        varQuantityInit=100;
        varQuantityMin=0;
        varQuantityMax=100;
        itemSize[]={3,3};
        weight=500;
        weightPerQuantityUnit=15;
        fragility=0.0099999998;
        repairKitType=0;

        hiddenSelections[]={"zbytek"};
        hiddenSelectionsTextures[]={"InediaInfectedAI\gear\medical\data\inediapain_splintkit_co.paa"};
        hiddenSelectionsMaterials[]={"DZ\gear\containers\data\FirsAidKit.rvmat"};

        class DamageSystem
        {
            class GlobalHealth
            {
                class Health
                {
                    hitpoints=80;
                    healthLevels[]={{1,{"DZ\gear\containers\data\FirsAidKit.rvmat"}},{0.69999999,{"DZ\gear\containers\data\FirsAidKit.rvmat"}},{0.5,{"DZ\gear\containers\data\FirsAidKit_damage.rvmat"}},{0.30000001,{"DZ\gear\containers\data\FirsAidKit_damage.rvmat"}},{0,{"DZ\gear\containers\data\FirsAidKit_destruct.rvmat"}}};
                };
            };
        };
        soundImpactType="textile";
        class AnimEvents { class SoundWeapon { class pickUpItem_Light { soundSet="pickUpCourierBag_Light_SoundSet"; id=796; }; class pickUpItem { soundSet="pickUpCourierBag_SoundSet"; id=797; }; }; };

        class InventorySlotsOffsets
        {
            class Belt_Left
            {
                position[]={0,-0.05,0}; // Position offset: X (left/right), Y (up/down), Z (forward/backward)
                orientation[]={0,180,180}; // Rotation: X (tilt forward/backward), Y (rotate left/right), Z (tilt sideways)
            };
        };
    };

    class IodineTincture: Edible_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_Disinfectants1"
        };
    };

    class DisinfectantSpray: Edible_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_Disinfectants1"
        };
    };

    class DisinfectantAlcohol: Edible_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_Disinfectants1"
        };
    };

    class BloodBagEmpty: Inventory_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_BloodSupplies1",
            "InediaPain_BloodSupplies2",
            "InediaPain_BloodSupplies3"
        };
    };
    class BloodBagFull: Inventory_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_BloodSupplies1",
            "InediaPain_BloodSupplies2",
            "InediaPain_BloodSupplies3"
        };
    };
    class BloodBagIV: Inventory_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_BloodSupplies1",
            "InediaPain_BloodSupplies2",
            "InediaPain_BloodSupplies3"
        };
    };
    class SalineBag: Inventory_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_BloodSupplies1",
            "InediaPain_BloodSupplies2",
            "InediaPain_BloodSupplies3"
        };
    };
    class SalineBagIV: Inventory_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_BloodSupplies1",
            "InediaPain_BloodSupplies2",
            "InediaPain_BloodSupplies3"
        };
    };
    class StartKitIV: Inventory_Base
    {
        inventorySlot[]+=
        {
            "InediaPain_BloodSupplies1",
            "InediaPain_BloodSupplies2",
            "InediaPain_BloodSupplies3"
        };
    };

    class ZmbM_usSoldier_normal_Base;
    class ZmbM_usSoldier_Officer_Desert: ZmbM_usSoldier_normal_Base
    {
        aiAgentTemplate="InfectedMSoldier";
    };

    // The BI increased the vanilla ranged damage to the body of infected by 42% in DayZ update 1.28.
    // Since the damage for "InediaInfectedAI" is based on vanilla damage, this also affected "InediaInfectedAI" damage.
    // Restoring this multiplier back to "1".
    class DayZInfected;
    class ZombieBase: DayZInfected
    {
        class DamageSystem
        {
            class DamageZones
            {
                class Torso
                {
                    class ArmorType
                    {
                        class Projectile
                        {
                            class Health
                            {
                                damage=1;
                            };
                        };
                    };
                };
            };
        };
    };
};

class CfgSlots
{
    class Slot_InediaPain_Bandages1
    {
        name="InediaPain_Bandages1";
        displayName = "#STR_CfgMedicalBandage0";
        ghostIcon="set:dayz_inventory image:medicalbandage";
    };

    class Slot_InediaPain_Bandages2
    {
        name="InediaPain_Bandages2";
        displayName = "#STR_CfgMedicalBandage0";
        ghostIcon="set:dayz_inventory image:medicalbandage";
    };

    class Slot_InediaPain_Bandages3
    {
        name="InediaPain_Bandages3";
        displayName = "#STR_CfgMedicalBandage0";
        ghostIcon="set:dayz_inventory image:medicalbandage";
    };

    class Slot_InediaPain_Bandages4
    {
        name="InediaPain_Bandages4";
        displayName = "#STR_CfgMedicalBandage0";
        ghostIcon="set:dayz_inventory image:medicalbandage";
    };

    class Slot_InediaPain_Injectors1
    {
        name="InediaPain_Injectors1";
        displayName = "$STR_inediainfectedai_injectors";
        ghostIcon="set:inediainfectedai_slots image:inediainfectedai_injectors";
    };

    class Slot_InediaPain_Injectors2
    {
        name="InediaPain_Injectors2";
        displayName = "$STR_inediainfectedai_injectors";
        ghostIcon="set:inediainfectedai_slots image:inediainfectedai_injectors";
    };

    class Slot_InediaPain_Injectors3
    {
        name="InediaPain_Injectors3";
        displayName = "$STR_inediainfectedai_injectors";
        ghostIcon="set:inediainfectedai_slots image:inediainfectedai_injectors";
    };

    class Slot_InediaPain_Injectors4
    {
        name="InediaPain_Injectors4";
        displayName = "$STR_inediainfectedai_injectors";
        ghostIcon="set:inediainfectedai_slots image:inediainfectedai_injectors";
    };

    class Slot_InediaPain_SurgicalKits1
    {
        name="InediaPain_SurgicalKits1";
        displayName="$STR_inediainfectedai_surgicalkit_display_name";
        ghostIcon="set:inediainfectedai_slots image:inediainfectedai_surgery";
    };

    class Slot_InediaPain_SurgicalKits2
    {
        name="InediaPain_SurgicalKits2";
        displayName="$STR_inediainfectedai_surgicalkit_display_name";
        ghostIcon="set:inediainfectedai_slots image:inediainfectedai_surgery";
    };

    class Slot_InediaPain_Splints1
    {
        name="InediaPain_Splints1";
        displayName="$STR_inediainfectedai_splintkit_display_name";
        ghostIcon="set:inediainfectedai_slots image:inediainfectedai_splints";
    };

    class Slot_InediaPain_Disinfectants1
    {
        name="InediaPain_Disinfectants1";
        displayName="$STR_inediainfectedai_disinfectants";
        ghostIcon="set:dayz_inventory image:disinfectant";
    };

    class Slot_InediaPain_Pills1
    {
        name="InediaPain_Pills1";
        displayName="$STR_inediainfectedai_pills";
        ghostIcon="set:inediainfectedai_slots image:inediainfectedai_pills";
    };

    class Slot_InediaPain_Pills2
    {
        name="InediaPain_Pills2";
        displayName="$STR_inediainfectedai_pills";
        ghostIcon="set:inediainfectedai_slots image:inediainfectedai_pills";
    };

    class Slot_InediaPain_Pills3
    {
        name="InediaPain_Pills3";
        displayName="$STR_inediainfectedai_pills";
        ghostIcon="set:inediainfectedai_slots image:inediainfectedai_pills";
    };

    class Slot_InediaPain_Pills4
    {
        name="InediaPain_Pills4";
        displayName="$STR_inediainfectedai_pills";
        ghostIcon="set:inediainfectedai_slots image:inediainfectedai_pills";
    };

    class Slot_InediaPain_Pills5
    {
        name="InediaPain_Pills5";
        displayName="$STR_inediainfectedai_pills";
        ghostIcon="set:inediainfectedai_slots image:inediainfectedai_pills";
    };

    class Slot_InediaPain_Pills6
    {
        name="InediaPain_Pills6";
        displayName="$STR_inediainfectedai_pills";
        ghostIcon="set:inediainfectedai_slots image:inediainfectedai_pills";
    };

    class Slot_InediaPain_Pills7
    {
        name="InediaPain_Pills7";
        displayName="$STR_inediainfectedai_pills";
        ghostIcon="set:inediainfectedai_slots image:inediainfectedai_pills";
    };

    class Slot_InediaPain_Pills8
    {
        name="InediaPain_Pills8";
        displayName="$STR_inediainfectedai_pills";
        ghostIcon="set:inediainfectedai_slots image:inediainfectedai_pills";
    };

    class Slot_InediaPain_BloodSupplies1
    {
        name="InediaPain_BloodSupplies1";
        displayName="$STR_inediainfectedai_blood_supplies";
        ghostIcon="set:inediainfectedai_slots image:inediainfectedai_blood";
    };

    class Slot_InediaPain_BloodSupplies2
    {
        name="InediaPain_BloodSupplies2";
        displayName="$STR_inediainfectedai_blood_supplies";
        ghostIcon="set:inediainfectedai_slots image:inediainfectedai_blood";
    };

    class Slot_InediaPain_BloodSupplies3
    {
        name="InediaPain_BloodSupplies3";
        displayName="$STR_inediainfectedai_blood_supplies";
        ghostIcon="set:inediainfectedai_slots image:inediainfectedai_blood";
    };
};

/*
class AIParams
{
    class AgentTeams
    {
        class Predator
        {
            friends[]=
            {
                "Predator",
                "AmbientLife"
            };
        };
        class Zombies
        {
            friends[]=
            {
                "Zombies",
                "AmbientLife"
            };
        };
    };
};
*/

class CfgAIBehaviours
{
    class Infected
    {
        class NoiseSystemParams
        {
            rangeShotMax=180;
        };
    };

    class InfectedMale: Infected {};
    class InfectedFemale: Infected {};

    class InfectedMSoldier: InfectedMale
    {
        class NoiseSystemParams: NoiseSystemParams
        {
            rangeShotMax=180;
        };
    };

    class InfectedMParamedic: InfectedMale
    {
        class NoiseSystemParams: NoiseSystemParams
        {
            rangeShotMax=180;
        };
    };

    class InfectedFParamedic: InfectedFemale
    {
        class NoiseSystemParams: NoiseSystemParams
        {
            rangeShotMax=180;
        };
    };

    class InfectedMHunter: InfectedMale
    {
        class NoiseSystemParams: NoiseSystemParams
        {
            rangeShotMax=180;
        };
    };

    class InfectedPriest: InfectedMale
    {
        class NoiseSystemParams: NoiseSystemParams
        {
            rangeShotMax=180;
        };
    };
};

class PathGraphFilters
{
    class ZombieAlerted
    {
        class Flags
        {
            include[]=
            {
                "walk",
                "door",
                "inside",
                "jump",
                "climb",
                "swim",
                "swimsea"
            };
            exclude[]=
            {
                "disabled",
                "crawl",
                "crouch"
            };
        };
    };
};

class CfgAmmo
{
    class MeleeDamage;
    class InediaInfectedAI_ThrowingProjectile: MeleeDamage
    {
        class DamageApplied
        {
            type="Melee";
            bleedThreshold=0;
            class Health
            {
                damage=10;
            };
            class Blood
            {
                damage=10;
            };
            class Shock
            {
                damage=10;
            };
        };
    };
}

class CfgSoundShaders {
    class baseCharacter_SoundShader;

    class inedia_infectedai_wooden_door_hit_1_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\wooden_door_hit_1.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };
    class inedia_infectedai_wooden_door_hit_2_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\wooden_door_hit_2.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };
    class inedia_infectedai_wooden_door_hit_3_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\wooden_door_hit_3.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };
    class inedia_infectedai_wooden_door_hit_4_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\wooden_door_hit_4.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };
    class inedia_infectedai_wooden_door_open_1_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\wooden_door_open_1.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };
    class inedia_infectedai_wooden_door_destroy_1_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\wooden_door_destroy_1.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };
    class inedia_infectedai_wooden_door_destroy_2_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\wooden_door_destroy_2.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };
    class inedia_infectedai_wooden_door_destroy_3_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\wooden_door_destroy_3.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };
    class inedia_infectedai_wooden_door_destroy_4_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\wooden_door_destroy_4.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };

    class inedia_infectedai_metal_door_hit_1_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\metal_door_hit_1.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };
    class inedia_infectedai_metal_door_hit_2_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\metal_door_hit_2.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };
    class inedia_infectedai_metal_door_hit_3_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\metal_door_hit_3.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };
    class inedia_infectedai_metal_door_hit_4_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\metal_door_hit_4.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };
    class inedia_infectedai_metal_door_hit_5_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\metal_door_hit_5.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };
    class inedia_infectedai_metal_door_hit_6_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\metal_door_hit_6.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };
    class inedia_infectedai_metal_door_open_1_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\metal_door_open_1.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };
    class inedia_infectedai_metal_door_open_2_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\metal_door_open_2.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };
    class inedia_infectedai_metal_door_destroy_1_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\metal_door_destroy_1.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };
    class inedia_infectedai_metal_door_destroy_2_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\metal_door_destroy_2.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };

    class inedia_infectedai_wallgate_door_hit_1_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\wallgate_door_hit_1.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };
    class inedia_infectedai_wallgate_door_hit_2_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\wallgate_door_hit_2.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };
    class inedia_infectedai_wallgate_door_open_1_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\wallgate_door_open_1.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };
    class inedia_infectedai_wallgate_door_destroy_1_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\wallgate_door_destroy_1.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };

    class inedia_infectedai_item_hit_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\item_hit.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };

    class inedia_infectedai_attempt_open_destroyed_door_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"DZ\sounds\environment\buildings\doors\WoodRattle\doorWoodChurchLargeRattle_1", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };

    class inedia_infectedai_tinnitus_SoundShader : baseCharacter_SoundShader {
        samples[] = {{"InediaInfectedAI\sounds\tinnitus.ogg", 1}};
        volume = 1;
        range = 50;
        rangeCurve[] = {{0, 1}, {0.5, 0.7}, {1, 0}};
    };

    class inedia_infectedai_bodyfall_hand_dirt_ext_Zmb_SoundShader: baseCharacter_SoundShader
    {
        samples[]=
        {
            {"DZ\sounds\Characters\movement\bodyfall\dirt_ext\bodyfall_hand_1", 1},
            {"DZ\sounds\Characters\movement\bodyfall\dirt_ext\bodyfall_hand_2", 1},
            {"DZ\sounds\Characters\movement\bodyfall\dirt_ext\bodyfall_hand_3", 1},
            {"DZ\sounds\Characters\movement\bodyfall\dirt_ext\bodyfall_hand_4", 1}
        };
        volume = 1.5;
    };
    class inedia_infectedai_bodyfall_dirt_ext_Zmb_SoundShader: baseCharacter_SoundShader
    {
        samples[]=
        {
            {"DZ\sounds\Characters\movement\bodyfall\dirt_ext\bodyfall_1", 1},
            {"DZ\sounds\Characters\movement\bodyfall\dirt_ext\bodyfall_2", 1},
            {"DZ\sounds\Characters\movement\bodyfall\dirt_ext\bodyfall_3", 1},
            {"DZ\sounds\Characters\movement\bodyfall\dirt_ext\bodyfall_4", 1}
        };
        volume = 1;
    };

    class inedia_infectedai_pain_male_1_SoundShader: baseCharacter_SoundShader
    {
        samples[]=
        {
            {"DZ\sounds\Characters\voice\male_1\pain_1", 1},
            {"DZ\sounds\Characters\voice\male_1\pain_2", 1},
            {"DZ\sounds\Characters\voice\male_1\pain_3", 1},
            {"DZ\sounds\Characters\voice\male_1\pain_4", 1},
            {"DZ\sounds\Characters\voice\male_1\pain_5", 1},
            {"DZ\sounds\Characters\voice\male_1\pain_6", 1},
            {"DZ\sounds\Characters\voice\male_1\pain_7", 1},
            {"DZ\sounds\Characters\voice\male_1\pain_8", 1}
        };
        volume = 1;
    };
    class inedia_infectedai_pain_male_2_SoundShader: baseCharacter_SoundShader
    {
        samples[]=
        {
            {"DZ\sounds\Characters\voice\male_2\pain_1", 1},
            {"DZ\sounds\Characters\voice\male_2\pain_2", 1},
            {"DZ\sounds\Characters\voice\male_2\pain_3", 1},
            {"DZ\sounds\Characters\voice\male_2\pain_4", 1},
            {"DZ\sounds\Characters\voice\male_2\pain_5", 1},
            {"DZ\sounds\Characters\voice\male_2\pain_6", 1},
            {"DZ\sounds\Characters\voice\male_2\pain_7", 1},
            {"DZ\sounds\Characters\voice\male_2\pain_8", 1}
        };
        volume = 1;
    };
    class inedia_infectedai_pain_female_1_SoundShader: baseCharacter_SoundShader
    {
        samples[]=
        {
            {"DZ\sounds\Characters\voice\female_1\pain_1", 1},
            {"DZ\sounds\Characters\voice\female_1\pain_2", 1},
            {"DZ\sounds\Characters\voice\female_1\pain_3", 1},
            {"DZ\sounds\Characters\voice\female_1\pain_4", 1},
            {"DZ\sounds\Characters\voice\female_1\pain_5", 1},
            {"DZ\sounds\Characters\voice\female_1\pain_6", 1},
            {"DZ\sounds\Characters\voice\female_1\pain_7", 1},
            {"DZ\sounds\Characters\voice\female_1\pain_8", 1}
        };
        volume = 1;
    };
    class inedia_infectedai_pain_female_2_SoundShader: baseCharacter_SoundShader
    {
        samples[]=
        {
            {"DZ\sounds\Characters\voice\female_2\pain_1", 1},
            {"DZ\sounds\Characters\voice\female_2\pain_2", 1},
            {"DZ\sounds\Characters\voice\female_2\pain_3", 1},
            {"DZ\sounds\Characters\voice\female_2\pain_4", 1},
            {"DZ\sounds\Characters\voice\female_2\pain_5", 1},
            {"DZ\sounds\Characters\voice\female_2\pain_6", 1},
            {"DZ\sounds\Characters\voice\female_2\pain_7", 1},
            {"DZ\sounds\Characters\voice\female_2\pain_8", 1}
        };
        volume = 1;
    };
};

class CfgSoundSets {
    class baseCharacter_SoundSet;

    class inedia_infectedai_wooden_door_hit_1_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_wooden_door_hit_1_SoundShader"};
    };
    class inedia_infectedai_wooden_door_hit_2_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_wooden_door_hit_2_SoundShader"};
    };
    class inedia_infectedai_wooden_door_hit_3_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_wooden_door_hit_3_SoundShader"};
    };
    class inedia_infectedai_wooden_door_hit_4_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_wooden_door_hit_4_SoundShader"};
    };
    class inedia_infectedai_wooden_door_open_1_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_wooden_door_open_1_SoundShader"};
    };
    class inedia_infectedai_wooden_door_destroy_1_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_wooden_door_destroy_1_SoundShader"};
    };
    class inedia_infectedai_wooden_door_destroy_2_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_wooden_door_destroy_2_SoundShader"};
    };
    class inedia_infectedai_wooden_door_destroy_3_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_wooden_door_destroy_3_SoundShader"};
    };
    class inedia_infectedai_wooden_door_destroy_4_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_wooden_door_destroy_4_SoundShader"};
    };

    class inedia_infectedai_metal_door_hit_1_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_metal_door_hit_1_SoundShader"};
    };
    class inedia_infectedai_metal_door_hit_2_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_metal_door_hit_2_SoundShader"};
    };
    class inedia_infectedai_metal_door_hit_3_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_metal_door_hit_3_SoundShader"};
    };
    class inedia_infectedai_metal_door_hit_4_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_metal_door_hit_4_SoundShader"};
    };
    class inedia_infectedai_metal_door_hit_5_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_metal_door_hit_5_SoundShader"};
    };
    class inedia_infectedai_metal_door_hit_6_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_metal_door_hit_6_SoundShader"};
    };
    class inedia_infectedai_metal_door_open_1_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_metal_door_open_1_SoundShader"};
    };
    class inedia_infectedai_metal_door_open_2_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_metal_door_open_2_SoundShader"};
    };
    class inedia_infectedai_metal_door_destroy_1_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_metal_door_destroy_1_SoundShader"};
    };
    class inedia_infectedai_metal_door_destroy_2_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_metal_door_destroy_2_SoundShader"};
    };

    class inedia_infectedai_wallgate_door_hit_1_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_wallgate_door_hit_1_SoundShader"};
    };
    class inedia_infectedai_wallgate_door_hit_2_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_wallgate_door_hit_2_SoundShader"};
    };
    class inedia_infectedai_wallgate_door_open_1_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_wallgate_door_open_1_SoundShader"};
    };
    class inedia_infectedai_wallgate_door_destroy_1_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_wallgate_door_destroy_1_SoundShader"};
    };

    class inedia_infectedai_item_hit_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_item_hit_SoundShader"};
    };

    class inedia_infectedai_attempt_open_destroyed_door_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_attempt_open_destroyed_door_SoundShader"};
    };

    class inedia_infectedai_tinnitus_SoundSet : baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_tinnitus_SoundShader"};
    };

    class inedia_infectedai_Bodyfall_hand_dirt_ext_Zmb_SoundSet: baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_bodyfall_hand_dirt_ext_Zmb_SoundShader"};
    };
    class inedia_infectedai_Bodyfall_dirt_ext_Zmb_SoundSet: baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_bodyfall_dirt_ext_Zmb_SoundShader"};
    };

    class inedia_infectedai_pain_male_1_SoundSet: baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_pain_male_1_SoundShader"};
    };
    class inedia_infectedai_pain_male_2_SoundSet: baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_pain_male_2_SoundShader"};
    };
    class inedia_infectedai_pain_female_1_SoundSet: baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_pain_female_1_SoundShader"};
    };
    class inedia_infectedai_pain_female_2_SoundSet: baseCharacter_SoundSet {
        soundShaders[] = {"inedia_infectedai_pain_female_2_SoundShader"};
    };

    class inedia_infectedai_default_sound_noise {
        strength = 10;
        type = "sound";
    };
};
