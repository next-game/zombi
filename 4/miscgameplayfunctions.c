/**
 * Created: 2023-09-01
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

modded class MiscGameplayFunctions
{
	static bool InediaInfectedAI_IsUnderRoofBuilding(vector position, float raycastRadius = 0)
	{
		return InediaInfectedAI_IsBuildingOnLine(position, position + 25 * vector.Up, raycastRadius);
	}

	static bool InediaInfectedAI_IsBuildingOnLine(vector fromPosition, vector toPosition, float raycastRadius = 0)
	{
		float hitFraction;
		vector hitPosition, hitNormal;
		Object hitObject;
		PhxInteractionLayers collisionLayerMask = PhxInteractionLayers.ITEM_LARGE|PhxInteractionLayers.BUILDING|PhxInteractionLayers.FENCE;
		
		if (raycastRadius > 0) {
			DayZPhysics.SphereCastBullet(fromPosition, toPosition, raycastRadius, collisionLayerMask, null, hitObject, hitPosition, hitNormal, hitFraction);
		} else {
			DayZPhysics.RayCastBullet(fromPosition, toPosition, collisionLayerMask, null, hitObject, hitPosition, hitNormal, hitFraction);
		}

		return (hitObject && (hitObject.IsInherited(House) || hitObject.IsInherited(TentBase) || hitObject.IsInherited(BaseBuildingBase)));
	}
	
	static bool InediaInfectedAI_IsObjectOrNothingOnLine(vector fromPosition, vector toPosition, Object specObject = null, Object ignoreObject = null, float raycastRadius = 0, int intersectType = ObjIntersectFire /* ObjIntersectFire OR ObjIntersectView */)
	{
		return InediaInfectedAI_IsObjectsOrNothingOnLine(fromPosition, toPosition, {specObject}, ignoreObject, raycastRadius, intersectType);
	}
	
	static bool InediaInfectedAI_IsObjectsOrNothingOnLine(vector fromPosition, vector toPosition, array<Object> specObjects = null, Object ignoreObject = null, float raycastRadius = 0, int intersectType = ObjIntersectFire /* ObjIntersectFire OR ObjIntersectView */)
	{
		if (!fromPosition || !toPosition) {
			return false;
		}
		
		vector contact_pos, contact_dir;
        int contact_component;
        set<Object> objects = new set<Object>;
		
		if (!DayZPhysics.RaycastRV(fromPosition, toPosition, contact_pos, contact_dir, contact_component, objects, null, ignoreObject, false, false, intersectType, raycastRadius, CollisionFlags.NEARESTCONTACT)) {
			return true;
		} else if (objects.Count() == 0) {
			return false;
		}
		
		Object obj = objects.Get(0);
		
		if (obj && specObjects && specObjects.Count() > 0) {
			foreach (Object specObject : specObjects) {
				if (specObject && obj == specObject) {
					return true;
				}
			}
		}

		return false;
	}
	
	static bool InediaInfectedAI_IsRaycastHit(vector fromPosition, vector toPosition, Object ignoreObject = null, float raycastRadius = 0, int intersectType = ObjIntersectFire /* ObjIntersectFire OR ObjIntersectView */)
	{
		if (!fromPosition || !toPosition) {
			return false;
		}
		
		vector contact_pos, contact_dir;
        int contact_component;
		
		return DayZPhysics.RaycastRV(fromPosition, toPosition, contact_pos, contact_dir, contact_component, null, null, ignoreObject, false, false, intersectType, raycastRadius, CollisionFlags.NEARESTCONTACT);
	}
	
	static vector InediaInfectedAI_GetRaycastHitPosition(vector fromPosition, vector toPosition, Object ignoreObject = null, float raycastRadius = 0, int intersectType = ObjIntersectFire /* ObjIntersectFire OR ObjIntersectView */)
	{
		if (!fromPosition || !toPosition) {
			return vector.Zero;
		}
		
		vector contact_pos, contact_dir;
        int contact_component;
		
		if (DayZPhysics.RaycastRV(fromPosition, toPosition, contact_pos, contact_dir, contact_component, null, null, ignoreObject, false, false, intersectType, raycastRadius, CollisionFlags.NEARESTCONTACT)) {
			return contact_pos;
		}
		
		return vector.Zero;
	}
	
	static Object InediaInfectedAI_GetRaycastHitObject(vector fromPosition, vector toPosition, Object ignoreObject = null, float raycastRadius = 0, int intersectType = ObjIntersectFire /* ObjIntersectFire OR ObjIntersectView */)
	{
		if (!fromPosition || !toPosition) {
			return null;
		}
		
		vector contact_pos, contact_dir;
        int contact_component;
        set<Object> objects = new set<Object>;
		
		if (DayZPhysics.RaycastRV(fromPosition, toPosition, contact_pos, contact_dir, contact_component, objects, null, ignoreObject, false, false, intersectType, raycastRadius, CollisionFlags.NEARESTCONTACT)) {
			if (objects.Count() > 0) {
				return objects.Get(0);
			}
		}
		
		return null;
	}

	static bool InediaInfectedAI_OnObstacle(vector position, float minObstacleHeight = 0.5, float maxObstacleHeight = 100)
	{
		if (!position) {
			return false;
		}

		float positionSurfaceY = GetGame().SurfaceY(position[0], position[2]);
		float currentObstacleHeight = position[1] - positionSurfaceY;
		if (currentObstacleHeight < 0) {
			currentObstacleHeight = 0;
		}

		return currentObstacleHeight >= minObstacleHeight && currentObstacleHeight <= maxObstacleHeight;
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

	static float InediaInfectedAI_GetGlobalSoundMultiplier(vector soundPosition)
	{
		return MiscGameplayFunctions.InediaInfectedAI_GetHouseSoundMultiplier(soundPosition) * MiscGameplayFunctions.InediaInfectedAI_GetWeatherSoundMultiplier();
	}

	static float InediaInfectedAI_GetHouseSoundMultiplier(vector soundPosition)
	{
		if (!soundPosition) {
			return 1;
		}

		float multiplier = 1;

		if (MiscGameplayFunctions.InediaInfectedAI_IsUnderRoofBuilding(soundPosition + vector.Up * 0.1)) {
			multiplier *= InediaInfectedAI_Config.GetInstance().SoundInHouseMultiplier;
		}

		return multiplier;
	}
	
	static float InediaInfectedAI_GetWeatherSoundMultiplier()
	{
		float rainMultiplier = 1 - 0.6 * GetGame().GetWeather().GetRain().GetActual();
		
		float windMagnitude = GetGame().GetWeather().GetWindMagnitude().GetActual();
		float snowfallMultiplierWeight = 0.2;
		if (windMagnitude >= 15) {
			snowfallMultiplierWeight = 0.6;
		} else if (windMagnitude >= 10) {
			snowfallMultiplierWeight = 0.4;
		}
		
		float snowfallMultiplier = 1 - snowfallMultiplierWeight * GetGame().GetWeather().GetSnowfall().GetActual();
		
		return rainMultiplier * snowfallMultiplier;
	}

	static float InediaInfectedAI_GetGlobalVisibilityMultiplier(vector lightPosition)
	{
		return MiscGameplayFunctions.InediaInfectedAI_GetHouseVisibilityMultiplier(lightPosition) * MiscGameplayFunctions.InediaInfectedAI_GetWeatherVisibilityMultiplier();
	}

	static float InediaInfectedAI_GetHouseVisibilityMultiplier(vector lightPosition)
	{
		if (!lightPosition) {
			return 1;
		}

		float multiplier = 1;

		if (MiscGameplayFunctions.InediaInfectedAI_IsUnderRoofBuilding(lightPosition + vector.Up * 0.1)) {
			multiplier *= InediaInfectedAI_Config.GetInstance().LightInHouseMultiplier;
		}

		return multiplier;
	}
	
	static float InediaInfectedAI_GetWeatherVisibilityMultiplier()
	{
		float rainMultiplier = 1 - 0.3 * GetGame().GetWeather().GetRain().GetActual();
		float snowfallMultiplier = 1 - 0.6 * GetGame().GetWeather().GetSnowfall().GetActual();
		
		return rainMultiplier * snowfallMultiplier;
	}
	
	static float InediaInfectedAI_GetFogVisibilityDistance()
	{
		float fogLevel = GetGame().GetWeather().GetFog().GetActual();
		float distance = 1000000;
		
		if (fogLevel >= 0.992) {
			distance = 40;
		} else if (fogLevel >= 0.981) {
			distance = 50;
		} else if (fogLevel >= 0.965) {
			distance = 60;
		} else if (fogLevel >= 0.956) {
			distance = 70;
		} else if (fogLevel >= 0.945) {
			distance = 80;
		} else if (fogLevel >= 0.933) {
			distance = 90;
		} else if (fogLevel >= 0.922) {
			distance = 100;
		} else if (fogLevel >= 0.905) {
			distance = 110;
		} else if (fogLevel >= 0.87) {
			distance = 120;
		} else if (fogLevel >= 0.75) {
			distance = 200;
		} else if (fogLevel >= 0.65) {
			distance = 300;
		} else if (fogLevel >= 0.50) {
			distance = 400;
		} else if (fogLevel >= 0.40) {
			distance = 500;
		} else if (fogLevel >= 0.35) {
			distance = 600;
		} else if (fogLevel >= 0.2) {
			distance = 1000;
		}
		
		return distance;
	}

// vn7W87PC0Z3X7zLeW6BnBlpJTjZhbzF4SVpVdnBLYVc0UTJiM2ZBYThKQWs2c2YwY0dCQ1U2UDBQRmFKY01RT2dmSEpUMzBXTkgrbHE4ZUtGbmwrQlJMRVNORE01cnJEblh1Z3I2NkdKMVhaZkFvZHd1Q2tIRlh5a3hrOUl6YnZPVkx4WlNIdTZOQTA4WEFYeUFRZzRQTjdyNktVTkJNNWRCOVIrVFFFZjFHcWttQTQrVmwvSGNhblArcGNOa1MvbnFaL2tJdldvdldkL3JSb0ZDQzdEaklvbDBtaXNvN2pWWXVwRWNqaHdOblNqeENnU04weTdYb2g4NU9UVFBrUlgya0FudDFpeFpzRDJDcXdESjlXNk5JQ0ZWM2xkNG85T0dhVjlnPT0=

	static void InediaInfectedAI_UpdatePathgraphRegionByPosition(vector position, float radius)
	{
		if (!position || radius <= 0) {
			return;
		}

		vector min = Vector (position[0] - radius, position[1], position[2] - radius);
		vector max = Vector (position[0] + radius, position[1], position[2] + radius);
		GetGame().UpdatePathgraphRegion(min, max);
	}

	static ref NoiseParams m_InediaInfectedAI_NoiseParams;
	static void InediaInfectedAI_MakeNoise(vector position, float multiplier = 3, float lifetime = 1)
	{
		if (!m_InediaInfectedAI_NoiseParams) {
			m_InediaInfectedAI_NoiseParams = new NoiseParams();
			m_InediaInfectedAI_NoiseParams.LoadFromPath("CfgSoundSets inedia_infectedai_default_sound_noise");
		}

		GetGame().GetNoiseSystem().AddNoiseTarget(position, lifetime, m_InediaInfectedAI_NoiseParams, multiplier);
	}
	
	/*
		This method is used to irritate an infected, bringing them out of the CALM state.
		It triggers only once for multiple zombies within a certain area to improve performance, as the sound-generation procedure is very resource-intensive.
	*/
	static void InediaInfectedAI_MakeNoiseForZombie(ZombieBase zombie)
	{
		if (!zombie || !zombie.IsAlive() || (GetGame().GetTickTime() - zombie.m_InediaInfectedAI_LastNoiseIrritationTime) < 3) {
			return;
		}
		
		MiscGameplayFunctions.InediaInfectedAI_MakeNoise(zombie.GetPosition());
		
		// All zombies within a 10-meter radius are forbidden from generating noise for 3 seconds.
		array<ZombieBase> nearZombies = new array<ZombieBase>;
		MiscGameplayFunctions.InediaInfectedAI_GetZombiesAtPosition3D(zombie.GetPosition(), 10, nearZombies);
		foreach (ZombieBase zmb : nearZombies) {
			if (zmb && zmb.IsAlive()) {
				zmb.m_InediaInfectedAI_LastNoiseIrritationTime = GetGame().GetTickTime();
			}
		}
	}

	static float InediaInfectedAI_CalculateThrowAngle(vector fromPosition, vector toPosition, float initialSpeed = 20)
	{
		if (!fromPosition || !toPosition || initialSpeed <= 0) {
			return -1;
		}

		float g = 9.80665;

		float y = toPosition[1] - fromPosition[1];
		float x = vector.Distance(fromPosition, Vector(toPosition[0], fromPosition[1], toPosition[2]));

		if (x == 0) {
			// Prevent division by zero
			return -90;
		}

		// Prevent taking the square root of a negative number.
		// This happens when the projectile's speed is insufficient to reach the target.
		float underSqrtValue = Math.Pow(initialSpeed, 4) - g * (g * Math.Pow(x, 2) + 2 * Math.Pow(initialSpeed, 2) * y);
		if (underSqrtValue < 0) {
			return -90;
		}

		float angle1 = Math.Atan((Math.Pow(initialSpeed, 2) + Math.Sqrt(underSqrtValue)) / (g * x)) * Math.RAD2DEG;
		float angle2 = Math.Atan((Math.Pow(initialSpeed, 2) - Math.Sqrt(underSqrtValue)) / (g * x)) * Math.RAD2DEG;

		return Math.Min(angle1, angle2);
	}
	
	static vector InediaInfectedAI_GetCreatureHeadPosition(EntityAI creature)
	{
		if (!creature) {
			return vector.Zero;
		}
		
		if (creature.IsInherited(PlayerBase)) {
			PlayerBase player = PlayerBase.Cast(creature);
			if (player) {
				return player.InediaInfectedAI_GetHeadPosition();
			}
		} else if (creature.IsInherited(AnimalBase)) {
			AnimalBase animal = AnimalBase.Cast(creature);
			if (animal) {
				return animal.InediaInfectedAI_GetHeadPosition();
			}
		} else if (creature.IsInherited(CarScript)) {
			CarScript vehicle = CarScript.Cast(creature);
			if (vehicle) {
				return vehicle.InediaInfectedAI_GetPlayerHeadPosition();
			}
		}
		
		return vector.Zero;
	}
	
	static void InediaInfectedAI_GetEntitiesAtPosition3D(vector position, float radius, out array<EntityAI> entities)
	{
		if (!entities) {
			return;
		}
		
		entities.Clear();

		vector minCorner = position - Vector(radius, radius, radius);
		vector maxCorner = position + Vector(radius, radius, radius);
		array<EntityAI> nearbyEntities = new array<EntityAI>;
		
		DayZPlayerUtils.SceneGetEntitiesInBox(minCorner, maxCorner, nearbyEntities);
		if (nearbyEntities.Count() > 0) {
		    foreach (EntityAI nearEntity : nearbyEntities) {
		        if (vector.DistanceSq(position, nearEntity.GetPosition()) > radius * radius) {
					continue;
				}
				
		        entities.Insert(nearEntity);
		    }
		}
	}
	
	static void InediaInfectedAI_GetZombiesAtPosition3D(vector position, float radius, out array<ZombieBase> zombies)
	{
		if (!zombies) {
			return;
		}

		zombies.Clear();
		
		array<EntityAI> nearbyEntities = new array<EntityAI>;
		InediaInfectedAI_GetEntitiesAtPosition3D(position, radius, nearbyEntities);

	    foreach (EntityAI nearEntity : nearbyEntities) {
	        if (!nearEntity.IsInherited(ZombieBase)) {
				continue;
			}
			
	        ZombieBase zmb = ZombieBase.Cast(nearEntity);
	        if (zmb) {
				zombies.Insert(zmb);
	        }
	    }
	}
	
	static void InediaInfectedAI_GetPlayersAtPosition3D(vector position, float radius, out array<PlayerBase> players)
	{
	    if (!players) {
	        return;
	    }
	
	    players.Clear();
	    
	    array<EntityAI> nearbyEntities = new array<EntityAI>;
	    InediaInfectedAI_GetEntitiesAtPosition3D(position, radius, nearbyEntities);
	
	    foreach (EntityAI nearEntity : nearbyEntities) {
	        if (!nearEntity.IsInherited(PlayerBase)) {
	            continue;
	        }
	
	        PlayerBase player = PlayerBase.Cast(nearEntity);
	        if (player) {
	            players.Insert(player);
	        }
	    }
	}
	
	static void InediaInfectedAI_GetVehiclesAtPosition3D(vector position, float radius, out array<CarScript> vehicles)
	{
	    if (!vehicles) {
	        return;
	    }
	
	    vehicles.Clear();
	    
	    array<EntityAI> nearbyEntities = new array<EntityAI>;
	    InediaInfectedAI_GetEntitiesAtPosition3D(position, radius, nearbyEntities);
	
	    foreach (EntityAI nearEntity : nearbyEntities) {
	        if (!nearEntity.IsInherited(CarScript)) {
	            continue;
	        }
	
	        CarScript vehicle = CarScript.Cast(nearEntity);
	        if (vehicle) {
	            vehicles.Insert(vehicle);
	        }
	    }
	}

	static void InediaInfectedAI_GetItemsAtPosition3D(vector position, float radius, out array<ItemBase> items)
	{
	    if (!items) {
	        return;
	    }
	
	    items.Clear();
	    
	    array<EntityAI> nearbyEntities = new array<EntityAI>;
	    InediaInfectedAI_GetEntitiesAtPosition3D(position, radius, nearbyEntities);
	
	    foreach (EntityAI nearEntity : nearbyEntities) {
	        if (!nearEntity.IsInherited(ItemBase)) {
	            continue;
	        }
	
	        ItemBase item = ItemBase.Cast(nearEntity);
	        if (item) {
	            items.Insert(item);
	        }
	    }
	}
	
	static void InediaInfectedAI_DmgDebug(string target, string source, string ammo, string dmgZone, float damage, float shockDamage, float remainingHP)
	{
		if (!InediaInfectedAI_Debug.IsDebugRuleEnabled("dmg")) {
			return;
		}
		
		if (remainingHP < 0) {
			remainingHP = 0;
		}
		
		int hour;
		int minute;
		int second;
		GetHourMinuteSecond(hour, minute, second);
		
		string msg;
		
		if (shockDamage != -1) {
		    msg = string.Format("Src: \"%1\" | Ammo: \"%2\" | Tgt: \"%3\" | Zone: \"%4\" | Dmg: \"%5\" | Shock: \"%6\" | RHP: \"%7\"\n", 
		                        source, ammo, target, dmgZone, Math.Round(damage * 10) / 10, Math.Round(shockDamage * 10) / 10, Math.Round(remainingHP * 10) / 10);
		} else {
		    msg = string.Format("Src: \"%1\" | Ammo: \"%2\" | Tgt: \"%3\" | Zone: \"%4\" | Dmg: \"%5\" | RHP: \"%6\"\n", 
		                        source, ammo, target, dmgZone, Math.Round(damage * 10) / 10, Math.Round(remainingHP * 10) / 10);
		}
		
		InediaInfectedAI_SendChatMessageToAllAdmins(msg);
		
		InediaInfectedAI_Debug.Log(msg, "dmg");
	}
	
	static void InediaInfectedAI_SendChatMessageToAllAdmins(string msg)
	{
		if (!msg || msg == "") {
			return;
		}
		
		array<Man> players();
		GetGame().GetPlayers(players);
		
		if (players) {
			foreach (Man player : players) {
				PlayerBase pbPlayer = PlayerBase.Cast(player);
				if (pbPlayer && pbPlayer.InediaInfectedAI_IsAdmin()) {
					pbPlayer.InediaInfectedAI_SendActionMessage(msg);
				}
			}
		}
	}
	

	/*
		The method splatters blood from the position "startPosition" in the direction and at the distance "directionSpeed",
		with a damage power "power" ranging from 1 to 5.
		"hasExited" is true if it is splatter from a bullet that has exited, otherwise false.
	*/
	static void InediaInfectedAI_ThrowBloodSplatFromPosition(vector startPosition, vector directionSpeed, int power, bool hasExited, Object excludeObject = null, int birthRateLimit = 3)
	{
#ifndef SERVER
		if (!startPosition || !directionSpeed) {
			return;
		}
		
		PlayerBase currentPlayer = PlayerBase.Cast(GetGame().GetPlayer());
		if (!currentPlayer || currentPlayer.m_InediaInfectedAI_BloodParticleDurationSeconds <= 0) {
			return;
		}
		
		power = Math.Clamp(power, 1, 5);
		birthRateLimit = Math.Clamp(birthRateLimit, 1, 3);
		
		//Debug.DrawArrow(startPosition, startPosition + directionSpeed, 0.05);
		
		Object projected_surface;
		vector hit_pos;
		vector hit_normal;
		float hit_fraction;
		
		bool isLandedOnGround = false; // Flag indicating whether the blood splatter hit the ground or collided with an obstacle.
		
		if (!DayZPhysics.RayCastBullet(startPosition, startPosition + directionSpeed, PhxInteractionLayers.BUILDING | PhxInteractionLayers.TERRAIN, excludeObject, projected_surface, hit_pos, hit_normal, hit_fraction)) {
			// Since the splashes didn't reach an obstacle, they fall to the ground within a radius of 2-3 meters from the point of exit.
			float rand23 = Math.RandomFloatInclusive(2, 3);
			vector downCastFrom = startPosition + directionSpeed.Normalized() * rand23;
			if (DayZPhysics.RayCastBullet(downCastFrom, downCastFrom - 10 * vector.Up, PhxInteractionLayers.BUILDING | PhxInteractionLayers.TERRAIN, excludeObject, projected_surface, hit_pos, hit_normal, hit_fraction)) {
				isLandedOnGround = true;
			}
		}
		
		if (!hit_pos) {
			return;
		}
		
		float maxSplatRadius = 0;
		float splatBirthRate = 0;
		float dropsSize = 0;
		float dropsBirthRate = 0;
		float chunksSize = 0;
		float chunksBirthRate = 0;
		float drippingChunksSize = 0;
		float drippingChunksBirthRate = 0;

		switch (power) {
			case 1:
				maxSplatRadius = 0.3;
				splatBirthRate = 1;
				dropsSize = 0.03;
				dropsBirthRate = 3;
				chunksSize = 0.001;
				chunksBirthRate = 3;
				drippingChunksSize = 0.0006;
				drippingChunksBirthRate = 4;
			break;
		
			case 2:
				maxSplatRadius = 0.4;
				splatBirthRate = 1;
				dropsSize = 0.05;
				dropsBirthRate = 3;
				chunksSize = 0.0025;
				chunksBirthRate = 3;
				drippingChunksSize = 0.00125;
				drippingChunksBirthRate = 4;
			break;
		
			case 3:
				maxSplatRadius = 0.5;
				splatBirthRate = 1;
				dropsSize = 0.07;
				dropsBirthRate = 3;
				chunksSize = 0.005;
				chunksBirthRate = 3;
				drippingChunksSize = 0.0025;
				drippingChunksBirthRate = 4;
			break;
		
			case 4:
				maxSplatRadius = 0.6;
				splatBirthRate = 2;
				dropsSize = 0.09;
				dropsBirthRate = 3;
				chunksSize = 0.015;
				chunksBirthRate = 3;
				drippingChunksSize = 0.0075;
				drippingChunksBirthRate = 4;
			break;
			
			case 5:
				maxSplatRadius = 0.7;
				splatBirthRate = 2;
				dropsSize = 0.11;
				dropsBirthRate = 3;
				chunksSize = 0.02;
				chunksBirthRate = 3;
				drippingChunksSize = 0.01;
				drippingChunksBirthRate = 4;
			break;
		}
		
		splatBirthRate = Math.Clamp(splatBirthRate, 1, birthRateLimit);
		dropsBirthRate = Math.Clamp(dropsBirthRate, 1, birthRateLimit);
		chunksBirthRate = Math.Clamp(chunksBirthRate, 1, birthRateLimit);
		//drippingChunksBirthRate = Math.Clamp(drippingChunksBirthRate, 1, birthRateLimit);
		
		maxSplatRadius = Math.RandomFloatInclusive(0.9 * maxSplatRadius, 1.1 * maxSplatRadius);
		
		vector hitNormal = hit_normal.Normalized();
		vector particleOrientation = hitNormal.VectorToAngles();
		particleOrientation[1] = particleOrientation[1] + 90;
		
		vector correctedHitPos;
		float correctedSplatRadius;
		InediaInfectedAI_CorrectParticleForCurvedSurface(correctedHitPos, correctedSplatRadius, hit_pos, hitNormal, maxSplatRadius, excludeObject);
		
		if (!correctedHitPos || correctedSplatRadius <= 0) {
			return;
		}
		
		vector particlesPos = correctedHitPos + hitNormal * 0.05;
		
		if (correctedSplatRadius > 0) {
			Particle splat = ParticleManager.GetInstance().PlayInWorld(ParticleList.INEDIAINFECTEDAI_BLOOD_SPLATTER, particlesPos);
			splat.SetOrientation(particleOrientation);
			splat.SetParticleParam(EmitorParam.SIZE, correctedSplatRadius);
			splat.SetParticleParam(EmitorParam.BIRTH_RATE, splatBirthRate);
			splat.SetParticleParam(EmitorParam.LIFETIME, currentPlayer.m_InediaInfectedAI_BloodParticleDurationSeconds * 2/3);
			splat.SetParticleParam(EmitorParam.LIFETIME_RND, currentPlayer.m_InediaInfectedAI_BloodParticleDurationSeconds * 1/3);
			currentPlayer.InediaInfectedAI_AddBloodParticleClient(splat);
		}
			
		if (correctedSplatRadius >= 0.5) {
			if (chunksSize > 0 && Math.RandomBool()) {
				Particle chunks = ParticleManager.GetInstance().PlayInWorld(ParticleList.INEDIAINFECTEDAI_BLOOD_CHUNKS, particlesPos);
				chunks.SetOrientation(particleOrientation);
				chunks.SetParticleParam(EmitorParam.SIZE, Math.RandomFloatInclusive(0.9 * chunksSize, 1.1 * chunksSize));
				chunks.SetParticleParam(EmitorParam.RANDOM_ROT, 1);
				chunks.SetParticleParam(EmitorParam.BIRTH_RATE_RND, chunksBirthRate);
				chunks.SetParticleParam(EmitorParam.LIFETIME, currentPlayer.m_InediaInfectedAI_BloodParticleDurationSeconds * 2/3);
				chunks.SetParticleParam(EmitorParam.LIFETIME_RND, currentPlayer.m_InediaInfectedAI_BloodParticleDurationSeconds * 1/3);
				currentPlayer.InediaInfectedAI_AddBloodParticleClient(chunks);
			}
			
			if (dropsSize > 0 && Math.RandomBool()) {
				Particle drops = ParticleManager.GetInstance().PlayInWorld(ParticleList.INEDIAINFECTEDAI_BLOOD_DROPS, particlesPos);
				drops.SetOrientation(particleOrientation);
				drops.SetParticleParam(EmitorParam.SIZE, Math.RandomFloatInclusive(0.9 * dropsSize, 1.1 * dropsSize));
				drops.SetParticleParam(EmitorParam.RANDOM_ROT, 1);
				drops.SetParticleParam(EmitorParam.BIRTH_RATE_RND, dropsBirthRate);
				drops.SetParticleParam(EmitorParam.LIFETIME, currentPlayer.m_InediaInfectedAI_BloodParticleDurationSeconds * 2/3);
				drops.SetParticleParam(EmitorParam.LIFETIME_RND, currentPlayer.m_InediaInfectedAI_BloodParticleDurationSeconds * 1/3);
				currentPlayer.InediaInfectedAI_AddBloodParticleClient(drops);
			}
			
			// Dripping particles are possible only when the normal vector deviation from the horizon plane is no more than 10 degrees.
			float verticalAngleDeviation = Math.AbsFloat(InediaInfectedAI_DegAngleBetweenVectors(hitNormal, vector.Up));
			if (drippingChunksSize > 0 && Math.RandomBool() && verticalAngleDeviation >= 85 && verticalAngleDeviation <= 95) {
				Particle drippingChunks = ParticleManager.GetInstance().PlayInWorld(ParticleList.INEDIAINFECTEDAI_BLOOD_CHUNKS, particlesPos);
				drippingChunks.SetOrientation(particleOrientation);
				drippingChunks.SetParticleParam(EmitorParam.SIZE, Math.RandomFloatInclusive(0.9 * drippingChunksSize, 1.1 * drippingChunksSize));
				drippingChunks.SetParticleParam(EmitorParam.RANDOM_ROT, 1);
				drippingChunks.SetParticleParam(EmitorParam.BIRTH_RATE_RND, drippingChunksBirthRate);
				drippingChunks.SetParticleParam(EmitorParam.GRAVITY_SCALE_RND, 0.0004);
				drippingChunks.SetParticleParam(EmitorParam.LIFETIME, 20);
				drippingChunks.SetParticleParam(EmitorParam.LIFETIME_RND, 10);
			}
		}
		
		//Debug.DrawArrow(startPosition, particlesPos, 0.05, 0x42AAFF);
#endif
	}
	
	/*
		The method leaves a blood trail on the ground at "position" position.
		"bloodLossLevel" - blood loss level, integer from 1 to 5.
	*/
	static void InediaInfectedAI_CreateBloodTrailInPosition(vector position, int bloodLossLevel, Object excludeObject = null, int birthRateLimit = 3)
	{
#ifndef SERVER
		if (!position || bloodLossLevel <= 0) {
			return;
		}
		
		PlayerBase currentPlayer = PlayerBase.Cast(GetGame().GetPlayer());
		if (!currentPlayer || currentPlayer.m_InediaInfectedAI_BloodParticleDurationSeconds <= 0) {
			return;
		}
		
		bloodLossLevel = Math.Clamp(bloodLossLevel, 1, 5);
		birthRateLimit = Math.Clamp(birthRateLimit, 1, 3);
		
		Object projected_surface;
		vector hit_pos;
		vector hit_normal;
		float hit_fraction;
		
		DayZPhysics.RayCastBullet(position + vector.Up * 0.1, position + vector.Up * 0.1 - vector.Up * 10.1, PhxInteractionLayers.BUILDING | PhxInteractionLayers.TERRAIN, excludeObject, projected_surface, hit_pos, hit_normal, hit_fraction);
		
		if (!hit_pos) {
			return;
		}
		
		float dropsSize;
		float dropsBirthRate;
		float splatSize;
		float splatBirthRate;
		if (bloodLossLevel == 5) {
			dropsSize = 0.12;
			dropsBirthRate = 2;
			splatSize = 0.7;
			splatBirthRate = 1;
		} else if (bloodLossLevel == 4) {
			dropsSize = 0.10;
			dropsBirthRate = 2;
			splatSize = 0.6;
			splatBirthRate = 1;
		} else if (bloodLossLevel == 3) {
			dropsSize = 0.08;
			dropsBirthRate = 1;
			splatSize = 0.5;
			splatBirthRate = 1;
		} else if (bloodLossLevel == 2) {
			dropsSize = 0.06;
			dropsBirthRate = 1;
			splatSize = 0.4;
			splatBirthRate = 1;
		} else if (bloodLossLevel == 1) {
			dropsSize = 0.04;
			dropsBirthRate = 1;
			splatSize = 0.3;
			splatBirthRate = 1;
		}
		
		dropsBirthRate = Math.Clamp(dropsBirthRate, 1, birthRateLimit);
		splatBirthRate = Math.Clamp(splatBirthRate, 1, birthRateLimit);
		
		vector hitNormal = hit_normal.Normalized();
		
		vector particleOrientation = hitNormal.VectorToAngles();
		particleOrientation[1] = particleOrientation[1] + 90;
		
		vector correctedHitPos;
		float correctedRadius;
		InediaInfectedAI_CorrectParticleForCurvedSurface(correctedHitPos, correctedRadius, hit_pos, hitNormal, 1, excludeObject);
		
		if (!correctedHitPos || correctedRadius < 0.5) {
			return;
		}
		
		vector particlesPos = correctedHitPos + hitNormal * 0.05;
		
		Particle drops = ParticleManager.GetInstance().PlayInWorld(ParticleList.INEDIAINFECTEDAI_BLOOD_DROPS, particlesPos);
		drops.SetOrientation(particleOrientation);
		drops.SetParticleParam(EmitorParam.SIZE, Math.RandomFloatInclusive(0.9 * dropsSize, 1.1 * dropsSize));
		drops.SetParticleParam(EmitorParam.RANDOM_ROT, 1);
		drops.SetParticleParam(EmitorParam.BIRTH_RATE_RND, dropsBirthRate);
		drops.SetParticleParam(EmitorParam.LIFETIME, currentPlayer.m_InediaInfectedAI_BloodParticleDurationSeconds * 2/3);
		drops.SetParticleParam(EmitorParam.LIFETIME_RND, currentPlayer.m_InediaInfectedAI_BloodParticleDurationSeconds * 1/3);
		currentPlayer.InediaInfectedAI_AddBloodParticleClient(drops);
		
		if (Math.RandomFloatInclusive(0, 100) <= 20) {
			Particle splat = ParticleManager.GetInstance().PlayInWorld(ParticleList.INEDIAINFECTEDAI_BLOOD_SPLATTER, particlesPos);
			splat.SetOrientation(particleOrientation);
			splat.SetParticleParam(EmitorParam.SIZE, Math.RandomFloatInclusive(0.8 * splatSize, 1.2 * splatSize));
			splat.SetParticleParam(EmitorParam.RANDOM_ROT, 1);
			splat.SetParticleParam(EmitorParam.BIRTH_RATE, splatBirthRate);
			splat.SetParticleParam(EmitorParam.LIFETIME, currentPlayer.m_InediaInfectedAI_BloodParticleDurationSeconds * 2/3);
			splat.SetParticleParam(EmitorParam.LIFETIME_RND, currentPlayer.m_InediaInfectedAI_BloodParticleDurationSeconds * 1/3);
			currentPlayer.InediaInfectedAI_AddBloodParticleClient(splat);
		}
#endif
	}
	
	/*
		The method leaves a blood pool on the ground at "position" position.
		"bloodLossLevel" - blood loss level, integer from 1 to 5.
	*/
	static void InediaInfectedAI_CreateBloodPoolInPosition(vector position, int bloodLossLevel, Object excludeObject = null)
	{
#ifndef SERVER
		if (!position || bloodLossLevel <= 0) {
			return;
		}
		
		PlayerBase currentPlayer = PlayerBase.Cast(GetGame().GetPlayer());
		if (!currentPlayer || currentPlayer.m_InediaInfectedAI_BloodParticleDurationSeconds <= 0) {
			return;
		}
		
		bloodLossLevel = Math.Clamp(bloodLossLevel, 1, 5);
		
		Object projected_surface;
		vector hit_pos;
		vector hit_normal;
		float hit_fraction;
		
		DayZPhysics.RayCastBullet(position + vector.Up * 0.1, position + vector.Up * 0.1 - vector.Up * 10.1, PhxInteractionLayers.BUILDING | PhxInteractionLayers.TERRAIN, excludeObject, projected_surface, hit_pos, hit_normal, hit_fraction);
		
		if (!hit_pos) {
			return;
		}
		
		float poolSize;
		if (bloodLossLevel == 5) {
			poolSize = 0.7;
		} else if (bloodLossLevel == 4) {
			poolSize = 0.6;
		} else if (bloodLossLevel == 3) {
			poolSize = 0.5;
		} else if (bloodLossLevel == 2) {
			poolSize = 0.4;
		} else if (bloodLossLevel == 1) {
			poolSize = 0.3;
		}
		poolSize = Math.RandomFloatInclusive(0.9 * poolSize, 1.1 * poolSize);
		
		vector hitNormal = hit_normal.Normalized();
		
		vector particleOrientation = hitNormal.VectorToAngles();
		particleOrientation[1] = particleOrientation[1] + 90;
		
		vector correctedHitPos;
		float correctedPoolSize;
		InediaInfectedAI_CorrectParticleForCurvedSurface(correctedHitPos, correctedPoolSize, hit_pos, hitNormal, poolSize, excludeObject);
		
		if (!correctedHitPos || correctedPoolSize < 0.2) {
			return;
		}
		
		vector particlesPos = correctedHitPos + hitNormal * 0.05;
		
		float verticalAngleDeviation = Math.AbsFloat(InediaInfectedAI_DegAngleBetweenVectors(hitNormal, vector.Up));
		if (verticalAngleDeviation <= 15) {
			// Blood pool are possible only when the normal vector deviation from the gravity vector is no more than 10 degrees.
			
			Particle pool = ParticleManager.GetInstance().PlayInWorld(ParticleList.INEDIAINFECTEDAI_BLOOD_POOL, particlesPos);
			pool.SetOrientation(particleOrientation);
			pool.SetParticleParam(EmitorParam.SIZE, correctedPoolSize);
			pool.SetParticleParam(EmitorParam.LIFETIME, currentPlayer.m_InediaInfectedAI_BloodParticleDurationSeconds * 2/3);
			pool.SetParticleParam(EmitorParam.LIFETIME_RND, currentPlayer.m_InediaInfectedAI_BloodParticleDurationSeconds * 1/3);
			currentPlayer.InediaInfectedAI_AddBloodParticleClient(pool);
		} else {
			// If creating a pool is impossible due to the slope - create several blood trails.
			
			for (int i = 1; i <= 3; i++) {
				InediaInfectedAI_CreateBloodTrailInPosition(correctedHitPos, bloodLossLevel, excludeObject);
			}
		}
#endif
	}
	
	static void InediaInfectedAI_CorrectParticleForCurvedSurface(out vector correctedHitPos, out float correctedRadius, vector hitPos, vector hitNormal, float radius, Object excludeObject = null)
	{
		if (!hitPos || !hitNormal || radius <= 0) {
			return;
		}
		
		vector hitNormalPerpend = vector.RotateAroundZeroDeg(hitNormal.Perpend().Normalized(), hitNormal, Math.RandomFloatInclusive(0, 360));

		float particleRadiusStep = 0.05;
		vector raycastCenterPos = hitPos + hitNormal * 0.1;
		
		float actualHorizontalSplatDiameter = 0;
		float currentHorizontalSplatRadius = 0;
		bool rightBorderReached = false;
		bool leftBorderReached = false;
		while (currentHorizontalSplatRadius <= radius) {
			currentHorizontalSplatRadius += particleRadiusStep;
			vector raycastDiffRightPos = raycastCenterPos + currentHorizontalSplatRadius * hitNormalPerpend;
			vector raycastDiffLeftPos = raycastCenterPos - currentHorizontalSplatRadius * hitNormalPerpend;
			
			if (!rightBorderReached && DayZPhysics.RayCastBullet(raycastDiffRightPos, raycastDiffRightPos - hitNormal * 0.2, PhxInteractionLayers.BUILDING | PhxInteractionLayers.TERRAIN, excludeObject, null, null, null, null)) {
				// The splat can be expanded to the right, thus increasing the actual diameter.
				actualHorizontalSplatDiameter += particleRadiusStep;
			} else {
				// The splat cannot be expanded to the right, shifting the center to the left.
				raycastCenterPos = raycastCenterPos - particleRadiusStep * hitNormalPerpend;
				rightBorderReached = true;
			}
			
			if (!leftBorderReached && DayZPhysics.RayCastBullet(raycastDiffLeftPos, raycastDiffLeftPos - hitNormal * 0.2, PhxInteractionLayers.BUILDING | PhxInteractionLayers.TERRAIN, excludeObject, null, null, null, null)) {
				// The splat can be expanded to the left, thus increasing the actual diameter.
				actualHorizontalSplatDiameter += particleRadiusStep;
			} else {
				// The splat cannot be expanded to the left, shifting the center to the right.
				raycastCenterPos = raycastCenterPos + particleRadiusStep * hitNormalPerpend;
				leftBorderReached = true;
			}
		}
		
		vector hitNormalPerpendVertical = vector.RotateAroundZeroDeg(hitNormalPerpend, hitNormal, 90);
		float actualVerticalSplatDiameter = 0;
		float currentVerticalSplatRadius = 0;
		bool topBorderReached = false;
		bool bottomBorderReached = false;
		while (currentVerticalSplatRadius <= radius) {
			currentVerticalSplatRadius += particleRadiusStep;
			vector raycastDiffTopPos = raycastCenterPos + currentVerticalSplatRadius * hitNormalPerpendVertical;
			vector raycastDiffBottomPos = raycastCenterPos - currentVerticalSplatRadius * hitNormalPerpendVertical;
			
			if (!topBorderReached && DayZPhysics.RayCastBullet(raycastDiffTopPos, raycastDiffTopPos - hitNormal * 0.2, PhxInteractionLayers.BUILDING | PhxInteractionLayers.TERRAIN, excludeObject, null, null, null, null)) {
				// The splat can be expanded to the right, thus increasing the actual diameter.
				actualVerticalSplatDiameter += particleRadiusStep;
			} else {
				// The splat cannot be expanded to the right, shifting the center to the left.
				raycastCenterPos = raycastCenterPos - particleRadiusStep * hitNormalPerpendVertical;
				topBorderReached = true;
			}
			
			if (!bottomBorderReached && DayZPhysics.RayCastBullet(raycastDiffBottomPos, raycastDiffBottomPos - hitNormal * 0.2, PhxInteractionLayers.BUILDING | PhxInteractionLayers.TERRAIN, excludeObject, null, null, null, null)) {
				// The splat can be expanded to the left, thus increasing the actual diameter.
				actualVerticalSplatDiameter += particleRadiusStep;
			} else {
				// The splat cannot be expanded to the left, shifting the center to the right.
				raycastCenterPos = raycastCenterPos + particleRadiusStep * hitNormalPerpendVertical;
				bottomBorderReached = true;
			}
		}
		
		correctedHitPos = raycastCenterPos - hitNormal * 0.1;
		correctedRadius = Math.Min(actualHorizontalSplatDiameter, actualVerticalSplatDiameter) / 2;
	}
	
	static int InediaInfectedAI_GetBloodLossLevelFromBloodLossRatePerSec(float bloodLossRatePerSec) {
		if (bloodLossRatePerSec > 40) {
			return 5;
		} else if (bloodLossRatePerSec > 25) {
			return 4;
		} else if (bloodLossRatePerSec > 15) {
			return 3;
		} else if (bloodLossRatePerSec > 5) {
			return 2;
		} else if (bloodLossRatePerSec > 0) {
			return 1;
		} else {
			return 0;
		}
	}
	
	static float InediaInfectedAI_DegAngleBetweenVectors(vector v1, vector v2) {
	    return Math.Acos(vector.Dot(v1, v2) / (v1.Length() * v2.Length())) * Math.RAD2DEG;
	}
	
	static float InediaInfectedAI_GetItemMeleeHeavyDamage(ItemBase item, string type = "Health")
	{
		float damage = 0;
		
		if (item && (type == "Health" || type == "Shock" || type == "Blood")) {
			string projectileMeleeHeavyAmmo = GetGame().ConfigGetTextOut("CfgVehicles " + item.GetType() + " MeleeModes Heavy ammo");
			if (projectileMeleeHeavyAmmo) {
				damage = GetGame().ConfigGetFloat("CfgAmmo " + projectileMeleeHeavyAmmo + " DamageApplied " + type + " damage");
			}
		}
		
		return damage;
	}
	
	static vector InediaInfectedAI_VectorCross(vector a, vector b)
	{
	    return Vector(a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]);
	}
}