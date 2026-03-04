/**
 * Created: 2023-09-01
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

modded class House : BuildingBase
{
    ref array<int> m_InediaInfectedAI_DestroyedDoors = {};
	
	void House() {}

    void InediaInfectedAI_DestroyDoor(int index)
    {
        m_InediaInfectedAI_DestroyedDoors.Insert(index);
    }

    bool InediaInfectedAI_DoorDestroyed(int index)
    {
        if (m_InediaInfectedAI_DestroyedDoors.Find(index) > -1) {
            return true;
        }

        return false;
	}
	
	override bool CanDoorBeClosed(int doorIndex)
	{
#ifdef SERVER
		if (InediaInfectedAI_DoorDestroyed(doorIndex)) {
			return false;
		}
#endif
		
		return super.CanDoorBeClosed(doorIndex);
	}
	
	void InediaInfectedAI_SendPlayerInfoIfDoorDestroyedServer(PlayerBase player, int doorIndex)
	{
#ifdef SERVER
		if (!player) {
			return;
		}
		
		if (InediaInfectedAI_DoorDestroyed(doorIndex)) {
			player.InediaInfectedAI_SendDoorDestroyedInfoServer();
		}
#endif
	}
	
	bool InediaInfectedAI_OpenDoor(int index, PlayerBase player, bool quietly = false)
    {
#ifdef SERVER
		if (!CanDoorBeOpened(index, true)) {
			return false;
		}
		
		if (!quietly && player.InediaInfectedAI_QuietDoorOpeningMechanicAvailable(this)) {
			InediaInfectedAI_IrritationActions.OpenDoorIrritation(player);
		}

		OpenDoor(index);
#endif
		
		return true;
    }
	
	
    bool InediaInfectedAI_CloseDoor(int index, PlayerBase player, bool quietly = false)
    {
#ifdef SERVER
		InediaInfectedAI_SendPlayerInfoIfDoorDestroyedServer(player, index);
		
		if (!CanDoorBeClosed(index)) {
			return false;
		}
		
		if (!quietly && player.InediaInfectedAI_QuietDoorClosingMechanicAvailable(this)) {
			InediaInfectedAI_IrritationActions.CloseDoorIrritation(player);
		}

		CloseDoor(index);
#endif
		
		return true;
    }
}

//                                                                                                                                       -**= == ::=-
//                                                                                                                                      :#-= :#.:: =
//                                                                      .:-==+====-:.                                                 .=+++ :#.:: ::
//       .                                                        .-=+*%*=+==++*=--++##+-.                                            =+-* :#:::.:.
//       ==                                                 .-=++#%%##**::=+##%#++++++*+*#*=.                                        =**+-.#:::--
//       +%.                              :.            :=+#%%%#*==++**=*:%*+=::.::. :==-::=#=.                                     =#%+=:%-:.--
//       +#=                             :.          :+#%%*====+**=+#=#*#*=:::.    .::.  :=++**.                                   -#%++:%-::-:
//       +*+                                      :=##++**+++=---=*==#*#*=+++--+=-:   .---. .=#+                                  :*%#*:#=.:=-
//       +*=-                                  .=*#*%#*=-==++*+====**#***++=-:-=-::=-:    :-: .**.                               .+%##:#+.+:=
//       ==-*                                .++-*%*:.:---:::.#+*==%#*##+=-=*%*--=--.:=:    .-: -#:                             .+%##:#+.+.=
//       =-=:-:                            .++.-#*=*#%*+++=+*#%###%*--::.::::-#%#--=-- .=:    .:.:#-                            =%##.#* = -.
//       =-+ +=.                         .+=:=#%+*%***%%#==-*+-..   ::=-..::--:-+##:===- .--    ..:#=                          =%##:##:=.-.
//       -==.:=+                        ==.+%**+#++%%%%%+             .-*+=-::::-=%%+.+==-.:=:     :*=                        =#%#:##----.
//       .*-= +*.                     .+:+*+#=%=-#%%%%%%=                .-+##+--+%%%*.++.===-.      =+:                     -#%#:*%-:+-:
//        #.# :*=                    .=+++:+*%++=%%%%%%%:                    =*###%%%%#:++=. :-====-:..--:.                 -###:*%+.=:-
//        #.#: ##.                   +*-+=*#+%--#%#%%%%+.          .:-::.      :=**+*%%%-*+===-.   .:======::::            :*##:*%*.=--
//        +.== -%-            .     +==#**: *++-%%%%%%%=       .-+#%*=-----:      =#%%%%%-+=+-:-====-:   .:=+- -.         :+%*-+%* =--
//        -:-+  +#             ::..=:=*=-::+%#%+*%%%%%##%#=   =%%+:::.....         -#%#%%%+**+*+==-:::-----. :*+:        :*%*-=%* -=-
//        .---:  *:         .     .:++::+**%%%%%-%%%%%%*#%%#.   .**#*+=#+.          *+=##%%#++*+=:-=++===:.-++:-*.      :*%+==%%.:=-
//        :-.:=  -=         ..    =:#.-%##%*%%%%=#%%%%%%%#%#=     =---:              :*%#%%%%#+=-===--=--==+:.=*=*     .+%*+-%%-.:-
//        -- :+   #         .... ..:+ **=#%%%%%%%+%%%%%%+. +:                         =*.%%%%%#%%%#***+==-=-=+:.**-   .+%**:%%=.:-
//        =: :+   +:         .....  +=:*%##%%*%%#+%%%%%%:  =   .                      #.. .:%%%*+%#*%%%%%%*+==*+=*#  .+%+#-%%* --.
//        =. :=:  :+            -:  *=+***%*+%%%#%%%%%%%- +   :::-                   :-=::+%%%%%%++**#***##*%%*%+#%.:*%%%-#%%.-+.
//        =: .-=   *          :. .-==*=#**---#=#%%%%%%%%%.+%=*++:-                   :.#+##%%%%%%%%*++**+--+*#%%#%%+%%%%**@%:=+.
//        --  :*   =-          ::::=+###+:.  **%%%%%%%%%%**%#+.                     - :=  :+#%%%%%#%%%%###%****@%#%%%%%#+%%-++.
//        .+  :#   .*           ...:-*+#+==+**==*#%%%%%%%%%%+*--::.               ..  +.  .#:%%%%%%##%%%%%%%%#*+%%%%%%%#%%*+#.
//         *  :*.   *.              -+-+....  ::+:#%%%%%%%%%%##**===-:              -=    -# -%%%%%%#*#%%####%%#%%%*=-::::::-+++:
//         +. .=:   :+             .:=:+-      ::-.*%%%%%%%%%+-:---.             .==+.    *=  =%%%%%%%#*%%%%%%%#=..-----::::-:-:=*=
//         -=  ==    #.            . =:-*---:.  .::.-+%%%%%%%%*+==-          .:+#+:+.    -#.:-=##**##%%%%%%%%*:.--- .:=*#####*=-=:=#:
//         .*  .+    =-            . .+. ----==-:.:-..:=##%%%%+          .-+#%%*: +.  .-+#==---===+#%%%%%%%+.:--.-+**++===+*#%%%*=-:++.
//          *.  *    .*             . .++: .....   :==-::--+*%%*:. .:-+*%%%%%#:  :==+*=:  .::::-==++*%%%%*::=:-+=----------::.-*%%%+:-*-
//          =:  +    -+              ..:+=++=-:..    ..:=+*#%%%%%%%%%%%%%%%%%+.=+*+-:.  .--=++=---=*%%%%- -:-=:--:           .. .+%%#---+=:
//          --  =: :- =                 :===++*#%%%%%%%%%%###*%%%%%%%%%%%%%%%#*+--:..:-+=::.  .:-==+#%*.:-:---:                   .*%%::::-=+:
//          --  -- +  =                 .::.:=+#########*+***%%%%%%%%%%%%%%#++=-:.:-+-:  .=+****###%%:.=--==.     .----:::::::::::::+*%*=.:=:+*
//          :=  .= =: =                 .: .::..::----===*%#*-.  ..+%%%%%%+**=..-=-:  .-**=++=**+*%%.:=-++.    :--:      ........    -=+%%=.=:=+
//           *   + .+ -.                 .:.  ..:::-+++++-:        .#%%%%%#=.::::..:=+++-=++++*-#%%::=-*:     +:                     .=-=%%*:*.#-
//           +.  *  + ::                    .:::----::.       :-=+++#%-==- .: .--=+=-+*#%=:.=*:#+=* + =-     .%                       -: -*%*-=:%.
//           --  +. --.-                                  .=*%%+=+#%%%*===+=++==+**%#+*#*+*#%:** *# + =-:    :*                        .-  %%:* *=
//           .*  =:  * =                               .=#%%*+-*%%%%%%%%%*++*#%%#+==****=--.*:%- %%#+=---::---+====-:.                   - -%*-=:#
//            *  :=  -:=                             .=#%*=--+%%%%#:%%%%%%%%%#*+=+++=-::--+-*-%: #%%%%#*+++=--======--=====:.             - *%.+ #:
//            =-  *  :.+                             =*+::--#%##+##=.+%%#*****=--------.  -=*=+- =+*%%%%%%*====+=-:-=-----::-===-:.       ..:@%*-+=
//            .*  *  ::+                            :+::==-%#*+**:    :=+++=--::::.       :**-:+  =  :=+#%%%%%%*++=--==-::-:----:.:--::.::-=+#*#*:*
//             *. =: .:+                           +=:==.+%#+*#*                          .##+- =..+     #*-=*%%%%%%%*+=-:--:--:.::::. ::-=+. :+*.%
//             =-::=  -+.                        .+=--: *%#+#:+*:                  ..:   :--:%+=.-:.=    ++    #%%*%%%%%%%*++--:---:        :-:+++#.
//             .+: +  -=:                       .+=-:: *####  .*=- .:--+#+=---=+*#****==+=+*%%%*=-:-:=   -+.   .##%==+*#%%%%%%###+--::.       =.*%*.
//              *:.=  --=                       *=--- *%%%==+  .=++=-=*+=-:-++==:..::----*: .*%%%+==-:+.  -+    -*#%#-::-=+=*#%%%%%%#+=-.   ..::*%*:
//              +.::- --+                      ==-=- +%%*- -:-    -*+-.:--=-:-----:..  ..-#=  :%%%#=*+ --  -*:   %=*@%%+=-:=-===*#%%%%#*+=--.:=*.*#:
//              --: + -:+                     .#::-.*%%%%:= +=  =*=:::-++==:.              *=  =%*%%*-=-.=:  -=: *=+@+=#%%#**=-==-=*#%%%%%*+=+=--*#.
//              :+: + -.*                     +=.***=-.*%== =-=+-::-+*+:     :::::::.      .%.  #*=%%%*--::=:  ::#:=#:::.:=*%%%%#*=-==+*%%%**#*==*=
//              .* .=.- #                     #+*=...  =%+= -*-..-+*-  .::::.        ....   =*  =%:-%%%%*=-::-: :-:*+:......:-=+#%%%#*+==+##=
//               * .:=: #                   -##-..     +@-= +- .=+- .::.                    .%. -@- .*%%%%%+-:.:--**.:::...  :::=+++*%%%#+==#=
//               +.: *:.#                 .**+         *% =:* .=+..:.      .     ...         %: +%..  -#%%%%%*+-+#*--::  :--=-:: .:--.:+%%#+*%:
//               =-: +..#                :#=+  .       %#.=+. ++...       :    .:            %..%*  -   -#%%%%%%%*-.  :---::  :--.:-++++*#%%*#+
//               :+:.-:.#               .#-#.   ..     %*:-* ==- .       -    :             -* +%-   =-   :#%%%%:  .---:.  :-==+++-:      .+%%%.
//                # -.=:#.              -#-*           ##:=* *=..       -.   :             .+ -%+     .=----=%%%%-::--  .-==*+:             :#@:
//                * - =:#.              =*=+           +%:*+:+=:        +    .            .+.=#*            .*%%%%+:  .==+#-             ..   +=
//                =:- ==*.              -%-*           :@**=*:*=        +                -=:*#+   ...      .#:+%%%%#.=*#%+       ... .:----==++*#.
//                --- -+#.               +%#=:    .:.   *%#%#+===.      +              -+-**#-.----::===.  -= -%%%%%%%%%--+    ..:---+++***+===*#:
//                .+- :**                 -%#+=.    .:-=-*%%%%*==::.    :=           :+**+*#=-:         -:.-=  %%%%%%%%.   =..--=++*+=-.       .:#.
//                 +:::#:                  .*#+===:     :-=%%%%%=+=:     -=       .-*#+=+#+:  .-===*+::.  :=%: :%%%%%%%%-  -*++*+-....           :#.
//                 +.=-#-:+-                 :+#*=-----=++=-#-= =+:-+++-:..-::--+*+-.:+*-. .=+=.    :==-::. :#= :#%%%%%%%#**#+:  ..             . :#.
//               :=+==-::##*                    :*%%#*+===+*--%+..==:..-=*##*+=-. .===:.:=+-.          :=-:--:-#*+%%%%%%%%%+:+. .          :-::.   :*.
//             -=-:..   :+#%+=:                 :%%%%%#%%-:- :# -:  .::::.     :==-..:==:     .::---------*-.:==+%%%%%%#:+#-+.+:        :--.   .-=--=+--:
//           -=:..:-:   -*.:=:*-                 *%%%*.*-=- .-=- =..         -=:.. =+:    ..:::.   .....  .-%%#+%%%%%%#:  =#:+.+.    .==. .:=+=+++#%%%%%+
//          :*::==:==+==%:  .+=*                 *%%%%+=:  . +.* :: ......  --:  .*-    ..  .-====--:::--:-+*%%%%%%%%+.    -#:*.+  -*=:=*##+=+%%%%%#=--%-
//          *--. ...    =%++*: *.                *%==%%+*  . * +  =      : -:.  :*.      .==-.   :-----=*%%%%%%#%%%#-       -#-+=##%#%%%#*+**+=-:.:-:  -#
//          *%.    .::::%##%*  --                *% =*%%+- . * =. +      :.-:  .*      .+=      ..:--=#%%%%%%%*=%*-          =%*%#%%##+#-. .....:-   :  *
//          *=  .:--:+.:-+**#  :+                *%.+:#%++ . * -: *     ..-..  *.     =+     :===-:..  .:+%%%%-=.             +%%%#*#%++:        =:  .*..
//          *=--:   :-+   :%#:-#+.               =%=+.+%+#  .* :- *     . -:  -+     +:   .=+-   :-====*%%%%%==.             -##++**=-====+::----.   +:
//          =*: .:---:+==+###=##*=-::.           :%*=.:%**: :=:.= *    . ..-  *:   .+.   ++.  .++: :=++#%%%%=-.            -##*%*-.   :--+-++       =: .+
//           :+-  +.:-+  .%=%%%+: :--::::::.      #%:+ %#:* .:= + *      -.:  #    +.  .*:   ==  -=-:.*%%%#--.           .*%%#=.    ..:.::-+-*:    =-:+%%
//            .###+=+-*===#%%%#+.... :=-: .:---==-=%+--*%.== :+ + *      =:.  +   -.  :*.  .+.  -..:=%%%%%*-.           -%%#-  ..      .- -:+=+-  +##%%#-
//              *%%=:   .-%%%###%#=..:. .----:. =%%%%-=#%+ +::-:-:+     .:-  :-  .-   #.   +  .: :=#%#*+=-.           .*%#: ..         --  :-=++=*%%%*-
//               =##++===*%%%%%%%%%%#=::...  .::-%%%%%:#%% :+:-=.==:    = -  -.  =   -=   +: ::.++%#=*               :%%-..        :--:    :* #:%%%*:
//                -*.-   .##%%%%%%%%%%%%*=:.    =%%%%%%+#%..*.-= +-:   :: -  =   =   +=:-=*--=---+:-%%=            .+%+..    .:::::.  .::--:+:#=%+.

modded class ActionOpenDoors: ActionInteractBase
{
	override void OnEndServer(ActionData action_data)
	{
		if (action_data.m_Player && action_data.m_Player.m_InediaInfectedAI_ConfigPlayers && action_data.m_Target.GetObject() && action_data.m_Target.GetObject().IsInherited(House)) {
			House house = House.Cast(action_data.m_Target.GetObject());
			if (house && action_data.m_Player.InediaInfectedAI_QuietDoorOpeningMechanicAvailable(house)) {
				InediaInfectedAI_IrritationActions.OpenDoorIrritation(action_data.m_Player);
			}
		}

		if (!action_data.m_Player || !action_data.m_Player.m_InediaInfectedAI_ConfigPlayers || !action_data.m_Player.m_InediaInfectedAI_ConfigPlayers.DisableVanillaDoorNoiseSystem) {
			super.OnEndServer(action_data);
		}
	}
}

modded class ActionCloseDoors: ActionInteractBase
{
// TTPZmUeyZgxuisxUxFE3PzQ1UTN1ZW5qYjdJK0N2RUdmTk52RUJYZmx5ZkprWmJlclAyNWZNc01MUEhpL3A5SkN0SFNCZ1U0NVRIWWpHSmhKcVBlVWk2RWVlYmgzZDZUUWdMMW43aVVhZXpWcEx1L1RucUovYkgrRDFwMXBVRkdsWDBaNi94TmdaTkVCWlZtSVNoVlUxWmw3U2gxVkJUcGp1Zm1TQksrVmlqZFBKbE1zMHkvT0s1MWVxMWM5QkgwcmZtbkFqWis4RGRMVVFKaDJUcEpOWTllSkpkR0F2b3g0UUpyZ1hlVmMzbzdRekpadlppSHlGMDhtUld2K1dzTXhjYkhFN1FRK1RGN3ZsajA2Y2JNYnIrSXdYUysyamhpSmtPckJRPT0=
	
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
#ifdef SERVER
		if (target && IsBuilding(target)) {
			House house = House.Cast(target.GetObject());
			if (house) {
				int doorIndex = house.GetDoorIndex(target.GetComponentIndex());
				if (doorIndex != -1 && IsInReach(player, target, UAMaxDistances.DEFAULT)) {
					house.InediaInfectedAI_SendPlayerInfoIfDoorDestroyedServer(player, doorIndex);
				}
			}
		}
#endif
		
		return super.ActionCondition(player, target, item);
	}
	
	override void OnEndServer(ActionData action_data)
	{
		if (action_data.m_Player && action_data.m_Player.m_InediaInfectedAI_ConfigPlayers && action_data.m_Target.GetObject() && action_data.m_Target.GetObject().IsInherited(House)) {
			House house = House.Cast(action_data.m_Target.GetObject());
			if (house && action_data.m_Player.InediaInfectedAI_QuietDoorClosingMechanicAvailable(house)) {
				InediaInfectedAI_IrritationActions.CloseDoorIrritation(action_data.m_Player);
			}
		}

		if (!action_data.m_Player || !action_data.m_Player.m_InediaInfectedAI_ConfigPlayers || !action_data.m_Player.m_InediaInfectedAI_ConfigPlayers.DisableVanillaDoorNoiseSystem) {
			super.OnEndServer(action_data);
		}
	}
}

