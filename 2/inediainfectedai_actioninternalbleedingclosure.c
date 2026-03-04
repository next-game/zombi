/**
 * Created: 2024-12-03
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

class InediaInfectedAI_ActionInternalBleedingStopCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        float actionTimeSeconds = 10;

        if (m_ActionData.m_MainItem) {
            actionTimeSeconds = GetGame().ConfigGetFloat("CfgVehicles " + m_ActionData.m_MainItem.GetType() + " InediaPain InternalBleedingStoppingItemActionTimeSeconds");
			
			if (m_ActionData.m_Player) {
				actionTimeSeconds = InediaInfectedAI_Restrictions.PainSystem_ModifyInternalBleedingClosureSurgeryActionTime(m_ActionData.m_Player, m_ActionData.m_MainItem, actionTimeSeconds);
			}
			
            if (actionTimeSeconds < 0) {
                actionTimeSeconds = 0;
            }
        }

        m_ActionData.m_ActionComponent = new CAContinuousTime(actionTimeSeconds);
    }
}

class InediaInfectedAI_ActionInternalBleedingStop : ActionContinuousBase
{
    string m_InediaInfectedAI_Text = "";
	
	float m_InediaInfectedAI_ProcedureBloodLossMultiplier = 1;

    void InediaInfectedAI_ActionInternalBleedingStop()
    {
        m_CallbackClass     = InediaInfectedAI_ActionInternalBleedingStopCB;
        m_FullBody          = true;
        m_StanceMask        = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;

        if (!HasTarget()) {
            m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_STITCHUPSELF;
        } else {
            m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
        }
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem     = new CCINonRuined();

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

        if (!player || !player.InediaInfectedAI_GetPainManager().InternalBleedingSystemIsActive()) {
            return false;
        }

        if (!targetPlayer || !targetPlayer.IsAlive()) {
            return false;
        }
		
		if (!HasTarget()) {
			int animationId = GetGame().ConfigGetInt("CfgVehicles " + item.GetType() + " InediaPain InternalBleedingStoppingItemActionAnimationId");
			if (animationId > 0) {
				m_CommandUID = animationId;
			} else {
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_STITCHUPSELF;
			}
		}

		m_InediaInfectedAI_Text = "[#STR_inediainfectedai_torso]";
		if (HasTarget()) {
			m_InediaInfectedAI_Text = m_InediaInfectedAI_Text + " [#STR_inediainfectedai_other_player]";
		}
		string actionText;
		GetGame().ConfigGetText("CfgVehicles " + item.GetType() + " InediaPain InternalBleedingStoppingItemActionText", actionText);
		if (actionText != "") {
			m_InediaInfectedAI_Text = m_InediaInfectedAI_Text + " " + actionText;
		} else {
			m_InediaInfectedAI_Text = m_InediaInfectedAI_Text + " #STR_inediainfectedai_internal_bleeding_treatment_tool_stop_internal_bleeding";
		}
		
        string tips = "";
        if (targetPlayer.InediaInfectedAI_GetPainManager().m_ShowMedicationInfo && GetGame().ConfigGetInt("CfgVehicles " + item.GetType() + " InediaPain ShowTips") == 1) {
            float successChancePercent;
			if (!HasTarget()) {
				successChancePercent = Math.Clamp(GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain InternalBleedingStoppingItemSuccessChancePercent"), 0, 100);
			} else {
				successChancePercent = Math.Clamp(GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain InternalBleedingStoppingItemAssistSuccessChancePercent"), 0, 100);
			}
			
			successChancePercent = Math.Clamp(InediaInfectedAI_Restrictions.PainSystem_ModifyInternalBleedingClosureSurgerySuccessChance(player, item, successChancePercent), 0, 100);
			
            float procedureBloodLoss = GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain InternalBleedingStoppingItemProcedureBloodLossMl");
            if (procedureBloodLoss < 0) procedureBloodLoss = 0;
            tips = " [#STR_inediainfectedai_success_chance: " + successChancePercent + "%] [#STR_inediainfectedai_blood_loss: " + procedureBloodLoss + "ml]";
        }

        m_Text = m_InediaInfectedAI_Text + tips;

#ifdef SERVER
		m_InediaInfectedAI_ProcedureBloodLossMultiplier = 1;
		
        if (player && targetPlayer && targetPlayer.m_InediaInfectedAI_ConfigPlayers) {
            bool successValidation = true;

            // Bandage validator
            if (targetPlayer.InediaInfectedAI_GetPainManager().IsLimbDeepWoundBandaged("torso")) {
				string targetStrPrefix = "";
				if (HasTarget()) {
					targetStrPrefix = "_target";
				}
				
                player.InediaInfectedAI_SendNotSpamingImportantMessage("#STR_inediainfectedai_bandage_must_be_removed" + targetStrPrefix, 5, player.m_InediaInfectedAI_ConfigPlayers.PainSystemNotificationsType, 5);
                successValidation = false;
            }
			
			// Painkiller validator
			int requiredPainkillerLevel = GetGame().ConfigGetInt("CfgVehicles " + item.GetType() + " InediaPain InternalBleedingStoppingItemRequiredPainkillerLevel");
			
			if (requiredPainkillerLevel >= 3) {
			    if (!targetPlayer.InediaInfectedAI_GetPainManager().PainkillerStrongEffectIsActive()) {
					if (HasTarget()) {
						// If another character performs the surgery, anesthesia is not required, however, the patient will lose three times more blood.
						player.InediaInfectedAI_SendNotSpamingImportantMessage("#STR_inediainfectedai_procedure_lack_of_anesthesia", 15, player.m_InediaInfectedAI_ConfigPlayers.PainSystemNotificationsType, 5);
						m_InediaInfectedAI_ProcedureBloodLossMultiplier = 3;
					} else {
				        player.InediaInfectedAI_SendNotSpamingImportantMessage("#STR_inediainfectedai_procedure_fail_need_painkiller_heavy", 5, player.m_InediaInfectedAI_ConfigPlayers.PainSystemNotificationsType, 5);
				        successValidation = false;
					}
			    }
			} else if (requiredPainkillerLevel >= 2) {
			    if (!targetPlayer.InediaInfectedAI_GetPainManager().PainkillerMediumEffectIsActive() && !targetPlayer.InediaInfectedAI_GetPainManager().PainkillerStrongEffectIsActive()) {
			        if (HasTarget()) {
						// If another character performs the surgery, anesthesia is not required, however, the patient will lose three times more blood.
						player.InediaInfectedAI_SendNotSpamingImportantMessage("#STR_inediainfectedai_procedure_lack_of_anesthesia", 15, player.m_InediaInfectedAI_ConfigPlayers.PainSystemNotificationsType, 5);
						m_InediaInfectedAI_ProcedureBloodLossMultiplier = 3;
					} else {
						player.InediaInfectedAI_SendNotSpamingImportantMessage("#STR_inediainfectedai_procedure_fail_need_painkiller_medium", 5, player.m_InediaInfectedAI_ConfigPlayers.PainSystemNotificationsType, 5);
				        successValidation = false;
					}
			    }
			} else if (requiredPainkillerLevel >= 1) {
			    if (!targetPlayer.InediaInfectedAI_GetPainManager().PainkillerLightEffectIsActive() && !targetPlayer.InediaInfectedAI_GetPainManager().PainkillerMediumEffectIsActive() && !targetPlayer.InediaInfectedAI_GetPainManager().PainkillerStrongEffectIsActive()) {
			        if (HasTarget()) {
						// If another character performs the surgery, anesthesia is not required, however, the patient will lose three times more blood.
						player.InediaInfectedAI_SendNotSpamingImportantMessage("#STR_inediainfectedai_procedure_lack_of_anesthesia", 15, player.m_InediaInfectedAI_ConfigPlayers.PainSystemNotificationsType, 5);
						m_InediaInfectedAI_ProcedureBloodLossMultiplier = 3;
					} else {
						player.InediaInfectedAI_SendNotSpamingImportantMessage("#STR_inediainfectedai_procedure_fail_need_painkiller_any", 5, player.m_InediaInfectedAI_ConfigPlayers.PainSystemNotificationsType, 5);
				        successValidation = false;
					}
			    }
			}

            if (successValidation) {
				// If the operation is being performed on an unconscious character, bring them to consciousness.
				if (targetPlayer.IsUnconscious()) {
	                if (targetPlayer.GetHealth("", "Shock") < PlayerConstants.CONSCIOUS_THRESHOLD) {
						targetPlayer.SetHealth("", "Shock", PlayerConstants.CONSCIOUS_THRESHOLD);
					}
	            }
				
				// Pain sound
				if (targetPlayer.InediaInfectedAI_GetPainManager().GetPainWithBuffsLevelForLimb("torso") > InediaInfectedAI_LimbPainLevels.ZERO) {
				    targetPlayer.InediaInfectedAI_PlayPainSoundServer(5);
				}
            } else {
				return false;
			}
        }
#endif

        return targetPlayer.InediaInfectedAI_GetPainManager().IsInternalBleedingActive();
    }

    override void OnStartAnimationLoopServer(ActionData action_data)
    {
        if (action_data) {
            if (!HasTarget()) {
                if (action_data.m_Player && action_data.m_Player.IsAlive()) {
                    action_data.m_Player.InediaInfectedAI_GetPainManager().ActivateDeepWoundForLimb("torso", 0.1);
                }
            } else {
                if (action_data.m_Target) {
                    PlayerBase targetPlayer = PlayerBase.Cast(action_data.m_Target.GetObject());
                    if (targetPlayer && targetPlayer.IsAlive()) {
                        targetPlayer.InediaInfectedAI_GetPainManager().ActivateDeepWoundForLimb("torso", 0.1);
                    }
                }
            }
        }

        super.OnStartAnimationLoopServer(action_data);
    }

// 42oFn2QQNioWWsIZ9KwR+29kamwwRll0cUNGOVBodjh0TlQ3aVdpSmFJWVA1RlJXVis0N2hSa3VQeFJmcDFJMGc3OEVHeHh0WXQvSFNOMlhOSXlWT1IvZXBFb3B5SmR6ZHlrNm1LVXhvV0VLdEYwOFpzUW9lVUhkTFJzektxTVFYcFdPZUIxR1BPdEJWSyt2U0dzTUwvbkNTUUhqdXYrR3pRR1NCZ1VuYnNvOEJUeWl1UmVyWHVQSThrb2tPUzZYWXBHWjJRUmN6K2crTGIyNEF0YndBMVlKNUJ3bndKSjliNmEyYk1MaXpDNkV0SXp0TGp3bHlENnpPcmNqOVZuQVM0TDBwaEE0ZWRqNUwvTWF0bDVVN0ZiUmQ0Qi96Qm81eTdKY1JnPT0=

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

            ItemBase item = ItemBase.Cast(action_data.m_MainItem);
            if (player && targetPlayer && targetPlayer.m_InediaInfectedAI_ConfigPlayers && item) {
				bool surgeryStatus = false;
				bool hasSepsis = false;
				
                float successChancePercent;
				if (!HasTarget()) {
					successChancePercent = Math.Clamp(GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain InternalBleedingStoppingItemSuccessChancePercent"), 0, 100);
				} else {
					successChancePercent = Math.Clamp(GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain InternalBleedingStoppingItemAssistSuccessChancePercent"), 0, 100);
				}
				
				successChancePercent = Math.Clamp(InediaInfectedAI_Restrictions.PainSystem_ModifyInternalBleedingClosureSurgerySuccessChance(player, item, successChancePercent), 0, 100);
				
                if (successChancePercent > 0 && Math.RandomFloatInclusive(0, 100) <= successChancePercent) {
                    targetPlayer.InediaInfectedAI_GetPainManager().DeactivateInternalBleeding();
					surgeryStatus = true;
                } else {
                    targetPlayer.InediaInfectedAI_PlayPainSoundServer(5);
                    player.InediaInfectedAI_SendNotSpamingImportantMessage("#STR_inediainfectedai_internal_bleeding_treatment_tool_stop_fail_chance", 5, player.m_InediaInfectedAI_ConfigPlayers.PainSystemNotificationsType, 5);
                }

                float procedureBloodLoss = GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain InternalBleedingStoppingItemProcedureBloodLossMl");
                if (procedureBloodLoss < 0) procedureBloodLoss = 0;
                if (procedureBloodLoss > 0) {
					procedureBloodLoss *= m_InediaInfectedAI_ProcedureBloodLossMultiplier;
                    if (player.GetModifiersManager() && player.GetModifiersManager().IsModifierActive(InediaInfectedAI_eModifiers.MDF_BLOOD_CLOTTING)) {
                        if (player.InediaInfectedAI_GetPainManager().m_BloodClottingItemClottingMultiplier > 1) {
                            procedureBloodLoss /= player.InediaInfectedAI_GetPainManager().m_BloodClottingItemClottingMultiplier;
                        }
                    }
                    targetPlayer.DecreaseHealth("GlobalHealth", "Blood", procedureBloodLoss);
                }

                PluginTransmissionAgents transmissionAgents = PluginTransmissionAgents.Cast(GetPlugin(PluginTransmissionAgents));
                transmissionAgents.TransmitAgents(item, targetPlayer, AGT_ITEM_TO_FLESH);

                if (!item.GetCleanness()) {
                    targetPlayer.InsertAgent(eAgents.WOUND_AGENT);
					hasSepsis = true;
                }

                int quantityPerUse = GetGame().ConfigGetInt("CfgVehicles " + item.GetType() + " InediaPain InternalBleedingStoppingItemQuantityPerUse");
                if (quantityPerUse > 0) {
                    if (item.HasQuantity()) {
                        item.AddQuantity(-quantityPerUse, true);
                    } else {
                        item.Delete();
                    }
                }
				
				InediaInfectedAI_Restrictions.PainSystem_InternalBleedingClosureSurgeryEndEvent(player, item, surgeryStatus, hasSepsis);
            }
        }
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

class InediaInfectedAI_ActionInternalBleedingStopSelf : InediaInfectedAI_ActionInternalBleedingStop
{
    void InediaInfectedAI_ActionInternalBleedingStopSelf()
    {
        m_InediaInfectedAI_Text = "#STR_inediainfectedai_internal_bleeding_treatment_tool_stop_internal_bleeding";
    }

    override bool HasTarget()
    {
        return false;
    }
}

class InediaInfectedAI_ActionInternalBleedingStopTarget : InediaInfectedAI_ActionInternalBleedingStop
{
    void InediaInfectedAI_ActionInternalBleedingStopTarget()
    {
        m_InediaInfectedAI_Text = "#STR_inediainfectedai_internal_bleeding_treatment_tool_stop_internal_bleeding";
    }

    override bool HasTarget()
    {
        return true;
    }
}
