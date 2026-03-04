/**
 * Created: 2024-12-03
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

class InediaInfectedAI_ActionDeepWoundBandageCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
		float actionTimeSeconds = 3;

		if (m_ActionData.m_MainItem) {
			actionTimeSeconds = GetGame().ConfigGetFloat("CfgVehicles " + m_ActionData.m_MainItem.GetType() + " InediaPain DeepWoundBandageItemActionTimeSeconds");
			
			if (m_ActionData.m_Player) {
				actionTimeSeconds = InediaInfectedAI_Restrictions.PainSystem_ModifyDeepWoundBandageActionTime(m_ActionData.m_Player, m_ActionData.m_MainItem, actionTimeSeconds);
			}
			
			if (actionTimeSeconds < 0) {
				actionTimeSeconds = 0;
			}
		}

		m_ActionData.m_ActionComponent = new CAContinuousTime(actionTimeSeconds);
    }
}

class InediaInfectedAI_ActionDeepWoundBandage : ActionContinuousBase
{
	string m_InediaInfectedAI_PainLimb = "head";
	string m_InediaInfectedAI_Text = "";

    void InediaInfectedAI_ActionDeepWoundBandage()
    {
		m_CallbackClass 	= InediaInfectedAI_ActionDeepWoundBandageCB;
		m_FullBody 			= true;
		m_StanceMask 		= DayZPlayerConstants.STANCEMASK_CROUCH;

		if (!HasTarget()) {
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BANDAGE;
		} else {
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BANDAGETARGET;
		}
    }

    override void CreateConditionComponents()
    {
		m_ConditionItem 	= new CCINonRuined();

		if (!HasTarget()) {
			m_ConditionTarget = new CCTSelf();
		} else {
			m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
		}
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
		PlayerBase targetPlayer;
		if (!HasTarget()) {
			targetPlayer = player;
		} else {
			if (target) {
				targetPlayer = PlayerBase.Cast(target.GetObject());
			}
		}

		if (!player || !player.InediaInfectedAI_GetPainManager().LimbsDeepWoundSystemIsActive()) {
			return false;
		}

		if (!targetPlayer || !targetPlayer.IsAlive()) {
			return false;
		}

		array<string> allowedLimbs = new array<string>;
		GetGame().ConfigGetTextArray("CfgVehicles " + item.GetType() + " InediaPain DeepWoundBandageItemAllowedLimbs", allowedLimbs);
		if (allowedLimbs.Find(m_InediaInfectedAI_PainLimb) < 0) {
			return false;
		}
		
		if (!HasTarget()) {
			int animationId = GetGame().ConfigGetInt("CfgVehicles " + item.GetType() + " InediaPain DeepWoundBandageItemActionAnimationId");
			if (animationId > 0) {
				m_CommandUID = animationId;
			} else {
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BANDAGE;
			}
		}
		
		m_InediaInfectedAI_Text = "[#STR_inediainfectedai_" + m_InediaInfectedAI_PainLimb + "]";
		if (HasTarget()) {
			m_InediaInfectedAI_Text = m_InediaInfectedAI_Text + " [#STR_inediainfectedai_other_player]";
		}
		string actionText;
		GetGame().ConfigGetText("CfgVehicles " + item.GetType() + " InediaPain DeepWoundBandageItemActionText", actionText);
		if (actionText != "") {
			m_InediaInfectedAI_Text = m_InediaInfectedAI_Text + " " + actionText;
		} else {
			m_InediaInfectedAI_Text = m_InediaInfectedAI_Text + " #STR_inediainfectedai_bandage_wound";
		}

		string tips = "";
		if (targetPlayer.InediaInfectedAI_GetPainManager().m_ShowMedicationInfo && GetGame().ConfigGetInt("CfgVehicles " + item.GetType() + " InediaPain ShowTips") == 1) {
			float effectiveDurationSeconds = GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain DeepWoundBandageItemEffectiveDurationSeconds");
			if (effectiveDurationSeconds > 0) {
				tips = tips + " [#STR_inediainfectedai_effective_duration: " + effectiveDurationSeconds + "s]";
			}

			float regenerationMultiplier = GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain DeepWoundBandageItemRegenerationMultiplier");
			if (regenerationMultiplier < 1) regenerationMultiplier = 1;
			if (regenerationMultiplier > 1) {
				tips = tips + " [#STR_inediainfectedai_regeneration_wound: x" + regenerationMultiplier + "]";
			}
		}

		m_Text = m_InediaInfectedAI_Text + tips;

#ifdef SERVER
		if (player && targetPlayer && targetPlayer.m_InediaInfectedAI_ConfigPlayers) {
			bool successValidation = true;

			// Bandage validator
			if (targetPlayer.InediaInfectedAI_GetPainManager().IsLimbDeepWoundBandaged(m_InediaInfectedAI_PainLimb)) {
				string targetStrPrefix = "";
				if (HasTarget()) {
					targetStrPrefix = "_target";
				}
				
				player.InediaInfectedAI_SendNotSpamingImportantMessage("#STR_inediainfectedai_bandage_must_be_removed" + targetStrPrefix, 5, player.m_InediaInfectedAI_ConfigPlayers.PainSystemNotificationsType, 5);
				successValidation = false;
			}

			if (!successValidation) {
				return false;
			}
		}
#endif

		return targetPlayer.InediaInfectedAI_GetPainManager().IsLimbDeepWounded(m_InediaInfectedAI_PainLimb) && (!targetPlayer.InediaInfectedAI_GetPainManager().IsLimbDeepWoundBandaged(m_InediaInfectedAI_PainLimb) || !targetPlayer.InediaInfectedAI_GetPainManager().IsLimbDeepWoundBandageEffective(m_InediaInfectedAI_PainLimb));
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
		if (action_data) {
			PlayerBase player = action_data.m_Player;

			PlayerBase targetPlayer;
			if (!HasTarget()) {
				targetPlayer = player;
			} else {
				if (action_data.m_Target) {
					targetPlayer = PlayerBase.Cast(action_data.m_Target.GetObject());
				}
			}

    //                                                            .::.....::...     ......::::.::::.
//                                                      ::::::.                                .:::::.
//                                                 :::::.         #- :=++*++=-:  -+                  ......
//                                             .:::              .#%%*=:...:-+#%*##                        .::.
//                                         .:::.                =-+*  :=-::---:=*==+                          .:::
//                                      .::.                   +- -=:+:.:::.:.:==*.:*                             .::.
//                                   .::.                     -+   ==..:     .:==-  :=......                         :::
//                                 :::                        #    .:-.      -.-=    +.     .......                     ::.
//                              .::.                    ......%-----==-:::::====----=+=            .....                  .::
//                            .::                  .....    .+%%#%@**#*+:.:-*+==-=%#+#.                 ...                 .::
//                          .::                 ...       ..#=***##:=+*#%+:*-*%#+*%=@%..                   ...                .:.
//                        .::               ...  .:-==+===--#%**=*# .:..---=.::::=@-#@-. ........             ...               .:.
//                       ::              ... .-=++==+*#%@@@@@@*==*%.    .. =     +@:##+::::::::  ....            ...              ::
//                     .:.             ... :==:...::.:=*%@@@@@*-##%:    :=:-:   -%%-#+*  ...  .----.  ...          ...              :.
//                    ::             ..   +:    .---------=+*%:=-%%:.  .---=:   *@*-#%+=     ....  :---.  ...         ..             .:
//                  .-.            ..    -+   -+- .++=-:--=*%-:+=@@%-  ::---.  =@+:=%@*#         ...   -==:  ..         ..             :.
//                 ::            ..     .+==-=:  :+=*#%%@@#+:=*+*@#=##-     .=%@+ ==#@*#             ...  -==. ...       ..             ..
//                :.           ..      .-=   ===+%+@@@@@%****+%.%*=:::-+**#%@@@@. %:#@=+                ... .-=- ...       ..            .:            .=
//               -.           ..     ..:::-=.  .-+*=-::-+**%@@*  :==--::::--#@@% :@-@=*.                   ... :==. ..       ..           .:        .=+=+
//              -           ..     ..:.     :-=#=.  .::--=+=*%---.   :=======+%@+=@%*+.                       .. .==. .       ..            :    .==-.-*:
//             -.          ..     .:.        =+          --==+=. :---.       -@#%+*=.                           .. .==...       ..           ::==-..=+:
//            -           .      ..         +-            ==.-#=-..  :---:  .*@%%#++++=                            . .== ..      ..        .==: .:++:
//           :.          .     ..          ++::::.       .-+:=+-++:--.   .----#*=+=. .-#.                           .. .+:..       .    .==- ..-++.
//          ::          .     ..          +=:::.  .......--.-%- *:-. :-------- .#:==. .:#.                            .  -=..       ..==- ..:=+=:
//         .-          =.   ..         :-*#+=-----::......-*+.: *- .:.         :#+-=-  :==                             .. .+:.    .==- ..:-**:   :
//         -         .*:   ..          :--=====++++*=++=#@%.  : * -=..-.       +.#-=-:  :%                               .  -=..-=-   :-+*=.     ..
//        .:        .#-   ..            =%#+==-----=++++=%-  .-=*=-.:-:.:.   .=..*++-=-::*+                               . .++-...--=+=-         :
//        :         ++   ..            =**++=-----=++-.---   =#=   -=..=:.. :: . .*%%*+=-=**-                             -=-...----==.-.         .:
//       ..        :#   ..                 =+=++++-   =+:- .+*.      ==::=..  .  :**#%%%+#-.                           -==...:--:==:   .*          :
//       :         #:   .                 .%- -      .%#%#*=.          ==.::.  -=: :#%*+*-+                         :==:..:--:-=-  .    =+         ..
//      ..        =*   .                  %+=*=:     *+=:  -            :+...-=.    .+%+=*%.                     :==: ..--:-==.     .    %-         .
//      .        .%:   .                .=@*=-=*=.  :@+-   =              : .        +*%: =-                  .==- ..--::==.  .     ..   =#         .
//               =#   .                 +-:=-.-+=*+=#-%-   :-          .:           .+##:            .:    .==-...-:::==:     ..     .   .%:        .
//               #-   .                -*+: .=- +-+%- +=    .==-:::::.  .=         :+*%:=          .==* .==-....:::==:         .     .    *=
//              .%    .                #. :=: .-.=@%  .%#=.     ...      .- .-----+#%*. #-        .*-=#=-......:==-            .      .   =#
//              -#   .               -++-:  :=: :+@-   -*-+*+=:.   ....   :::..-+*%%.  .#=      :=#+. +:::. .==-                .     .   :%
//              =+   .              -#   .-:  -++%-     =*-  .:=*==-:  .  ::=+=.+-%@*-**- .#:.+%##@% ..+..==-                   .     .   .@.
//              +=                .+#----=-. .:%#:       +=--   *:  :==- .-*-  :-*%@%=:    -***-+-+%+ ..*+.                     .     .    %.
//              +=               -+.  .:  .=#@#:          #=.:  +-.   ..-*+    +=#+%%#+--  :*#=----=%=-+=                       .     .    %:
//              +=              =+    :.:  -%@:           =#+=. :=+   .  +.    *==%+:=+*=*+#%*=+=*+=-=*-                              .    @:
//              ++             :+    --: :*@@+            +*-+#. *::    .=   .-#+=:. .+#*--%%**+-.    .                                   .@.
//              =*             *   ::. ::=#@#            +*+-..:-=**-:..*:-===-:.  :+#%+:*%%%#-                                           -%
//              -#            =- ::  := ++@@:          :#= .*-....   .::-++--.  :=**-=+*%#=-*#*=                                          +#
//              .%.           #.: .. -..*:@*         .+#*========-.  .==-. .-=*+:-:.=%* :%: .%=+-                                         #+
//               *+          -#:.-.  =:*.+%.         =*:.....:-==.-==- .-++=-#*:#==+=*#: *#  :-:#.                                       :@:
//               -%.         *---   --=::%:         :%***+==---===:  -*+-..:-==-.. --+%*==@+  .--#.                                      +*
//                #=        :*. .  :+-::#.         =*+-: .:===-. .-+=: .:..     -:=:*-.#*-+%+.  =+*                                     .%:
//                :%.       *. .   =--=*.          *+----:::::-===. ..:.      :=====   *:%=.=#-: =**.                                   *+
//                 +*      :*  .  ::-==           :@-------=++:  .-:..      .==***.  -+.-#@#..*-=:-**                                  -%.
//                 .#-     #. :  ::-+-            +-   :++=.  .::.         :+-%%- .=*: .+ %@%..#+**+**.                               .%-
//                  :%-   == :  .:=+:            :*:=++:  .:::           .-+:#*+:.:== :=  =@@= :#*+=+#%-                             .#=
//                   :%: :* .. .:=*:           :+#+-.  ::-:             .=-.#. *-= -+:-   .%@#   +*=-.+%+                            *+
//                    :%:#=-:  -:%:         -++=%. ::--..              .=:.#-=+- *=.%.=    *@@:   #    =%#                          **
//                     +#=+==*+=#:      .-==: .#=.-::.        .::.    .=. #*=. -+:#.+-=.   -@@*  :#    ::%-                       .*+
//                    -#++-=++#%=    .===.  -+%+           :==:      .=  *+.:+=.  ==.* +    #@%.-+  :  .-=+                      .#=
//                   -*  .-==+*#. .==-. .-+*==#         -++:        .-  *#++:      #.*. -   -@@-#.-=.:  = *                     -%-
//                 -=%         *==*. .-++=: .#.     :=+=:           : .*+:       :==#:*  .   %@: =+=-:+.*.--                  .+*.
//                 +=#        :#.:--*+-.    *.   -++-                :%+.::    .+-  .*-+     #@: :=*-=#%* :*                 -#=
//                 .%-+:.    -*-=**%:      =-    -+==-.             +@@+++-   ==      *:+    #@:  .*-=*#::*.               .**.
//                  += ++  -%:-#@%*-     . #         :-**++==-----=#@@@% -- .+.        *-+   %@    .- -:=+               .+*:
//                  ++=%. .%++*#-        * *.       .::      .::*:.#@%%% :- *           +:* .@%       .+:              .+*:
//                 .# -#-=+--#:+-:-:    .* :#                -:*. +@#:*% :=.+           .==--@#       .              :++:
//                 == +=     :*+:  :=-..**  +*             .-:+ -%@+. -@..+ +            +.**@+                    -+=.
//                .#  %:     :#+     .=#=%.  *+           .--=-+*%:   .@- + *            + *@@-                 .-=.
//                += :@.     .+       .% +*   **         :---=::%:     #* + +      .    .= *@%               ..:.
//               :%  +%.               %-.%.   -#-      ----- :%=      =@.=.:-     ..   =. %@=           .....
//               *:  *%.              :+* -+     =#-   --=:  -%+       :@=:= +     ..   + -@%.       .....
//              -=  :=@.              *.:+-+:     .=*+=-=::+*##         ## * =:    .   =: *@+  ......
//            .=+   *:@.             :+  *.+*        :##*+-.:#:......   =@.:= *    .  .* .@%..
//            =*   .=-@.             +.  * =-*       -:+   +%:       ...:@- + :=   .  =. +@+
//            :%   = =@.            .+   *  *-=      +=: .#%.            #+ .= +      *  %@:
//             %- :..#%.            +:  :=  :++.    +-= :%%:             +#  +.:-     * -@*
//             +#.-+%+:            .+   +.   *-=   --* :%%:              -@: .+ +    .+ *@:
//             .@%*-.              +.  .+    =-*  .++.:%%-               .%+. # +     +:%#
//              .                 .=   +.    =-+  +=-:%@+                 *%--+:+     #-%=

			ItemBase item = ItemBase.Cast(action_data.m_MainItem);
			if (targetPlayer && targetPlayer.m_InediaInfectedAI_ConfigPlayers && item) {
				bool hasSepsis = false;

				float effectiveDuration = GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain DeepWoundBandageItemEffectiveDurationSeconds");
				if (effectiveDuration < 0) effectiveDuration = 0;
				float regenerationMultiplier = GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain DeepWoundBandageItemRegenerationMultiplier");
				if (regenerationMultiplier < 1) regenerationMultiplier = 1;
				targetPlayer.InediaInfectedAI_GetPainManager().ActivateDeepWoundBandageForLimb(m_InediaInfectedAI_PainLimb, effectiveDuration, regenerationMultiplier);

				PluginTransmissionAgents transmissionAgents = PluginTransmissionAgents.Cast(GetPlugin(PluginTransmissionAgents));
        		transmissionAgents.TransmitAgents(item, targetPlayer, AGT_ITEM_TO_FLESH);

				if (!item.GetCleanness()) {
					targetPlayer.InsertAgent(eAgents.WOUND_AGENT);
					hasSepsis = true;
				}

				int quantityPerUse = GetGame().ConfigGetInt("CfgVehicles " + item.GetType() + " InediaPain DeepWoundBandageItemQuantityPerUse");
				if (quantityPerUse > 0) {
			        if (item.HasQuantity()) {
						item.AddQuantity(-quantityPerUse, true);
					} else {
						item.Delete();
					}
				}
				
				InediaInfectedAI_Restrictions.PainSystem_DeepWoundBandageEndEvent(player, item, hasSepsis);
			}
		}
    }
}


class InediaInfectedAI_ActionDeepWoundBandageSelf : InediaInfectedAI_ActionDeepWoundBandage
{
    override bool HasTarget()
    {
        return false;
    }
}

class InediaInfectedAI_ActionDeepWoundBandageTarget : InediaInfectedAI_ActionDeepWoundBandage
{
    override bool HasTarget()
    {
        return true;
    }
}


class InediaInfectedAI_ActionDeepWoundBandageHeadSelf: InediaInfectedAI_ActionDeepWoundBandageSelf
{
    void InediaInfectedAI_ActionDeepWoundBandageHeadSelf()
    {
		m_InediaInfectedAI_PainLimb = "head";
    }
}
class InediaInfectedAI_ActionDeepWoundBandageHeadTarget: InediaInfectedAI_ActionDeepWoundBandageTarget
{
    void InediaInfectedAI_ActionDeepWoundBandageHeadTarget()
    {
		m_InediaInfectedAI_PainLimb = "head";
    }
}

class InediaInfectedAI_ActionDeepWoundBandageArmsSelf: InediaInfectedAI_ActionDeepWoundBandageSelf
{
    void InediaInfectedAI_ActionDeepWoundBandageArmsSelf()
    {
		m_InediaInfectedAI_PainLimb = "arms";
    }
}
class InediaInfectedAI_ActionDeepWoundBandageArmsTarget: InediaInfectedAI_ActionDeepWoundBandageTarget
{
    void InediaInfectedAI_ActionDeepWoundBandageArmsTarget()
    {
		m_InediaInfectedAI_PainLimb = "arms";
    }
}

// KUo3WfVSKag7xn0gT8lZtmtIT1gwSEJKTlZYWVppN2EwblA4UEtyWTRDUnJuZ2d0ZHFpSHYrTjJPUVQ4bDZ2ZGk4Tml3TlVUc2NZZnZmejFTVW9zWUxNNTNjaUM1Q2pnbCtxMVZ5WmZCNnhrcjdPM01DZ0xTTXBZSDJ6bXUxTjlWODM5bC9YN002dlZPd0xoQ1pWNHFMOVNZb0FmSFk4RzNWZFNiOTVtbE81TTN5YnhQVkM0cGZ6WUlzLzhlZUU4UUlkRXJJWkd2Z2xWdzNiTzBpVzc0emFCQmdxb0c0ZUtrcFQyaGlQYVpxQ2N5bWdrbWpMYkU5UDJGTnA1emprQUg4Y0hjaS9oL3ZJaUtxeVdHVkVFT240SkZFSTd4WnJjZGRvclR3PT0=

class InediaInfectedAI_ActionDeepWoundBandageLegsSelf: InediaInfectedAI_ActionDeepWoundBandageSelf
{
    void InediaInfectedAI_ActionDeepWoundBandageLegsSelf()
    {
		m_InediaInfectedAI_PainLimb = "legs";
    }
}
class InediaInfectedAI_ActionDeepWoundBandageLegsTarget: InediaInfectedAI_ActionDeepWoundBandageTarget
{
    void InediaInfectedAI_ActionDeepWoundBandageLegsTarget()
    {
		m_InediaInfectedAI_PainLimb = "legs";
    }
}

class InediaInfectedAI_ActionDeepWoundBandageTorsoSelf: InediaInfectedAI_ActionDeepWoundBandageSelf
{
    void InediaInfectedAI_ActionDeepWoundBandageTorsoSelf()
    {
		m_InediaInfectedAI_PainLimb = "torso";
    }
}
class InediaInfectedAI_ActionDeepWoundBandageTorsoTarget: InediaInfectedAI_ActionDeepWoundBandageTarget
{
    void InediaInfectedAI_ActionDeepWoundBandageTorsoTarget()
    {
		m_InediaInfectedAI_PainLimb = "torso";
    }
}
