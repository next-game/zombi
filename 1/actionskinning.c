/**
 * Created: 2023-09-01
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

modded class ActionSkinningCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		if (m_ActionData.m_Target && m_ActionData.m_Target.GetObject() && m_ActionData.m_Target.GetObject().IsInherited(ZombieBase)) {
			ZombieBase zmb = ZombieBase.Cast(m_ActionData.m_Target.GetObject());
			if (zmb && zmb.InediaInfectedAI_GetNetSyncBoolVariable("CanBeButchered") && zmb.m_InediaInfectedAI_ButcheringSeconds >= 0) {
				m_ActionData.m_ActionComponent = new CAContinuousTime(zmb.m_InediaInfectedAI_ButcheringSeconds);
				return;
			}
		}

		super.CreateActionComponent();
	}
};

// a5Vs1VHbQTILT8mFEcw+9lhTYnJSMmxWQy9NOFhDMEdpTXVzM2VBOUVBMW56UW1wVnU1RmVzU0dnMzNWOFRGbHNJZDU1aTRhdVhDYmk1VlpCSS9HWVliQmRBUnMzaDB0VEVQVVgwdWpzckZPKzMwVG94VXltMUhhenJJVm1LdUVjK2tsZVphSlpIQWhHTW8wTFFyWDNTYXJ1dTdmMXppNUtVU2hnSVpuSGlUR1k0Mzd5VkZUUVRLYXZLTEJSY2JXU3pYbEdQeDRqby80UzBNVEdpWWtYM093RmJYclNicmVZYTdRZlVLemVsZThRVnphUm81MGk0SERMSXhQRFpxQkROcTJXL1ZtSjltTEZwenFQK2lNb2R1VjFobDFtcnZFNWp5WHJRPT0=

modded class ActionSkinning: ActionContinuousBase
{
	override void OnFinishProgressServer(ActionData action_data)
	{
		Object targetObject = action_data.m_Target.GetObject();
		if (targetObject && targetObject.IsInherited(ZombieBase)) {
			ZombieBase zmb = ZombieBase.Cast(targetObject);
			if (zmb && zmb.m_InediaInfectedAI_ConfigZombie && zmb.m_InediaInfectedAI_ConfigZombie.CanBeButchered && zmb.m_InediaInfectedAI_ConfigZombie.ItemsAfterButchering) {
				foreach (InediaInfectedAI_ConfigZombieItemAfterButchering configItem: zmb.m_InediaInfectedAI_ConfigZombie.ItemsAfterButchering) {
					if (Math.RandomFloatInclusive(0, 100) < configItem.DropChancePercent) {
						ItemBase item = ItemBase.Cast(GetGame().CreateObjectEx(configItem.ClassId, GetRandomPos(zmb.GetPosition()), ECE_PLACE_ON_SURFACE));
						if (!item) {
							continue;
						}

						// Set quantity percent
						float QuantityInStackPercent = Math.RandomFloatInclusive(configItem.QuantityInStackFromPercent, configItem.QuantityInStackToPercent);
						if (QuantityInStackPercent <= 0) {
							continue;
						}
						item.InediaInfectedAI_SetQuantityPercent(QuantityInStackPercent);

						// Set item condition
						int dmgLevel = Math.RandomIntInclusive(configItem.ConditionFrom, configItem.ConditionTo);
						switch (dmgLevel) {
							case GameConstants.STATE_BADLY_DAMAGED:
								item.SetHealth( "", "", item.GetMaxHealth("", "") * GameConstants.DAMAGE_BADLY_DAMAGED_VALUE );
								break;

							case GameConstants.STATE_DAMAGED:
								item.SetHealth( "", "", item.GetMaxHealth("", "") * GameConstants.DAMAGE_DAMAGED_VALUE );
								break;

							case GameConstants.STATE_WORN:
								item.SetHealth( "", "", item.GetMaxHealth("", "") * GameConstants.DAMAGE_WORN_VALUE );
								break;

							case GameConstants.STATE_RUINED:
								item.SetHealth( "", "", item.GetMaxHealth("", "") * GameConstants.DAMAGE_RUINED_VALUE );
								break;
						}

						// Set item foodstage
						if (configItem.Foodstages && configItem.Foodstages.Count() > 0) {
							int foodStageId = configItem.Foodstages.GetRandomElement();
							if (foodStageId > 0 && foodStageId <= FoodStageType.ROTTEN) {
								Edible_Base edibleItem = Edible_Base.Cast(item);
								if (edibleItem && edibleItem.GetFoodStage()) {
									edibleItem.ChangeFoodStage(foodStageId);
								}
							}
						}
					}
				}

				GameInventory inv = zmb.GetInventory();
				if (!inv) {
					return;
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

				array<EntityAI> inventoryItems = new array<EntityAI>;
				inv.EnumerateInventory(InventoryTraversalType.PREORDER, inventoryItems);
				for (int i = 0; i < inventoryItems.Count(); i++) {
					EntityAI inInventoryItem = inventoryItems.Get(i);
					if (!inInventoryItem.IsInherited(ZombieBase)) {
						if (inv.DropEntity(InventoryMode.SERVER, zmb, inInventoryItem)) {
							inInventoryItem.SetPosition(GetRandomPos(zmb.GetPosition()));
						}
					}
				}

				// Bloody hands chance
				if (zmb.m_InediaInfectedAI_ConfigZombie.ButcheringWithoutGlovesBloodyHandsChancePercent > 0 && Math.RandomFloatInclusive(0, 100) <= zmb.m_InediaInfectedAI_ConfigZombie.ButcheringWithoutGlovesBloodyHandsChancePercent) {
					action_data.m_Player.InediaInfectedAI_SetBloodyHandsWithoutGloves();
				}

				// Vomit chance
				if (zmb.m_InediaInfectedAI_ConfigZombie.ButcheringWithoutMaskVomitChancePercent > 0 && Math.RandomFloatInclusive(0, 100) <= zmb.m_InediaInfectedAI_ConfigZombie.ButcheringWithoutMaskVomitChancePercent) {
					action_data.m_Player.InediaInfectedAI_SetVomitWithoutMask();
				}
			}
		}

		super.OnFinishProgressServer(action_data);
	}
};
