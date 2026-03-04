/**
 * Created: 2024-12-03
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

class InediaInfectedAI_ActionDeepWoundUnbandageEffectiveCB : ActionContinuousBaseCB
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

class InediaInfectedAI_ActionDeepWoundUnbandageEffective : ActionContinuousBase
{
    string m_InediaInfectedAI_PainLimb = "head";

    void InediaInfectedAI_ActionDeepWoundUnbandageEffective()
    {
        m_CallbackClass     = InediaInfectedAI_ActionDeepWoundUnbandageEffectiveCB;
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
		m_Text = text + " #STR_inediainfectedai_unbandage_wound";

        return targetPlayer.InediaInfectedAI_GetPainManager().IsLimbDeepWounded(m_InediaInfectedAI_PainLimb) && targetPlayer.InediaInfectedAI_GetPainManager().IsLimbDeepWoundBandaged(m_InediaInfectedAI_PainLimb) && targetPlayer.InediaInfectedAI_GetPainManager().IsLimbDeepWoundBandageEffective(m_InediaInfectedAI_PainLimb);
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


class InediaInfectedAI_ActionDeepWoundUnbandageEffectiveSelf : InediaInfectedAI_ActionDeepWoundUnbandageEffective
{
    override bool HasTarget()
    {
        return false;
    }
}

class InediaInfectedAI_ActionDeepWoundUnbandageEffectiveTarget : InediaInfectedAI_ActionDeepWoundUnbandageEffective
{
    override bool HasTarget()
    {
        return true;
    }
}


class InediaInfectedAI_ActionDeepWoundUnbandageEffectiveHeadSelf: InediaInfectedAI_ActionDeepWoundUnbandageEffectiveSelf
{
// 3dKwfjFKgrAAJ5RqRdqP6W11UndtaG03SEpuMXFVc0FsQVBIRHhzWE93aCtQc3RKRGZZRC8wZHlscGtZNFZXTjFkNG5BdWxEME1yOVp1NGVGald1VU91S3o4akZVTUQ4aWFjUmU5SzhoZ0RyNTIxb2ZnYUVlMXZnalFWOTZXUXBnNXl0T2k5cTF6eWZxS0NWam9rby9jb2NmQUdzQ3I3Ymh0cUN0NHVBT244SXZicmlRZU53ellCcHFESVIzTXFOWTdFc1ZqMG5BSXZjdkY4cEJ4V0hLZ1VZOWQxMXdYbWtkWXdubFNwNWpuRXY5STFYdXl6RUxUR3VIenJ0TjhkY3EvMHNFUGxlbUM3aFdPYnBEOE1GNVYyeXpSSjhHK3p6b05yb293PT0=

    void InediaInfectedAI_ActionDeepWoundUnbandageEffectiveHeadSelf()
    {
        m_InediaInfectedAI_PainLimb = "head";
    }
}
class InediaInfectedAI_ActionDeepWoundUnbandageEffectiveHeadTarget: InediaInfectedAI_ActionDeepWoundUnbandageEffectiveTarget
{
    void InediaInfectedAI_ActionDeepWoundUnbandageEffectiveHeadTarget()
    {
        m_InediaInfectedAI_PainLimb = "head";
    }
}

class InediaInfectedAI_ActionDeepWoundUnbandageEffectiveArmsSelf: InediaInfectedAI_ActionDeepWoundUnbandageEffectiveSelf
{
    void InediaInfectedAI_ActionDeepWoundUnbandageEffectiveArmsSelf()
    {
        m_InediaInfectedAI_PainLimb = "arms";
    }
}
class InediaInfectedAI_ActionDeepWoundUnbandageEffectiveArmsTarget: InediaInfectedAI_ActionDeepWoundUnbandageEffectiveTarget
{
    void InediaInfectedAI_ActionDeepWoundUnbandageEffectiveArmsTarget()
    {
        m_InediaInfectedAI_PainLimb = "arms";
    }
}

class InediaInfectedAI_ActionDeepWoundUnbandageEffectiveLegsSelf: InediaInfectedAI_ActionDeepWoundUnbandageEffectiveSelf
{
    void InediaInfectedAI_ActionDeepWoundUnbandageEffectiveLegsSelf()
    {
        m_InediaInfectedAI_PainLimb = "legs";
    }
}
class InediaInfectedAI_ActionDeepWoundUnbandageEffectiveLegsTarget: InediaInfectedAI_ActionDeepWoundUnbandageEffectiveTarget
{
    void InediaInfectedAI_ActionDeepWoundUnbandageEffectiveLegsTarget()
    {
        m_InediaInfectedAI_PainLimb = "legs";
    }
}

class InediaInfectedAI_ActionDeepWoundUnbandageEffectiveTorsoSelf: InediaInfectedAI_ActionDeepWoundUnbandageEffectiveSelf
{
    void InediaInfectedAI_ActionDeepWoundUnbandageEffectiveTorsoSelf()
    {
        m_InediaInfectedAI_PainLimb = "torso";
    }
}
class InediaInfectedAI_ActionDeepWoundUnbandageEffectiveTorsoTarget: InediaInfectedAI_ActionDeepWoundUnbandageEffectiveTarget
{
    void InediaInfectedAI_ActionDeepWoundUnbandageEffectiveTorsoTarget()
    {
        m_InediaInfectedAI_PainLimb = "torso";
    }
}

//                                                                                                                          +*+-
//                                                                        ..   ..                                           *+++
//                                                                   .-+####%%#%%%#*=:                                      #==*
//                                                                .-*#*+*##***#%%%##%%*-                                    %--#
//                                                              .=##++#%#++++*#%%*+++=*%*.                                 .%--%
//                                                             :##++#**++++++=+#%#*###+*##.                                .%::%
//                                                            =%*=*#++++**+++=+=*%#***%=#%#.                               :%::%.
//                                                          .+%*=**+*+#******#+%+%#+*%**:%##.                              :#::#:
//                                                         :##*++**###*#*+===--:-=+#=+%+-:*##.                             -#::#:
//                                                       :*%**=*###%%%#.           .#+=##=:+%#:                            =#:.#-
//                                                      +%%+*=###%%%%#---------------##+*#*=+*%=.                          =#:.#-
//                                                     =%#+=+#%*#%%%*.                =%%#*#+=*##-                         +*-.#=
//                                                    :%%+=#*##%%%%=                   *%#%#****%%+.                       **-.#=
//                                                    *#*####*+*+#: :-=+=:.    .-=+=-. *%:%%%###*#%=                       *+-.*=
//                                                   -%%#+=-=+%%=+. :++*##+.  -++*##*- %--%%%%%##*%*                       #+:.*=
//                                                 .+%*=-+#*=#%%++-  .--:--=  :-.:::. =*-#%%%*%*#%%#                       %+:.*=
//                                               .-%*+*##%--%#+#%%#:       =         ===%%%%%=#-#%%*                       %=-.*=
//                                               .:-*###+=+=-=#%%%%==.    --  .    :=.:%%%%%%=+-#%%=                      .%=-.*+
//                                               .+%#*+==-=#%%%%%%%=.=.   .:-.:   .:  =%%%%%*--#%%%:                      .%=:.++
//                                            .=*%*=--=*%%##**#%%%%%: -.  .:-::.     :%%%%#+==#%%%#                       .%-:.++
//                                          :*%*--+##**++=+*#%%%%%%%%=.: :=::::=:  .+%##***##*%%%%-.:.                    :%-:.+*
//                                         =##--==#+===+=#%%#%%%%%%%%%%=. :--::. .=#=#+%%%%%%*#%%#**=+*+:                 :%-:.+*.
//                                        -*=:=-=*-#-+-*%%%#+%%%%%%%%%%**+:    .+*++*+-*%%%%%%%%%+=-:::-+*=.              -%-:.=*.
//                                        +.+ =+=:#+.+=##%%**%%%%%%%%%%+-+******===-:.==#%%%%%*=-:.    .::-**-.           -%+:-+#
//                                        =.*.=#..*#.=++##%#%%%%%%%%%%%:-:::::--:.     -#%%%*--. :.       .::=*-.         =%*:#-#
//                                        .:-*:**::++:+*.-+%%%%%%%%%%%%.-:    =:     .:-*%*=-. ::           :-=##-        =%+:#:%
//                                         ..:#++%%#%#+#*==*%#==+%%%#*%.-:    :-..:-=+#%#=-. --        ----------+*:      =%+.%.%
//                                         .=+-*%%%%%%%%%%%+:.*%%%*=.****+=====++--==#%=-. .=.      :=+:.---. .. --+=.    =%+.% %.
//                                        .+.+%%%%%%%%%%#=. =%%%*-*.=*#=       ::-+=*%:   .:      :--.:-:       ...--*:   +%+ # %.
//                  .                     -.*%%%%%%%%#+:  :#%##-.:--*=.      :--==::#*    .      ... .            ..:-+=  +%+.# %.
//                  ..                    .=%%%%%%%*-    -%%+*.   :::-----=+=-::    +#:                            .. =++.+%==+ %.
//                   ...                   #%%%%%+:     +%%*=    .:=-::::..        ::*%*=-::::---=====------------::.--*+**%=-+ %.
//                     .:                  %%%%=.      +%%*:  .--:.            .:-=---:=%##**++++++****#####***++=-==++==%%%* + %:.
//                       ::               :%%%=:     .*%%#:.-=:.           .-=+=-:  :::.=:=*%%%%%%%%%#*-:-*+==-::.  .-*#*+%%* + *++...
//                        .:-.           :*%%*-     .*%%%++-.           :=*+-:    ..  :-=:+*%%%%%%%%%#=+-  -*==++**#*+-.::-%#.+.=%+  ..:::.
//                           :-::.       .-%%=.    :#%#+-:.         .-+=:         .:::-=+ *-%%%%%%%%%%%+-+-. -+-..:..:-*###%* ==+%+      ..:.....
//                             .:::::--=+#%%+:    +%*-..         .-+*-          :::---. - =:#%%%%%%%%%%%#::=.:.:+=-=**+=--=%# ---%+             ..::.
//                                .::..:--+#-   .**:        .  ::--:         .::--:      - +-%%%%%%%%%%%%%+.-+==+*+-..:=*=+%* -=-%+                 :..
//                                   .::. ++.  :%=      .::...:--.         .:..+.        -:--+%%%%%%%%%%%%%#=*+=:..:=+=-  -%* -=-%+                   .
//                                      .-%*-..%+     ::..::.--           :- .+.          -:=-:=*#%%%%%%%%%%#+:---++-     -%* -=-%+
//                                        :-=*%%    .: .:. --             =  +.            -:=-  :-+*##*#%%%%%***=.      .=%* -=-%=
//                                          -+#*         :=.             -. :-              --+      ..=%%%%%%%%*.      :=#%* -=-#.
//                                         ==-=%..      ==               -  =               *+:       :%%%%%%%%%%%=  :+#+:=%* -+-##-
//                                       .+*=: #+=.    --                :  -              *= :-=-...-%%%%%%%%%%%%%###=.  =%* -+-%%**.
//                                    -+#%# =- :%+*:   -                    .            -=--===-===#%%%%%%%%%%%%%%%%#:  .=%* -%#*: :+.
//                                 .-*#-%%-  :=.:##%- .                              .-======-.   .*%=*%%%%%%%%%%%%%%%%=  =%* -%=.   .+:
//                              .-*#+::=*%#    :--*%%+.                      ..::-==**=-===:      +#:  .+%%%%%%%%%%%%%%%*.=%+ =%:    -=+=
//                            :+#*-.   *:%%*.     .-+%%%###===============+*=*+++=++=---.       .#*.     .=%%%%%%%%%%%%%%#*%+ =%:..:--. #*.=:
//                          :**=:.     =::#%%=:::::-+%%%*-.--.            = =.:-:.             :%+.        .*%%%%%%%%%%%%%%%+ =*=.-:. :*%*+=%*.
//                      .-+*=-:        .+  :+##***#%%=.+%=-. :=-.        .= * =               -%-           .%%%%+-%%%%%%**%+ -++:. :*%#+:::+%+
//                    -**-::.           =.    ..:=%*:  =% .-=- .-=-.      = * =              :#:           .*%%*=..%%%%+: +%= -:#-=##=.  :+= *#
//                 .=*=...               +    .=#*-    =#    .-=: .---.   = +.-.      ...    #:         .:=##+:   =%%+:   +%= -:*%+:  :==: = *#
//                =#=  .                 :=-+##=:      .#+      .=-:  ---:= -:.=        :---=+          .:..    .=#=.     +%= -:*% :=*+-.  = *#
//              .=*=:--=-           .:-+***=-.          =%*:       :==:  :---+.=    .:::----*+                 .-:        +%= -:#%*=+: .   = **.
//             .*#:==-: .=-   .:-+*#*+-:.               #%%=          .-=-.  .-=**=-:...    :%*:                          +%= -:#%.==    .:+ #*.
//             -%:  .::=: -**##*=-.                     =%-==-.          .-==:   :==-.    ::-*#%#-.                       +%= -:#% ==      + %+
//            :#*=+++++=++**-.                          .%#:  :---.          :---:. .:------::-#%%#=.                     +%= -:#% =-      = %=
//           :%#+=-----*%+.                             .%+.-==:  :---.          .-===-:.   ....=%%%%+:                   +%= -:#%.*.      =.%-
//          -=:        #=                              .*%: .--:-=-: .--=-.           .:-=====+==+%%%%%*-                 +%= -:#%.+       =:%:
//       .==.         =+                              :*##=::      :---: .:----.              -..:+=*%%%%#=.              +%= -:#%:+       -=%.
//       == .:.:-.    *.                             :#+==*=.          :----: .:-----.  .:------:. .:=%%%%%%=.            +%= -.##--       =#+
//      +-.  :+%%#.  :=                             .###. .===-.            .---=-::-=--:..      .-=**..=#%%%%+:          *%= -=+#=        =*-
//      .+=+.-*++*: :%:                            -%+ .==-.::--===:..::------::..::::....::-=+**+=:.+.   -*%%%%*-        +%= -:+#::::--:. ==-
//       -+*..-#=. -%#+                             -%*:  :+=. :::.:::...::::::::::-=+*###*=-:.      :*.    :+%%%%#=.     +%= --#+:::::.:-=++=
//       .+.==*=. :%-:#                             .***+.  .+*:  ::-:::. .-=+*##*+=-:--:::::--::.    -+      .=#%%%%+.   +%= -*%*++==-:.::*=.
//       .===+#--=**-=-                           ==-- .-*+.  .=*=.    :+*+=-:.  .::::.       .::---:::*:       .-#%%%%*- *%+ *%*--==+**##*#.
//           .-=+--+:                            .#+:===++#%+.   -#+-=*+.    .---.         .::.    .::..:---.      :*%%%%##%= *%-         .#
//                                              =#-.=.     :=#*-   :=-.   :=-:           ::.  :+*#%%%%%%%#=.+-       :*%+=#%+ *%-         .*
//                                              #*::.=.       :+#+:   :===:            .:.:.-#%%*=-:...:=#%#=====      :  *%* *%-        .*:.:.
//                                             -%#--#.+          :=+++=.              .:.:.+%#-           -#%#- +-  =-:.  **-##%=::::---=##*###
//                                           .+%=.  =+-:      .                      .:.-:#%+               :#%=-#- %#+=-:=*=-=##########***#:.
//                                          -#+.    :%.+      :.        .            : :=%%*                  *%*#%*%%#+.:=+-*--=====+=--+=*=
//                                        .=#:      -%:+      -.        =           -..+%#.:+                 .:#%%%%%#-+-.+ #  .:    ...=+*
//                                       .**.       -%:+      --        *          :: *%+.  *.                  -+%%%%*#::+*+#=.       ::=#:
//                                      -#=         =%.+      :+       :*          - *%-    ::                   *=%%%=-%+.:%=  =: .:  :=*=
