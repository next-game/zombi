/**
 * Created: 2024-09-15
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

class InediaInfectedAI_PainMdfr: ModifierBase
{
// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ................................................................:--====---===+++-----::...............................................................
// ..........................................................:=*##*=-.         ..::::::-=+++=-...........................................................
// ........................................................-*+*+:      .:--=========-:       ............................................................
// .......................................................==+=   .-----:. .:---:::..:-++=. .    ..-......................................................
// .....................................................:==+  .--:   .-=--:.            .=+-:---:-.-=:...................................................
// .....................................................+=+   :  :===:                :-.  -*=. -*#-.=+-.................................................
// ....................................................=-#.--::-====+++=---===+++=-:    :+:  *#=: :#+:.-++-:.............................................
// ...................................................:**%#-. ..                  .-++=.  ++: ==.:. +*-==:-===-:.........................................
// ...................................................=-*=  -+:..:=====--------:.     :*=  *== :*... -%-.-==-..:.........................................
// ...................................................=*@=+.  -=::    ..::-----=++=---: -+ .%-- :=-   .*=....:::.........................................
// ..................................................:*@=:.+=:                   .-------+* +*-: :.-=:  -*-..............................................
// ...................................................:%  - -+ .::--::------::::.     .-=-:+.%:=- -- :==. -+-:...........................................
// ....................................................+  :  .*:==---*+:.:-=::::-=++=-:  .-+*-%.:=. ==. :==-:===-:.......................................
// ....................................................:+-   .===::-:       .:....::.:-+=:: :=+#. --. -==-..-===------:..................................
// .....................................................+--=-=*- .-*#**+=:    +*=--+*+= :=:+=:-*%.  .--. .=++=:.-++*=-:-::...............................
// .....................................................+#+.:-  #@+:           --##--.++-.=.=%#.-%-    :--  .:=++=:.:=*+:.::.............................
// .....................................................:+-+*=  =.:+*%##:     .:.==#= ===--#..*+-.##+-    :-=:...:==-:..-::.::...........................
// .....................................................=#%%##=   .-:-:      := :  -: *  + =#- ===.-=:--:.  .++:...........:.............................
// ......................................................:*-..=             :+    .: +@. :- #-+:-+--.-=....:...--:.......................................
// .......................................................*  +.            -=    :*-=%@-  = *-.=+:+:-:.--.........::.....................................
// .......................................................*.*:   .:       :-     :++=.@:  : +=..-*:*-.:::::..............................................
// .......................................................==--:--.               -: +-@:  . +=...#:.+*:.::::.............................................
// ........................................................+-  =                 *.  *@=    *=...*-..:++-:...............................................
// .........................................................=+*=-+*=-.          +=:  *+%.   =+...%:.....::::.............................................
// ..........................................................=++:..--         -*: +  #:@#.  .#:.*#---:...................................................
// ...........................................................-#:::        .==:  -- .*-##*.. =#+-=++-=#*=:...............................................
// ............................................................==        -=-     #  *=%+=%+=*=-==..=*= .-#+-.............................................
// .............................................................==:..:--%.       * -*##%%#---=-     .+#=  -*==...........................................
// ...............................................................:--:.:#-       #+*#%#===*+.         :++- .+===:........................................
// ...................................................................*%**  :=+##*%@+-==.  ++           :=*. :+:==.......................................
// ..........................................................::::....=@#*%**=-. :%*:=#      .+=           :*+  =-.=-.....................................
// ......................................................:==+=..:=+++*%. .   .-*%--: .==      .+=.         .-*: :+ .=-...................................
// ....................................................-=+*+:-*%@@@@@@%--:-=*+##::     .+-      .+=           -=:-#. -+..................................
// ..................................................-+=*+-*@@@@@@@@@@@---:..+%-         :+-      .++.  .--:------==--=+-................................
// ................................................:==**-#@@@@@@###*=:* .:-+%@.-           :+:       =-==++===--:--=*#***=...............................
// ...............................................===#-*@@@@@%*+%@#++**+=--:#- -             -*.      --.             *@@*=-.............................
// ..............................................+-**=@@@@@%++%%+..        -+: .              .#-                      +@@%=+............................
// ............................................:+-#+*@@@@%+++: -+          %.-- ..          --:.-=-                     +@@@=+...........................
// ...........................................:+-%=%@@@@*++:   =%:   .:--::%* .=..:      :-- -=-  .=-                    *@@#+-..........................
// ..........................................:+=%=@@@@%=##  .--=++==-:.:---.*#: :-.::  -=: -+:      .=:     .::::::::::::.#@@:+..........................
// .........................................-+=#+@@@@*=***.  +-    :::-::::-+%#=  :-.-=. :+-          :+..:.             .-*@-*..........................
// ........................................:==*#@@@@=*+=. --. .=*++----::--=::++=:  :=. ==           .:    .:------------::.:#+..........................
// .......................................:+=*#@@@#-%==.    -==-.  .-===:.. :-=++:=.  :=:            .----=====+*****####**#*=-..........................
// ......................................:+-*%@%#++#:.       :-----:. .-----:   -%--=-::.      :----+++====-:.          =@@@%=:..........................
// ......................................*:*@@***:--    :--::. .:-----.        ==%@#+=++====---==+*#-:-::               .@@*-=...........................
// .....................................=**%#*%%*: :=-::   :==-:.            :+. *#=   :----#*:-+#*=-==-.:::::::::::---. *++=............................
// .....................................:=+*#@=  +-  .:-=--=:              .+-  :++=*.      .%:   .=*#+--=-            .:=#:.............................
// ........................................-*.    :-*=-= :=               ==   =-  +:*.   =: .#.      :+#+.-=-  ...:::::-+%..............................
// .......................................+=       .+:+ --              :+.  :+.    *-%-    :.:#         -#-.=#**+=-::....*-.............................
// .....................................:+.       .#.+ :-             .+-   =-     =::*@*.     :#.    .-+**=-+*==--:-=++*#=+.............................
// ....................................-+         #.+  =             ==   -+.    .+   :+#%=     :#:-*#*=. .==:   -+=+#=----=.............................
// ...................................==         *=.+ +.           :+.   +-     :+       -=*+:  .+#+: .-=+*+===-:  =-%=..................................
// ..................................=+       :.*= := *          .=:   -=      =-      .+.  .:=+*%=:=++        .:===*@=..................................
// ...................................*-      +-=  .+ *         =-   .+.      *:      :*      :*.#@*.:#*--:..       :+#:.................................
// ................................... ++     +=-   * +       -+.   ==      .*.      ==      :#. *%#=. .#-:-=++++=:   .*.................................
// ...................................:.:*-   :%-    =+=.   .+:   :+.      :=    :::*-      -#. :%::*    *.      .-=+- *.................................
// ....................................:- -*=.-#-      =*+.==    +=       .:   ==:++       :#  :#:..*.   :+  ...     :*#.................................
// .....................................:+. .-* ==.......:*=   :*.          -==:+*:       :#. :*:...:%=--=***+===+*+-  #:................................
// .......................................+- =#--+=--==--===*=+=        :---:-*%:        -#. :*:.....**      .:----::=+#=................................
// ........................................=-%*  +.  :.     : -==-::::::.:=++: .+=      -#  -*:......:@:           .-=-+#................................
// ..........................................-=  *   :      :    .**===+=:       =*-   -#  -*.........+%               =@:...............................
// ..........................................=-  *   -       .   -*           .-:  :=:=+  =*..........:%+               +=...............................
// ..........................................=-  *   =          .# :        --.      ++  +%=...........-@-            .-+++++:...........................
// ..........................................=:  *   =          *:-.     :-:      -++-:+*+-+............#%         :--=*#+:.-=...........................
// ..........................................-: .+   =         +=-:   .--.    .-==: ---#= *-............-@=      --=*#=.     +...........................
// ..........................................-- -+   =        :#.-  :-:    .==- .-=-  *=:*:..............*#    -==%+.     .:-*...........................
// ..........................................:= ==   =        #.-.::    .==- .==-    =%=%:...............-@. --=%+.   :--===:==..........................
// ..........................................:+ +-   =       ==+:.   :==: .--:    .-+#.==.................%*--#+.  :----::+-:=*..........................
// ...........................................* *:   =.     .*=   -==: .::.   :---: *  %..................*.**. .==:--:=-:.* :%..........................
// ...........................................* #.   -:     *+-==-.   .   :---.     =  %:.................+*. :=-.=-:=:    +-.@:.........................
// ...........................................*.%    .=  .-=+-.  ....:----.    .::-:=-::*.................:%:+-.--.=.      .# %-.........................

	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID = InediaInfectedAI_eModifiers.MDF_PAIN;
		m_TickIntervalInactive = DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive = 1;
	}

	override protected bool ActivateCondition(PlayerBase player)
	{
		return player && player.InediaInfectedAI_GetPainManager().PainSystemIsActive();
	}

	override protected void OnTick(PlayerBase player, float deltaT)
	{		
		if (!player || !player.m_InediaInfectedAI_ConfigPlayers || !player.InediaInfectedAI_GetPainManager().PainSystemIsActive() || !player.GetAllowDamage()) return;

		InediaInfectedAI_HealLimbsHandler(player, deltaT);
		InediaInfectedAI_UpdatePainLevelsHandler(player, deltaT);
		InediaInfectedAI_UpdateFracturesHandler(player, deltaT);
		InediaInfectedAI_UpdateDeepWoundsHandler(player, deltaT);
		InediaInfectedAI_ApplyDeepWoundsDeebuffsHandler(player, deltaT);
		InediaInfectedAI_UpdateBulletsHandler(player, deltaT);
		InediaInfectedAI_ApplyBulletsDebuffsHandler(player, deltaT);
		InediaInfectedAI_ApplyInternalBleedingDebuffsHandler(player, deltaT);
		InediaInfectedAI_ApplyPainDebuffsHandler(player, deltaT);
		InediaInfectedAI_ApplyMedicineBuffsHandler(player, deltaT);

// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ................................................................:--====---===+++-----::...............................................................
// ..........................................................:=*##*=-.         ..::::::-=+++=-...........................................................
// ........................................................-*+*+:      .:--=========-:       ............................................................
// .......................................................==+=   .-----:. .:---:::..:-++=. .    ..-......................................................
// .....................................................:==+  .--:   .-=--:.            .=+-:---:-.-=:...................................................
// .....................................................+=+   :  :===:                :-.  -*=. -*#-.=+-.................................................
// ....................................................=-#.--::-====+++=---===+++=-:    :+:  *#=: :#+:.-++-:.............................................
// ...................................................:**%#-. ..                  .-++=.  ++: ==.:. +*-==:-===-:.........................................
// ...................................................=-*=  -+:..:=====--------:.     :*=  *== :*... -%-.-==-..:.........................................
// ...................................................=*@=+.  -=::    ..::-----=++=---: -+ .%-- :=-   .*=....:::.........................................
// ..................................................:*@=:.+=:                   .-------+* +*-: :.-=:  -*-..............................................
// ...................................................:%  - -+ .::--::------::::.     .-=-:+.%:=- -- :==. -+-:...........................................
// ....................................................+  :  .*:==---*+:.:-=::::-=++=-:  .-+*-%.:=. ==. :==-:===-:.......................................
// ....................................................:+-   .===::-:       .:....::.:-+=:: :=+#. --. -==-..-===------:..................................
// .....................................................+--=-=*- .-*#**+=:    +*=--+*+= :=:+=:-*%.  .--. .=++=:.-++*=-:-::...............................
// .....................................................+#+.:-  #@+:           --##--.++-.=.=%#.-%-    :--  .:=++=:.:=*+:.::.............................
// .....................................................:+-+*=  =.:+*%##:     .:.==#= ===--#..*+-.##+-    :-=:...:==-:..-::.::...........................
// .....................................................=#%%##=   .-:-:      := :  -: *  + =#- ===.-=:--:.  .++:...........:.............................
// ......................................................:*-..=             :+    .: +@. :- #-+:-+--.-=....:...--:.......................................
// .......................................................*  +.            -=    :*-=%@-  = *-.=+:+:-:.--.........::.....................................
// .......................................................*.*:   .:       :-     :++=.@:  : +=..-*:*-.:::::..............................................
// .......................................................==--:--.               -: +-@:  . +=...#:.+*:.::::.............................................
// ........................................................+-  =                 *.  *@=    *=...*-..:++-:...............................................
// .........................................................=+*=-+*=-.          +=:  *+%.   =+...%:.....::::.............................................
// ..........................................................=++:..--         -*: +  #:@#.  .#:.*#---:...................................................
// ...........................................................-#:::        .==:  -- .*-##*.. =#+-=++-=#*=:...............................................
// ............................................................==        -=-     #  *=%+=%+=*=-==..=*= .-#+-.............................................
// .............................................................==:..:--%.       * -*##%%#---=-     .+#=  -*==...........................................
// ...............................................................:--:.:#-       #+*#%#===*+.         :++- .+===:........................................
// ...................................................................*%**  :=+##*%@+-==.  ++           :=*. :+:==.......................................
// ..........................................................::::....=@#*%**=-. :%*:=#      .+=           :*+  =-.=-.....................................
// ......................................................:==+=..:=+++*%. .   .-*%--: .==      .+=.         .-*: :+ .=-...................................
// ....................................................-=+*+:-*%@@@@@@%--:-=*+##::     .+-      .+=           -=:-#. -+..................................
// ..................................................-+=*+-*@@@@@@@@@@@---:..+%-         :+-      .++.  .--:------==--=+-................................
// ................................................:==**-#@@@@@@###*=:* .:-+%@.-           :+:       =-==++===--:--=*#***=...............................
// ...............................................===#-*@@@@@%*+%@#++**+=--:#- -             -*.      --.             *@@*=-.............................
// ..............................................+-**=@@@@@%++%%+..        -+: .              .#-                      +@@%=+............................
// ............................................:+-#+*@@@@%+++: -+          %.-- ..          --:.-=-                     +@@@=+...........................
// ...........................................:+-%=%@@@@*++:   =%:   .:--::%* .=..:      :-- -=-  .=-                    *@@#+-..........................
// ..........................................:+=%=@@@@%=##  .--=++==-:.:---.*#: :-.::  -=: -+:      .=:     .::::::::::::.#@@:+..........................
// .........................................-+=#+@@@@*=***.  +-    :::-::::-+%#=  :-.-=. :+-          :+..:.             .-*@-*..........................
// ........................................:==*#@@@@=*+=. --. .=*++----::--=::++=:  :=. ==           .:    .:------------::.:#+..........................
// .......................................:+=*#@@@#-%==.    -==-.  .-===:.. :-=++:=.  :=:            .----=====+*****####**#*=-..........................
// ......................................:+-*%@%#++#:.       :-----:. .-----:   -%--=-::.      :----+++====-:.          =@@@%=:..........................
// ......................................*:*@@***:--    :--::. .:-----.        ==%@#+=++====---==+*#-:-::               .@@*-=...........................
// .....................................=**%#*%%*: :=-::   :==-:.            :+. *#=   :----#*:-+#*=-==-.:::::::::::---. *++=............................
// .....................................:=+*#@=  +-  .:-=--=:              .+-  :++=*.      .%:   .=*#+--=-            .:=#:.............................
// ........................................-*.    :-*=-= :=               ==   =-  +:*.   =: .#.      :+#+.-=-  ...:::::-+%..............................
// .......................................+=       .+:+ --              :+.  :+.    *-%-    :.:#         -#-.=#**+=-::....*-.............................
// .....................................:+.       .#.+ :-             .+-   =-     =::*@*.     :#.    .-+**=-+*==--:-=++*#=+.............................
// ....................................-+         #.+  =             ==   -+.    .+   :+#%=     :#:-*#*=. .==:   -+=+#=----=.............................
// ...................................==         *=.+ +.           :+.   +-     :+       -=*+:  .+#+: .-=+*+===-:  =-%=..................................
// ..................................=+       :.*= := *          .=:   -=      =-      .+.  .:=+*%=:=++        .:===*@=..................................
// ...................................*-      +-=  .+ *         =-   .+.      *:      :*      :*.#@*.:#*--:..       :+#:.................................
// ................................... ++     +=-   * +       -+.   ==      .*.      ==      :#. *%#=. .#-:-=++++=:   .*.................................
// ...................................:.:*-   :%-    =+=.   .+:   :+.      :=    :::*-      -#. :%::*    *.      .-=+- *.................................
// ....................................:- -*=.-#-      =*+.==    +=       .:   ==:++       :#  :#:..*.   :+  ...     :*#.................................
// .....................................:+. .-* ==.......:*=   :*.          -==:+*:       :#. :*:...:%=--=***+===+*+-  #:................................
// .......................................+- =#--+=--==--===*=+=        :---:-*%:        -#. :*:.....**      .:----::=+#=................................
// ........................................=-%*  +.  :.     : -==-::::::.:=++: .+=      -#  -*:......:@:           .-=-+#................................
// ..........................................-=  *   :      :    .**===+=:       =*-   -#  -*.........+%               =@:...............................
// ..........................................=-  *   -       .   -*           .-:  :=:=+  =*..........:%+               +=...............................
// ..........................................=-  *   =          .# :        --.      ++  +%=...........-@-            .-+++++:...........................
// ..........................................=:  *   =          *:-.     :-:      -++-:+*+-+............#%         :--=*#+:.-=...........................
// ..........................................-: .+   =         +=-:   .--.    .-==: ---#= *-............-@=      --=*#=.     +...........................
// ..........................................-- -+   =        :#.-  :-:    .==- .-=-  *=:*:..............*#    -==%+.     .:-*...........................
// ..........................................:= ==   =        #.-.::    .==- .==-    =%=%:...............-@. --=%+.   :--===:==..........................
// ..........................................:+ +-   =       ==+:.   :==: .--:    .-+#.==.................%*--#+.  :----::+-:=*..........................
// ...........................................* *:   =.     .*=   -==: .::.   :---: *  %..................*.**. .==:--:=-:.* :%..........................
// ...........................................* #.   -:     *+-==-.   .   :---.     =  %:.................+*. :=-.=-:=:    +-.@:.........................
// ...........................................*.%    .=  .-=+-.  ....:----.    .::-:=-::*.................:%:+-.--.=.      .# %-.........................
	}
	
// HEAL LIMBS HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_HealLimbsHandler(PlayerBase player, float pDt)
	{
		// Decrease limbs pain (Heal)
		foreach (string limb: InediaInfectedAI_PainManager.m_InediaPainLimbs) {
			if (player.InediaInfectedAI_GetPainManager().GetPainPercentForLimb(limb) > 0) {
				if (player.m_InediaInfectedAI_ConfigPlayers.PainHealingRatePerSecondPercent > 0) {
					player.InediaInfectedAI_GetPainManager().AddPainPercentForLimb(limb, -player.m_InediaInfectedAI_ConfigPlayers.PainHealingRatePerSecondPercent);
				} else {
					player.InediaInfectedAI_GetPainManager().AddPainPercentForLimb(limb, -0.1);
				}
			} else {
				// To activate pain if, for some reason, the character has sustained a fracture/deep wound/bullet without pain.
				if (player.InediaInfectedAI_GetPainManager().IsLimbBroken(limb) || player.InediaInfectedAI_GetPainManager().IsLimbDeepWounded(limb) || player.InediaInfectedAI_GetPainManager().GetBulletCountInLimb(limb) > 0) {
					player.InediaInfectedAI_GetPainManager().AddPainPercentForLimb(limb, -1);
				}
			}
		}
	}
	
// UPDATE FRACTURES HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_UpdateFracturesHandler(PlayerBase player, float pDt)
	{
		if (!player.InediaInfectedAI_GetPainManager().LimbsBreakSystemIsActive()) {
			return;
		}
		
		foreach (string inediaPainLimb: InediaInfectedAI_PainManager.m_InediaPainLimbs) {
			if (inediaPainLimb == "legs") continue; // At the moment, the vanilla leg fracture is being used.
			
			float limbBreakTimeRemainingSeconds = player.InediaInfectedAI_GetPainManager().GetBreakTimeRemainingSecondsForLimb(inediaPainLimb);
			
			if (limbBreakTimeRemainingSeconds > 0) {
				if (!player.InediaInfectedAI_GetPainManager().IsLimbBroken(inediaPainLimb)) {
					// Restoring fracture
					bool stabilizationStatusBefore = player.InediaInfectedAI_GetPainManager().IsLimbStabilized(inediaPainLimb);
					float stabilizationShockReductionMultiplierBefore = player.InediaInfectedAI_GetPainManager().GetBreakStabilizationShockReductionMultiplierForLimb(inediaPainLimb);
					float stabilizationRegenerationMultiplierBefore = player.InediaInfectedAI_GetPainManager().GetBreakStabilizationRegenerationMultiplierForLimb(inediaPainLimb);

					player.InediaInfectedAI_GetPainManager().ActivateBreakForLimb(inediaPainLimb, 0, false, false);
					player.InediaInfectedAI_GetPainManager().SetBreakTimeRemainingSecondsForLimb(inediaPainLimb, limbBreakTimeRemainingSeconds);
					
					if (stabilizationStatusBefore) {
						player.InediaInfectedAI_GetPainManager().ActivateBreakStabilizeForLimb(inediaPainLimb, stabilizationShockReductionMultiplierBefore, stabilizationRegenerationMultiplierBefore);
					}
				} else {
					player.InediaInfectedAI_GetPainManager().SetBreakTimeRemainingSecondsForLimb(inediaPainLimb, limbBreakTimeRemainingSeconds - pDt);
				}
			} else {
				if (player.InediaInfectedAI_GetPainManager().IsLimbBroken(inediaPainLimb)) {
					player.InediaInfectedAI_GetPainManager().DeactivateBreakForLimb(inediaPainLimb);
				}
			}
		}
	}
	
// UPDATE DEEP WOUNDS HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_UpdateDeepWoundsHandler(PlayerBase player, float pDt)
	{
		if (!player.InediaInfectedAI_GetPainManager().LimbsDeepWoundSystemIsActive()) {
			return;
		}
		
		foreach (string inediaPainLimb: InediaInfectedAI_PainManager.m_InediaPainLimbs) {					
			float limbDeepWoundTimeRemainingSeconds = player.InediaInfectedAI_GetPainManager().GetDeepWoundTimeRemainingSecondsForLimb(inediaPainLimb);
			
			if (limbDeepWoundTimeRemainingSeconds > 0) {
				if (!player.InediaInfectedAI_GetPainManager().IsLimbDeepWounded(inediaPainLimb)) {
					// Restoring deep wound
					bool bandagedStatusBefore = player.InediaInfectedAI_GetPainManager().IsLimbDeepWoundBandaged(inediaPainLimb);
					float bandageEffectivenessTimeRemainingSecondsBefore = player.InediaInfectedAI_GetPainManager().GetDeepWoundBandageEffectivenessTimeRemainingSecondsForLimb(inediaPainLimb);
					float bandageRegenerationMultiplierBefore = player.InediaInfectedAI_GetPainManager().GetDeepWoundBandageRegenerationMultiplierForLimb(inediaPainLimb);
					
					player.InediaInfectedAI_GetPainManager().ActivateDeepWoundForLimb(inediaPainLimb, 0, -2, 0, false, false);
					player.InediaInfectedAI_GetPainManager().SetDeepWoundTimeRemainingSecondsForLimb(inediaPainLimb, limbDeepWoundTimeRemainingSeconds);
					
					if (bandagedStatusBefore) {
						player.InediaInfectedAI_GetPainManager().ActivateDeepWoundBandageForLimb(inediaPainLimb, bandageEffectivenessTimeRemainingSecondsBefore, bandageRegenerationMultiplierBefore);
					}
				} else {
					// Regenerate the wound itself over time.
					player.InediaInfectedAI_GetPainManager().SetDeepWoundTimeRemainingSecondsForLimb(inediaPainLimb, limbDeepWoundTimeRemainingSeconds - pDt);
				
					// Reduce the wound's bleeding over time.
					float newBleedingRate = player.InediaInfectedAI_GetPainManager().GetDeepWoundBleedingRateForLimb(inediaPainLimb) - InediaInfectedAI_PainManager.DEEP_WOUND_BLEEDING_RATE_REGEN_ML_SEC;
					player.InediaInfectedAI_GetPainManager().SetDeepWoundBleedingRateForLimb(inediaPainLimb, Math.Clamp(newBleedingRate, InediaInfectedAI_PainManager.DEEP_WOUND_BLEEDING_RATE_MIN, InediaInfectedAI_PainManager.DEEP_WOUND_BLEEDING_RATE_MAX));
				}
			} else {
				if (player.InediaInfectedAI_GetPainManager().IsLimbDeepWounded(inediaPainLimb)) {
					player.InediaInfectedAI_GetPainManager().DeactivateDeepWoundForLimb(inediaPainLimb);
				}
			}
		}
	}
	
// DEEP WOUNDS DEBUFFS HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_ApplyDeepWoundsDeebuffsHandler(PlayerBase player, float pDt)
	{
		if (!player.InediaInfectedAI_GetPainManager().LimbsDeepWoundSystemIsActive()) {
			return;
		}
		
		float bloodLossRatePerSec = player.InediaInfectedAI_GetPainManager().GetDeepWoundsBleedingRateWithMultipliers();

		if (bloodLossRatePerSec > 0) {
			player.DecreaseHealth("GlobalHealth", "Blood", bloodLossRatePerSec * pDt);
		}
	}
	
// UPDATE BULLETS HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_UpdateBulletsHandler(PlayerBase player, float pDt)
	{
		if (!player.InediaInfectedAI_GetPainManager().LimbsBulletSystemIsActive()) {
			return;
		}
		
		foreach (string inediaPainLimb: InediaInfectedAI_PainManager.m_InediaPainLimbs) {
			if (player.InediaInfectedAI_GetPainManager().GetBulletCountInLimb(inediaPainLimb) > 0) {
				player.InediaInfectedAI_GetPainManager().SetBulletInBodySecondsForLimb(inediaPainLimb, player.InediaInfectedAI_GetPainManager().GetBulletInBodySecondsForLimb(inediaPainLimb) + pDt);
			}
		}
	}
	
// BULLETS DEBUFFS HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_ApplyBulletsDebuffsHandler(PlayerBase player, float pDt)
	{
		if (!player.InediaInfectedAI_GetPainManager().LimbsBulletSystemIsActive()) {
			return;
		}
		
		foreach (string inediaPainLimb: InediaInfectedAI_PainManager.m_InediaPainLimbs) {
			if (player.InediaInfectedAI_GetPainManager().GetBulletCountInLimb(inediaPainLimb) > 0) {
				InediaInfectedAI_BulletInBodyStages bulletInBodyStage = player.InediaInfectedAI_GetPainManager().GetBulletInBodyStageForLimb(inediaPainLimb);
				
				if (bulletInBodyStage >= InediaInfectedAI_BulletInBodyStages.SECOND) {
					player.InsertAgent(eAgents.HEAVYMETAL, 2);
				} else {
					// No debuffs on first stage
					//
				}
			}
		}
	}
	
// INTERNAL BLEEDING DEBUFF HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_ApplyInternalBleedingDebuffsHandler(PlayerBase player, float pDt)
	{
		if (!player.InediaInfectedAI_GetPainManager().InternalBleedingSystemIsActive()) {
			return;
		}
		
		if (player.InediaInfectedAI_GetPainManager().IsInternalBleedingActive()) {
			float bloodLossRatePerSec = player.m_InediaInfectedAI_ConfigPlayers.InternalBleedingBloodLossRateMl;
			
			if (player.GetModifiersManager() && player.GetModifiersManager().IsModifierActive(InediaInfectedAI_eModifiers.MDF_BLOOD_CLOTTING)) {
				if (player.InediaInfectedAI_GetPainManager().m_BloodClottingItemClottingMultiplier > 1) {
					bloodLossRatePerSec /= player.InediaInfectedAI_GetPainManager().m_BloodClottingItemClottingMultiplier;
				}
			}
	
			if (bloodLossRatePerSec > 0) {
				player.DecreaseHealth("GlobalHealth", "Blood", bloodLossRatePerSec * pDt);
			}
		}
	}

// UPDATE PAIN LEVELS HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	float m_InediaInfectedAI_UpdatePainLevelsHandlerTimer = 0;
	void InediaInfectedAI_UpdatePainLevelsHandler(PlayerBase player, float pDt)
	{
		m_InediaInfectedAI_UpdatePainLevelsHandlerTimer -= pDt;
		if (m_InediaInfectedAI_UpdatePainLevelsHandlerTimer > 0) return;
		m_InediaInfectedAI_UpdatePainLevelsHandlerTimer = 3;

		// Every 3 seconds - update pain levels
		player.InediaInfectedAI_GetPainManager().UpdatePainLevelsForAllLimbs();
	}
	
// APPLY PAIN DEBUFFS HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_ApplyPainDebuffsHandler(PlayerBase player, float pDt)
	{
		// Every second apply the necessary debuffs to the player and decrease limbs pain

// Pain torso level >=LOW: reset stamina debuff
		if (player.m_StaminaHandler && !player.InediaInfectedAI_IsStaminaRegenDisabled()) {
			InediaInfectedAI_LimbPainLevels painTorsoLevel = player.InediaInfectedAI_GetPainManager().GetPainWithBuffsLevelForLimb("torso");
			float staminaDepletionChanceMultiplier = player.InediaInfectedAI_GetPainManager().GetActionsShockMultiplierForLimb("torso");
			
			if (painTorsoLevel >= InediaInfectedAI_LimbPainLevels.BADLY) {
				if (Math.RandomFloatInclusive(0, 100) <= 8 * staminaDepletionChanceMultiplier) {
					player.InediaInfectedAI_DisableStaminaRegenForSeconds(player.m_InediaInfectedAI_ConfigPlayers.DisableStaminaRegenWhenTorsoPainDurationSeconds);
				}
			} else if (painTorsoLevel >= InediaInfectedAI_LimbPainLevels.MEDIUM) {
				if (Math.RandomFloatInclusive(0, 100) <= 5 * staminaDepletionChanceMultiplier) {
					player.InediaInfectedAI_DisableStaminaRegenForSeconds(player.m_InediaInfectedAI_ConfigPlayers.DisableStaminaRegenWhenTorsoPainDurationSeconds);
				}
			} else if (painTorsoLevel >= InediaInfectedAI_LimbPainLevels.LOW) {
				if (Math.RandomFloatInclusive(0, 100) <= 2 * staminaDepletionChanceMultiplier) {
					player.InediaInfectedAI_DisableStaminaRegenForSeconds(player.m_InediaInfectedAI_ConfigPlayers.DisableStaminaRegenWhenTorsoPainDurationSeconds);
				}
			}
		}
		
		// Debuffs that only work when the player is conscious.
		if (!player.IsUnconscious()) {
			
// Shock damage from limb pain
			float shockDamage = 0;
			
			// Pain legs value >=MEDIUM - shock damage when running
			if (player.m_InediaInfectedAI_ConfigPlayers.ShockIfRunsWithPainedLegs) {
				player.GetMovementState(player.m_MovementState);
				if (player.m_MovementState) {
					float legsShockDamage = 0;
					
					InediaInfectedAI_LimbPainLevels painLegsLevel = player.InediaInfectedAI_GetPainManager().GetPainWithBuffsLevelForLimb("legs");
					
					if ((player.GetCommand_Ladder() && player.m_MovementState.m_iMovement != DayZPlayerConstants.MOVEMENTIDX_IDLE) || player.m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_RUN || player.m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_SPRINT || player.m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_CROUCH_RUN || InediaInfectedAI_DayzBicycleCompatibility.IsPlayerOnBikeAndCycling(player)) {
						// The player is climbing a ladder or running or on bicycle
						if (painLegsLevel >= InediaInfectedAI_LimbPainLevels.BADLY) {
							legsShockDamage = 0.4;
						} else if (painLegsLevel >= InediaInfectedAI_LimbPainLevels.MEDIUM) {
							legsShockDamage = 0.2;
						}
					} else if (player.m_InediaInfectedAI_ConfigPlayers.DisableVanillaBrokenLegWalk && player.m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_WALK && (player.m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT || player.m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT) && player.InediaInfectedAI_GetPainManager().IsLimbBroken("legs") && !player.InediaInfectedAI_GetPainManager().IsLimbStabilized("legs")) {
						// If the character is walking with a broken, unstabilized leg
						if (painLegsLevel >= InediaInfectedAI_LimbPainLevels.BADLY) {
							legsShockDamage = 1.2;
						} else if (painLegsLevel >= InediaInfectedAI_LimbPainLevels.MEDIUM) {
							legsShockDamage = 0.6;
						} else if (painLegsLevel >= InediaInfectedAI_LimbPainLevels.LOW) {
							legsShockDamage = 0.3;
						}
					}
					
					if (legsShockDamage > 0) {
						legsShockDamage *= player.InediaInfectedAI_GetPainManager().GetActionsShockMultiplierForLimb("legs");
							
						// Load weight shock multiplier
						float loadWeightKg = player.GetPlayerLoad() / 1000;
						legsShockDamage *= 1 + Math.Clamp(Math.Pow((loadWeightKg - 10) / 30, 2), 0, 4);
						
						shockDamage += legsShockDamage;
					}
				}
			}
			
			// Pain arms value >=MEDIUM - shock damage when running with heavy objects in hands/climbing ladders/crawling prone
			if (player.m_InediaInfectedAI_ConfigPlayers.ShockIfRunsWithPainedArms) {
				float armsShockDamage = 0;
				
				InediaInfectedAI_LimbPainLevels painArmsLevel_as = player.InediaInfectedAI_GetPainManager().GetPainWithBuffsLevelForLimb("arms");
				
				if (painArmsLevel_as >= InediaInfectedAI_LimbPainLevels.MEDIUM) {
					player.GetMovementState(player.m_MovementState);
					
					if (player.m_MovementState && player.m_MovementState.m_iMovement != DayZPlayerConstants.MOVEMENTIDX_IDLE) {
						// The player moves by any means
						
						ItemBase itemInHands = player.GetItemInHands();
						if ((itemInHands && itemInHands.GetWeightEx() > 10000) || player.GetCommand_Ladder() || player.m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE) {
							if (painArmsLevel_as >= InediaInfectedAI_LimbPainLevels.BADLY) {
								armsShockDamage = 2;
							} else if (painArmsLevel_as >= InediaInfectedAI_LimbPainLevels.MEDIUM) {
								armsShockDamage = 1;
							}
							
							if (itemInHands && itemInHands.GetWeightEx() > 10000) {
								armsShockDamage *= 1 + Math.Clamp(Math.Pow((itemInHands.GetWeightEx() / 1000 - 10) / 10, 2), 0, 4);
							}
							
							armsShockDamage *= player.InediaInfectedAI_GetPainManager().GetActionsShockMultiplierForLimb("arms");
							
							if (armsShockDamage >= 2) {
								player.InediaInfectedAI_PlayPainSoundServer(5);
							}
						}
					}
	
					shockDamage += armsShockDamage;
				}
			}
			
			if (shockDamage > 0) {
				player.DecreaseHealth("GlobalHealth", "Shock", (shockDamage + player.InediaInfectedAI_GetShockRefillSpeed()) * pDt);
			}
			
			
			InediaInfectedAI_LimbPainLevels painHeadLevel = player.InediaInfectedAI_GetPainManager().GetPainWithBuffsLevelForLimb("head");
			
// Pain head level >=LOW: auditory hallucinations
			if (player.m_InediaInfectedAI_ConfigPlayers.AuditoryHallucinationsWithPainedHead && !player.InediaInfectedAI_IsNPC()) {
				float auditoryHallucinationsChance = 0;
				if (painHeadLevel >= InediaInfectedAI_LimbPainLevels.BADLY) {
					auditoryHallucinationsChance = 0.3;
				} else if (painHeadLevel >= InediaInfectedAI_LimbPainLevels.MEDIUM) {
					auditoryHallucinationsChance = 0.2;
				} else if (painHeadLevel >= InediaInfectedAI_LimbPainLevels.LOW) {
					auditoryHallucinationsChance = 0.1;
				}
				auditoryHallucinationsChance *= player.InediaInfectedAI_GetPainManager().GetActionsShockMultiplierForLimb("head");
				if (auditoryHallucinationsChance > 0 && Math.RandomFloatInclusive(0, 100) <= auditoryHallucinationsChance) {
					player.InediaInfectedAI_PlayAuditoryHallucinationSoundServer();
				}
			}

			if (player.m_InediaInfectedAI_ConfigPlayers.VomitWithPainedHead && player.GetEmoteManager() && !player.GetEmoteManager().InediaInfectedAI_IsResting() && !player.GetEmoteManager().InediaInfectedAI_IsLyingDown()) {
// Pain head level >=LOW: vomit
				float vomitChance = 0;
				if (painHeadLevel >= InediaInfectedAI_LimbPainLevels.BADLY) {
					vomitChance = 0.2;
				} else if (painHeadLevel >= InediaInfectedAI_LimbPainLevels.MEDIUM) {
					vomitChance = 0.1;
				}
				vomitChance *= player.InediaInfectedAI_GetPainManager().GetActionsShockMultiplierForLimb("head");
				if (vomitChance > 0 && Math.RandomFloatInclusive(0, 100) <= vomitChance) {
					player.InediaInfectedAI_SymptomVomit();
				}
				
// Head fracture, unconscious chance
				if (player.m_InediaInfectedAI_ConfigPlayers.UnconsciousWhenHeadBreak && Math.RandomFloatInclusive(0, 100) <= 0.2 && player.InediaInfectedAI_GetPainManager().IsLimbBroken("head") && !player.InediaInfectedAI_GetPainManager().IsLimbStabilized("head")) {
					player.InediaInfectedAI_PlayPainSoundServer();
					player.InediaInfectedAI_GetPainManager().HeadHitAction(1);
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(player.InediaInfectedAI_MakeUnconscious, Math.RandomIntInclusive(6000, 8000), false);
				}
			}
			
// Pain arms level >=LOW: arms lowering debuff
			if (player.m_InediaInfectedAI_ConfigPlayers.ArmsLowerWhenAimingWithPainedArms && !player.InediaInfectedAI_IsNPC()) {
				float decreaseWeaponHoldingEnergyPercent = 0;
				
				InediaInfectedAI_LimbPainLevels painArmsLevel = player.InediaInfectedAI_GetPainManager().GetPainWithBuffsLevelForLimb("arms");
				if (painArmsLevel >= InediaInfectedAI_LimbPainLevels.LOW) {
					if (player.GetInputController() && player.GetInputController().IsWeaponRaised()) {
						// if ADS (Aim Down Sights) is active
						
						ItemBase itemHands = player.GetItemInHands();
						if (itemHands && itemHands.IsWeapon()) {
							// If the player is holding a ranged weapon.
							
							player.GetMovementState(player.m_MovementState);
							if (player.m_MovementState && player.m_MovementState.m_iStanceIdx != DayZPlayerConstants.STANCEIDX_RAISEDPRONE) {
								// If the player is not aiming while prone.
								
								decreaseWeaponHoldingEnergyPercent = 1.5 * itemHands.GetWeightEx() / 1000;
								
								if (painArmsLevel >= InediaInfectedAI_LimbPainLevels.BADLY) {
									decreaseWeaponHoldingEnergyPercent *= 3;
								} else if (painArmsLevel >= InediaInfectedAI_LimbPainLevels.MEDIUM) {
									decreaseWeaponHoldingEnergyPercent *= 1.5;
								}
								
								decreaseWeaponHoldingEnergyPercent *= player.InediaInfectedAI_GetPainManager().GetActionsShockMultiplierForLimb("arms");
							}
						}
					}
				}
				
				if (decreaseWeaponHoldingEnergyPercent > 0) {
					player.InediaInfectedAI_GetPainManager().m_WeaponHoldingWhileArmsPainEnergyPercent -= decreaseWeaponHoldingEnergyPercent;
					
					if (player.InediaInfectedAI_GetPainManager().m_WeaponHoldingWhileArmsPainEnergyPercent <= 0) {
						player.InediaInfectedAI_PlayPainSoundServer(5);
						player.InediaInfectedAI_ResetADS();
						
						// If the player raises a ranged weapon from the hip with pain in arms, they receive shock damage.
						float painArmsAdsShockMultiplier = player.InediaInfectedAI_GetPainManager().GetActionsShockMultiplierForLimb("arms");
						
						if (painArmsLevel >= InediaInfectedAI_LimbPainLevels.BADLY) {
							player.DecreaseHealth("GlobalHealth", "Shock", (2 * painArmsAdsShockMultiplier + player.InediaInfectedAI_GetShockRefillSpeed()) * pDt);
						} else if (painArmsLevel >= InediaInfectedAI_LimbPainLevels.MEDIUM) {
							player.DecreaseHealth("GlobalHealth", "Shock", (1 * painArmsAdsShockMultiplier + player.InediaInfectedAI_GetShockRefillSpeed()) * pDt);
						} else if (painArmsLevel >= InediaInfectedAI_LimbPainLevels.LOW) {
							player.DecreaseHealth("GlobalHealth", "Shock", (0.5 * painArmsAdsShockMultiplier + player.InediaInfectedAI_GetShockRefillSpeed()) * pDt);
						}
					}
				} else if (player.InediaInfectedAI_GetPainManager().m_WeaponHoldingWhileArmsPainEnergyPercent < 100) {
					player.InediaInfectedAI_GetPainManager().m_WeaponHoldingWhileArmsPainEnergyPercent = Math.Clamp(player.InediaInfectedAI_GetPainManager().m_WeaponHoldingWhileArmsPainEnergyPercent + 20, 0, 100);
				}
			}
			
// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ................................................................:--====---===+++-----::...............................................................
// ..........................................................:=*##*=-.         ..::::::-=+++=-...........................................................
// ........................................................-*+*+:      .:--=========-:       ............................................................
// .......................................................==+=   .-----:. .:---:::..:-++=. .    ..-......................................................
// .....................................................:==+  .--:   .-=--:.            .=+-:---:-.-=:...................................................
// .....................................................+=+   :  :===:                :-.  -*=. -*#-.=+-.................................................
// ....................................................=-#.--::-====+++=---===+++=-:    :+:  *#=: :#+:.-++-:.............................................
// ...................................................:**%#-. ..                  .-++=.  ++: ==.:. +*-==:-===-:.........................................
// ...................................................=-*=  -+:..:=====--------:.     :*=  *== :*... -%-.-==-..:.........................................
// ...................................................=*@=+.  -=::    ..::-----=++=---: -+ .%-- :=-   .*=....:::.........................................
// ..................................................:*@=:.+=:                   .-------+* +*-: :.-=:  -*-..............................................
// ...................................................:%  - -+ .::--::------::::.     .-=-:+.%:=- -- :==. -+-:...........................................
// ....................................................+  :  .*:==---*+:.:-=::::-=++=-:  .-+*-%.:=. ==. :==-:===-:.......................................
// ....................................................:+-   .===::-:       .:....::.:-+=:: :=+#. --. -==-..-===------:..................................
// .....................................................+--=-=*- .-*#**+=:    +*=--+*+= :=:+=:-*%.  .--. .=++=:.-++*=-:-::...............................
// .....................................................+#+.:-  #@+:           --##--.++-.=.=%#.-%-    :--  .:=++=:.:=*+:.::.............................
// .....................................................:+-+*=  =.:+*%##:     .:.==#= ===--#..*+-.##+-    :-=:...:==-:..-::.::...........................
// .....................................................=#%%##=   .-:-:      := :  -: *  + =#- ===.-=:--:.  .++:...........:.............................
// ......................................................:*-..=             :+    .: +@. :- #-+:-+--.-=....:...--:.......................................
// .......................................................*  +.            -=    :*-=%@-  = *-.=+:+:-:.--.........::.....................................
// .......................................................*.*:   .:       :-     :++=.@:  : +=..-*:*-.:::::..............................................
// .......................................................==--:--.               -: +-@:  . +=...#:.+*:.::::.............................................
// ........................................................+-  =                 *.  *@=    *=...*-..:++-:...............................................
// .........................................................=+*=-+*=-.          +=:  *+%.   =+...%:.....::::.............................................
// ..........................................................=++:..--         -*: +  #:@#.  .#:.*#---:...................................................
// ...........................................................-#:::        .==:  -- .*-##*.. =#+-=++-=#*=:...............................................
// ............................................................==        -=-     #  *=%+=%+=*=-==..=*= .-#+-.............................................
// .............................................................==:..:--%.       * -*##%%#---=-     .+#=  -*==...........................................
// ...............................................................:--:.:#-       #+*#%#===*+.         :++- .+===:........................................
// ...................................................................*%**  :=+##*%@+-==.  ++           :=*. :+:==.......................................
// ..........................................................::::....=@#*%**=-. :%*:=#      .+=           :*+  =-.=-.....................................
// ......................................................:==+=..:=+++*%. .   .-*%--: .==      .+=.         .-*: :+ .=-...................................
// ....................................................-=+*+:-*%@@@@@@%--:-=*+##::     .+-      .+=           -=:-#. -+..................................
// ..................................................-+=*+-*@@@@@@@@@@@---:..+%-         :+-      .++.  .--:------==--=+-................................
// ................................................:==**-#@@@@@@###*=:* .:-+%@.-           :+:       =-==++===--:--=*#***=...............................
// ...............................................===#-*@@@@@%*+%@#++**+=--:#- -             -*.      --.             *@@*=-.............................
// ..............................................+-**=@@@@@%++%%+..        -+: .              .#-                      +@@%=+............................
// ............................................:+-#+*@@@@%+++: -+          %.-- ..          --:.-=-                     +@@@=+...........................
// ...........................................:+-%=%@@@@*++:   =%:   .:--::%* .=..:      :-- -=-  .=-                    *@@#+-..........................
// ..........................................:+=%=@@@@%=##  .--=++==-:.:---.*#: :-.::  -=: -+:      .=:     .::::::::::::.#@@:+..........................
// .........................................-+=#+@@@@*=***.  +-    :::-::::-+%#=  :-.-=. :+-          :+..:.             .-*@-*..........................
// ........................................:==*#@@@@=*+=. --. .=*++----::--=::++=:  :=. ==           .:    .:------------::.:#+..........................
// .......................................:+=*#@@@#-%==.    -==-.  .-===:.. :-=++:=.  :=:            .----=====+*****####**#*=-..........................
// ......................................:+-*%@%#++#:.       :-----:. .-----:   -%--=-::.      :----+++====-:.          =@@@%=:..........................
// ......................................*:*@@***:--    :--::. .:-----.        ==%@#+=++====---==+*#-:-::               .@@*-=...........................
// .....................................=**%#*%%*: :=-::   :==-:.            :+. *#=   :----#*:-+#*=-==-.:::::::::::---. *++=............................
// .....................................:=+*#@=  +-  .:-=--=:              .+-  :++=*.      .%:   .=*#+--=-            .:=#:.............................
// ........................................-*.    :-*=-= :=               ==   =-  +:*.   =: .#.      :+#+.-=-  ...:::::-+%..............................
// .......................................+=       .+:+ --              :+.  :+.    *-%-    :.:#         -#-.=#**+=-::....*-.............................
// .....................................:+.       .#.+ :-             .+-   =-     =::*@*.     :#.    .-+**=-+*==--:-=++*#=+.............................
// ....................................-+         #.+  =             ==   -+.    .+   :+#%=     :#:-*#*=. .==:   -+=+#=----=.............................
// ...................................==         *=.+ +.           :+.   +-     :+       -=*+:  .+#+: .-=+*+===-:  =-%=..................................
// ..................................=+       :.*= := *          .=:   -=      =-      .+.  .:=+*%=:=++        .:===*@=..................................
// ...................................*-      +-=  .+ *         =-   .+.      *:      :*      :*.#@*.:#*--:..       :+#:.................................
// ................................... ++     +=-   * +       -+.   ==      .*.      ==      :#. *%#=. .#-:-=++++=:   .*.................................
// ...................................:.:*-   :%-    =+=.   .+:   :+.      :=    :::*-      -#. :%::*    *.      .-=+- *.................................
// ....................................:- -*=.-#-      =*+.==    +=       .:   ==:++       :#  :#:..*.   :+  ...     :*#.................................
// .....................................:+. .-* ==.......:*=   :*.          -==:+*:       :#. :*:...:%=--=***+===+*+-  #:................................
// .......................................+- =#--+=--==--===*=+=        :---:-*%:        -#. :*:.....**      .:----::=+#=................................
// ........................................=-%*  +.  :.     : -==-::::::.:=++: .+=      -#  -*:......:@:           .-=-+#................................
// ..........................................-=  *   :      :    .**===+=:       =*-   -#  -*.........+%               =@:...............................
// ..........................................=-  *   -       .   -*           .-:  :=:=+  =*..........:%+               +=...............................
// ..........................................=-  *   =          .# :        --.      ++  +%=...........-@-            .-+++++:...........................
// ..........................................=:  *   =          *:-.     :-:      -++-:+*+-+............#%         :--=*#+:.-=...........................
// ..........................................-: .+   =         +=-:   .--.    .-==: ---#= *-............-@=      --=*#=.     +...........................
// ..........................................-- -+   =        :#.-  :-:    .==- .-=-  *=:*:..............*#    -==%+.     .:-*...........................
// ..........................................:= ==   =        #.-.::    .==- .==-    =%=%:...............-@. --=%+.   :--===:==..........................
// ..........................................:+ +-   =       ==+:.   :==: .--:    .-+#.==.................%*--#+.  :----::+-:=*..........................
// ...........................................* *:   =.     .*=   -==: .::.   :---: *  %..................*.**. .==:--:=-:.* :%..........................
// ...........................................* #.   -:     *+-==-.   .   :---.     =  %:.................+*. :=-.=-:=:    +-.@:.........................
// ...........................................*.%    .=  .-=+-.  ....:----.    .::-:=-::*.................:%:+-.--.=.      .# %-.........................
			
// Limb pain with buffs level >=BADLY: draining health
			if (!player.m_InediaInfectedAI_Killed && !player.InediaInfectedAI_IsNPC()) {
				if (player.m_InediaInfectedAI_ConfigPlayers.HealthDrainWithCriticalPainLimbPerSecondPercent > 0 || player.m_InediaInfectedAI_ConfigPlayers.HealthDrainWithRedPainLimbPerSecondPercent > 0) {
					float decreaseHealthPerSecond = 0;
					
					foreach (string inediaLimb: InediaInfectedAI_PainManager.m_InediaPainLimbs) {
						int limbPainWithBuffsLevel = player.InediaInfectedAI_GetPainManager().GetPainWithBuffsLevelForLimb(inediaLimb);
						
						if (limbPainWithBuffsLevel >= InediaInfectedAI_LimbPainLevels.CRITICAL) {
							if (player.m_InediaInfectedAI_ConfigPlayers.HealthDrainWithCriticalPainLimbPerSecondPercent > 0) {
								decreaseHealthPerSecond += player.m_InediaInfectedAI_ConfigPlayers.HealthDrainWithCriticalPainLimbPerSecondPercent;
							}
						} else if (limbPainWithBuffsLevel >= InediaInfectedAI_LimbPainLevels.BADLY) {
							if (player.m_InediaInfectedAI_ConfigPlayers.HealthDrainWithRedPainLimbPerSecondPercent > 0) {
								decreaseHealthPerSecond += player.m_InediaInfectedAI_ConfigPlayers.HealthDrainWithRedPainLimbPerSecondPercent;
							}
						}
					}
					
					if (decreaseHealthPerSecond > 0) {
						float healthRegenSpeed = player.GetHealthRegenSpeed();
						if (healthRegenSpeed > 0) {
						    decreaseHealthPerSecond += healthRegenSpeed;
						}
						
						player.DecreaseHealth("GlobalHealth", "Health", decreaseHealthPerSecond * pDt);
					}
				}
			}
		}
	}
	
// APPLY MEDICINE BUFFS HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_ApplyMedicineBuffsHandler(PlayerBase player, float pDt)
	{
		if (!player.GetModifiersManager()) {
			return;
		}
		
// Health regen
		if (player.GetModifiersManager().IsModifierActive(InediaInfectedAI_eModifiers.MDF_HEALTH_REGEN)) {
			float healthRegenMultiplier = player.InediaInfectedAI_GetPainManager().m_HealthRegenItemRegenMultiplier - 1;
			if (healthRegenMultiplier > 0) {
				float healthRegenSpeed = player.GetHealthRegenSpeed();
				
				if (healthRegenSpeed > 0) {
					player.AddHealth("", "Health", healthRegenSpeed * healthRegenMultiplier * pDt);
				}
			}
		}
		
// Blood regen
		if (player.GetModifiersManager().IsModifierActive(InediaInfectedAI_eModifiers.MDF_BLOOD_REGEN)) {
			float bloodRegenMultiplier = player.InediaInfectedAI_GetPainManager().m_BloodRegenItemRegenMultiplier - 1;
			if (bloodRegenMultiplier > 0) {
				if (player.m_InediaInfectedAI_VanillaBloodRegenMdfr) {
					float regen_modifier_water = player.m_InediaInfectedAI_VanillaBloodRegenMdfr.GetRegenModifierWater(player.GetStatWater().Get());
					float regen_modifier_energy = player.m_InediaInfectedAI_VanillaBloodRegenMdfr.GetRegenModifierEnergy(player.GetStatEnergy().Get());
		
					float bloodRegenSpeed = PlayerConstants.BLOOD_REGEN_RATE_PER_SEC * regen_modifier_water * regen_modifier_energy;
		
					player.AddHealth("", "Blood", bloodRegenSpeed * bloodRegenMultiplier * pDt);
				}
			}
		}
	
// Bone regen
		if (player.InediaInfectedAI_GetPainManager().LimbsBreakSystemIsActive()) {
			foreach (string inediaBrokenLimb: InediaInfectedAI_PainManager.m_InediaPainLimbs) {
				if (inediaBrokenLimb == "legs") {
					// For legs, the vanilla fracture system is used.
					if (player.GetBrokenLegs() != eBrokenLegs.NO_BROKEN_LEGS) {
						float legsBoneRegenMultiplier = 1;
						
						if (player.InediaInfectedAI_GetPainManager().IsLimbStabilized("legs")) {
							legsBoneRegenMultiplier *= player.InediaInfectedAI_GetPainManager().GetBreakStabilizationRegenerationMultiplierForLimb("legs");
						}
						
						if (player.GetModifiersManager().IsModifierActive(InediaInfectedAI_eModifiers.MDF_BONE_REGEN)) {
							legsBoneRegenMultiplier *= player.InediaInfectedAI_GetPainManager().m_BoneRegenItemRegenMultiplier;
						}
						
						if (legsBoneRegenMultiplier < 1) legsBoneRegenMultiplier = 1;
						
						player.InediaInfectedAI_GetPainManager().SyncBreakRegenerationLevelForLimbServer("legs", legsBoneRegenMultiplier);
						
						// Additional regeneration to be able to affect the recovery speed of a vanilla fracture with a multiplier.
						float legsAdditionalRegen;
						if (player.m_InediaInfectedAI_ConfigPlayers.LegsBreakDurationSeconds > 0) {
							float vanillaLegsRegenPerSecond = PlayerConstants.LEG_HEALTH_REGEN_BROKEN / 3;
							legsAdditionalRegen = vanillaLegsRegenPerSecond * (100 / vanillaLegsRegenPerSecond / player.m_InediaInfectedAI_ConfigPlayers.LegsBreakDurationSeconds * legsBoneRegenMultiplier - 1);
						} else {
							legsAdditionalRegen = player.GetMaxHealth("RightLeg", "Health");
						}
						
						player.AddHealth("RightLeg", "Health", legsAdditionalRegen);
						player.AddHealth("RightFoot", "Health", legsAdditionalRegen);
						player.AddHealth("LeftLeg", "Health", legsAdditionalRegen);
						player.AddHealth("LeftFoot", "Health", legsAdditionalRegen);
					}
				} else if (player.InediaInfectedAI_GetPainManager().IsLimbBroken(inediaBrokenLimb)) {
					float limbBreakTimeRemainingSeconds = player.InediaInfectedAI_GetPainManager().GetBreakTimeRemainingSecondsForLimb(inediaBrokenLimb);
					if (limbBreakTimeRemainingSeconds > 0) {
						float boneRegenMultiplier = 1;
						
						if (player.InediaInfectedAI_GetPainManager().IsLimbStabilized(inediaBrokenLimb)) {
							boneRegenMultiplier *= player.InediaInfectedAI_GetPainManager().GetBreakStabilizationRegenerationMultiplierForLimb(inediaBrokenLimb);
						}
						
						if (player.GetModifiersManager().IsModifierActive(InediaInfectedAI_eModifiers.MDF_BONE_REGEN)) {
							boneRegenMultiplier *= player.InediaInfectedAI_GetPainManager().m_BoneRegenItemRegenMultiplier;
						}
						
						if (boneRegenMultiplier < 1) {
							boneRegenMultiplier = 1;
						}
						
						player.InediaInfectedAI_GetPainManager().SyncBreakRegenerationLevelForLimbServer(inediaBrokenLimb, boneRegenMultiplier);
						
						if (boneRegenMultiplier > 1) {
							float additionalBoneRegen = boneRegenMultiplier - 1;
							player.InediaInfectedAI_GetPainManager().SetBreakTimeRemainingSecondsForLimb(inediaBrokenLimb, limbBreakTimeRemainingSeconds - pDt * additionalBoneRegen);
						}	
					}
				}
			}
		}
		
// Deep wound regen
		if (player.InediaInfectedAI_GetPainManager().LimbsDeepWoundSystemIsActive()) {
			foreach (string inediaWoundedLimb: InediaInfectedAI_PainManager.m_InediaPainLimbs) {
				if (player.InediaInfectedAI_GetPainManager().IsLimbDeepWounded(inediaWoundedLimb)) {
					float limbDeepWoundTimeRemainingSeconds = player.InediaInfectedAI_GetPainManager().GetDeepWoundTimeRemainingSecondsForLimb(inediaWoundedLimb);
					if (limbDeepWoundTimeRemainingSeconds > 0) {
						// Regenerate the wound itself over time.
						float deepWoundRegenMultiplier = 1;
						
						if (player.InediaInfectedAI_GetPainManager().IsLimbDeepWoundBandaged(inediaWoundedLimb)) {
							deepWoundRegenMultiplier *= player.InediaInfectedAI_GetPainManager().GetDeepWoundBandageRegenerationMultiplierForLimb(inediaWoundedLimb);
							
							// Reduction in bandage effectiveness, if present, and synchronization of effectiveness status.
							float newEffectivenessTimeRemainingSeconds = player.InediaInfectedAI_GetPainManager().GetDeepWoundBandageEffectivenessTimeRemainingSecondsForLimb(inediaWoundedLimb) - pDt;
							player.InediaInfectedAI_GetPainManager().SetDeepWoundBandageEffectivenessTimeRemainingSecondsForLimb(inediaWoundedLimb, newEffectivenessTimeRemainingSeconds);
						
							if (newEffectivenessTimeRemainingSeconds <= 0) {
								if (player.InediaInfectedAI_GetPainManager().IsLimbDeepWoundBandageEffective(inediaWoundedLimb)) {
									player.InediaInfectedAI_GetPainManager().SetDeepWoundBandageEffectivenessForLimb(inediaWoundedLimb, false);
								}
							} else {
								if (!player.InediaInfectedAI_GetPainManager().IsLimbDeepWoundBandageEffective(inediaWoundedLimb)) {
									player.InediaInfectedAI_GetPainManager().SetDeepWoundBandageEffectivenessForLimb(inediaWoundedLimb, true);
								}
							}
						}
						
						if (player.GetModifiersManager().IsModifierActive(InediaInfectedAI_eModifiers.MDF_DEEP_WOUND_REGEN)) {
							deepWoundRegenMultiplier *= player.InediaInfectedAI_GetPainManager().m_DeepWoundRegenItemRegenMultiplier;
						}
						
						if (deepWoundRegenMultiplier < 1) {
							deepWoundRegenMultiplier = 1;
						}
						
						player.InediaInfectedAI_GetPainManager().SyncDeepWoundRegenerationLevelForLimbServer(inediaWoundedLimb, deepWoundRegenMultiplier);
						
						if (deepWoundRegenMultiplier > 1) {
							float additionalDeepWoundRegen = deepWoundRegenMultiplier - 1;
							player.InediaInfectedAI_GetPainManager().SetDeepWoundTimeRemainingSecondsForLimb(inediaWoundedLimb, limbDeepWoundTimeRemainingSeconds - pDt * additionalDeepWoundRegen);
						}
					}
				}
			}
		}

// Blood clotting (InediaInfectedAI_eModifiers.MDF_BLOOD_CLOTTING) is handled in the methods where bleeding speed is calculated.
// InediaInfectedAI_PainManager.GetDeepWoundsBleedingRateWithMultipliers()
// BleedingSource.OnUpdateServer()
	}
};