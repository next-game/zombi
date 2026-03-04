/**
 * Created: 2024-12-03
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

class InediaInfectedAI_ActionExtractBulletCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        float actionTimeSeconds = 10;

        if (m_ActionData.m_MainItem) {
            actionTimeSeconds = GetGame().ConfigGetFloat("CfgVehicles " + m_ActionData.m_MainItem.GetType() + " InediaPain ExtractBulletItemActionTimeSeconds");
			
			if (m_ActionData.m_Player) {
				actionTimeSeconds = InediaInfectedAI_Restrictions.PainSystem_ModifyExtractBulletSurgeryActionTime(m_ActionData.m_Player, m_ActionData.m_MainItem, actionTimeSeconds);
			}
			
            if (actionTimeSeconds < 0) {
                actionTimeSeconds = 0;
            }
        }

        m_ActionData.m_ActionComponent = new CAContinuousTime(actionTimeSeconds);
    }
}

class InediaInfectedAI_ActionExtractBullet : ActionContinuousBase
{
    string m_InediaInfectedAI_PainLimb = "head";
    string m_InediaInfectedAI_Text = "";
	
	float m_InediaInfectedAI_ProcedureBloodLossMultiplier = 1;

    void InediaInfectedAI_ActionExtractBullet()
    {
        m_CallbackClass     = InediaInfectedAI_ActionExtractBulletCB;
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

// nZ//qRLI3cmjuO66I5JrSWRZZ3piVFRvMWQ5ZzR1WEZXRWM0V2RuQjh6R1hFS2ZIaU1YYUZkTEdBbVFyVlV0eFNaNEtjL2JsSHRvdnJqWERuMUc2MkRqYnFvTytVUU1obW5zRjltM05Bc3VCTlVIUVA1ZjdzS1FhUExhL1dYalBzVUFjMzFPNW9NZXhUWlZqbkc1M0tMSDQyZDBFazlBSnJ5SlRqQU80d1dwMldTTE1XVmVJNTBRWm56YVhySVhHUkluQ3dsemFXbi93aTdtNnpwVDBncHc5V1dmV3BNMHdXY3Y4a3Zsbkl3WFZWcW5QMFlKL3ZLcWllWXhVMlJ1Rk9hTHovaWtORER0Y2lUWVFyNEw5cUJ3UjVZMlZHRG5EeHJaaDlRPT0=

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

		if (!player || !player.InediaInfectedAI_GetPainManager().LimbsBulletSystemIsActive()) {
			return false;
		}

		if (!targetPlayer || !targetPlayer.IsAlive()) {
			return false;
		}

        array<string> allowedLimbs = new array<string>;
        GetGame().ConfigGetTextArray("CfgVehicles " + item.GetType() + " InediaPain ExtractBulletItemAllowedLimbs", allowedLimbs);
        if (allowedLimbs.Find(m_InediaInfectedAI_PainLimb) < 0) {
            return false;
        }

		if (!HasTarget()) {
			int animationId = GetGame().ConfigGetInt("CfgVehicles " + item.GetType() + " InediaPain ExtractBulletItemActionAnimationId");
			if (animationId > 0) {
				m_CommandUID = animationId;
			} else {
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_STITCHUPSELF;
			}
		}
				
		m_InediaInfectedAI_Text = "[#STR_inediainfectedai_" + m_InediaInfectedAI_PainLimb + "]";
		if (HasTarget()) {
			m_InediaInfectedAI_Text = m_InediaInfectedAI_Text + " [#STR_inediainfectedai_other_player]";
		}
		string actionText;
		GetGame().ConfigGetText("CfgVehicles " + item.GetType() + " InediaPain ExtractBulletItemActionText", actionText);
		if (actionText != "") {
			m_InediaInfectedAI_Text = m_InediaInfectedAI_Text + " " + actionText;
		} else {
			m_InediaInfectedAI_Text = m_InediaInfectedAI_Text + " #STR_inediainfectedai_bullet_extraction_tool_extract_bullet";
		}

		string tips = "";
		if (targetPlayer.InediaInfectedAI_GetPainManager().m_ShowMedicationInfo && GetGame().ConfigGetInt("CfgVehicles " + item.GetType() + " InediaPain ShowTips") == 1) {
			float successChancePercent;
			if (!HasTarget()) {
				successChancePercent = Math.Clamp(GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain ExtractBulletItemSuccessChancePercent"), 0, 100);
			} else {
				successChancePercent = Math.Clamp(GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain ExtractBulletItemAssistSuccessChancePercent"), 0, 100);
			}
			
			successChancePercent = Math.Clamp(InediaInfectedAI_Restrictions.PainSystem_ModifyExtractBulletSurgerySuccessChance(player, item, successChancePercent), 0, 100);
			
			float procedureBloodLoss = GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain ExtractBulletItemProcedureBloodLossMl");
			if (procedureBloodLoss < 0) procedureBloodLoss = 0;
			tips = " [#STR_inediainfectedai_success_chance: " + successChancePercent + "%] [#STR_inediainfectedai_blood_loss: " + procedureBloodLoss + "ml]";
		}

		m_Text = m_InediaInfectedAI_Text + tips;

#ifdef SERVER
		m_InediaInfectedAI_ProcedureBloodLossMultiplier = 1;
		
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

			// Painkiller validator
		    int requiredPainkillerLevel = GetGame().ConfigGetInt("CfgVehicles " + item.GetType() + " InediaPain ExtractBulletItemRequiredPainkillerLevel");
		
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
				if (targetPlayer.InediaInfectedAI_GetPainManager().GetPainWithBuffsLevelForLimb(m_InediaInfectedAI_PainLimb) > InediaInfectedAI_LimbPainLevels.ZERO) {
			      targetPlayer.InediaInfectedAI_PlayPainSoundServer(5);
			    }
			} else {
				return false;
			}
        }
#endif

		return targetPlayer.InediaInfectedAI_GetPainManager().GetBulletCountInLimb(m_InediaInfectedAI_PainLimb) > 0;
    }

	override void OnStartAnimationLoopServer(ActionData action_data)
	{
        if (action_data) {
			PlayerBase targetPlayer;
			
            if (!HasTarget()) {
		        if (action_data.m_Player) {
					targetPlayer = action_data.m_Player;					
		        }
            } else {
                if (action_data.m_Target) {
                    targetPlayer = PlayerBase.Cast(action_data.m_Target.GetObject());
                }
            }
			
			if (targetPlayer && targetPlayer.IsAlive()) {
				if (targetPlayer.InediaInfectedAI_GetPainManager().LimbsDeepWoundSystemIsActive()) {
					targetPlayer.InediaInfectedAI_GetPainManager().ActivateDeepWoundForLimb(m_InediaInfectedAI_PainLimb, 0.2);
				} else {
					targetPlayer.InediaInfectedAI_AddRandomBleedingSource();
				}
	        }
		}

		super.OnStartAnimationLoopServer(action_data);
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

            ItemBase item = ItemBase.Cast(action_data.m_MainItem);
            if (player && targetPlayer && targetPlayer.m_InediaInfectedAI_ConfigPlayers && item) {
				bool surgeryStatus = false;
				bool hasSepsis = false;
				
                float successChancePercent;
				if (!HasTarget()) {
					successChancePercent = Math.Clamp(GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain ExtractBulletItemSuccessChancePercent"), 0, 100);
				} else {
					successChancePercent = Math.Clamp(GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain ExtractBulletItemAssistSuccessChancePercent"), 0, 100);
				}
				
				successChancePercent = Math.Clamp(InediaInfectedAI_Restrictions.PainSystem_ModifyExtractBulletSurgerySuccessChance(player, item, successChancePercent), 0, 100);
				
                if (successChancePercent > 0 && Math.RandomFloatInclusive(0, 100) <= successChancePercent) {
                    targetPlayer.InediaInfectedAI_GetPainManager().SetBulletCountInLimb(m_InediaInfectedAI_PainLimb, targetPlayer.InediaInfectedAI_GetPainManager().GetBulletCountInLimb(m_InediaInfectedAI_PainLimb) - 1);
					surgeryStatus = true;
                } else {
                    targetPlayer.InediaInfectedAI_PlayPainSoundServer(5);
                    player.InediaInfectedAI_SendNotSpamingImportantMessage("#STR_inediainfectedai_bullet_extraction_tool_extract_bullet_fail_chance", 5, player.m_InediaInfectedAI_ConfigPlayers.PainSystemNotificationsType, 5);
                }

				float procedureBloodLoss = GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain ExtractBulletItemProcedureBloodLossMl");
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

                int quantityPerUse = GetGame().ConfigGetInt("CfgVehicles " + item.GetType() + " InediaPain ExtractBulletItemQuantityPerUse");
                if (quantityPerUse > 0) {
                    if (item.HasQuantity()) {
                        item.AddQuantity(-quantityPerUse, true);
                    } else {
                        item.Delete();
                    }
                }
				
				InediaInfectedAI_Restrictions.PainSystem_ExtractBulletSurgeryEndEvent(player, item, surgeryStatus, hasSepsis);
            }
        }
    }
}

// = .. :. -%-  .-    .:.................................................................................................................................
// +- :. =  +%.  =.    :::............................................--====-:::::.......................................................................
// .+: -  =  *#.  +    .:-:......................................:-+**++==----:-=:::--------:............................................................
// .:+: -  - .#*  .=   ..:-:..................................-*###**+++=::---.  ...   :--:-=+=..........................................................
// ..-+:.- :: :%+  -:   .:.-:..............................:=+=-::--====--:::.-:::::: -..:---:----.......................................................
// ...=+..: :. =%-  +   : -.-:..........................:==-:-****+=++-:...:--=====----=#=.  .:::-=--....................................................
// ....+= :. -  +%: .+   - =.-:........................== :**+=-..-=: :----:.:--::. .:+-. :==-:.  .-==-..................................................
// .....+= -  =  ##  :=  .- = --......................+:.*=--: :=-:---.   :-=-. :----::-++%-  ..:--:.===-................................................
// .....:+- -  = :%*  =.  =- - -....................:* -*::..++---:    :--:.:--:..-=+**+=*=.*%%%%*+--::-==-:.............................................
// ......:+- - := -%=  +   *..- =...................*.+=.:.=#+=+.   .--..---. :++=:..-+**##=+.  .:----...:=:-:...........................................
// .......-+. : =- +%. .+  .* -.:-.................+:--:.==#*-.    --..==: :++:   .-=:--:.++-++=-----:--...-::-..........................................
// ........=*..: #: ##  :-  -- = +................:+:-:.+*#-.     =..-=:.=*:    ----:::+#%%%%%%%+:   :-+++.::-.=.........................................
// .........++ :::%..%=  =.  =..- =...............+:-..+#+   :   + :=- -#+    -=:-: -##+-::--=##%%*:   .+#*.--:::........................................
// ..........+=:% =* .=.  +  :+ -.::..............*:. =#:  :+.  = :=: =+=   --::..=#: = .::-=--=*%%#-    -%+=-= =........................................
// ..........:+*%* *=  -  .-  == = =.............+:  =#: :=+   +.+=: +=-. :=-:..++::= -   :.---+.+%%=:.   -%#-= =........................................
// ...........-=%%+ #: .:  -. :+:.-.+...........:*  =%:.===  .+.++: *+.: =-: .+#=    =    = =+*.=*%%%..-   *%==:-........................................
// ............+=%%-:%. :   =  :* -.:-..........+: :#-=-=:  :=.*=. **  .=-.-+= :==:  :=   +.=*=-=*:%%#  =  -%+=+.........................................
// .............=+%%.-#  :  .- .=+ = =..........* -+=+-:  :=:.*=..#*  :=-:+*-    ---. --. ++=+-=*  #%*+ =- :*+=+.........................................
// .............:-#%# ++ ..  =  :+::=.=........*==++-  :=-..+#= :#- .--:.#*.      :-:: := +*-+*+   =%**=-= +*=-=.........................................
// ..............=:%%* *- -  .=  -*.=:=......:*%*=. :-=:.:+%+..==.:---.=*-::.      .=:=:.-=:**-   .:%%.++*.#==:-.........................................
// ...............+=%%=.#. -  -. .+= +=+:--+**+--=====+**+-::-=--:::-+*++*#%%%*+-:.  -:--=-#+.:-+#%#%%=+:=+%:==..........................................
// ...............--*%%-:# :.  =  -=+ :#+==#::---=+=#+-:::=---. :===:       .-+#%%%%- .=:=*=*%%#*=:.%%%: =-:===..........................................
// ................=.#%%.=+ -  ::  *=- :=:+    .=:.+:  :--.:=+#%+.   .--==++-:::.:+#-   :-*%*+++*+=*%#*#:%+---*..........................................
// .................=:%%#.*-.-  =  :=+  *=   -=: ==  --:.-+*%%- :*-:  .=#%+*+*+-+..#%-   ++=+++*=%*#%%-.#+#.:=-=-........................................
// .................-:=%%#.#.-. .=  +-*+. -++. :+. -=:-+%%%%%%+   ==--:  :==+=--+.  :-: .%:.:-==+- =%%*-:+-=- .:---:.....................................
// ..................+.#%%+   +  -..=*==**-. .+- :=.=%%##%%*%%++.  .=-.+:::::::.      - .#         -%%#++.:-==::.-:-+--:-+==:............................
// ...................+:%%%+..=++++-=+*-   :+- -+::#%##%=-%#*# =+-   .-=:             :. *         =%%%%+==. .:::::::::::................................
// ...................:-=%%%%#+=:-+++=  :-+= .-+ -%#+*#%*...-*  =:=:    *:          . --:+-       .%%%##%%+++-:..........................................
// ....................+=*=--:.-***+----=-. ::=-*%**#=+%%%+.-%   - .--: =-:       :-:%=   =:      ##%%+..:-=+*=:::.......................................
// .................=++-:=+**###+=::.:--. -===**#*#+.*++*#%%#*.   =   .--:=:      * =+=++=+=     +%=%%%.......+#::=......................................
// ................++=+*+=-:..  .:---. .=+-+*+=*#%- *: .*%%%%-+   -.       =.     .=*+=*#=:     =%%==%%*.......-%-.=.....................................
// ..............:#*+=. .:-=-=--:.:-=+###*+==**#%:.*:  =*%%%%+*.   =        =         :-       -%%%*-:*%+.......+=:-.....................................
// ............:+*=...:==-::-=+**+====+++*####%*.-=  .-+%%%%%#:*   ::       :.   .::-::--:-.  :%%%%%+-.:**:.....--#:.....................................
// +===:....:-=-....:+=:-==:::-=++*#%%####%%%*---. .:+%+%%%%%%.-*.  -:       -.+=++****+==-+..#%%%%%%*::..+-....:=#-.....................................
// .::-++==:.......=+-==..:+#%%#######%%%%*+--:   :=#%=%%%%%%%: :#+. .-:       .-:::  =:-=:..#%%%%%%%%#:-. --...=-*......................................
// ......::=+=--:.+*+=..-*%***#%%%%##**=-...   .:+#%%+%%%%%%%%=   =%*=..:::       :+##**+=::%%%%%%%%%%%#.+. -...--.......................................
// ..........:--=+%#:::+#=+##%#+-:::..      .-+*%%%*+%%%%%%%%%+     =%%#+-::::      .     :%%%%%%%%%%%%%::-..-..-........................................
// ..............=%-::*+==:+==...-:    .-=+#%%%%%**#%%%%#*%#++*      .*%%%%%*==::.       =%%%%%%%%%%%%%%--:.-==-.........................................
// ..............#+:*#=----..-...#-+==-*##%%%****%%%%%%%#=*#%##:.      -#%%%%%%%#**+===+#%%%%%%%%%%%%%%%--..*-...........................................
// ..............#:=#-......-...:* -#==#*%#*#%%%%%%#%%%%- .--=+***+=-.   -#%%%%%%%%###%%%%%%%%%%%%%%%%#+:..+:............................................
// ..............=.+-.......=..=- =%%+#=#+#%%%%%%%#*%%#%-       .::==+**+-:=*%%%%%%%%#**%%%%%%%%%%%%%%--.=+..............................................
// ..............:.:........=-+:=**%++*-=%%%%%%%%#*%%++=---.        ::-==:=#%%%%%%%%%%%%**%%%%%%%%%%%=:-=:...............................................
// ................-......:-+++#**%%=++-#%%%%%%##%%#.  .-=::==-:       :=-----+**+*%%%%%%=%%%%%%%%%*=-...................................................
// .................-=+++++*####%%%%%%%%%%%%%##%%#+       :=. .-++=:.    =:  ..:=#*%%%%%%+%%%%%%#*-:.....................................................
// ...................:=*#####*++==+++++#%%*#%#+=#*=:       --    .-+*=--:==      .%%%%%=:.-++*-:........................................................
// ...............-+++=:     :-==:    :=--++-  .-==**#*-.    .=       :--==+++=-.  -+%#--     =+.........................................................
// ............-*#+--------.     .==:    =--=+.   -=*+**#+=:   +.        .-==*::-=**+-  ::   :=:**+=-=-----:.............................................
// ..........=*=.          :--.     :=-   :=:+=-   .+*.-++====-:+.           =:---=%    :-  :+ .*=%#+=+=+=--==++=-:......................................
// ........=#+::::            :=-.    .=-   +.*-+   -**  :+#*=:.-++-.         -    *-   := += -+ =#=+#++===--.-:-+==--:..................................
// ......-#-     .:---:          -=:    .+. .+.*-+  .##:   .-+%%+-:.-===-.         =#   :+.:.*=.+- -+-#%+--=-- -:.=-=::==:...............................
// ....-*#=-===+++=-: .---.        :=-    -+.-=-*=- .%=+       :=**+===-:-==-:  .  .%.   *=**:-=    .==+%*.-=+- .:.:=-=:=--=:............................
// ...*#+=:        .-+++-.:==-.      .=-.=+*=-*.#:*.-%++.          -=++++===--=++*: *- .+%+--=.       :++%%::=--  .:.=.:.:-.-+-..........................
// ..:%#= :=+=.         -=++-:-=---:---===+-.-+:==:*#%%:=              :=++++=======+#*+-:==.          .=-#%- ==:  =  +  . -::-+-........................
// ..:%-     .=+=:        :-+++++=====*+=.     -#+ *%%%-+                   :-----====---:               :=%%+.-+.  = :: .  :-=--+:......................
// .=++-----:::..-++-.-==-.       :==-      :+%%** :%%%*:-                                                -+%+*=:#  =- =..   -:-=:-=.....................
// *+..   .   ..--. .:.        :==-   .:-=+#%%#- *  %%%%.+.-:.                                           :++%%#* #- .=-.*:. : + =-=:+....................
// #*+++++++=-.   :.        -+=:  :=*#%%%%%*=.   +. #*%%#=*####**+=--:.                              .-++===+%%%= *   -:*   .:=. --=:*:..................
// *=#*#%##%%%%%#*+=-:.     . :=*%%%%**+. +      +. #+*%#++++++++++++=+*+==-:.                    :=+=::+*%%##%%%::=   -:.   .-   :-=**-.................
// %%-...:--=+=+*#%%%%%%#**+*%%%%#+: .*  =       + .%..=%*=--:.  ..:-+++++==+++=---:.         :=++-.:=*+*=.   -%%%=.:   .:::::     :-+-:-=:..............
// %=               .:-+*##%%%%*:   -+  =.       + +#    :.  .:-----:    .-==+*+==----==+==+*+-::-++--+-       -%=%*.          :::::..  ..:=:............
// *=         .:------==+++++-    =*: :+.       +.:%+---:-------:   .-=-:      :-=+++*=-:::..-++=%= ==         .-#:#%#**+=-:. ...::--=--:::::=-..........
// *-+:  :-------=+***+=-.     :+*- :+-       .+.:%+            .-==-.  .-=-.      .-=+==--==-. -* +:        .+  =%-=**%%%%%%%*#%%%####***+===+*-........
// %*=-====+***=-:.       .:-++=. -*+::-----==-.+%*                  :--:   ---.      .-++=.    %.=:        -+ .+*#%#+-..=#%%%%%%%%........:--==+*-......
// %+*++-:.      .:-=++++==++. :+=:::..    .:=*%%=                       ---:: :=-       -=+:  -% +        -+ :*.   =%*--  ++::--#%......................
// %*-.*.  .-====-:.     :+: -#%++++++++*#%%%%%%%:     .=.                  -==-:.==      .=== -# =       .* .*      +%*:+..=.   =%-.....................
// %+::++--:.   ..::----=-.=#%%%%%%%%%%%%%%%%%%%%:   :=:                      .-=-: --.     :+=-* =       +..*        =%#:-=:=:  -%=.....................
// - -:  .:-+*##*+=-:::..=#%%%%%%%%%%%%%%%%%%%%%%:  ==  .------.                 -=-: --     .*-# -      := *.         :%#::*+-=:.%+.....................
// +-+*#%%%*******%%%%%%%*#%%%%%%%%%%%%%%%%%%%%%%  =-          :----.              -==..=.    :+*.       * +-           #%%%%%%=+=%*.....................
//  :+%%#:       +.=+-=--%%%%%%%%%%*=---==*%%%%%- -=                :---.            -=- --    -+-      =::+           -#%*#==#%*=#*.....................
//  *%%+        :: + . -%%%%%%+++=         .#%%+ :*                     :--:          .=+.:=    =#     .= *            ##%=#.  +%%**.....................
// #%#:         = +:  =%%%+.#%%:            *%* .*.                        -==:         =+..=    #:    + =-           =+*%+-=   %%:......................
// %*          = -= .*%%%-...*%%-          =%= .*:                            :==:       =+.:=   -*  .+ -=          .=*-+%# *   *%-......................
// -          -.:* -#%%#:.....+%%*.      -#*. :*-   .::                         .=+:      =+ --   %==- =-          -+-+*#%%*:=  +%=......................
//           .- *-+%%%*........-%**-   :**: .=*-      .==-.                       .++.     += =:  #- -=.          =--##%%%%%+:= =%=......................
//           : ++#%%%=...........+*+*=*+: :-*+.           :-=-.                     -*-     +=.:--%+-.          :=.+#%%%%%*:#*:=+%=......................

class InediaInfectedAI_ActionExtractBulletSelf : InediaInfectedAI_ActionExtractBullet
{
    override bool HasTarget()
    {
        return false;
    }
}

class InediaInfectedAI_ActionExtractBulletTarget : InediaInfectedAI_ActionExtractBullet
{
    override bool HasTarget()
    {
        return true;
    }
}


class InediaInfectedAI_ActionExtractBulletHeadSelf: InediaInfectedAI_ActionExtractBulletSelf
{
    void InediaInfectedAI_ActionExtractBulletHeadSelf()
    {
        m_InediaInfectedAI_PainLimb = "head";
    }
}
class InediaInfectedAI_ActionExtractBulletHeadTarget: InediaInfectedAI_ActionExtractBulletTarget
{
    void InediaInfectedAI_ActionExtractBulletHeadTarget()
    {
        m_InediaInfectedAI_PainLimb = "head";
    }
}

class InediaInfectedAI_ActionExtractBulletArmsSelf: InediaInfectedAI_ActionExtractBulletSelf
{
    void InediaInfectedAI_ActionExtractBulletArmsSelf()
    {
        m_InediaInfectedAI_PainLimb = "arms";
    }
}
class InediaInfectedAI_ActionExtractBulletArmsTarget: InediaInfectedAI_ActionExtractBulletTarget
{
    void InediaInfectedAI_ActionExtractBulletArmsTarget()
    {
        m_InediaInfectedAI_PainLimb = "arms";
    }
}

class InediaInfectedAI_ActionExtractBulletLegsSelf: InediaInfectedAI_ActionExtractBulletSelf
{
    void InediaInfectedAI_ActionExtractBulletLegsSelf()
    {
        m_InediaInfectedAI_PainLimb = "legs";
    }
}
class InediaInfectedAI_ActionExtractBulletLegsTarget: InediaInfectedAI_ActionExtractBulletTarget
{
    void InediaInfectedAI_ActionExtractBulletLegsTarget()
    {
        m_InediaInfectedAI_PainLimb = "legs";
    }
}

class InediaInfectedAI_ActionExtractBulletTorsoSelf: InediaInfectedAI_ActionExtractBulletSelf
{
    void InediaInfectedAI_ActionExtractBulletTorsoSelf()
    {
        m_InediaInfectedAI_PainLimb = "torso";
    }
}
class InediaInfectedAI_ActionExtractBulletTorsoTarget: InediaInfectedAI_ActionExtractBulletTarget
{
    void InediaInfectedAI_ActionExtractBulletTorsoTarget()
    {
        m_InediaInfectedAI_PainLimb = "torso";
    }
}
