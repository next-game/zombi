/**
 * Created: 2024-12-03
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

class InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        float actionTimeSeconds = 3;

        if (m_ActionData.m_MainItem) {
            actionTimeSeconds = GetGame().ConfigGetFloat("CfgVehicles " + m_ActionData.m_MainItem.GetType() + " InediaPain DeepWoundBandageItemActionTimeSeconds");
            if (actionTimeSeconds < 0) {
                actionTimeSeconds = 0;
            }
        }

        m_ActionData.m_ActionComponent = new CAContinuousTime(actionTimeSeconds);
    }
}

class InediaInfectedAI_ActionDeepWoundUnbandageIneffective : ActionContinuousBase
{
    string m_InediaInfectedAI_PainLimb = "head";

    void InediaInfectedAI_ActionDeepWoundUnbandageIneffective()
    {
        m_CallbackClass     = InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveCB;
        m_FullBody          = true;
        m_StanceMask        = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;

        if (!HasTarget()) {
            m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
        } else {
            m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BANDAGETARGET;
        }
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINotPresent();

        if (!HasTarget()) {
        	m_ConditionTarget = new CCTNone();
        } else {
            m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
        }
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        PlayerBase targetPlayer;
        if (!HasTarget()) {
            targetPlayer = player;
#ifndef SERVER
			if (targetPlayer && targetPlayer.m_CurrentCamera && targetPlayer.m_CurrentCamera.GetCurrentPitch() >= -50) {
				return false;
			}
#endif
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
		
		string text = "[#STR_inediainfectedai_" + m_InediaInfectedAI_PainLimb + "]";
		if (HasTarget()) {
			text = text + " [#STR_inediainfectedai_other_player]";
		}
		m_Text = text + " #STR_inediainfectedai_unbandage_wound_ineffective";

        return targetPlayer.InediaInfectedAI_GetPainManager().IsLimbDeepWounded(m_InediaInfectedAI_PainLimb) && targetPlayer.InediaInfectedAI_GetPainManager().IsLimbDeepWoundBandaged(m_InediaInfectedAI_PainLimb) && !targetPlayer.InediaInfectedAI_GetPainManager().IsLimbDeepWoundBandageEffective(m_InediaInfectedAI_PainLimb);
    }
	
	override bool ActionConditionContinue(ActionData action_data)
	{
		return true;
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

            if (targetPlayer && targetPlayer.m_InediaInfectedAI_ConfigPlayers) {
				targetPlayer.InediaInfectedAI_GetPainManager().m_AllowDeepWoundPainDropBelowBadlySeconds[m_InediaInfectedAI_PainLimb] = 60;
                targetPlayer.InediaInfectedAI_GetPainManager().DeactivateDeepWoundBandageForLimb(m_InediaInfectedAI_PainLimb);
            }
        }
    }
}


class InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveSelf : InediaInfectedAI_ActionDeepWoundUnbandageIneffective
{
    override bool HasTarget()
    {
        return false;
    }
}

class InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveTarget : InediaInfectedAI_ActionDeepWoundUnbandageIneffective
{
    override bool HasTarget()
    {
        return true;
    }
}


class InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveHeadSelf: InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveSelf
{
// krVYBzIy7IFv7dJvQ+30PHJzVVg3cU5mdy9hSHd1WStYclc0SUh6dWV3V25QUGkyckFzc1gyamM1M2ptblFLTEV4bFVSVlkvelpHZkNwYjN3bjZ5UGZaMzRVQXltRG5nUHpMekZXQ1RGMVdDNm1sdXhQd09rZm9vcjJrdjkreU96ckRDaEhYUlJpcGYzdnhmemhrVXNZUmRiNU90UEwwZldsUTBDTE1ZZ3I5Q2J5ZnVJaTFHTXY4TGorcEs4VisxN0Q4WkpLMmlLN1dESWtpYVRRb1FScVZZbHBOZm1tTFpuWEpkNVBvREtLR2JXMy9kRjhjY3hkV01mUlY0QkNHa2lTQS93YzNjWVQxTDdYdUplWk9NNjdKWXBISnM1MWE4RDRWOHN3PT0=

    void InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveHeadSelf()
    {
        m_InediaInfectedAI_PainLimb = "head";
    }
}
class InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveHeadTarget: InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveTarget
{
    void InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveHeadTarget()
    {
        m_InediaInfectedAI_PainLimb = "head";
    }
}

class InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveArmsSelf: InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveSelf
{
    void InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveArmsSelf()
    {
        m_InediaInfectedAI_PainLimb = "arms";
    }
}
class InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveArmsTarget: InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveTarget
{
    void InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveArmsTarget()
    {
        m_InediaInfectedAI_PainLimb = "arms";
    }
}

class InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveLegsSelf: InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveSelf
{
    void InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveLegsSelf()
    {
        m_InediaInfectedAI_PainLimb = "legs";
    }
}
class InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveLegsTarget: InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveTarget
{
    void InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveLegsTarget()
    {
        m_InediaInfectedAI_PainLimb = "legs";
    }
}

class InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveTorsoSelf: InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveSelf
{
    void InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveTorsoSelf()
    {
        m_InediaInfectedAI_PainLimb = "torso";
    }
}
class InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveTorsoTarget: InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveTarget
{
    void InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveTorsoTarget()
    {
        m_InediaInfectedAI_PainLimb = "torso";
    }
}

// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ............*=....................................................................:=+++*+++=-:........................................................
// ............* -=................................................................--=+**+=======+=......................................................
// ............*  .+............................................................:=*+#*#**##++++*#+=*:....................................................
// ............-=  -=........................................................:=+*+=+=+=*=++*+*****%==-...................................................
// .............*   +:......................................................=+=+=+++=++=+==-===+=#*#==...................................................
// .............--   #....................................................:*--=--==--+=+=+=++=**@#=#+*...................................................
// ..............*   :+...................................................*-==-+==-==+******+***+++**+...................................................
// ..............--   +-.................................................=:::-===---#=-===+++*+++*#%+....................................................
// ...............#    #.................................................*=:.:=+:    -++=++==+=====#.....................................................
// ...............-=   :*................................................+=:=+= .-=+=:  -*+++=====+%+....................................................
// ................#.   +-...............................................#=*:.+#+-:.      -#**++++%*#=...................................................
// ................-+    *...............................................==++   :*#@*:     -=+#+-:=#**+..................................................
// .................%.   :=.............................................:=*=+   ...        :   :=-.%++#=:................................................
// .................=#  . *:.............................................-%#..             .   :.:-++*=+-:...............................................
// ..................#- *:.#.............................................=@=.:- ::            ..-%-==+=#.-...............................................
// ..................:%:*:.+-.............................................#%+-.              =-:+#--=++#..:..............................................
// ...................+-:.-.#..............................................%*=-::..          +  *#-==+**:................................................
// ....................# -.-+=..............................................*+.:..          :-  *#-+++*+-................................................
// ....................+=.:-.#...............................................+=:       .    =: .+%=*++*+=................................................
// .....................%.- -=+..............................................-=. .:-+*+.   :=++-:-#++++=*................................................
// .....................-+.---#-...............................................---=%#=---++-:  = *%+###++--------........................................
// ......................#.-..-*................................................:-++. .--.      -*%+-.::::::::-==+:......................................
// .......................* =: =+.............................................-*%@@#--:      :*#+:.:=**#**+++-.  =+:.....................................
// .......................+-:-.:+-..........................................-++@@@#=.      -%#:.--==-.    ::-+#%+: --....................................
// .......................-% += -#......................................:-*====#@@*:     -%*::---:   .::::     .=*#-+....................................
// ........................%=:* :-+...................................:-..+ *-++@@:% :+*#%::---.  .--.  .:::::...:-##-...................................
// ........................=# +- -+:.................................=:.+===%*####*+-+++:.---   :-. .--:.           -*:..................................
// ........................:++.#  -*................................+- ---+@*-   .:*+-.  .*. ::: .--:     ..:::......:+-.................................
// .........................++.-: :++...............................=-=-:=%#-.::---#@@@#. :-:..::.    ....             .=................................
// .........................-+%.: :**...............................= - .%%+==:::-*@@@@@#  ::.     :::              ...:.+...............................
// ...........................+=: --+...............................* =-#@%+=-+#@@@#=:-*@: -   .:::   .-=+#%%%%###*+=---=**..............................
// ............................*.: -=+..............................*+=%%@@@@@@@%+-.   ==+ =:::. .-+#@%=. .-=-.  .=.:--=##*:.............................
// ............................:+:.::+-...........................:=@@@@+-*%@%+::.     .-# =-=+#%#%@%*+#%+: ..:---*:    -+*:.............................
// .............................*:: --+...........................+%%@**.:==:::.        --%*@@@@+ =*:=*.::+#+:.:::..-----*+*.............................
// .............................:#:. -==.........................**#%==-#=.::            ==*%%@*#- .-..**-  :=%*=-:---:  .-.+............................
// ..............................==- --+........................=:-%---= ::               =:++#@=+#-. :+@#=.  .==++#*+==-::--*...........................
// ...............................#:: -=+......................-- #-=== ::              .-=-.-=##+:+#@@@@@%=:   :-=-=:-+*####-...........................
// ...............................:*: :-+:.....................#. %---= =             -+=  = = .*-*=  .=@#%%+:    ---=-.   =+............................
// ................................*:- --*....................:=+:@=-:- =           -*:    = + ..+=-+=:%#=@%@#.     :--==-. .=...........................
// .................................#:..-==....................*= ++-=- =          *-      = #=.  .=*+@%+@%:-%+       ::=:-=-+...........................
// .................................-=: --+:...................:#. ++-:- -       .*.      = -+.--.  .%%#@%:..:#:         .:=++...........................
// ..................................*:: --+....................:*::=*---.-:     #.     :- -+     :--*%@#.....-*====-----=*:%%:..........................
// ..................................:*- ::--.....................#==#%==+.:=--:*-  .::: -+-        ==##-......:*%*-==----+-==+.:*:......................
// ...................................+:: --+...................-+%*==-::--::::::::..:-==:         -::-=.........=+         .:*#*:+......................
// ....................................#:..:-*...............:*%@@# ::++:+-=======+#-:.      .--::+-=*=...........-=:--===-*%@@=  .=.....................
// ....................................-=: ---=...........-+@#--:%#:-:== =         *       :-: ::-*-:=.............-=====.+%@+:::::+:....................
// .....................................*.: --=:.......:---=#%-++=#:+-== =    .::::      ---+=-. + .+...............=%@*:#%=-:.  .=**....................
// .....................................:+: .-:*....:++:-##*+#@@%%#..+== =---:.        :-:*-    .+.*.................-@*%=--  .==:..#-...................
// ......................................+:- :=-+.---**#+=----=:.@=*-#-:-:-           =:==   -+++*#..................-%+:-  -*=   :.#%...................
// .......................................#--=++***##. .+.:::::*%%+*:.=-+ +          -:*-  =+-  =:=..................:*-:.+#+:    - %-...................
// ...................................:---=+==*#+#: .*. *.:--=+#*:+:*:=---.+        -:*: -#:   .+**-..................=+#*---==-= = @....................
// .................................=++==+*-+*%@%.  .*#*+==-::....:-==.=== :-       +== =+   -+*=. =-..................#.-:    :#.= @....................
// .......................................=*-:-*%:.+-:.............=.*---== -:     =:# =+.-=+-. .-=+%-.................@.-      * = @....................
// .......................................*-. .-+=:=:..............+..*.+:=+ -:   .=#++%+-:. :==-  =:%=................#--      # - %....................
// .......................................++::.::..*=.............:**-*#%@*=--===----:.   .=*-    :-*+##...............*=-      #.-.#....................
// ........................................+-:. .::-*............*@*+ +@@@=             :++. :------::=+%=:............+==     .*-.:+....................
// .........................................+=::.:=#:...........=+@%**#@@@#=====++****##+:--:.-:::.    .::#:...........+==     :== =-....................
// ..........................................-=*===#:...........=*@*===--::::::::::::::::--:-+          .-.++..........++-     --= +.....................
// .............................................##@@*...........-*@:                .::::::::%.           -.=*.........:%::    +:= *.....................
// .............................................=#+=:+.........-:#@----::::::::::::::::::::. %#**********+++ =#:........%::    * = *.....................
// ..............................................#:---=........=.%@++---------------------=:#+----------==+*#%%#:.......%-.    #.-.+.....................
// ..............................................====:+:........:%*#-:                  .- ++                 -@%......:%=.    *::-=.....................
// ...............................................#.--.*........:==+=:                .-::+.-.   .:::::::::::::+@-.....-#=.   :+::=:.....................
// ...............................................==:::-+.......+-. *::.            --:.*+:-:.-==-----------------:....=@=--. =--.+......................
// ................................................#:-:-+-......+-.-%- .::::  .::::: -+#+-   -=:-:.              -+....=*%%+:...: +......................
// ................................................-+- ::#......*-=*#%%+:   ::.  :=++== =%-    .--:--:           ::+..:+-=+#@#+=+#@......................
// .................................................*:- --+.....#+%+**:--++=-====:  ==  **:+=.    .----:::.       +++#@-  :--=+++=*:.....................
// .................................................:*:.:.*:...-%%@-%% -   #.     =+.   @*  :*#=.     ::::-=-:::.:+- :++-.        +......................
// ..................................................*:- =.*...*%@@:%@: - .@    =*:    :@+    .+%%+-.       .:::+*  ::  -+***+=--+:......................
// ..................................................:%:.:.+-..@@@@:%@+   :@=.=+:      +@-       .-+#%#*+-:.   -*  -.        .:#=:.......................
// ...................................................==- -.#.-@@@@.#@%   .*=-         %@.            .:=+*#%%%% .--           +.........................
