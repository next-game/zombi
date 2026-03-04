/**
 * Created: 2024-12-03
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

class InediaInfectedAI_ActionCutClosureCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
		float actionTimeSeconds = 10;

		if (m_ActionData.m_MainItem) {
			actionTimeSeconds = GetGame().ConfigGetFloat("CfgVehicles " + m_ActionData.m_MainItem.GetType() + " InediaPain CutClosureItemActionTimeSeconds");
			if (actionTimeSeconds < 0) {
				actionTimeSeconds = 0;
			}
		}

		m_ActionData.m_ActionComponent = new CAContinuousRepeat(actionTimeSeconds);
    }
}

class InediaInfectedAI_ActionCutClosure : ActionBandageBase
{
    void InediaInfectedAI_ActionCutClosure()
    {
        m_CallbackClass     = InediaInfectedAI_ActionCutClosureCB;
        m_FullBody          = true;
        m_StanceMask        = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;

		if (!HasTarget()) {
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_STITCHUPSELF;
		} else {
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		}

// ilq2S00C8JH4hBYt4drZ2HR6ai9IcTJhckdaQTV2QUk4WTg2cEhPZ0hvd2Ntb1VOeEJwWFhXN3RrSTVYS0NZclRxL1NkQjlBc05tb2haemJFeDBtTTJ0REdULzNsOWZaUzB2SUxMaTVtOVM1anlxSzVHYWs2bTlNVkFxUzlDRCtzT2J6UEd1RVNYcmZlWjJ3UTVyczViZ3R2Y0l3ZmpVZWNKaFJyNERlZ0JoekxMQXhVQXhTc25ac0N6aUFiakZTWUd5cW5ZbGdJS2dtVTVRQS9kVFhvNHFhbUtRbjVrUTgrdnBjYlJqN0xnQ0hwbTN4VzhWN0czb0JwS016bnBhdWdOQVJya3VHTkFlazRGb3NsWHdScnUxZEhOQ01JOXZBRFBzN0R3PT0=
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

		if (!targetPlayer || !targetPlayer.IsAlive()) {
			return false;
		}
		
		if (!HasTarget()) {
			int animationId = GetGame().ConfigGetInt("CfgVehicles " + item.GetType() + " InediaPain CutClosureItemActionAnimationId");
			if (animationId > 0) {
				m_CommandUID = animationId;
			} else {
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_STITCHUPSELF;
			}
		}
		
		string actionText;
		GetGame().ConfigGetText("CfgVehicles " + item.GetType() + " InediaPain CutClosureItemActionText", actionText);
		if (actionText != "") {
			m_Text = actionText;
		} else {
			if (targetPlayer && targetPlayer.InediaInfectedAI_GetPainManager().LimbsDeepWoundSystemIsActive()) {
				string text = "";
				if (HasTarget()) {
					text = "[#STR_inediainfectedai_other_player] ";
				}
				
				m_Text = text + "#STR_inediainfectedai_remove_scratch";
			} else {
				if (!HasTarget()) {
					m_Text = "#treat_wound";
				} else {
					m_Text = "#treat_persons_wound";
				}
			}
		}

        return targetPlayer.IsBleeding();
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

            if (targetPlayer && action_data.m_MainItem) {
                ApplyBandage(action_data.m_MainItem, targetPlayer);
            }
        }
    }

    override void ApplyBandage(ItemBase item, PlayerBase player)
    {
        if (player.GetBleedingManagerServer())
            player.GetBleedingManagerServer().RemoveMostSignificantBleedingSourceEx(item);

        PluginTransmissionAgents transmissionAgents = PluginTransmissionAgents.Cast(GetPlugin(PluginTransmissionAgents));
        transmissionAgents.TransmitAgents(item, player, AGT_ITEM_TO_FLESH);

        int quantityPerUse = GetGame().ConfigGetInt("CfgVehicles " + item.GetType() + " InediaPain CutClosureItemQuantityPerUse");

        if (quantityPerUse > 0) {
            if (item.HasQuantity()) {
                item.AddQuantity(-quantityPerUse, true);
            } else {
                item.Delete();
            }
        }
    }
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::-=++++*++==-:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::-==+=:      :=+--:*+=:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::+*=::::::::--:   -=--..-+=::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::-*=-::::----.   ---: +.=-  :+-::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::-#-.         :-.    .:-+:==:::*+:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::-#.     :::...--.::.    ::.-  = #+::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#.   :-::-:::.=.:------. = =  .-*-=:::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*:  :-:-=:    =.    :-- :-=:-.::==-%:::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::+ .-=-:. .:.:=*        :-:  .+ .-: *-::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::-=.+-:::::.:=+#*#+-:      .+ .=.-   *=::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*++-:..---=+*-   .-=*+::   .-.--  .=*=::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*=+:+=:::-+=     :-==++=+=::+::---=+*:::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::++*+-*+::+=     .    :=*##**#=:**#=-#:::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#%:.-#=+:      .**++++- .  .=**---=*:::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::-++*%-:#-           -+++:+-    +=**=*%-:::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::-=++*+##+#.:=                      +.....-*:::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::-+*++*=-+==:*+:                       .*     %:::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::=+*+------.=.-= *##                :     .+   :*:::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::=**=-=---=:.-- := -%%=-               =::--:+:  .#::::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::-**----:.::: .-..=- +**% *                    :    #:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::-#--=: ::. :::.:==.-#+:#% =+            ...:-::-=  *-:::::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::-*:+. -: :--.:-=:-+#+:.:*%  *%+.         .-=:.::-*.*-::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::*=- :-:--:==+++*#+=::::*+%   *@@#-.         .:::- +-:::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::+*: -:=-=**+=-=#=:::::-+--#    -%@@@#=.          .*-::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::*= --=+*=-..-+=::::::=--::*      =%@@@@%+-:....:=+::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::+:=-+#=.:===-:::::-=-:..+*=        =%@@@@@@@@==-::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::+:+*#===-:::::::-=-:.:.-+ :==:       =*%@@%+#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::+#+-::::::::::-=-.::::++:    :==.           -#=:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::-:::::::-*#+========-:-=-=====+#=.:---:.  -==.        =++#=--:=+=======+=:::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::---=++*=#.:==::::.:=#*=.     :+#==:.:--::::++=-::---=.-%=-*=+##++++==---==-:::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::*+=:        *+=%+:.     :++*+---:......::......:+#: =: -*-:::..-==::++:::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::-+=:         .#%==+%#=-:     -*==-:=+-::::::::::::-==+= -- :+*-=+*==::=-.*+:::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::==:...        -#%:   :*%#+=:     ====-::-=-:::::::-=--=*-+ .= :=.  .+ :+..+.+=+:::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::+= ::          +%#-.     :#%#+=:    -=:===::.....:::: .==::=+: =.:-   := :+*:+ +.++:::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::-+-  -.      :. :*#: :=+-     +@%#+-    := ++=: ........:.:-=-+-=- --=    *  - +#=.= :+::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::==   =       -. =*=      :==:   :#@%*     + .*-=-.        -:=-+  :----=   .=  .- :#::- :+:::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::-*   -.   .::- .++:      .:...-+=:-%%+     *-*#  ==-.     =:-==     =+#+*++#*    =. *=:- +:::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::#.   :.:::.   =++    ....   .=*=--=%#:     #=-*   .==:  .=--+-      +.-+#=  -#-  .=. ==.-:+::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::==.::.       =++-   .::.   -***   .**.     -*:+-     :=--=--+.      =   +#-= .@@*. .-  =*:=%::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::*- .      :=+:+:  ..:.  .=#+=.+  :#+    .:-+.=+=:      -=:==       +   =+ .-+:*%**-  .. %*#%::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::=*==+======: -+. .::.  .+**=#- + -#-   --:.. +*  :==.    --.       =   +*   . -*@+-*+  . +-::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::=+::::::-=++:  :-.  .++-::*=  ++#. .-- .. -+.:+:   :--:  --      -. .#+     :::%#=-:+-  ==::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::*. :=++-:    .-:   +*::.:+=  :**. =-  . .+-   .+-     .--::=     = :#-        =:*+=:.:*--+::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::#    .    .:::   =*-:. -++:::++:.+. .  =+      .=-        -==    .-#.          -:*-=-:.:=#::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::*...  :..::.   -+=... .-*=   =-+=* .. :*:        .--        =+.    =*           .:-#.+%#=+-::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::+:   .:     -==::.. :::#=   :=+=*..  +=           .--       .==    ==:           :.=*-%*:::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::#+=:   :=*#*=-=.-::.=%%-:::* #:+. .*.             .=        =+    :-=           : -# -*:::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::+=*%*++-:      :-==*=%@+: .=.*-: -%                :=       ==     =+           - .+: #:::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::-+            .+%::+#-=--+-+=:-+:-                *       =::    =+           -  +:.#:::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::-=            +=+:::#..=++**--%: =.               +       =:*-   =-=          -  #=:=+::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::+.            **:::::*  :==*-%#*. =:              *       +=.*-=:.===.       :. :%.:--+:::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::-#----::..    -++::::::*   :==*  :: :=.            =    -==   +  .=-*=+:      . .#=-  -%:::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::#     ..::**---=++=::::-*.   -=.      :=-:        :. -==:     +     .-=**=:::--=#==*--.*-::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::*-::......:*.  -  :*-:::::*     -=.       .------=**+=.      :-==        .-=+**#%----.-==:::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::*--------:.#. .=-==#-:::::-*      :-=        :-==-.      .-==:   :---:       .-@#====:+:::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::-+        .:=*-+.  :*:::::::-*-------*   :-==-:        -==-.          :-----:  :#:     =-::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::*            -@%****:::::::::-+     .+=--.        :----.                    .:-#=:::.. -+::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::*:        .:: #+*::::::::::::::-##+=-:       .:----.                           :#:-::::=+*::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::===-.::.       ..#::::::::::::::::+.     .-----:.                                *.     -- -=:::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::==-: .+:  .::::.  +-:::::::::::::::::*.    -                .:---==+++++===-::.    %-:::::.  .#:::::::::::::::::::::::::::::::::::
// :::::::::::::::::::=   .-= =+::::::..:+:::::::::::::::::::*:    :          .---=++=++==----===+**+*+=::%.   .:::-::*::::::::::::::::::::::::::::::::::
// :::::::::::::::::::*--=   =:.+.    ::*:::::::::::::::::::::#*.   .    .-===--===:              :- .-*#+=+:::.    . =*-::::::::::::::::::::::::::::::::
// ::::::::::::::::::+:  .-.  :-.=-    :+::::::::::::::::::::::#-=-========**==-:                 -.   =+  *         .==#::::::::::::::::::::::::::::::::
// ::::::::::::::::::* .   :-   ::=+   #:::::::::::::::::::::*=*+          .+=  :-                =    ==  -=       =-.=:*--:::::::::::::::::::::::::::::
// :::::::::::::::::=: .     --    == =-::::::::::::::::::::-*--===:        :==  -:               =    =+   #     ==  =  ==:---::::::::::::::::::::::::::
// :::::::::::::::::*  :      .=    *=+:::::::::::::::::::::*    :-=+-:      -=-  =.              =    =*  +*: .--  .=    *=.::--::::::::::::::::::::::::
// :::::::::::::::::*--=----:.  =.   #::::::::::::::::::::=*=--:    :-==-.    =+.  =              =   ++-.++#%=:   --    --%::::--:::::::::::::::::::::::
// :::::::::::::::::+        .:. +. :*::::::::::::::::::::*    .----.  :-+-:   =+  =.             =  ===:-**=-#:::-   :--.:=*::::=:::::::::::::::::::::::
// ::::::::::::::::-=          :-.*.*:::::::::::::::::::-*-.        :---: :-+-:.+=  +             - -=--**-.-::*-:--:. .::.-=#+::=:::::::::::::::::::::::
// ::::::::::::::::-=           :+-#:::::::::::::::::::++--:---         .---:-=*++. --            = **++: -:  :-*.  .--.  ...-#::=:::::::::::::::::::::::
// ::::::::::::::::-=            .#=:::::::::::::::::-#.   :--===-.         .-=----  +            =.*+. -- .-:.=-*::.        :#:-::::::::::::::::::::::::
// ::::::::::::::::=-             *:::::::::::::::::-#:---.  ..:--+--:         .--+:  =          :*=. --     =-  .+           *:=::::::::::::::::::::::::

class InediaInfectedAI_ActionCutClosureSelf : InediaInfectedAI_ActionCutClosure
{
    override bool HasTarget()
    {
        return false;
    }
}

class InediaInfectedAI_ActionCutClosureTarget : InediaInfectedAI_ActionCutClosure
{
    override bool HasTarget()
    {
        return true;
    }
}


// Change vanilla cut closure actions texts
modded class ActionBandageTarget : ActionBandageBase
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (target) {
			PlayerBase otherPlayer = PlayerBase.Cast(target.GetObject());
			if (otherPlayer && otherPlayer.InediaInfectedAI_GetPainManager().LimbsDeepWoundSystemIsActive()) {
				m_Text = "[#STR_inediainfectedai_other_player] #STR_inediainfectedai_remove_scratch";
			} else {
				m_Text = "#treat_persons_wound";
			}
		}

		return super.ActionCondition(player, target, item);
	}
}

modded class ActionBandageSelf : ActionBandageBase
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (player && player.InediaInfectedAI_GetPainManager().LimbsDeepWoundSystemIsActive()) {
			m_Text = "#STR_inediainfectedai_remove_scratch";
		} else {
			m_Text = "#treat_wound";
		}

		return super.ActionCondition(player, target, item);
	}
}


modded class ActionSewTarget : ActionBandageBase
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (target) {
			PlayerBase otherPlayer = PlayerBase.Cast(target.GetObject());
			if (otherPlayer && otherPlayer.InediaInfectedAI_GetPainManager().LimbsDeepWoundSystemIsActive()) {
				m_Text = "[#STR_inediainfectedai_other_player] #STR_inediainfectedai_remove_scratch";
			} else {
				m_Text = "#sew_targets_cuts";
			}
		}

		return super.ActionCondition(player, target, item);
	}
}

modded class ActionSewSelf : ActionBandageBase
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (player && player.InediaInfectedAI_GetPainManager().LimbsDeepWoundSystemIsActive()) {
			m_Text = "#STR_inediainfectedai_remove_scratch";
		} else {
			m_Text = "#sew_cuts";
		}

		return super.ActionCondition(player, target, item);
	}
}
