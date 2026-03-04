/**
 * Created: 2024-11-29
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

modded class ModItemRegisterCallbacks
{
    override void RegisterOneHanded(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior)
    {
    	super.RegisterOneHanded(pType, pBehavior);

		pType.AddItemInHandsProfileIK("InediaPain_FirstAidKit_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/first_aid_kit.anm");
    	pType.AddItemInHandsProfileIK("InediaPain_FirstAidCase_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/protector_case.anm");
		pType.AddItemInHandsProfileIK("InediaPain_Tablet1Base", "dz/anims/workspaces/player/player_main/props/player_main_1h_tablets.asi", pBehavior, "dz/anims/anm/player/ik/gear/painkillers2.anm");
		pType.AddItemInHandsProfileIK("InediaPain_Tablet2Base", "dz/anims/workspaces/player/player_main/props/player_main_1h_tablets.asi", pBehavior, "dz/anims/anm/player/ik/gear/charcoal_tablets.anm");
		pType.AddItemInHandsProfileIK("InediaPain_HemostaticDressing", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/StartKit.anm");
		pType.AddItemInHandsProfileIK("InediaPain_SurgicalKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/sewing_kit.anm");
    	pType.AddItemInHandsProfileIK("InediaPain_SplintKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/first_aid_kit.anm");
	}
}