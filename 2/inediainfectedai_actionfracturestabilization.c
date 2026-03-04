/**
 * Created: 2024-12-03
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

class InediaInfectedAI_ActionFractureStabilizationCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        float actionTimeSeconds = 3;

        if (m_ActionData.m_MainItem) {
            actionTimeSeconds = GetGame().ConfigGetFloat("CfgVehicles " + m_ActionData.m_MainItem.GetType() + " InediaPain FractureStabilizationItemActionTimeSeconds");
            if (actionTimeSeconds < 0) {
                actionTimeSeconds = 0;
            }
        }

        m_ActionData.m_ActionComponent = new CAContinuousTime(actionTimeSeconds);
    }
}

class InediaInfectedAI_ActionFractureStabilization : ActionContinuousBase
{
    string m_InediaInfectedAI_PainLimb = "head";
	string m_InediaInfectedAI_Text = "";

    void InediaInfectedAI_ActionFractureStabilization()
    {
        m_CallbackClass     = InediaInfectedAI_ActionFractureStabilizationCB;
        m_FullBody          = true;
        m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;

        if (!HasTarget()) {
            m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
        } else {
            m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BANDAGETARGET;
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

		if (!player || !player.InediaInfectedAI_GetPainManager().LimbsBreakSystemIsActive()) {
			return false;
		}

		if (!targetPlayer || !targetPlayer.IsAlive()) {
			return false;
		}

		if (!item) {
			return false;
		}

		array<string> allowedLimbs = new array<string>;
		GetGame().ConfigGetTextArray("CfgVehicles " + item.GetType() + " InediaPain FractureStabilizationItemAllowedLimbs", allowedLimbs);
		if (allowedLimbs.Find(m_InediaInfectedAI_PainLimb) < 0) {
			return false;
		}
		
		if (!HasTarget()) {
			int animationId = GetGame().ConfigGetInt("CfgVehicles " + item.GetType() + " InediaPain FractureStabilizationItemActionAnimationId");
			if (animationId > 0) {
				m_CommandUID = animationId;
			} else {
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
			}
		}
		
		m_InediaInfectedAI_Text = "[#STR_inediainfectedai_" + m_InediaInfectedAI_PainLimb + "]";
		if (HasTarget()) {
			m_InediaInfectedAI_Text = m_InediaInfectedAI_Text + " [#STR_inediainfectedai_other_player]";
		}
		string actionText;
		GetGame().ConfigGetText("CfgVehicles " + item.GetType() + " InediaPain FractureStabilizationItemActionText", actionText);
		if (actionText != "") {
			m_InediaInfectedAI_Text = m_InediaInfectedAI_Text + " " + actionText;
		} else {
			m_InediaInfectedAI_Text = m_InediaInfectedAI_Text + " #STR_inediainfectedai_stabilize_break";
		}

		string tips = "";
		if (targetPlayer.InediaInfectedAI_GetPainManager().m_ShowMedicationInfo && GetGame().ConfigGetInt("CfgVehicles " + item.GetType() + " InediaPain ShowTips") == 1) {
			//if (m_InediaInfectedAI_PainLimb == "arms" || m_InediaInfectedAI_PainLimb == "legs") {
				float shockReductionMultiplier = GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain FractureStabilizationItemShockReductionMultiplier");
				if (shockReductionMultiplier < 1) shockReductionMultiplier = 1;
				if (shockReductionMultiplier > 1) {
					tips = tips + " [#STR_inediainfectedai_shock_reduction: x" + shockReductionMultiplier + "]";
				}
			//}

			float regenerationMultiplier = GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain FractureStabilizationItemRegenerationMultiplier");
			if (regenerationMultiplier < 1) regenerationMultiplier = 1;

			if (regenerationMultiplier > 1) {
				tips = tips + " [#STR_inediainfectedai_regeneration_bone: x" + regenerationMultiplier + "]";
			}
		}

		m_Text = m_InediaInfectedAI_Text + tips;

        return targetPlayer.InediaInfectedAI_GetPainManager().IsLimbBroken(m_InediaInfectedAI_PainLimb) && !targetPlayer.InediaInfectedAI_GetPainManager().IsLimbStabilized(m_InediaInfectedAI_PainLimb);
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
            if (targetPlayer && targetPlayer.m_InediaInfectedAI_ConfigPlayers && item) {
				float shockReductionMultiplier = GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain FractureStabilizationItemShockReductionMultiplier");
				if (shockReductionMultiplier < 1) shockReductionMultiplier = 1;

				float regenerationMultiplier = GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain FractureStabilizationItemRegenerationMultiplier");
				if (regenerationMultiplier < 1) regenerationMultiplier = 1;

                targetPlayer.InediaInfectedAI_GetPainManager().ActivateBreakStabilizeForLimb(m_InediaInfectedAI_PainLimb, shockReductionMultiplier, regenerationMultiplier, item);

                PluginTransmissionAgents transmissionAgents = PluginTransmissionAgents.Cast(GetPlugin(PluginTransmissionAgents));
                transmissionAgents.TransmitAgents(item, targetPlayer, AGT_ITEM_TO_FLESH);

                int quantityPerUse = GetGame().ConfigGetInt("CfgVehicles " + item.GetType() + " InediaPain FractureStabilizationItemQuantityPerUse");
                if (quantityPerUse > 0) {
                    if (item.HasQuantity()) {
                        item.AddQuantity(-quantityPerUse, true);
                    } else {
                        item.Delete();
                    }
                }
            }
        }
    }
}


class InediaInfectedAI_ActionFractureStabilizationSelf : InediaInfectedAI_ActionFractureStabilization
{
    override bool HasTarget()
    {
        return false;
    }
}

class InediaInfectedAI_ActionFractureStabilizationTarget : InediaInfectedAI_ActionFractureStabilization
{
    override bool HasTarget()
    {
        return true;
    }
}

// ......................................................................................................................................................
// .............................................................................  .......................................................................
// .....................................................................  .  .              .............................................................
// ........................................................ ........      .::--::::..            ........................................................
// ......................................................   ..         .=*%%%%%%%%%%%#+-.            ....................................................
// ................................................                  .+#**##%%%%%%%%%%%%%=.             .................................................
// .............................................                    =*+*%%####%%%#+*+*%%%%#:                  . .........................................
// ...........................................                     ++%#=+#%%%%=:   -   :%#%%=                     .......................................
// ......................................                         -*%==%%%%%%#+-   -   -%%*+%*.                    ......................................
// .....................................                          %%**%%#%#=.  .+. :.  #.=%#*%%-                      .. ................................
// ................................ .                            :%%%%##%-       +:.- *=::=%+%%%+.                         ..............................
// ..............................                                :%%%#%+----::::::*===+::::%#+%%%+-                         .............................
// ...........................                                 .:=%%%*+**=-=+##*+=-=%#=+==+%%%+=#%=+.                          ..........................
// .........................                        .:::-----===--%**+%-  =#***** ..+##**=%%%%%%+-#=+.                           ........................
// .......................                                      :=*%--%:   .--:..:...:---.%%%%%%%%=+==                             ......................
// .....................                          .:----------=#%%%%% .=         . .      %%%%%%%%%=#.:                            ......................
// ....................                        :-:. ...:-=+*#%%%%%%%%#=*        -  .:    =%%%%%%%%%%#.                               ....................
// ..................                        :=***+=====*#%%%%%%%%%%%%%%:       :-:::   -%%%%%%%%%%%*                                  ..................
// .................                       :-:::  .:-==*%%%%%%%%#%%%%%%%#      .:-:--  .#%%%%%%%%%%=                                    .................
// ................                        . . .==*%%%%%#####%%%%*+=#%%%%*.   .==---=:.#%%%%%%%%*-                                       ................
// ..............                             :=:*%%%%%%##%%#****+*%+-#%%%%*-.   .:. .#%%%%%%%*.                                          ...............
// .............                             .=-*+#%%###%#**%%###%*-=#*#%#%%%%*=.  .-#%%%%%%%%-                                            ..............
// .......... .                           .  =-#*%%%%%*=-:...::=**#%*.=%# .-*%%%%%%%%%%%%++==#+                                             .............
// ...........                :==++++=-:.  --**%%%*=:        .: .++##*-=#+.   .:-=+****%%%%%%+=:.                                            ............
// ........                 :=:    :-++*##%%%%*=:              -  ==#*%:+-*  ...      .%%%%%%%%-#+=                                           ...........
// ........                 -         :-==#*=-:::::::. .:::::::::: ==%#*:=*-   .:::.  :%#*++%%%%-==*-                                          ..........
// .......                               =+.       .:-:.           .=%#- +.#*=-.      +%#*-*-=%%=*.%.-=:                                       ..........
// ......                                *+     .-:.   :::::.::::::=-%+ :=  *=-+**+=+**#+=* +=:%*% #..-:=:                                      .........
// ......                               :#:   :-:   .:------:.    ++=#.=+    +:  .:---:.  +. *::%%:*-..: -=.                                    .........
// .....                                ++  :-..-===-:..  ..:+##*#*+#+*=:::.  --    ..::::.+. +:.##=*.::   +-                                    ........
// .....                               .#-=-.=+-.         -*++*%%#++=:      :::.--:       :=   =-:=*++ .-- =*=                                   ........
// ....                               :*:.-=+:         -+*=+#%#=#*.  .-------.  ..:--:  .-:  .--==+-+*#*-.=.-*                                    .......
// .....                              =--+-:  .    .-++==*%%#-.%= .--.       :==:  .::::.  :-:   :-*+#+#++-=-+                                    .......
// .....                             :+*=:--:.  .=++-=*%%%#: -#%.=:             .==:  ::::-. .-=-. .+=%%*=-=++                                    .......
// ....                             =%=-=-   .=+=:-*%%%%%%==+%*#*     .            :==.    ---       =:##=+:++                                    .......
// ....                            :+#    :===:-*%%%%%%%#+##:+-%#    :=.              :---:           =:%*++.*                                    .......
// .....                            -%*++=-.-*%%%%%%%%%%+-*:.= #%+    .*                 :            : %%++++.                                   .......
// .....                            #*+#**#%#*#*#%%%%%#==*..# .:%%*    %             .:-:.==:         *.%%%+=*=                                   .......
// .....                            .+#+===+#%=   .-+#*=#-+#:. -:*%%-  +-         .::. .--. .-:.      *.%#%%*=+                                  = ......
// ......                             .=++*==#+++=-. -+ :#:=:.  :::*%#=.:          .-==-..:==-         -% =%%*#-                                -* ......
// .....                               * +. :=   .-=#=    +.+.   .-:.=*%#+-:..:-===-:-=****=:.==-     -%#=**%#%*                               :%+   ....
// ......                             =::=  *      +-      ===-     ::::--=====---=**+=-*+-=**+-.-==-=%%=*. :#*                               .*=+  .....
// ......                            .+ +  --     ==        .+++=-:.....:-=++++=-*:     -    -**%+==+%%%%.*                                   +:+-  .....
// .......                           -:-+-:*     :+           *--=+**+--=+:.     *      .    =: ..*-%%%%%*:=                                 -- *:  .....
// ........                         :+=- .=-:   :*            .+     *   +       *           *   -- .#%%%%-+-                               .# .*  ......
// ..........                      =+:.:=-   . :*.             .+    --  :=      *           *   *.  .#%%%%*%=:                             *: =-........
// .........                      .*.=.--.==-.==                =-    *   +      *   .:--:.  *   %---==%%%- ..=-                           =+ .*  .......
// ...........                  +*#:*.  :+-  =+                 -+--. +.  =:     *           +  :#=+  .=+.    .#                          :#. =-  .......
// ...........                 -+#%%+##-..:.  *                .+:-.:-=+---+     *           +  =* .+*-..:----*#+.                       .#- .*.. .......
// ............                * .#%#*%%#+===*:              =+%*----  .=-:#--   *           +  **   +- .   +*## ==                      +*  *:  ........
// ............               --+=*%*:.:=+##-.              -*=%--:---:   -=. :-:*           +..#*-   =+===*#%%*: :+.                   -%. -+   ........
// ..............             #-=  %%-::-=#:              .*-:+.:==-::.--.  --   --:        :+ -+:#    *     .+%* :.#+:                .%-  #.  .........
// ...............           :=*   **=. .%=               %-*%-..  .==-:.:-:  =:   .-:     =. -- -==   +.      .%:.=. +=               **  =-   .........
// ................          =-*  .+ =-:+#+             .+-+#--++=::  :=+-:.:. :-    =.   :- -= :: ==  .+     -=.#+:   +*.            =%. :*   ..........
// ................          *-=  :=  =%-=*            -=  +:=#*==+=-::  -++:.:. --   = ..* -= -:  ++-  .+:.-+:.+: .:.  +#:          .%:  #:  ...........
// ................  .       *=-  :=  =*:*-          .**-=+--. .-*#+++:::: .=*-.:..=: = -+.-= -.  -:*+   -*-. =+.    :-  =*:         *=  == .............
// ..................        *+:   + .#..#          .#+*##=. .:-: .-***=-:::. :==-:..=-.#**-:: .+*-#==+   +#=#.       .=  :+-       -#  .*. .............
// ...................       **:   +.++ +-         -+=+*##%%#=. .--. .-#*+-:.::. .==-.:=#=.:::-#*+#:-=-=   :+--=-      .+.  +:     .%.  +:  .............
// .....................    .*#.   .#%..*        :+-=*#%%#**##%#-  .-:  :*#*-  .::. :-==:--::+%=*+.=* =%.    -=.:+.      +.  *:    *-  -+   .............
// ................... ..   .*#    +%#-*.       ===*#*=::::::::-*#+.  --  :=+**-. .:-: .-::+%++#- -%+-*#+     .==:+=      +--=*-  =+  .#.  ..:...........
// ........................ :+%   .%%-+.      .*:+**:--:         .-+*: .=:  :-.-**=. .=##%%+=#+  +%#:  .+=      .=++*   -=-..--++.#   *-  ..++...........
// .........................=-%   +*%+:      .*:#=:--               %%*. .-.  -:  -+**=---=*+.  .+-%-:  .*-       .=##=+: -=-  .+%-  :+  .=#+............
// .........................+:*  .%-%=      .*:#=:=.               :=%%%*. :-. .-.    :---:.    .= +%-:   *:         -+--=:::=#=%%*  #..-+==.............
// .........................++=--=%*#.     .*:#=--               .-=*=-%%%+  --  :-.           .:  .+%-=   *.          .+#*+%#:+:=%*+--+:-=..............
// ........................:=*  :=%%-      +-#==:       .:------:-+#=  .*%%%-  -:  :-.        ::  :.-%%++=::*            =%%= .+  :%%*-:-*...............
// ........................*:+.::.#%.     ==*+=:    .--+=--==++*+-.-=-   -%%%*:  -:  :-.     -:  -:.#+.+*-==*=           -+   .#=-::#*:  .#:.............
// .......................:+:===+-+@+    :+=++.  .-+==++*#%%%%%%%+  = -:   +%%%*.  --  .: - -.  -:.#%.*%#%*#*%.          -=  :+%*=-*-  :.==..............
// ........................#. :--::-#+. .*.+=. :==-=*%%#+-.  **%%%#..  .-.  :#%%%+   -:   =-   =..#%:=%%+%%%###.     ....:* *=.# ::..:-==+...............
// ........................#-::::---=#*.+-.+..=--+%%#=.     ++.%%%%%=    :-.  -#%%%=   ::    :- :##..%%: =%%%**+.   ......*-%%.#%-:-:  :*................
// .......................:+       :*-***.. -=-#%%%-       -#  *%%%%%#:    -=   =%%%%+.  :::-: =%*  #%:   *%%%%:+  .......-#%..%%+:::--#.................
// .......................=.      .+.:-%-   -#%%*.*.       #.  :%%%%%%%+.   -:   .+%%%%+.    -#%=  #%:    .%%%%%*:......-+# +=%%%+-::-*%#:...............
// ......................:+...... =+%%+#:  +%%*.  *.      +-    *%%%%%%%%=   =     .+#%%%#**%#-= .#%-      #%*%%%#...:+#%%#+=%*#%*:.:=#-.................
// ......................:*----::-%. =-% .#%%-   =*-     =+     *%%%%%%%%%#- .=       :=+*+=. -:.#%=       +* +%%%=..........=*:-*-:-+:..................
// .......................=+:::  -*  :##:%%#-:  -=.*-:  -*     *--%%%%%%%%%%#-.=             -=:#%+*       =+-:+%%%:..........*=:#-:%:...................
// ........................-==+*+*--=-=#%%#:    #  * .-+-+.  .*-  #%%%%%%%%%%%%+*-         .*+=%%%#:-      =- .-#%%#..........+: %  %....................

class InediaInfectedAI_ActionFractureStabilizationHeadSelf: InediaInfectedAI_ActionFractureStabilizationSelf
{
    void InediaInfectedAI_ActionFractureStabilizationHeadSelf()
    {
        m_InediaInfectedAI_PainLimb = "head";
    }
}
class InediaInfectedAI_ActionFractureStabilizationHeadTarget: InediaInfectedAI_ActionFractureStabilizationTarget
{
    void InediaInfectedAI_ActionFractureStabilizationHeadTarget()
    {
        m_InediaInfectedAI_PainLimb = "head";
    }
}

class InediaInfectedAI_ActionFractureStabilizationArmsSelf: InediaInfectedAI_ActionFractureStabilizationSelf
{
    void InediaInfectedAI_ActionFractureStabilizationArmsSelf()
    {
        m_InediaInfectedAI_PainLimb = "arms";
    }
}
class InediaInfectedAI_ActionFractureStabilizationArmsTarget: InediaInfectedAI_ActionFractureStabilizationTarget
{
    void InediaInfectedAI_ActionFractureStabilizationArmsTarget()
    {
        m_InediaInfectedAI_PainLimb = "arms";
    }
}

class InediaInfectedAI_ActionFractureStabilizationLegsSelf: InediaInfectedAI_ActionFractureStabilizationSelf
{
    void InediaInfectedAI_ActionFractureStabilizationLegsSelf()
    {
        m_InediaInfectedAI_PainLimb = "legs";
    }
}
class InediaInfectedAI_ActionFractureStabilizationLegsTarget: InediaInfectedAI_ActionFractureStabilizationTarget
{
    void InediaInfectedAI_ActionFractureStabilizationLegsTarget()
    {
        m_InediaInfectedAI_PainLimb = "legs";
    }
}

class InediaInfectedAI_ActionFractureStabilizationTorsoSelf: InediaInfectedAI_ActionFractureStabilizationSelf
{
    void InediaInfectedAI_ActionFractureStabilizationTorsoSelf()
    {
        m_InediaInfectedAI_PainLimb = "torso";
    }
}
class InediaInfectedAI_ActionFractureStabilizationTorsoTarget: InediaInfectedAI_ActionFractureStabilizationTarget
{
    void InediaInfectedAI_ActionFractureStabilizationTorsoTarget()
    {
        m_InediaInfectedAI_PainLimb = "torso";
    }
}

modded class ActionSplintTarget: ActionContinuousBase
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
       	if (target) {
            PlayerBase targetPlayer = PlayerBase.Cast(target.GetObject());
			if (targetPlayer && targetPlayer.InediaInfectedAI_GetPainManager().LimbsBreakSystemIsActive()) {
				return false;
			}
		}

		return super.ActionCondition(player, target, item);

	}
};

modded class ActionSplintSelf: ActionContinuousBase
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (player && player.InediaInfectedAI_GetPainManager().LimbsBreakSystemIsActive()) {
			return false;
		}

// yAw5GNqe4yl6HW01BOu/gGh0YW8xbkw2bThHUXdkK2FnbDdQMWR2ay9iMUJ2K3gvV29BN1RtRHNHRmdnZGh5MjJEcm1ja2g2bnB1cUEzMFRGdnZ4bTUwN0oxclpJTlZGVUtQWUR2NWtQenYrSXhqS1dEaytvR3QzSzUwUW14RFloa0tiVFBHZ3NEc2wvQkhiZXNFSjVKdXhFRWlKMHdadkExYU56eXpTWjRyY1A2T3hRYTVyYUE4TllOWkcrVXgwRVNjZ0o2SVFwOVBLL1NTSTR0UWFPbVFTVER0M0VCK3ZCdjhGSk01RXcxTDJjN1JoN3JUMVJVbzlFdktiUzk4RTNPYU9mZWJ5ckxoODE4RTRicld1YTdZU0JxQWVWd1VpN1pVM1lBPT0=

		return super.ActionCondition(player, target, item);

	}
};
