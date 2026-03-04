/**
 * Created: 2023-09-01
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

/*
To avoid serialization of certain class variable, use NonSerialized attribute:

[NonSerialized()]
string m_dbg; // I don't want to serialize this variable
*/

class InediaInfectedAI_Config
{
	protected ref static InediaInfectedAI_Config m_Instance;
	static float m_InstanceInitLastTime;
	static bool m_InstanceInitError;

	protected static string configDir = "$profile:\\Inedia\\";
	protected static string configFilename = "InediaInfectedAIConfig.json";

	ref InediaInfectedAI_ConfigZombies Zombies = new InediaInfectedAI_ConfigZombies;
	ref InediaInfectedAI_ConfigPlayers Players = new InediaInfectedAI_ConfigPlayers;
	ref InediaInfectedAI_ConfigVehicles Vehicles = new InediaInfectedAI_ConfigVehicles;

	float LossInterestToIrritantAfterSeconds = 120;
	float LightInHouseMultiplier = 0.2;
	float SoundInHouseMultiplier = 0.5;
	float SuppressorShotCooldownSeconds = 5;
	float SuppressorShotOverfireDistanceMeters = 80;

	float BloodParticlesLimit = 1000;
	float BloodParticleDurationSeconds = 600;

	bool SaveConfigAfterInit = false;

	[NonSerialized()]
	ref array<ref InediaInfectedAI_ConfigNavMeshFix> NavMeshChernarusFixes = new array<ref InediaInfectedAI_ConfigNavMeshFix>;
	[NonSerialized()]
	ref array<ref InediaInfectedAI_ConfigNavMeshFix> NavMeshEnochFixes = new array<ref InediaInfectedAI_ConfigNavMeshFix>;

	static InediaInfectedAI_Config GetInstance()
	{
		if (!m_Instance) {
			m_Instance = new InediaInfectedAI_Config;
			m_InstanceInitError = false;

			string configPath = configDir + configFilename;
			if (FileExist(configPath)) {
				// Configuration file exist, load it
				bool configLoaded = InediaInfectedAI_JsonFileLoader<InediaInfectedAI_Config>.JsonLoadFile(configPath, m_Instance);

				m_Instance.Zombies.Initialize();
				m_Instance.Players.Initialize();
				m_Instance.Vehicles.Initialize();

				if (configLoaded) {
					if (m_Instance.SaveConfigAfterInit) {
						InediaInfectedAI_JsonFileLoader<InediaInfectedAI_Config>.JsonSaveFile(configPath, m_Instance);
					}
				} else {
					m_InstanceInitError = true;
					Error("Parsing error in '" + configPath + "' configuration file.\nThe config file will be ignored and the default game config will be used instead.\nCheck the contents of this file using JSON-validator https:\/\/jsonlint.com\nIf the online validator does not find any errors, the issue is most likely a type mismatch. Analyze the error message in this crash log to understand which parameter caused the error.\n\n" + InediaInfectedAI_JsonFileLoader<InediaInfectedAI_Config>.GetLastError());
				}
			} else {
				// Configuration file does not exist, create it
				if (!FileExist(configDir)) {
					MakeDirectory(configDir);
				}

				// Init default options before creating config file
				m_Instance.Zombies.InitializeBeforeCreatingConfigFile();
				m_Instance.Players.InitializeBeforeCreatingConfigFile();
				m_Instance.Vehicles.InitializeBeforeCreatingConfigFile();

				// Create config file
				InediaInfectedAI_JsonFileLoader<InediaInfectedAI_Config>.JsonSaveFile(configPath, m_Instance);
				
				// Init default options after creating config file
				m_Instance.Zombies.InitializeAfterCreatingConfigFile();
			}

			m_Instance.InitNavmeshFixes();
			
			m_InstanceInitLastTime = GetGame().GetTickTime();
		}
		
		return m_Instance;
	}
	
	static void ReloadInstance()
	{
		m_Instance = null;
		GetInstance();
	}

	array<ref InediaInfectedAI_ConfigNavMeshFix> GetNavmeshChernarusFixes()
	{
		return NavMeshChernarusFixes;
	}

	array<ref InediaInfectedAI_ConfigNavMeshFix> GetNavmeshEnochFixes()
	{
		return NavMeshEnochFixes;
	}

	[NonSerialized()]
	protected string InediaInfectedAI_ConfigClass;
	string InediaInfectedAI_GetConfigClass()
	{
		return InediaInfectedAI_ConfigClass;
	}

	[NonSerialized()]
	protected string InediaInfectedAI_ConfigVariable;
	string InediaInfectedAI_GetConfigVariable()
	{
		return InediaInfectedAI_ConfigVariable;
	}

	void InitNavmeshFixes()
	{
		InediaInfectedAI_ConfigClass = "I"+"ned"+"ia"+"I"+"nfec"+"ted"+"A"+"I"+"_"+"C"+"onfi"+"g";
		InediaInfectedAI_ConfigVariable = "m_"+"I"+"ned"+"ia"+"I"+"nfec"+"ted"+"A"+"I"+"_"+"Activ"+"eTargetMin"+"dstate";
	}
}

class InediaInfectedAI_ConfigNavMeshFix
{
	string m_Name;
	vector m_Position;
	float m_Radius;

	ref TVectorArray m_Goals = new TVectorArray();

	void InediaInfectedAI_ConfigNavMeshFix(string pName, vector pPosition, float pRadius)
	{
		m_Name = pName;
		m_Position = pPosition;
		m_Radius = pRadius;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------:::::::::::::::::::::----------------------------------------------------------------
// -----------------------------------------------------------::::::::::::::::::::::::::::::::::::-------------------------------------------------------
// ----------------------------------------------------:::::::::::::::::::::::::::::::::::::::::::::::::-------------------------------------------------
// -----------------------------------------------::::::::::::::::::::::::::::::::::::::--==--:::::::::::::::--------------------------------------------
// -------------------------------------------:::::::::::::::::::::::::::::-+++++++++***+=--======++=-:::::::::::----------------------------------------
// ---------------------------------------:::::::::::::::::::::::::::::-+**+-.........-::::. ::-====-=+++++=-::::::--------------------------------------
// ------------------------------------:::::::::::::::::::::::::::::-+*+-..       -++=:...  .::.    :-=++--=+*=-:::::::----------------------------------
// ---------------------------------:::::::::::::::::::::::::::::=**+:.        :--     .:::::...  ..... .-=+=-===::::::::::------------------------------
// ------------------------------:::::::::::::::::::::::::::::-*#*+=--::...----.    .::.                    :=+=-==::::::::::----------------------------
// ----------------------------:::::::::::::::::::::::::::::-*+:-=.....::=*%*====-=%*-.  .::..              .: .-++#*=:::::::::--------------------------
// -------------------------:::::::::::::::::::::::::::::..-#.:-=.......:--:......::+*@%*.: .:::.                 :+#:-:::::::::::-----------------------
// ------------------------:::::::::::::::::::::::::.......#:-:=:.....:---......:-==:#%@@%.-:  ..:... ...   ...   -::++::::::::::::----------------------
// -----------------------:::::::::::::::::::::::.........=+--:+:::::-:-:.::::---=::+-==+%# =  :::-:..  .--.....   .-::#:::::::::::::--------------------
// ----------------------::::::::::::::::::::::..........:%:+:=:::::::-:::::-==--::=::---#@ ---:...::::::::-::.:=:    -+*::::::::::::::------------------
// ---------------------::::::::::::::::::::.............*==::+::::::---:::+*---::-::-=--%*:+.-::---....  :--..:-+=    -.#:::::::::::::::----------------
// -------------------:::::::::::::::::::::.............-%.+::=:::::==-:::*+--::--::----#=-=.:...::-=--=-:: ::--.:==: .*-*=::::::::::::::::--------------
// -----------------:::::::::::::::::::::...............%.-=:+-::::+*::-++==-::==-----=#==:-:    .   .::-::-:=+:---:.::+-#-:::::::::::::::::-------------
// ---------------:::::::::::::::::::::................+#.*--*----++-+**-==---+==+===**==. .....::-::.   :::-=-#=--==-:+*.#:::::::::::::::::::-----------
// --------------:::::::::::::::::::.................=*=+.@=-*---**==++-*=--+#++*=-+*=--:.===+#%@@@%*++=:  .-.--+==:.:-%*-%::::::::::::::::::::----------
// ------------::::::::::::::::::::..................=**--@+=%**%*==**+#+=+*+-=#=*+=-----:.=@%%@%###%@@@@#: -. -.==#===-=:+*::::::::::::::::::::---------
// -----------::::::::::::::::::::................... *@:+@#+.   :+#%#%*+=#===%#+:-:::=-:+%%#%*++*++**%@@@@=::   :-:- .=*=-**..:::::::::::::::::---------
// ----------:::::::::::::::::::...................   .%:#%:=#%#+.::-*******++=:--...:+-*=*%%*%@*:....=%@@@@*=::.== *. . .  :*-:::::::::::::::::::-------
// ---------::::::::::::::::::::.................     :#:@-+#-+%%%=.=.+ +-=--:-. .:.  :-*---#%@%+:::%-.*@@@@@@%%#+--++:.==   .-#:::::::::::::::::::------
// --------:::::::::::::::::::..................      =#-@:+ +*:*%@##-* +:  ::--::.  :-.:+**##*%*=---=*%@@@@@@@%=. ...%@@%%##:-=#::::::::::::::::::------
// -------:::::::::::::::::::.................        :%=@* :*:-+@@@%:+*=       :--=-.:::--+#@@####**#%@@@%#%#=:--- .--%@#@@%*:.#-.::::::::::::::::::----
// ------::::::::::::::::::::................          =#*#.:%:.#@@=:.%@%+:  .-           :-=+*#**###%##*+==+:--:---::-=+#*%@#-*=....::::::::::::::::----
// ------:::::::::::::::::::................           ++##: =#-+@@= .@@+@@@#-.::-##*=:-+##*=: .=+++++--:-=*+=----.=%@@@+**+%+-.......::::::::::::::::---
// ------::::::::::::::::::................            #+:%+: .*%@@@*:=@+-%%%@@#=-=--+**=:.-*@@#=::::.-*%#*+-=-+.-%@@@%@=#**-.........::::::::::::::::---
// -----::::::::::::::::::................              =#=*:+:.:=:-++=*::.==:##@@#*=------- .#%%*+::*%%+*==:=@ =@@@@%###@%+...........::::::::::::::::--
// -----::::::::::::::::::...............                -%=*.-..-:=-:-::*:- . .%@@@@%%%%+#@= .+%.+--+**+:* :+@.+@@@@#+%+*##............::::::::::::::::-
// ----::::::::::::::::::................                 =*+@%*--::..=@%@#.:.  #@@@@#%%%%:*@. +%:+-*++:.*-=..=##:-=****#=*#-...........::::::::::::::::-
// ---:::::::::::::::::::...............                   %=%*#@#++*%@@@@@%*   *@@*...=%##.%= =#**++::.=**:- :.@.-:#=+@#.: +-..........::::::::::::::::-
// ---:::::::::::::::::::...............                   =#+=:+@#+*%%%@@@@*   *@@*....#+#.#* #%#=+.: .:=+=    #  =-++%%:-+#-..........::::::::::::::::-
// ---:::::::::::::::::::...............                   .%-%:-== .+*#%@@@+   +%#@*=-+#-=.%# @*==-=%@#*@+*@*+%#+-:.:+%#%*-............::::::::::::::::-
// ---:::::::::::::::::::...............                   .%=%%-.= .-+@%@@@= . -@%*%@@#-=-=@*:*++*@#+:-@*.==.=%=#*=%=-%-................::::::::::::::::
// ----::::::::::::::::::...............                   :#=@%%.:  -*%*@@@: . :%@@*@*::: +%+:++=@@%%-*@-.+. %:.#..#+%- ................::::::::::::::::
// --::::::::::::::::::::...............                   =++%-%=:  =#+.#@%=.. .=#@@%-:.:.-%-.*.%@@@@@@@%+%+**. * :*-# ................:::::::::::::::::
// ---:::::::::::::::::::...............               .:-=%-%:-%#-  *** +@#:    .+@@*=::.:-# +--@@@@@@@@#@@@@%**#--#-*.................:::::::::::::::::
// ---:::::::::::::::::::................           :#++==#+%-.=+%:..%@@.#*   :  :-.=*##+---# *:+@+%=#%*==#--=#+@@@@%#:.................:::::::::::::::--
// ---:::::::::::::::::::................          -#..*%#%#-.-:%@.=:*%%=@. .  :-. --..-+##@#-#-=*-::..#+===#*:.*####-  ...............::::::::::::::::--
// ----:::::::::::::::::::...............         +#..-*@@#..--%%@.+:#=*+*#  -:  -=-:=-====+*%#+:**+++=*+:-+=#=+*+*:-#.................:::::::::::::::---
// -----::::::::::::::::::................      .*+  ---** :+*%@*@.+-@:=*.@#  =:   .-+. ::+#%+-=-+-##=. .*-.:*: ::*%#:.................:::::::::::::::---
// ----:::::::::::::::::::.................    :#= :-..+-*+%@#@*-@:-%@*-% *@%. =.:::.:-. :::.-+*=-+..=#:-: .*=++**=-*=................::::::::::::::::---
// -----:::::::::::::::::::.................  -#: .=.   -=+@@@==.@::***=@ .%@@+-%*+*#*+=+*+*=..--++-- -%-*.+-=+*-=**=................::::::::::::::::----
// ------:::::::::::::::::::.................+%.   +     .+=@+.-=@::++-*@. =@@@@@@@@@@@#-. ...::-. -.- =* +=:.+:. %..................::::::::::::::::----
// ------::::::::::::::::::::..............=#%.    -.      +-%=#-@.:*.:*%:..@@@@%#%@@@@@@@@%*--=+*-    :. :.  -   +=................::::::::::::::::-----
// -------::::::::::::::::::::........:=++*+@@: .. .        -=*@+# -+ .##-. @@#%*#*-#@@@@@@@@@@#*##-::      .-. .:.%...............:::::::::::::::::-----
// --------::::::::::::::::::::...:=**##=-. +@@: .:          :+=@- *=  **+: %@:=#.-#@@@%@@#+=#@@@@@@%**=:..-+#@@@=:#...............:::::::::::::::::-----
// --------::::::::::::::::::::-+**+-:  +=*+-=@@= ::           +=+-@:  +=#- ##++:#@%*+**-.-*@%#++#%+=-=++++****#*+#:.............::::::::::::::::::------
// ----------::::::::::::::=+***-:.   .=---.  =@@* .-           :=+%.  *-%-.=@::%%+:**: :##*-:-*#-        ......:+..............::::::::::::::::::-------
// -----------::::::::-=*#**--: .::::::     :--#@@#..:            ==*- *-#==.@#*#-*+.  =++-..*@##=        .....................:::::::::::::::::::-------
// ------------::-=+***+==--:-:-=--.     :--.   #@@%..-            .==*%+-#= #%==%%. :#%*:.:%@%+-*+      ....................::::::::::::::::::::--------
// ------------=**+==**=-:::+--:      :--:=:    -@@@%:.-             -=+%:#=--%#@%-.*@@%- =@@*. * +#.   ....................::::::::::::::::::::---------
// -------------::::......         .. :+...--   .#:%@@-.-              ==*%*+ %%- :%@@+ .#@@=   =.--#:....................::::::::::::::::::::::---------
// ---------------:::::......   .::::.  +-. :=. :* .%@@.=               .=+## =%=#%@%: -@@@#    :- =:%*-................:::::::::::::::::::::::----------
// ----------------::::=--:::::::::::::. -+.  :-:=: *@@=-.                :=*+:@@%#*.--@@@@@    .=  -:#+++=............::::::::::::::::::::::------------
// ------------------::::---::::::::::::. .+::. =-* :@@#.=                  :=*@@*+-+:@@@@@@+    =   -.#*==+*+:......:::::::::::::::::::::::-------------
// --------------------:::::::::::::::::::..+-.  .++ %@@ +                   :-+%#+*-%@@*@@@@-   +   .: **==:=##+-.:::::::::::::::::::::::---------------
// ---------------------::::::::::::::::::::.-=:.  *=+@@:=.               :-::=+=+%+#@@#=@=%@@:  %-   .  +=.-: .:=++==:::::::::::::::::::----------------
// -----------------------:::::::::::::::::::::=:::.+*@@- =           .--::+%@%*@%.-#@@--%+*@@%  %+    :-:%:..:    ..-#::::::::::::::::------------------
// -------------------------::::::::::::::::::::==:::#@@- +        .--::+%@@*-  *@..:.-**##=@@@: #+ -#@@@*#%-+=--.. .:=%=---::::::::::-------------------
// ---------------------------::::::::::::::::::::=:=@@@= =     .--:-+%@@%+.    =@   .: =@**%@@* #+-@@@@@+#=#=.  ::::::::::---:::::----------------------
// -----------------------------:::::::::::::::::. ===@@+ :-.-+*-=*@@%%@@=. ...-*#::    *#=@@@@+ **-@@@@@-#--+. :::::::::::::::::------------------------
// -------------------------------:::::::::::::::::.:==@@##@@@@@@@@@%-.+#@%+. --+#-.  .:%+%@*=.:--=-@@@@@#@=--.:::::::::::::::::-------------------------
// ----------------------------------:::::::::::::::::+=#@@@@@@@#==*@@#=.:+%@*=**+.*=  *%+-  ::  -+@@@@@++@:=:::::::::::::::-----------------------------
// ------------------------------------::::::::::::::::-:=%@@**=.-:..@%@%+.-#@@@@*. .: %==-.  :+%@@@@@#-::#.:::::::::::::--------------------------------
// ---------------------------------------:::::::::::::::::=-::-+:  :#=@@@*-=-+#%* ..  @  .-+%@@%%@@%=::::-::::::::::------------------------------------
// --------------------------------------------::::::::::::::::::-::.-:*@%@@#=::-# ::..% .+%%+-##@@*-*:::::::::::::--------------------------------------
// ----------------------------------------------::::::::::::::::::::::::::=#@%+:*:::::+=#*-:-:--:-:::::::::::-------------------------------------------
// --------------------------------------------------::::::::::::::::::::::::::::-::::::::::::::::::::::-------------------------------------------------
// --------------------------------------------------------::::::::::::::::::::::::::::::::::::::::------------------------------------------------------
// ----------------------------------------------------------------::::::::::::::::::::::::--------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------

class InediaInfectedAI_ConfigPlayers
{
	bool SmellsHandlerIsActive = true;
	float SmellRadiusDefault = 1;
	float SmellRadiusWounded = 6;
	float SmellRadiusBandaged = 3;
	float SmellLifetimeSeconds = 120;
	int SmellsCountOnMapLimit = 1000;
	bool FootstepsNoiseHandlerIsActive = true;
	bool SearchBodyToViewCargo = true;
	float SearchBodyToViewCargoSeconds = 3;
	float SearchBodyExtendLootingToPlayersInRadiusMeters = 30;
	bool SearchBodyOnlyEmptyHands = false;

	bool QuietDoorOpeningMechanicIsActive = true;
	bool QuietDoorOpeningMechanicDisabledForOpening = true;
	float QuietDoorOpeningMechanicSeconds = 3;
	ref array<string> QuietDoorOpeningMechanicRestrictedBuildings = new array<string>;

	/*
	float FootstepsNoiseJogRadius = 30;
	float FootstepsNoiseCrouchSprintRadius = 10;
	float FootstepsNoiseSprintRadius = 40;
	*/
	
	bool DisableVanillaDoorNoiseSystem = true;
	
	bool OverrideGetHitComponentChances = true;

	bool PainSystemIsActive = true;
	float PainSystemNotificationsType = 2;
	bool ShowPainBadges = true;
	bool ShowPainBlur = true;
	bool ShowMedicationInfo = true;
	float PainHealingRatePerSecondPercent = 0.1;
	float HealthDrainWithCriticalPainLimbPerSecondPercent = 0.15;
	float HealthDrainWithRedPainLimbPerSecondPercent = 0.0;
	float VehicleCrashPainMultiplier = 1;
	float VehicleCollisionsPainMultiplier = 1;
	float FallPainMultiplier = 1;
	float ExplosionsPainMultiplier = 1;
	float FireDamagePainMultiplier = 1;
	bool UnconsciousWhenOverPainedHeadReceivingHits = true;
	bool CrouchWhenOverPainedTorsoReceivingHits = true;
	bool ArmsLowerWhenAimingWithPainedArms = true;
	bool ArmsLowerWhenOverPainedArmsReceivingHits = true;
	ref map<string, float> ArmsPainShockDamageActions = new map<string, float>;
	[NonSerialized()]
	ref map<string, float> ArmsPainShockDamageDefaultActions = new map<string, float>;
	ref map<string, float> ArmsPainUnrestrainItems = new map<string, float>;
	bool AuditoryHallucinationsWithPainedHead = true;
	bool VomitWithPainedHead = true;
	bool TinnitusWhenHeadReceivingHits = true;
	bool BlurWhenHeadReceivingHits = true;
	bool SoundAttenuationWhenHeadReceivingHits = true;
	bool WeaponSwayWhenHeadReceivingHits = true;
	bool MovementCoordinationImpairmentWhenHeadReceivingHits = true;
	bool PainArmsWhenHitWithoutGloves = true;
	bool ShockIfRunsWithPainedLegs = true;
	bool ShockIfRunsWithPainedArms = true;
	bool ShockIfAttackingWithPainedArms = true;
	float DisableStaminaRegenWhenTorsoPainDurationSeconds = 10;
	
	bool LimbsBreakSystemIsActive = true;
	float LimbsBreakPainThresholdPercent = 30;
	float HeadBreakDurationSeconds = 1800;
	float ArmsBreakDurationSeconds = 1800;
	float LegsBreakDurationSeconds = 1800;
	float TorsoBreakDurationSeconds = 1800;
	bool LimbsBreakFromFalls = true;
	bool LegsBreakFromTraps = true;
	bool UnconsciousWhenHeadBreak = true;
	bool DisableVanillaBrokenLegWalk = true;
	
	bool LimbsDeepWoundSystemIsActive = true;
	float HeadDeepWoundDurationSeconds = 1800;
	float ArmsDeepWoundDurationSeconds = 1800;
	float LegsDeepWoundDurationSeconds = 1800;
	float TorsoDeepWoundDurationSeconds = 1800;
	float LimbsDeepWoundConsciousBleedRateMultiplier = 1;
	float LimbsDeepWoundUnconsciousBleedRateMultiplier = 1;
	float LimbsDeepWoundVanillaCutBleedRateMultiplier = 0.1;
	bool LimbsDeepWoundFromFalls = true;
	bool LegsDeepWoundFromTraps = true;
	
	bool LimbsBulletSystemIsActive = true;
	float LimbsBulletTimeToSecondStageSeconds = 7200;
	
	bool InternalBleedingSystemIsActive = true;
	float InternalBleedingBloodLossRateMl = 2;
	
	float DestroyedDoorNotificationType = 2;

	float NoiseMultiplierDay = 2;
	float NoiseMultiplierNight = 2;
	float NoiseMultiplierCrouchDay = 1;
	float NoiseMultiplierCrouchNight = 1;
	float NoiseMultiplierLadderDay = 0.2;
	float NoiseMultiplierLadderNight = 0.2;

	ref map<string, float> CamoClothingVisibilityMultipliers = new map<string, float>;

	void InitializeBeforeCreatingConfigFile()
	{
		Initialize();

		QuietDoorOpeningMechanicRestrictedBuildings.Insert("DNA_Strongroom_ColourBase");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("DNA_Strongroom_Open_Base");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("DNA_Lockout_ColourBase");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("DNA_Lockout_OneWayDoor");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("DNA_Door_NoLock_ColourBase");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("DNA_WarpDoor_NoLock");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("DNA_Lockout_Dbl_ColourBase");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("DNA_Door_NoLock_Dbl_ColourBase");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("DNA_LockoutSteel_ColourBase");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("Land_KlimaX_T1Door");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("Land_KlimaX_T2Door");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("Land_KlimaX_T3Door");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("EVG_KeycardDoor");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("land_bunker_door_base");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("land_bunker_entrance");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("land_bunker_entrance1");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("land_bunker_entrance2");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("land_bunker_door_area04");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("Land_WarheadStorage_Main");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("Jmc_Security_Door");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("Jmc_Security_Door_Carrier");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("jmc_dungeon_Door06_Double");
		QuietDoorOpeningMechanicRestrictedBuildings.Insert("ContainerLockedBase");
		
		ArmsPainUnrestrainItems["MetalWireLocked"] = 4;
		ArmsPainUnrestrainItems["BarbedWireLocked"] = 3;
		ArmsPainUnrestrainItems["HandcuffsLocked"] = 8;

		CamoClothingVisibilityMultipliers["GhillieTop_ColorBase"] = 0.8;
		CamoClothingVisibilityMultipliers["GhillieHood_ColorBase"] = 0.8;
		CamoClothingVisibilityMultipliers["GhillieSuit_ColorBase"] = 0.8;
	}

	void Initialize()
	{
		if (!ArmsPainShockDamageActions) {
			ArmsPainShockDamageActions = new map<string, float>;
		}
		if (!ArmsPainShockDamageActions.Contains("all")) {
			ArmsPainShockDamageActions["all"] = 2;
		}
		
		if (!ArmsPainShockDamageDefaultActions) {
			ArmsPainShockDamageDefaultActions = new map<string, float>;
		}
		ArmsPainShockDamageDefaultActions["ActionConsume"] = 0;
		ArmsPainShockDamageDefaultActions["ActionForceConsume"] = 0;
		ArmsPainShockDamageDefaultActions["ActionConsumeSingle"] = 0;
		ArmsPainShockDamageDefaultActions["ActionForceConsumeSingle"] = 0;
		ArmsPainShockDamageDefaultActions["ActionDrinkPondContinuous"] = 0;
		ArmsPainShockDamageDefaultActions["ActionDrinkWellContinuous"] = 0;
		ArmsPainShockDamageDefaultActions["ActionLightItemOnFire"] = 0;
		ArmsPainShockDamageDefaultActions["ActionCookOnStick"] = 0;
		ArmsPainShockDamageDefaultActions["ActionInjectSelf"] = 0;
		ArmsPainShockDamageDefaultActions["ActionInjectTarget"] = 0;
		ArmsPainShockDamageDefaultActions["ActionDisinfectSelf"] = 0;
		ArmsPainShockDamageDefaultActions["ActionDisinfectTarget"] = 0;
		ArmsPainShockDamageDefaultActions["ActionUnrestrainSelf"] = 1;
		ArmsPainShockDamageDefaultActions["ActionPushCar"] = 4;
		ArmsPainShockDamageDefaultActions["ActionMineRock"] = 4;
		ArmsPainShockDamageDefaultActions["ActionMineTree"] = 4;
		ArmsPainShockDamageDefaultActions["ActionGiveBloodSelf"] = 0;
		ArmsPainShockDamageDefaultActions["ActionGiveBloodTarget"] = 0;
		ArmsPainShockDamageDefaultActions["ActionGiveSalineSelf"] = 0;
		ArmsPainShockDamageDefaultActions["ActionGiveSalineTarget"] = 0;
		
		if (!ArmsPainUnrestrainItems) {
			ArmsPainUnrestrainItems = new map<string, float>;
		}
		if (!ArmsPainUnrestrainItems.Contains("all")) {
			ArmsPainUnrestrainItems["all"] = 2;
		}
		
		if (!CamoClothingVisibilityMultipliers) {
			CamoClothingVisibilityMultipliers = new map<string, float>;
		}
		if (!CamoClothingVisibilityMultipliers.Contains("all")) {
			CamoClothingVisibilityMultipliers["all"] = 1;
		}
		
		if (DisableStaminaRegenWhenTorsoPainDurationSeconds < 1) {
			DisableStaminaRegenWhenTorsoPainDurationSeconds = 1;
		}
	}
	
	float GetArmsPainShockDamageForAction(ActionBase_Basic action)
	{
		if (!action) {
			return 0;
		}
		
		float actionShockDamage = -1;
		
		foreach (string actionConfigClass, float actionConfigValue : ArmsPainShockDamageActions) {
	        if (actionConfigClass != "all" && action.IsInherited(actionConfigClass.ToType())) {
				actionShockDamage = actionConfigValue;
				break;
			}
        }
		
		if (actionShockDamage == -1) {
			foreach (string defaultActionConfigClass, float defaultActionConfigValue : ArmsPainShockDamageDefaultActions) {
		        if (defaultActionConfigClass != "all" && action.IsInherited(defaultActionConfigClass.ToType())) {
					actionShockDamage = defaultActionConfigValue;
					break;
				}
	        }
		}
		
		if (actionShockDamage == -1) {
			if (!ArmsPainShockDamageActions.Find("all", actionShockDamage)) {
				actionShockDamage = 2;
			}
		}

		if (actionShockDamage < 0) {
			actionShockDamage = 0;
		}
		
		return actionShockDamage;
	}
	
	float GetArmsPainUnrestrainForItem(EntityAI item)
	{
		if (!item) {
			return 0;
		}
		
		float itemPainDamage = -1;
		
		if (!ArmsPainUnrestrainItems.Find(item.GetType(), itemPainDamage)) {
			itemPainDamage = -1;
			foreach (string itemConfigClass, float itemConfigValue : ArmsPainUnrestrainItems) {
		        if (itemConfigClass != "all" && InediaInfectedAI_Functions.IsKindOfCached(itemConfigClass, item)) {
					itemPainDamage = itemConfigValue;
					break;
				}
	        }
		}

		if (itemPainDamage == -1) {
			if (!ArmsPainUnrestrainItems.Find("all", itemPainDamage)) {
				itemPainDamage = 2;
			}
		}

		if (itemPainDamage < 0) {
			itemPainDamage = 0;
		}

		return itemPainDamage;
	}

	float GetClothingVisibilityMultiplier(Object cloth)
	{
		if (!cloth) {
			return 1;
		}

		float multiplier = -1;

		if (!CamoClothingVisibilityMultipliers.Find(cloth.GetType(), multiplier)) {
			multiplier = -1;
			foreach (string clothConfigClass, float clothConfigValue : CamoClothingVisibilityMultipliers) {
		        if (clothConfigClass != "all" && InediaInfectedAI_Functions.IsKindOfCached(clothConfigClass, cloth)) {
					multiplier = clothConfigValue;
					break;
				}
	        }
		}

		if (multiplier == -1) {
			if (!CamoClothingVisibilityMultipliers.Find("all", multiplier)) {
				multiplier = 1;
			}
		}

		if (multiplier < 0) {
			multiplier = 1;
		}

		return multiplier;
	}
}

class InediaInfectedAI_ConfigVehicles
{
	ref map<string, ref map<string, float>> Noise;
	ref map<string, float> CollisionsSoundThreshold;
	ref map<string, float> CollisionsSpeedReductionMultiplier;
	ref map<string, float> CollisionsDamageSpeedThresholdKmH;

	void InitializeBeforeCreatingConfigFile()
	{
		Initialize();

		Noise["Offroad_02"] = new map<string, float>;
		Noise["Offroad_02"]["RpmIdleMeters"] = 50;
		Noise["Offroad_02"]["RpmMaxMeters"] = 140;

		Noise["Truck_01_Base"] = new map<string, float>;
		Noise["Truck_01_Base"]["RpmIdleMeters"] = 60;
		Noise["Truck_01_Base"]["RpmMaxMeters"] = 150;

		Noise["Bicycle_Base"] = new map<string, float>;
		Noise["Bicycle_Base"]["RpmIdleMeters"] = 20;
		Noise["Bicycle_Base"]["RpmMaxMeters"] = 40;
		
		CollisionsDamageSpeedThresholdKmH["Bicycle_Base"] = 0;
	}

	void Initialize()
	{
		if (!Noise) {
			Noise = new map<string, ref map<string, float>>;
		}
		if (!Noise.Contains("all")) {
			map<string, float> NoiseAllMap = new map<string, float>;
			NoiseAllMap["RpmIdleMeters"] = 40;
			NoiseAllMap["RpmMaxMeters"] = 120;
			Noise["all"] = NoiseAllMap;
		}

		if (!CollisionsSoundThreshold) {
			CollisionsSoundThreshold = new map<string, float>;
		}
		if (!CollisionsSoundThreshold.Contains("all")) {
			CollisionsSoundThreshold["all"] = 50;
		}

		if (!CollisionsSpeedReductionMultiplier) {
			CollisionsSpeedReductionMultiplier = new map<string, float>;
		}
		if (!CollisionsSpeedReductionMultiplier.Contains("all")) {
			CollisionsSpeedReductionMultiplier["all"] = 1;
		}
		
		if (!CollisionsDamageSpeedThresholdKmH) {
			CollisionsDamageSpeedThresholdKmH = new map<string, float>;
		}
		if (!CollisionsDamageSpeedThresholdKmH.Contains("all")) {
			CollisionsDamageSpeedThresholdKmH["all"] = 10;
		}
	}

// ENdbAALU+E+t5Hkl2qD9QVVLQXZnejhkdytLVUtvQmNDY0dNMklDcmQ3L1RxYWJjV2VuR0g0Ymp4VjlwL2d3VUlLcEgxaFJMYU8vaXVzemR4bzJDUW5ZQ0ZrQ0k2ME5CQ2dYcmt5c2tCU3ZZbGxhY1ROb2FrVTZJdHhNbXlXRnIyampnZE5MTm9JSDhmTDlLcWU1TFowN0FreHdjOWMyVDNHRlByTFFTNW9ONktDNUhjcHdGc3oxNGs5VXUxL3FZam9XUGVQdXBqeERZMGtqTVJ6V3FUNXhCQnBoVUdZTnl2MGI3VGQxa2dVOTAxVVZFNzVocWMxVERqcUxtZGlFRW5LaTNjVStoS3Vaa1NlQWt1bkhhTG1qRVZFby9pbkxienNKU3FRPT0=

	float GetCollisionsSoundThreshold(Object vehicle)
	{
		if (!vehicle) {
			return 50;
		}

		float speedThreshold = -1;
		
		if (!CollisionsSoundThreshold.Find(vehicle.GetType(), speedThreshold)) {
			speedThreshold = -1;
			foreach (string vehicleConfigClass, float vehicleConfigValue : CollisionsSoundThreshold) {
		        if (vehicleConfigClass != "all" && InediaInfectedAI_Functions.IsKindOfCached(vehicleConfigClass, vehicle)) {
					speedThreshold = vehicleConfigValue;
					break;
				}
	        }
		}

		if (speedThreshold == -1) {
			if (!CollisionsSoundThreshold.Find("all", speedThreshold)) {
				speedThreshold = 50;
			}
		}

		if (speedThreshold < 0) {
			speedThreshold = 0;
		}

		return speedThreshold;
	}

	float GetCollisionsSpeedReductionMultiplier(Object vehicle)
	{
		if (!vehicle) {
			return 1;
		}

		float speedReductionMultiplier = -1;
		
		if (!CollisionsSpeedReductionMultiplier.Find(vehicle.GetType(), speedReductionMultiplier)) {
			speedReductionMultiplier = -1;
			foreach (string vehicleConfigClass, float vehicleConfigValue : CollisionsSpeedReductionMultiplier) {
		        if (vehicleConfigClass != "all" && InediaInfectedAI_Functions.IsKindOfCached(vehicleConfigClass, vehicle)) {
					speedReductionMultiplier = vehicleConfigValue;
					break;
				}
	        }
		}

		if (speedReductionMultiplier == -1) {
			if (!CollisionsSpeedReductionMultiplier.Find("all", speedReductionMultiplier)) {
				speedReductionMultiplier = 1;
			}
		}

		if (speedReductionMultiplier < 0) {
			speedReductionMultiplier = 0;
		}

		return speedReductionMultiplier;
	}
	
	float GetCollisionsDamageSpeedThresholdKmH(Object vehicle)
	{
		if (!vehicle) {
			return 10;
		}

		float damageSpeedThresholdKmH = -1;
		
		if (!CollisionsDamageSpeedThresholdKmH.Find(vehicle.GetType(), damageSpeedThresholdKmH)) {
			damageSpeedThresholdKmH = -1;
			foreach (string vehicleConfigClass, float vehicleConfigValue : CollisionsDamageSpeedThresholdKmH) {
		        if (vehicleConfigClass != "all" && InediaInfectedAI_Functions.IsKindOfCached(vehicleConfigClass, vehicle)) {
					damageSpeedThresholdKmH = vehicleConfigValue;
					break;
				}
	        }
		}

		if (damageSpeedThresholdKmH == -1) {
			if (!CollisionsDamageSpeedThresholdKmH.Find("all", damageSpeedThresholdKmH)) {
				damageSpeedThresholdKmH = 10;
			}
		}

		if (damageSpeedThresholdKmH < 0) {
			damageSpeedThresholdKmH = 10;
		}

		return damageSpeedThresholdKmH;
	}

	float GetNoiseRadius(Object vehicle, float rpmIdle, float rpmMax, float rpmCurrent)
	{
		float radius;

		if (!vehicle) {
			return 0;
		}

		map<string, float> noiseRanges;
		
		if (!Noise.Find(vehicle.GetType(), noiseRanges)) {
			foreach (string vehicleConfigClass, map<string, float> vehicleConfigValue : Noise) {
		        if (vehicleConfigClass != "all" && InediaInfectedAI_Functions.IsKindOfCached(vehicleConfigClass, vehicle)) {
					noiseRanges = vehicleConfigValue;
					break;
				}
	        }
		}

		if (!noiseRanges) {
			if (!Noise.Find("all", noiseRanges)) {
				noiseRanges = new map<string, float>;
				noiseRanges["RpmIdleMeters"] = 40;
				noiseRanges["RpmMaxMeters"] = 120;
			}
		}

		float rpmIdleMeters;
		if (!noiseRanges.Find("RpmIdleMeters", rpmIdleMeters) || rpmIdleMeters < 0) {
			rpmIdleMeters = 0;
		}

		float rpmMaxMeters;
		if (!noiseRanges.Find("RpmMaxMeters", rpmMaxMeters) || rpmMaxMeters < 0) {
			rpmMaxMeters = 0;
		}

		float rpmMetersDelta = rpmMaxMeters - rpmIdleMeters;
		float rpmCurrentDelta = rpmCurrent - rpmIdle;
		float rpmDelta = rpmMax - rpmIdle;

		if (rpmDelta == 0) {
			radius = rpmIdleMeters;
		} else {
			radius = rpmIdleMeters + rpmMetersDelta * rpmCurrentDelta / rpmDelta;
		}

		if (radius < 0) {
			radius = 0;
		}

		return radius;
	}
}

class InediaInfectedAI_ConfigZombies
{
    ref map<string, ref array<string>> Groups;
	ref map<string, ref InediaInfectedAI_CustomIrritant> CustomIrritants;
	ref map<string, ref InediaInfectedAI_ConfigThrowingProjectile> ThrowingProjectiles;

    ref map<string, bool> BreakingDoorsHandlerIsActive;
	ref map<string, ref map<string, ref array<int>>> BreakingDoorsRestrictedDoors;
    ref map<string, float> BreakingDoorsDestroyAfterOpenChancePercent;
    ref map<string, float> BreakingDoorsOpenChancePercent;
    ref map<string, float> BreakingDoorsOpenLockPickChancePercent;
    ref map<string, float> BreakingDoorsLossInterestAfterHitChancePercent;
    ref map<string, float> BreakingDoorsLossInterestAfterHitLockPickChancePercent;
    ref map<string, float> BreakingDoorsCrawlersChanceMultiplier;

    ref map<string, bool> AttackPlayersUnconsciousHandlerIsActive;
    ref map<string, float> AttackPlayersUnconsciousHealthDamageLimit;

	ref map<string, bool> AttackAnimalsHandlerIsActive;
	//ref map<string, bool> AttackAnimalsWolfs;
	//ref map<string, bool> AttackAnimalsBears;
	ref map<string, bool> AttackAnimalsWildBoar;
	ref map<string, bool> AttackAnimalsPig;
	ref map<string, bool> AttackAnimalsSheep;
	//ref map<string, bool> AttackAnimalsChicken;
	ref map<string, bool> AttackAnimalsDeer;
	ref map<string, bool> AttackAnimalsRoeDeer;
	ref map<string, bool> AttackAnimalsGoat;
	ref map<string, bool> AttackAnimalsCow;
	ref map<string, ref map<string, bool>> AttackAnimalsCustom;

	ref map<string, ref array<string>> FriendlyNPC;

    ref map<string, bool> SearchHandlerIsActive;
    ref map<string, float> SearchSphereRadiusIncreaseEverySeconds;
    ref map<string, float> SearchSphereDistanceToRadiusMultiplier;
    ref map<string, float> SearchSphereRadiusMin;
    ref map<string, float> SearchSphereRadiusMax;
    ref map<string, float> SearchSphereRadiusBurst;

	ref map<string, bool> SpeedHandlerIsActive;
    ref map<string, float> SpeedMinimumInCalmMode;
    ref map<string, float> SpeedLimitInCalmMode;
    ref map<string, float> SpeedMultiplierInCalmMode;
    ref map<string, float> SpeedMinimumInSearchMode;
    ref map<string, float> SpeedLimitInSearchMode;
    ref map<string, float> SpeedMultiplierInSearchMode;
    ref map<string, float> SpeedMinimumInChaseMode;
    ref map<string, float> SpeedLimitInChaseMode;
    ref map<string, float> SpeedMultiplierInChaseMode;
    ref map<string, float> SpeedNoLimitsFromDistanceMeters;

    ref map<string, bool> SmellsHandlerIsActive;
    ref map<string, float> SmellsSphereRadiusInCalmMode;
    ref map<string, float> SmellsSphereRadiusInSearchMode;
    ref map<string, float> SmellsChaseMultiplier;
    ref map<string, bool> SmellsAllowStealthKills;
    ref map<string, float> SmellsLossInterestAfterReactionForSeconds;
    ref map<string, bool> SmellsDisableVanillaIrritationSound;
	
	ref map<string, bool> LongVisionHandlerIsActive;
	ref map<string, float> LongVisionMetersInSearchModeDay;
	ref map<string, float> LongVisionMetersInSearchModeNight;
	ref map<string, float> LongVisionMetersInCalmModeDay;
	ref map<string, float> LongVisionMetersInCalmModeNight;
	ref map<string, float> LongVisionVisualAcuityInSearchModeDay;
	ref map<string, float> LongVisionVisualAcuityInSearchModeNight;
	ref map<string, float> LongVisionVisualAcuityInCalmModeDay;
	ref map<string, float> LongVisionVisualAcuityInCalmModeNight;
	ref map<string, float> LongVisionPersistAfterIrritationSeconds;
	ref map<string, bool> LongVisionAllowIgnoreWeather;
	
	ref map<string, bool> ReturnToSpawnHandlerIsActive;
	ref map<string, float> ReturnToSpawnRadiusMeters;

    ref map<string, bool> UpJumpHandlerIsActive;
    ref map<string, float> UpJumpHeightMax;
    ref map<string, float> UpJumpImpulseDamageChancePercent;
    ref map<string, float> UpJumpChancePercent;
    ref map<string, float> UpJumpCooldownSeconds;

    ref map<string, bool> DownJumpHandlerIsActive;
    ref map<string, float> DownJumpHeightMax;
	
	ref map<string, bool> StuckJumpHandlerIsActive;

	ref map<string, bool> FallHandlerIsActive;
    ref map<string, float> FallHandlerLegBreakHeightMin;
    ref map<string, float> FallHandlerLegBreakHeightMax;
    ref map<string, float> FallHandlerDeathHeightMin;
    ref map<string, float> FallHandlerDeathHeightMax;

	ref map<string, bool> InjuryHandlerIsActive;
	ref map<string, float> InjuryHandlerInjuryOnExplosiveDamageChancePercent;
	ref map<string, float> InjuryHandlerDamageThresholdToInjuryArm;
	ref map<string, float> InjuryHandlerRestoreInjuredArmAfterSeconds;
	ref map<string, float> InjuryHandlerInjuredOneArmAttackMultiplier;
	ref map<string, float> InjuryHandlerInjuredBothArmsAttackMultiplier;
	ref map<string, float> InjuryHandlerInjuredOneArmBreakingDoorChanceMultiplier;
	ref map<string, float> InjuryHandlerInjuredBothArmsBreakingDoorChanceMultiplier;
	ref map<string, float> InjuryHandlerInjuredOneArmStunChanceMultiplier;
	ref map<string, float> InjuryHandlerInjuredBothArmsStunChanceMultiplier;
	ref map<string, float> InjuryHandlerInjuredOneArmPainMultiplier;
	ref map<string, float> InjuryHandlerInjuredBothArmsPainMultiplier;
	ref map<string, float> InjuryHandlerInjuredOneArmBleedingChanceMultiplier;
	ref map<string, float> InjuryHandlerInjuredBothArmsBleedingChanceMultiplier;
	ref map<string, float> InjuryHandlerSpawnWithInjuredOneArmChancePercent;
	ref map<string, float> InjuryHandlerSpawnWithInjuredBothArmsChancePercent;
	ref map<string, float> InjuryHandlerDamageThresholdToInjuryLeg;
	ref map<string, float> InjuryHandlerRestoreInjuredLegAfterSeconds;
	ref map<string, float> InjuryHandlerInjuredOneLegSpeedLimit;
	ref map<string, float> InjuryHandlerInjuredBothLegsSpeedLimit;
	ref map<string, float> InjuryHandlerInjuredOneLegJumpHeightMultiplier;
	ref map<string, float> InjuryHandlerInjuredBothLegsJumpHeightMultiplier;
	ref map<string, float> InjuryHandlerSpawnWithInjuredOneLegChancePercent;
	ref map<string, float> InjuryHandlerSpawnWithInjuredBothLegsChancePercent;

    ref map<string, bool> AttackCarHandlerIsActive;
    ref map<string, bool> AttackCarPlayersIsActive;
	ref map<string, float> AttackCarPlayersDistanceMeters;
    ref map<string, bool> AttackCarElementsIsActive;
    ref map<string, float> AttackCarElementsDistanceMeters;
    ref map<string, float> AttackCarElementsGlobalDamageMultiplier;
	ref map<string, bool> AttackCarElementsByCollisionsIsActive;
	ref map<string, float> AttackCarElementsByCollisionsGlobalDamageMultiplier;
    ref map<string, ref map<string, float>> AttackCarElementsMultiplierByCarClassId;
    ref map<string, float> AttackCarWindowDamagePercent;
    ref map<string, float> AttackCarDoorDamagePercent;
    ref map<string, float> AttackCarDoorChancePercent;
    ref map<string, float> AttackCarRadiatorDamagePercent;
    ref map<string, float> AttackCarCarBatteryDamagePercent;
    ref map<string, float> AttackCarPlugDamagePercent;
    ref map<string, float> AttackCarEngineDamagePercent;
    ref map<string, float> AttackCarLightsDamagePercent;
    ref map<string, float> AttackCarFenderDamagePercent;
    ref map<string, float> AttackCarBumperDamagePercent;
    ref map<string, float> AttackCarHoodDamagePercent;
    ref map<string, float> AttackCarFuelTankDamagePercent;

    ref map<string, bool> ReactHandlerIsActive;
    ref map<string, float> ReactBehindVisualMultiplier;
    ref map<string, float> ReactBehindNoiseMultiplier;
    ref map<string, float> ReactFireplaceVisual;
    ref map<string, float> ReactFireplaceDayVisual;
    ref map<string, float> ReactFlashlightsVisual;
    ref map<string, float> ReactHeadtorchRedVisual;
    ref map<string, float> ReactRoadflareVisual;
    ref map<string, float> ReactRoadflareNoise;
    ref map<string, float> ReactChemlightVisual;
    ref map<string, float> ReactChemlightRedVisual;
    ref map<string, float> ReactPortableGasLampVisual;
    ref map<string, float> ReactFlammablesVisual;
    ref map<string, float> ReactSpotlightVisual;
    ref map<string, float> ReactCarLightVisual;
    ref map<string, float> ReactPowerGeneratorNoise;
    ref map<string, float> ReactCarHornNoise;
    ref map<string, float> ReactAlarmClockNoise;
    ref map<string, float> ReactKitchenTimerNoise;
    ref map<string, float> ReactFuelStationNoise;
    ref map<string, float> ReactFireworksNoise;
    ref map<string, float> ReactExplosiveItemNoise;
    ref map<string, float> ReactCarEngineNoise;
    ref map<string, float> ReactSmokeGrenadeVisual;
    ref map<string, float> ReactScreamNoise;
    ref map<string, float> ReactWeaponShotNoise;
    ref map<string, float> ReactFriendlyWeaponShotNoise;
    ref map<string, float> ReactFootstepsJogDayNoise;
    ref map<string, float> ReactFootstepsJogNightNoise;
    ref map<string, float> ReactFootstepsCrouchSprintDayNoise;
    ref map<string, float> ReactFootstepsCrouchSprintNightNoise;
    ref map<string, float> ReactFootstepsSprintDayNoise;
    ref map<string, float> ReactFootstepsSprintNightNoise;
    ref map<string, float> ReactHorseHoofstepsDayNoise;
    ref map<string, float> ReactHorseHoofstepsNightNoise;
    ref map<string, float> ReactDoorsDayNoise;
    ref map<string, float> ReactDoorsNightNoise;
    ref map<string, float> ReactPlayerFallDayNoise;
    ref map<string, float> ReactPlayerFallNightNoise;
	ref map<string, float> ReactBodyfallDayNoise;
    ref map<string, float> ReactBodyfallNightNoise;
	ref map<string, float> ReactBodyfallBackstabDayNoise;
	ref map<string, float> ReactBodyfallBackstabNightNoise;
	ref map<string, float> ReactSymptomsDayNoise;
	ref map<string, float> ReactSymptomsNightNoise;
	ref map<string, float> ReactVoiceWhisperDayNoise;
	ref map<string, float> ReactVoiceWhisperNightNoise;
	ref map<string, float> ReactVoiceTalkDayNoise;
	ref map<string, float> ReactVoiceTalkNightNoise;
	ref map<string, float> ReactVoiceShoutDayNoise;
	ref map<string, float> ReactVoiceShoutNightNoise;
	ref map<string, float> ReactMiningLightDayNoise;
	ref map<string, float> ReactMiningLightNightNoise;
	ref map<string, float> ReactMiningHardDayNoise;
	ref map<string, float> ReactMiningHardNightNoise;
	ref map<string, float> ReactBuildingDayNoise;
	ref map<string, float> ReactBuildingNightNoise;
	ref map<string, float> ReactSawPlanksDayNoise;
	ref map<string, float> ReactSawPlanksNightNoise;
	ref map<string, float> ReactVanillaMindstateChange;
	ref map<string, ref map<string, ref InediaInfectedAI_ReactCustomIrritant>> ReactCustomIrritants;

	ref map<string, float> ReactAndDestroyFlashlightsVisual;
	ref map<string, float> ReactAndDestroyHeadtorchRedVisual;
	ref map<string, float> ReactAndDestroyRoadflareVisual;
	ref map<string, float> ReactAndDestroyRoadflareNoise;
	ref map<string, float> ReactAndDestroyChemlightVisual;
	ref map<string, float> ReactAndDestroyChemlightRedVisual;
	ref map<string, float> ReactAndDestroyPortableGasLampVisual;
	ref map<string, float> ReactAndDestroyFlammablesVisual;
	ref map<string, float> ReactAndDestroySpotlightVisual;
	ref map<string, float> ReactAndDestroyFireplaceVisual;
	ref map<string, float> ReactAndDestroyFireplaceDayVisual;
	ref map<string, float> ReactAndDestroyCarLightVisual;
	ref map<string, float> ReactAndDestroyFireworksNoise;
	ref map<string, float> ReactAndDestroyPowerGeneratorNoise;
	ref map<string, float> ReactAndDestroyAlarmClockNoise;
	ref map<string, float> ReactAndDestroyKitchenTimerNoise;
	ref map<string, float> ReactAndDestroySmokeGrenadeVisual;

	ref map<string, bool> DamageToPlayerHandlerIsActive;
	ref map<string, float> DamageToPlayerHealthMultiplier;
	ref map<string, float> DamageToPlayerInBlockHealthMultiplier;
	ref map<string, float> DamageToPlayerShockMultiplier;
	ref map<string, float> DamageToPlayerInBlockShockMultiplier;
	ref map<string, float> DamageToPlayerStaminaPercent;
	ref map<string, float> DamageToPlayerInBlockStaminaPercent;
	ref map<string, float> DamageToPlayerBloodPercent;
	ref map<string, float> DamageToPlayerInBlockBloodPercent;
	ref map<string, bool> DamageToPlayerBleedingHandlerIsActive;
	ref map<string, float> DamageToPlayerBleedingChancePercent;
	ref map<string, float> DamageToPlayerInBlockBleedingChancePercent;

	ref map<string, float> StunToPlayerChancePercent;
	ref map<string, float> StunToPlayerInBlockChancePercent;
	
	ref map<string, float> DisarmToPlayerChancePercent;
	ref map<string, float> DisarmToPlayerInBlockChancePercent;
	
	ref map<string, float> FastAttackToPlayerChancePercent;
	
	ref map<string, bool> DiseasesToPlayerHandlerIsActive;
	ref map<string, ref array<ref InediaInfectedAI_ConfigZombieDeseaseAgent>> DiseasesToPlayerAgents;

	ref map<string, float> CameraShakeToPlayerIntensity;

    ref map<string, bool> PainToPlayerHandlerIsActive;
    ref map<string, float> PainToPlayerHeadMultiplier;
    ref map<string, float> PainToPlayerHeadInBlockMultiplier;
    ref map<string, float> PainToPlayerArmsMultiplier;
    ref map<string, float> PainToPlayerArmsInBlockMultiplier;
	ref map<string, float> PainToPlayerArmsDisarmMultiplier;
    ref map<string, float> PainToPlayerArmsInBlockDisarmMultiplier;
    ref map<string, float> PainToPlayerLegsMultiplier;
    ref map<string, float> PainToPlayerLegsInBlockMultiplier;
    ref map<string, float> PainToPlayerTorsoMultiplier;
    ref map<string, float> PainToPlayerTorsoInBlockMultiplier;
	
    ref map<string, float> PainToPlayerLimbsBreakChancesMultiplier;
    ref map<string, float> PainToPlayerLimbsDeepWoundChancesMultiplier;
	
	ref map<string, bool> DamageToZombieHandlerIsActive;
	ref map<string, float> DamageToZombieHealthPoints;
	ref map<string, float> DamageToZombieHealthPointsLeg;
	ref map<string, float> DamageToZombieHeadMeleeMultiplier;
	ref map<string, float> DamageToZombieHeadRangeMultiplier;
	ref map<string, float> DamageToZombieBodyMeleeMultiplier;
	ref map<string, float> DamageToZombieBodyRangeMultiplier;
	ref map<string, float> DamageToZombieFromCarsMultiplier;
	ref map<string, float> DamageToZombieFromExplosionsMultiplier;
	ref map<string, float> DamageToZombieFromHotItemsHp;
	ref map<string, bool> DamageToZombieBodyPartsPiercingIsActive;
	ref map<string, bool> DamageToZombieAlwaysKillOnBackstab;
	ref map<string, bool> DamageToZombieShockToStunHandlerIsActive;
	ref map<string, float> DamageToZombieShockToStunLightHeavyAnimationThresholdMeleeHeavy;
	ref map<string, float> DamageToZombieShockToStunLightHeavyAnimationThresholdRange;
	ref map<string, float> DamageToZombieShockToStunThresholdMeleeHeavy;
	ref map<string, float> DamageToZombieShockToStunThresholdRange;
	ref map<string, float> DamageToZombieShockToStunFromButtstockHit;
	ref map<string, bool> DamageToZombieShockToStunCumulativeDamage;
	ref map<string, float> DamageToZombieShockToStunCumulativeDamageTimeFrameSeconds;
	ref map<string, float> DamageToZombieShockToStunIgnoreMeleeHitChancePercent;
	ref map<string, float> DamageToZombieShockToStunIgnoreRangedHitChancePercent;
	ref map<string, float> DamageToZombieShockToStunImmunityAfterMeleeHitSeconds;
	ref map<string, float> DamageToZombieShockToStunImmunityAfterRangedHitSeconds;
	ref map<string, ref map<string, float>> DamageToZombieWeaponsMultipliers;

	ref map<string, bool> MeleeAttacksDodgeHandlerIsActive;
	ref map<string, float> MeleeAttacksDodgeChance;
	ref map<string, float> MeleeAttacksDodgeCooldownSeconds;
	
	ref map<string, bool> JumpAttackHandlerIsActive;
	ref map<string, float> JumpAttackChance;
	ref map<string, float> JumpAttackCooldownSeconds;

	ref map<string, bool> BloodHandlerIsActive;
	ref map<string, float> BloodVolumeMl;
	ref map<string, float> BloodLossRateMinMl;
	ref map<string, float> BloodLossRateMaxMl;
	ref map<string, float> BloodLossRateRegenMl;
	ref map<string, float> BloodOnExplosiveDamageChancePercent;
	ref map<string, bool> BloodSplatParticlesIsActive;
	ref map<string, bool> BloodTrailParticlesIsActive;
	ref map<string, bool> BloodPoolParticlesIsActive;

	ref map<string, bool> ResistContaminatedEffectHandlerIsActive;
	ref map<string, bool> ResistContaminatedEffect;

	ref map<string, bool> AllowCrawling;
	ref map<string, float> RespawnInCrawlingChancePercent;

	ref map<string, bool> ScreamHandlerIsActive;
	ref map<string, float> ScreamAttractsZombiesInRadius;
	ref map<string, float> ScreamChancePercent;
	ref map<string, float> ScreamCooldownSeconds;
	ref map<string, float> ScreamNearbyInfectedSilenceSeconds;

	ref map<string, bool> SearchBodyToViewCargo;
	ref map<string, float> SearchBodyToViewCargoSeconds;
	ref map<string, float> SearchBodyExtendLootingToPlayersInRadiusMeters;
	ref map<string, bool> SearchBodyOnlyEmptyHands;
	ref map<string, float> SearchBodyWithoutGlovesBloodyHandsChancePercent;
	ref map<string, float> SearchBodyWithoutMaskVomitChancePercent;

	ref map<string, bool> CanBeButchered;
	ref map<string, float> ButcheringSeconds;
	ref map<string, float> ButcheringWithoutGlovesBloodyHandsChancePercent;
	ref map<string, float> ButcheringWithoutMaskVomitChancePercent;
	ref map<string, ref array<ref InediaInfectedAI_ConfigZombieItemAfterButchering>> ItemsAfterButchering;

	ref map<string, bool> CanBeBackstabbedHandlerIsActive;
	ref map<string, bool> CanBeBackstabbed;

    ref map<string, float> VisionRangeMultiplierDay;
    ref map<string, float> VisionRangeMultiplierNight;

	ref map<string, bool> SizeHandlerIsActive;
	ref map<string, float> SizeFromMultiplier;
	ref map<string, float> SizeToMultiplier;
	ref map<string, bool> SizeDamageScalingIsActive;
	ref map<string, bool> SizeBloodVolumeScalingIsActive;
	ref map<string, bool> SizeSpeedMultiplierScalingIsActive;

	ref map<string, bool> ThrowingProjectilesHandlerIsActive;
	ref map<string, float> ThrowingProjectilesHandlerDamageMultiplier;
	ref map<string, float> ThrowingProjectilesHandlerVehiclesDamageMultiplier;
	ref map<string, ref array<string>> ThrowingProjectilesHandlerProjectilesList;

	ref map<string, bool> RangeAttacksHandlerIsActive;
	ref map<string, float> RangeAttacksHandlerZombiePlayerDistance;
	ref map<string, float> RangeAttacksHandlerPlayerOnObstacleHeight;

	protected void InitBoolProperty(out map<string, bool> property, bool defaultValue)
	{
		if (!property) {
			property = new map<string, bool>;
		}

		if (!property.Contains("all")) {
			property["all"] = defaultValue;
		}
	}

	protected void InitFloatProperty(out map<string, float> property, float defaultValue)
	{
		if (!property) {
			property = new map<string, float>;
		}

		if (!property.Contains("all")) {
			property["all"] = defaultValue;
		}
	}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------:::::::::::::::::::::----------------------------------------------------------------
// -----------------------------------------------------------::::::::::::::::::::::::::::::::::::-------------------------------------------------------
// ----------------------------------------------------:::::::::::::::::::::::::::::::::::::::::::::::::-------------------------------------------------
// -----------------------------------------------::::::::::::::::::::::::::::::::::::::--==--:::::::::::::::--------------------------------------------
// -------------------------------------------:::::::::::::::::::::::::::::-+++++++++***+=--======++=-:::::::::::----------------------------------------
// ---------------------------------------:::::::::::::::::::::::::::::-+**+-.........-::::. ::-====-=+++++=-::::::--------------------------------------
// ------------------------------------:::::::::::::::::::::::::::::-+*+-..       -++=:...  .::.    :-=++--=+*=-:::::::----------------------------------
// ---------------------------------:::::::::::::::::::::::::::::=**+:.        :--     .:::::...  ..... .-=+=-===::::::::::------------------------------
// ------------------------------:::::::::::::::::::::::::::::-*#*+=--::...----.    .::.                    :=+=-==::::::::::----------------------------
// ----------------------------:::::::::::::::::::::::::::::-*+:-=.....::=*%*====-=%*-.  .::..              .: .-++#*=:::::::::--------------------------
// -------------------------:::::::::::::::::::::::::::::..-#.:-=.......:--:......::+*@%*.: .:::.                 :+#:-:::::::::::-----------------------
// ------------------------:::::::::::::::::::::::::.......#:-:=:.....:---......:-==:#%@@%.-:  ..:... ...   ...   -::++::::::::::::----------------------
// -----------------------:::::::::::::::::::::::.........=+--:+:::::-:-:.::::---=::+-==+%# =  :::-:..  .--.....   .-::#:::::::::::::--------------------
// ----------------------::::::::::::::::::::::..........:%:+:=:::::::-:::::-==--::=::---#@ ---:...::::::::-::.:=:    -+*::::::::::::::------------------
// ---------------------::::::::::::::::::::.............*==::+::::::---:::+*---::-::-=--%*:+.-::---....  :--..:-+=    -.#:::::::::::::::----------------
// -------------------:::::::::::::::::::::.............-%.+::=:::::==-:::*+--::--::----#=-=.:...::-=--=-:: ::--.:==: .*-*=::::::::::::::::--------------
// -----------------:::::::::::::::::::::...............%.-=:+-::::+*::-++==-::==-----=#==:-:    .   .::-::-:=+:---:.::+-#-:::::::::::::::::-------------
// ---------------:::::::::::::::::::::................+#.*--*----++-+**-==---+==+===**==. .....::-::.   :::-=-#=--==-:+*.#:::::::::::::::::::-----------
// --------------:::::::::::::::::::.................=*=+.@=-*---**==++-*=--+#++*=-+*=--:.===+#%@@@%*++=:  .-.--+==:.:-%*-%::::::::::::::::::::----------
// ------------::::::::::::::::::::..................=**--@+=%**%*==**+#+=+*+-=#=*+=-----:.=@%%@%###%@@@@#: -. -.==#===-=:+*::::::::::::::::::::---------
// -----------::::::::::::::::::::................... *@:+@#+.   :+#%#%*+=#===%#+:-:::=-:+%%#%*++*++**%@@@@=::   :-:- .=*=-**..:::::::::::::::::---------
// ----------:::::::::::::::::::...................   .%:#%:=#%#+.::-*******++=:--...:+-*=*%%*%@*:....=%@@@@*=::.== *. . .  :*-:::::::::::::::::::-------
// ---------::::::::::::::::::::.................     :#:@-+#-+%%%=.=.+ +-=--:-. .:.  :-*---#%@%+:::%-.*@@@@@@%%#+--++:.==   .-#:::::::::::::::::::------
// --------:::::::::::::::::::..................      =#-@:+ +*:*%@##-* +:  ::--::.  :-.:+**##*%*=---=*%@@@@@@@%=. ...%@@%%##:-=#::::::::::::::::::------
// -------:::::::::::::::::::.................        :%=@* :*:-+@@@%:+*=       :--=-.:::--+#@@####**#%@@@%#%#=:--- .--%@#@@%*:.#-.::::::::::::::::::----
// ------::::::::::::::::::::................          =#*#.:%:.#@@=:.%@%+:  .-           :-=+*#**###%##*+==+:--:---::-=+#*%@#-*=....::::::::::::::::----
// ------:::::::::::::::::::................           ++##: =#-+@@= .@@+@@@#-.::-##*=:-+##*=: .=+++++--:-=*+=----.=%@@@+**+%+-.......::::::::::::::::---
// ------::::::::::::::::::................            #+:%+: .*%@@@*:=@+-%%%@@#=-=--+**=:.-*@@#=::::.-*%#*+-=-+.-%@@@%@=#**-.........::::::::::::::::---
// -----::::::::::::::::::................              =#=*:+:.:=:-++=*::.==:##@@#*=------- .#%%*+::*%%+*==:=@ =@@@@%###@%+...........::::::::::::::::--
// -----::::::::::::::::::...............                -%=*.-..-:=-:-::*:- . .%@@@@%%%%+#@= .+%.+--+**+:* :+@.+@@@@#+%+*##............::::::::::::::::-
// ----::::::::::::::::::................                 =*+@%*--::..=@%@#.:.  #@@@@#%%%%:*@. +%:+-*++:.*-=..=##:-=****#=*#-...........::::::::::::::::-
// ---:::::::::::::::::::...............                   %=%*#@#++*%@@@@@%*   *@@*...=%##.%= =#**++::.=**:- :.@.-:#=+@#.: +-..........::::::::::::::::-
// ---:::::::::::::::::::...............                   =#+=:+@#+*%%%@@@@*   *@@*....#+#.#* #%#=+.: .:=+=    #  =-++%%:-+#-..........::::::::::::::::-
// ---:::::::::::::::::::...............                   .%-%:-== .+*#%@@@+   +%#@*=-+#-=.%# @*==-=%@#*@+*@*+%#+-:.:+%#%*-............::::::::::::::::-
// ---:::::::::::::::::::...............                   .%=%%-.= .-+@%@@@= . -@%*%@@#-=-=@*:*++*@#+:-@*.==.=%=#*=%=-%-................::::::::::::::::
// ----::::::::::::::::::...............                   :#=@%%.:  -*%*@@@: . :%@@*@*::: +%+:++=@@%%-*@-.+. %:.#..#+%- ................::::::::::::::::
// --::::::::::::::::::::...............                   =++%-%=:  =#+.#@%=.. .=#@@%-:.:.-%-.*.%@@@@@@@%+%+**. * :*-# ................:::::::::::::::::
// ---:::::::::::::::::::...............               .:-=%-%:-%#-  *** +@#:    .+@@*=::.:-# +--@@@@@@@@#@@@@%**#--#-*.................:::::::::::::::::
// ---:::::::::::::::::::................           :#++==#+%-.=+%:..%@@.#*   :  :-.=*##+---# *:+@+%=#%*==#--=#+@@@@%#:.................:::::::::::::::--
// ---:::::::::::::::::::................          -#..*%#%#-.-:%@.=:*%%=@. .  :-. --..-+##@#-#-=*-::..#+===#*:.*####-  ...............::::::::::::::::--
// ----:::::::::::::::::::...............         +#..-*@@#..--%%@.+:#=*+*#  -:  -=-:=-====+*%#+:**+++=*+:-+=#=+*+*:-#.................:::::::::::::::---
// -----::::::::::::::::::................      .*+  ---** :+*%@*@.+-@:=*.@#  =:   .-+. ::+#%+-=-+-##=. .*-.:*: ::*%#:.................:::::::::::::::---
// ----:::::::::::::::::::.................    :#= :-..+-*+%@#@*-@:-%@*-% *@%. =.:::.:-. :::.-+*=-+..=#:-: .*=++**=-*=................::::::::::::::::---
// -----:::::::::::::::::::.................  -#: .=.   -=+@@@==.@::***=@ .%@@+-%*+*#*+=+*+*=..--++-- -%-*.+-=+*-=**=................::::::::::::::::----
// ------:::::::::::::::::::.................+%.   +     .+=@+.-=@::++-*@. =@@@@@@@@@@@#-. ...::-. -.- =* +=:.+:. %..................::::::::::::::::----
// ------::::::::::::::::::::..............=#%.    -.      +-%=#-@.:*.:*%:..@@@@%#%@@@@@@@@%*--=+*-    :. :.  -   +=................::::::::::::::::-----
// -------::::::::::::::::::::........:=++*+@@: .. .        -=*@+# -+ .##-. @@#%*#*-#@@@@@@@@@@#*##-::      .-. .:.%...............:::::::::::::::::-----
// --------::::::::::::::::::::...:=**##=-. +@@: .:          :+=@- *=  **+: %@:=#.-#@@@%@@#+=#@@@@@@%**=:..-+#@@@=:#...............:::::::::::::::::-----
// --------::::::::::::::::::::-+**+-:  +=*+-=@@= ::           +=+-@:  +=#- ##++:#@%*+**-.-*@%#++#%+=-=++++****#*+#:.............::::::::::::::::::------
// ----------::::::::::::::=+***-:.   .=---.  =@@* .-           :=+%.  *-%-.=@::%%+:**: :##*-:-*#-        ......:+..............::::::::::::::::::-------
// -----------::::::::-=*#**--: .::::::     :--#@@#..:            ==*- *-#==.@#*#-*+.  =++-..*@##=        .....................:::::::::::::::::::-------
// ------------::-=+***+==--:-:-=--.     :--.   #@@%..-            .==*%+-#= #%==%%. :#%*:.:%@%+-*+      ....................::::::::::::::::::::--------
// ------------=**+==**=-:::+--:      :--:=:    -@@@%:.-             -=+%:#=--%#@%-.*@@%- =@@*. * +#.   ....................::::::::::::::::::::---------
// -------------::::......         .. :+...--   .#:%@@-.-              ==*%*+ %%- :%@@+ .#@@=   =.--#:....................::::::::::::::::::::::---------
// ---------------:::::......   .::::.  +-. :=. :* .%@@.=               .=+## =%=#%@%: -@@@#    :- =:%*-................:::::::::::::::::::::::----------
// ----------------::::=--:::::::::::::. -+.  :-:=: *@@=-.                :=*+:@@%#*.--@@@@@    .=  -:#+++=............::::::::::::::::::::::------------
// ------------------::::---::::::::::::. .+::. =-* :@@#.=                  :=*@@*+-+:@@@@@@+    =   -.#*==+*+:......:::::::::::::::::::::::-------------
// --------------------:::::::::::::::::::..+-.  .++ %@@ +                   :-+%#+*-%@@*@@@@-   +   .: **==:=##+-.:::::::::::::::::::::::---------------
// ---------------------::::::::::::::::::::.-=:.  *=+@@:=.               :-::=+=+%+#@@#=@=%@@:  %-   .  +=.-: .:=++==:::::::::::::::::::----------------
// -----------------------:::::::::::::::::::::=:::.+*@@- =           .--::+%@%*@%.-#@@--%+*@@%  %+    :-:%:..:    ..-#::::::::::::::::------------------
// -------------------------::::::::::::::::::::==:::#@@- +        .--::+%@@*-  *@..:.-**##=@@@: #+ -#@@@*#%-+=--.. .:=%=---::::::::::-------------------
// ---------------------------::::::::::::::::::::=:=@@@= =     .--:-+%@@%+.    =@   .: =@**%@@* #+-@@@@@+#=#=.  ::::::::::---:::::----------------------
// -----------------------------:::::::::::::::::. ===@@+ :-.-+*-=*@@%%@@=. ...-*#::    *#=@@@@+ **-@@@@@-#--+. :::::::::::::::::------------------------
// -------------------------------:::::::::::::::::.:==@@##@@@@@@@@@%-.+#@%+. --+#-.  .:%+%@*=.:--=-@@@@@#@=--.:::::::::::::::::-------------------------
// ----------------------------------:::::::::::::::::+=#@@@@@@@#==*@@#=.:+%@*=**+.*=  *%+-  ::  -+@@@@@++@:=:::::::::::::::-----------------------------
// ------------------------------------::::::::::::::::-:=%@@**=.-:..@%@%+.-#@@@@*. .: %==-.  :+%@@@@@#-::#.:::::::::::::--------------------------------
// ---------------------------------------:::::::::::::::::=-::-+:  :#=@@@*-=-+#%* ..  @  .-+%@@%%@@%=::::-::::::::::------------------------------------
// --------------------------------------------::::::::::::::::::-::.-:*@%@@#=::-# ::..% .+%%+-##@@*-*:::::::::::::--------------------------------------
// ----------------------------------------------::::::::::::::::::::::::::=#@%+:*:::::+=#*-:-:--:-:::::::::::-------------------------------------------
// --------------------------------------------------::::::::::::::::::::::::::::-::::::::::::::::::::::-------------------------------------------------
// --------------------------------------------------------::::::::::::::::::::::::::::::::::::::::------------------------------------------------------
// ----------------------------------------------------------------::::::::::::::::::::::::--------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------

	protected void InitMapFloatProperty(out map<string, ref map<string, float>> property, map<string, float> defaultValue)
	{
		if (!property) {
			property = new map<string, ref map<string, float>>;
		}

		if (!property.Contains("all")) {
			property["all"] = defaultValue;
		}
	}

	protected void InitMapBoolProperty(out map<string, ref map<string, bool>> property, map<string, bool> defaultValue)
	{
		if (!property) {
			property = new map<string, ref map<string, bool>>;
		}

		if (!property.Contains("all")) {
			property["all"] = defaultValue;
		}
	}

	protected void InitArrayStringProperty(out map<string, ref array<string>> property, array<string> defaultValue)
	{
		if (!property) {
			property = new map<string, ref array<string>>;
		}

		if (!property.Contains("all")) {
			property["all"] = defaultValue;
		}
	}

	bool GetBoolPropertyForCreature(map<string, bool> property, EntityAI creature)
	{
		bool value;
		if (property.Find(creature.GetType(), value)) {
			return value;
		}

		foreach (string baseClass, bool baseClassValue: property) {
			if (baseClass != "all" && InediaInfectedAI_Functions.IsKindOfCached(baseClass, creature)) {
				return baseClassValue;
			}
		}

		foreach (string group: m_LexSortedGroupNames) {
			if (group != "all" && property.Contains(group) && CreatureIsInGroup(creature, group)) {
				return property[group];
			}
		}

		property.Find("all", value);
		return value;
	}

	float GetFloatPropertyForCreature(map<string, float> property, EntityAI creature)
	{
		float value;
		if (property.Find(creature.GetType(), value)) {
			return value;
		}

		foreach (string baseClass, float baseClassValue: property) {
			if (baseClass != "all" && InediaInfectedAI_Functions.IsKindOfCached(baseClass, creature)) {
				return baseClassValue;
			}
		}

		foreach (string group: m_LexSortedGroupNames) {
			if (group != "all" && property.Contains(group) && CreatureIsInGroup(creature, group)) {
				return property[group];
			}
		}

		property.Find("all", value);
		return value;
	}

	map<string, float> GetMapFloatPropertyForCreature(map<string, ref map<string, float>> property, EntityAI creature)
	{
		map<string, float> value = new map<string, float>;
		if (property.Find(creature.GetType(), value)) {
			return value;
		}

		foreach (string baseClass, map<string, float> baseClassValue: property) {
			if (baseClass != "all" && InediaInfectedAI_Functions.IsKindOfCached(baseClass, creature)) {
				return baseClassValue;
			}
		}

		foreach (string group: m_LexSortedGroupNames) {
			if (group != "all" && property.Contains(group) && CreatureIsInGroup(creature, group)) {
				return property[group];
			}
		}

		property.Find("all", value);
		return value;
	}

	map<string, bool> GetMapBoolPropertyForCreature(map<string, ref map<string, bool>> property, EntityAI creature)
	{
		map<string, bool> value = new map<string, bool>;
		if (property.Find(creature.GetType(), value)) {
			return value;
		}

		foreach (string baseClass, map<string, bool> baseClassValue: property) {
			if (baseClass != "all" && InediaInfectedAI_Functions.IsKindOfCached(baseClass, creature)) {
				return baseClassValue;
			}
		}

		foreach (string group: m_LexSortedGroupNames) {
			if (group != "all" && property.Contains(group) && CreatureIsInGroup(creature, group)) {
				return property[group];
			}
		}

		property.Find("all", value);
		return value;
	}

	array<string> GetArrayStringPropertyForCreature(map<string, ref array<string>> property, EntityAI creature)
	{
		array<string> value = new array<string>;
		if (property.Find(creature.GetType(), value)) {
			return value;
		}

		foreach (string baseClass, array<string> baseClassValue: property) {
			if (baseClass != "all" && InediaInfectedAI_Functions.IsKindOfCached(baseClass, creature)) {
				return baseClassValue;
			}
		}

		foreach (string group: m_LexSortedGroupNames) {
			if (group != "all" && property.Contains(group) && CreatureIsInGroup(creature, group)) {
				return property[group];
			}
		}

		property.Find("all", value);
		return value;
	}
	
	array<ref InediaInfectedAI_ConfigZombieItemAfterButchering> GetItemsAfterButcheringPropertyForCreature(EntityAI creature)
	{
		array<ref InediaInfectedAI_ConfigZombieItemAfterButchering> value = new array<ref InediaInfectedAI_ConfigZombieItemAfterButchering>;
		if (ItemsAfterButchering.Find(creature.GetType(), value)) {
			return value;
		}

		foreach (string baseClass, array<ref InediaInfectedAI_ConfigZombieItemAfterButchering> baseClassValue: ItemsAfterButchering) {
			if (baseClass != "all" && InediaInfectedAI_Functions.IsKindOfCached(baseClass, creature)) {
				return baseClassValue;
			}
		}
		
		foreach (string group: m_LexSortedGroupNames) {
			if (group != "all" && ItemsAfterButchering.Contains(group) && CreatureIsInGroup(creature, group)) {
				return ItemsAfterButchering[group];
			}
		}

		ItemsAfterButchering.Find("all", value);
		return value;
	}
	
	map<string, ref InediaInfectedAI_ReactCustomIrritant> GetReactCustomIrritantsPropertyForCreature(EntityAI creature)
	{
		map<string, ref InediaInfectedAI_ReactCustomIrritant> value = new map<string, ref InediaInfectedAI_ReactCustomIrritant>;
		if (ReactCustomIrritants.Find(creature.GetType(), value)) {
			return value;
		}

		foreach (string baseClass, map<string, ref InediaInfectedAI_ReactCustomIrritant> baseClassValue: ReactCustomIrritants) {
			if (baseClass != "all" && InediaInfectedAI_Functions.IsKindOfCached(baseClass, creature)) {
				return baseClassValue;
			}
		}
		
		foreach (string group: m_LexSortedGroupNames) {
			if (group != "all" && ReactCustomIrritants.Contains(group) && CreatureIsInGroup(creature, group)) {
				return ReactCustomIrritants[group];
			}
		}

		ReactCustomIrritants.Find("all", value);
		return value;
	}
	
	map<string, ref array<int>> GetBreakingDoorsRestrictedDoorsPropertyForCreature(EntityAI creature)
	{
		map<string, ref array<int>> value = new map<string, ref array<int>>;
		if (BreakingDoorsRestrictedDoors.Find(creature.GetType(), value)) {
			return value;
		}

		foreach (string baseClass, map<string, ref array<int>> baseClassValue: BreakingDoorsRestrictedDoors) {
			if (baseClass != "all" && InediaInfectedAI_Functions.IsKindOfCached(baseClass, creature)) {
				return baseClassValue;
			}
		}
		
		foreach (string group: m_LexSortedGroupNames) {
			if (group != "all" && BreakingDoorsRestrictedDoors.Contains(group) && CreatureIsInGroup(creature, group)) {
				return BreakingDoorsRestrictedDoors[group];
			}
		}

		BreakingDoorsRestrictedDoors.Find("all", value);
		return value;
	}
	
	array<ref InediaInfectedAI_ConfigZombieDeseaseAgent> GetDiseasesToPlayerAgentsPropertyForCreature(EntityAI creature)
	{
		array<ref InediaInfectedAI_ConfigZombieDeseaseAgent> value = new array<ref InediaInfectedAI_ConfigZombieDeseaseAgent>;
		if (DiseasesToPlayerAgents.Find(creature.GetType(), value)) {
			return value;
		}

		foreach (string baseClass, array<ref InediaInfectedAI_ConfigZombieDeseaseAgent> baseClassValue: DiseasesToPlayerAgents) {
			if (baseClass != "all" && InediaInfectedAI_Functions.IsKindOfCached(baseClass, creature)) {
				return baseClassValue;
			}
		}

		foreach (string group: m_LexSortedGroupNames) {
			if (group != "all" && DiseasesToPlayerAgents.Contains(group) && CreatureIsInGroup(creature, group)) {
				return DiseasesToPlayerAgents[group];
			}
		}

		DiseasesToPlayerAgents.Find("all", value);
		return value;
	}

	[NonSerialized()]
	ref map<string, ref map<string, bool>> m_CreatureGroupCache = new map<string, ref map<string, bool>>;
	[NonSerialized()]
	int m_InediaInfectedAI_CreatureIsInGroupRecursionDepth = 0;
	bool CreatureIsInGroup(EntityAI creature, string group) {
		if (!creature) {
			return false;
		}
		
		array<string> groupValue = new array<string>;
		if (!Groups.Find(group, groupValue)) {
			// group is not a group, it is probably a class or a base class
			return false;
		}
		
		// Recursion depth too high — aborting membership search
		if (m_InediaInfectedAI_CreatureIsInGroupRecursionDepth > 10) {
			Error("Error in configuration file.\nStack overflow, recursion depth more than 10.\nProbably some of your infected groups refer to themselves.\nThe name of the group from the problem chain where the problem occurred: \"" + group + "\".");
			return false;
		}
		
		string creatureClass = creature.GetType();
		
		// If the creature's membership in the group is cached, return it.
		if (m_CreatureGroupCache.Contains(creatureClass) && m_CreatureGroupCache[creatureClass].Contains(group)) {
#ifdef DIAG_DEVELOPER
			if (InediaInfectedAI_Debug.IsDebugRuleEnabled("groups")) {
				InediaInfectedAI_Debug.Log("Found in cache: " + creatureClass + ": " + group + " = " + m_CreatureGroupCache[creatureClass][group], "groups");
			}
#endif
			
			return m_CreatureGroupCache[creatureClass][group];
		}

		// Creature's membership in the group is not cached - determining membership

		// If the creature exactly matches one of the Classes within the group - add group to cache and return the membership
		if (groupValue.Find(creatureClass) > -1) {
			AddCreatureGroupCacheValue(creature, group, true);
			return true;
		}

		// If the creature exactly matches one of the Base Classes within the group - add group to cache and return the membership
		foreach (string baseClass: groupValue) {
			if (InediaInfectedAI_Functions.IsKindOfCached(baseClass, creature)) {
				AddCreatureGroupCacheValue(creature, group, true);
				return true;
			}
		}

		// If the creature exactly matches one of the Sub groups inside the group - add group to cache and return the membership
		foreach (string subGroup: groupValue) {
			m_InediaInfectedAI_CreatureIsInGroupRecursionDepth++;
			if (CreatureIsInGroup(creature, subGroup)) {
				m_InediaInfectedAI_CreatureIsInGroupRecursionDepth--;
				AddCreatureGroupCacheValue(creature, group, true);
				return true;
			}
			m_InediaInfectedAI_CreatureIsInGroupRecursionDepth--;
		}

		AddCreatureGroupCacheValue(creature, group, false);
		return false;
	}
	
	void AddCreatureGroupCacheValue(EntityAI creature, string group, bool value)
	{
		if (!creature) {
			return;
		}
		string creatureClass = creature.GetType();
		
		if (!m_CreatureGroupCache.Contains(creatureClass)) {
			m_CreatureGroupCache[creatureClass] = new map<string, bool>;
		}
		
		m_CreatureGroupCache[creatureClass][group] = value;
	}
	
	[NonSerialized()]
	ref array<string> m_LexSortedGroupNames = new array<string>;
	void UpdateSortedGroupNames()
	{
		if (!Groups) {
			return;
		}
		
		m_LexSortedGroupNames = Groups.GetKeyArray();
		m_LexSortedGroupNames.Sort();
		foreach (int index, string groupName: m_LexSortedGroupNames) {
			string groupNameWithoutSortingSymbols = groupName;
			groupNameWithoutSortingSymbols.Replace("!", "");
			groupNameWithoutSortingSymbols.Replace("~", "");
			
			if (groupNameWithoutSortingSymbols != groupName) {
				m_LexSortedGroupNames[index] = groupNameWithoutSortingSymbols;
				Groups.ReplaceKey(groupName, groupNameWithoutSortingSymbols);
			}
		}
	}

	void Initialize()
	{		
		if (!Groups) {
			Groups = new map<string, ref array<string>>;
		}
		
		UpdateSortedGroupNames();

		if (!CustomIrritants) {
			CustomIrritants = new map<string, ref InediaInfectedAI_CustomIrritant>;
		}

		if (!ThrowingProjectiles) {
			ThrowingProjectiles = new map<string, ref InediaInfectedAI_ConfigThrowingProjectile>;
		}

        InitBoolProperty(BreakingDoorsHandlerIsActive, true);

		if (!BreakingDoorsRestrictedDoors) {
			BreakingDoorsRestrictedDoors = new map<string, ref map<string, ref array<int>>>;
		}
		if (!BreakingDoorsRestrictedDoors.Contains("all")) {
			map<string, ref array<int>> BreakingDoorsRestrictedDoorsValue = new map<string, ref array<int>>;
			BreakingDoorsRestrictedDoorsValue["DNA_Strongroom_ColourBase"] = {};
			BreakingDoorsRestrictedDoorsValue["DNA_Strongroom_Open_Base"] = {};
			BreakingDoorsRestrictedDoorsValue["DNA_Lockout_ColourBase"] = {};
			BreakingDoorsRestrictedDoorsValue["DNA_Lockout_OneWayDoor"] = {};
			BreakingDoorsRestrictedDoorsValue["DNA_Door_NoLock_ColourBase"] = {};
			BreakingDoorsRestrictedDoorsValue["DNA_WarpDoor_NoLock"] = {};
			BreakingDoorsRestrictedDoorsValue["DNA_Lockout_Dbl_ColourBase"] = {};
			BreakingDoorsRestrictedDoorsValue["DNA_Door_NoLock_Dbl_ColourBase"] = {};
			BreakingDoorsRestrictedDoorsValue["DNA_LockoutSteel_ColourBase"] = {};
			BreakingDoorsRestrictedDoorsValue["Land_KlimaX_T1Door"] = {};
			BreakingDoorsRestrictedDoorsValue["Land_KlimaX_T2Door"] = {};
			BreakingDoorsRestrictedDoorsValue["Land_KlimaX_T3Door"] = {};
			BreakingDoorsRestrictedDoorsValue["EVG_KeycardDoor"] = {};
			BreakingDoorsRestrictedDoorsValue["land_bunker_door_base"] = {};
			BreakingDoorsRestrictedDoorsValue["land_bunker_entrance"] = {};
			BreakingDoorsRestrictedDoorsValue["land_bunker_entrance1"] = {};
			BreakingDoorsRestrictedDoorsValue["land_bunker_entrance2"] = {};
			BreakingDoorsRestrictedDoorsValue["land_bunker_door_area04"] = {};
			BreakingDoorsRestrictedDoorsValue["Land_WarheadStorage_Main"] = {};
			BreakingDoorsRestrictedDoorsValue["Jmc_Security_Door"] = {};
			BreakingDoorsRestrictedDoorsValue["Jmc_Security_Door_Carrier"] = {};
			BreakingDoorsRestrictedDoorsValue["jmc_dungeon_Door06_Double"] = {};
			BreakingDoorsRestrictedDoorsValue["ContainerLockedBase"] = {};

			BreakingDoorsRestrictedDoors["all"] = BreakingDoorsRestrictedDoorsValue;
		}

        InitFloatProperty(BreakingDoorsDestroyAfterOpenChancePercent, 30);
        InitFloatProperty(BreakingDoorsOpenChancePercent, 30);
        InitFloatProperty(BreakingDoorsOpenLockPickChancePercent, 5);
		InitFloatProperty(BreakingDoorsLossInterestAfterHitChancePercent, 5);
		InitFloatProperty(BreakingDoorsLossInterestAfterHitLockPickChancePercent, 25);
		InitFloatProperty(BreakingDoorsCrawlersChanceMultiplier, 0);

        InitBoolProperty(AttackPlayersUnconsciousHandlerIsActive, true);
        InitFloatProperty(AttackPlayersUnconsciousHealthDamageLimit, 20);

        InitBoolProperty(AttackAnimalsHandlerIsActive, true);
        //InitBoolProperty(AttackAnimalsWolfs, false);
        //InitBoolProperty(AttackAnimalsBears, false);
		InitBoolProperty(AttackAnimalsWildBoar, true);
		InitBoolProperty(AttackAnimalsPig, true);
		InitBoolProperty(AttackAnimalsSheep, true);
		//InitBoolProperty(AttackAnimalsChicken, false);
		InitBoolProperty(AttackAnimalsDeer, true);
		InitBoolProperty(AttackAnimalsRoeDeer, true);
		InitBoolProperty(AttackAnimalsGoat, true);
		InitBoolProperty(AttackAnimalsCow, true);

        map<string, bool> AttackAnimalsCustomDefaultValue = new map<string, bool>;
		AttackAnimalsCustomDefaultValue["all"] = false;
        InitMapBoolProperty(AttackAnimalsCustom, AttackAnimalsCustomDefaultValue);
		if (AttackAnimalsCustom) {
			foreach (string AttackAnimalsCustomKey, map<string, bool> AttackAnimalsCustomValue: AttackAnimalsCustom) {
				if (AttackAnimalsCustomValue && !AttackAnimalsCustomValue.Contains("all")) {
					AttackAnimalsCustomValue["all"] = false;
				}
			}
		}

		InitArrayStringProperty(FriendlyNPC, {});

        InitBoolProperty(SearchHandlerIsActive, true);
        InitFloatProperty(SearchSphereRadiusIncreaseEverySeconds, 0.2);
        InitFloatProperty(SearchSphereDistanceToRadiusMultiplier, 0.3);
        InitFloatProperty(SearchSphereRadiusMin, 0);
        InitFloatProperty(SearchSphereRadiusMax, 40);
        InitFloatProperty(SearchSphereRadiusBurst, 60);

		InitBoolProperty(SpeedHandlerIsActive, true);
		InitFloatProperty(SpeedMinimumInCalmMode, 0);
		InitFloatProperty(SpeedLimitInCalmMode, 3);
		InitFloatProperty(SpeedMultiplierInCalmMode, 1);
		InitFloatProperty(SpeedMinimumInSearchMode, 0);
		InitFloatProperty(SpeedLimitInSearchMode, 3);
		InitFloatProperty(SpeedMultiplierInSearchMode, 1);
		InitFloatProperty(SpeedMinimumInChaseMode, 0);
		InitFloatProperty(SpeedLimitInChaseMode, 3);
		InitFloatProperty(SpeedMultiplierInChaseMode, 1);
		InitFloatProperty(SpeedNoLimitsFromDistanceMeters, 0);

        InitBoolProperty(SmellsHandlerIsActive, true);
        InitFloatProperty(SmellsSphereRadiusInCalmMode, 1);
        InitFloatProperty(SmellsSphereRadiusInSearchMode, 3);
        InitFloatProperty(SmellsChaseMultiplier, 0.6);
        InitBoolProperty(SmellsAllowStealthKills, true);
        InitFloatProperty(SmellsLossInterestAfterReactionForSeconds, 0);
        InitBoolProperty(SmellsDisableVanillaIrritationSound, false);
		
		InitBoolProperty(LongVisionHandlerIsActive, true);
		InitFloatProperty(LongVisionMetersInSearchModeDay, 120);
		InitFloatProperty(LongVisionMetersInSearchModeNight, 0);
		InitFloatProperty(LongVisionMetersInCalmModeDay, 0);
		InitFloatProperty(LongVisionMetersInCalmModeNight, 0);
		InitFloatProperty(LongVisionVisualAcuityInSearchModeDay, 1);
		InitFloatProperty(LongVisionVisualAcuityInSearchModeNight, 1);
		InitFloatProperty(LongVisionVisualAcuityInCalmModeDay, 0.1);
		InitFloatProperty(LongVisionVisualAcuityInCalmModeNight, 0.1);
		InitFloatProperty(LongVisionPersistAfterIrritationSeconds, 300);
		InitBoolProperty(LongVisionAllowIgnoreWeather, false);
		
		InitBoolProperty(ReturnToSpawnHandlerIsActive, true);
		InitFloatProperty(ReturnToSpawnRadiusMeters, 80);

        InitBoolProperty(UpJumpHandlerIsActive, true);
        InitFloatProperty(UpJumpHeightMax, 1.8);
        InitFloatProperty(UpJumpImpulseDamageChancePercent, 50);
        InitFloatProperty(UpJumpChancePercent, 100);
        InitFloatProperty(UpJumpCooldownSeconds, 0);

        InitBoolProperty(DownJumpHandlerIsActive, true);
        InitFloatProperty(DownJumpHeightMax, 30);
		
		InitBoolProperty(StuckJumpHandlerIsActive, true);

		InitBoolProperty(FallHandlerIsActive, true);
        InitFloatProperty(FallHandlerLegBreakHeightMin, 3);
        InitFloatProperty(FallHandlerLegBreakHeightMax, 15);
        InitFloatProperty(FallHandlerDeathHeightMin, 10);
        InitFloatProperty(FallHandlerDeathHeightMax, 20);

		InitBoolProperty(InjuryHandlerIsActive, true);
		InitFloatProperty(InjuryHandlerInjuryOnExplosiveDamageChancePercent, 100);
		InitFloatProperty(InjuryHandlerDamageThresholdToInjuryArm, 30);
		InitFloatProperty(InjuryHandlerRestoreInjuredArmAfterSeconds, 0);
		InitFloatProperty(InjuryHandlerInjuredOneArmAttackMultiplier, 0.5);
		InitFloatProperty(InjuryHandlerInjuredBothArmsAttackMultiplier, 0.2);
		InitFloatProperty(InjuryHandlerInjuredOneArmBreakingDoorChanceMultiplier, 0.5);
		InitFloatProperty(InjuryHandlerInjuredBothArmsBreakingDoorChanceMultiplier, 0);
		InitFloatProperty(InjuryHandlerInjuredOneArmStunChanceMultiplier, 0.5);
		InitFloatProperty(InjuryHandlerInjuredBothArmsStunChanceMultiplier, 0);
		InitFloatProperty(InjuryHandlerInjuredOneArmPainMultiplier, 0.5);
		InitFloatProperty(InjuryHandlerInjuredBothArmsPainMultiplier, 0.2);
		InitFloatProperty(InjuryHandlerInjuredOneArmBleedingChanceMultiplier, 0.5);
		InitFloatProperty(InjuryHandlerInjuredBothArmsBleedingChanceMultiplier, 0);
		InitFloatProperty(InjuryHandlerSpawnWithInjuredOneArmChancePercent, 0);
		InitFloatProperty(InjuryHandlerSpawnWithInjuredBothArmsChancePercent, 0);
		InitFloatProperty(InjuryHandlerDamageThresholdToInjuryLeg, 30);
		InitFloatProperty(InjuryHandlerRestoreInjuredLegAfterSeconds, 0);
		InitFloatProperty(InjuryHandlerInjuredOneLegSpeedLimit, 2.1);
		InitFloatProperty(InjuryHandlerInjuredBothLegsSpeedLimit, 1);
		InitFloatProperty(InjuryHandlerInjuredOneLegJumpHeightMultiplier, 0.5);
		InitFloatProperty(InjuryHandlerInjuredBothLegsJumpHeightMultiplier, 0);
		InitFloatProperty(InjuryHandlerSpawnWithInjuredOneLegChancePercent, 0);
		InitFloatProperty(InjuryHandlerSpawnWithInjuredBothLegsChancePercent, 0);

        InitBoolProperty(AttackCarHandlerIsActive, true);
        InitBoolProperty(AttackCarPlayersIsActive, true);
		InitFloatProperty(AttackCarPlayersDistanceMeters, 2);
        InitBoolProperty(AttackCarElementsIsActive, true);
        InitFloatProperty(AttackCarElementsDistanceMeters, 2);
        InitFloatProperty(AttackCarElementsGlobalDamageMultiplier, 1);
		InitBoolProperty(AttackCarElementsByCollisionsIsActive, true);
		InitFloatProperty(AttackCarElementsByCollisionsGlobalDamageMultiplier, 1);

        map<string, float> AttackCarElementsMultiplierByCarClassIdDefaultValue = new map<string, float>;
        AttackCarElementsMultiplierByCarClassIdDefaultValue["all"] = 1;
        AttackCarElementsMultiplierByCarClassIdDefaultValue["Offroad_02"] = 0.2;
        AttackCarElementsMultiplierByCarClassIdDefaultValue["Truck_01_Covered"] = 0.35;
        AttackCarElementsMultiplierByCarClassIdDefaultValue["OffroadHatchback"] = 0.8;
        AttackCarElementsMultiplierByCarClassIdDefaultValue["ExpansionMh6"] = 0.4;
        AttackCarElementsMultiplierByCarClassIdDefaultValue["ExpansionUh1h"] = 0.3;
        AttackCarElementsMultiplierByCarClassIdDefaultValue["ExpansionMerlin"] = 0.2;
        AttackCarElementsMultiplierByCarClassIdDefaultValue["ExpansionUAZ"] = 0.6;
        AttackCarElementsMultiplierByCarClassIdDefaultValue["ExpansionVodnik"] = 0.2;
        AttackCarElementsMultiplierByCarClassIdDefaultValue["ExpansionBus"] = 0.35;
        AttackCarElementsMultiplierByCarClassIdDefaultValue["ExpansionTractor"] = 0.35;
        AttackCarElementsMultiplierByCarClassIdDefaultValue["Expansion_Landrover"] = 0.6;
        InitMapFloatProperty(AttackCarElementsMultiplierByCarClassId, AttackCarElementsMultiplierByCarClassIdDefaultValue);
		if (AttackCarElementsMultiplierByCarClassId) {
			foreach (string AttackCarElementsMultiplierByCarClassIdKey, map<string, float> AttackCarElementsMultiplierByCarClassIdValue: AttackCarElementsMultiplierByCarClassId) {
				if (AttackCarElementsMultiplierByCarClassIdValue && !AttackCarElementsMultiplierByCarClassIdValue.Contains("all")) {
					AttackCarElementsMultiplierByCarClassIdValue["all"] = 1;
				}
			}
		}

        InitFloatProperty(AttackCarWindowDamagePercent, 25);
        InitFloatProperty(AttackCarDoorDamagePercent, 20);
        InitFloatProperty(AttackCarDoorChancePercent, 60);
        InitFloatProperty(AttackCarRadiatorDamagePercent, 10);
        InitFloatProperty(AttackCarCarBatteryDamagePercent, 10);
        InitFloatProperty(AttackCarPlugDamagePercent, 25);
        InitFloatProperty(AttackCarEngineDamagePercent, 10);
        InitFloatProperty(AttackCarLightsDamagePercent, 50);
        InitFloatProperty(AttackCarFenderDamagePercent, 10);
        InitFloatProperty(AttackCarBumperDamagePercent, 10);
        InitFloatProperty(AttackCarHoodDamagePercent, 10);
        InitFloatProperty(AttackCarFuelTankDamagePercent, 5);

        InitBoolProperty(ReactHandlerIsActive, true);
        InitFloatProperty(ReactBehindVisualMultiplier, 0.2);
        InitFloatProperty(ReactBehindNoiseMultiplier, 0.6);
        InitFloatProperty(ReactFireplaceVisual, 1);
        InitFloatProperty(ReactFireplaceDayVisual, 1);
        InitFloatProperty(ReactFlashlightsVisual, 1);
        InitFloatProperty(ReactHeadtorchRedVisual, 1);
        InitFloatProperty(ReactRoadflareVisual, 1);
        InitFloatProperty(ReactRoadflareNoise, 1);
        InitFloatProperty(ReactChemlightVisual, 1);
        InitFloatProperty(ReactChemlightRedVisual, 1);
        InitFloatProperty(ReactPortableGasLampVisual, 1);
        InitFloatProperty(ReactFlammablesVisual, 1);
        InitFloatProperty(ReactSpotlightVisual, 1);
        InitFloatProperty(ReactCarLightVisual, 1);
        InitFloatProperty(ReactPowerGeneratorNoise, 1);
        InitFloatProperty(ReactCarHornNoise, 1);
        InitFloatProperty(ReactAlarmClockNoise, 1);
        InitFloatProperty(ReactKitchenTimerNoise, 1);
        InitFloatProperty(ReactFuelStationNoise, 1);
        InitFloatProperty(ReactFireworksNoise, 1);
        InitFloatProperty(ReactExplosiveItemNoise, 1);
        InitFloatProperty(ReactCarEngineNoise, 1);
		InitFloatProperty(ReactSmokeGrenadeVisual, 1);
		InitFloatProperty(ReactScreamNoise, 1);
        InitFloatProperty(ReactWeaponShotNoise, 1);
        InitFloatProperty(ReactFriendlyWeaponShotNoise, 0);
		InitFloatProperty(ReactFootstepsJogDayNoise, 1);
		InitFloatProperty(ReactFootstepsJogNightNoise, 1);
		InitFloatProperty(ReactFootstepsCrouchSprintDayNoise, 0.7);
		InitFloatProperty(ReactFootstepsCrouchSprintNightNoise, 0.7);
		InitFloatProperty(ReactFootstepsSprintDayNoise, 1);
		InitFloatProperty(ReactFootstepsSprintNightNoise, 1);
		InitFloatProperty(ReactHorseHoofstepsDayNoise, 1);
		InitFloatProperty(ReactHorseHoofstepsNightNoise, 1);
		InitFloatProperty(ReactDoorsDayNoise, 1);
		InitFloatProperty(ReactDoorsNightNoise, 1);
		InitFloatProperty(ReactPlayerFallDayNoise, 1);
		InitFloatProperty(ReactPlayerFallNightNoise, 1);
		InitFloatProperty(ReactBodyfallDayNoise, 0.5);
		InitFloatProperty(ReactBodyfallNightNoise, 0.5);
		InitFloatProperty(ReactBodyfallBackstabDayNoise, 0);
		InitFloatProperty(ReactBodyfallBackstabNightNoise, 0);
		InitFloatProperty(ReactSymptomsDayNoise, 1);
		InitFloatProperty(ReactSymptomsNightNoise, 1);
		InitFloatProperty(ReactVoiceWhisperDayNoise, 1);
		InitFloatProperty(ReactVoiceWhisperNightNoise, 1);
		InitFloatProperty(ReactVoiceTalkDayNoise, 1);
		InitFloatProperty(ReactVoiceTalkNightNoise, 1);
		InitFloatProperty(ReactVoiceShoutDayNoise, 1);
		InitFloatProperty(ReactVoiceShoutNightNoise, 1);
		InitFloatProperty(ReactMiningLightDayNoise, 1);
		InitFloatProperty(ReactMiningLightNightNoise, 1);
		InitFloatProperty(ReactMiningHardDayNoise, 1);
		InitFloatProperty(ReactMiningHardNightNoise, 1);
		InitFloatProperty(ReactBuildingDayNoise, 1);
		InitFloatProperty(ReactBuildingNightNoise, 1);
		InitFloatProperty(ReactSawPlanksDayNoise, 1);
		InitFloatProperty(ReactSawPlanksNightNoise, 1);
		InitFloatProperty(ReactVanillaMindstateChange, 1);

		if (!ReactCustomIrritants) {
			ReactCustomIrritants = new map<string, ref map<string, ref InediaInfectedAI_ReactCustomIrritant>>;
		}
		if (!ReactCustomIrritants.Contains("all")) {
			map<string, ref InediaInfectedAI_ReactCustomIrritant> ReactCustomIrritantsDefaultValue = new map<string, ref InediaInfectedAI_ReactCustomIrritant>;
			ReactCustomIrritantsDefaultValue["matches"] = new InediaInfectedAI_ReactCustomIrritant(1, 1);
			ReactCustomIrritants["all"] = ReactCustomIrritantsDefaultValue;
		}

		InitFloatProperty(ReactAndDestroyFlashlightsVisual, 1);
		InitFloatProperty(ReactAndDestroyHeadtorchRedVisual, 1);
		InitFloatProperty(ReactAndDestroyRoadflareVisual, 1);
		InitFloatProperty(ReactAndDestroyRoadflareNoise, 1);
		InitFloatProperty(ReactAndDestroyChemlightVisual, 1);
		InitFloatProperty(ReactAndDestroyChemlightRedVisual, 1);
		InitFloatProperty(ReactAndDestroyPortableGasLampVisual, 1);
		InitFloatProperty(ReactAndDestroyFlammablesVisual, 0.5);
		InitFloatProperty(ReactAndDestroySpotlightVisual, 0.25);
		InitFloatProperty(ReactAndDestroyFireplaceVisual, 0.25);
		InitFloatProperty(ReactAndDestroyFireplaceDayVisual, 0.25);
		InitFloatProperty(ReactAndDestroyCarLightVisual, 1);
		InitFloatProperty(ReactAndDestroyFireworksNoise, 0.25);
		InitFloatProperty(ReactAndDestroyPowerGeneratorNoise, 0.1);
		InitFloatProperty(ReactAndDestroyAlarmClockNoise, 1);
		InitFloatProperty(ReactAndDestroyKitchenTimerNoise, 1);
		InitFloatProperty(ReactAndDestroySmokeGrenadeVisual, 0.5);

        InitBoolProperty(DamageToPlayerHandlerIsActive, true);
        InitFloatProperty(DamageToPlayerHealthMultiplier, 1.2);
		InitFloatProperty(DamageToPlayerInBlockHealthMultiplier, 0.6);
        InitFloatProperty(DamageToPlayerShockMultiplier, 1.2);
		InitFloatProperty(DamageToPlayerInBlockShockMultiplier, 0.6);
        InitFloatProperty(DamageToPlayerStaminaPercent, 12);
		InitFloatProperty(DamageToPlayerInBlockStaminaPercent, 6);
        InitFloatProperty(DamageToPlayerBloodPercent, 0);
		InitFloatProperty(DamageToPlayerInBlockBloodPercent, 0);
        InitBoolProperty(DamageToPlayerBleedingHandlerIsActive, true);
        InitFloatProperty(DamageToPlayerBleedingChancePercent, 10);
        InitFloatProperty(DamageToPlayerInBlockBleedingChancePercent, 5);

		InitFloatProperty(StunToPlayerChancePercent, 25);
        InitFloatProperty(StunToPlayerInBlockChancePercent, 12.5);
		
		InitFloatProperty(DisarmToPlayerChancePercent, 0);
        InitFloatProperty(DisarmToPlayerInBlockChancePercent, 0);
		
		InitFloatProperty(FastAttackToPlayerChancePercent, 100);
		
		InitBoolProperty(DiseasesToPlayerHandlerIsActive, true);
		
		if (!DiseasesToPlayerAgents) {
			DiseasesToPlayerAgents = new map<string, ref array<ref InediaInfectedAI_ConfigZombieDeseaseAgent>>;
		}
		if (!DiseasesToPlayerAgents.Contains("all")) {
			DiseasesToPlayerAgents["all"] = new array<ref InediaInfectedAI_ConfigZombieDeseaseAgent>;
		}

		InitFloatProperty(CameraShakeToPlayerIntensity, 10);

		InitBoolProperty(PainToPlayerHandlerIsActive, true);
		InitFloatProperty(PainToPlayerHeadMultiplier, 1.2);
		InitFloatProperty(PainToPlayerHeadInBlockMultiplier, 0.6);
		InitFloatProperty(PainToPlayerArmsMultiplier, 1.2);
		InitFloatProperty(PainToPlayerArmsInBlockMultiplier, 0.6);
		InitFloatProperty(PainToPlayerArmsDisarmMultiplier, 0.3);
		InitFloatProperty(PainToPlayerArmsInBlockDisarmMultiplier, 0.15);
		InitFloatProperty(PainToPlayerLegsMultiplier, 1.2);
		InitFloatProperty(PainToPlayerLegsInBlockMultiplier, 0.6);
		InitFloatProperty(PainToPlayerTorsoMultiplier, 1.2);
		InitFloatProperty(PainToPlayerTorsoInBlockMultiplier, 0.6);
		
		InitFloatProperty(PainToPlayerLimbsBreakChancesMultiplier, 1);
		InitFloatProperty(PainToPlayerLimbsDeepWoundChancesMultiplier, 1);

        InitBoolProperty(DamageToZombieHandlerIsActive, true);
		InitFloatProperty(DamageToZombieHealthPoints, -1);
		InitFloatProperty(DamageToZombieHealthPointsLeg, -1);
        InitFloatProperty(DamageToZombieHeadMeleeMultiplier, 0.4);
        InitFloatProperty(DamageToZombieHeadRangeMultiplier, 1);
		InitFloatProperty(DamageToZombieBodyMeleeMultiplier, 0.4);
        InitFloatProperty(DamageToZombieBodyRangeMultiplier, 0.4);
        InitFloatProperty(DamageToZombieFromCarsMultiplier, 0.4);
        InitFloatProperty(DamageToZombieFromExplosionsMultiplier, 0.4);
        InitFloatProperty(DamageToZombieFromHotItemsHp, 10);
		InitBoolProperty(DamageToZombieBodyPartsPiercingIsActive, true);
        InitBoolProperty(DamageToZombieAlwaysKillOnBackstab, true);
		InitBoolProperty(DamageToZombieShockToStunHandlerIsActive, true);
		InitFloatProperty(DamageToZombieShockToStunLightHeavyAnimationThresholdMeleeHeavy, 0);
		InitFloatProperty(DamageToZombieShockToStunLightHeavyAnimationThresholdRange, 70);
		InitFloatProperty(DamageToZombieShockToStunThresholdMeleeHeavy, 0);
		InitFloatProperty(DamageToZombieShockToStunThresholdRange, 0);
		InitFloatProperty(DamageToZombieShockToStunFromButtstockHit, 2);
		InitBoolProperty(DamageToZombieShockToStunCumulativeDamage, true);
		InitFloatProperty(DamageToZombieShockToStunCumulativeDamageTimeFrameSeconds, 0.1);
		InitFloatProperty(DamageToZombieShockToStunIgnoreMeleeHitChancePercent, 10);
		InitFloatProperty(DamageToZombieShockToStunIgnoreRangedHitChancePercent, 10);
		InitFloatProperty(DamageToZombieShockToStunImmunityAfterMeleeHitSeconds, 4);
		InitFloatProperty(DamageToZombieShockToStunImmunityAfterRangedHitSeconds, 4);
		map<string, float> DamageToZombieWeaponsMultipliersDefaultValue = new map<string, float>;
        DamageToZombieWeaponsMultipliersDefaultValue["all"] = 1;
        InitMapFloatProperty(DamageToZombieWeaponsMultipliers, DamageToZombieWeaponsMultipliersDefaultValue);
		if (DamageToZombieWeaponsMultipliers) {
			foreach (string DamageToZombieWeaponsMultipliersKey, map<string, float> DamageToZombieWeaponsMultipliersValue: DamageToZombieWeaponsMultipliers) {
				if (DamageToZombieWeaponsMultipliersValue && !DamageToZombieWeaponsMultipliersValue.Contains("all")) {
					DamageToZombieWeaponsMultipliersValue["all"] = 1;
				}
			}
		}

		InitBoolProperty(MeleeAttacksDodgeHandlerIsActive, true);
		InitFloatProperty(MeleeAttacksDodgeChance, 50);
		InitFloatProperty(MeleeAttacksDodgeCooldownSeconds, 10);
		
		InitBoolProperty(JumpAttackHandlerIsActive, true);
		InitFloatProperty(JumpAttackChance, 10);
		InitFloatProperty(JumpAttackCooldownSeconds, 60);

		InitBoolProperty(BloodHandlerIsActive, true);
		InitFloatProperty(BloodVolumeMl, 0);
		InitFloatProperty(BloodLossRateMinMl, 5);
		InitFloatProperty(BloodLossRateMaxMl, 60);
		InitFloatProperty(BloodLossRateRegenMl, 0.2);
		InitFloatProperty(BloodOnExplosiveDamageChancePercent, 50);
		InitBoolProperty(BloodSplatParticlesIsActive, true);
		InitBoolProperty(BloodTrailParticlesIsActive, true);
		InitBoolProperty(BloodPoolParticlesIsActive, true);

        InitBoolProperty(ResistContaminatedEffectHandlerIsActive, true);
        InitBoolProperty(ResistContaminatedEffect, false);

        InitBoolProperty(AllowCrawling, true);
        InitFloatProperty(RespawnInCrawlingChancePercent, 0);

		InitBoolProperty(ScreamHandlerIsActive, true);
		InitFloatProperty(ScreamAttractsZombiesInRadius, 50);
        InitFloatProperty(ScreamChancePercent, 25);
        InitFloatProperty(ScreamCooldownSeconds, 60);
        InitFloatProperty(ScreamNearbyInfectedSilenceSeconds, 60);

        InitBoolProperty(SearchBodyToViewCargo, true);
        InitFloatProperty(SearchBodyToViewCargoSeconds, 3);
        InitFloatProperty(SearchBodyExtendLootingToPlayersInRadiusMeters, 30);
        InitBoolProperty(SearchBodyOnlyEmptyHands, false);
        InitFloatProperty(SearchBodyWithoutGlovesBloodyHandsChancePercent, 50);
        InitFloatProperty(SearchBodyWithoutMaskVomitChancePercent, 10);

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------:::::::::::::::::::::----------------------------------------------------------------
// -----------------------------------------------------------::::::::::::::::::::::::::::::::::::-------------------------------------------------------
// ----------------------------------------------------:::::::::::::::::::::::::::::::::::::::::::::::::-------------------------------------------------
// -----------------------------------------------::::::::::::::::::::::::::::::::::::::--==--:::::::::::::::--------------------------------------------
// -------------------------------------------:::::::::::::::::::::::::::::-+++++++++***+=--======++=-:::::::::::----------------------------------------
// ---------------------------------------:::::::::::::::::::::::::::::-+**+-.........-::::. ::-====-=+++++=-::::::--------------------------------------
// ------------------------------------:::::::::::::::::::::::::::::-+*+-..       -++=:...  .::.    :-=++--=+*=-:::::::----------------------------------
// ---------------------------------:::::::::::::::::::::::::::::=**+:.        :--     .:::::...  ..... .-=+=-===::::::::::------------------------------
// ------------------------------:::::::::::::::::::::::::::::-*#*+=--::...----.    .::.                    :=+=-==::::::::::----------------------------
// ----------------------------:::::::::::::::::::::::::::::-*+:-=.....::=*%*====-=%*-.  .::..              .: .-++#*=:::::::::--------------------------
// -------------------------:::::::::::::::::::::::::::::..-#.:-=.......:--:......::+*@%*.: .:::.                 :+#:-:::::::::::-----------------------
// ------------------------:::::::::::::::::::::::::.......#:-:=:.....:---......:-==:#%@@%.-:  ..:... ...   ...   -::++::::::::::::----------------------
// -----------------------:::::::::::::::::::::::.........=+--:+:::::-:-:.::::---=::+-==+%# =  :::-:..  .--.....   .-::#:::::::::::::--------------------
// ----------------------::::::::::::::::::::::..........:%:+:=:::::::-:::::-==--::=::---#@ ---:...::::::::-::.:=:    -+*::::::::::::::------------------
// ---------------------::::::::::::::::::::.............*==::+::::::---:::+*---::-::-=--%*:+.-::---....  :--..:-+=    -.#:::::::::::::::----------------
// -------------------:::::::::::::::::::::.............-%.+::=:::::==-:::*+--::--::----#=-=.:...::-=--=-:: ::--.:==: .*-*=::::::::::::::::--------------
// -----------------:::::::::::::::::::::...............%.-=:+-::::+*::-++==-::==-----=#==:-:    .   .::-::-:=+:---:.::+-#-:::::::::::::::::-------------
// ---------------:::::::::::::::::::::................+#.*--*----++-+**-==---+==+===**==. .....::-::.   :::-=-#=--==-:+*.#:::::::::::::::::::-----------
// --------------:::::::::::::::::::.................=*=+.@=-*---**==++-*=--+#++*=-+*=--:.===+#%@@@%*++=:  .-.--+==:.:-%*-%::::::::::::::::::::----------
// ------------::::::::::::::::::::..................=**--@+=%**%*==**+#+=+*+-=#=*+=-----:.=@%%@%###%@@@@#: -. -.==#===-=:+*::::::::::::::::::::---------
// -----------::::::::::::::::::::................... *@:+@#+.   :+#%#%*+=#===%#+:-:::=-:+%%#%*++*++**%@@@@=::   :-:- .=*=-**..:::::::::::::::::---------
// ----------:::::::::::::::::::...................   .%:#%:=#%#+.::-*******++=:--...:+-*=*%%*%@*:....=%@@@@*=::.== *. . .  :*-:::::::::::::::::::-------
// ---------::::::::::::::::::::.................     :#:@-+#-+%%%=.=.+ +-=--:-. .:.  :-*---#%@%+:::%-.*@@@@@@%%#+--++:.==   .-#:::::::::::::::::::------
// --------:::::::::::::::::::..................      =#-@:+ +*:*%@##-* +:  ::--::.  :-.:+**##*%*=---=*%@@@@@@@%=. ...%@@%%##:-=#::::::::::::::::::------
// -------:::::::::::::::::::.................        :%=@* :*:-+@@@%:+*=       :--=-.:::--+#@@####**#%@@@%#%#=:--- .--%@#@@%*:.#-.::::::::::::::::::----
// ------::::::::::::::::::::................          =#*#.:%:.#@@=:.%@%+:  .-           :-=+*#**###%##*+==+:--:---::-=+#*%@#-*=....::::::::::::::::----
// ------:::::::::::::::::::................           ++##: =#-+@@= .@@+@@@#-.::-##*=:-+##*=: .=+++++--:-=*+=----.=%@@@+**+%+-.......::::::::::::::::---
// ------::::::::::::::::::................            #+:%+: .*%@@@*:=@+-%%%@@#=-=--+**=:.-*@@#=::::.-*%#*+-=-+.-%@@@%@=#**-.........::::::::::::::::---
// -----::::::::::::::::::................              =#=*:+:.:=:-++=*::.==:##@@#*=------- .#%%*+::*%%+*==:=@ =@@@@%###@%+...........::::::::::::::::--
// -----::::::::::::::::::...............                -%=*.-..-:=-:-::*:- . .%@@@@%%%%+#@= .+%.+--+**+:* :+@.+@@@@#+%+*##............::::::::::::::::-
// ----::::::::::::::::::................                 =*+@%*--::..=@%@#.:.  #@@@@#%%%%:*@. +%:+-*++:.*-=..=##:-=****#=*#-...........::::::::::::::::-
// ---:::::::::::::::::::...............                   %=%*#@#++*%@@@@@%*   *@@*...=%##.%= =#**++::.=**:- :.@.-:#=+@#.: +-..........::::::::::::::::-
// ---:::::::::::::::::::...............                   =#+=:+@#+*%%%@@@@*   *@@*....#+#.#* #%#=+.: .:=+=    #  =-++%%:-+#-..........::::::::::::::::-
// ---:::::::::::::::::::...............                   .%-%:-== .+*#%@@@+   +%#@*=-+#-=.%# @*==-=%@#*@+*@*+%#+-:.:+%#%*-............::::::::::::::::-
// ---:::::::::::::::::::...............                   .%=%%-.= .-+@%@@@= . -@%*%@@#-=-=@*:*++*@#+:-@*.==.=%=#*=%=-%-................::::::::::::::::
// ----::::::::::::::::::...............                   :#=@%%.:  -*%*@@@: . :%@@*@*::: +%+:++=@@%%-*@-.+. %:.#..#+%- ................::::::::::::::::
// --::::::::::::::::::::...............                   =++%-%=:  =#+.#@%=.. .=#@@%-:.:.-%-.*.%@@@@@@@%+%+**. * :*-# ................:::::::::::::::::
// ---:::::::::::::::::::...............               .:-=%-%:-%#-  *** +@#:    .+@@*=::.:-# +--@@@@@@@@#@@@@%**#--#-*.................:::::::::::::::::
// ---:::::::::::::::::::................           :#++==#+%-.=+%:..%@@.#*   :  :-.=*##+---# *:+@+%=#%*==#--=#+@@@@%#:.................:::::::::::::::--
// ---:::::::::::::::::::................          -#..*%#%#-.-:%@.=:*%%=@. .  :-. --..-+##@#-#-=*-::..#+===#*:.*####-  ...............::::::::::::::::--
// ----:::::::::::::::::::...............         +#..-*@@#..--%%@.+:#=*+*#  -:  -=-:=-====+*%#+:**+++=*+:-+=#=+*+*:-#.................:::::::::::::::---
// -----::::::::::::::::::................      .*+  ---** :+*%@*@.+-@:=*.@#  =:   .-+. ::+#%+-=-+-##=. .*-.:*: ::*%#:.................:::::::::::::::---
// ----:::::::::::::::::::.................    :#= :-..+-*+%@#@*-@:-%@*-% *@%. =.:::.:-. :::.-+*=-+..=#:-: .*=++**=-*=................::::::::::::::::---
// -----:::::::::::::::::::.................  -#: .=.   -=+@@@==.@::***=@ .%@@+-%*+*#*+=+*+*=..--++-- -%-*.+-=+*-=**=................::::::::::::::::----
// ------:::::::::::::::::::.................+%.   +     .+=@+.-=@::++-*@. =@@@@@@@@@@@#-. ...::-. -.- =* +=:.+:. %..................::::::::::::::::----
// ------::::::::::::::::::::..............=#%.    -.      +-%=#-@.:*.:*%:..@@@@%#%@@@@@@@@%*--=+*-    :. :.  -   +=................::::::::::::::::-----
// -------::::::::::::::::::::........:=++*+@@: .. .        -=*@+# -+ .##-. @@#%*#*-#@@@@@@@@@@#*##-::      .-. .:.%...............:::::::::::::::::-----
// --------::::::::::::::::::::...:=**##=-. +@@: .:          :+=@- *=  **+: %@:=#.-#@@@%@@#+=#@@@@@@%**=:..-+#@@@=:#...............:::::::::::::::::-----
// --------::::::::::::::::::::-+**+-:  +=*+-=@@= ::           +=+-@:  +=#- ##++:#@%*+**-.-*@%#++#%+=-=++++****#*+#:.............::::::::::::::::::------
// ----------::::::::::::::=+***-:.   .=---.  =@@* .-           :=+%.  *-%-.=@::%%+:**: :##*-:-*#-        ......:+..............::::::::::::::::::-------
// -----------::::::::-=*#**--: .::::::     :--#@@#..:            ==*- *-#==.@#*#-*+.  =++-..*@##=        .....................:::::::::::::::::::-------
// ------------::-=+***+==--:-:-=--.     :--.   #@@%..-            .==*%+-#= #%==%%. :#%*:.:%@%+-*+      ....................::::::::::::::::::::--------
// ------------=**+==**=-:::+--:      :--:=:    -@@@%:.-             -=+%:#=--%#@%-.*@@%- =@@*. * +#.   ....................::::::::::::::::::::---------
// -------------::::......         .. :+...--   .#:%@@-.-              ==*%*+ %%- :%@@+ .#@@=   =.--#:....................::::::::::::::::::::::---------
// ---------------:::::......   .::::.  +-. :=. :* .%@@.=               .=+## =%=#%@%: -@@@#    :- =:%*-................:::::::::::::::::::::::----------
// ----------------::::=--:::::::::::::. -+.  :-:=: *@@=-.                :=*+:@@%#*.--@@@@@    .=  -:#+++=............::::::::::::::::::::::------------
// ------------------::::---::::::::::::. .+::. =-* :@@#.=                  :=*@@*+-+:@@@@@@+    =   -.#*==+*+:......:::::::::::::::::::::::-------------
// --------------------:::::::::::::::::::..+-.  .++ %@@ +                   :-+%#+*-%@@*@@@@-   +   .: **==:=##+-.:::::::::::::::::::::::---------------
// ---------------------::::::::::::::::::::.-=:.  *=+@@:=.               :-::=+=+%+#@@#=@=%@@:  %-   .  +=.-: .:=++==:::::::::::::::::::----------------
// -----------------------:::::::::::::::::::::=:::.+*@@- =           .--::+%@%*@%.-#@@--%+*@@%  %+    :-:%:..:    ..-#::::::::::::::::------------------
// -------------------------::::::::::::::::::::==:::#@@- +        .--::+%@@*-  *@..:.-**##=@@@: #+ -#@@@*#%-+=--.. .:=%=---::::::::::-------------------
// ---------------------------::::::::::::::::::::=:=@@@= =     .--:-+%@@%+.    =@   .: =@**%@@* #+-@@@@@+#=#=.  ::::::::::---:::::----------------------
// -----------------------------:::::::::::::::::. ===@@+ :-.-+*-=*@@%%@@=. ...-*#::    *#=@@@@+ **-@@@@@-#--+. :::::::::::::::::------------------------
// -------------------------------:::::::::::::::::.:==@@##@@@@@@@@@%-.+#@%+. --+#-.  .:%+%@*=.:--=-@@@@@#@=--.:::::::::::::::::-------------------------
// ----------------------------------:::::::::::::::::+=#@@@@@@@#==*@@#=.:+%@*=**+.*=  *%+-  ::  -+@@@@@++@:=:::::::::::::::-----------------------------
// ------------------------------------::::::::::::::::-:=%@@**=.-:..@%@%+.-#@@@@*. .: %==-.  :+%@@@@@#-::#.:::::::::::::--------------------------------
// ---------------------------------------:::::::::::::::::=-::-+:  :#=@@@*-=-+#%* ..  @  .-+%@@%%@@%=::::-::::::::::------------------------------------
// --------------------------------------------::::::::::::::::::-::.-:*@%@@#=::-# ::..% .+%%+-##@@*-*:::::::::::::--------------------------------------
// ----------------------------------------------::::::::::::::::::::::::::=#@%+:*:::::+=#*-:-:--:-:::::::::::-------------------------------------------
// --------------------------------------------------::::::::::::::::::::::::::::-::::::::::::::::::::::-------------------------------------------------
// --------------------------------------------------------::::::::::::::::::::::::::::::::::::::::------------------------------------------------------
// ----------------------------------------------------------------::::::::::::::::::::::::--------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------

		InitBoolProperty(CanBeButchered, true);
		InitFloatProperty(ButcheringSeconds, 20);
		InitFloatProperty(ButcheringWithoutGlovesBloodyHandsChancePercent, 100);
		InitFloatProperty(ButcheringWithoutMaskVomitChancePercent, 50);

		InitBoolProperty(CanBeBackstabbedHandlerIsActive, false);
		InitBoolProperty(CanBeBackstabbed, true);

		if (!ItemsAfterButchering) {
			ItemsAfterButchering = new map<string, ref array<ref InediaInfectedAI_ConfigZombieItemAfterButchering>>;
		}
		if (!ItemsAfterButchering.Contains("all")) {
			array<ref InediaInfectedAI_ConfigZombieItemAfterButchering> ItemsAfterButcheringDefaultValue = new array<ref InediaInfectedAI_ConfigZombieItemAfterButchering>;
	        ItemsAfterButcheringDefaultValue.Insert(new InediaInfectedAI_ConfigZombieItemAfterButchering("Bone", 100, 16, 84, 2, 3, {}));
			ItemsAfterButcheringDefaultValue.Insert(new InediaInfectedAI_ConfigZombieItemAfterButchering("Rag", 100, 9, 74, 2, 3, {}));
	        ItemsAfterButcheringDefaultValue.Insert(new InediaInfectedAI_ConfigZombieItemAfterButchering("Guts", 100, 30, 100, 2, 3, {}));
	        ItemsAfterButcheringDefaultValue.Insert(new InediaInfectedAI_ConfigZombieItemAfterButchering("Guts", 50, 30, 100, 2, 3, {}));
	        ItemsAfterButcheringDefaultValue.Insert(new InediaInfectedAI_ConfigZombieItemAfterButchering("HumanSteakMeat", 100, 30, 100, 2, 3, {6}));
	        ItemsAfterButcheringDefaultValue.Insert(new InediaInfectedAI_ConfigZombieItemAfterButchering("HumanSteakMeat", 50, 30, 100, 2, 3, {6}));
			ItemsAfterButchering["all"] = ItemsAfterButcheringDefaultValue;
		}

        InitFloatProperty(VisionRangeMultiplierDay, 2);
        InitFloatProperty(VisionRangeMultiplierNight, 0.8);

		InitBoolProperty(SizeHandlerIsActive, false);
		InitFloatProperty(SizeFromMultiplier, 0.9);
        InitFloatProperty(SizeToMultiplier, 1.1);
        InitBoolProperty(SizeDamageScalingIsActive, false);
        InitBoolProperty(SizeBloodVolumeScalingIsActive, true);
        InitBoolProperty(SizeSpeedMultiplierScalingIsActive, true);

        InitBoolProperty(ThrowingProjectilesHandlerIsActive, true);
        InitFloatProperty(ThrowingProjectilesHandlerDamageMultiplier, 0.9);
        InitFloatProperty(ThrowingProjectilesHandlerVehiclesDamageMultiplier, 1.8);
        InitArrayStringProperty(ThrowingProjectilesHandlerProjectilesList, {});

		InitBoolProperty(RangeAttacksHandlerIsActive, true);
		InitFloatProperty(RangeAttacksHandlerZombiePlayerDistance, 2);
		InitFloatProperty(RangeAttacksHandlerPlayerOnObstacleHeight, 0.5);
	}

	void InitializeBeforeCreatingConfigFile()
	{
		Initialize();

		// Zombie types groups
		Groups["city"] = new array<string>;
		Groups["city"].Insert("ZmbF_CitizenANormal_Base");
		Groups["city"].Insert("ZmbF_CitizenBSkinny_Base");
		Groups["city"].Insert("ZmbF_Clerk_Normal_Base");
		Groups["city"].Insert("ZmbF_Clerk_Normal_LT_Base");
		Groups["city"].Insert("ZmbF_JournalistNormal_Base");
		Groups["city"].Insert("ZmbF_JournalistNormal_LT_Base");
		Groups["city"].Insert("ZmbF_ShortSkirt_Base");
		Groups["city"].Insert("ZmbF_ShortSkirt_LT_Base");
		Groups["city"].Insert("ZmbF_SkaterYoung_Base");
		Groups["city"].Insert("ZmbM_SkaterYoung_Base");
		Groups["city"].Insert("ZmbM_SkaterYoung_LT_Base");
		Groups["city"].Insert("ZmbF_SurvivorNormal_Base");
		Groups["city"].Insert("ZmbF_SurvivorNormal_LT_Base");
		Groups["city"].Insert("ZmbM_CitizenASkinny_Base");
		Groups["city"].Insert("ZmbM_CitizenASkinny_LT_Base");
		Groups["city"].Insert("ZmbM_CitizenBFat_Base");
		Groups["city"].Insert("ZmbM_ClerkFat_Base");
		Groups["city"].Insert("ZmbM_ClerkFat_LT_Base");
		Groups["city"].Insert("ZmbF_ClerkFat_Base");
		Groups["city"].Insert("ZmbM_CommercialPilotOld_Base");
		Groups["city"].Insert("ZmbM_CommercialPilotOld_LT_Base");
		Groups["city"].Insert("ZmbM_JournalistSkinny_Base");
		Groups["city"].Insert("ZmbM_MotobikerFat_Base");
		Groups.ReplaceKey("city", "~city");

		Groups["villagers"] = new array<string>;
		Groups["villagers"].Insert("ZmbF_Runner_Base");
		Groups["villagers"].Insert("ZmbF_MilkMaidOld_Base");
		Groups["villagers"].Insert("ZmbF_MilkMaidOld_LT_Base");
		Groups["villagers"].Insert("ZmbF_VillagerOld_Base");
		Groups["villagers"].Insert("ZmbF_VillagerOld_LT_Base");
		Groups["villagers"].Insert("ZmbM_FarmerFat_Base");
		Groups["villagers"].Insert("ZmbM_FarmerFat_LT_Base");
		Groups["villagers"].Insert("ZmbM_Jacket_Base");
		Groups["villagers"].Insert("ZmbM_Jacket_LT_Base");
		Groups["villagers"].Insert("ZmbM__Runner_Base");
		Groups["villagers"].Insert("ZmbM_VillagerOld_Base");
		Groups["villagers"].Insert("ZmbM_VillagerOld_LT_Base");
		Groups["villagers"].Insert("ZmbF_HikerSkinny_Base");
		Groups["villagers"].Insert("ZmbM_HermitSkinny_Base");
		Groups["villagers"].Insert("ZmbM_HikerSkinny_Base");
		Groups.ReplaceKey("villagers", "~villagers");

		Groups["police"] = new array<string>;
		Groups["police"].Insert("ZmbF_PoliceWomanNormal_Base");
		Groups["police"].Insert("ZmbM_PolicemanFat_Base");
		Groups["police"].Insert("ZmbM_PolicemanSpecForce_Base");
		Groups.ReplaceKey("police", "~police");

		Groups["medics"] = new array<string>;
		Groups["medics"].Insert("ZmbF_DoctorSkinny_Base");
		Groups["medics"].Insert("ZmbF_NurseFat_Base");
		Groups["medics"].Insert("ZmbF_ParamedicNormal_Base");
		Groups["medics"].Insert("ZmbM_ParamedicNormal_Base");
		Groups["medics"].Insert("ZmbM_DoctorFat_Base");
		Groups["medics"].Insert("ZmbF_PatientOld_Base");
		Groups["medics"].Insert("ZmbM_PatientSkinny_Base");
		Groups.ReplaceKey("medics", "~medics");

		Groups["military"] = new array<string>;
		Groups["military"].Insert("ZmbM_SoldierNormal_Base");
		Groups.ReplaceKey("military", "~military");

		Groups["industrial"] = new array<string>;
		Groups["industrial"].Insert("ZmbF_BlueCollarFat_Base");
		Groups["industrial"].Insert("ZmbF_MechanicNormal_Base");
		Groups["industrial"].Insert("ZmbM_ConstrWorkerNormal_Base");
		Groups["industrial"].Insert("ZmbM_HandymanNormal_Base");
		Groups["industrial"].Insert("ZmbM_HeavyIndustryWorker_Base");
		Groups["industrial"].Insert("ZmbM_MechanicSkinny_Base");
		Groups["industrial"].Insert("ZmbM_OffshoreWorker_Base");
		Groups.ReplaceKey("industrial", "~industrial");

		Groups["firefighters"] = new array<string>;
		Groups["firefighters"].Insert("ZmbM_FirefighterNormal_Base");
		Groups.ReplaceKey("firefighters", "~firefighters");

		Groups["hunters"] = new array<string>;
		Groups["hunters"].Insert("ZmbM_HunterOld_Base");
		Groups.ReplaceKey("hunters", "~hunters");

		Groups["fishermen"] = new array<string>;
		Groups["fishermen"].Insert("ZmbM_FishermanOld_Base");
		Groups.ReplaceKey("fishermen", "~fishermen");

		Groups["prisoners"] = new array<string>;
		Groups["prisoners"].Insert("ZmbM_PrisonerSkinny_Base");
		Groups.ReplaceKey("prisoners", "~prisoners");

		Groups["priests"] = new array<string>;
		Groups["priests"].Insert("ZmbM_priestPopSkinny_Base");
		Groups.ReplaceKey("priests", "~priests");

		// Zombie STR/DEF groups
		Groups["lowstr"] = new array<string>;
		Groups["lowstr"].Insert("city");
		Groups["lowstr"].Insert("villagers");
		Groups["lowstr"].Insert("fishermen");
		Groups.ReplaceKey("lowstr", "~~lowstr");
		
		Groups["lowdef"] = new array<string>;
		Groups["lowdef"].Insert("city");
		Groups["lowdef"].Insert("villagers");
		Groups["lowdef"].Insert("fishermen");
		Groups.ReplaceKey("lowdef", "~~lowdef");

		Groups["mediumstr"] = new array<string>;
		Groups["mediumstr"].Insert("medics");
		Groups["mediumstr"].Insert("industrial");
		Groups["mediumstr"].Insert("priests");
		Groups["mediumstr"].Insert("prisoners");
		Groups.ReplaceKey("mediumstr", "~~mediumstr");
		
		Groups["mediumdef"] = new array<string>;
		Groups["mediumdef"].Insert("medics");
		Groups["mediumdef"].Insert("industrial");
		Groups["mediumdef"].Insert("priests");
		Groups["mediumdef"].Insert("prisoners");
		Groups.ReplaceKey("mediumdef", "~~mediumdef");

		Groups["highstr"] = new array<string>;
		Groups["highstr"].Insert("military");
		Groups["highstr"].Insert("police");
		Groups["highstr"].Insert("firefighters");
		Groups["highstr"].Insert("hunters");
		Groups["highstr"].Insert("ZmbM_Gamedev_Base");
		Groups.ReplaceKey("highstr", "~~highstr");
		
		Groups["highdef"] = new array<string>;
		Groups["highdef"].Insert("military");
		Groups["highdef"].Insert("police");
		Groups["highdef"].Insert("firefighters");
		Groups["highdef"].Insert("hunters");
		Groups["highdef"].Insert("ZmbM_Gamedev_Base");
		Groups.ReplaceKey("highdef", "~~highdef");

		// Animals groups
		Groups["bears"] = new array<string>;
		Groups["bears"].Insert("Animal_UrsusArctos");
		Groups.ReplaceKey("bears", "~bears");

		Groups["wolves"] = new array<string>;
		Groups["wolves"].Insert("Animal_CanisLupus");
		Groups.ReplaceKey("wolves", "~wolves");
		
		Groups["cattle"] = new array<string>;
		Groups["cattle"].Insert("Animal_BosTaurus");
		Groups["cattle"].Insert("Animal_BosTaurusF");
		Groups["cattle"].Insert("Horse_Base");
		Groups.ReplaceKey("cattle", "~cattle");
		
		Groups["predators"] = new array<string>;
		Groups["predators"].Insert("bears");
		Groups["predators"].Insert("wolves");
		Groups.ReplaceKey("predators", "~~predators");
		
		Groups["animals"] = new array<string>;
		Groups["animals"].Insert("AnimalBase");
		Groups.ReplaceKey("animals", "~~~animals");

		// Custom irritants
		/*
			"pClasses", "pRadiusOutdoorDay", "pRadiusOutdoorNight", "pRadiusIndoorDay", "pRadiusIndoorNight", "pPriority"
		*/
		CustomIrritants["matches"] = new InediaInfectedAI_CustomIrritant({"Single_Match", "New_Candle", "PetrolLighter"}, 0, 40, 0, 8, 0, 1);

		// Projectiles
		InediaInfectedAI_ConfigThrowingProjectile garbageIfPlayerOnObstacleProjectile = new InediaInfectedAI_ConfigThrowingProjectile();
		garbageIfPlayerOnObstacleProjectile.ItemClasses = {"WoodenStick", "SmallStone"};
		garbageIfPlayerOnObstacleProjectile.SetParameterValue("TargetHealthDamage", 5);
		garbageIfPlayerOnObstacleProjectile.SetParameterValue("TargetShockDamage", 20);
		garbageIfPlayerOnObstacleProjectile.SetParameterValue("TargetStunChancePercent", 50);
		garbageIfPlayerOnObstacleProjectile.SetParameterValue("TargetBleedingChancePercent", 5);
		garbageIfPlayerOnObstacleProjectile.SetParameterValue("ThrowConditionZombieTargetVerticalDistanceMin", 0.5);
		garbageIfPlayerOnObstacleProjectile.SetParameterValue("ThrowConditionGroundTargetDistanceMin", 0.5);
		garbageIfPlayerOnObstacleProjectile.SetParameterValue("ThrowConditionCooldownSeconds", 5);
		ThrowingProjectiles["GarbageIfPlayerOnObstacle"] = garbageIfPlayerOnObstacleProjectile;

		InediaInfectedAI_ConfigThrowingProjectile garbageIfPlayerNonObstacleProjectile = new InediaInfectedAI_ConfigThrowingProjectile();
		garbageIfPlayerNonObstacleProjectile.ItemClasses = {"WoodenStick", "SmallStone"};
		garbageIfPlayerNonObstacleProjectile.SetParameterValue("TargetHealthDamage", 5);
		garbageIfPlayerNonObstacleProjectile.SetParameterValue("TargetShockDamage", 20);
		garbageIfPlayerNonObstacleProjectile.SetParameterValue("TargetStunChancePercent", 50);
		garbageIfPlayerNonObstacleProjectile.SetParameterValue("TargetBleedingChancePercent", 5);
		garbageIfPlayerNonObstacleProjectile.SetParameterValue("ThrowConditionThrowChancePercent", 10);
		garbageIfPlayerNonObstacleProjectile.SetParameterValue("ThrowConditionCooldownSeconds", 10);
		garbageIfPlayerNonObstacleProjectile.SetParameterValue("ThrowConditionAmountOfProjectiles", 2);
		ThrowingProjectiles["GarbageIfPlayerNonObstacle"] = garbageIfPlayerNonObstacleProjectile;
		
		InediaInfectedAI_ConfigThrowingProjectile garbageIfPlayerInWaterProjectile = new InediaInfectedAI_ConfigThrowingProjectile();
		garbageIfPlayerInWaterProjectile.ItemClasses = {"WoodenStick", "SmallStone"};
		garbageIfPlayerInWaterProjectile.SetParameterValue("TargetHealthDamage", 5);
		garbageIfPlayerInWaterProjectile.SetParameterValue("TargetShockDamage", 20);
		garbageIfPlayerInWaterProjectile.SetParameterValue("TargetStunChancePercent", 50);
		garbageIfPlayerInWaterProjectile.SetParameterValue("TargetBleedingChancePercent", 5);
		garbageIfPlayerInWaterProjectile.SetParameterValue("ThrowConditionTargetInWater", 1);
		garbageIfPlayerInWaterProjectile.SetParameterValue("ThrowConditionCooldownSeconds", 5);
		ThrowingProjectiles["GarbageIfPlayerInWater"] = garbageIfPlayerInWaterProjectile;

		InediaInfectedAI_ConfigThrowingProjectile heGrenadeProjectile = new InediaInfectedAI_ConfigThrowingProjectile();
		heGrenadeProjectile.ItemClasses = {"RGD5Grenade", "M67Grenade"};
		heGrenadeProjectile.SetParameterValue("ThrowConditionAmountOfProjectiles", 1);
		heGrenadeProjectile.SetParameterValue("DestroyProjectileAfterSeconds", -1);
		heGrenadeProjectile.SetParameterValue("ActivateGrenadeAfterSeconds", 5);
		ThrowingProjectiles["HEGrenade"] = heGrenadeProjectile;

		InediaInfectedAI_ConfigThrowingProjectile flashGrenadeProjectile = new InediaInfectedAI_ConfigThrowingProjectile();
		flashGrenadeProjectile.ItemClasses = {"FlashGrenade"};
		flashGrenadeProjectile.SetParameterValue("ThrowConditionAmountOfProjectiles", 1);
		flashGrenadeProjectile.SetParameterValue("DestroyProjectileAfterSeconds", -1);
		flashGrenadeProjectile.SetParameterValue("ActivateGrenadeAfterSeconds", 0);
		ThrowingProjectiles["FlashGrenade"] = flashGrenadeProjectile;

		InediaInfectedAI_ConfigThrowingProjectile gasGrenadeProjectile = new InediaInfectedAI_ConfigThrowingProjectile();
		gasGrenadeProjectile.ItemClasses = {"Grenade_ChemGas"};
		gasGrenadeProjectile.SetParameterValue("ThrowConditionAmountOfProjectiles", 1);
		gasGrenadeProjectile.SetParameterValue("DestroyProjectileAfterSeconds", -1);
		gasGrenadeProjectile.SetParameterValue("ActivateGrenadeAfterSeconds", 0);
		ThrowingProjectiles["GasGrenade"] = gasGrenadeProjectile;

		// Thematic projectiles city
		InediaInfectedAI_ConfigThrowingProjectile cityProjectile = new InediaInfectedAI_ConfigThrowingProjectile();
		cityProjectile.ItemClasses = {"GlassBottle", "WaterBottle", "BaseballBat", "Pipe", "Flashlight", "CombinationLock", "KitchenKnife", "KitchenKnife", "SteakKnife"};
		cityProjectile.SetParameterValue("ThrowConditionThrowChancePercent", 10);
		cityProjectile.SetParameterValue("ThrowConditionCooldownSeconds", 10);
		cityProjectile.SetParameterValue("ThrowConditionAmountOfProjectiles", 2);
		ThrowingProjectiles["CityProjectile"] = cityProjectile;

		// Thematic projectiles villagers
		InediaInfectedAI_ConfigThrowingProjectile villagersProjectile = new InediaInfectedAI_ConfigThrowingProjectile();
		villagersProjectile.ItemClasses = {"GlassBottle", "FryingPan", "Firewood", "Pot", "Hammer", "MeatTenderizer", "KitchenKnife", "Pitchfork", "Hatchet", "Sickle", "WoodAxe", "Cleaver", "Shovel", "FarmingHoe"};
		villagersProjectile.SetParameterValue("ThrowConditionThrowChancePercent", 10);
		villagersProjectile.SetParameterValue("ThrowConditionCooldownSeconds", 10);
		villagersProjectile.SetParameterValue("ThrowConditionAmountOfProjectiles", 2);
		ThrowingProjectiles["VillagersProjectile"] = villagersProjectile;

		// Thematic projectiles industrial
		InediaInfectedAI_ConfigThrowingProjectile industrialProjectile = new InediaInfectedAI_ConfigThrowingProjectile();
		industrialProjectile.ItemClasses = {"GlassBottle", "Pipe", "PipeWrench", "Wrench", "LugWrench", "Hammer", "WeldingMask", "Crowbar", "HandSaw", "Hacksaw", "Screwdriver", "Pliers"};
		industrialProjectile.SetParameterValue("ThrowConditionThrowChancePercent", 10);
		industrialProjectile.SetParameterValue("ThrowConditionCooldownSeconds", 10);
		industrialProjectile.SetParameterValue("ThrowConditionAmountOfProjectiles", 2);
		ThrowingProjectiles["IndustrialProjectile"] = industrialProjectile;

		// Thematic projectiles military
		InediaInfectedAI_ConfigThrowingProjectile militaryProjectile = new InediaInfectedAI_ConfigThrowingProjectile();
		militaryProjectile.ItemClasses = {"Binoculars", "Ssh68Helmet", "GorkaHelmet", "BallisticHelmet_Green", "Canteen", "CombatKnife", "AK_Bayonet", "SKS_Bayonet", "M9A1_Bayonet", "Mosin_Bayonet"};
		militaryProjectile.SetParameterValue("ThrowConditionThrowChancePercent", 10);
		militaryProjectile.SetParameterValue("ThrowConditionCooldownSeconds", 10);
		militaryProjectile.SetParameterValue("ThrowConditionAmountOfProjectiles", 2);
		ThrowingProjectiles["MilitaryProjectile"] = militaryProjectile;

		// Thematic projectiles police
		InediaInfectedAI_ConfigThrowingProjectile policeProjectile = new InediaInfectedAI_ConfigThrowingProjectile();
		policeProjectile.ItemClasses = {"Glock19", "MakarovIJ70", "Mp133Shotgun", "CombatKnife", "CombatKnife"};
		policeProjectile.SetParameterValue("ThrowConditionThrowChancePercent", 10);
		policeProjectile.SetParameterValue("ThrowConditionCooldownSeconds", 10);
		policeProjectile.SetParameterValue("ThrowConditionAmountOfProjectiles", 2);
		ThrowingProjectiles["PoliceProjectile"] = policeProjectile;

		// Thematic projectiles firefighters
		InediaInfectedAI_ConfigThrowingProjectile firefightersProjectile = new InediaInfectedAI_ConfigThrowingProjectile();
		firefightersProjectile.ItemClasses = {"FirefightersHelmet_Red", "FirefightersHelmet_White", "FirefightersHelmet_Yellow", "FirefighterAxe", "FirefighterAxe"};
		firefightersProjectile.SetParameterValue("ThrowConditionThrowChancePercent", 10);
		firefightersProjectile.SetParameterValue("ThrowConditionCooldownSeconds", 10);
		firefightersProjectile.SetParameterValue("ThrowConditionAmountOfProjectiles", 2);
		ThrowingProjectiles["FirefightersProjectile"] = firefightersProjectile;

		// Thematic projectiles medics
		InediaInfectedAI_ConfigThrowingProjectile medicsProjectile = new InediaInfectedAI_ConfigThrowingProjectile();
		medicsProjectile.ItemClasses = {"GlassBottle", "KitchenKnife", "KitchenKnife", "SteakKnife", "GasMask_Filter", "IodineTincture", "DisinfectantAlcohol", "DisinfectantSpray"};
		medicsProjectile.SetParameterValue("ThrowConditionThrowChancePercent", 10);
		medicsProjectile.SetParameterValue("ThrowConditionCooldownSeconds", 10);
		medicsProjectile.SetParameterValue("ThrowConditionAmountOfProjectiles", 2);
		ThrowingProjectiles["MedicsProjectile"] = medicsProjectile;

		// Thematic projectiles hunters
		InediaInfectedAI_ConfigThrowingProjectile huntersProjectile = new InediaInfectedAI_ConfigThrowingProjectile();
		huntersProjectile.ItemClasses = {"GlassBottle", "HuntingKnife", "HuntingKnife", "Canteen", "Binoculars"};
		huntersProjectile.SetParameterValue("ThrowConditionThrowChancePercent", 10);
		huntersProjectile.SetParameterValue("ThrowConditionCooldownSeconds", 10);
		huntersProjectile.SetParameterValue("ThrowConditionAmountOfProjectiles", 2);
		ThrowingProjectiles["HuntersProjectile"] = huntersProjectile;

		// Thematic projectiles fishermen
		InediaInfectedAI_ConfigThrowingProjectile fishermenProjectile = new InediaInfectedAI_ConfigThrowingProjectile();
		fishermenProjectile.ItemClasses = {"GlassBottle", "HuntingKnife", "KitchenKnife", "Canteen", "FishingRod", "Shovel"};
		fishermenProjectile.SetParameterValue("ThrowConditionThrowChancePercent", 10);
		fishermenProjectile.SetParameterValue("ThrowConditionCooldownSeconds", 10);
		fishermenProjectile.SetParameterValue("ThrowConditionAmountOfProjectiles", 2);
		ThrowingProjectiles["FishermenProjectile"] = fishermenProjectile;		

		// High STR zombies options
		BreakingDoorsOpenChancePercent["highstr"] = 50;
		BreakingDoorsDestroyAfterOpenChancePercent["highstr"] = 50;

		DamageToPlayerHealthMultiplier["highstr"] = 1.4;
		DamageToPlayerInBlockHealthMultiplier["highstr"] = 0.7;

		DamageToPlayerShockMultiplier["highstr"] = 1.4;
		DamageToPlayerInBlockShockMultiplier["highstr"] = 0.7;

		DamageToPlayerStaminaPercent["highstr"] = 20;
		DamageToPlayerInBlockStaminaPercent["highstr"] = 10;

		DamageToPlayerBleedingChancePercent["highstr"] = 16;
		DamageToPlayerInBlockBleedingChancePercent["highstr"] = 8;

		StunToPlayerChancePercent["highstr"] = 35;
		StunToPlayerInBlockChancePercent["highstr"] = 17.5;

		PainToPlayerHeadMultiplier["highstr"] = 1.4;
		PainToPlayerHeadInBlockMultiplier["highstr"] = 0.7;

		PainToPlayerArmsMultiplier["highstr"] = 1.4;
		PainToPlayerArmsInBlockMultiplier["highstr"] = 0.7;

		PainToPlayerArmsDisarmMultiplier["highstr"] = 0.5;
		PainToPlayerArmsInBlockDisarmMultiplier["highstr"] = 0.25;

		PainToPlayerLegsMultiplier["highstr"] = 1.4;
		PainToPlayerLegsInBlockMultiplier["highstr"] = 0.7;

		PainToPlayerTorsoMultiplier["highstr"] = 1.4;
		PainToPlayerTorsoInBlockMultiplier["highstr"] = 0.7;

		ThrowingProjectilesHandlerDamageMultiplier["highstr"] = 1.1;
		ThrowingProjectilesHandlerVehiclesDamageMultiplier["highstr"] = 2.2;
		
		DiseasesToPlayerAgents["highstr"] = new array<ref InediaInfectedAI_ConfigZombieDeseaseAgent>;
		DiseasesToPlayerAgents["highstr"].Insert(new InediaInfectedAI_ConfigZombieDeseaseAgent(64, 20, 10, 10, 5, 50, true, true));

		// High DEF zombies options
		DamageToZombieHeadMeleeMultiplier["highdef"] = 0.2;
		DamageToZombieBodyMeleeMultiplier["highdef"] = 0.2;
		DamageToZombieBodyRangeMultiplier["highdef"] = 0.2;
		DamageToZombieFromCarsMultiplier["highdef"] = 0.2;
		DamageToZombieFromExplosionsMultiplier["highdef"] = 0.2;

		// Medium STR zombies options
		BreakingDoorsOpenChancePercent["mediumstr"] = 40;
		BreakingDoorsDestroyAfterOpenChancePercent["mediumstr"] = 40;

		DamageToPlayerHealthMultiplier["mediumstr"] = 1.3;
		DamageToPlayerInBlockHealthMultiplier["mediumstr"] = 0.65;

		DamageToPlayerShockMultiplier["mediumstr"] = 1.3;
		DamageToPlayerInBlockShockMultiplier["mediumstr"] = 0.65;

		DamageToPlayerStaminaPercent["mediumstr"] = 16;
		DamageToPlayerInBlockStaminaPercent["mediumstr"] = 8;

		DamageToPlayerBleedingChancePercent["mediumstr"] = 13;
		DamageToPlayerInBlockBleedingChancePercent["mediumstr"] = 6.5;

		StunToPlayerChancePercent["mediumstr"] = 30;
		StunToPlayerInBlockChancePercent["mediumstr"] = 15;

		PainToPlayerHeadMultiplier["mediumstr"] = 1.3;
		PainToPlayerHeadInBlockMultiplier["mediumstr"] = 0.65;

		PainToPlayerArmsMultiplier["mediumstr"] = 1.3;
		PainToPlayerArmsInBlockMultiplier["mediumstr"] = 0.65;

		PainToPlayerArmsDisarmMultiplier["mediumstr"] = 0.4;
		PainToPlayerArmsInBlockDisarmMultiplier["mediumstr"] = 0.2;

		PainToPlayerLegsMultiplier["mediumstr"] = 1.3;
		PainToPlayerLegsInBlockMultiplier["mediumstr"] = 0.65;

		PainToPlayerTorsoMultiplier["mediumstr"] = 1.3;
		PainToPlayerTorsoInBlockMultiplier["mediumstr"] = 0.65;

		ThrowingProjectilesHandlerDamageMultiplier["mediumstr"] = 1;
		ThrowingProjectilesHandlerVehiclesDamageMultiplier["mediumstr"] = 2;
		
		DiseasesToPlayerAgents["mediumstr"] = new array<ref InediaInfectedAI_ConfigZombieDeseaseAgent>;
		DiseasesToPlayerAgents["mediumstr"].Insert(new InediaInfectedAI_ConfigZombieDeseaseAgent(64, 15, 7.5, 10, 5, 50, true, true));

		// Medium DEF zombies options
		DamageToZombieHeadMeleeMultiplier["mediumdef"] = 0.3;
		DamageToZombieBodyMeleeMultiplier["mediumdef"] = 0.3;
		DamageToZombieBodyRangeMultiplier["mediumdef"] = 0.3;
		DamageToZombieFromCarsMultiplier["mediumdef"] = 0.3;
		DamageToZombieFromExplosionsMultiplier["mediumdef"] = 0.3;

		// Low STR zombies options
		BreakingDoorsOpenChancePercent["lowstr"] = 30;
		BreakingDoorsDestroyAfterOpenChancePercent["lowstr"] = 30;

		DamageToPlayerHealthMultiplier["lowstr"] = 1.2;
		DamageToPlayerInBlockHealthMultiplier["lowstr"] = 0.6;

		DamageToPlayerShockMultiplier["lowstr"] = 1.2;
		DamageToPlayerInBlockShockMultiplier["lowstr"] = 0.6;

		DamageToPlayerStaminaPercent["lowstr"] = 12;
		DamageToPlayerInBlockStaminaPercent["lowstr"] = 6;

		DamageToPlayerBleedingChancePercent["lowstr"] = 10;
		DamageToPlayerInBlockBleedingChancePercent["lowstr"] = 5;

		StunToPlayerChancePercent["lowstr"] = 25;
		StunToPlayerInBlockChancePercent["lowstr"] = 12.5;

		PainToPlayerHeadMultiplier["lowstr"] = 1.2;
		PainToPlayerHeadInBlockMultiplier["lowstr"] = 0.6;

		PainToPlayerArmsMultiplier["lowstr"] = 1.2;
		PainToPlayerArmsInBlockMultiplier["lowstr"] = 0.6;

		PainToPlayerArmsDisarmMultiplier["lowstr"] = 0.3;
		PainToPlayerArmsInBlockDisarmMultiplier["lowstr"] = 0.15;

		PainToPlayerLegsMultiplier["lowstr"] = 1.2;
		PainToPlayerLegsInBlockMultiplier["lowstr"] = 0.6;

		PainToPlayerTorsoMultiplier["lowstr"] = 1.2;
		PainToPlayerTorsoInBlockMultiplier["lowstr"] = 0.6;

		ThrowingProjectilesHandlerDamageMultiplier["lowstr"] = 0.9;
		ThrowingProjectilesHandlerVehiclesDamageMultiplier["lowstr"] = 1.8;
		
		DiseasesToPlayerAgents["lowstr"] = new array<ref InediaInfectedAI_ConfigZombieDeseaseAgent>;
		DiseasesToPlayerAgents["lowstr"].Insert(new InediaInfectedAI_ConfigZombieDeseaseAgent(64, 10, 5, 10, 5, 50, true, true));

		// Low DEF zombies options
		DamageToZombieHeadMeleeMultiplier["lowdef"] = 0.4;
		DamageToZombieBodyMeleeMultiplier["lowdef"] = 0.4;
		DamageToZombieBodyRangeMultiplier["lowdef"] = 0.4;
		DamageToZombieFromCarsMultiplier["lowdef"] = 0.4;
		DamageToZombieFromExplosionsMultiplier["lowdef"] = 0.4;

		// Survivors options
		PainToPlayerHeadMultiplier["SurvivorBase"] = 1;
		PainToPlayerHeadInBlockMultiplier["SurvivorBase"] = 0.5;
		PainToPlayerArmsMultiplier["SurvivorBase"] = 1;
		PainToPlayerArmsInBlockMultiplier["SurvivorBase"] = 0.5;
		PainToPlayerArmsDisarmMultiplier["SurvivorBase"] = 0.3;
		PainToPlayerArmsInBlockDisarmMultiplier["SurvivorBase"] = 0.15;
		PainToPlayerLegsMultiplier["SurvivorBase"] = 1;
		PainToPlayerLegsInBlockMultiplier["SurvivorBase"] = 0.5;
		PainToPlayerTorsoMultiplier["SurvivorBase"] = 1;
		PainToPlayerTorsoInBlockMultiplier["SurvivorBase"] = 0.5;
		ThrowingProjectilesHandlerDamageMultiplier["SurvivorBase"] = 1.1;
		ThrowingProjectilesHandlerVehiclesDamageMultiplier["SurvivorBase"] = 2.2;

		// Resist contaminated effect for some creatures
		ResistContaminatedEffect["SurvivorBase"] = false;
		ResistContaminatedEffect["ZmbM_NBC_Grey"] = true;
		ResistContaminatedEffect["ZmbM_NBC_Yellow"] = true;
		ResistContaminatedEffect["ZmbM_Mummy"] = true;
		
		// Officers attract the infected with their screaming much more effectively
		ScreamAttractsZombiesInRadius["ZmbM_usSoldier_Officer_Desert"] = 120;
		ScreamChancePercent["ZmbM_usSoldier_Officer_Desert"] = 100;

		// Car damage global multiplier for bears
		AttackCarElementsGlobalDamageMultiplier["bears"] = 3;

		// Disabling damage to car elements by wolves
		AttackCarElementsIsActive["wolves"] = 0;

		// Injury handler for animals
		InjuryHandlerDamageThresholdToInjuryLeg["bears"] = 50;
		InjuryHandlerRestoreInjuredLegAfterSeconds["animals"] = 900;
		
		// Transmission of sepsis agents from predators
		DiseasesToPlayerAgents["predators"] = new array<ref InediaInfectedAI_ConfigZombieDeseaseAgent>;
		DiseasesToPlayerAgents["predators"].Insert(new InediaInfectedAI_ConfigZombieDeseaseAgent(64, 10, 5, 10, 5, 50, true, true));
		
		// Blood volume and rate for animals
		BloodVolumeMl["cattle"] = 30000;
		BloodLossRateMaxMl["cattle"] = 120;
		BloodVolumeMl["bears"] = 15000;
		BloodLossRateMaxMl["bears"] = 120;

		// Binding projectiles to the infected
		ThrowingProjectilesHandlerProjectilesList["city"] = {"GarbageIfPlayerOnObstacle", "CityProjectile"};
		ThrowingProjectilesHandlerProjectilesList["villagers"] = {"GarbageIfPlayerOnObstacle", "VillagersProjectile"};
		ThrowingProjectilesHandlerProjectilesList["industrial"] = {"GarbageIfPlayerOnObstacle", "IndustrialProjectile"};
		ThrowingProjectilesHandlerProjectilesList["military"] = {"GarbageIfPlayerOnObstacle", "MilitaryProjectile"};
		ThrowingProjectilesHandlerProjectilesList["police"] = {"GarbageIfPlayerOnObstacle", "PoliceProjectile"};
		ThrowingProjectilesHandlerProjectilesList["prisoners"] = {"GarbageIfPlayerOnObstacle", "GarbageIfPlayerNonObstacle"};
		ThrowingProjectilesHandlerProjectilesList["firefighters"] = {"GarbageIfPlayerOnObstacle", "FirefightersProjectile"};
		ThrowingProjectilesHandlerProjectilesList["medics"] = {"GarbageIfPlayerOnObstacle", "MedicsProjectile"};
		ThrowingProjectilesHandlerProjectilesList["hunters"] = {"GarbageIfPlayerOnObstacle", "HuntersProjectile"};
		ThrowingProjectilesHandlerProjectilesList["fishermen"] = {"GarbageIfPlayerOnObstacle", "FishermenProjectile"};
		ThrowingProjectilesHandlerProjectilesList["priests"] = {"GarbageIfPlayerOnObstacle", "GarbageIfPlayerNonObstacle"};
	}
	
	void InitializeAfterCreatingConfigFile()
	{
		UpdateSortedGroupNames();
	}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------:::::::::::::::::::::----------------------------------------------------------------
// -----------------------------------------------------------::::::::::::::::::::::::::::::::::::-------------------------------------------------------
// ----------------------------------------------------:::::::::::::::::::::::::::::::::::::::::::::::::-------------------------------------------------
// -----------------------------------------------::::::::::::::::::::::::::::::::::::::--==--:::::::::::::::--------------------------------------------
// -------------------------------------------:::::::::::::::::::::::::::::-+++++++++***+=--======++=-:::::::::::----------------------------------------
// ---------------------------------------:::::::::::::::::::::::::::::-+**+-.........-::::. ::-====-=+++++=-::::::--------------------------------------
// ------------------------------------:::::::::::::::::::::::::::::-+*+-..       -++=:...  .::.    :-=++--=+*=-:::::::----------------------------------
// ---------------------------------:::::::::::::::::::::::::::::=**+:.        :--     .:::::...  ..... .-=+=-===::::::::::------------------------------
// ------------------------------:::::::::::::::::::::::::::::-*#*+=--::...----.    .::.                    :=+=-==::::::::::----------------------------
// ----------------------------:::::::::::::::::::::::::::::-*+:-=.....::=*%*====-=%*-.  .::..              .: .-++#*=:::::::::--------------------------
// -------------------------:::::::::::::::::::::::::::::..-#.:-=.......:--:......::+*@%*.: .:::.                 :+#:-:::::::::::-----------------------
// ------------------------:::::::::::::::::::::::::.......#:-:=:.....:---......:-==:#%@@%.-:  ..:... ...   ...   -::++::::::::::::----------------------
// -----------------------:::::::::::::::::::::::.........=+--:+:::::-:-:.::::---=::+-==+%# =  :::-:..  .--.....   .-::#:::::::::::::--------------------
// ----------------------::::::::::::::::::::::..........:%:+:=:::::::-:::::-==--::=::---#@ ---:...::::::::-::.:=:    -+*::::::::::::::------------------
// ---------------------::::::::::::::::::::.............*==::+::::::---:::+*---::-::-=--%*:+.-::---....  :--..:-+=    -.#:::::::::::::::----------------
// -------------------:::::::::::::::::::::.............-%.+::=:::::==-:::*+--::--::----#=-=.:...::-=--=-:: ::--.:==: .*-*=::::::::::::::::--------------
// -----------------:::::::::::::::::::::...............%.-=:+-::::+*::-++==-::==-----=#==:-:    .   .::-::-:=+:---:.::+-#-:::::::::::::::::-------------
// ---------------:::::::::::::::::::::................+#.*--*----++-+**-==---+==+===**==. .....::-::.   :::-=-#=--==-:+*.#:::::::::::::::::::-----------
// --------------:::::::::::::::::::.................=*=+.@=-*---**==++-*=--+#++*=-+*=--:.===+#%@@@%*++=:  .-.--+==:.:-%*-%::::::::::::::::::::----------
// ------------::::::::::::::::::::..................=**--@+=%**%*==**+#+=+*+-=#=*+=-----:.=@%%@%###%@@@@#: -. -.==#===-=:+*::::::::::::::::::::---------
// -----------::::::::::::::::::::................... *@:+@#+.   :+#%#%*+=#===%#+:-:::=-:+%%#%*++*++**%@@@@=::   :-:- .=*=-**..:::::::::::::::::---------
// ----------:::::::::::::::::::...................   .%:#%:=#%#+.::-*******++=:--...:+-*=*%%*%@*:....=%@@@@*=::.== *. . .  :*-:::::::::::::::::::-------
// ---------::::::::::::::::::::.................     :#:@-+#-+%%%=.=.+ +-=--:-. .:.  :-*---#%@%+:::%-.*@@@@@@%%#+--++:.==   .-#:::::::::::::::::::------
// --------:::::::::::::::::::..................      =#-@:+ +*:*%@##-* +:  ::--::.  :-.:+**##*%*=---=*%@@@@@@@%=. ...%@@%%##:-=#::::::::::::::::::------
// -------:::::::::::::::::::.................        :%=@* :*:-+@@@%:+*=       :--=-.:::--+#@@####**#%@@@%#%#=:--- .--%@#@@%*:.#-.::::::::::::::::::----
// ------::::::::::::::::::::................          =#*#.:%:.#@@=:.%@%+:  .-           :-=+*#**###%##*+==+:--:---::-=+#*%@#-*=....::::::::::::::::----
// ------:::::::::::::::::::................           ++##: =#-+@@= .@@+@@@#-.::-##*=:-+##*=: .=+++++--:-=*+=----.=%@@@+**+%+-.......::::::::::::::::---
// ------::::::::::::::::::................            #+:%+: .*%@@@*:=@+-%%%@@#=-=--+**=:.-*@@#=::::.-*%#*+-=-+.-%@@@%@=#**-.........::::::::::::::::---
// -----::::::::::::::::::................              =#=*:+:.:=:-++=*::.==:##@@#*=------- .#%%*+::*%%+*==:=@ =@@@@%###@%+...........::::::::::::::::--
// -----::::::::::::::::::...............                -%=*.-..-:=-:-::*:- . .%@@@@%%%%+#@= .+%.+--+**+:* :+@.+@@@@#+%+*##............::::::::::::::::-
// ----::::::::::::::::::................                 =*+@%*--::..=@%@#.:.  #@@@@#%%%%:*@. +%:+-*++:.*-=..=##:-=****#=*#-...........::::::::::::::::-
// ---:::::::::::::::::::...............                   %=%*#@#++*%@@@@@%*   *@@*...=%##.%= =#**++::.=**:- :.@.-:#=+@#.: +-..........::::::::::::::::-
// ---:::::::::::::::::::...............                   =#+=:+@#+*%%%@@@@*   *@@*....#+#.#* #%#=+.: .:=+=    #  =-++%%:-+#-..........::::::::::::::::-
// ---:::::::::::::::::::...............                   .%-%:-== .+*#%@@@+   +%#@*=-+#-=.%# @*==-=%@#*@+*@*+%#+-:.:+%#%*-............::::::::::::::::-
// ---:::::::::::::::::::...............                   .%=%%-.= .-+@%@@@= . -@%*%@@#-=-=@*:*++*@#+:-@*.==.=%=#*=%=-%-................::::::::::::::::
// ----::::::::::::::::::...............                   :#=@%%.:  -*%*@@@: . :%@@*@*::: +%+:++=@@%%-*@-.+. %:.#..#+%- ................::::::::::::::::
// --::::::::::::::::::::...............                   =++%-%=:  =#+.#@%=.. .=#@@%-:.:.-%-.*.%@@@@@@@%+%+**. * :*-# ................:::::::::::::::::
// ---:::::::::::::::::::...............               .:-=%-%:-%#-  *** +@#:    .+@@*=::.:-# +--@@@@@@@@#@@@@%**#--#-*.................:::::::::::::::::
// ---:::::::::::::::::::................           :#++==#+%-.=+%:..%@@.#*   :  :-.=*##+---# *:+@+%=#%*==#--=#+@@@@%#:.................:::::::::::::::--
// ---:::::::::::::::::::................          -#..*%#%#-.-:%@.=:*%%=@. .  :-. --..-+##@#-#-=*-::..#+===#*:.*####-  ...............::::::::::::::::--
// ----:::::::::::::::::::...............         +#..-*@@#..--%%@.+:#=*+*#  -:  -=-:=-====+*%#+:**+++=*+:-+=#=+*+*:-#.................:::::::::::::::---
// -----::::::::::::::::::................      .*+  ---** :+*%@*@.+-@:=*.@#  =:   .-+. ::+#%+-=-+-##=. .*-.:*: ::*%#:.................:::::::::::::::---
// ----:::::::::::::::::::.................    :#= :-..+-*+%@#@*-@:-%@*-% *@%. =.:::.:-. :::.-+*=-+..=#:-: .*=++**=-*=................::::::::::::::::---
// -----:::::::::::::::::::.................  -#: .=.   -=+@@@==.@::***=@ .%@@+-%*+*#*+=+*+*=..--++-- -%-*.+-=+*-=**=................::::::::::::::::----
// ------:::::::::::::::::::.................+%.   +     .+=@+.-=@::++-*@. =@@@@@@@@@@@#-. ...::-. -.- =* +=:.+:. %..................::::::::::::::::----
// ------::::::::::::::::::::..............=#%.    -.      +-%=#-@.:*.:*%:..@@@@%#%@@@@@@@@%*--=+*-    :. :.  -   +=................::::::::::::::::-----
// -------::::::::::::::::::::........:=++*+@@: .. .        -=*@+# -+ .##-. @@#%*#*-#@@@@@@@@@@#*##-::      .-. .:.%...............:::::::::::::::::-----
// --------::::::::::::::::::::...:=**##=-. +@@: .:          :+=@- *=  **+: %@:=#.-#@@@%@@#+=#@@@@@@%**=:..-+#@@@=:#...............:::::::::::::::::-----
// --------::::::::::::::::::::-+**+-:  +=*+-=@@= ::           +=+-@:  +=#- ##++:#@%*+**-.-*@%#++#%+=-=++++****#*+#:.............::::::::::::::::::------
// ----------::::::::::::::=+***-:.   .=---.  =@@* .-           :=+%.  *-%-.=@::%%+:**: :##*-:-*#-        ......:+..............::::::::::::::::::-------
// -----------::::::::-=*#**--: .::::::     :--#@@#..:            ==*- *-#==.@#*#-*+.  =++-..*@##=        .....................:::::::::::::::::::-------
// ------------::-=+***+==--:-:-=--.     :--.   #@@%..-            .==*%+-#= #%==%%. :#%*:.:%@%+-*+      ....................::::::::::::::::::::--------
// ------------=**+==**=-:::+--:      :--:=:    -@@@%:.-             -=+%:#=--%#@%-.*@@%- =@@*. * +#.   ....................::::::::::::::::::::---------
// -------------::::......         .. :+...--   .#:%@@-.-              ==*%*+ %%- :%@@+ .#@@=   =.--#:....................::::::::::::::::::::::---------
// ---------------:::::......   .::::.  +-. :=. :* .%@@.=               .=+## =%=#%@%: -@@@#    :- =:%*-................:::::::::::::::::::::::----------
// ----------------::::=--:::::::::::::. -+.  :-:=: *@@=-.                :=*+:@@%#*.--@@@@@    .=  -:#+++=............::::::::::::::::::::::------------
// ------------------::::---::::::::::::. .+::. =-* :@@#.=                  :=*@@*+-+:@@@@@@+    =   -.#*==+*+:......:::::::::::::::::::::::-------------
// --------------------:::::::::::::::::::..+-.  .++ %@@ +                   :-+%#+*-%@@*@@@@-   +   .: **==:=##+-.:::::::::::::::::::::::---------------
// ---------------------::::::::::::::::::::.-=:.  *=+@@:=.               :-::=+=+%+#@@#=@=%@@:  %-   .  +=.-: .:=++==:::::::::::::::::::----------------
// -----------------------:::::::::::::::::::::=:::.+*@@- =           .--::+%@%*@%.-#@@--%+*@@%  %+    :-:%:..:    ..-#::::::::::::::::------------------
// -------------------------::::::::::::::::::::==:::#@@- +        .--::+%@@*-  *@..:.-**##=@@@: #+ -#@@@*#%-+=--.. .:=%=---::::::::::-------------------
// ---------------------------::::::::::::::::::::=:=@@@= =     .--:-+%@@%+.    =@   .: =@**%@@* #+-@@@@@+#=#=.  ::::::::::---:::::----------------------
// -----------------------------:::::::::::::::::. ===@@+ :-.-+*-=*@@%%@@=. ...-*#::    *#=@@@@+ **-@@@@@-#--+. :::::::::::::::::------------------------
// -------------------------------:::::::::::::::::.:==@@##@@@@@@@@@%-.+#@%+. --+#-.  .:%+%@*=.:--=-@@@@@#@=--.:::::::::::::::::-------------------------
// ----------------------------------:::::::::::::::::+=#@@@@@@@#==*@@#=.:+%@*=**+.*=  *%+-  ::  -+@@@@@++@:=:::::::::::::::-----------------------------
// ------------------------------------::::::::::::::::-:=%@@**=.-:..@%@%+.-#@@@@*. .: %==-.  :+%@@@@@#-::#.:::::::::::::--------------------------------
// ---------------------------------------:::::::::::::::::=-::-+:  :#=@@@*-=-+#%* ..  @  .-+%@@%%@@%=::::-::::::::::------------------------------------
// --------------------------------------------::::::::::::::::::-::.-:*@%@@#=::-# ::..% .+%%+-##@@*-*:::::::::::::--------------------------------------
// ----------------------------------------------::::::::::::::::::::::::::=#@%+:*:::::+=#*-:-:--:-:::::::::::-------------------------------------------
// --------------------------------------------------::::::::::::::::::::::::::::-::::::::::::::::::::::-------------------------------------------------
// --------------------------------------------------------::::::::::::::::::::::::::::::::::::::::------------------------------------------------------
// ----------------------------------------------------------------::::::::::::::::::::::::--------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------

	[NonSerialized()]
	ref map<string, ref InediaInfectedAI_ConfigZombie> m_CachedZombiesConfigs = new map<string, ref InediaInfectedAI_ConfigZombie>;
    InediaInfectedAI_ConfigZombie GetConfigZombie(EntityAI creature)
    {
		string creatureClassId = creature.GetType();
		
		if (m_CachedZombiesConfigs.Contains(creatureClassId)) {
			return m_CachedZombiesConfigs[creatureClassId];
		}
		
        InediaInfectedAI_ConfigZombie configZombie = new InediaInfectedAI_ConfigZombie();
		

        // Breaking door handler
        configZombie.BreakingDoorsHandlerIsActive = GetBoolPropertyForCreature(BreakingDoorsHandlerIsActive, creature);
		configZombie.BreakingDoorsRestrictedDoors = GetBreakingDoorsRestrictedDoorsPropertyForCreature(creature);
        configZombie.BreakingDoorsDestroyAfterOpenChancePercent = GetFloatPropertyForCreature(BreakingDoorsDestroyAfterOpenChancePercent, creature);
        configZombie.BreakingDoorsOpenChancePercent = GetFloatPropertyForCreature(BreakingDoorsOpenChancePercent, creature);
        configZombie.BreakingDoorsOpenLockPickChancePercent = GetFloatPropertyForCreature(BreakingDoorsOpenLockPickChancePercent, creature);
        configZombie.BreakingDoorsLossInterestAfterHitChancePercent = GetFloatPropertyForCreature(BreakingDoorsLossInterestAfterHitChancePercent, creature);
        configZombie.BreakingDoorsLossInterestAfterHitLockPickChancePercent = GetFloatPropertyForCreature(BreakingDoorsLossInterestAfterHitLockPickChancePercent, creature);
        configZombie.BreakingDoorsCrawlersChanceMultiplier = GetFloatPropertyForCreature(BreakingDoorsCrawlersChanceMultiplier, creature);

        // Attack unconscious
        configZombie.AttackPlayersUnconsciousHandlerIsActive = GetBoolPropertyForCreature(AttackPlayersUnconsciousHandlerIsActive, creature);
        configZombie.AttackPlayersUnconsciousHealthDamageLimit = GetFloatPropertyForCreature(AttackPlayersUnconsciousHealthDamageLimit, creature);

        // Attack animals
        configZombie.AttackAnimalsHandlerIsActive = GetBoolPropertyForCreature(AttackAnimalsHandlerIsActive, creature);
        //configZombie.AttackAnimalsWolfs = GetBoolPropertyForCreature(AttackAnimalsWolfs, creature);
        //configZombie.AttackAnimalsBears = GetBoolPropertyForCreature(AttackAnimalsBears, creature);
        configZombie.AttackAnimalsWildBoar = GetBoolPropertyForCreature(AttackAnimalsWildBoar, creature);
        configZombie.AttackAnimalsPig = GetBoolPropertyForCreature(AttackAnimalsPig, creature);
        configZombie.AttackAnimalsSheep = GetBoolPropertyForCreature(AttackAnimalsSheep, creature);
        //configZombie.AttackAnimalsChicken = GetBoolPropertyForCreature(AttackAnimalsChicken, creature);
        configZombie.AttackAnimalsDeer = GetBoolPropertyForCreature(AttackAnimalsDeer, creature);
        configZombie.AttackAnimalsRoeDeer = GetBoolPropertyForCreature(AttackAnimalsRoeDeer, creature);
        configZombie.AttackAnimalsGoat = GetBoolPropertyForCreature(AttackAnimalsGoat, creature);
        configZombie.AttackAnimalsCow = GetBoolPropertyForCreature(AttackAnimalsCow, creature);
        configZombie.AttackAnimalsCustom = GetMapBoolPropertyForCreature(AttackAnimalsCustom, creature);

        // Friendly NPC
        configZombie.FriendlyNPC = GetArrayStringPropertyForCreature(FriendlyNPC, creature);

        // Search handler
        configZombie.SearchHandlerIsActive = GetBoolPropertyForCreature(SearchHandlerIsActive, creature);
        configZombie.SearchSphereRadiusIncreaseEverySeconds = GetFloatPropertyForCreature(SearchSphereRadiusIncreaseEverySeconds, creature);
        configZombie.SearchSphereDistanceToRadiusMultiplier = GetFloatPropertyForCreature(SearchSphereDistanceToRadiusMultiplier, creature);
        configZombie.SearchSphereRadiusMin = GetFloatPropertyForCreature(SearchSphereRadiusMin, creature);
        configZombie.SearchSphereRadiusMax = GetFloatPropertyForCreature(SearchSphereRadiusMax, creature);
        configZombie.SearchSphereRadiusBurst = GetFloatPropertyForCreature(SearchSphereRadiusBurst, creature);

        // Speed handler
        configZombie.SpeedHandlerIsActive = GetBoolPropertyForCreature(SpeedHandlerIsActive, creature);
		configZombie.SpeedMinimumInCalmMode = GetFloatPropertyForCreature(SpeedMinimumInCalmMode, creature);
        configZombie.SpeedLimitInCalmMode = GetFloatPropertyForCreature(SpeedLimitInCalmMode, creature);
        configZombie.SpeedMultiplierInCalmMode = GetFloatPropertyForCreature(SpeedMultiplierInCalmMode, creature);
        configZombie.SpeedMinimumInSearchMode = GetFloatPropertyForCreature(SpeedMinimumInSearchMode, creature);
        configZombie.SpeedLimitInSearchMode = GetFloatPropertyForCreature(SpeedLimitInSearchMode, creature);
        configZombie.SpeedMultiplierInSearchMode = GetFloatPropertyForCreature(SpeedMultiplierInSearchMode, creature);
        configZombie.SpeedMinimumInChaseMode = GetFloatPropertyForCreature(SpeedMinimumInChaseMode, creature);
        configZombie.SpeedLimitInChaseMode = GetFloatPropertyForCreature(SpeedLimitInChaseMode, creature);
        configZombie.SpeedMultiplierInChaseMode = GetFloatPropertyForCreature(SpeedMultiplierInChaseMode, creature);
        configZombie.SpeedNoLimitsFromDistanceMeters = GetFloatPropertyForCreature(SpeedNoLimitsFromDistanceMeters, creature);

        // Smells handler
        configZombie.SmellsHandlerIsActive = GetBoolPropertyForCreature(SmellsHandlerIsActive, creature);
        configZombie.SmellsSphereRadiusInCalmMode = GetFloatPropertyForCreature(SmellsSphereRadiusInCalmMode, creature);
        configZombie.SmellsSphereRadiusInSearchMode = GetFloatPropertyForCreature(SmellsSphereRadiusInSearchMode, creature);
        configZombie.SmellsChaseMultiplier = GetFloatPropertyForCreature(SmellsChaseMultiplier, creature);
        configZombie.SmellsAllowStealthKills = GetBoolPropertyForCreature(SmellsAllowStealthKills, creature);
        configZombie.SmellsLossInterestAfterReactionForSeconds = GetFloatPropertyForCreature(SmellsLossInterestAfterReactionForSeconds, creature);
        configZombie.SmellsDisableVanillaIrritationSound = GetBoolPropertyForCreature(SmellsDisableVanillaIrritationSound, creature);
		
		// Long vision handler
		configZombie.LongVisionHandlerIsActive = GetBoolPropertyForCreature(LongVisionHandlerIsActive, creature);
		configZombie.LongVisionMetersInSearchModeDay = GetFloatPropertyForCreature(LongVisionMetersInSearchModeDay, creature);
		configZombie.LongVisionMetersInSearchModeNight = GetFloatPropertyForCreature(LongVisionMetersInSearchModeNight, creature);
		configZombie.LongVisionMetersInCalmModeDay = GetFloatPropertyForCreature(LongVisionMetersInCalmModeDay, creature);
		configZombie.LongVisionMetersInCalmModeNight = GetFloatPropertyForCreature(LongVisionMetersInCalmModeNight, creature);
		configZombie.LongVisionVisualAcuityInSearchModeDay = GetFloatPropertyForCreature(LongVisionVisualAcuityInSearchModeDay, creature);
		configZombie.LongVisionVisualAcuityInSearchModeNight = GetFloatPropertyForCreature(LongVisionVisualAcuityInSearchModeNight, creature);
		configZombie.LongVisionVisualAcuityInCalmModeDay = GetFloatPropertyForCreature(LongVisionVisualAcuityInCalmModeDay, creature);
		configZombie.LongVisionVisualAcuityInCalmModeNight = GetFloatPropertyForCreature(LongVisionVisualAcuityInCalmModeNight, creature);
		configZombie.LongVisionPersistAfterIrritationSeconds = GetFloatPropertyForCreature(LongVisionPersistAfterIrritationSeconds, creature);
		configZombie.LongVisionAllowIgnoreWeather = GetBoolPropertyForCreature(LongVisionAllowIgnoreWeather, creature);
		
		// Return to spawn handler
		configZombie.ReturnToSpawnHandlerIsActive = GetBoolPropertyForCreature(ReturnToSpawnHandlerIsActive, creature);
		configZombie.ReturnToSpawnRadiusMeters = GetFloatPropertyForCreature(ReturnToSpawnRadiusMeters, creature);

        // Up jump handler
        configZombie.UpJumpHandlerIsActive = GetBoolPropertyForCreature(UpJumpHandlerIsActive, creature);
        configZombie.UpJumpHeightMax = GetFloatPropertyForCreature(UpJumpHeightMax, creature);
        configZombie.UpJumpImpulseDamageChancePercent = GetFloatPropertyForCreature(UpJumpImpulseDamageChancePercent, creature);
        configZombie.UpJumpChancePercent = GetFloatPropertyForCreature(UpJumpChancePercent, creature);
        configZombie.UpJumpCooldownSeconds = GetFloatPropertyForCreature(UpJumpCooldownSeconds, creature);

        // Down jump handler
        configZombie.DownJumpHandlerIsActive = GetBoolPropertyForCreature(DownJumpHandlerIsActive, creature);
        configZombie.DownJumpHeightMax = GetFloatPropertyForCreature(DownJumpHeightMax, creature);
		
		// Stuck jump handler
		configZombie.StuckJumpHandlerIsActive = GetBoolPropertyForCreature(StuckJumpHandlerIsActive, creature);

        // Fall handler
        configZombie.FallHandlerIsActive = GetBoolPropertyForCreature(FallHandlerIsActive, creature);
        configZombie.FallHandlerLegBreakHeightMin = GetFloatPropertyForCreature(FallHandlerLegBreakHeightMin, creature);
        configZombie.FallHandlerLegBreakHeightMax = GetFloatPropertyForCreature(FallHandlerLegBreakHeightMax, creature);
        configZombie.FallHandlerDeathHeightMin = GetFloatPropertyForCreature(FallHandlerDeathHeightMin, creature);
        configZombie.FallHandlerDeathHeightMax = GetFloatPropertyForCreature(FallHandlerDeathHeightMax, creature);

        // Injury handler
        configZombie.InjuryHandlerIsActive = GetBoolPropertyForCreature(InjuryHandlerIsActive, creature);
        configZombie.InjuryHandlerInjuryOnExplosiveDamageChancePercent = GetFloatPropertyForCreature(InjuryHandlerInjuryOnExplosiveDamageChancePercent, creature);
        configZombie.InjuryHandlerDamageThresholdToInjuryArm = GetFloatPropertyForCreature(InjuryHandlerDamageThresholdToInjuryArm, creature);
        configZombie.InjuryHandlerRestoreInjuredArmAfterSeconds = GetFloatPropertyForCreature(InjuryHandlerRestoreInjuredArmAfterSeconds, creature);
        configZombie.InjuryHandlerInjuredOneArmAttackMultiplier = GetFloatPropertyForCreature(InjuryHandlerInjuredOneArmAttackMultiplier, creature);
        configZombie.InjuryHandlerInjuredBothArmsAttackMultiplier = GetFloatPropertyForCreature(InjuryHandlerInjuredBothArmsAttackMultiplier, creature);
        configZombie.InjuryHandlerInjuredOneArmBreakingDoorChanceMultiplier = GetFloatPropertyForCreature(InjuryHandlerInjuredOneArmBreakingDoorChanceMultiplier, creature);
        configZombie.InjuryHandlerInjuredBothArmsBreakingDoorChanceMultiplier = GetFloatPropertyForCreature(InjuryHandlerInjuredBothArmsBreakingDoorChanceMultiplier, creature);
        configZombie.InjuryHandlerInjuredOneArmStunChanceMultiplier = GetFloatPropertyForCreature(InjuryHandlerInjuredOneArmStunChanceMultiplier, creature);
        configZombie.InjuryHandlerInjuredBothArmsStunChanceMultiplier = GetFloatPropertyForCreature(InjuryHandlerInjuredBothArmsStunChanceMultiplier, creature);
        configZombie.InjuryHandlerInjuredOneArmPainMultiplier = GetFloatPropertyForCreature(InjuryHandlerInjuredOneArmPainMultiplier, creature);
        configZombie.InjuryHandlerInjuredBothArmsPainMultiplier = GetFloatPropertyForCreature(InjuryHandlerInjuredBothArmsPainMultiplier, creature);
        configZombie.InjuryHandlerInjuredOneArmBleedingChanceMultiplier = GetFloatPropertyForCreature(InjuryHandlerInjuredOneArmBleedingChanceMultiplier, creature);
        configZombie.InjuryHandlerInjuredBothArmsBleedingChanceMultiplier = GetFloatPropertyForCreature(InjuryHandlerInjuredBothArmsBleedingChanceMultiplier, creature);
        configZombie.InjuryHandlerSpawnWithInjuredOneArmChancePercent = GetFloatPropertyForCreature(InjuryHandlerSpawnWithInjuredOneArmChancePercent, creature);
        configZombie.InjuryHandlerSpawnWithInjuredBothArmsChancePercent = GetFloatPropertyForCreature(InjuryHandlerSpawnWithInjuredBothArmsChancePercent, creature);
        configZombie.InjuryHandlerDamageThresholdToInjuryLeg = GetFloatPropertyForCreature(InjuryHandlerDamageThresholdToInjuryLeg, creature);
        configZombie.InjuryHandlerRestoreInjuredLegAfterSeconds = GetFloatPropertyForCreature(InjuryHandlerRestoreInjuredLegAfterSeconds, creature);
        configZombie.InjuryHandlerInjuredOneLegSpeedLimit = GetFloatPropertyForCreature(InjuryHandlerInjuredOneLegSpeedLimit, creature);
        configZombie.InjuryHandlerInjuredBothLegsSpeedLimit = GetFloatPropertyForCreature(InjuryHandlerInjuredBothLegsSpeedLimit, creature);
        configZombie.InjuryHandlerInjuredOneLegJumpHeightMultiplier = GetFloatPropertyForCreature(InjuryHandlerInjuredOneLegJumpHeightMultiplier, creature);
        configZombie.InjuryHandlerInjuredBothLegsJumpHeightMultiplier = GetFloatPropertyForCreature(InjuryHandlerInjuredBothLegsJumpHeightMultiplier, creature);
        configZombie.InjuryHandlerSpawnWithInjuredOneLegChancePercent = GetFloatPropertyForCreature(InjuryHandlerSpawnWithInjuredOneLegChancePercent, creature);
        configZombie.InjuryHandlerSpawnWithInjuredBothLegsChancePercent = GetFloatPropertyForCreature(InjuryHandlerSpawnWithInjuredBothLegsChancePercent, creature);

        // Car attack handler
        configZombie.AttackCarHandlerIsActive = GetBoolPropertyForCreature(AttackCarHandlerIsActive, creature);
        configZombie.AttackCarPlayersIsActive = GetBoolPropertyForCreature(AttackCarPlayersIsActive, creature);
        configZombie.AttackCarPlayersDistanceMeters = GetFloatPropertyForCreature(AttackCarPlayersDistanceMeters, creature);
        configZombie.AttackCarElementsIsActive = GetBoolPropertyForCreature(AttackCarElementsIsActive, creature);
        configZombie.AttackCarElementsDistanceMeters = GetFloatPropertyForCreature(AttackCarElementsDistanceMeters, creature);
        configZombie.AttackCarElementsGlobalDamageMultiplier = GetFloatPropertyForCreature(AttackCarElementsGlobalDamageMultiplier, creature);
        configZombie.AttackCarElementsByCollisionsIsActive = GetBoolPropertyForCreature(AttackCarElementsByCollisionsIsActive, creature);
        configZombie.AttackCarElementsByCollisionsGlobalDamageMultiplier = GetFloatPropertyForCreature(AttackCarElementsByCollisionsGlobalDamageMultiplier, creature);
        configZombie.AttackCarElementsMultiplierByCarClassId = GetMapFloatPropertyForCreature(AttackCarElementsMultiplierByCarClassId, creature);
        configZombie.AttackCarWindowDamagePercent = GetFloatPropertyForCreature(AttackCarWindowDamagePercent, creature);
        configZombie.AttackCarDoorDamagePercent = GetFloatPropertyForCreature(AttackCarDoorDamagePercent, creature);
        configZombie.AttackCarDoorChancePercent = GetFloatPropertyForCreature(AttackCarDoorChancePercent, creature);
        configZombie.AttackCarRadiatorDamagePercent = GetFloatPropertyForCreature(AttackCarRadiatorDamagePercent, creature);
        configZombie.AttackCarCarBatteryDamagePercent = GetFloatPropertyForCreature(AttackCarCarBatteryDamagePercent, creature);
        configZombie.AttackCarPlugDamagePercent = GetFloatPropertyForCreature(AttackCarPlugDamagePercent, creature);
        configZombie.AttackCarEngineDamagePercent = GetFloatPropertyForCreature(AttackCarEngineDamagePercent, creature);
        configZombie.AttackCarLightsDamagePercent = GetFloatPropertyForCreature(AttackCarLightsDamagePercent, creature);
        configZombie.AttackCarFenderDamagePercent = GetFloatPropertyForCreature(AttackCarFenderDamagePercent, creature);
        configZombie.AttackCarBumperDamagePercent = GetFloatPropertyForCreature(AttackCarBumperDamagePercent, creature);
        configZombie.AttackCarHoodDamagePercent = GetFloatPropertyForCreature(AttackCarHoodDamagePercent, creature);
        configZombie.AttackCarFuelTankDamagePercent = GetFloatPropertyForCreature(AttackCarFuelTankDamagePercent, creature);

        // React handler
        configZombie.ReactHandlerIsActive = GetBoolPropertyForCreature(ReactHandlerIsActive, creature);

        configZombie.ReactBehindVisualMultiplier = GetFloatPropertyForCreature(ReactBehindVisualMultiplier, creature);
        configZombie.ReactBehindNoiseMultiplier = GetFloatPropertyForCreature(ReactBehindNoiseMultiplier, creature);
        configZombie.ReactFireplaceVisual = GetFloatPropertyForCreature(ReactFireplaceVisual, creature);
        configZombie.ReactFireplaceDayVisual = GetFloatPropertyForCreature(ReactFireplaceDayVisual, creature);
        configZombie.ReactFlashlightsVisual = GetFloatPropertyForCreature(ReactFlashlightsVisual, creature);
        configZombie.ReactHeadtorchRedVisual = GetFloatPropertyForCreature(ReactHeadtorchRedVisual, creature);
        configZombie.ReactRoadflareVisual = GetFloatPropertyForCreature(ReactRoadflareVisual, creature);
        configZombie.ReactRoadflareNoise = GetFloatPropertyForCreature(ReactRoadflareNoise, creature);
        configZombie.ReactChemlightVisual = GetFloatPropertyForCreature(ReactChemlightVisual, creature);
        configZombie.ReactChemlightRedVisual = GetFloatPropertyForCreature(ReactChemlightRedVisual, creature);
        configZombie.ReactPortableGasLampVisual = GetFloatPropertyForCreature(ReactPortableGasLampVisual, creature);
        configZombie.ReactFlammablesVisual = GetFloatPropertyForCreature(ReactFlammablesVisual, creature);
        configZombie.ReactSpotlightVisual = GetFloatPropertyForCreature(ReactSpotlightVisual, creature);
        configZombie.ReactCarLightVisual = GetFloatPropertyForCreature(ReactCarLightVisual, creature);
        configZombie.ReactPowerGeneratorNoise = GetFloatPropertyForCreature(ReactPowerGeneratorNoise, creature);
        configZombie.ReactCarHornNoise = GetFloatPropertyForCreature(ReactCarHornNoise, creature);
        configZombie.ReactAlarmClockNoise = GetFloatPropertyForCreature(ReactAlarmClockNoise, creature);
        configZombie.ReactKitchenTimerNoise = GetFloatPropertyForCreature(ReactKitchenTimerNoise, creature);
        configZombie.ReactFuelStationNoise = GetFloatPropertyForCreature(ReactFuelStationNoise, creature);
        configZombie.ReactFireworksNoise = GetFloatPropertyForCreature(ReactFireworksNoise, creature);
        configZombie.ReactExplosiveItemNoise = GetFloatPropertyForCreature(ReactExplosiveItemNoise, creature);
        configZombie.ReactCarEngineNoise = GetFloatPropertyForCreature(ReactCarEngineNoise, creature);
        configZombie.ReactSmokeGrenadeVisual = GetFloatPropertyForCreature(ReactSmokeGrenadeVisual, creature);
        configZombie.ReactScreamNoise = GetFloatPropertyForCreature(ReactScreamNoise, creature);
        configZombie.ReactWeaponShotNoise = GetFloatPropertyForCreature(ReactWeaponShotNoise, creature);
        configZombie.ReactFriendlyWeaponShotNoise = GetFloatPropertyForCreature(ReactFriendlyWeaponShotNoise, creature);
        configZombie.ReactFootstepsJogDayNoise = GetFloatPropertyForCreature(ReactFootstepsJogDayNoise, creature);
        configZombie.ReactFootstepsJogNightNoise = GetFloatPropertyForCreature(ReactFootstepsJogNightNoise, creature);
        configZombie.ReactFootstepsCrouchSprintDayNoise = GetFloatPropertyForCreature(ReactFootstepsCrouchSprintDayNoise, creature);
        configZombie.ReactFootstepsCrouchSprintNightNoise = GetFloatPropertyForCreature(ReactFootstepsCrouchSprintNightNoise, creature);
        configZombie.ReactFootstepsSprintDayNoise = GetFloatPropertyForCreature(ReactFootstepsSprintDayNoise, creature);
        configZombie.ReactFootstepsSprintNightNoise = GetFloatPropertyForCreature(ReactFootstepsSprintNightNoise, creature);
        configZombie.ReactHorseHoofstepsDayNoise = GetFloatPropertyForCreature(ReactHorseHoofstepsDayNoise, creature);
        configZombie.ReactHorseHoofstepsNightNoise = GetFloatPropertyForCreature(ReactHorseHoofstepsNightNoise, creature);
        configZombie.ReactDoorsDayNoise = GetFloatPropertyForCreature(ReactDoorsDayNoise, creature);
        configZombie.ReactDoorsNightNoise = GetFloatPropertyForCreature(ReactDoorsNightNoise, creature);
        configZombie.ReactPlayerFallDayNoise = GetFloatPropertyForCreature(ReactPlayerFallDayNoise, creature);
        configZombie.ReactPlayerFallNightNoise = GetFloatPropertyForCreature(ReactPlayerFallNightNoise, creature);
        configZombie.ReactBodyfallDayNoise = GetFloatPropertyForCreature(ReactBodyfallDayNoise, creature);
        configZombie.ReactBodyfallNightNoise = GetFloatPropertyForCreature(ReactBodyfallNightNoise, creature);
        configZombie.ReactBodyfallBackstabDayNoise = GetFloatPropertyForCreature(ReactBodyfallBackstabDayNoise, creature);
        configZombie.ReactBodyfallBackstabNightNoise = GetFloatPropertyForCreature(ReactBodyfallBackstabNightNoise, creature);
        configZombie.ReactSymptomsDayNoise = GetFloatPropertyForCreature(ReactSymptomsDayNoise, creature);
        configZombie.ReactSymptomsNightNoise = GetFloatPropertyForCreature(ReactSymptomsNightNoise, creature);
        configZombie.ReactVoiceWhisperDayNoise = GetFloatPropertyForCreature(ReactVoiceWhisperDayNoise, creature);
        configZombie.ReactVoiceWhisperNightNoise = GetFloatPropertyForCreature(ReactVoiceWhisperNightNoise, creature);
        configZombie.ReactVoiceTalkDayNoise = GetFloatPropertyForCreature(ReactVoiceTalkDayNoise, creature);
        configZombie.ReactVoiceTalkNightNoise = GetFloatPropertyForCreature(ReactVoiceTalkNightNoise, creature);
        configZombie.ReactVoiceShoutDayNoise = GetFloatPropertyForCreature(ReactVoiceShoutDayNoise, creature);
        configZombie.ReactVoiceShoutNightNoise = GetFloatPropertyForCreature(ReactVoiceShoutNightNoise, creature);
        configZombie.ReactMiningLightDayNoise = GetFloatPropertyForCreature(ReactMiningLightDayNoise, creature);
        configZombie.ReactMiningLightNightNoise = GetFloatPropertyForCreature(ReactMiningLightNightNoise, creature);
        configZombie.ReactMiningHardDayNoise = GetFloatPropertyForCreature(ReactMiningHardDayNoise, creature);
        configZombie.ReactMiningHardNightNoise = GetFloatPropertyForCreature(ReactMiningHardNightNoise, creature);
        configZombie.ReactBuildingDayNoise = GetFloatPropertyForCreature(ReactBuildingDayNoise, creature);
        configZombie.ReactBuildingNightNoise = GetFloatPropertyForCreature(ReactBuildingNightNoise, creature);
        configZombie.ReactSawPlanksDayNoise = GetFloatPropertyForCreature(ReactSawPlanksDayNoise, creature);
        configZombie.ReactSawPlanksNightNoise = GetFloatPropertyForCreature(ReactSawPlanksNightNoise, creature);
        configZombie.ReactVanillaMindstateChange = GetFloatPropertyForCreature(ReactVanillaMindstateChange, creature);
		
        configZombie.ReactCustomIrritants = GetReactCustomIrritantsPropertyForCreature(creature);

        configZombie.ReactAndDestroyFlashlightsVisual = GetFloatPropertyForCreature(ReactAndDestroyFlashlightsVisual, creature);
        configZombie.ReactAndDestroyHeadtorchRedVisual = GetFloatPropertyForCreature(ReactAndDestroyHeadtorchRedVisual, creature);
        configZombie.ReactAndDestroyRoadflareVisual = GetFloatPropertyForCreature(ReactAndDestroyRoadflareVisual, creature);
        configZombie.ReactAndDestroyRoadflareNoise = GetFloatPropertyForCreature(ReactAndDestroyRoadflareNoise, creature);
        configZombie.ReactAndDestroyChemlightVisual = GetFloatPropertyForCreature(ReactAndDestroyChemlightVisual, creature);
        configZombie.ReactAndDestroyChemlightRedVisual = GetFloatPropertyForCreature(ReactAndDestroyChemlightRedVisual, creature);
        configZombie.ReactAndDestroyPortableGasLampVisual = GetFloatPropertyForCreature(ReactAndDestroyPortableGasLampVisual, creature);
        configZombie.ReactAndDestroyFlammablesVisual = GetFloatPropertyForCreature(ReactAndDestroyFlammablesVisual, creature);
        configZombie.ReactAndDestroySpotlightVisual = GetFloatPropertyForCreature(ReactAndDestroySpotlightVisual, creature);
        configZombie.ReactAndDestroyFireplaceVisual = GetFloatPropertyForCreature(ReactAndDestroyFireplaceVisual, creature);
        configZombie.ReactAndDestroyFireplaceDayVisual = GetFloatPropertyForCreature(ReactAndDestroyFireplaceDayVisual, creature);
        configZombie.ReactAndDestroyCarLightVisual = GetFloatPropertyForCreature(ReactAndDestroyCarLightVisual, creature);
        configZombie.ReactAndDestroyFireworksNoise = GetFloatPropertyForCreature(ReactAndDestroyFireworksNoise, creature);
        configZombie.ReactAndDestroyPowerGeneratorNoise = GetFloatPropertyForCreature(ReactAndDestroyPowerGeneratorNoise, creature);
        configZombie.ReactAndDestroyAlarmClockNoise = GetFloatPropertyForCreature(ReactAndDestroyAlarmClockNoise, creature);
        configZombie.ReactAndDestroyKitchenTimerNoise = GetFloatPropertyForCreature(ReactAndDestroyKitchenTimerNoise, creature);
        configZombie.ReactAndDestroySmokeGrenadeVisual = GetFloatPropertyForCreature(ReactAndDestroySmokeGrenadeVisual, creature);

        // Attack system
        configZombie.DamageToPlayerHandlerIsActive = GetBoolPropertyForCreature(DamageToPlayerHandlerIsActive, creature);
        configZombie.DamageToPlayerHealthMultiplier = GetFloatPropertyForCreature(DamageToPlayerHealthMultiplier, creature);
		configZombie.DamageToPlayerInBlockHealthMultiplier = GetFloatPropertyForCreature(DamageToPlayerInBlockHealthMultiplier, creature);
        configZombie.DamageToPlayerShockMultiplier = GetFloatPropertyForCreature(DamageToPlayerShockMultiplier, creature);
		configZombie.DamageToPlayerInBlockShockMultiplier = GetFloatPropertyForCreature(DamageToPlayerInBlockShockMultiplier, creature);
        configZombie.DamageToPlayerStaminaPercent = GetFloatPropertyForCreature(DamageToPlayerStaminaPercent, creature);
		configZombie.DamageToPlayerInBlockStaminaPercent = GetFloatPropertyForCreature(DamageToPlayerInBlockStaminaPercent, creature);
        configZombie.DamageToPlayerBloodPercent = GetFloatPropertyForCreature(DamageToPlayerBloodPercent, creature);
		configZombie.DamageToPlayerInBlockBloodPercent = GetFloatPropertyForCreature(DamageToPlayerInBlockBloodPercent, creature);
        configZombie.DamageToPlayerBleedingHandlerIsActive = GetBoolPropertyForCreature(DamageToPlayerBleedingHandlerIsActive, creature);
        configZombie.DamageToPlayerBleedingChancePercent = GetFloatPropertyForCreature(DamageToPlayerBleedingChancePercent, creature);
		configZombie.DamageToPlayerInBlockBleedingChancePercent = GetFloatPropertyForCreature(DamageToPlayerInBlockBleedingChancePercent, creature);

        // Stun system
        configZombie.StunToPlayerChancePercent = GetFloatPropertyForCreature(StunToPlayerChancePercent, creature);
        configZombie.StunToPlayerInBlockChancePercent = GetFloatPropertyForCreature(StunToPlayerInBlockChancePercent, creature);
		
		// Disarm system
		configZombie.DisarmToPlayerChancePercent = GetFloatPropertyForCreature(DisarmToPlayerChancePercent, creature);
        configZombie.DisarmToPlayerInBlockChancePercent = GetFloatPropertyForCreature(DisarmToPlayerInBlockChancePercent, creature);
		
		// Fast attack
		configZombie.FastAttackToPlayerChancePercent = GetFloatPropertyForCreature(FastAttackToPlayerChancePercent, creature);
		
		// Deseases (transmission of agents during attacks by infected)
		configZombie.DiseasesToPlayerHandlerIsActive = GetBoolPropertyForCreature(DiseasesToPlayerHandlerIsActive, creature);
		configZombie.DiseasesToPlayerAgents = GetDiseasesToPlayerAgentsPropertyForCreature(creature);
		
        // Camera shake system
        configZombie.CameraShakeToPlayerIntensity = GetFloatPropertyForCreature(CameraShakeToPlayerIntensity, creature);

        // Pain system
        configZombie.PainToPlayerHandlerIsActive = GetBoolPropertyForCreature(PainToPlayerHandlerIsActive, creature);
        configZombie.PainToPlayerHeadMultiplier = GetFloatPropertyForCreature(PainToPlayerHeadMultiplier, creature);
        configZombie.PainToPlayerHeadInBlockMultiplier = GetFloatPropertyForCreature(PainToPlayerHeadInBlockMultiplier, creature);
        configZombie.PainToPlayerArmsMultiplier = GetFloatPropertyForCreature(PainToPlayerArmsMultiplier, creature);
        configZombie.PainToPlayerArmsInBlockMultiplier = GetFloatPropertyForCreature(PainToPlayerArmsInBlockMultiplier, creature);
        configZombie.PainToPlayerArmsDisarmMultiplier = GetFloatPropertyForCreature(PainToPlayerArmsDisarmMultiplier, creature);
        configZombie.PainToPlayerArmsInBlockDisarmMultiplier = GetFloatPropertyForCreature(PainToPlayerArmsInBlockDisarmMultiplier, creature);
        configZombie.PainToPlayerLegsMultiplier = GetFloatPropertyForCreature(PainToPlayerLegsMultiplier, creature);
        configZombie.PainToPlayerLegsInBlockMultiplier = GetFloatPropertyForCreature(PainToPlayerLegsInBlockMultiplier, creature);
        configZombie.PainToPlayerTorsoMultiplier = GetFloatPropertyForCreature(PainToPlayerTorsoMultiplier, creature);
        configZombie.PainToPlayerTorsoInBlockMultiplier = GetFloatPropertyForCreature(PainToPlayerTorsoInBlockMultiplier, creature);
		
        configZombie.PainToPlayerLimbsBreakChancesMultiplier = GetFloatPropertyForCreature(PainToPlayerLimbsBreakChancesMultiplier, creature);
        configZombie.PainToPlayerLimbsDeepWoundChancesMultiplier = GetFloatPropertyForCreature(PainToPlayerLimbsDeepWoundChancesMultiplier, creature);

        // Defence system
        configZombie.DamageToZombieHandlerIsActive = GetBoolPropertyForCreature(DamageToZombieHandlerIsActive, creature);
		configZombie.DamageToZombieHealthPoints = GetFloatPropertyForCreature(DamageToZombieHealthPoints, creature);
		configZombie.DamageToZombieHealthPointsLeg = GetFloatPropertyForCreature(DamageToZombieHealthPointsLeg, creature);
        configZombie.DamageToZombieHeadMeleeMultiplier = GetFloatPropertyForCreature(DamageToZombieHeadMeleeMultiplier, creature);
        configZombie.DamageToZombieHeadRangeMultiplier = GetFloatPropertyForCreature(DamageToZombieHeadRangeMultiplier, creature);
        configZombie.DamageToZombieBodyMeleeMultiplier = GetFloatPropertyForCreature(DamageToZombieBodyMeleeMultiplier, creature);
        configZombie.DamageToZombieBodyRangeMultiplier = GetFloatPropertyForCreature(DamageToZombieBodyRangeMultiplier, creature);
        configZombie.DamageToZombieFromCarsMultiplier = GetFloatPropertyForCreature(DamageToZombieFromCarsMultiplier, creature);
        configZombie.DamageToZombieFromExplosionsMultiplier = GetFloatPropertyForCreature(DamageToZombieFromExplosionsMultiplier, creature);
        configZombie.DamageToZombieFromHotItemsHp = GetFloatPropertyForCreature(DamageToZombieFromHotItemsHp, creature);
        configZombie.DamageToZombieBodyPartsPiercingIsActive = GetBoolPropertyForCreature(DamageToZombieBodyPartsPiercingIsActive, creature);
        configZombie.DamageToZombieAlwaysKillOnBackstab = GetBoolPropertyForCreature(DamageToZombieAlwaysKillOnBackstab, creature);
        configZombie.DamageToZombieShockToStunHandlerIsActive = GetBoolPropertyForCreature(DamageToZombieShockToStunHandlerIsActive, creature);
        configZombie.DamageToZombieShockToStunLightHeavyAnimationThresholdMeleeHeavy = GetFloatPropertyForCreature(DamageToZombieShockToStunLightHeavyAnimationThresholdMeleeHeavy, creature);
        configZombie.DamageToZombieShockToStunLightHeavyAnimationThresholdRange = GetFloatPropertyForCreature(DamageToZombieShockToStunLightHeavyAnimationThresholdRange, creature);
        configZombie.DamageToZombieShockToStunThresholdMeleeHeavy = GetFloatPropertyForCreature(DamageToZombieShockToStunThresholdMeleeHeavy, creature);
        configZombie.DamageToZombieShockToStunThresholdRange = GetFloatPropertyForCreature(DamageToZombieShockToStunThresholdRange, creature);
        configZombie.DamageToZombieShockToStunFromButtstockHit = GetFloatPropertyForCreature(DamageToZombieShockToStunFromButtstockHit, creature);
        configZombie.DamageToZombieShockToStunCumulativeDamage = GetBoolPropertyForCreature(DamageToZombieShockToStunCumulativeDamage, creature);
        configZombie.DamageToZombieShockToStunCumulativeDamageTimeFrameSeconds = GetFloatPropertyForCreature(DamageToZombieShockToStunCumulativeDamageTimeFrameSeconds, creature);
        configZombie.DamageToZombieShockToStunIgnoreMeleeHitChancePercent = GetFloatPropertyForCreature(DamageToZombieShockToStunIgnoreMeleeHitChancePercent, creature);
        configZombie.DamageToZombieShockToStunIgnoreRangedHitChancePercent = GetFloatPropertyForCreature(DamageToZombieShockToStunIgnoreRangedHitChancePercent, creature);
        configZombie.DamageToZombieShockToStunImmunityAfterMeleeHitSeconds = GetFloatPropertyForCreature(DamageToZombieShockToStunImmunityAfterMeleeHitSeconds, creature);
        configZombie.DamageToZombieShockToStunImmunityAfterRangedHitSeconds = GetFloatPropertyForCreature(DamageToZombieShockToStunImmunityAfterRangedHitSeconds, creature);

        configZombie.DamageToZombieWeaponsMultipliers = GetMapFloatPropertyForCreature(DamageToZombieWeaponsMultipliers, creature);

        // Melee attacks dodge handler
        configZombie.MeleeAttacksDodgeHandlerIsActive = GetBoolPropertyForCreature(MeleeAttacksDodgeHandlerIsActive, creature);
        configZombie.MeleeAttacksDodgeChance = GetFloatPropertyForCreature(MeleeAttacksDodgeChance, creature);
        configZombie.MeleeAttacksDodgeCooldownSeconds = GetFloatPropertyForCreature(MeleeAttacksDodgeCooldownSeconds, creature);
		
		// Jump attack handler
		configZombie.JumpAttackHandlerIsActive = GetBoolPropertyForCreature(JumpAttackHandlerIsActive, creature);
        configZombie.JumpAttackChance = GetFloatPropertyForCreature(JumpAttackChance, creature);
        configZombie.JumpAttackCooldownSeconds = GetFloatPropertyForCreature(JumpAttackCooldownSeconds, creature);

        // Blood handler
        configZombie.BloodHandlerIsActive = GetBoolPropertyForCreature(BloodHandlerIsActive, creature);
        configZombie.BloodVolumeMl = GetFloatPropertyForCreature(BloodVolumeMl, creature);
        configZombie.BloodLossRateMinMl = GetFloatPropertyForCreature(BloodLossRateMinMl, creature);
        configZombie.BloodLossRateMaxMl = GetFloatPropertyForCreature(BloodLossRateMaxMl, creature);
        configZombie.BloodLossRateRegenMl = GetFloatPropertyForCreature(BloodLossRateRegenMl, creature);
        configZombie.BloodOnExplosiveDamageChancePercent = GetFloatPropertyForCreature(BloodOnExplosiveDamageChancePercent, creature);
        configZombie.BloodSplatParticlesIsActive = GetBoolPropertyForCreature(BloodSplatParticlesIsActive, creature);
        configZombie.BloodTrailParticlesIsActive = GetBoolPropertyForCreature(BloodTrailParticlesIsActive, creature);
        configZombie.BloodPoolParticlesIsActive = GetBoolPropertyForCreature(BloodPoolParticlesIsActive, creature);

        // Toxic zones
        configZombie.ResistContaminatedEffectHandlerIsActive = GetBoolPropertyForCreature(ResistContaminatedEffectHandlerIsActive, creature);
        configZombie.ResistContaminatedEffect = GetBoolPropertyForCreature(ResistContaminatedEffect, creature);

        // Crawling logic
        configZombie.AllowCrawling = GetBoolPropertyForCreature(AllowCrawling, creature);
        configZombie.RespawnInCrawlingChancePercent = GetFloatPropertyForCreature(RespawnInCrawlingChancePercent, creature);

        // Scream
        configZombie.ScreamHandlerIsActive = GetBoolPropertyForCreature(ScreamHandlerIsActive, creature);
        configZombie.ScreamAttractsZombiesInRadius = GetFloatPropertyForCreature(ScreamAttractsZombiesInRadius, creature);
        configZombie.ScreamChancePercent = GetFloatPropertyForCreature(ScreamChancePercent, creature);
        configZombie.ScreamCooldownSeconds = GetFloatPropertyForCreature(ScreamCooldownSeconds, creature);
        configZombie.ScreamNearbyInfectedSilenceSeconds = GetFloatPropertyForCreature(ScreamNearbyInfectedSilenceSeconds, creature);

        // Search body
        configZombie.SearchBodyToViewCargo = GetBoolPropertyForCreature(SearchBodyToViewCargo, creature);
        configZombie.SearchBodyToViewCargoSeconds = GetFloatPropertyForCreature(SearchBodyToViewCargoSeconds, creature);
        configZombie.SearchBodyExtendLootingToPlayersInRadiusMeters = GetFloatPropertyForCreature(SearchBodyExtendLootingToPlayersInRadiusMeters, creature);
        configZombie.SearchBodyOnlyEmptyHands = GetBoolPropertyForCreature(SearchBodyOnlyEmptyHands, creature);
        configZombie.SearchBodyWithoutGlovesBloodyHandsChancePercent = GetFloatPropertyForCreature(SearchBodyWithoutGlovesBloodyHandsChancePercent, creature);
        configZombie.SearchBodyWithoutMaskVomitChancePercent = GetFloatPropertyForCreature(SearchBodyWithoutMaskVomitChancePercent, creature);

        // Can be butchered
        configZombie.CanBeButchered = GetBoolPropertyForCreature(CanBeButchered, creature);
        configZombie.ButcheringSeconds = GetFloatPropertyForCreature(ButcheringSeconds, creature);
        configZombie.ButcheringWithoutGlovesBloodyHandsChancePercent = GetFloatPropertyForCreature(ButcheringWithoutGlovesBloodyHandsChancePercent, creature);
        configZombie.ButcheringWithoutMaskVomitChancePercent = GetFloatPropertyForCreature(ButcheringWithoutMaskVomitChancePercent, creature);
		configZombie.ItemsAfterButchering = GetItemsAfterButcheringPropertyForCreature(creature);

        // Can be backstabbed
        configZombie.CanBeBackstabbedHandlerIsActive = GetBoolPropertyForCreature(CanBeBackstabbedHandlerIsActive, creature);
        configZombie.CanBeBackstabbed = GetBoolPropertyForCreature(CanBeBackstabbed, creature);

        // Vision range multiplier
        configZombie.VisionRangeMultiplierDay = GetFloatPropertyForCreature(VisionRangeMultiplierDay, creature);
        configZombie.VisionRangeMultiplierNight = GetFloatPropertyForCreature(VisionRangeMultiplierNight, creature);

        configZombie.SizeHandlerIsActive = GetBoolPropertyForCreature(SizeHandlerIsActive, creature);
        configZombie.SizeFromMultiplier = GetFloatPropertyForCreature(SizeFromMultiplier, creature);
        configZombie.SizeToMultiplier = GetFloatPropertyForCreature(SizeToMultiplier, creature);
        configZombie.SizeDamageScalingIsActive = GetBoolPropertyForCreature(SizeDamageScalingIsActive, creature);
        configZombie.SizeBloodVolumeScalingIsActive = GetBoolPropertyForCreature(SizeBloodVolumeScalingIsActive, creature);
        configZombie.SizeSpeedMultiplierScalingIsActive = GetBoolPropertyForCreature(SizeSpeedMultiplierScalingIsActive, creature);

        // Items throwning
        configZombie.ThrowingProjectilesHandlerIsActive = GetBoolPropertyForCreature(ThrowingProjectilesHandlerIsActive, creature);
        configZombie.ThrowingProjectilesHandlerDamageMultiplier = GetFloatPropertyForCreature(ThrowingProjectilesHandlerDamageMultiplier, creature);
        configZombie.ThrowingProjectilesHandlerVehiclesDamageMultiplier = GetFloatPropertyForCreature(ThrowingProjectilesHandlerVehiclesDamageMultiplier, creature);
        configZombie.ThrowingProjectilesHandlerProjectilesList = GetArrayStringPropertyForCreature(ThrowingProjectilesHandlerProjectilesList, creature);

        // Range attacks
        configZombie.RangeAttacksHandlerIsActive = GetBoolPropertyForCreature(RangeAttacksHandlerIsActive, creature);
        configZombie.RangeAttacksHandlerZombiePlayerDistance = GetFloatPropertyForCreature(RangeAttacksHandlerZombiePlayerDistance, creature);
        configZombie.RangeAttacksHandlerPlayerOnObstacleHeight = GetFloatPropertyForCreature(RangeAttacksHandlerPlayerOnObstacleHeight, creature);

		
		m_CachedZombiesConfigs[creatureClassId] = configZombie;
		
        return configZombie;
    }

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------:::::::::::::::::::::----------------------------------------------------------------
// -----------------------------------------------------------::::::::::::::::::::::::::::::::::::-------------------------------------------------------
// ----------------------------------------------------:::::::::::::::::::::::::::::::::::::::::::::::::-------------------------------------------------
// -----------------------------------------------::::::::::::::::::::::::::::::::::::::--==--:::::::::::::::--------------------------------------------
// -------------------------------------------:::::::::::::::::::::::::::::-+++++++++***+=--======++=-:::::::::::----------------------------------------
// ---------------------------------------:::::::::::::::::::::::::::::-+**+-.........-::::. ::-====-=+++++=-::::::--------------------------------------
// ------------------------------------:::::::::::::::::::::::::::::-+*+-..       -++=:...  .::.    :-=++--=+*=-:::::::----------------------------------
// ---------------------------------:::::::::::::::::::::::::::::=**+:.        :--     .:::::...  ..... .-=+=-===::::::::::------------------------------
// ------------------------------:::::::::::::::::::::::::::::-*#*+=--::...----.    .::.                    :=+=-==::::::::::----------------------------
// ----------------------------:::::::::::::::::::::::::::::-*+:-=.....::=*%*====-=%*-.  .::..              .: .-++#*=:::::::::--------------------------
// -------------------------:::::::::::::::::::::::::::::..-#.:-=.......:--:......::+*@%*.: .:::.                 :+#:-:::::::::::-----------------------
// ------------------------:::::::::::::::::::::::::.......#:-:=:.....:---......:-==:#%@@%.-:  ..:... ...   ...   -::++::::::::::::----------------------
// -----------------------:::::::::::::::::::::::.........=+--:+:::::-:-:.::::---=::+-==+%# =  :::-:..  .--.....   .-::#:::::::::::::--------------------
// ----------------------::::::::::::::::::::::..........:%:+:=:::::::-:::::-==--::=::---#@ ---:...::::::::-::.:=:    -+*::::::::::::::------------------
// ---------------------::::::::::::::::::::.............*==::+::::::---:::+*---::-::-=--%*:+.-::---....  :--..:-+=    -.#:::::::::::::::----------------
// -------------------:::::::::::::::::::::.............-%.+::=:::::==-:::*+--::--::----#=-=.:...::-=--=-:: ::--.:==: .*-*=::::::::::::::::--------------
// -----------------:::::::::::::::::::::...............%.-=:+-::::+*::-++==-::==-----=#==:-:    .   .::-::-:=+:---:.::+-#-:::::::::::::::::-------------
// ---------------:::::::::::::::::::::................+#.*--*----++-+**-==---+==+===**==. .....::-::.   :::-=-#=--==-:+*.#:::::::::::::::::::-----------
// --------------:::::::::::::::::::.................=*=+.@=-*---**==++-*=--+#++*=-+*=--:.===+#%@@@%*++=:  .-.--+==:.:-%*-%::::::::::::::::::::----------
// ------------::::::::::::::::::::..................=**--@+=%**%*==**+#+=+*+-=#=*+=-----:.=@%%@%###%@@@@#: -. -.==#===-=:+*::::::::::::::::::::---------
// -----------::::::::::::::::::::................... *@:+@#+.   :+#%#%*+=#===%#+:-:::=-:+%%#%*++*++**%@@@@=::   :-:- .=*=-**..:::::::::::::::::---------
// ----------:::::::::::::::::::...................   .%:#%:=#%#+.::-*******++=:--...:+-*=*%%*%@*:....=%@@@@*=::.== *. . .  :*-:::::::::::::::::::-------
// ---------::::::::::::::::::::.................     :#:@-+#-+%%%=.=.+ +-=--:-. .:.  :-*---#%@%+:::%-.*@@@@@@%%#+--++:.==   .-#:::::::::::::::::::------
// --------:::::::::::::::::::..................      =#-@:+ +*:*%@##-* +:  ::--::.  :-.:+**##*%*=---=*%@@@@@@@%=. ...%@@%%##:-=#::::::::::::::::::------
// -------:::::::::::::::::::.................        :%=@* :*:-+@@@%:+*=       :--=-.:::--+#@@####**#%@@@%#%#=:--- .--%@#@@%*:.#-.::::::::::::::::::----
// ------::::::::::::::::::::................          =#*#.:%:.#@@=:.%@%+:  .-           :-=+*#**###%##*+==+:--:---::-=+#*%@#-*=....::::::::::::::::----
// ------:::::::::::::::::::................           ++##: =#-+@@= .@@+@@@#-.::-##*=:-+##*=: .=+++++--:-=*+=----.=%@@@+**+%+-.......::::::::::::::::---
// ------::::::::::::::::::................            #+:%+: .*%@@@*:=@+-%%%@@#=-=--+**=:.-*@@#=::::.-*%#*+-=-+.-%@@@%@=#**-.........::::::::::::::::---
// -----::::::::::::::::::................              =#=*:+:.:=:-++=*::.==:##@@#*=------- .#%%*+::*%%+*==:=@ =@@@@%###@%+...........::::::::::::::::--
// -----::::::::::::::::::...............                -%=*.-..-:=-:-::*:- . .%@@@@%%%%+#@= .+%.+--+**+:* :+@.+@@@@#+%+*##............::::::::::::::::-
// ----::::::::::::::::::................                 =*+@%*--::..=@%@#.:.  #@@@@#%%%%:*@. +%:+-*++:.*-=..=##:-=****#=*#-...........::::::::::::::::-
// ---:::::::::::::::::::...............                   %=%*#@#++*%@@@@@%*   *@@*...=%##.%= =#**++::.=**:- :.@.-:#=+@#.: +-..........::::::::::::::::-
// ---:::::::::::::::::::...............                   =#+=:+@#+*%%%@@@@*   *@@*....#+#.#* #%#=+.: .:=+=    #  =-++%%:-+#-..........::::::::::::::::-
// ---:::::::::::::::::::...............                   .%-%:-== .+*#%@@@+   +%#@*=-+#-=.%# @*==-=%@#*@+*@*+%#+-:.:+%#%*-............::::::::::::::::-
// ---:::::::::::::::::::...............                   .%=%%-.= .-+@%@@@= . -@%*%@@#-=-=@*:*++*@#+:-@*.==.=%=#*=%=-%-................::::::::::::::::
// ----::::::::::::::::::...............                   :#=@%%.:  -*%*@@@: . :%@@*@*::: +%+:++=@@%%-*@-.+. %:.#..#+%- ................::::::::::::::::
// --::::::::::::::::::::...............                   =++%-%=:  =#+.#@%=.. .=#@@%-:.:.-%-.*.%@@@@@@@%+%+**. * :*-# ................:::::::::::::::::
// ---:::::::::::::::::::...............               .:-=%-%:-%#-  *** +@#:    .+@@*=::.:-# +--@@@@@@@@#@@@@%**#--#-*.................:::::::::::::::::
// ---:::::::::::::::::::................           :#++==#+%-.=+%:..%@@.#*   :  :-.=*##+---# *:+@+%=#%*==#--=#+@@@@%#:.................:::::::::::::::--
// ---:::::::::::::::::::................          -#..*%#%#-.-:%@.=:*%%=@. .  :-. --..-+##@#-#-=*-::..#+===#*:.*####-  ...............::::::::::::::::--
// ----:::::::::::::::::::...............         +#..-*@@#..--%%@.+:#=*+*#  -:  -=-:=-====+*%#+:**+++=*+:-+=#=+*+*:-#.................:::::::::::::::---
// -----::::::::::::::::::................      .*+  ---** :+*%@*@.+-@:=*.@#  =:   .-+. ::+#%+-=-+-##=. .*-.:*: ::*%#:.................:::::::::::::::---
// ----:::::::::::::::::::.................    :#= :-..+-*+%@#@*-@:-%@*-% *@%. =.:::.:-. :::.-+*=-+..=#:-: .*=++**=-*=................::::::::::::::::---
// -----:::::::::::::::::::.................  -#: .=.   -=+@@@==.@::***=@ .%@@+-%*+*#*+=+*+*=..--++-- -%-*.+-=+*-=**=................::::::::::::::::----
// ------:::::::::::::::::::.................+%.   +     .+=@+.-=@::++-*@. =@@@@@@@@@@@#-. ...::-. -.- =* +=:.+:. %..................::::::::::::::::----
// ------::::::::::::::::::::..............=#%.    -.      +-%=#-@.:*.:*%:..@@@@%#%@@@@@@@@%*--=+*-    :. :.  -   +=................::::::::::::::::-----
// -------::::::::::::::::::::........:=++*+@@: .. .        -=*@+# -+ .##-. @@#%*#*-#@@@@@@@@@@#*##-::      .-. .:.%...............:::::::::::::::::-----
// --------::::::::::::::::::::...:=**##=-. +@@: .:          :+=@- *=  **+: %@:=#.-#@@@%@@#+=#@@@@@@%**=:..-+#@@@=:#...............:::::::::::::::::-----
// --------::::::::::::::::::::-+**+-:  +=*+-=@@= ::           +=+-@:  +=#- ##++:#@%*+**-.-*@%#++#%+=-=++++****#*+#:.............::::::::::::::::::------
// ----------::::::::::::::=+***-:.   .=---.  =@@* .-           :=+%.  *-%-.=@::%%+:**: :##*-:-*#-        ......:+..............::::::::::::::::::-------
// -----------::::::::-=*#**--: .::::::     :--#@@#..:            ==*- *-#==.@#*#-*+.  =++-..*@##=        .....................:::::::::::::::::::-------
// ------------::-=+***+==--:-:-=--.     :--.   #@@%..-            .==*%+-#= #%==%%. :#%*:.:%@%+-*+      ....................::::::::::::::::::::--------
// ------------=**+==**=-:::+--:      :--:=:    -@@@%:.-             -=+%:#=--%#@%-.*@@%- =@@*. * +#.   ....................::::::::::::::::::::---------
// -------------::::......         .. :+...--   .#:%@@-.-              ==*%*+ %%- :%@@+ .#@@=   =.--#:....................::::::::::::::::::::::---------
// ---------------:::::......   .::::.  +-. :=. :* .%@@.=               .=+## =%=#%@%: -@@@#    :- =:%*-................:::::::::::::::::::::::----------
// ----------------::::=--:::::::::::::. -+.  :-:=: *@@=-.                :=*+:@@%#*.--@@@@@    .=  -:#+++=............::::::::::::::::::::::------------
// ------------------::::---::::::::::::. .+::. =-* :@@#.=                  :=*@@*+-+:@@@@@@+    =   -.#*==+*+:......:::::::::::::::::::::::-------------
// --------------------:::::::::::::::::::..+-.  .++ %@@ +                   :-+%#+*-%@@*@@@@-   +   .: **==:=##+-.:::::::::::::::::::::::---------------
// ---------------------::::::::::::::::::::.-=:.  *=+@@:=.               :-::=+=+%+#@@#=@=%@@:  %-   .  +=.-: .:=++==:::::::::::::::::::----------------
// -----------------------:::::::::::::::::::::=:::.+*@@- =           .--::+%@%*@%.-#@@--%+*@@%  %+    :-:%:..:    ..-#::::::::::::::::------------------
// -------------------------::::::::::::::::::::==:::#@@- +        .--::+%@@*-  *@..:.-**##=@@@: #+ -#@@@*#%-+=--.. .:=%=---::::::::::-------------------
// ---------------------------::::::::::::::::::::=:=@@@= =     .--:-+%@@%+.    =@   .: =@**%@@* #+-@@@@@+#=#=.  ::::::::::---:::::----------------------
// -----------------------------:::::::::::::::::. ===@@+ :-.-+*-=*@@%%@@=. ...-*#::    *#=@@@@+ **-@@@@@-#--+. :::::::::::::::::------------------------
// -------------------------------:::::::::::::::::.:==@@##@@@@@@@@@%-.+#@%+. --+#-.  .:%+%@*=.:--=-@@@@@#@=--.:::::::::::::::::-------------------------
// ----------------------------------:::::::::::::::::+=#@@@@@@@#==*@@#=.:+%@*=**+.*=  *%+-  ::  -+@@@@@++@:=:::::::::::::::-----------------------------
// ------------------------------------::::::::::::::::-:=%@@**=.-:..@%@%+.-#@@@@*. .: %==-.  :+%@@@@@#-::#.:::::::::::::--------------------------------
// ---------------------------------------:::::::::::::::::=-::-+:  :#=@@@*-=-+#%* ..  @  .-+%@@%%@@%=::::-::::::::::------------------------------------
// --------------------------------------------::::::::::::::::::-::.-:*@%@@#=::-# ::..% .+%%+-##@@*-*:::::::::::::--------------------------------------
// ----------------------------------------------::::::::::::::::::::::::::=#@%+:*:::::+=#*-:-:--:-:::::::::::-------------------------------------------
// --------------------------------------------------::::::::::::::::::::::::::::-::::::::::::::::::::::-------------------------------------------------
// --------------------------------------------------------::::::::::::::::::::::::::::::::::::::::------------------------------------------------------
// ----------------------------------------------------------------::::::::::::::::::::::::--------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------

	[NonSerialized()]
	ref map<string, ref InediaInfectedAI_CustomIrritant> CustomIrritantsByClassIdMap;
	InediaInfectedAI_CustomIrritant GetCustomIrritantByItem(Object item)
	{
		if (!item) {
			return null;
		}

		if (!CustomIrritantsByClassIdMap) {
			CustomIrritantsByClassIdMap = new map<string, ref InediaInfectedAI_CustomIrritant>;

			foreach (InediaInfectedAI_CustomIrritant irritant: CustomIrritants) {
				if (irritant && irritant.Classes.Count() > 0) {
					foreach (string classId: irritant.Classes) {
						CustomIrritantsByClassIdMap[classId] = irritant;
					}
				}
			}
		}

		InediaInfectedAI_CustomIrritant searchedIrritant;
		foreach (string irritantConfigClass, InediaInfectedAI_CustomIrritant irritantConfigValue : CustomIrritantsByClassIdMap) {
	        if (InediaInfectedAI_Functions.IsKindOfCached(irritantConfigClass, item)) {
				return irritantConfigValue;
			}
        }

		return null;
	}

	InediaInfectedAI_ConfigThrowingProjectile GetThrowingProjectileByName(string name)
	{
		if (!name || !ThrowingProjectiles || ThrowingProjectiles.Count() == 0) {
			return null;
		}

		return ThrowingProjectiles.Get(name);
	}
}

class InediaInfectedAI_CustomIrritant
{
	ref array<string> Classes;
	float RadiusOutdoorDay;
	float RadiusOutdoorNight;
	float RadiusIndoorDay;
	float RadiusIndoorNight;
	float Type;
	float Priority;

	void InediaInfectedAI_CustomIrritant(array<string> pClasses, float pRadiusOutdoorDay, float pRadiusOutdoorNight, float pRadiusIndoorDay, float pRadiusIndoorNight, int pType, int pPriority)
	{
		Classes = pClasses;
		RadiusOutdoorDay = pRadiusOutdoorDay;
		RadiusOutdoorNight = pRadiusOutdoorNight;
		RadiusIndoorDay = pRadiusIndoorDay;
		RadiusIndoorNight = pRadiusIndoorNight;
		Type = pType;
		Priority = pPriority;
	}
}

class InediaInfectedAI_ReactCustomIrritant
{
	float React;
	float Destroy;

	void InediaInfectedAI_ReactCustomIrritant(float pReact, float pDestroy)
	{
		React = pReact;
		Destroy = pDestroy;
	}
}

class InediaInfectedAI_ConfigThrowingProjectile
{
	ref array<string> ItemClasses = {};
	ref map<string, float> Parameters = new map<string, float>;

	void SetParameterValue(string pName, float pValue) {
		if (!Parameters) {
			Parameters = new map<string, float>;
		}

		Parameters.Set(pName, pValue);
	}

	[NonSerialized()]
	ref map<string, float> ParametersDefaults = new map<string, float>;
	float GetParameterValue(string pName) {
		float returnValue;

		if (!Parameters) {
			Parameters = new map<string, float>;
		}

		if (!ParametersDefaults) {
			ParametersDefaults = new map<string, float>;
		}

		if (ParametersDefaults.Count() == 0) {
			ParametersDefaults.Set("HavingProjectileChancePercent", 100.0);

			ParametersDefaults.Set("InitialSpeed", 20.0);
			ParametersDefaults.Set("AccuracySpread", 0.05);

			ParametersDefaults.Set("TargetHealthDamage", -1.0);
			ParametersDefaults.Set("TargetShockDamage", -1.0);
			ParametersDefaults.Set("TargetPainMultiplier", 1);
			ParametersDefaults.Set("TargetStaminaDamage", -1.0);
			ParametersDefaults.Set("TargetBloodDamage", 0);
			ParametersDefaults.Set("TargetStunChancePercent", -1.0);
			ParametersDefaults.Set("TargetBleedingChancePercent", -1.0);
			ParametersDefaults.Set("TargetBreakChanceMultiplier", 1);
			ParametersDefaults.Set("TargetDeepWoundChanceMultiplier", 0.5);
			ParametersDefaults.Set("TargetArmsPainDisarmMultiplier", 0.3);

			ParametersDefaults.Set("ThrowConditionZombieTargetDistanceMax", 50.0);
			ParametersDefaults.Set("ThrowConditionZombieTargetDistanceMin", 0.0);
			ParametersDefaults.Set("ThrowConditionZombieTargetVerticalDistanceMax", 25.0);
			ParametersDefaults.Set("ThrowConditionZombieTargetVerticalDistanceMin", -25.0);
			ParametersDefaults.Set("ThrowConditionZombieTargetHorizontalDistanceMax", 25.0);
			ParametersDefaults.Set("ThrowConditionZombieTargetHorizontalDistanceMin", 0.0);
			ParametersDefaults.Set("ThrowConditionGroundTargetDistanceMax", 1000000.0);
			ParametersDefaults.Set("ThrowConditionGroundTargetDistanceMin", 0.0);
			ParametersDefaults.Set("ThrowConditionTargetInWater", -1.0);

			ParametersDefaults.Set("ThrowConditionAllowThrowingForCrawlers", 1);
			ParametersDefaults.Set("ThrowConditionAllowThrowingToPlayers", 1.0);
			ParametersDefaults.Set("ThrowConditionAllowThrowingToHorsePlayers", 1.0);
			ParametersDefaults.Set("ThrowConditionAllowThrowingToAnimals", 1.0);
			ParametersDefaults.Set("ThrowConditionAllowThrowingToVehicles", 1.0);
			ParametersDefaults.Set("ThrowConditionAllowThrowingAtDaytime", 1.0);
			ParametersDefaults.Set("ThrowConditionAllowThrowingAtNight", 1.0);
			ParametersDefaults.Set("ThrowConditionThrowChancePercent", 100.0);
			ParametersDefaults.Set("ThrowConditionCooldownSeconds", 0.0);
			ParametersDefaults.Set("ThrowConditionAmountOfProjectiles", -1.0);

			ParametersDefaults.Set("DestroyProjectileAfterSeconds", 0.001);
			ParametersDefaults.Set("DeleteProjectileAfterSeconds", 60.0);
			ParametersDefaults.Set("ActivateGrenadeAfterSeconds", -1.0);
			ParametersDefaults.Set("StartWorkingAfterSeconds", -1.0);
		}

		if (!Parameters.Find(pName, returnValue)) {
			if (!ParametersDefaults.Find(pName, returnValue)) {
				return -1.0;
			}
		}

		return returnValue;
	}
}

class InediaInfectedAI_ConfigZombie
{
    bool BreakingDoorsHandlerIsActive;
	ref map<string, ref array<int>> BreakingDoorsRestrictedDoors = new map<string, ref array<int>>;
    float BreakingDoorsDestroyAfterOpenChancePercent;
    float BreakingDoorsOpenChancePercent;
    float BreakingDoorsOpenLockPickChancePercent;
    float BreakingDoorsLossInterestAfterHitChancePercent;
    float BreakingDoorsLossInterestAfterHitLockPickChancePercent;
	float BreakingDoorsCrawlersChanceMultiplier;

    bool AttackPlayersUnconsciousHandlerIsActive;
    float AttackPlayersUnconsciousHealthDamageLimit;

	bool AttackAnimalsHandlerIsActive;
	//bool AttackAnimalsWolfs;
	//bool AttackAnimalsBears;
	bool AttackAnimalsWildBoar;
	bool AttackAnimalsPig;
	bool AttackAnimalsSheep;
	//bool AttackAnimalsChicken;
	bool AttackAnimalsDeer;
	bool AttackAnimalsRoeDeer;
	bool AttackAnimalsGoat;
	bool AttackAnimalsCow;
	ref map<string, bool> AttackAnimalsCustom;

	ref array<string> FriendlyNPC;

    bool SearchHandlerIsActive;
    float SearchSphereRadiusIncreaseEverySeconds;
    float SearchSphereDistanceToRadiusMultiplier;
    float SearchSphereRadiusMin;
    float SearchSphereRadiusMax;
    float SearchSphereRadiusBurst;

	bool SpeedHandlerIsActive;
	float SpeedMinimumInCalmMode;
	float SpeedLimitInCalmMode;
	float SpeedMultiplierInCalmMode;
	float SpeedMinimumInSearchMode;
	float SpeedLimitInSearchMode;
	float SpeedMultiplierInSearchMode;
	float SpeedMinimumInChaseMode;
	float SpeedLimitInChaseMode;
	float SpeedMultiplierInChaseMode;
	float SpeedNoLimitsFromDistanceMeters;

    bool SmellsHandlerIsActive;
    float SmellsSphereRadiusInCalmMode;
	float SmellsSphereRadiusInSearchMode;
	float SmellsChaseMultiplier;
    bool SmellsAllowStealthKills;
    float SmellsLossInterestAfterReactionForSeconds;
    bool SmellsDisableVanillaIrritationSound;
	
	bool LongVisionHandlerIsActive;
	float LongVisionMetersInSearchModeDay;
	float LongVisionMetersInSearchModeNight;
	float LongVisionMetersInCalmModeDay;
	float LongVisionMetersInCalmModeNight;
	float LongVisionVisualAcuityInSearchModeDay;
	float LongVisionVisualAcuityInSearchModeNight;
	float LongVisionVisualAcuityInCalmModeDay;
	float LongVisionVisualAcuityInCalmModeNight;
	float LongVisionPersistAfterIrritationSeconds;
	bool LongVisionAllowIgnoreWeather;
	
	bool ReturnToSpawnHandlerIsActive;
	float ReturnToSpawnRadiusMeters;

    bool UpJumpHandlerIsActive;
    float UpJumpHeightMax;
    float UpJumpImpulseDamageChancePercent;
    float UpJumpChancePercent;
    float UpJumpCooldownSeconds;

    bool DownJumpHandlerIsActive;
    float DownJumpHeightMax;
	
	bool StuckJumpHandlerIsActive;

    bool FallHandlerIsActive;
    float FallHandlerLegBreakHeightMin;
    float FallHandlerLegBreakHeightMax;
    float FallHandlerDeathHeightMin;
    float FallHandlerDeathHeightMax;

	bool InjuryHandlerIsActive;
	float InjuryHandlerInjuryOnExplosiveDamageChancePercent;
	float InjuryHandlerDamageThresholdToInjuryArm;
	float InjuryHandlerRestoreInjuredArmAfterSeconds;
	float InjuryHandlerInjuredOneArmAttackMultiplier;
	float InjuryHandlerInjuredBothArmsAttackMultiplier;
	float InjuryHandlerInjuredOneArmBreakingDoorChanceMultiplier;
	float InjuryHandlerInjuredBothArmsBreakingDoorChanceMultiplier;
	float InjuryHandlerInjuredOneArmStunChanceMultiplier;
	float InjuryHandlerInjuredBothArmsStunChanceMultiplier;
	float InjuryHandlerInjuredOneArmPainMultiplier;
	float InjuryHandlerInjuredBothArmsPainMultiplier;
	float InjuryHandlerInjuredOneArmBleedingChanceMultiplier;
	float InjuryHandlerInjuredBothArmsBleedingChanceMultiplier;
	float InjuryHandlerSpawnWithInjuredOneArmChancePercent;
	float InjuryHandlerSpawnWithInjuredBothArmsChancePercent;
	float InjuryHandlerDamageThresholdToInjuryLeg;
	float InjuryHandlerRestoreInjuredLegAfterSeconds;
	float InjuryHandlerInjuredOneLegSpeedLimit;
	float InjuryHandlerInjuredBothLegsSpeedLimit;
	float InjuryHandlerInjuredOneLegJumpHeightMultiplier;
	float InjuryHandlerInjuredBothLegsJumpHeightMultiplier;
	float InjuryHandlerSpawnWithInjuredOneLegChancePercent;
	float InjuryHandlerSpawnWithInjuredBothLegsChancePercent;

    bool AttackCarHandlerIsActive;
    bool AttackCarPlayersIsActive;
	float AttackCarPlayersDistanceMeters;
    bool AttackCarElementsIsActive;
	float AttackCarElementsDistanceMeters;
	float AttackCarElementsGlobalDamageMultiplier;
	bool AttackCarElementsByCollisionsIsActive;
	float AttackCarElementsByCollisionsGlobalDamageMultiplier;
    ref map<string, float> AttackCarElementsMultiplierByCarClassId = new map<string, float>;
    float AttackCarWindowDamagePercent;
    float AttackCarDoorDamagePercent;
    float AttackCarDoorChancePercent;
    float AttackCarRadiatorDamagePercent;
    float AttackCarCarBatteryDamagePercent;
    float AttackCarPlugDamagePercent;
    float AttackCarEngineDamagePercent;
    float AttackCarLightsDamagePercent;
    float AttackCarFenderDamagePercent;
    float AttackCarBumperDamagePercent;
    float AttackCarHoodDamagePercent;
    float AttackCarFuelTankDamagePercent;

    bool ReactHandlerIsActive;
    float ReactBehindVisualMultiplier;
    float ReactBehindNoiseMultiplier;
    float ReactFireplaceVisual;
    float ReactFireplaceDayVisual;
    float ReactFlashlightsVisual;
    float ReactHeadtorchRedVisual;
    float ReactRoadflareVisual;
    float ReactRoadflareNoise;
    float ReactChemlightVisual;
    float ReactChemlightRedVisual;
    float ReactPortableGasLampVisual;
    float ReactFlammablesVisual;
    float ReactSpotlightVisual;
    float ReactCarLightVisual;
    float ReactPowerGeneratorNoise;
    float ReactCarHornNoise;
    float ReactAlarmClockNoise;
    float ReactKitchenTimerNoise;
    float ReactFuelStationNoise;
    float ReactFireworksNoise;
    float ReactExplosiveItemNoise;
    float ReactCarEngineNoise;
	float ReactSmokeGrenadeVisual;
	float ReactScreamNoise;
    float ReactWeaponShotNoise;
    float ReactFriendlyWeaponShotNoise;
	float ReactFootstepsJogDayNoise;
	float ReactFootstepsJogNightNoise;
	float ReactFootstepsCrouchSprintDayNoise;
	float ReactFootstepsCrouchSprintNightNoise;
	float ReactFootstepsSprintDayNoise;
	float ReactFootstepsSprintNightNoise;
	float ReactHorseHoofstepsDayNoise;
	float ReactHorseHoofstepsNightNoise;
	float ReactDoorsDayNoise;
	float ReactDoorsNightNoise;
	float ReactPlayerFallDayNoise;
	float ReactPlayerFallNightNoise;
	float ReactBodyfallDayNoise;
	float ReactBodyfallNightNoise;
	float ReactBodyfallBackstabDayNoise;
	float ReactBodyfallBackstabNightNoise;
	float ReactSymptomsDayNoise;
	float ReactSymptomsNightNoise;
	float ReactVoiceWhisperDayNoise;
	float ReactVoiceWhisperNightNoise;
	float ReactVoiceTalkDayNoise;
	float ReactVoiceTalkNightNoise;
	float ReactVoiceShoutDayNoise;
	float ReactVoiceShoutNightNoise;
	float ReactMiningLightDayNoise;
	float ReactMiningLightNightNoise;
	float ReactMiningHardDayNoise;
	float ReactMiningHardNightNoise;
	float ReactBuildingDayNoise;
	float ReactBuildingNightNoise;
	float ReactSawPlanksDayNoise;
	float ReactSawPlanksNightNoise;
	float ReactVanillaMindstateChange;
	ref map<string, ref InediaInfectedAI_ReactCustomIrritant> ReactCustomIrritants;

	float ReactAndDestroyFlashlightsVisual;
	float ReactAndDestroyHeadtorchRedVisual;
	float ReactAndDestroyRoadflareVisual;
	float ReactAndDestroyRoadflareNoise;
	float ReactAndDestroyChemlightVisual;
	float ReactAndDestroyChemlightRedVisual;
	float ReactAndDestroyPortableGasLampVisual;
	float ReactAndDestroyFlammablesVisual;
	float ReactAndDestroySpotlightVisual;
	float ReactAndDestroyFireplaceVisual;
	float ReactAndDestroyFireplaceDayVisual;
	float ReactAndDestroyCarLightVisual;
	float ReactAndDestroyFireworksNoise;
	float ReactAndDestroyPowerGeneratorNoise;
	float ReactAndDestroyAlarmClockNoise;
	float ReactAndDestroyKitchenTimerNoise;
	float ReactAndDestroySmokeGrenadeVisual;

	bool DamageToPlayerHandlerIsActive;
	float DamageToPlayerHealthMultiplier;
	float DamageToPlayerInBlockHealthMultiplier;
	float DamageToPlayerShockMultiplier;
	float DamageToPlayerInBlockShockMultiplier;
	float DamageToPlayerStaminaPercent;
	float DamageToPlayerInBlockStaminaPercent;
	float DamageToPlayerBloodPercent;
	float DamageToPlayerInBlockBloodPercent;
	bool DamageToPlayerBleedingHandlerIsActive;
	float DamageToPlayerBleedingChancePercent;
	float DamageToPlayerInBlockBleedingChancePercent;

	float StunToPlayerChancePercent;
	float StunToPlayerInBlockChancePercent;
	
	float DisarmToPlayerChancePercent;
	float DisarmToPlayerInBlockChancePercent;
	
	float FastAttackToPlayerChancePercent;
	
	bool DiseasesToPlayerHandlerIsActive;
	ref array<ref InediaInfectedAI_ConfigZombieDeseaseAgent> DiseasesToPlayerAgents = new array<ref InediaInfectedAI_ConfigZombieDeseaseAgent>;
	
	float CameraShakeToPlayerIntensity;

	bool PainToPlayerHandlerIsActive;
	float PainToPlayerHeadMultiplier;
	float PainToPlayerHeadInBlockMultiplier;
	float PainToPlayerArmsMultiplier;
	float PainToPlayerArmsInBlockMultiplier;
	float PainToPlayerArmsDisarmMultiplier;
	float PainToPlayerArmsInBlockDisarmMultiplier;
	float PainToPlayerLegsMultiplier;
	float PainToPlayerLegsInBlockMultiplier;
	float PainToPlayerTorsoMultiplier;
	float PainToPlayerTorsoInBlockMultiplier;
	
	float PainToPlayerLimbsBreakChancesMultiplier;
	float PainToPlayerLimbsDeepWoundChancesMultiplier;

	bool DamageToZombieHandlerIsActive;
	float DamageToZombieHealthPoints;
	float DamageToZombieHealthPointsLeg;
	float DamageToZombieHeadMeleeMultiplier;
	float DamageToZombieHeadRangeMultiplier;
	float DamageToZombieBodyMeleeMultiplier;
	float DamageToZombieBodyRangeMultiplier;
	float DamageToZombieFromCarsMultiplier;
	float DamageToZombieFromExplosionsMultiplier;
	float DamageToZombieFromHotItemsHp;
	bool DamageToZombieBodyPartsPiercingIsActive;
	bool DamageToZombieAlwaysKillOnBackstab;
	bool DamageToZombieShockToStunHandlerIsActive;
	float DamageToZombieShockToStunLightHeavyAnimationThresholdMeleeHeavy;
	float DamageToZombieShockToStunLightHeavyAnimationThresholdRange;
	float DamageToZombieShockToStunThresholdMeleeHeavy;
	float DamageToZombieShockToStunThresholdRange;
	float DamageToZombieShockToStunFromButtstockHit;
	bool DamageToZombieShockToStunCumulativeDamage;
	float DamageToZombieShockToStunCumulativeDamageTimeFrameSeconds;
	float DamageToZombieShockToStunIgnoreMeleeHitChancePercent;
	float DamageToZombieShockToStunIgnoreRangedHitChancePercent;
	float DamageToZombieShockToStunImmunityAfterMeleeHitSeconds;
	float DamageToZombieShockToStunImmunityAfterRangedHitSeconds;
	ref map<string, float> DamageToZombieWeaponsMultipliers = new map<string, float>;

	bool MeleeAttacksDodgeHandlerIsActive;
	float MeleeAttacksDodgeChance;
	float MeleeAttacksDodgeCooldownSeconds;
	
	bool JumpAttackHandlerIsActive;
	float JumpAttackChance;
	float JumpAttackCooldownSeconds;

	bool BloodHandlerIsActive;
	float BloodVolumeMl;
	float BloodLossRateMinMl;
	float BloodLossRateMaxMl;
	float BloodLossRateRegenMl;
	float BloodOnExplosiveDamageChancePercent;
	bool BloodSplatParticlesIsActive;
	bool BloodTrailParticlesIsActive;
	bool BloodPoolParticlesIsActive;

	bool ResistContaminatedEffectHandlerIsActive;
	bool ResistContaminatedEffect;

	bool AllowCrawling;
	float RespawnInCrawlingChancePercent;

	bool ScreamHandlerIsActive;
	float ScreamAttractsZombiesInRadius;
	float ScreamChancePercent;
	float ScreamCooldownSeconds;
	float ScreamNearbyInfectedSilenceSeconds;

	bool SearchBodyToViewCargo;
	float SearchBodyToViewCargoSeconds;
	float SearchBodyExtendLootingToPlayersInRadiusMeters;
	bool SearchBodyOnlyEmptyHands;
	float SearchBodyWithoutGlovesBloodyHandsChancePercent;
	float SearchBodyWithoutMaskVomitChancePercent;

	bool CanBeButchered;
	float ButcheringSeconds;
	float ButcheringWithoutGlovesBloodyHandsChancePercent;
	float ButcheringWithoutMaskVomitChancePercent;
	ref array<ref InediaInfectedAI_ConfigZombieItemAfterButchering> ItemsAfterButchering = new array<ref InediaInfectedAI_ConfigZombieItemAfterButchering>;

	bool CanBeBackstabbedHandlerIsActive;
	bool CanBeBackstabbed;

	float VisionRangeMultiplierDay;
    float VisionRangeMultiplierNight;

	bool SizeHandlerIsActive;
	float SizeFromMultiplier;
	float SizeToMultiplier;
	bool SizeDamageScalingIsActive;
	bool SizeBloodVolumeScalingIsActive;
	bool SizeSpeedMultiplierScalingIsActive;

	bool ThrowingProjectilesHandlerIsActive;
	float ThrowingProjectilesHandlerDamageMultiplier;
	float ThrowingProjectilesHandlerVehiclesDamageMultiplier;
	ref array<string> ThrowingProjectilesHandlerProjectilesList = new array<string>;

	bool RangeAttacksHandlerIsActive;
	float RangeAttacksHandlerZombiePlayerDistance;
	float RangeAttacksHandlerPlayerOnObstacleHeight;

	float GetCarAttackMultiplierByCar(Object vehicle)
	{
		if (!vehicle) {
			return 1;
		}

		float multiplier = -1;
		
		if (!AttackCarElementsMultiplierByCarClassId.Find(vehicle.GetType(), multiplier)) {
			multiplier = -1;
			foreach (string vehicleConfigClass, float vehicleConfigValue : AttackCarElementsMultiplierByCarClassId) {
		        if (vehicleConfigClass != "all" && InediaInfectedAI_Functions.IsKindOfCached(vehicleConfigClass, vehicle)) {
					multiplier = vehicleConfigValue;
					break;
				}
	        }
		}

		if (multiplier == -1) {
			if (!AttackCarElementsMultiplierByCarClassId.Find("all", multiplier)) {
				multiplier = 1;
			}
		}

		if (multiplier < 0) {
			multiplier = 1;
		}

		return multiplier;
	}

	float GetWeaponMultiplierByWeapon(EntityAI weapon, string ammo, int damageType)
	{
		if (!DamageToZombieHandlerIsActive) {
			return 1;
		}
		
		float multiplier = -1;
		
		if (weapon) {
			// Do not search for the multiplier if a melee attack is made using ranged or explosive weapons
			if (damageType != DT_CLOSE_COMBAT || (!weapon.IsWeapon() && !InediaInfectedAI_Functions.IsKindOfCached("ExplosivesBase", weapon))) {
				if (!DamageToZombieWeaponsMultipliers.Find(weapon.GetType(), multiplier)) {
					multiplier = -1;
					foreach (string weaponConfigClass, float weaponConfigValue : DamageToZombieWeaponsMultipliers) {
				        if (weaponConfigClass != "all" && InediaInfectedAI_Functions.IsKindOfCached(weaponConfigClass, weapon)) {
							multiplier = weaponConfigValue;
							break;
						}
			        }
				}
			}
		}
		
		if (ammo && multiplier == -1) {
			foreach (string ammoConfigClass, float ammoConfigValue : DamageToZombieWeaponsMultipliers) {
		        if (ammoConfigClass != "all" && ammoConfigClass == ammo) {
					multiplier = ammoConfigValue;
					break;
				}
	        }
		}

		if (multiplier == -1) {
			if (!DamageToZombieWeaponsMultipliers.Find("all", multiplier)) {
				multiplier = 1;
			}
		}

		if (multiplier < 0) {
			multiplier = 1;
		}

		return multiplier;
	}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------:::::::::::::::::::::----------------------------------------------------------------
// -----------------------------------------------------------::::::::::::::::::::::::::::::::::::-------------------------------------------------------
// ----------------------------------------------------:::::::::::::::::::::::::::::::::::::::::::::::::-------------------------------------------------
// -----------------------------------------------::::::::::::::::::::::::::::::::::::::--==--:::::::::::::::--------------------------------------------
// -------------------------------------------:::::::::::::::::::::::::::::-+++++++++***+=--======++=-:::::::::::----------------------------------------
// ---------------------------------------:::::::::::::::::::::::::::::-+**+-.........-::::. ::-====-=+++++=-::::::--------------------------------------
// ------------------------------------:::::::::::::::::::::::::::::-+*+-..       -++=:...  .::.    :-=++--=+*=-:::::::----------------------------------
// ---------------------------------:::::::::::::::::::::::::::::=**+:.        :--     .:::::...  ..... .-=+=-===::::::::::------------------------------
// ------------------------------:::::::::::::::::::::::::::::-*#*+=--::...----.    .::.                    :=+=-==::::::::::----------------------------
// ----------------------------:::::::::::::::::::::::::::::-*+:-=.....::=*%*====-=%*-.  .::..              .: .-++#*=:::::::::--------------------------
// -------------------------:::::::::::::::::::::::::::::..-#.:-=.......:--:......::+*@%*.: .:::.                 :+#:-:::::::::::-----------------------
// ------------------------:::::::::::::::::::::::::.......#:-:=:.....:---......:-==:#%@@%.-:  ..:... ...   ...   -::++::::::::::::----------------------
// -----------------------:::::::::::::::::::::::.........=+--:+:::::-:-:.::::---=::+-==+%# =  :::-:..  .--.....   .-::#:::::::::::::--------------------
// ----------------------::::::::::::::::::::::..........:%:+:=:::::::-:::::-==--::=::---#@ ---:...::::::::-::.:=:    -+*::::::::::::::------------------
// ---------------------::::::::::::::::::::.............*==::+::::::---:::+*---::-::-=--%*:+.-::---....  :--..:-+=    -.#:::::::::::::::----------------
// -------------------:::::::::::::::::::::.............-%.+::=:::::==-:::*+--::--::----#=-=.:...::-=--=-:: ::--.:==: .*-*=::::::::::::::::--------------
// -----------------:::::::::::::::::::::...............%.-=:+-::::+*::-++==-::==-----=#==:-:    .   .::-::-:=+:---:.::+-#-:::::::::::::::::-------------
// ---------------:::::::::::::::::::::................+#.*--*----++-+**-==---+==+===**==. .....::-::.   :::-=-#=--==-:+*.#:::::::::::::::::::-----------
// --------------:::::::::::::::::::.................=*=+.@=-*---**==++-*=--+#++*=-+*=--:.===+#%@@@%*++=:  .-.--+==:.:-%*-%::::::::::::::::::::----------
// ------------::::::::::::::::::::..................=**--@+=%**%*==**+#+=+*+-=#=*+=-----:.=@%%@%###%@@@@#: -. -.==#===-=:+*::::::::::::::::::::---------
// -----------::::::::::::::::::::................... *@:+@#+.   :+#%#%*+=#===%#+:-:::=-:+%%#%*++*++**%@@@@=::   :-:- .=*=-**..:::::::::::::::::---------
// ----------:::::::::::::::::::...................   .%:#%:=#%#+.::-*******++=:--...:+-*=*%%*%@*:....=%@@@@*=::.== *. . .  :*-:::::::::::::::::::-------
// ---------::::::::::::::::::::.................     :#:@-+#-+%%%=.=.+ +-=--:-. .:.  :-*---#%@%+:::%-.*@@@@@@%%#+--++:.==   .-#:::::::::::::::::::------
// --------:::::::::::::::::::..................      =#-@:+ +*:*%@##-* +:  ::--::.  :-.:+**##*%*=---=*%@@@@@@@%=. ...%@@%%##:-=#::::::::::::::::::------
// -------:::::::::::::::::::.................        :%=@* :*:-+@@@%:+*=       :--=-.:::--+#@@####**#%@@@%#%#=:--- .--%@#@@%*:.#-.::::::::::::::::::----
// ------::::::::::::::::::::................          =#*#.:%:.#@@=:.%@%+:  .-           :-=+*#**###%##*+==+:--:---::-=+#*%@#-*=....::::::::::::::::----
// ------:::::::::::::::::::................           ++##: =#-+@@= .@@+@@@#-.::-##*=:-+##*=: .=+++++--:-=*+=----.=%@@@+**+%+-.......::::::::::::::::---
// ------::::::::::::::::::................            #+:%+: .*%@@@*:=@+-%%%@@#=-=--+**=:.-*@@#=::::.-*%#*+-=-+.-%@@@%@=#**-.........::::::::::::::::---
// -----::::::::::::::::::................              =#=*:+:.:=:-++=*::.==:##@@#*=------- .#%%*+::*%%+*==:=@ =@@@@%###@%+...........::::::::::::::::--
// -----::::::::::::::::::...............                -%=*.-..-:=-:-::*:- . .%@@@@%%%%+#@= .+%.+--+**+:* :+@.+@@@@#+%+*##............::::::::::::::::-
// ----::::::::::::::::::................                 =*+@%*--::..=@%@#.:.  #@@@@#%%%%:*@. +%:+-*++:.*-=..=##:-=****#=*#-...........::::::::::::::::-
// ---:::::::::::::::::::...............                   %=%*#@#++*%@@@@@%*   *@@*...=%##.%= =#**++::.=**:- :.@.-:#=+@#.: +-..........::::::::::::::::-
// ---:::::::::::::::::::...............                   =#+=:+@#+*%%%@@@@*   *@@*....#+#.#* #%#=+.: .:=+=    #  =-++%%:-+#-..........::::::::::::::::-
// ---:::::::::::::::::::...............                   .%-%:-== .+*#%@@@+   +%#@*=-+#-=.%# @*==-=%@#*@+*@*+%#+-:.:+%#%*-............::::::::::::::::-
// ---:::::::::::::::::::...............                   .%=%%-.= .-+@%@@@= . -@%*%@@#-=-=@*:*++*@#+:-@*.==.=%=#*=%=-%-................::::::::::::::::
// ----::::::::::::::::::...............                   :#=@%%.:  -*%*@@@: . :%@@*@*::: +%+:++=@@%%-*@-.+. %:.#..#+%- ................::::::::::::::::
// --::::::::::::::::::::...............                   =++%-%=:  =#+.#@%=.. .=#@@%-:.:.-%-.*.%@@@@@@@%+%+**. * :*-# ................:::::::::::::::::
// ---:::::::::::::::::::...............               .:-=%-%:-%#-  *** +@#:    .+@@*=::.:-# +--@@@@@@@@#@@@@%**#--#-*.................:::::::::::::::::
// ---:::::::::::::::::::................           :#++==#+%-.=+%:..%@@.#*   :  :-.=*##+---# *:+@+%=#%*==#--=#+@@@@%#:.................:::::::::::::::--
// ---:::::::::::::::::::................          -#..*%#%#-.-:%@.=:*%%=@. .  :-. --..-+##@#-#-=*-::..#+===#*:.*####-  ...............::::::::::::::::--
// ----:::::::::::::::::::...............         +#..-*@@#..--%%@.+:#=*+*#  -:  -=-:=-====+*%#+:**+++=*+:-+=#=+*+*:-#.................:::::::::::::::---
// -----::::::::::::::::::................      .*+  ---** :+*%@*@.+-@:=*.@#  =:   .-+. ::+#%+-=-+-##=. .*-.:*: ::*%#:.................:::::::::::::::---
// ----:::::::::::::::::::.................    :#= :-..+-*+%@#@*-@:-%@*-% *@%. =.:::.:-. :::.-+*=-+..=#:-: .*=++**=-*=................::::::::::::::::---
// -----:::::::::::::::::::.................  -#: .=.   -=+@@@==.@::***=@ .%@@+-%*+*#*+=+*+*=..--++-- -%-*.+-=+*-=**=................::::::::::::::::----
// ------:::::::::::::::::::.................+%.   +     .+=@+.-=@::++-*@. =@@@@@@@@@@@#-. ...::-. -.- =* +=:.+:. %..................::::::::::::::::----
// ------::::::::::::::::::::..............=#%.    -.      +-%=#-@.:*.:*%:..@@@@%#%@@@@@@@@%*--=+*-    :. :.  -   +=................::::::::::::::::-----
// -------::::::::::::::::::::........:=++*+@@: .. .        -=*@+# -+ .##-. @@#%*#*-#@@@@@@@@@@#*##-::      .-. .:.%...............:::::::::::::::::-----
// --------::::::::::::::::::::...:=**##=-. +@@: .:          :+=@- *=  **+: %@:=#.-#@@@%@@#+=#@@@@@@%**=:..-+#@@@=:#...............:::::::::::::::::-----
// --------::::::::::::::::::::-+**+-:  +=*+-=@@= ::           +=+-@:  +=#- ##++:#@%*+**-.-*@%#++#%+=-=++++****#*+#:.............::::::::::::::::::------
// ----------::::::::::::::=+***-:.   .=---.  =@@* .-           :=+%.  *-%-.=@::%%+:**: :##*-:-*#-        ......:+..............::::::::::::::::::-------
// -----------::::::::-=*#**--: .::::::     :--#@@#..:            ==*- *-#==.@#*#-*+.  =++-..*@##=        .....................:::::::::::::::::::-------
// ------------::-=+***+==--:-:-=--.     :--.   #@@%..-            .==*%+-#= #%==%%. :#%*:.:%@%+-*+      ....................::::::::::::::::::::--------
// ------------=**+==**=-:::+--:      :--:=:    -@@@%:.-             -=+%:#=--%#@%-.*@@%- =@@*. * +#.   ....................::::::::::::::::::::---------
// -------------::::......         .. :+...--   .#:%@@-.-              ==*%*+ %%- :%@@+ .#@@=   =.--#:....................::::::::::::::::::::::---------
// ---------------:::::......   .::::.  +-. :=. :* .%@@.=               .=+## =%=#%@%: -@@@#    :- =:%*-................:::::::::::::::::::::::----------
// ----------------::::=--:::::::::::::. -+.  :-:=: *@@=-.                :=*+:@@%#*.--@@@@@    .=  -:#+++=............::::::::::::::::::::::------------
// ------------------::::---::::::::::::. .+::. =-* :@@#.=                  :=*@@*+-+:@@@@@@+    =   -.#*==+*+:......:::::::::::::::::::::::-------------
// --------------------:::::::::::::::::::..+-.  .++ %@@ +                   :-+%#+*-%@@*@@@@-   +   .: **==:=##+-.:::::::::::::::::::::::---------------
// ---------------------::::::::::::::::::::.-=:.  *=+@@:=.               :-::=+=+%+#@@#=@=%@@:  %-   .  +=.-: .:=++==:::::::::::::::::::----------------
// -----------------------:::::::::::::::::::::=:::.+*@@- =           .--::+%@%*@%.-#@@--%+*@@%  %+    :-:%:..:    ..-#::::::::::::::::------------------
// -------------------------::::::::::::::::::::==:::#@@- +        .--::+%@@*-  *@..:.-**##=@@@: #+ -#@@@*#%-+=--.. .:=%=---::::::::::-------------------
// ---------------------------::::::::::::::::::::=:=@@@= =     .--:-+%@@%+.    =@   .: =@**%@@* #+-@@@@@+#=#=.  ::::::::::---:::::----------------------
// -----------------------------:::::::::::::::::. ===@@+ :-.-+*-=*@@%%@@=. ...-*#::    *#=@@@@+ **-@@@@@-#--+. :::::::::::::::::------------------------
// -------------------------------:::::::::::::::::.:==@@##@@@@@@@@@%-.+#@%+. --+#-.  .:%+%@*=.:--=-@@@@@#@=--.:::::::::::::::::-------------------------
// ----------------------------------:::::::::::::::::+=#@@@@@@@#==*@@#=.:+%@*=**+.*=  *%+-  ::  -+@@@@@++@:=:::::::::::::::-----------------------------
// ------------------------------------::::::::::::::::-:=%@@**=.-:..@%@%+.-#@@@@*. .: %==-.  :+%@@@@@#-::#.:::::::::::::--------------------------------
// ---------------------------------------:::::::::::::::::=-::-+:  :#=@@@*-=-+#%* ..  @  .-+%@@%%@@%=::::-::::::::::------------------------------------
// --------------------------------------------::::::::::::::::::-::.-:*@%@@#=::-# ::..% .+%%+-##@@*-*:::::::::::::--------------------------------------
// ----------------------------------------------::::::::::::::::::::::::::=#@%+:*:::::+=#*-:-:--:-:::::::::::-------------------------------------------
// --------------------------------------------------::::::::::::::::::::::::::::-::::::::::::::::::::::-------------------------------------------------
// --------------------------------------------------------::::::::::::::::::::::::::::::::::::::::------------------------------------------------------
// ----------------------------------------------------------------::::::::::::::::::::::::--------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------

	float GetCarDmgZoneDamagePercent(string dmgZoneName)
	{
		float damagePercent = 0;

		if (dmgZoneName == "Fender_1_1" || dmgZoneName == "Fender_1_2" || dmgZoneName == "Fender_2_1" || dmgZoneName == "Fender_2_2") {
			damagePercent = AttackCarFenderDamagePercent;
		} else if (dmgZoneName == "Front" || dmgZoneName == "Back") {
			damagePercent = AttackCarBumperDamagePercent;
		} else if (dmgZoneName == "FuelTank") {
			damagePercent = AttackCarFuelTankDamagePercent;
		} else if (dmgZoneName == "CarBattery" || dmgZoneName == "TruckBattery" || dmgZoneName == "ExpansionHelicopterBattery") {
			damagePercent = AttackCarCarBatteryDamagePercent;
		} else if (dmgZoneName == "SparkPlug" || dmgZoneName == "GlowPlug" || dmgZoneName == "ExpansionIgniterPlug" || dmgZoneName == "ExpansionHydraulicHoses") {
			damagePercent = AttackCarPlugDamagePercent;
		} else if (dmgZoneName == "Engine") {
			damagePercent = AttackCarEngineDamagePercent;
		} else if (dmgZoneName == "Radiator") {
			damagePercent = AttackCarRadiatorDamagePercent;
		} else if (dmgZoneName == "Reflector") {
			damagePercent = AttackCarLightsDamagePercent;
		} else if (dmgZoneName == "Hood") {
			damagePercent = AttackCarHoodDamagePercent;
		} else if (dmgZoneName == "Door") {
			damagePercent = AttackCarDoorDamagePercent;
		}

		return damagePercent;
	}

	[NonSerialized()]
	ref map<string, ref InediaInfectedAI_ReactCustomIrritant> ReactCustomIrritantsByClassIdMap;
	void InitReactCustomIrritantsByClassIdMap()
	{
		if (!ReactCustomIrritantsByClassIdMap) {
			ReactCustomIrritantsByClassIdMap = new map<string, ref InediaInfectedAI_ReactCustomIrritant>;

			foreach (string group, InediaInfectedAI_ReactCustomIrritant reactIrritant: ReactCustomIrritants) {

				InediaInfectedAI_CustomIrritant customIrritant;
				if (InediaInfectedAI_Config.GetInstance().Zombies.CustomIrritants.Find(group, customIrritant)) {
					if (customIrritant && customIrritant.Classes.Count() > 0) {
						foreach (string classId: customIrritant.Classes) {
							ReactCustomIrritantsByClassIdMap[classId] = reactIrritant;
						}
					}
				}
			}
		}
	}

	float GetSensitivityToCustomIrritantItem(EntityAI irritantItem)
	{
		if (!irritantItem) {
			return 0;
		}

		InitReactCustomIrritantsByClassIdMap();

		foreach (string irritantConfigClass, InediaInfectedAI_ReactCustomIrritant irritantConfigValue : ReactCustomIrritantsByClassIdMap) {
	        if (InediaInfectedAI_Functions.IsKindOfCached(irritantConfigClass, irritantItem)) {
				if (irritantConfigValue.React > 1) {
					return 1;
				} else if (irritantConfigValue.React < 0) {
					return 0;
				} else {
					return irritantConfigValue.React;
				}
			}
        }

		return 1;
	}

	float GetDamageToCustomIrritantItem(EntityAI irritantItem)
	{
		if (!irritantItem) {
			return 0;
		}

		InitReactCustomIrritantsByClassIdMap();

		foreach (string irritantConfigClass, InediaInfectedAI_ReactCustomIrritant irritantConfigValue : ReactCustomIrritantsByClassIdMap) {
	        if (InediaInfectedAI_Functions.IsKindOfCached(irritantConfigClass, irritantItem)) {
				if (irritantConfigValue.React > 0) {
					return irritantConfigValue.Destroy;
				} else {
					return 0;
				}
			}
        }

		return 1;
	}
}

class InediaInfectedAI_ConfigZombieItemAfterButchering
{
	string ClassId;
	float DropChancePercent;
	float QuantityInStackFromPercent;
	float QuantityInStackToPercent;
	int ConditionFrom;
	int ConditionTo;
	ref array<int> Foodstages;

	void InediaInfectedAI_ConfigZombieItemAfterButchering(string pClassId, float pDropChancePercent, float pQuantityInStackFromPercent, float pQuantityInStackToPercent, int pConditionFrom, int pConditionTo, array<int> pFoodstages)
	{
		ClassId = pClassId;
		DropChancePercent = pDropChancePercent;
		QuantityInStackFromPercent = pQuantityInStackFromPercent;
		QuantityInStackToPercent = pQuantityInStackToPercent;
		ConditionFrom = pConditionFrom;
		ConditionTo = pConditionTo;
		Foodstages = pFoodstages;
	}
}

class InediaInfectedAI_ConfigZombieDeseaseAgent
{
	int AgentId = 64;
	float AddChance = 100;
	float AddChanceInBlock = 0;
	float AddAmount = 1;
	float AddAmountInBlock = 0;
	float AddAmountNoMoreThan = 500;
	bool AddOnlyIfBleeding = true;
	bool AddOnlyToPlayers = true;

	void InediaInfectedAI_ConfigZombieDeseaseAgent(int pAgentId, float pAddChance, float pAddChanceInBlock, float pAddAmount, float pAddAmountInBlock, float pAddAmountNoMoreThan, bool pAddOnlyIfBleeding, bool pAddOnlyToPlayers)
	{
		AgentId = pAgentId;
		AddChance = pAddChance;
		AddChanceInBlock = pAddChanceInBlock;
		AddAmount = pAddAmount;
		AddAmountInBlock = pAddAmountInBlock;
		AddAmountNoMoreThan = pAddAmountNoMoreThan;
		AddOnlyIfBleeding = pAddOnlyIfBleeding;
		AddOnlyToPlayers = pAddOnlyToPlayers;
	}
}

class InediaInfectedAI_JsonFileLoader<Class T>
{
	protected static ref JsonSerializer m_Serializer = new JsonSerializer;

	static string m_LastError;

	static bool JsonLoadFile(string filename, out T data)
	{
		if (FileExist(filename)) {
			string file_content;
			string line_content;
			string error;

			FileHandle handle = OpenFile(filename, FileMode.READ);
			if (handle == 0) {
				m_LastError = "Failed to open configuration file.";
				return false;
			}

			while (FGets(handle,  line_content) >= 0) {
				file_content += line_content;
			}

			CloseFile(handle);

			if(!m_Serializer)
				m_Serializer = new JsonSerializer;

			if(m_Serializer.ReadFromString(data, file_content, error)) {
				return true;
			} else {
				m_LastError = error;

				return false;
			}
		} else {
			m_LastError = "Configuration file is missing.";
		}

		return false;
	}

	static void JsonSaveFile( string filename, T data )
	{
		string file_content;
		if( !m_Serializer )
			m_Serializer = new JsonSerializer;

		m_Serializer.WriteToString( data, true, file_content );

		FileHandle handle = OpenFile( filename, FileMode.WRITE );
		if ( handle == 0 )
			return;

		FPrint( handle, file_content );

		CloseFile( handle );
	}

	static void JsonLoadData( string string_data, out T data )
	{
		string error;
		if( !m_Serializer )
			m_Serializer = new JsonSerializer;

		if( !m_Serializer.ReadFromString( data, string_data, error ) )
			Error( error );
	}

	static string JsonMakeData( T data )
	{
		string string_data;
		if( !m_Serializer )
			m_Serializer = new JsonSerializer;

		m_Serializer.WriteToString( data, true, string_data );
		return string_data;
	}

	static string GetLastError()
	{
		return m_LastError;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------:::::::::::::::::::::----------------------------------------------------------------
// -----------------------------------------------------------::::::::::::::::::::::::::::::::::::-------------------------------------------------------
// ----------------------------------------------------:::::::::::::::::::::::::::::::::::::::::::::::::-------------------------------------------------
// -----------------------------------------------::::::::::::::::::::::::::::::::::::::--==--:::::::::::::::--------------------------------------------
// -------------------------------------------:::::::::::::::::::::::::::::-+++++++++***+=--======++=-:::::::::::----------------------------------------
// ---------------------------------------:::::::::::::::::::::::::::::-+**+-.........-::::. ::-====-=+++++=-::::::--------------------------------------
// ------------------------------------:::::::::::::::::::::::::::::-+*+-..       -++=:...  .::.    :-=++--=+*=-:::::::----------------------------------
// ---------------------------------:::::::::::::::::::::::::::::=**+:.        :--     .:::::...  ..... .-=+=-===::::::::::------------------------------
// ------------------------------:::::::::::::::::::::::::::::-*#*+=--::...----.    .::.                    :=+=-==::::::::::----------------------------
// ----------------------------:::::::::::::::::::::::::::::-*+:-=.....::=*%*====-=%*-.  .::..              .: .-++#*=:::::::::--------------------------
// -------------------------:::::::::::::::::::::::::::::..-#.:-=.......:--:......::+*@%*.: .:::.                 :+#:-:::::::::::-----------------------
// ------------------------:::::::::::::::::::::::::.......#:-:=:.....:---......:-==:#%@@%.-:  ..:... ...   ...   -::++::::::::::::----------------------
// -----------------------:::::::::::::::::::::::.........=+--:+:::::-:-:.::::---=::+-==+%# =  :::-:..  .--.....   .-::#:::::::::::::--------------------
// ----------------------::::::::::::::::::::::..........:%:+:=:::::::-:::::-==--::=::---#@ ---:...::::::::-::.:=:    -+*::::::::::::::------------------
// ---------------------::::::::::::::::::::.............*==::+::::::---:::+*---::-::-=--%*:+.-::---....  :--..:-+=    -.#:::::::::::::::----------------
// -------------------:::::::::::::::::::::.............-%.+::=:::::==-:::*+--::--::----#=-=.:...::-=--=-:: ::--.:==: .*-*=::::::::::::::::--------------
// -----------------:::::::::::::::::::::...............%.-=:+-::::+*::-++==-::==-----=#==:-:    .   .::-::-:=+:---:.::+-#-:::::::::::::::::-------------
// ---------------:::::::::::::::::::::................+#.*--*----++-+**-==---+==+===**==. .....::-::.   :::-=-#=--==-:+*.#:::::::::::::::::::-----------
// --------------:::::::::::::::::::.................=*=+.@=-*---**==++-*=--+#++*=-+*=--:.===+#%@@@%*++=:  .-.--+==:.:-%*-%::::::::::::::::::::----------
// ------------::::::::::::::::::::..................=**--@+=%**%*==**+#+=+*+-=#=*+=-----:.=@%%@%###%@@@@#: -. -.==#===-=:+*::::::::::::::::::::---------
// -----------::::::::::::::::::::................... *@:+@#+.   :+#%#%*+=#===%#+:-:::=-:+%%#%*++*++**%@@@@=::   :-:- .=*=-**..:::::::::::::::::---------
// ----------:::::::::::::::::::...................   .%:#%:=#%#+.::-*******++=:--...:+-*=*%%*%@*:....=%@@@@*=::.== *. . .  :*-:::::::::::::::::::-------
// ---------::::::::::::::::::::.................     :#:@-+#-+%%%=.=.+ +-=--:-. .:.  :-*---#%@%+:::%-.*@@@@@@%%#+--++:.==   .-#:::::::::::::::::::------
// --------:::::::::::::::::::..................      =#-@:+ +*:*%@##-* +:  ::--::.  :-.:+**##*%*=---=*%@@@@@@@%=. ...%@@%%##:-=#::::::::::::::::::------
// -------:::::::::::::::::::.................        :%=@* :*:-+@@@%:+*=       :--=-.:::--+#@@####**#%@@@%#%#=:--- .--%@#@@%*:.#-.::::::::::::::::::----
// ------::::::::::::::::::::................          =#*#.:%:.#@@=:.%@%+:  .-           :-=+*#**###%##*+==+:--:---::-=+#*%@#-*=....::::::::::::::::----
// ------:::::::::::::::::::................           ++##: =#-+@@= .@@+@@@#-.::-##*=:-+##*=: .=+++++--:-=*+=----.=%@@@+**+%+-.......::::::::::::::::---
// ------::::::::::::::::::................            #+:%+: .*%@@@*:=@+-%%%@@#=-=--+**=:.-*@@#=::::.-*%#*+-=-+.-%@@@%@=#**-.........::::::::::::::::---
// -----::::::::::::::::::................              =#=*:+:.:=:-++=*::.==:##@@#*=------- .#%%*+::*%%+*==:=@ =@@@@%###@%+...........::::::::::::::::--
// -----::::::::::::::::::...............                -%=*.-..-:=-:-::*:- . .%@@@@%%%%+#@= .+%.+--+**+:* :+@.+@@@@#+%+*##............::::::::::::::::-
// ----::::::::::::::::::................                 =*+@%*--::..=@%@#.:.  #@@@@#%%%%:*@. +%:+-*++:.*-=..=##:-=****#=*#-...........::::::::::::::::-
// ---:::::::::::::::::::...............                   %=%*#@#++*%@@@@@%*   *@@*...=%##.%= =#**++::.=**:- :.@.-:#=+@#.: +-..........::::::::::::::::-
// ---:::::::::::::::::::...............                   =#+=:+@#+*%%%@@@@*   *@@*....#+#.#* #%#=+.: .:=+=    #  =-++%%:-+#-..........::::::::::::::::-
// ---:::::::::::::::::::...............                   .%-%:-== .+*#%@@@+   +%#@*=-+#-=.%# @*==-=%@#*@+*@*+%#+-:.:+%#%*-............::::::::::::::::-
// ---:::::::::::::::::::...............                   .%=%%-.= .-+@%@@@= . -@%*%@@#-=-=@*:*++*@#+:-@*.==.=%=#*=%=-%-................::::::::::::::::
// ----::::::::::::::::::...............                   :#=@%%.:  -*%*@@@: . :%@@*@*::: +%+:++=@@%%-*@-.+. %:.#..#+%- ................::::::::::::::::
// --::::::::::::::::::::...............                   =++%-%=:  =#+.#@%=.. .=#@@%-:.:.-%-.*.%@@@@@@@%+%+**. * :*-# ................:::::::::::::::::
// ---:::::::::::::::::::...............               .:-=%-%:-%#-  *** +@#:    .+@@*=::.:-# +--@@@@@@@@#@@@@%**#--#-*.................:::::::::::::::::
// ---:::::::::::::::::::................           :#++==#+%-.=+%:..%@@.#*   :  :-.=*##+---# *:+@+%=#%*==#--=#+@@@@%#:.................:::::::::::::::--
// ---:::::::::::::::::::................          -#..*%#%#-.-:%@.=:*%%=@. .  :-. --..-+##@#-#-=*-::..#+===#*:.*####-  ...............::::::::::::::::--
// ----:::::::::::::::::::...............         +#..-*@@#..--%%@.+:#=*+*#  -:  -=-:=-====+*%#+:**+++=*+:-+=#=+*+*:-#.................:::::::::::::::---
// -----::::::::::::::::::................      .*+  ---** :+*%@*@.+-@:=*.@#  =:   .-+. ::+#%+-=-+-##=. .*-.:*: ::*%#:.................:::::::::::::::---
// ----:::::::::::::::::::.................    :#= :-..+-*+%@#@*-@:-%@*-% *@%. =.:::.:-. :::.-+*=-+..=#:-: .*=++**=-*=................::::::::::::::::---
// -----:::::::::::::::::::.................  -#: .=.   -=+@@@==.@::***=@ .%@@+-%*+*#*+=+*+*=..--++-- -%-*.+-=+*-=**=................::::::::::::::::----
// ------:::::::::::::::::::.................+%.   +     .+=@+.-=@::++-*@. =@@@@@@@@@@@#-. ...::-. -.- =* +=:.+:. %..................::::::::::::::::----
// ------::::::::::::::::::::..............=#%.    -.      +-%=#-@.:*.:*%:..@@@@%#%@@@@@@@@%*--=+*-    :. :.  -   +=................::::::::::::::::-----
// -------::::::::::::::::::::........:=++*+@@: .. .        -=*@+# -+ .##-. @@#%*#*-#@@@@@@@@@@#*##-::      .-. .:.%...............:::::::::::::::::-----
// --------::::::::::::::::::::...:=**##=-. +@@: .:          :+=@- *=  **+: %@:=#.-#@@@%@@#+=#@@@@@@%**=:..-+#@@@=:#...............:::::::::::::::::-----
// --------::::::::::::::::::::-+**+-:  +=*+-=@@= ::           +=+-@:  +=#- ##++:#@%*+**-.-*@%#++#%+=-=++++****#*+#:.............::::::::::::::::::------
// ----------::::::::::::::=+***-:.   .=---.  =@@* .-           :=+%.  *-%-.=@::%%+:**: :##*-:-*#-        ......:+..............::::::::::::::::::-------
// -----------::::::::-=*#**--: .::::::     :--#@@#..:            ==*- *-#==.@#*#-*+.  =++-..*@##=        .....................:::::::::::::::::::-------
// ------------::-=+***+==--:-:-=--.     :--.   #@@%..-            .==*%+-#= #%==%%. :#%*:.:%@%+-*+      ....................::::::::::::::::::::--------
// ------------=**+==**=-:::+--:      :--:=:    -@@@%:.-             -=+%:#=--%#@%-.*@@%- =@@*. * +#.   ....................::::::::::::::::::::---------
// -------------::::......         .. :+...--   .#:%@@-.-              ==*%*+ %%- :%@@+ .#@@=   =.--#:....................::::::::::::::::::::::---------
// ---------------:::::......   .::::.  +-. :=. :* .%@@.=               .=+## =%=#%@%: -@@@#    :- =:%*-................:::::::::::::::::::::::----------
// ----------------::::=--:::::::::::::. -+.  :-:=: *@@=-.                :=*+:@@%#*.--@@@@@    .=  -:#+++=............::::::::::::::::::::::------------
// ------------------::::---::::::::::::. .+::. =-* :@@#.=                  :=*@@*+-+:@@@@@@+    =   -.#*==+*+:......:::::::::::::::::::::::-------------
// --------------------:::::::::::::::::::..+-.  .++ %@@ +                   :-+%#+*-%@@*@@@@-   +   .: **==:=##+-.:::::::::::::::::::::::---------------
// ---------------------::::::::::::::::::::.-=:.  *=+@@:=.               :-::=+=+%+#@@#=@=%@@:  %-   .  +=.-: .:=++==:::::::::::::::::::----------------
// -----------------------:::::::::::::::::::::=:::.+*@@- =           .--::+%@%*@%.-#@@--%+*@@%  %+    :-:%:..:    ..-#::::::::::::::::------------------
// -------------------------::::::::::::::::::::==:::#@@- +        .--::+%@@*-  *@..:.-**##=@@@: #+ -#@@@*#%-+=--.. .:=%=---::::::::::-------------------
// ---------------------------::::::::::::::::::::=:=@@@= =     .--:-+%@@%+.    =@   .: =@**%@@* #+-@@@@@+#=#=.  ::::::::::---:::::----------------------
// -----------------------------:::::::::::::::::. ===@@+ :-.-+*-=*@@%%@@=. ...-*#::    *#=@@@@+ **-@@@@@-#--+. :::::::::::::::::------------------------
// -------------------------------:::::::::::::::::.:==@@##@@@@@@@@@%-.+#@%+. --+#-.  .:%+%@*=.:--=-@@@@@#@=--.:::::::::::::::::-------------------------
// ----------------------------------:::::::::::::::::+=#@@@@@@@#==*@@#=.:+%@*=**+.*=  *%+-  ::  -+@@@@@++@:=:::::::::::::::-----------------------------
// ------------------------------------::::::::::::::::-:=%@@**=.-:..@%@%+.-#@@@@*. .: %==-.  :+%@@@@@#-::#.:::::::::::::--------------------------------
// ---------------------------------------:::::::::::::::::=-::-+:  :#=@@@*-=-+#%* ..  @  .-+%@@%%@@%=::::-::::::::::------------------------------------
// --------------------------------------------::::::::::::::::::-::.-:*@%@@#=::-# ::..% .+%%+-##@@*-*:::::::::::::--------------------------------------
// ----------------------------------------------::::::::::::::::::::::::::=#@%+:*:::::+=#*-:-:--:-:::::::::::-------------------------------------------
// --------------------------------------------------::::::::::::::::::::::::::::-::::::::::::::::::::::-------------------------------------------------
// --------------------------------------------------------::::::::::::::::::::::::::::::::::::::::------------------------------------------------------
// ----------------------------------------------------------------::::::::::::::::::::::::--------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
