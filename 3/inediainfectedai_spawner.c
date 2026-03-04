class InediaInfectedAI_Spawner
{
	static void Zombies(notnull PlayerBase player, float distance, float amount, int type = 0)
	{
		// Remove all zombies, radius 1km from player
		array<EntityAI> nearbyEntities = new array<EntityAI>;
		MiscGameplayFunctions.InediaInfectedAI_GetEntitiesAtPosition3D(player.GetPosition(), 1000, nearbyEntities);
	    foreach (EntityAI nearEntity : nearbyEntities) {
	        if (nearEntity.IsInherited(ZombieBase)) {
				nearEntity.Delete();
			}
		}
		
		array<string> availableCreatures = {};
		
		if (type == 2) {
			// High STR/DEF zombies
			availableCreatures = {"ZmbM_PatrolNormal_Autumn", "ZmbM_PatrolNormal_Flat", "ZmbM_PatrolNormal_PautRev", "ZmbM_PatrolNormal_Summer", "ZmbM_SoldierNormal", "ZmbM_usSoldier_normal_Desert", "ZmbM_usSoldier_normal_Woodland"};
		} else if (type == 1) {
			// Medium STR/DEF zombies
			availableCreatures = {"ZmbF_DoctorSkinny", "ZmbF_NurseFat", "ZmbF_NurseFat_Winter", "ZmbM_ParamedicNormal_Black", "ZmbM_ParamedicNormal_Green", "ZmbM_ParamedicNormal_Red", "ZmbM_ParamedicNormal_Blue", "ZmbF_ParamedicNormal_Green", "ZmbF_ParamedicNormal_Red", "ZmbF_ParamedicNormal_Blue", "ZmbM_DoctorFat", "ZmbM_DoctorFat_Winter"};
		} else {
			// Low STR/DEF zombies
			availableCreatures = {"ZmbM_MotobikerFat_Beige", "ZmbM_Jacket_grey", "ZmbM_Jacket_brown", "ZmbM_Jacket_grey", "ZmbM_CitizenBFat_Blue", "ZmbF_MilkMaidOld_Green", "ZmbF_MilkMaidOld_Black", "ZmbM_ClerkFat_Khaki", "ZmbM_ClerkFat_Brown", "ZmbM_CitizenASkinny_Blue", "ZmbM_CitizenASkinny_Grey", "ZmbF_ShortSkirt_red", "ZmbF_ShortSkirt_grey", "ZmbF_ShortSkirt_beige", "ZmbM_HikerSkinny_Blue", "ZmbF_JournalistNormal_Blue", "ZmbF_JournalistNormal_Red", "ZmbF_ClerkFat_White", "ZmbM_ClerkFat_Brown", "ZmbF_CitizenBSkinny", "ZmbF_SkaterYoung_Violet", "ZmbF_SkaterYoung_Brown", "ZmbF_SurvivorNormal_Red", "ZmbF_SurvivorNormal_Blue", "ZmbF_VillagerOld_Red", "ZmbF_VillagerOld_Blue", "ZmbF_VillagerOld_Green", "ZmbM_SkaterYoung_Green", "ZmbM_VillagerOld_Green"};
		}
		
		
		float dispersionRadius = Math.Sqrt(amount * Math.Pow(1.5, 2) / Math.PI);
		
		
		int beforeTime = GetGame().GetTime();
		
		for (int i = 0; i < amount; i++) {
			float randomAngle = Math.RandomFloatInclusive(0, 360);
			vector dispersion = Vector(randomAngle, 0, 0).AnglesToVector() * Math.RandomFloat01() * dispersionRadius;
			vector creaturePos = player.GetPosition() + distance * player.GetDirection() + dispersion;
			
			float creaturePosSurfaceY = GetGame().SurfaceRoadY(creaturePos[0], creaturePos[2]) + 1;
			creaturePos = Vector(creaturePos[0], creaturePosSurfaceY, creaturePos[2]);
			
			ZombieBase zmb = ZombieBase.Cast(player.SpawnEntityOnGroundPos(availableCreatures.GetRandomElement(), creaturePos));
			if (zmb) {
				zmb.InediaInfectedAI_InitOnceServer();
			}
		}
		
		string msg = "Spawned zombies: " + amount + ", spawn time: " + (GetGame().GetTime() - beforeTime) + " ms.";
		MiscGameplayFunctions.InediaInfectedAI_SendChatMessageToAllAdmins(msg);
		InediaInfectedAI_Debug.Log(msg);
	}
	
// ......................................................................................................................................................
// ......................................................................................................................................................
// .....................................................................::::.............................................................................
// ..................................................................-=--:::-+=...........:=:............................................................
// ................................................................:=:.-+*##+=*-.:-==+****- .=-..........................................................
// ...............................................................=-:++-==+#@@*=+++=-=+=#:::  :*=........................................................
// .............................................................:=:=-:-==+##===:.  .:::*=   -  .*=-......................................................
// .............................................................=-=.=:=*#%*:-     :----=.-   -: .+.+.....................................................
// .............................................................*-:=.+-*%*--  :--:.    .: -:  .-.+..+....................................................
// .............................................................==.+--***=: -=:::::.... -. .-  .--= :-...................................................
// .............................................................--=====%::.+-     ..:---:=-::=.  +*  +...................................................
// .............................................................:-+==*-== = =---=*%#-  ::-=++=-::=%::+...................................................
// ..............................................................#++=*=--=.*=::+@@-    :-=+---=  :+=+*...................................................
// ..............................................................+*+=*==:=====*@%-      :=+== . .*=+*-...................................................
// ..............................................................:%+*+-+==**:--....           .. =.:*....................................................
// ...............................................................=+%:==*++%:::              :..  : =:...................................................
// ............................................................::-:+%.=+*%+%#+==             ..::-: +....................................................
// .........................................................:::..::#= +##%*+%@=-:            ..:-:.+:....................................................
// ............................................................-+--..**@@%#=#@: -:         :-:::-=*......................................................
// .......................................................--=+**+==+##@@#%:+%@:  :*=:        :==++:......................................................
// .........................................................::..-+%#+=+=*:=+%#+:   -*%*+-.      +-.......................................................
// .......................................................==:=*+--....+*-:=%*:-=----..*@@@%#++==:........:+-.............................................
// .....................................................==+.  .-=:...=+#:++#-.-.-=-.---+%@@@@@+=.......:=+.:=:...........................................
// ..................................................:==.  --.   :-+.-*:*+#+   == .-- .. :=#@%:#.......*=--..-=..........................................
// ................................................:=-       :=:   :+=*+=#%:    .==    +... *.-%......+=+ .-- .-=........................................
// ..............................................:=-           :--. =#*%+:-++=.   :==. .=   + +##+=*==#+*   .-- .=-......................................
// ............................................:=-                :-:**+==+---===-::-==.:-  + ++:#++=:-%==    =-: .=:....................................
// ..........................................:=:    .-:..          .*-=%*=::::   :-++*+*+=- =.+--=-:*-:#%=+.   =::-.:=...................................
// .........................................=-     =:         .-:  .+--#+ :=-.:::     .:--=-=-+===:  * .*@*=+-  -- :-.=-.................................
// .......................................-=.     -         :-:  .==.+%*#.  .--: :---:               *   =%@*===-==  :::=:...............................
// ......................................*-...::::::=-::---------:.-#@#+#::    .--:  .-===--:.      =-    :@@@#- :--   . --..............................
// ......................................*       .=-.  :::::-=+*#%@@%%**:  :::.   .--:     .:---+=-=:      +%@@@#..:--..::=*.............................
// ......................................*     :-:  :-- -*%@@@@@%%*=+%=     .=-::::. .::::       =         +=*@@@%=.---.   --............................
// ......................................*   .:   :=: :#@@@@@%*=-+++*#=    --      :--:   ::::   .+       =: :%%+*@#::-..   +............................
// ......................................+      .=- .*@@@@@@#*++=+%+##:--:-           .--.    ::: ==     =:  =: =*#-== --   =:...........................
// .....+................................+    :=- .+@@@@@@@%#+++=%@#+%=.                 :-.     ..*:  .-.   =    ==.:+:--: .+...........................
// .....+*:..............................+  :=+=:=-+@@@#+-:.   :#@@+=%%=                   -:      .#  .    ::     .*..-=-==.=...........................
// .....-@=:.............................*:++===-..#%*:         %@%#:#-%=.                  -:      +=      .       *....-=+**=..........................
// ......%#-:...........................+#+=--....-%=+         =@@%::# *-=-                  -.     ==              #.......:=*:.........................
// ......+%+:-....................................==*:+       .%@@%=#- -=-.--:                :     =:             =#=...................................
// ......:*%--:..................................:=.=.:=      +%+=@@# .- -+. .---:.           .     =.          :--%.#...................................
// .......=++:=:.................................-:- -.:=.   .@-..-%@#-- ==+=:   .::::::::::.   .:-=*=-:.    ..:.-#= :+..................................
// .......-:*+.+.................................=.:  =..+.  *#....:%@@.-*  .=**=-.     ..:-+**++-:.. .-+#+:..:+%@*   +:.................................
// ........=:=+ +................................+.    =. --:@-......*@%%.     .:=######*+=-:.  :===+===. -*%@@@@%:   .+.................................
// ........=:=-=.=............................:*.=     .=: :#@*.......-%%*=:      ::         -==:   =.  -=-  -%@@=     =:................................
// .........+ *=:--...........................*@*.       -*+*%:.........*@*:----.  .:     .=+:      -=    :+. =@@:      *................................
// .........:-:+*.=:.........................:@*%.   .::::#@%-...........--    .::::-=   .%:        =-     :* =@@=.     --.--............................
// ..........= ==* +.........................=%=*+.--: :-#@%=.............--         .=.  :#:       +.     =: #@@:---:   *=#-............................
// ...........+ *=+.=........................*.*+-=..--.:+=*...............#+--:       =-  .*.      +      + -@@%*:  :=- =*@*............................
// ...........-:.++-:=......................:=.+#=:.+::--:%=...............-:::-=:      --  .+     .+     :- #@#-@#-:. :=*-@++...........................
// ............+.=-*.--.....................+-.-.+.-=:..-@@=...............-+: :::=:     -:  :-    .=     =. @@-.##%-.:: .-%*.=:...........:::-+=-.......
// .............+ #:* =:....................*.=. .+-:-=-%@%=...............*:.-: ::-=.    -:  -    .=     +  %@...#=**----=*.= =:....::::::----..........
// .............--.*-* +....................+ -    .=::*%=*...............:+ :..:. .:+.    -  .:    -    .=  #@:...*:--=::=. +. +-:::  :---..............
// ..............+.=-+=.+..................:=.     .-+*=:#:..............:**-    ..  .-.   ..  -    -    :. .%=*-...*: .=-..:--.  .----..................
// ...............+ *.*::=.................=:      :+:. *:..............:+:=-=-.       :.    .-==-:----=:: :%*:-*-...+-::--.  :-=+#......................
// ...............:=.# # =:................+.    .=-  :+%*..............*+==+-=*==-----------:.----++=::-=====-:-#--::.  :-=*-.  .-+.....................
// ................=.==:# +...............==:   :=. .=*#-..............+-----:::::::::::----==-.-=-.-.--===++*+=-.  .--*++. .==    ==....................
// .................+ *:-+ +..............*=. .+-  -+#+................*:...:-++=+*#+=----:: .==..::+------:.   :-+*:...-===. .+:   =-...................
// .................:= # +:.+............+:= :+. :+##:................==:-----:....:-=+==::-=-. .-=+=:::   :==++- -=:.....==+=. -+.  +:..................
// ..................-:-* #.:=..........:+=.-= .=#%+................:+:       .:---------=++=::=+-:.  :--+#*++:..  .*:......-=+=..--  +:.................
// ...................= +=.# -:.........+:--: .-=#=................=-                   :+=--:.  .-=+*+=+:+- -=.    -%:.......=++- .=. =-................
// ....................+ #.:* =:........+.=:   :*+.................*.          .----::--:.  :--+#+=-:   -:.#*::+.    #%-........=+=. +--*-...............
// ....................:=.# == +.......++=#.  :+*.................-+=:      .---:-==-:..-=+*+--: :    .  * -@+::--   .*@=........-++:.=*:=*:.............
// .....................-:-+ +- +......#:::=-:#*..................++.:=:.:--====-..:=*%@@@:      :.   :. =: %%-.--=.   +@+.........+#*+-::-*.............
// ......................= =: *.:=....+*=+*=+#%:...............:...-++++++==::-+*%*++=##+*=       =   .- := ****::-=-   =@:.........-*:-+=:=+:...........
// .......................= =. + --...+--:-+*@-................:*==++++-:::=***#+*=:..++ :%:      +    =  + -+-=+=.-==:  *=..........=@%=::==-=..........
// .......................:= #: = =..+. :+=+%=................:*-::..:=*%+*#+=+=.=:   -#-::*      =.   =  + .# ::-*=-===:+%+..........#-  :+%-:-=:.......
// ........................-:.# .- =*:   :#%+.............--::. :=*#*=---:+..=.=.+-  --%.  *:     :-   =  *  @. -.:-*=-=**=**.........:=.   =#=+.--......
// .........................= -- :::*+.    *#........:-::: .-+*#@*-.  ::.+. -: =:+-.-..@.  :+     .+   +  *  %= -  =.*-=:++=-+=.........#-:.=%%%+.+......
// ..........................= *.:*+#:@=:-*%+....--::  :====+++##:   .- +. .+  =:=-... %:   *.     *   +  *. #=-.  =..=.*=+*+=+*-:::....*=:-*+===:==+....
// ..........................--=. :*- =#+:+#+--::  :--=:..-*+#+#+... . =:  +.  +..*....*-   :+     *   +  *. #-+.  +. =.-=.=%@@@@@@@@%==:=-++::**=:--....
// ...........................-*:.- .:-+%-=:+ .----.......**=:*.* ..  ::  --   +..*:...==    *.    +.  +  #  #=.=  +  .+:*  -=*#-%#===+====:==++*--......
// ...........................:#::-=:#++.=-+*=:..........-=.-*.+*.....:  .+.  -:..:*...:+    --....+.  = .*  *+ -: =   +:#   +.@=-@-.....................
// ...........................:%**--@#+.-:=*:............%:=+.+.=........-.  .-....%-...+.    =....=.  = ==  **  =.=   .+#   +.#@:*#.....................
// .....................-====-==+-+#%--=---*............+#*+-=. -:.......:.  -....-@%...+... .-....-. .- #.  *%  :+:    -*   --*@#.#-....................
	
	static void Wolves(notnull PlayerBase player, float distance = 10, int amount = 1)
	{
		// Remove all wolves, radius 1km from player
		array<EntityAI> nearbyEntities = new array<EntityAI>;
		MiscGameplayFunctions.InediaInfectedAI_GetEntitiesAtPosition3D(player.GetPosition(), 1000, nearbyEntities);
	    foreach (EntityAI nearEntity : nearbyEntities) {
	        if (nearEntity.IsInherited(Animal_CanisLupus)) {
				nearEntity.Delete();
			}
		}
		
		array<string> availableCreatures = {"Animal_CanisLupus_Grey", "Animal_CanisLupus_White"};
		
		float dispersionRadius = Math.Sqrt(amount * Math.Pow(2, 2) / Math.PI);
		
		int beforeTime = GetGame().GetTime();
		
		for (int i = 0; i < amount; i++) {
			float randomAngle = Math.RandomFloatInclusive(0, 360);
			vector dispersion = Vector(randomAngle, 0, 0).AnglesToVector() * Math.RandomFloat01() * dispersionRadius;
			vector creaturePos = player.GetPosition() + distance * player.GetDirection() + dispersion;
			
			float creaturePosSurfaceY = GetGame().SurfaceRoadY(creaturePos[0], creaturePos[2]) + 1;
			creaturePos = Vector(creaturePos[0], creaturePosSurfaceY, creaturePos[2]);

			AnimalBase animal = AnimalBase.Cast(player.SpawnEntityOnGroundPos(availableCreatures.GetRandomElement(), creaturePos));
			if (animal) {
				animal.InediaInfectedAI_InitOnceServer();
			}
		}
		
		string msg = "Spawned wolves: " + amount + ", spawn time: " + (GetGame().GetTime() - beforeTime) + " ms.";
		MiscGameplayFunctions.InediaInfectedAI_SendChatMessageToAllAdmins(msg);
		InediaInfectedAI_Debug.Log(msg);
	}
	
	static void Bears(notnull PlayerBase player, float distance = 10, int amount = 1)
	{
		// Remove all bears, radius 1km from player
		array<EntityAI> nearbyEntities = new array<EntityAI>;
		MiscGameplayFunctions.InediaInfectedAI_GetEntitiesAtPosition3D(player.GetPosition(), 1000, nearbyEntities);
	    foreach (EntityAI nearEntity : nearbyEntities) {
	        if (nearEntity.IsInherited(Animal_UrsusArctos)) {
				nearEntity.Delete();
			}
		}
		
		array<string> availableCreatures = {"Animal_UrsusArctos"};
		
		float dispersionRadius = Math.Sqrt(amount * Math.Pow(3, 2) / Math.PI);
		
		int beforeTime = GetGame().GetTime();
		
		for (int i = 0; i < amount; i++) {
			float randomAngle = Math.RandomFloatInclusive(0, 360);
			vector dispersion = Vector(randomAngle, 0, 0).AnglesToVector() * Math.RandomFloat01() * dispersionRadius;
			vector creaturePos = player.GetPosition() + distance * player.GetDirection() + dispersion;
			
			float creaturePosSurfaceY = GetGame().SurfaceRoadY(creaturePos[0], creaturePos[2]) + 1;
			creaturePos = Vector(creaturePos[0], creaturePosSurfaceY, creaturePos[2]);
			
			AnimalBase animal = AnimalBase.Cast(player.SpawnEntityOnGroundPos(availableCreatures.GetRandomElement(), creaturePos));
			if (animal) {
				animal.InediaInfectedAI_InitOnceServer();
			}
		}
		
		string msg = "Spawned bears: " + amount + ", spawn time: " + (GetGame().GetTime() - beforeTime) + " ms.";
		MiscGameplayFunctions.InediaInfectedAI_SendChatMessageToAllAdmins(msg);
		InediaInfectedAI_Debug.Log(msg);
	}
	
	static void Deers(notnull PlayerBase player, float distance = 10, int amount = 1)
	{
		// Remove all deers, radius 1km from player
		array<EntityAI> nearbyEntities = new array<EntityAI>;
		MiscGameplayFunctions.InediaInfectedAI_GetEntitiesAtPosition3D(player.GetPosition(), 1000, nearbyEntities);
	    foreach (EntityAI nearEntity : nearbyEntities) {
	        if (nearEntity.IsInherited(Animal_CervusElaphus)) {
				nearEntity.Delete();
			}
		}
		
		array<string> availableCreatures = {"Animal_CervusElaphus"};
		
		float dispersionRadius = Math.Sqrt(amount * Math.Pow(3, 2) / Math.PI);
		
		int beforeTime = GetGame().GetTime();
		
		for (int i = 0; i < amount; i++) {
			float randomAngle = Math.RandomFloatInclusive(0, 360);
			vector dispersion = Vector(randomAngle, 0, 0).AnglesToVector() * Math.RandomFloat01() * dispersionRadius;
			vector creaturePos = player.GetPosition() + distance * player.GetDirection() + dispersion;
			
			float creaturePosSurfaceY = GetGame().SurfaceRoadY(creaturePos[0], creaturePos[2]) + 1;
			creaturePos = Vector(creaturePos[0], creaturePosSurfaceY, creaturePos[2]);
			
			AnimalBase animal = AnimalBase.Cast(player.SpawnEntityOnGroundPos(availableCreatures.GetRandomElement(), creaturePos));
			if (animal) {
				animal.InediaInfectedAI_InitOnceServer();
			}
		}
		
		string msg = "Spawned deers: " + amount + ", spawn time: " + (GetGame().GetTime() - beforeTime) + " ms.";
		MiscGameplayFunctions.InediaInfectedAI_SendChatMessageToAllAdmins(msg);
		InediaInfectedAI_Debug.Log(msg);
	}
	
	static void Car(notnull PlayerBase player)
	{
		vector carPos = player.GetPosition() + 3 * player.GetDirection();
		float carPosSurfaceY = GetGame().SurfaceRoadY(carPos[0], carPos[2]) + 1;
		carPos = Vector(carPos[0], carPosSurfaceY, carPos[2]);
		Car car = Car.Cast(player.SpawnEntityOnGroundPos("Hatchback_02_Blue", carPos));
		car.GetInventory().CreateAttachment("Hatchback_02_Trunk_Blue");
		car.GetInventory().CreateAttachment("Hatchback_02_Door_1_1_Blue");
		car.GetInventory().CreateAttachment("Hatchback_02_Door_1_2_Blue");
		car.GetInventory().CreateAttachment("Hatchback_02_Door_2_1_Blue");
		car.GetInventory().CreateAttachment("Hatchback_02_Door_2_2_Blue");
		car.GetInventory().CreateAttachment("Hatchback_02_Hood_Blue");
		car.GetInventory().CreateAttachment("Hatchback_02_Wheel");
		car.GetInventory().CreateAttachment("Hatchback_02_Wheel");
		car.GetInventory().CreateAttachment("Hatchback_02_Wheel");
		car.GetInventory().CreateAttachment("Hatchback_02_Wheel");
		car.GetInventory().CreateAttachment("Hatchback_02_Wheel");
		
		// Do general car building matching all car types
		car.GetInventory().CreateAttachment("CarRadiator");
		car.GetInventory().CreateAttachment("CarBattery");
		car.GetInventory().CreateAttachment("SparkPlug");
		car.GetInventory().CreateAttachment("HeadlightH7");
		car.GetInventory().CreateAttachment("HeadlightH7");
		
		// Fill all the fluids
		car.Fill(CarFluid.FUEL, car.GetFluidCapacity(CarFluid.FUEL));
		car.Fill(CarFluid.OIL, car.GetFluidCapacity(CarFluid.OIL));
		car.Fill(CarFluid.BRAKE, car.GetFluidCapacity(CarFluid.BRAKE));
		car.Fill(CarFluid.COOLANT, car.GetFluidCapacity(CarFluid.COOLANT));
		
		// Set neutral gear
		car.GetController().ShiftTo(CarGear.NEUTRAL);
	}
	
// S9zv0f2Z12wDSg1tPHp0cWF6aFZVWVYrcEZycGpPMU1OTnpyMHd2NXNWVUdoUW9EbVBDMVkrRC9jRjZRRXYram0xdWYvR2dIZWxvVjZqYS84a0JnWG9yZGh4TXdDMkdSTUxDUWNmUlQ3R3pEMTljTVpBRFc2Mm1RKzRBQWFtbzNWR3Y5U1lkbGlWdlRBUjJpc2IzcERTOXhzSlpKbVJOeFQ1TEJqTG5kcnJhdGQzSkkvQlQxeXVpeFhEenRQNUlCSkRkU1dWUHZMbm9oQWFnVnF0ay9XYlpqbmdXQXorYk44VE10Z2hzQjBWem5Vak9SRVBPeElxKzZQT29ON0EzaStoMUdSbDIycHFxcjVsdUVVME1qVFlpbnYwZDB3b2REQ2hMa3FnPT0=
	
	static void Player(notnull PlayerBase player, float distance = 10, bool armor = true, bool reversePain = true)
	{
		// Remove all test players, radius 1km from player
		array<EntityAI> nearbyEntities = new array<EntityAI>;
		MiscGameplayFunctions.InediaInfectedAI_GetEntitiesAtPosition3D(player.GetPosition(), 1000, nearbyEntities);
	    foreach (EntityAI nearEntity : nearbyEntities) {
			if (nearEntity.IsInherited(PlayerBase)) {
		    	PlayerBase playerToDelete = PlayerBase.Cast(nearEntity);
		        if (playerToDelete && playerToDelete.m_InediaInfectedAI_IsTestPlayer) {
		        	playerToDelete.Delete();
		        }
			}
		}
		
	    // Create test player
		vector borisPos = player.GetPosition() + distance * player.GetDirection();
		float borisPosSurfaceY = GetGame().SurfaceRoadY(borisPos[0], borisPos[2]) + 1;
		borisPos = Vector(borisPos[0], borisPosSurfaceY, borisPos[2]);
		PlayerBase boris = PlayerBase.Cast(player.SpawnEntityOnGroundPos("SurvivorM_Boris", borisPos));

		if (!boris) {
			return;
		}
		
		boris.m_InediaInfectedAI_IsTestPlayer = true;
		boris.m_InediaInfectedAI_TestPlayerReversePainIsActive = reversePain;
		
	    InediaInfectedAI_RemoveAllPlayerItems(boris);
	
	    // Clothes
		if (armor) {
			boris.GetInventory().CreateInInventory("GorkaHelmet");
		    boris.GetInventory().CreateInInventory("PlateCarrierVest");
		} else {
		    boris.GetInventory().CreateInInventory("RadarCap_Black");
		    boris.GetInventory().CreateInInventory("HuntingVest");
		}
	
	    boris.GetInventory().CreateInInventory("HuntingJacket_Summer");
	    boris.GetInventory().CreateInInventory("HunterPants_Summer");
	    boris.GetInventory().CreateInInventory("HuntingBag");
	    boris.GetInventory().CreateInInventory("BalaclavaMask_Black");
	    boris.GetInventory().CreateInInventory("OMNOGloves_Gray");
	    boris.GetInventory().CreateInInventory("MilitaryBoots_Black");
	
	    // SKS
	    Weapon_Base sks = Weapon_Base.Cast(boris.GetInventory().CreateInInventory("SKS_Green"));
	    sks.GetInventory().CreateAttachment("PUScopeOptic");
	    sks.SpawnAmmo();
	    for (int sksAmmo = 1; sksAmmo <= 20; sksAmmo++) {
	        boris.GetInventory().CreateInInventory("Ammo_762x39");
	    }
		
		// Mosin
	    Weapon_Base mosin = Weapon_Base.Cast(boris.GetInventory().CreateInInventory("Mosin9130"));
	    mosin.GetInventory().CreateAttachment("PUScopeOptic");
	    mosin.SpawnAmmo();
	    for (int mosinAmmo = 1; mosinAmmo <= 20; mosinAmmo++) {
	        boris.GetInventory().CreateInInventory("Ammo_762x54");
	    }
	
	    // AS VAL
	    Weapon_Base asval = Weapon_Base.Cast(boris.GetInventory().CreateInInventory("ASVAL"));
	    asval.GetInventory().CreateAttachment("ACOGOptic_6x");
	    asval.SpawnAmmo("Mag_VAL_20Rnd");
	    for (int asvalMag = 1; asvalMag <= 5; asvalMag++) {
	        boris.GetInventory().CreateInInventory("Mag_VAL_20Rnd");
	    }
		for (int asvalAmmo = 1; asvalAmmo <= 20; asvalAmmo++) {
	        boris.GetInventory().CreateInInventory("Ammo_9x39");
	    }
	}
	
	static void InediaInfectedAI_PlayerTestEquip1(PlayerBase player)
	{
		if (!player) {
			return;
		}
		
		InediaInfectedAI_RemoveAllPlayerItems(player);
		
		DayZPlayerInventory playerInv = DayZPlayerInventory.Cast(player.GetInventory());
		if (!playerInv) {
			return;
		}
		
		// Clothes + attachments
		PressVest_Blue vest = PressVest_Blue.Cast(playerInv.CreateInInventory("PressVest_Blue"));
		M65Jacket_Black jacket = M65Jacket_Black.Cast(playerInv.CreateInInventory("M65Jacket_Black"));
		HunterPants_Summer pants = HunterPants_Summer.Cast(playerInv.CreateInInventory("HunterPants_Summer"));
		DryBag_Black backpack = DryBag_Black.Cast(playerInv.CreateInInventory("DryBag_Black"));
		PersonalRadio personalRadio = PersonalRadio.Cast(backpack.GetInventory().CreateAttachment("PersonalRadio"));
		personalRadio.GetInventory().CreateAttachment("Battery9V");
		backpack.GetInventory().CreateAttachment("Chemlight_Red");
		MilitaryBelt belt = MilitaryBelt.Cast(playerInv.CreateInInventory("MilitaryBelt"));
		PlateCarrierHolster pistolHolster = PlateCarrierHolster.Cast(belt.GetInventory().CreateAttachment("PlateCarrierHolster"));
		NylonKnifeSheath knifeSneath = NylonKnifeSheath.Cast(belt.GetInventory().CreateAttachment("NylonKnifeSheath"));
		playerInv.CreateInInventory("BalaclavaMask_Black");
		playerInv.CreateInInventory("ThinFramesGlasses");
		playerInv.CreateInInventory("OMNOGloves_Gray");
		MilitaryBoots_Black boots = MilitaryBoots_Black.Cast(playerInv.CreateInInventory("MilitaryBoots_Black"));
		Mich2001Helmet hemlet = Mich2001Helmet.Cast(playerInv.CreateInInventory("Mich2001Helmet"));
		NVGoggles nvgoogles = NVGoggles.Cast(hemlet.GetInventory().CreateAttachment("NVGoggles"));
		nvgoogles.GetInventory().CreateAttachment("Battery9V");
		UniversalLight universalLightForHelmet = UniversalLight.Cast(hemlet.GetInventory().CreateAttachment("UniversalLight"));
		universalLightForHelmet.GetInventory().CreateAttachment("Battery9V");
		
		// Medicine
		InediaPain_FirstAidCase_Camouflage medCase = InediaPain_FirstAidCase_Camouflage.Cast(belt.GetInventory().CreateAttachment("InediaPain_FirstAidCase_Camouflage"));
		PainkillerTablets painkillers = PainkillerTablets.Cast(medCase.GetInventory().CreateAttachment("PainkillerTablets"));	
		medCase.GetInventory().CreateAttachment("InediaPain_Tramadol");
		medCase.GetInventory().CreateAttachment("InediaPain_Calcium");
		medCase.GetInventory().CreateAttachment("InediaPain_Tranexam");
		medCase.GetInventory().CreateAttachment("InediaPain_Sorbifer");
		medCase.GetInventory().CreateAttachment("InediaPain_Metoclopramide");
		medCase.GetInventory().CreateAttachment("CharcoalTablets");
		medCase.GetInventory().CreateAttachment("TetracyclineAntibiotics");
		medCase.GetInventory().CreateAttachment("InediaPain_SurgicalKit");
		medCase.GetInventory().CreateAttachment("InediaPain_SurgicalKit");
		medCase.GetInventory().CreateAttachment("InediaPain_SplintKit");
		medCase.GetInventory().CreateAttachment("SalineBagIV");
		medCase.GetInventory().CreateAttachment("BloodBagEmpty");
		medCase.GetInventory().CreateAttachment("StartKitIV");
		medCase.GetInventory().CreateAttachment("Rag");
		medCase.GetInventory().CreateAttachment("IodineTincture");
		BandageDressing bandageDressing = BandageDressing.Cast(medCase.GetInventory().CreateAttachment("BandageDressing"));
		medCase.GetInventory().CreateAttachment("BandageDressing");
		medCase.GetInventory().CreateAttachment("InediaPain_HemostaticDressing");
		medCase.GetInventory().CreateAttachment("InediaPain_HemostaticDressing");
		Morphine morphine = Morphine.Cast(medCase.GetInventory().CreateAttachment("Morphine"));
		medCase.GetInventory().CreateAttachment("Morphine");
		Epinephrine epinephrine = Epinephrine.Cast(medCase.GetInventory().CreateAttachment("Epinephrine"));
		medCase.GetInventory().CreateAttachment("AntiChemInjector");
		
		medCase.GetInventory().CreateInInventory("Canteen");
		medCase.GetInventory().CreateInInventory("VitaminBottle");
		medCase.GetInventory().CreateInInventory("PurificationTablets");
		
		// Weapons + ammo
		Weapon_Base augAX = Weapon_Base.Cast(playerInv.CreateInInventory("Aug"));
		augAX.SpawnAmmo("Mag_STANAG_60Rnd");
		augAX.GetInventory().CreateAttachment("M4_Suppressor");
		augAX.GetInventory().CreateAttachment("ACOGOptic_6x");
		Weapon_Base mkII = Weapon_Base.Cast(pistolHolster.GetInventory().CreateInInventory("MKII"));
		mkII.SpawnAmmo("Mag_MKII_10Rnd");
		Weapon_Base winchecter70 = Weapon_Base.Cast(playerInv.CreateInInventory("Winchester70"));
		winchecter70.SpawnAmmo("Ammo_308WinTracer");
		winchecter70.GetInventory().CreateAttachment("HuntingOptic");
		winchecter70.GetInventory().CreateAttachment("ImprovisedSuppressor");
		
		for (int j = 1; j <= 1; j++) {
		    playerInv.CreateInInventory("Mag_STANAG_60Rnd");
		}
		for (int i = 1; i <= 1; i++) {
		    playerInv.CreateInInventory("Mag_MKII_10Rnd");
		}
		for (int ammo556 = 1; ammo556 <= 6; ammo556++) {
		    playerInv.CreateInInventory("Ammo_556x45");
		}
		for (int ammo22 = 1; ammo22 <= 2; ammo22++) {
		    playerInv.CreateInInventory("Ammo_22");
		}
		
		// Knifes
		knifeSneath.GetInventory().CreateAttachment("HuntingKnife");
		CombatKnife combatKnife = CombatKnife.Cast(boots.GetInventory().CreateAttachment("CombatKnife"));
		
		// Grenades
		RGD5Grenade grenade = RGD5Grenade.Cast(playerInv.CreateInInventory("RGD5Grenade"));
		for (int grenades = 1; grenades <= 3; grenades++) {
		    playerInv.CreateInInventory("RGD5Grenade");
		}
		
		// Jacket
		// jacket.GetInventory().CreateInInventory("XXX");
		
		// Pants
		for (int smokeGrenadesPants = 1; smokeGrenadesPants <= 1; smokeGrenadesPants++) {
		    pants.GetInventory().CreateInInventory("M18SmokeGrenade_Red");
		}
		
		for (int battery = 1; battery <= 2; battery++) {
			pants.GetInventory().CreateInInventory("Battery9V");
		}
		for (int uLight = 1; uLight <= 2; uLight++) {
		    UniversalLight universalLight = UniversalLight.Cast(pants.GetInventory().CreateInInventory("UniversalLight"));
			universalLight.GetInventory().CreateAttachment("Battery9V");
		}
		
		// Backpack
		for (int sticks = 1; sticks <= 2; sticks++) {
			WoodenStick stick = WoodenStick.Cast(backpack.GetInventory().CreateInInventory("WoodenStick"));
			stick.SetQuantity(stick.GetQuantityMax(), false);
		}
		OrienteeringCompass compass = OrienteeringCompass.Cast(backpack.GetInventory().CreateInInventory("OrienteeringCompass"));
		backpack.GetInventory().CreateInInventory("SodaCan_Cola");
		backpack.GetInventory().CreateInInventory("Lockpick");
		backpack.GetInventory().CreateInInventory("Rag");
		backpack.GetInventory().CreateInInventory("Rag");
		backpack.GetInventory().CreateInInventory("Rope");
		backpack.GetInventory().CreateInInventory("Matchbox");
		Rangefinder rangefinder = Rangefinder.Cast(backpack.GetInventory().CreateInInventory("Rangefinder"));
		rangefinder.GetInventory().CreateAttachment("Battery9V");
		backpack.GetInventory().CreateInInventory("SewingKit");
		backpack.GetInventory().CreateInInventory("M4_Suppressor");
		
		for (int k = 1; k <= 3; k++) {
		    backpack.GetInventory().CreateInInventory("Ammo_308WinTracer");
		}
		
		backpack.GetInventory().CreateInInventory("Handcuffs");
		backpack.GetInventory().CreateInInventory("HandcuffKeys");
		
		// Shortcuts
		player.SetQuickBarEntityShortcut(augAX, 0);
		player.SetQuickBarEntityShortcut(mkII, 1);
		player.SetQuickBarEntityShortcut(combatKnife, 2);
		player.SetQuickBarEntityShortcut(grenade, 3);
		player.SetQuickBarEntityShortcut(rangefinder, 4);
		player.SetQuickBarEntityShortcut(bandageDressing, 5);
		player.SetQuickBarEntityShortcut(painkillers, 6);
		player.SetQuickBarEntityShortcut(morphine, 7);
		player.SetQuickBarEntityShortcut(epinephrine, 8);
		player.SetQuickBarEntityShortcut(winchecter70, 9);
	}
	
	static void InediaInfectedAI_PlayerTestEquip2(PlayerBase player)
	{
		if (!player) {
			return;
		}
		
		InediaInfectedAI_RemoveAllPlayerItems(player);
		
		DayZPlayerInventory playerInv = DayZPlayerInventory.Cast(player.GetInventory());
		if (!playerInv) {
			return;
		}
		
		// Clothes + attachments
		playerInv.CreateInInventory("HighCapacityVest_Black");
		M65Jacket_Black jacket = M65Jacket_Black.Cast(playerInv.CreateInInventory("M65Jacket_Black"));
		CargoPants_Black pants = CargoPants_Black.Cast(playerInv.CreateInInventory("CargoPants_Black"));
		HipPack_Black hippack = HipPack_Black.Cast(playerInv.CreateInInventory("HipPack_Black"));
		LeatherSack_Black backpack = LeatherSack_Black.Cast(playerInv.CreateInInventory("LeatherSack_Black"));
		playerInv.CreateInInventory("BalaclavaMask_Black");
		playerInv.CreateInInventory("ThinFramesGlasses");
		playerInv.CreateInInventory("WorkingGloves_Black");
		MilitaryBoots_Black boots = MilitaryBoots_Black.Cast(playerInv.CreateInInventory("MilitaryBoots_Black"));
		HuntingKnife knife = HuntingKnife.Cast(boots.GetInventory().CreateAttachment("HuntingKnife"));
		playerInv.CreateInInventory("RadarCap_Black");

		// Grenades
		RGD5Grenade grenade = RGD5Grenade.Cast(playerInv.CreateInInventory("RGD5Grenade"));
		for (int grenades = 1; grenades <= 3; grenades++) {
		    playerInv.CreateInInventory("RGD5Grenade");
		}
		
		// CZ75
		player.CreateInInventory("Mag_CZ75_15Rnd");
		Weapon_Base cz75 = Weapon_Base.Cast(playerInv.CreateInInventory("CZ75"));
		cz75.GetInventory().CreateAttachment("PistolSuppressor");
		FNP45_MRDSOptic pistolOptic = FNP45_MRDSOptic.Cast(cz75.GetInventory().CreateAttachment("FNP45_MRDSOptic"));
		pistolOptic.GetInventory().CreateAttachment("Battery9V");
		cz75.SpawnAmmo("Mag_CZ75_15Rnd");
		for (int i = 1; i <= 12; i++) {
		    player.CreateInInventory("Ammo_9x19");
		}
		
		// Jacket
		jacket.GetInventory().CreateInInventory("Mag_MP5_30Rnd");
		Weapon_Base mp5 = Weapon_Base.Cast(jacket.GetInventory().CreateInInventory("MP5K"));
		mp5.GetInventory().CreateAttachment("PistolSuppressor");
		mp5.GetInventory().CreateAttachment("MP5k_StockBttstck");
		mp5.GetInventory().CreateAttachment("Mp5kRecoil");
		M68Optic optic = M68Optic.Cast(mp5.GetInventory().CreateAttachment("M68Optic"));
		optic.GetInventory().CreateAttachment("Battery9V");
		mp5.SpawnAmmo("Mag_MP5_30Rnd");
		jacket.GetInventory().CreateInInventory("PistolSuppressor");
		jacket.GetInventory().CreateInInventory("PistolSuppressor");
		
		// Hippack
		hippack.GetInventory().CreateInInventory("M18SmokeGrenade_Red");
		hippack.GetInventory().CreateInInventory("M18SmokeGrenade_Green");
		hippack.GetInventory().CreateInInventory("M18SmokeGrenade_White");
		
		// Medicine
		InediaPain_FirstAidCase_Black medCase = InediaPain_FirstAidCase_Black.Cast(hippack.GetInventory().CreateAttachment("InediaPain_FirstAidCase_Black"));
		PainkillerTablets painkillers = PainkillerTablets.Cast(medCase.GetInventory().CreateAttachment("PainkillerTablets"));	
		medCase.GetInventory().CreateAttachment("InediaPain_Tramadol");
		medCase.GetInventory().CreateAttachment("InediaPain_Calcium");
		medCase.GetInventory().CreateAttachment("InediaPain_Tranexam");
		medCase.GetInventory().CreateAttachment("InediaPain_Sorbifer");
		medCase.GetInventory().CreateAttachment("InediaPain_Metoclopramide");
		medCase.GetInventory().CreateAttachment("CharcoalTablets");
		medCase.GetInventory().CreateAttachment("TetracyclineAntibiotics");
		medCase.GetInventory().CreateAttachment("InediaPain_SurgicalKit");
		medCase.GetInventory().CreateAttachment("InediaPain_SurgicalKit");
		medCase.GetInventory().CreateAttachment("InediaPain_SplintKit");
		medCase.GetInventory().CreateAttachment("SalineBagIV");
		medCase.GetInventory().CreateAttachment("BloodBagEmpty");
		medCase.GetInventory().CreateAttachment("StartKitIV");
		medCase.GetInventory().CreateAttachment("Rag");
		medCase.GetInventory().CreateAttachment("IodineTincture");
		BandageDressing bandageDressing = BandageDressing.Cast(medCase.GetInventory().CreateAttachment("BandageDressing"));
		medCase.GetInventory().CreateAttachment("BandageDressing");
		medCase.GetInventory().CreateAttachment("InediaPain_HemostaticDressing");
		medCase.GetInventory().CreateAttachment("InediaPain_HemostaticDressing");
		Morphine morphine = Morphine.Cast(medCase.GetInventory().CreateAttachment("Morphine"));
		medCase.GetInventory().CreateAttachment("Morphine");
		Epinephrine epinephrine = Epinephrine.Cast(medCase.GetInventory().CreateAttachment("Epinephrine"));
		medCase.GetInventory().CreateAttachment("AntiChemInjector");
		
		medCase.GetInventory().CreateInInventory("Canteen");
		medCase.GetInventory().CreateInInventory("VitaminBottle");
		medCase.GetInventory().CreateInInventory("PurificationTablets");
		
		// Pants
		// pants.GetInventory().CreateInInventory("XXX");
		
		// Backpack
		OrienteeringCompass compass = OrienteeringCompass.Cast(backpack.GetInventory().CreateInInventory("OrienteeringCompass"));
		Rangefinder rangefinder = Rangefinder.Cast(backpack.GetInventory().CreateInInventory("Rangefinder"));
		rangefinder.GetInventory().CreateAttachment("Battery9V");
		WoodenStick stick = WoodenStick.Cast(backpack.GetInventory().CreateInInventory("WoodenStick"));
		stick.SetQuantity(stick.GetQuantityMax(), false);
		backpack.GetInventory().CreateInInventory("Matchbox");
		backpack.GetInventory().CreateInInventory("Rag");
		backpack.GetInventory().CreateInInventory("Rag");
		backpack.GetInventory().CreateInInventory("SewingKit");
		backpack.GetInventory().CreateInInventory("Lockpick");
		backpack.GetInventory().CreateInInventory("Rope");
		backpack.GetInventory().CreateInInventory("Handcuffs");
		backpack.GetInventory().CreateInInventory("HandcuffKeys");
		
		
		// Shortcuts
		player.SetQuickBarEntityShortcut(mp5, 0);
		player.SetQuickBarEntityShortcut(cz75, 1);
		player.SetQuickBarEntityShortcut(knife, 2);
		player.SetQuickBarEntityShortcut(grenade, 3);
		player.SetQuickBarEntityShortcut(rangefinder, 4);
		player.SetQuickBarEntityShortcut(bandageDressing, 5);
		player.SetQuickBarEntityShortcut(painkillers, 6);
		player.SetQuickBarEntityShortcut(morphine, 7);
		player.SetQuickBarEntityShortcut(epinephrine, 8);
	}
	
	static void InediaInfectedAI_PlayerTestEquip3(PlayerBase player)
	{
		if (!player) {
			return;
		}
		
		InediaInfectedAI_RemoveAllPlayerItems(player);
		
		DayZPlayerInventory playerInv = DayZPlayerInventory.Cast(player.GetInventory());
		if (!playerInv) {
			return;
		}
		
		// Clothes + attachments
		playerInv.CreateInInventory("HighCapacityVest_Black");
		M65Jacket_Black jacket = M65Jacket_Black.Cast(playerInv.CreateInInventory("M65Jacket_Black"));
		CargoPants_Black pants = CargoPants_Black.Cast(playerInv.CreateInInventory("CargoPants_Black"));
		HipPack_Black hippack = HipPack_Black.Cast(playerInv.CreateInInventory("HipPack_Black"));
		LeatherSack_Black backpack = LeatherSack_Black.Cast(playerInv.CreateInInventory("LeatherSack_Black"));
		playerInv.CreateInInventory("BalaclavaMask_Black");
		playerInv.CreateInInventory("ThinFramesGlasses");
		playerInv.CreateInInventory("WorkingGloves_Black");
		MilitaryBoots_Black boots = MilitaryBoots_Black.Cast(playerInv.CreateInInventory("MilitaryBoots_Black"));
		HuntingKnife knife = HuntingKnife.Cast(boots.GetInventory().CreateAttachment("HuntingKnife"));
		playerInv.CreateInInventory("RadarCap_Black");
		
		// Grenades
		RGD5Grenade grenade = RGD5Grenade.Cast(playerInv.CreateInInventory("RGD5Grenade"));
		for (int grenades = 1; grenades <= 3; grenades++) {
		    playerInv.CreateInInventory("RGD5Grenade");
		}
		
		// Crossbow
		Weapon_Base crossbow = Weapon_Base.Cast(playerInv.CreateInInventory("Crossbow_Black"));
		crossbow.SpawnAmmo("Ammo_ImprovisedBolt_2");
		for (int crossbowAmmo = 1; crossbowAmmo <= 8; crossbowAmmo++) {
		    player.CreateInInventory("Ammo_ImprovisedBolt_2");
		}
		
		// Izh43Shotgun
		Weapon_Base izh43 = Weapon_Base.Cast(playerInv.CreateInInventory("Izh43Shotgun"));
		izh43.SpawnAmmo("Ammo_12gaPellets");
		for (int izh43Ammo = 1; izh43Ammo <= 12; izh43Ammo++) {
		    player.CreateInInventory("Ammo_12gaPellets");
		}
		
		// Jacket
		//jacket.GetInventory().CreateInInventory("XXX");
		
		// Pants
		// pants.GetInventory().CreateInInventory("XXX");
		
		// Hippack
		hippack.GetInventory().CreateInInventory("M18SmokeGrenade_Red");
		hippack.GetInventory().CreateInInventory("M18SmokeGrenade_Green");
		hippack.GetInventory().CreateInInventory("M18SmokeGrenade_White");
		
		// Medicine
		InediaPain_FirstAidCase_Black medCase = InediaPain_FirstAidCase_Black.Cast(hippack.GetInventory().CreateAttachment("InediaPain_FirstAidCase_Black"));
		PainkillerTablets painkillers = PainkillerTablets.Cast(medCase.GetInventory().CreateAttachment("PainkillerTablets"));	
		medCase.GetInventory().CreateAttachment("InediaPain_Tramadol");
		medCase.GetInventory().CreateAttachment("InediaPain_Calcium");
		medCase.GetInventory().CreateAttachment("InediaPain_Tranexam");
		medCase.GetInventory().CreateAttachment("InediaPain_Sorbifer");
		medCase.GetInventory().CreateAttachment("InediaPain_Metoclopramide");
		medCase.GetInventory().CreateAttachment("CharcoalTablets");
		medCase.GetInventory().CreateAttachment("TetracyclineAntibiotics");
		medCase.GetInventory().CreateAttachment("InediaPain_SurgicalKit");
		medCase.GetInventory().CreateAttachment("InediaPain_SurgicalKit");
		medCase.GetInventory().CreateAttachment("InediaPain_SplintKit");
		medCase.GetInventory().CreateAttachment("SalineBagIV");
		medCase.GetInventory().CreateAttachment("BloodBagEmpty");
		medCase.GetInventory().CreateAttachment("StartKitIV");
		medCase.GetInventory().CreateAttachment("Rag");
		medCase.GetInventory().CreateAttachment("IodineTincture");
		BandageDressing bandageDressing = BandageDressing.Cast(medCase.GetInventory().CreateAttachment("BandageDressing"));
		medCase.GetInventory().CreateAttachment("BandageDressing");
		medCase.GetInventory().CreateAttachment("InediaPain_HemostaticDressing");
		medCase.GetInventory().CreateAttachment("InediaPain_HemostaticDressing");
		Morphine morphine = Morphine.Cast(medCase.GetInventory().CreateAttachment("Morphine"));
		medCase.GetInventory().CreateAttachment("Morphine");
		Epinephrine epinephrine = Epinephrine.Cast(medCase.GetInventory().CreateAttachment("Epinephrine"));
		medCase.GetInventory().CreateAttachment("AntiChemInjector");
		
		medCase.GetInventory().CreateInInventory("Canteen");
		medCase.GetInventory().CreateInInventory("VitaminBottle");
		medCase.GetInventory().CreateInInventory("PurificationTablets");
		
		// Backpack
		WoodenStick stick = WoodenStick.Cast(backpack.GetInventory().CreateInInventory("WoodenStick"));
		stick.SetQuantity(stick.GetQuantityMax(), false);
		OrienteeringCompass compass = OrienteeringCompass.Cast(backpack.GetInventory().CreateInInventory("OrienteeringCompass"));
		backpack.GetInventory().CreateInInventory("Rag");
		backpack.GetInventory().CreateInInventory("Rag");
		backpack.GetInventory().CreateInInventory("Rope");
		backpack.GetInventory().CreateInInventory("Lockpick");
		backpack.GetInventory().CreateInInventory("SewingKit");
		backpack.GetInventory().CreateInInventory("Matchbox");
		backpack.GetInventory().CreateInInventory("SewingKit");
		Rangefinder rangefinder = Rangefinder.Cast(backpack.GetInventory().CreateInInventory("Rangefinder"));
		rangefinder.GetInventory().CreateAttachment("Battery9V");
		backpack.GetInventory().CreateInInventory("Handcuffs");
		backpack.GetInventory().CreateInInventory("HandcuffKeys");
		
		// Shortcuts
		player.SetQuickBarEntityShortcut(izh43, 0);
		player.SetQuickBarEntityShortcut(crossbow, 1);
		player.SetQuickBarEntityShortcut(knife, 2);
		player.SetQuickBarEntityShortcut(grenade, 3);
		player.SetQuickBarEntityShortcut(rangefinder, 4);
		player.SetQuickBarEntityShortcut(bandageDressing, 5);
		player.SetQuickBarEntityShortcut(painkillers, 6);
		player.SetQuickBarEntityShortcut(morphine, 7);
		player.SetQuickBarEntityShortcut(epinephrine, 8);
	}
	
	static void InediaInfectedAI_PlayerTestEquip4(PlayerBase player)
	{
		if (!player) {
			return;
		}
		
		InediaInfectedAI_RemoveAllPlayerItems(player);
		
		DayZPlayerInventory playerInv = DayZPlayerInventory.Cast(player.GetInventory());
		if (!playerInv) {
			return;
		}
		
		// Clothes + attachments
		Sword sword = Sword.Cast(playerInv.CreateInInventory("Sword"));
		playerInv.CreateInInventory("HockeyMask");
		GorkaHelmet helmet = GorkaHelmet.Cast(playerInv.CreateInInventory("GorkaHelmet"));
		helmet.GetInventory().CreateInInventory("GorkaHelmetVisor");
		Chainmail jacket = Chainmail.Cast(playerInv.CreateInInventory("Chainmail"));
		Chainmail_Leggings pants = Chainmail_Leggings.Cast(playerInv.CreateInInventory("Chainmail_Leggings"));
		HipPack_Black hippack = HipPack_Black.Cast(playerInv.CreateInInventory("HipPack_Black"));
		PlateCarrierVest vest = PlateCarrierVest.Cast(playerInv.CreateInInventory("PlateCarrierVest"));
		PlateCarrierHolster pistolHolster = PlateCarrierHolster.Cast(vest.GetInventory().CreateAttachment("PlateCarrierHolster"));
		PlateCarrierPouches pouches = PlateCarrierPouches.Cast(vest.GetInventory().CreateInInventory("PlateCarrierPouches"));
		LeatherSack_Black backpack = LeatherSack_Black.Cast(playerInv.CreateInInventory("LeatherSack_Black"));
		playerInv.CreateInInventory("OMNOGloves_Gray");
		MilitaryBoots_Redpunk boots = MilitaryBoots_Redpunk.Cast(playerInv.CreateInInventory("MilitaryBoots_Redpunk"));
		HuntingKnife knife = HuntingKnife.Cast(boots.GetInventory().CreateAttachment("HuntingKnife"));
		
		
		// Grenades
		RGD5Grenade grenade = RGD5Grenade.Cast(vest.GetInventory().CreateInInventory("RGD5Grenade"));
		for (int grenades = 1; grenades <= 3; grenades++) {
		    vest.GetInventory().CreateInInventory("RGD5Grenade");
		}

		// Hippack
		hippack.GetInventory().CreateInInventory("M18SmokeGrenade_Red");
		hippack.GetInventory().CreateInInventory("M18SmokeGrenade_Green");
		hippack.GetInventory().CreateInInventory("M18SmokeGrenade_White");
		
		// Medicine
		InediaPain_FirstAidCase_Camouflage medCase = InediaPain_FirstAidCase_Camouflage.Cast(hippack.GetInventory().CreateAttachment("InediaPain_FirstAidCase_Camouflage"));
		PainkillerTablets painkillers = PainkillerTablets.Cast(medCase.GetInventory().CreateAttachment("PainkillerTablets"));
		medCase.GetInventory().CreateAttachment("InediaPain_Tramadol");
		medCase.GetInventory().CreateAttachment("InediaPain_Calcium");
		medCase.GetInventory().CreateAttachment("InediaPain_Tranexam");
		medCase.GetInventory().CreateAttachment("InediaPain_Sorbifer");
		medCase.GetInventory().CreateAttachment("InediaPain_Metoclopramide");
		medCase.GetInventory().CreateAttachment("CharcoalTablets");
		medCase.GetInventory().CreateAttachment("TetracyclineAntibiotics");
		medCase.GetInventory().CreateAttachment("InediaPain_SurgicalKit");
		medCase.GetInventory().CreateAttachment("InediaPain_SurgicalKit");
		medCase.GetInventory().CreateAttachment("InediaPain_SplintKit");
		medCase.GetInventory().CreateAttachment("SalineBagIV");
		medCase.GetInventory().CreateAttachment("BloodBagEmpty");
		medCase.GetInventory().CreateAttachment("StartKitIV");
		medCase.GetInventory().CreateAttachment("Rag");
		medCase.GetInventory().CreateAttachment("IodineTincture");
		BandageDressing bandageDressing = BandageDressing.Cast(medCase.GetInventory().CreateAttachment("BandageDressing"));
		medCase.GetInventory().CreateAttachment("BandageDressing");
		medCase.GetInventory().CreateAttachment("InediaPain_HemostaticDressing");
		medCase.GetInventory().CreateAttachment("InediaPain_HemostaticDressing");
		Morphine morphine = Morphine.Cast(medCase.GetInventory().CreateAttachment("Morphine"));
		medCase.GetInventory().CreateAttachment("Morphine");
		Epinephrine epinephrine = Epinephrine.Cast(medCase.GetInventory().CreateAttachment("Epinephrine"));
		medCase.GetInventory().CreateAttachment("AntiChemInjector");
		
		medCase.GetInventory().CreateInInventory("Canteen");
		medCase.GetInventory().CreateInInventory("VitaminBottle");
		medCase.GetInventory().CreateInInventory("PurificationTablets");
				
		// CZ75
		Weapon_Base cz75 = Weapon_Base.Cast(pistolHolster.GetInventory().CreateInInventory("CZ75"));		
		cz75.GetInventory().CreateAttachment("PistolSuppressor");
		FNP45_MRDSOptic pistolOptic = FNP45_MRDSOptic.Cast(cz75.GetInventory().CreateAttachment("FNP45_MRDSOptic"));
		pistolOptic.GetInventory().CreateAttachment("Battery9V");
		cz75.SpawnAmmo("Mag_CZ75_15Rnd");
		for (int i = 1; i <= 15; i++) {
		    pouches.GetInventory().CreateInInventory("Ammo_9x19");
		}
		
		// Jacket + Pants
		//jacket.GetInventory().CreateInInventory("XXX");
		
		// Backpack
		WoodenStick stick = WoodenStick.Cast(backpack.GetInventory().CreateInInventory("WoodenStick"));
		stick.SetQuantity(stick.GetQuantityMax(), false);
		backpack.GetInventory().CreateInInventory("Matchbox");
		backpack.GetInventory().CreateInInventory("Rag");
		backpack.GetInventory().CreateInInventory("Rag");
		backpack.GetInventory().CreateInInventory("Rope");
		OrienteeringCompass compass = OrienteeringCompass.Cast(backpack.GetInventory().CreateInInventory("OrienteeringCompass"));
		backpack.GetInventory().CreateInInventory("SewingKit");
		for (int smokeGrenades = 1; smokeGrenades <= 1; smokeGrenades++) {
		    backpack.GetInventory().CreateInInventory("M18SmokeGrenade_Red");
		}
		backpack.GetInventory().CreateInInventory("Lockpick");
		Rangefinder rangefinder = Rangefinder.Cast(backpack.GetInventory().CreateInInventory("Rangefinder"));
		rangefinder.GetInventory().CreateAttachment("Battery9V");
		backpack.GetInventory().CreateInInventory("Handcuffs");
		backpack.GetInventory().CreateInInventory("HandcuffKeys");
		
		// Shortcuts
		player.SetQuickBarEntityShortcut(sword, 0);
		player.SetQuickBarEntityShortcut(cz75, 1);
		player.SetQuickBarEntityShortcut(knife, 2);
		player.SetQuickBarEntityShortcut(grenade, 3);
		player.SetQuickBarEntityShortcut(rangefinder, 4);
		player.SetQuickBarEntityShortcut(bandageDressing, 5);
		player.SetQuickBarEntityShortcut(painkillers, 6);
		player.SetQuickBarEntityShortcut(morphine, 7);
		player.SetQuickBarEntityShortcut(epinephrine, 8);
	}
	
	static void InediaInfectedAI_PlayerTestEquip5(PlayerBase player)
	{
		if (!player) {
			return;
		}
		
		InediaInfectedAI_RemoveAllPlayerItems(player);
		
		DayZPlayerInventory playerInv = DayZPlayerInventory.Cast(player.GetInventory());
		if (!playerInv) {
			return;
		}
		
		// Clothes + attachments
		playerInv.CreateInInventory("HighCapacityVest_Black");
		M65Jacket_Black jacket = M65Jacket_Black.Cast(playerInv.CreateInInventory("M65Jacket_Black"));
		CargoPants_Black pants = CargoPants_Black.Cast(playerInv.CreateInInventory("CargoPants_Black"));
		HipPack_Black hippack = HipPack_Black.Cast(playerInv.CreateInInventory("HipPack_Black"));
		AliceBag_Black backpack = AliceBag_Black.Cast(playerInv.CreateInInventory("AliceBag_Black"));
		playerInv.CreateInInventory("BalaclavaMask_Black");
		playerInv.CreateInInventory("ThinFramesGlasses");
		playerInv.CreateInInventory("WorkingGloves_Black");
		MilitaryBoots_Black boots = MilitaryBoots_Black.Cast(playerInv.CreateInInventory("MilitaryBoots_Black"));
		HuntingKnife knife = HuntingKnife.Cast(boots.GetInventory().CreateAttachment("HuntingKnife"));
		playerInv.CreateInInventory("RadarCap_Black");
		
		// Backpack
		playerInv.CreateInInventory("TruckBattery");
		playerInv.CreateInInventory("CarBattery");
		playerInv.CreateInInventory("CarBattery");
		playerInv.CreateInInventory("TruckBattery");
		playerInv.CreateInInventory("CarBattery");
		playerInv.CreateInInventory("CarBattery");
		playerInv.CreateInInventory("CarBattery");
		playerInv.CreateInInventory("CarBattery");
		
		// Shortcuts
		player.SetQuickBarEntityShortcut(knife, 2);
	}
	
	static void InediaInfectedAI_RemoveAllPlayerItems(PlayerBase player)
	{
		if (!player) {
			return;
		}
		
		array<EntityAI> itemsArray = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, itemsArray);
		
		ItemBase item;
		InventoryLocation itemInventoryLocation = new InventoryLocation();
		
		for (int i = 0; i < itemsArray.Count(); i++) {
			Class.CastTo(item, itemsArray.Get(i));
			if (item && !item.IsInherited(SurvivorBase)) {
				if (item.GetInventory().GetCurrentInventoryLocation(itemInventoryLocation)) {
					player.GetInventory().LocalDestroyEntity(item);
				}
			}
		}
	}
}