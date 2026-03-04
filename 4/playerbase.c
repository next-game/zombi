/**
 * Created: 2023-09-01
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

enum InediaInfectedAI_PlayerBase_ERPCs
{
	RPC_SYNC_CONFIG_VARIABLES = 7284356,
	RPC_SHOW_SMELLS = 7284357,
	RPC_USER_INFO_MESSAGE = 7284358,
	RPC_USER_IMPORTANT_MESSAGE = 7284359,
	RPC_HIT_CAMERA_SHAKE = 7284360,
	RPC_PLAY_SOUND = 7284361,
	RPC_RESET_ADS = 7284362,
	RPC_KNOCK_ITEM_OUT_OF_HANDS = 7284363,
	RPC_ALLOW_PLAYERS_TO_VIEW_CARGO = 7284364,
	RPC_SET_UNLIMITED_AMMO = 7284365,
	RPC_UNSET_UNLIMITED_AMMO = 7284366,
	RPC_FORCE_STANCE = 7284367,
	RPC_HEAD_HIT_ACTION = 7284368,
	RPC_SYNC_LIMBS_DEEP_WOUND_REGENERATION_LEVELS = 7284369,
	RPC_SYNC_BREAK_REGENERATION_LEVELS = 7284370,
	RPC_SYNC_PAIN_LEVELS = 7284371,
	RPC_SYNC_STABILIZATION_SHOCK_REDUCTION_MULTIPLIER = 7284372,
}

enum InediaInfectedAI_PlayerBaseSoundTypes {
	PAIN = 1,
	AUDIORY_HALLUCINATIONS = 2,
	FRACTURE = 3,
	DEEP_WOUND = 4,
}

modded class PlayerBase
{
	ref InediaInfectedAI_PlayerStorage m_InediaInfectedAI_PlayerStorage;
	
	ref array<string> m_InediaInfectedAI_NetSyncBoolVariables = {
		"BloodSplatParticlesIsActive", 
		"BloodTrailParticlesIsActive",
		"BloodPoolParticlesIsActive"
	};
	
	ref array<string> m_InediaInfectedAI_NetSyncBoolVariables2 = {
	};
	
	int m_InediaInfectedAI_NetSyncBoolVariablesSyncBits;
	int m_InediaInfectedAI_NetSyncBoolVariables2SyncBits;
	int m_InediaInfectedAI_DeepWoundsBleedingsSyncBits;

	ref InediaInfectedAI_Config m_InediaInfectedAI_ConfigInstance;
	ref InediaInfectedAI_ConfigPlayers m_InediaInfectedAI_ConfigPlayers;
	ref InediaInfectedAI_ConfigZombie m_InediaInfectedAI_ConfigZombie;
	
	ref InediaInfectedAI_PainManager m_InediaInfectedAI_PainManager;
	ref InediaInfectedAI_Smells m_InediaInfectedAI_SmellsInstance;
	ref BloodRegenMdfr m_InediaInfectedAI_VanillaBloodRegenMdfr;

	ref HumanMovementState m_MovementState = new HumanMovementState();
	int m_InediaInfectedAI_headBoneIndex;

 	bool m_InediaInfectedAI_SearchBodyToViewCargo = true;
	float m_InediaInfectedAI_SearchBodyToViewCargoSeconds = 3;
 	bool m_InediaInfectedAI_SearchBodyOnlyEmptyHands = false;
 	ref array<string> m_InediaInfectedAI_SearchBodyToViewCargoPlayersIds = new array<string>;

	bool m_InediaInfectedAI_QuietDoorOpeningMechanicIsActive = true;
	bool m_InediaInfectedAI_QuietDoorOpeningMechanicDisabledForOpening = true;
	float m_InediaInfectedAI_QuietDoorOpeningMechanicSeconds = 3;
	ref array<string> m_InediaInfectedAI_QuietDoorOpeningMechanicRestrictedBuildings = new array<string>;
	
	float m_InediaInfectedAI_SuppressorShotCooldownSeconds = 5;
	float m_InediaInfectedAI_SuppressorShotOverfireDistanceMeters = 80;

	ref map<string, ref array<string>> m_InediaInfectedAI_AudiouryHallutinationsSoundsets = new map<string, ref array<string>>;
	ref array<string> m_InediaInfectedAI_AudiouryHallutinationsSoundsetsGroups = {};

	ref InediaInfectedAI_ThrowingProjectile m_InediaInfectedAI_DefaulThrowingProjectile;

	eInjuryHandlerLevels m_InediaInfectedAI_InjuryOverrideNoLessThan = eInjuryHandlerLevels.PRISTINE;
	
	float m_InediaInfectedAI_BloodTrailFrequencySeconds = 1;
	int m_InediaInfectedAI_BloodParticlesBirthRateLimit = 3;
	int m_InediaInfectedAI_BloodLossLevel = 0; // 0 - 5, for the intensity of the blood trails on the client
	
	int m_InediaInfectedAI_BloodParticlesLimit = 1000;
	float m_InediaInfectedAI_BloodParticleDurationSeconds = 600;
	
	bool m_InediaInfectedAI_IsTestPlayer = false;
	bool m_InediaInfectedAI_TestPlayerReversePainIsActive = false;
	
	float m_InediaInfectedAI_UnconsciousHealthDamageCounter = 0;
	
	ref array<Shape> m_InediaInfectedAI_DebugSmellsShapes = new array<Shape>;

	void PlayerBase()
	{
		RegisterNetSyncVariableInt("m_InediaInfectedAI_NetSyncBoolVariablesSyncBits");
		RegisterNetSyncVariableInt("m_InediaInfectedAI_NetSyncBoolVariables2SyncBits");
		RegisterNetSyncVariableInt("m_InediaInfectedAI_DeepWoundsBleedingsSyncBits");
		RegisterNetSyncVariableInt("m_InediaInfectedAI_BloodLossLevel");
		
		m_InediaInfectedAI_PainManager = new InediaInfectedAI_PainManager(this);
		
		//m_InediaInfectedAI_AudiouryHallutinationsSoundsets["zmb_move"] = {"ZmbF_NurseFat_Base_AlertedMove_Soundset", "ZmbF_CitizenBSkinny_Base_AlertedMove_Soundset", "ZmbM_FarmerFat_Base_AlertedMove_Soundset", "ZmbM_SkaterYoung_Base_AlertedMove_Soundset", "ZmbM_FirefighterNormal_Base_AlertedMove_Soundset", "ZmbM_VillagerOld_Base_AlertedMove_Soundset", "ZmbM_HermitSkinny_Base_AlertedMove_Soundset"};
        m_InediaInfectedAI_AudiouryHallutinationsSoundsets["zmb_disturb"] = {"ZmbF_CitizenANormal_Base_DisturbedIdle_Soundset", "ZmbF_CitizenBSkinny_Base_DisturbedIdle_Soundset", "ZmbM_FarmerFat_Base_DisturbedIdle_Soundset", "ZmbM_SkaterYoung_Base_DisturbedIdle_Soundset", "ZmbM_FirefighterNormal_Base_DisturbedIdle_Soundset", "ZmbM_VillagerOld_Base_DisturbedIdle_Soundset", "ZmbM_HermitSkinny_Base_DisturbedIdle_Soundset"};
        m_InediaInfectedAI_AudiouryHallutinationsSoundsets["zmb_scream"] = {"ZmbM_Normal_CallToArmsShort_Soundset", "ZmbM_Normal_Old_CallToArmsShort_Soundset", "ZmbM_Normal2_CallToArmsShort_Soundset", "ZmbF_Normal_CallToArmsShort_Soundset", "ZmbM_Skinny_CallToArmsShort_Soundset", "ZmbM_Skinny_Old_CallToArmsShort_Soundset", "ZmbF_Skinny_CallToArmsShort_Soundset", "ZmbM_Fat_CallToArmsShort_Soundset", "ZmbF_Fat_CallToArmsShort_Soundset", "ZmbM_Old_CallToArmsShort_Soundset", "ZmbF_Old_CallToArmsShort_Soundset"};
        //m_InediaInfectedAI_AudiouryHallutinationsSoundsets["zmb_attack"] = {"ZmbF_Normal_Attack_Soundset", "ZmbF_Skinny_Attack_Soundset", "ZmbM_Fat_Attack_Soundset", "ZmbM_Normal2_Attack_Soundset", "ZmbM_Normal_Attack_Soundset", "ZmbM_Old_Attack_Soundset", "ZmbM_Skinny_Attack_Soundset"};
        //m_InediaInfectedAI_AudiouryHallutinationsSoundsets["zmb_hit"] = {"ZmbF_Normal_HeavyHit_Soundset", "ZmbF_Skinny_HeavyHit_Soundset", "ZmbM_Fat_HeavyHit_Soundset", "ZmbM_Normal2_HeavyHit_Soundset", "ZmbM_Normal_HeavyHit_Soundset", "ZmbM_Old_HeavyHit_Soundset", "ZmbM_Skinny_HeavyHit_Soundset"};
        m_InediaInfectedAI_AudiouryHallutinationsSoundsets["rifle_shot"] = {"IZH18_Shot_SoundSet", "M16_Shot_SoundSet", "SKS_Shot_SoundSet", "AK_Shot_SoundSet", "Famas_Shot_SoundSet"};
        m_InediaInfectedAI_AudiouryHallutinationsSoundsets["pistol_shot"] = {"CZ75_Shot_SoundSet", "Makarov_Shot_SoundSet", "Glock19_Shot_SoundSet"};
        m_InediaInfectedAI_AudiouryHallutinationsSoundsets["bullet"] = {"bulletFlyBy_SoundSet"};
        m_InediaInfectedAI_AudiouryHallutinationsSoundsetsGroups = m_InediaInfectedAI_AudiouryHallutinationsSoundsets.GetKeyArray();
		
#ifndef SERVER
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InediaInfectedAI_BloodTrailHandlerClient, 3000, false);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InediaInfectedAI_RemoveInactiveBloodParticlesClient, 3000, false);
#endif
	}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::-======-----=======-:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::-===-:.                 .::-====-::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::-==-:                              .:====::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::-==:                                     ..:-==-::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::.-+-    .....    .                             ::.:==::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::.-+:           .::: ...                            .:..==::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::+-                 ::: .::                           .. .+-::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::-+                     .-:  :.          .:...           .-  :+:::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::=+    -.           .:--.   -: .-           .-.             -  .*-:::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::-+    :=         .....:=%#=...:. :..                ....     -   +=::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::*     =.                 =##:-..:   ..:::.              --    -   +=:::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::=-   .+.               :-=--%%::-::..     .--:   .::.           .   *-::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::#   .=             ..::...=*%%% -:.- .       .-:    :--=-:..   ...  .*::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::*    .                .-*%%%%%#-= :. ---:.     .-: ..   .:----::...  -=:::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::-+                     .::-%%%%+. . ..    :-:::   :-  :::   ..---::..  *..:::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::-=              :         .*%%:       .:..::..:--=:.-.   .:::. ..   .  -=.:::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::-=       :.     -:  =     .#%=    ::::.     :--+:.-*--: :.   .-. .:-:   +::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::*        *+ .  =..:+ .   :%%   .+.  ::-::   .:=:.  =*--.-*+-. ...  .  .-*:::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::*::::.    *#:: + .#-  =.+%#.. .+  =%%%%%%#-         .==. +=  :          -+::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::.-+==----=: #%* #.*%:  +%%*::: = .==*%%%##%%#-     :  -=: .# .*  .=-.     :+:::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::+=-#%%%#-:==%%#%=%%=-#%%= .:.-  *-%#-:+*#%%%%#:  .. :-+ ..#..=:  -*#%#*=. -=::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::+-#%%*+=#%=:#*#%%%%%%%%+:  .= + .%%#=++=-=+#%%%%*==::=%*--+#*. +**%%%%#*%%-=-::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::+=%%+::::*%.+= :=+%%*=..   =:=*=*%%%#.. - .-#%%%%%%#%%*+..:=%%#%%%%**##*#%*::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::+-%#:--==+%=-: .:-===:....:-+*++=####++:.:-++%%%%%%%%%#=-:::*%%%%+--:-##*#:::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::*=%=+==+#%-:.::::......  .:   #+:-**###%%##*#*+#%%%#-:::-=-=*%%%+:..:***=.::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::++-+-%%%+==-::::::::::-   .+=-%+=:+*=+*#%%#=*%#++#. :++*+=+-%%#%%#*++*+-.::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::+::-=%%*==-.:=+*=.   .:   ..:+--+++#%%%%##*=-:=-:+.=#%%=**+:-#%%#*++%.+:::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::+::--*#++%+ ..:=#=   ..::.. ..--##*##+-: .---. =+:**#*+=#%%*.:#%%#%*:-=::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::=-. :-+=.%:-....-+-.   .--:-:..:...+ .-=-.::==*-:+=- .:-=%*%=.=+%+#=:=::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::=+:.. .%# --:=*=:-===.  ::-----=+::--+*=-+.%%+#%%%#. :+%*%%+.::=-+=:::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::-=+#%%%-.+*-*%+%*+-**+-..:-:  .++*=:.:+  #%%%%%%#:+%+*#-*+ +-..==:::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::.=%#%%# :#=*%:=%#%*--=*#*--*.+=-...-#.---::%#*=.=##*++.-=.:-=-+::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::-#:#%%.-.**%-:%-#%%--..:##-*.:: .:+--.  -::+- +*+-*=-.--:+=:::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::-* :%%-.-:%%*.%=-%%%-+...*%:*   --:+  .-  =- :-==.*+-:*=+:.:::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::=:= -%= -.-%+-%*.%%%%:+.  #+-+ ::..:  -  .=  :.-:.=++*=+:.::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::+ =.:%+  =::*:##.%%%%=*.  ++-* -::: .:-=--:..::::..+*=+:::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::+  =:%=  -=..##=:*%%#-+-. +-:%..==*##**=+:####%##*##**::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::-=:.* %=  .=+:=%+#*%*#-*=. *.:* =#+*-- =-.%+=:.+:*-+==-::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::.*:-:+.#-   +*. #=*%%++-#+..=.=  #%%%%+=+-.%.:..-.*++-+:.:::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::-======-+--=---*-   .-  -#*#*#++*= +..  -%%#*#%##%#%%%%%%*#..==-.:::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::==.:+**###+ *+=.-=%*=: .   :+#*+**+::+:= =%%***####*##*%%%###::*+:::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::==. .#%%%%%* =-*=. :+%%%%#=.  ..=*###.-.:- +#**#*-=+:#.+:++-=*:# ==:::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::=+.  .#%%%%%#..* *=.   *:*%%#*=-.   .=%-.-=...-+*=+###*#+#***##%%+=*-:::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::+-=   +%%%%%* .*: +=:   *  =%%#%*+=-.  +::.-..-: #=--=---::::-==---#-::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::.-+. -.  *%%%%* .++  ==:   *. =%%%%%%%#*+..-:  . =  =++=-+**#%*+-:.==:+=::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::.-+:    =. :%%%*  +%...==:   =.=%%%%%#+%%%%#:..  . ::-+=:-=:==+*+----=.+++::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::-=-      ::  -%%=.+%+.: =-:  .:.+%%%#%%.-#%%%%*= .+- ::.:=*--=+=*==-=*:=-==::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::.:+  .:     =.  .#*-%%:.=-+-:  :=.+%%%#+%.:*%%%%%%%=     .=:-.-+:*.=.  -*:.::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::=+-  .+:    =    =%%*..+*==.  +::=%%++-*.%%%%%%%%##%= .-:  - :.=.  : . *:.::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::==-  =:   =+.   =    .#-.-+-==.  +:.+#%--:=.%%%%+: :*%%%#=.  :  :-:..    -=::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::-==.  .  =. . .*-   =.    -=:.:-+  :=: =#+-.:-.%%*. :#%%%%+:.=+===+++====*=+-:::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::-=-:     =   =:.-  =*.  -:    .=-:=*  -=. :#--.-..%=  +%%%%%%#+:......::::::+-:::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::-==:        # .:-=::-  .*.  =:     :***  -* .-+=:.::=: :%%%%*%%%%%%-....:::::::+=:::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::.:===:          -#-.   -..-   -   +       -*. -*.:+.%. .#  :%%%+=%%%%%%%#+:..:::::::+==::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::.:-=+==:            -==    .::+..-  .   .=        =====.* *= +: .%%*:+%%%%*%%%%-==::::::::=--::::::::::::::::::::::::::::::::::::::::::::::::::::
// .:-++++-:             :--.   :-+:   +..=      -          =++:+.:#+:  ###.*#%%%%=:%%%%.:*-:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// =-:.               :::   :::: :=-    +..=     :.           =**: %:- ==%==+%%%%%= .*%%#  ++==-:::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//                .:::   .--.            +. -     .             -+.#:+ *-%:-%%%%%%=   :*%+  =- :-====-:::::::::::::::::::::::::::::::::::::::::::::::::::
//     ....     ::.    :::                +: -.                   -*==+:#-:*%%%%%%=     :#-  :#-..   :-==+:::::::::::::::::::::::::::::::::::::::::::::::
// ...     .::-:   .::.                    =- ::             -.     :=**-=.#%#+%%%*    .  -=.  +.        :#=:::::::::::::::::::::::::::::::::::::::::::::
//   :----.    .:-:.                        -= :-        ..:=         .**+:#*=:%%%*     =+=-==  --        +.-=-::::::::::::::::::::::::::::::::::::::::::
//        :--.   ...                      .. .+ .-        :.-         =*.#+-=:.*%%*-.:.*%%%%#=+. .=.      +   :*==-::::::::::::::::::::::::::::::::::::::
//           :-:    :.                  .. .:  =. -.        .        -%: .+=::.=%%*+::*%%%%%%%++: .+:.:   -:    : :+=::::::::::::::::::::::::::::::::::::
//             .--   .:.                     :. =: ::               =%:    .-=::%%+*.*%%%%%#==%#==  :=+    -        #::::::::::::::::::::::::::::::::::::
//                --   .:.                    .. -- .-             =%*-       -=%%%:#%%%%%%:. .#:.=:  *    -       .+=:::::::::::::::::::::::::::::::::::
//                  =:   ::                     . := .-           =%%-=#-       :=-=%%%%%%#   .-   .-.-:   :.      : =+::::::::::::::::::::::::::::::::::
//                   .-.   :.                    . .=  -.        =%%%%::#%*-.    *#=+%+#%%=  .-    . :=:    :     .- -:+:::::::::::::::::::::::::::::::::
//                     :-   .-                    :  += ::      =%%%=+%. =%*=*+-*%%+:#:%%%: .=     -  :.          =: - .*-:::::::::::::::::::::::::::::::
//                       ::   :.                   :  =- ::    +%%+..:-+: .++ :-=#%#.+-#%%  =      +.   ..       :+. =   +-::::::::::::::::::::::::::::::
//                     ..  :.  .:                      :- .-  +%#:    :-=:. :+  . -%:-==#* =.      #    -       .:#  #:   *::::::::::::::::::::::::::::::
//                       ..  .   :.                     .= .-=.=-.      :--==..  .:-*:+--#+       -*    +       =.# :: .  *::::::::::::::::::::::::::::::
//                        .:  ..  ::                      =:-  . -=:       .:       =+#-.+*    -  #.   :=       --=.-  - :#::::::::::::::::::::::::::::::
//                          :.  :  .:                      .    .  :=:      .:     ..=%:.-%.  :- --    -=      .:*:=  :- ==-:::::::::::::::::::::::::::::

	void ~PlayerBase()
	{
#ifndef SERVER
	#ifdef DIAG_DEVELOPER
		InediaInfectedAI_DebugHideSmellsShapes();
	#endif
#endif
	}
	
	InediaInfectedAI_PainManager InediaInfectedAI_GetPainManager()
	{
		return m_InediaInfectedAI_PainManager;
	}

// OBJECT AND CONFIG INITIALIZATION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override void EOnInit(IEntity other, int extra)
	{
		InediaInfectedAI_InitOnceServer();

		m_InediaInfectedAI_headBoneIndex = GetBoneIndexByName("Head");
	}
	
	bool m_InediaInfectedAI_InitOnceServerAlreadyInit = false;
	void InediaInfectedAI_InitOnceServer()
	{
#ifdef SERVER
		if (m_InediaInfectedAI_InitOnceServerAlreadyInit) {
			// Since some methods use this method for immediate config initialization,
			// it cannot be executed twice to prevent unnecessary initializations.
			return;
		}
		m_InediaInfectedAI_InitOnceServerAlreadyInit = true;
	
		// Config
		InediaInfectedAI_InitConfig();		

		// Smells instance
		m_InediaInfectedAI_SmellsInstance = InediaInfectedAI_Smells.GetInstance();
		
		// Modifiers
		if (GetModifiersManager()) {
			m_InediaInfectedAI_VanillaBloodRegenMdfr = BloodRegenMdfr.Cast(GetModifiersManager().GetModifier(eModifiers.MDF_BLOOD_REGEN));
		}
		
		// Init vanilla health and shock
		m_InediaInfectedAI_VanillaHeadHealthBeforeEEHitBy = GetHealth("Head", "Health");
		m_InediaInfectedAI_VanillaHealthBeforeEEHitBy = GetHealth();
		m_InediaInfectedAI_VanillaShockBeforeEEHitBy = GetHealth("", "Shock");

		// Default configuration projectile used to throw any item by the player
		InediaInfectedAI_ConfigThrowingProjectile configProjectile = new InediaInfectedAI_ConfigThrowingProjectile();
		configProjectile.SetParameterValue("DestroyProjectileAfterSeconds", -1);
		configProjectile.SetParameterValue("DeleteProjectileAfterSeconds", -1);
		configProjectile.SetParameterValue("ActivateGrenadeAfterSeconds", -1);
		configProjectile.SetParameterValue("StartWorkingAfterSeconds", -1);
		m_InediaInfectedAI_DefaulThrowingProjectile = new InediaInfectedAI_ThrowingProjectile(configProjectile);
#endif
	}
	
	float m_InediaInfectedAI_ConfigInitLastTime = -1;
	void InediaInfectedAI_InitConfig()
	{
#ifdef SERVER
		// Config instances
		m_InediaInfectedAI_ConfigInstance = InediaInfectedAI_Config.GetInstance();
		m_InediaInfectedAI_ConfigInitLastTime = InediaInfectedAI_Config.m_InstanceInitLastTime;
		m_InediaInfectedAI_ConfigPlayers = m_InediaInfectedAI_ConfigInstance.Players;
		m_InediaInfectedAI_ConfigZombie = m_InediaInfectedAI_ConfigInstance.Zombies.GetConfigZombie(this);
		
		// Init pain system config
		InediaInfectedAI_GetPainManager().InitConfig();
		
		// Sync variables (these variables will be synchronized between the server and the client)
		m_InediaInfectedAI_SearchBodyToViewCargo = m_InediaInfectedAI_ConfigPlayers.SearchBodyToViewCargo;
		m_InediaInfectedAI_SearchBodyToViewCargoSeconds = m_InediaInfectedAI_ConfigPlayers.SearchBodyToViewCargoSeconds;	
		m_InediaInfectedAI_SearchBodyOnlyEmptyHands = m_InediaInfectedAI_ConfigPlayers.SearchBodyOnlyEmptyHands;
		
		m_InediaInfectedAI_QuietDoorOpeningMechanicIsActive = m_InediaInfectedAI_ConfigPlayers.QuietDoorOpeningMechanicIsActive;
		m_InediaInfectedAI_QuietDoorOpeningMechanicDisabledForOpening = m_InediaInfectedAI_ConfigPlayers.QuietDoorOpeningMechanicDisabledForOpening;
		m_InediaInfectedAI_QuietDoorOpeningMechanicSeconds = m_InediaInfectedAI_ConfigPlayers.QuietDoorOpeningMechanicSeconds;
		m_InediaInfectedAI_QuietDoorOpeningMechanicRestrictedBuildings = m_InediaInfectedAI_ConfigPlayers.QuietDoorOpeningMechanicRestrictedBuildings;
		
		m_InediaInfectedAI_SuppressorShotCooldownSeconds = m_InediaInfectedAI_ConfigInstance.SuppressorShotCooldownSeconds;
		m_InediaInfectedAI_SuppressorShotOverfireDistanceMeters = m_InediaInfectedAI_ConfigInstance.SuppressorShotOverfireDistanceMeters;
		
		if (m_InediaInfectedAI_ConfigZombie.BloodHandlerIsActive) {
			InediaInfectedAI_SetNetSyncBoolVariable("BloodSplatParticlesIsActive", m_InediaInfectedAI_ConfigZombie.BloodSplatParticlesIsActive);
			InediaInfectedAI_SetNetSyncBoolVariable("BloodTrailParticlesIsActive", m_InediaInfectedAI_ConfigZombie.BloodTrailParticlesIsActive);
			InediaInfectedAI_SetNetSyncBoolVariable("BloodPoolParticlesIsActive", m_InediaInfectedAI_ConfigZombie.BloodPoolParticlesIsActive);
		} else {
			InediaInfectedAI_SetNetSyncBoolVariable("BloodSplatParticlesIsActive", false);
			InediaInfectedAI_SetNetSyncBoolVariable("BloodTrailParticlesIsActive", false);
			InediaInfectedAI_SetNetSyncBoolVariable("BloodPoolParticlesIsActive", false);
		}
		
		// Blood particles
		m_InediaInfectedAI_BloodParticlesLimit = m_InediaInfectedAI_ConfigInstance.BloodParticlesLimit;
		m_InediaInfectedAI_BloodParticleDurationSeconds = m_InediaInfectedAI_ConfigInstance.BloodParticleDurationSeconds;
		
		// Sync config variables with client
		InediaInfectedAI_SyncConfigVariablesWithClient();
#endif
	}
	
	void InediaInfectedAI_SyncConfigVariablesWithClient()
	{
#ifdef SERVER
		if (GetIdentity()) {
		    ScriptRPC rpc = new ScriptRPC();
			
		    rpc.Write(m_InediaInfectedAI_SearchBodyToViewCargo);
		    rpc.Write(m_InediaInfectedAI_SearchBodyToViewCargoSeconds);
		    rpc.Write(m_InediaInfectedAI_SearchBodyOnlyEmptyHands);
		    rpc.Write(m_InediaInfectedAI_BloodParticlesLimit);
		    rpc.Write(m_InediaInfectedAI_BloodParticleDurationSeconds);
		    rpc.Write(m_InediaInfectedAI_QuietDoorOpeningMechanicIsActive);
		    rpc.Write(m_InediaInfectedAI_QuietDoorOpeningMechanicDisabledForOpening);
		    rpc.Write(m_InediaInfectedAI_QuietDoorOpeningMechanicSeconds);
		    rpc.Write(m_InediaInfectedAI_QuietDoorOpeningMechanicRestrictedBuildings);
		    rpc.Write(m_InediaInfectedAI_SuppressorShotCooldownSeconds);
		    rpc.Write(m_InediaInfectedAI_SuppressorShotOverfireDistanceMeters);
			
			InediaInfectedAI_GetPainManager().SyncConfigVariablesServer(rpc);
			
		    rpc.Send(this, InediaInfectedAI_PlayerBase_ERPCs.RPC_SYNC_CONFIG_VARIABLES, true, GetIdentity());
		}
#endif
	}
	
// CONNECT/DISCONNECT HANDLERS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override void OnConnect()
	{
		super.OnConnect();
		
		InediaInfectedAI_SyncConfigVariablesWithClient();
		
		if (InediaInfectedAI_IsAdmin()) {
			if (InediaInfectedAI_Config.m_InstanceInitError) {
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InediaInfectedAI_SendImportantMessage, 10000, false, "InediaInfectedAI: Error in the configuration file, please check the server crash logs.", 2, 60);
			} else {
#ifdef INEDIA_PAIN
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InediaInfectedAI_SendImportantMessage, 11000, false, "InediaInfectedAI: You do not need to install 'InediaPain', as this modification is already included in 'InediaInfectedAI', which you are using.", 2, 60);
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InediaInfectedAI_SendImportantMessage, 12000, false, "In this case, 'InediaPain' is configured in the 'InediaInfectedAIConfig.json' configuration file, under the 'Players.*' parameter.", 2, 60);
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InediaInfectedAI_SendImportantMessage, 13000, false, "And 'InediaPainConfig.json' will be completely ignored.", 2, 60);
#endif
			}
		}
	}

	override void OnDisconnect()
	{
#ifdef SERVER
		// Destroy marked throwing projectiles in inventory.
		// This is done to prevent abuses, as upon disconnection, the timers for completion will not be able to execute certain logic with projectile items,
		// and consequently, they will remain on the server and can be used by the player.
		GameInventory inv = this.GetInventory();
		if (!inv) {
			return;
		}

		array<EntityAI> inventoryItems = new array<EntityAI>;
		inv.EnumerateInventory(InventoryTraversalType.PREORDER, inventoryItems);
		for (int i = 0; i < inventoryItems.Count(); i++) {
			ItemBase inInventoryItem = ItemBase.Cast(inventoryItems.Get(i));
			if (!inInventoryItem) {
				continue;
			}

			if (inInventoryItem.m_InediaInfectedAI_DestroyItemInPlayerInventoryWhenDisconnected) {
				inInventoryItem.SetHealth("", "", inInventoryItem.GetMaxHealth("", "") * GameConstants.DAMAGE_RUINED_VALUE);
			}
		}
#endif

		super.OnDisconnect();
	}
	
// CONFIG RELOAD HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	float m_InediaInfectedAI_ConfigReloadHandlerTimer = 0;
	void InediaInfectedAI_ConfigReloadHandler(float pDt)
	{
		m_InediaInfectedAI_ConfigReloadHandlerTimer -= pDt;
		if (m_InediaInfectedAI_ConfigReloadHandlerTimer > 0) return;
		m_InediaInfectedAI_ConfigReloadHandlerTimer = 1;
		
		if (m_InediaInfectedAI_ConfigInitLastTime != InediaInfectedAI_Config.m_InstanceInitLastTime) {
			InediaInfectedAI_InitConfig();
		}
	}
	
// ACTIONS SETUP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override void SetActions(out TInputActionMap InputActionMap)
	{
		super.SetActions(InputActionMap);

		AddAction(ActionSearchZombieBodyToViewCargo, InputActionMap);
		AddAction(InediaInfectedAI_ActionOpenDoorQuietly, InputActionMap);
		AddAction(InediaInfectedAI_ActionCloseDoorQuietly, InputActionMap);
		
		AddAction(InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveArmsSelf, InputActionMap);
		AddAction(InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveLegsSelf, InputActionMap);
		AddAction(InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveTorsoSelf, InputActionMap);
		AddAction(InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveHeadSelf, InputActionMap);
		
		AddAction(InediaInfectedAI_ActionDeepWoundUnbandageEffectiveArmsSelf, InputActionMap);
		AddAction(InediaInfectedAI_ActionDeepWoundUnbandageEffectiveLegsSelf, InputActionMap);
		AddAction(InediaInfectedAI_ActionDeepWoundUnbandageEffectiveTorsoSelf, InputActionMap);
		AddAction(InediaInfectedAI_ActionDeepWoundUnbandageEffectiveHeadSelf, InputActionMap);
		
		AddAction(InediaInfectedAI_ActionFractureUnstabilizationArmsSelf, InputActionMap);
		AddAction(InediaInfectedAI_ActionFractureUnstabilizationLegsSelf, InputActionMap);
		AddAction(InediaInfectedAI_ActionFractureUnstabilizationTorsoSelf, InputActionMap);
		AddAction(InediaInfectedAI_ActionFractureUnstabilizationHeadSelf, InputActionMap);
		
		AddAction(InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveArmsTarget, InputActionMap);
		AddAction(InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveLegsTarget, InputActionMap);
		AddAction(InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveTorsoTarget, InputActionMap);
		AddAction(InediaInfectedAI_ActionDeepWoundUnbandageIneffectiveHeadTarget, InputActionMap);
		
		AddAction(InediaInfectedAI_ActionDeepWoundUnbandageEffectiveArmsTarget, InputActionMap);
		AddAction(InediaInfectedAI_ActionDeepWoundUnbandageEffectiveLegsTarget, InputActionMap);
		AddAction(InediaInfectedAI_ActionDeepWoundUnbandageEffectiveTorsoTarget, InputActionMap);
		AddAction(InediaInfectedAI_ActionDeepWoundUnbandageEffectiveHeadTarget, InputActionMap);
		
		AddAction(InediaInfectedAI_ActionFractureUnstabilizationArmsTarget, InputActionMap);
		AddAction(InediaInfectedAI_ActionFractureUnstabilizationLegsTarget, InputActionMap);
		AddAction(InediaInfectedAI_ActionFractureUnstabilizationTorsoTarget, InputActionMap);
		AddAction(InediaInfectedAI_ActionFractureUnstabilizationHeadTarget, InputActionMap);
	}
	
// SAVING AND LOADING FROM STORAGE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
		
#ifdef SERVER
		if (!m_InediaInfectedAI_PlayerStorage) {
			m_InediaInfectedAI_PlayerStorage = new InediaInfectedAI_PlayerStorage(this);
		}
		
		m_InediaInfectedAI_PlayerStorage.Set("m_InediaInfectedAI_DeepWoundsBleedingsSyncBits", m_InediaInfectedAI_DeepWoundsBleedingsSyncBits);
		
		InediaInfectedAI_GetPainManager().OnStoreSave(m_InediaInfectedAI_PlayerStorage);
		
		m_InediaInfectedAI_PlayerStorage.CommitData();
#endif
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version)) return false;
		
#ifdef SERVER
		InediaInfectedAI_InitOnceServer();
		
		if (!m_InediaInfectedAI_PlayerStorage) {
			m_InediaInfectedAI_PlayerStorage = new InediaInfectedAI_PlayerStorage(this);
		}

		m_InediaInfectedAI_PlayerStorage.RefreshData();
		
		m_InediaInfectedAI_DeepWoundsBleedingsSyncBits = m_InediaInfectedAI_PlayerStorage.Get("m_InediaInfectedAI_DeepWoundsBleedingsSyncBits");
		
		InediaInfectedAI_GetPainManager().OnStoreLoad(m_InediaInfectedAI_PlayerStorage);
#endif

		return true;
	}
	
// RPC HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

#ifndef SERVER
		InediaInfectedAI_GetPainManager().OnPlayerBaseRPC(sender, rpc_type, ctx);
		
		switch (rpc_type) {
			case InediaInfectedAI_PlayerBase_ERPCs.RPC_SYNC_CONFIG_VARIABLES:
				ctx.Read(m_InediaInfectedAI_SearchBodyToViewCargo);
				ctx.Read(m_InediaInfectedAI_SearchBodyToViewCargoSeconds);
				ctx.Read(m_InediaInfectedAI_SearchBodyOnlyEmptyHands);
				ctx.Read(m_InediaInfectedAI_BloodParticlesLimit);
				ctx.Read(m_InediaInfectedAI_BloodParticleDurationSeconds);
				ctx.Read(m_InediaInfectedAI_QuietDoorOpeningMechanicIsActive);
				ctx.Read(m_InediaInfectedAI_QuietDoorOpeningMechanicDisabledForOpening);
				ctx.Read(m_InediaInfectedAI_QuietDoorOpeningMechanicSeconds);
				ctx.Read(m_InediaInfectedAI_QuietDoorOpeningMechanicRestrictedBuildings);
				ctx.Read(m_InediaInfectedAI_SuppressorShotCooldownSeconds);
				ctx.Read(m_InediaInfectedAI_SuppressorShotOverfireDistanceMeters);
			
				if (m_InediaInfectedAI_BloodParticleDurationSeconds > 0) {
					Hit_MeatBones.m_InediaInfectedAI_DisableBloodParticles = true;
				} else {
					Hit_MeatBones.m_InediaInfectedAI_DisableBloodParticles = false;
				}
			
				InediaInfectedAI_GetPainManager().SyncConfigVariablesClient(ctx);
			
			break;
			
			case InediaInfectedAI_PlayerBase_ERPCs.RPC_ALLOW_PLAYERS_TO_VIEW_CARGO:
				array<string> playersIdsForSync;
				ctx.Read(playersIdsForSync);
				
				if (playersIdsForSync && playersIdsForSync.Count() > 0) {
					foreach (string playerId: playersIdsForSync) {
						m_InediaInfectedAI_SearchBodyToViewCargoPlayersIds.Insert(playerId);
					}
				}
			
			break;
			
			case InediaInfectedAI_PlayerBase_ERPCs.RPC_HIT_CAMERA_SHAKE:
			    ctx.Read(m_InediaInfectedAI_CameraShakeSeconds);
			    ctx.Read(m_InediaInfectedAI_CameraShakeIntensity);
			break;
			
			case InediaInfectedAI_PlayerBase_ERPCs.RPC_USER_INFO_MESSAGE:
				if (ctx.Read(m_UAParamMessage)) {
					GetGame().Chat(m_UAParamMessage.param1, "colorInfo");
				}
			
			break;

			case InediaInfectedAI_PlayerBase_ERPCs.RPC_USER_IMPORTANT_MESSAGE:
				if (ctx.Read(m_UAParamMessage)) {
					GetGame().Chat(m_UAParamMessage.param1, "colorImportant");
				}
			
			break;

			case InediaInfectedAI_PlayerBase_ERPCs.RPC_PLAY_SOUND:
			    InediaInfectedAI_PlayerBaseSoundTypes rpcPlayeSoundSoundType;
			    int rpcPlayeSoundSoundVariation;
				ctx.Read(rpcPlayeSoundSoundType);
			    ctx.Read(rpcPlayeSoundSoundVariation);

				if (rpcPlayeSoundSoundType <= 0) {
					break;
				}

				InediaInfectedAI_PlaySoundClient(rpcPlayeSoundSoundType, rpcPlayeSoundSoundVariation);
			break;

			case InediaInfectedAI_PlayerBase_ERPCs.RPC_RESET_ADS:
				InediaInfectedAI_ResetADS();
			break;

			case InediaInfectedAI_PlayerBase_ERPCs.RPC_KNOCK_ITEM_OUT_OF_HANDS:
				InediaInfectedAI_KnockItemOutOfHands();
			break;
			
			case InediaInfectedAI_PlayerBase_ERPCs.RPC_SET_UNLIMITED_AMMO:
				InediaInfectedAI_SetUnlimitedAmmo();
			break;
			
			case InediaInfectedAI_PlayerBase_ERPCs.RPC_UNSET_UNLIMITED_AMMO:
				InediaInfectedAI_UnsetUnlimitedAmmo();
			break;
			
			case InediaInfectedAI_PlayerBase_ERPCs.RPC_FORCE_STANCE:
				DayZPlayerConstants forceStance;
				if (ctx.Read(forceStance)) {
					InediaInfectedAI_ForceStance(forceStance);
				}
			break;
		}

	#ifdef DIAG_DEVELOPER
		if (rpc_type == InediaInfectedAI_PlayerBase_ERPCs.RPC_SHOW_SMELLS) {
			array<vector> positions;
			array<float> radiuses;
			ctx.Read(positions);
			ctx.Read(radiuses);
			InediaInfectedAI_DebugShowSmellsShapes(positions, radiuses);
		}
	#endif
#endif
	}
	
// SYNCHRONIZATION OF CLIENT-SERVER VARIABLES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
		InediaInfectedAI_GetPainManager().OnVariablesSynchronized();
	}
	
// LOOP ON CURRENT PLAYER CLIENT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	float m_InediaInfectedAI_EOnFrameLastRunTime = 0;
	override void EOnFrame(IEntity other, float timeSlice)
	{
		super.EOnFrame(other, timeSlice);


		float deltaTime = GetGame().GetTickTime() - m_InediaInfectedAI_EOnFrameLastRunTime;
		if (deltaTime >= 1) {
#ifdef SERVER
			InediaInfectedAI_OnTickServer(deltaTime);
#else
			InediaInfectedAI_OnTickClient(deltaTime);
#endif
			m_InediaInfectedAI_EOnFrameLastRunTime = GetGame().GetTickTime();
		}

	}
	void InediaInfectedAI_OnTickServer(float pDt)
	{
		InediaInfectedAI_GetPainManager().OnTickServer(pDt);
	}
	void InediaInfectedAI_OnTickClient(float pDt)
	{
		InediaInfectedAI_GetPainManager().OnTickClient(pDt);
	}
	
// COMMAND HANDLING LOOP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override void OnCommandHandlerTick(float delta_time, int pCurrentCommandID)
	{
#ifdef SERVER
		// This variables is necessary to correctly obtain HP/Shock before executing EEHitBy(), as EEHitBy is triggered after HP has already been reduced.
		m_InediaInfectedAI_VanillaHeadHealthBeforeEEHitBy = GetHealth("Head", "Health");
		m_InediaInfectedAI_VanillaHealthBeforeEEHitBy = GetHealth();
		m_InediaInfectedAI_VanillaShockBeforeEEHitBy = GetHealth("", "Shock");
		
		// m_InediaInfectedAI_Killed sync
		if (!m_InediaInfectedAI_Killed && !IsAlive()) {
			m_InediaInfectedAI_Killed = true;
		}
		
		if (m_InediaInfectedAI_ConfigPlayers && m_InediaInfectedAI_ConfigZombie && IsAlive()) {
			InediaInfectedAI_ConfigReloadHandler(delta_time);
			// InediaInfectedAI_TimersHandler(delta_time);
			InediaInfectedAI_StaminaHandler(delta_time);
			
			if (!InediaInfectedAI_IsInHideMode() && !InediaInfectedAI_IsNPC()) {
				InediaInfectedAI_SmellsHandler(delta_time);
				InediaInfectedAI_FootstepsNoiseHandler(delta_time);
				InediaInfectedAI_SpeakHandler(delta_time);
			}

			// This handler should work for both players and NPCs in any mode.
			InediaInfectedAI_UnconsciousHandler(delta_time);
			
			if (GetAllowDamage()) {
				InediaInfectedAI_MovementCoordinationImpairmentHandler(delta_time);
			}
			
	#ifdef DIAG_DEVELOPER
			InediaInfectedAI_ShowDebugSmellsHandler(delta_time);
	#endif
		}
#endif
		
		super.OnCommandHandlerTick(delta_time, pCurrentCommandID);
	}
	
// RESPAWN EVENT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_RespawnEvent()
	{
#ifdef SERVER
		// Sometimes, possibly due to mods interfering with the character respawn process or server issues during restarts,
		// Inedia debuffs would persist for players after death and respawn because the server did not recreate the PlayerBase object.
		// Now, upon a player's death, Inedia debuffs are cleared, which theoretically should prevent such situations.
		InediaInfectedAI_RemoveInediaDebuffs();
#endif
	}
	
// KILLED EVENT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override void EEKilled(Object killer)
	{
		// Remove bleeding particles from deep wounds ~30 seconds after death
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InediaInfectedAI_RemoveDeepWoundBleedingParticles, Math.RandomFloatInclusive(20000, 40000), false);

		super.EEKilled(killer);
	}
	
// DAMAGE RECEIVED HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	float m_InediaInfectedAI_WeaponMultiplier = 1;
	float m_InediaInfectedAI_DamageMultiplier = 1;
	float m_InediaInfectedAI_ShockDamageMultiplier = 1;
	
	bool m_InediaInfectedAI_Killed = false;
	
	float m_InediaInfectedAI_HeadHealthDamage;
	float m_InediaInfectedAI_HealthDamage;
	float m_InediaInfectedAI_ShockDamage;
	int m_InediaInfectedAI_EEHitByBleedingSourceCountBeforeHit;
	float m_InediaInfectedAI_VanillaHeadHealthBeforeEEHitBy;
	float m_InediaInfectedAI_VanillaHealthBeforeEEHitBy;
	float m_InediaInfectedAI_VanillaShockBeforeEEHitBy;
	
	TotalDamageResult m_InediaInfectedAI_EEHitByLastDamageResult;
	int m_InediaInfectedAI_EEHitByLastDamageType;
	EntityAI m_InediaInfectedAI_EEHitByLastSource;
	int m_InediaInfectedAI_EEHitByLastComponent;
	string m_InediaInfectedAI_EEHitByLastDmgZone;
	string m_InediaInfectedAI_EEHitByLastAmmo;
	vector m_InediaInfectedAI_EEHitByLastModelPos;
	float m_InediaInfectedAI_EEHitByLastSpeedCoef;
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (ammo == "InediaInfectedAI_ThrowingProjectile") {
			damageType = DT_CUSTOM;
		}
		
#ifdef SERVER
		m_InediaInfectedAI_WeaponMultiplier = 1;
		m_InediaInfectedAI_DamageMultiplier = 1;
		m_InediaInfectedAI_ShockDamageMultiplier = 1;
		
		m_InediaInfectedAI_EEHitByBleedingSourceCountBeforeHit = GetBleedingSourceCount();
		
		m_InediaInfectedAI_EEHitByLastDamageResult = damageResult;
		m_InediaInfectedAI_EEHitByLastDamageType = damageType;
		m_InediaInfectedAI_EEHitByLastSource = source;
		m_InediaInfectedAI_EEHitByLastComponent = component;
		m_InediaInfectedAI_EEHitByLastDmgZone = dmgZone;
		m_InediaInfectedAI_EEHitByLastAmmo = ammo;
		m_InediaInfectedAI_EEHitByLastModelPos = modelPos;
		m_InediaInfectedAI_EEHitByLastSpeedCoef = speedCoef;
		
		if (!damageResult || !source || !m_InediaInfectedAI_ConfigPlayers || !m_InediaInfectedAI_ConfigZombie || m_InediaInfectedAI_Killed) {
			super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
			return;
		}
		
		m_InediaInfectedAI_HeadHealthDamage = damageResult.GetDamage("Head", "Health");
		m_InediaInfectedAI_HealthDamage = damageResult.GetDamage("", "Health");
		m_InediaInfectedAI_ShockDamage = damageResult.GetDamage("", "Shock");

		InediaInfectedAI_EEHitByFromAnyToPlayerDamage(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);		
		InediaInfectedAI_EEHitByCarCollisionActions(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		InediaInfectedAI_EEHitByToCarDamage(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		InediaInfectedAI_EEHitByFromZombieToPlayerStun(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		InediaInfectedAI_EEHitByFromZombieToPlayerCameraShake(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		InediaInfectedAI_EEHitByFromAnyToPlayerDisarm(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);		
		
		int vanillaBleedingSourcesCount = GetBleedingSourceCount();
		float healthDamageFromOtherModsAfter = GetHealth();
		float shockDamageFromOtherModsAfter = GetHealth("", "Shock");
		
		if (!m_InediaInfectedAI_Killed) {
			super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		}
		
		vanillaBleedingSourcesCount = GetBleedingSourceCount() - vanillaBleedingSourcesCount;
		healthDamageFromOtherModsAfter -= GetHealth();
		shockDamageFromOtherModsAfter -= GetHealth("", "Shock");
		
		// Remove vanilla bleedings if Inedia is already processing bleedings above.
		if (source && source.IsZombie() && vanillaBleedingSourcesCount > 0) {
			ZombieBase zmb = ZombieBase.Cast(source);
			if (GetBleedingManagerServer() && zmb && zmb.m_InediaInfectedAI_ConfigZombie && zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerHandlerIsActive && zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerBleedingHandlerIsActive) {
				GetBleedingManagerServer().InediaInfectedAI_RemoveMostSignificantBleedingSourceWithoutSepsis();
			}
		}
		
		// Pain processing and some other processes that require information about bleeding must occur after the parent super.EEHitBy()
		// because the handler requires information about open vanilla bleedings.
		InediaInfectedAI_GetPainManager().EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		InediaInfectedAI_EEHitByToTestPlayerReversePain(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		InediaInfectedAI_EEHitByFromAnyToPlayerDisease(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		
		if (InediaInfectedAI_Debug.IsDebugRuleEnabled("dmg")) {
			float logHealth = GetHealth();
			if (m_InediaInfectedAI_Killed) {
				logHealth = 0;
			}
			MiscGameplayFunctions.InediaInfectedAI_DmgDebug(GetType(), source.GetType(), ammo, dmgZone, m_InediaInfectedAI_HealthDamage * m_InediaInfectedAI_DamageMultiplier + healthDamageFromOtherModsAfter, m_InediaInfectedAI_ShockDamage * m_InediaInfectedAI_ShockDamageMultiplier + shockDamageFromOtherModsAfter, logHealth);
		}

		m_InediaInfectedAI_EEHitByBleedingSourceCountBeforeHit = 0;
		
		if (!m_InediaInfectedAI_Killed) {
			m_InediaInfectedAI_VanillaHeadHealthBeforeEEHitBy = GetHealth("Head", "Health");
			m_InediaInfectedAI_VanillaHealthBeforeEEHitBy = GetHealth();
			m_InediaInfectedAI_VanillaShockBeforeEEHitBy = GetHealth("", "Shock");
		}
#else
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
#endif
	}
	
// DAMAGE FROM ENTITIES HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_EEHitByFromAnyToPlayerDamage(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!damageResult || !source || m_InediaInfectedAI_Killed) {
			return;
		}
		
		// The correct way to receive damage for further processing (takes into account the impact on HP from other mods).
		if (GetHealth() > 0) {
			m_InediaInfectedAI_HealthDamage = m_InediaInfectedAI_VanillaHealthBeforeEEHitBy - GetHealth();
		} else {
			// Unfortunately, if other mods or vanilla have reduced HP to zero, we cannot determine the impact of other mods, as vanilla HP does not drop below zero.
			// In this case, we negate the impact of other mods and only take the vanilla effect into account.
			m_InediaInfectedAI_HealthDamage = damageResult.GetDamage("", "Health");
		}
		if (GetHealth("Head", "Health") > 0) {
			m_InediaInfectedAI_HeadHealthDamage = m_InediaInfectedAI_VanillaHeadHealthBeforeEEHitBy - GetHealth("Head", "Health");
		} else {
			m_InediaInfectedAI_HeadHealthDamage = damageResult.GetDamage("Head", "Health");
		}
		if (GetHealth("", "Shock") > 0) {
			m_InediaInfectedAI_ShockDamage = m_InediaInfectedAI_VanillaShockBeforeEEHitBy - GetHealth("", "Shock");
		} else {
			m_InediaInfectedAI_ShockDamage = damageResult.GetDamage("", "Shock");
		}
		
		if (ammo == "InediaInfectedAI_ThrowingProjectile") {
			// Damage from thrown projectiles
			InediaInfectedAI_EEHitByFromThrowingProjectileToPlayerDamage(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		} else if (source.IsZombie()) {
			InediaInfectedAI_EEHitByFromZombieToPlayerDamage(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		} else if (source.IsAnimal()) {
			InediaInfectedAI_EEHitByFromAnimalToPlayerDamage(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		} else {
			InediaInfectedAI_EEHitByFromPlayerToPlayerDamage(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		}
	}
	
// DAMAGE FROM ZOMBIES HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_EEHitByFromZombieToPlayerDamage(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!damageResult || !source || !source.IsZombie()) {
			return;
		}
		
		if (InediaInfectedAI_IsNPC()) {
			if (IsUnconscious()) {
				m_InediaInfectedAI_UnconsciousHealthDamageCounter += damageResult.GetDamage("", "Health");
			}
			
			return;
		}

		ZombieBase zmb = ZombieBase.Cast(source);
		if (!zmb || !zmb.m_InediaInfectedAI_ConfigZombie || !zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerHandlerIsActive) {
			if (IsUnconscious()) {
				m_InediaInfectedAI_UnconsciousHealthDamageCounter += damageResult.GetDamage("", "Health");
			}
			
			return;
		}
		
		// Prevent the vanilla game or other mods from killing the player.
		InediaInfectedAI_RemoveFatalDamage();
			
		m_InediaInfectedAI_WeaponMultiplier = m_InediaInfectedAI_ConfigZombie.GetWeaponMultiplierByWeapon(source, ammo, damageType);
		float zmbDamageMultiplier = m_InediaInfectedAI_WeaponMultiplier * zmb.InediaInfectedAI_GetInjuryDamageMultiplier() * zmb.InediaInfectedAI_GetBloodDamageMultiplier() * zmb.InediaInfectedAI_GetScaleDamageMultiplier();
		m_InediaInfectedAI_DamageMultiplier *= zmbDamageMultiplier;
		m_InediaInfectedAI_ShockDamageMultiplier *= zmbDamageMultiplier;

		float staminaDamagePercent = 0;
		float bloodDamagePercent = 0;
		
		float injuryBleedingChanceMultiplier = zmb.InediaInfectedAI_GetInjuryBleedingChanceMultiplier();

		if (!InediaInfectedAI_IsInBlock()) {
			// Normal attack
			if (zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerHealthMultiplier >= 0) {
				m_InediaInfectedAI_DamageMultiplier *= zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerHealthMultiplier;
			}

			if (zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerShockMultiplier >= 0) {
				m_InediaInfectedAI_ShockDamageMultiplier *= zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerShockMultiplier;
			}

			if (zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerStaminaPercent > 0) {
				staminaDamagePercent = zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerStaminaPercent;
			}
			
			if (zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerBloodPercent > 0) {
				bloodDamagePercent = zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerBloodPercent;
			}

			// Bleeding chance
			if (zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerBleedingHandlerIsActive) {
				float noBlockBleedingChance = zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerBleedingChancePercent * injuryBleedingChanceMultiplier * Math.Clamp(damageResult.GetDamage(dmgZone, "Blood") / 100, 0, 1);
				if (noBlockBleedingChance > 0 && Math.RandomFloatInclusive(0, 100) <= noBlockBleedingChance) {
					InediaInfectedAI_AddRandomBleedingSource();
				}
			}
		} else {
			// Attack in block
			if (zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerInBlockHealthMultiplier >= 0) {
				m_InediaInfectedAI_DamageMultiplier *= zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerInBlockHealthMultiplier;
			}

			if (zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerInBlockShockMultiplier >= 0) {
				m_InediaInfectedAI_ShockDamageMultiplier *= zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerInBlockShockMultiplier;
			}

			if (zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerInBlockStaminaPercent > 0) {
				staminaDamagePercent = zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerInBlockStaminaPercent;
			}
			
			if (zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerInBlockBloodPercent > 0) {
				bloodDamagePercent = zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerInBlockBloodPercent;
			}

			// Bleeding chance in block
			if (zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerBleedingHandlerIsActive) {
				float blockBleedingChance = zmb.m_InediaInfectedAI_ConfigZombie.DamageToPlayerInBlockBleedingChancePercent * injuryBleedingChanceMultiplier * Math.Clamp(damageResult.GetDamage(dmgZone, "Blood") / 5, 0, 1);
				if (blockBleedingChance > 0 && Math.RandomFloatInclusive(0, 100) <= blockBleedingChance) {
					InediaInfectedAI_AddRandomBleedingSource();
				}
			}
		}

		// Health damage
		float healthDamage = m_InediaInfectedAI_DamageMultiplier * m_InediaInfectedAI_HealthDamage;
		if (healthDamage > 0 && IsUnconscious()) {
			m_InediaInfectedAI_UnconsciousHealthDamageCounter += healthDamage;
		}
		
		float healthAfterDamage = m_InediaInfectedAI_VanillaHealthBeforeEEHitBy - healthDamage;
		if (healthAfterDamage > 0) {
			SetHealth(healthAfterDamage);
		} else {
			InediaInfectedAI_Kill();
		}

		// Shock damage
		SetHealth("", "Shock", m_InediaInfectedAI_VanillaShockBeforeEEHitBy - m_InediaInfectedAI_ShockDamageMultiplier * m_InediaInfectedAI_ShockDamage);

		// Stamina damage percent
		if (staminaDamagePercent > 0 && m_StaminaHandler) {
			if (staminaDamagePercent > 100) {
				staminaDamagePercent = 100;
			}
			m_StaminaHandler.SetStamina(m_StaminaHandler.GetStamina() - staminaDamagePercent / 100 * m_StaminaHandler.GetStaminaMax());
		}
		
		// Blood damage percent
		if (bloodDamagePercent > 0) {
			DecreaseHealth("GlobalHealth", "Blood", (GetMaxHealth("GlobalHealth", "Blood") - PlayerConstants.BLOOD_THRESHOLD_FATAL) * bloodDamagePercent / 100);
		}
	}
	
// DAMAGE FROM ANIMALS HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_EEHitByFromAnimalToPlayerDamage(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!damageResult || !source || !source.IsAnimal()) {
			return;
		}
		
		if (InediaInfectedAI_IsNPC()) {
			return;
		}

		AnimalBase animal = AnimalBase.Cast(source);
		if (!animal || !animal.m_InediaInfectedAI_ConfigZombie || !animal.m_InediaInfectedAI_ConfigZombie.DamageToPlayerHandlerIsActive) {
			return;
		}
		
		// Prevent the vanilla game or other mods from killing the player.
		InediaInfectedAI_RemoveFatalDamage();
			
		m_InediaInfectedAI_WeaponMultiplier = m_InediaInfectedAI_ConfigZombie.GetWeaponMultiplierByWeapon(source, ammo, damageType);
		m_InediaInfectedAI_DamageMultiplier *= m_InediaInfectedAI_WeaponMultiplier;
		m_InediaInfectedAI_ShockDamageMultiplier *= m_InediaInfectedAI_WeaponMultiplier;

		// Health damage
		float healthAfterDamage = m_InediaInfectedAI_VanillaHealthBeforeEEHitBy - m_InediaInfectedAI_DamageMultiplier * m_InediaInfectedAI_HealthDamage;
		if (healthAfterDamage > 0) {
			SetHealth(healthAfterDamage);
		} else {
			InediaInfectedAI_Kill();
		}

		// Shock damage
		SetHealth("", "Shock", m_InediaInfectedAI_VanillaShockBeforeEEHitBy - m_InediaInfectedAI_ShockDamageMultiplier * m_InediaInfectedAI_ShockDamage);
	}
	
// DAMAGE FROM PLAYERS ATTACKS HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_EEHitByFromPlayerToPlayerDamage(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!damageResult || !source) {
			return;
		}
		
		PlayerBase player = PlayerBase.Cast(source.GetHierarchyRootPlayer());
		if (!player && damageType == DT_EXPLOSION) {
			player = this;
		}
		
		if (!player || !player.m_InediaInfectedAI_ConfigZombie || !player.m_InediaInfectedAI_ConfigZombie.DamageToPlayerHandlerIsActive) {
			return;
		}
		
		// The damage handler for players is disabled for "DayZ-Expansion-AI" bots
		// that have modified damage multipliers in quests (Quests/Objectives/AICamp => DamageReceivedMultiplier != 1 OR DamageMultiplier != 1).
		// If this is not done, "InediaInfectedAI" and "DayZ-Expansion-AI" will control damage simultaneously,
		// causing a conflict that will result in killed bots not being counted in quests "DayZ-Expansion-Quests".
		if (InediaInfectedAI_ExpansionAICompatibility.AllowsDisablingPlayerToPlayerDamageHandler(player, this)) {
			return;
		}
		
		// Prevent the vanilla game or other mods from killing the player.
		InediaInfectedAI_RemoveFatalDamage();
		
		m_InediaInfectedAI_WeaponMultiplier = m_InediaInfectedAI_ConfigZombie.GetWeaponMultiplierByWeapon(source, ammo, damageType);
		m_InediaInfectedAI_DamageMultiplier *= m_InediaInfectedAI_WeaponMultiplier;
		m_InediaInfectedAI_ShockDamageMultiplier *= m_InediaInfectedAI_WeaponMultiplier;

		if (damageType == DT_CLOSE_COMBAT) {
			float painArmsDamageMultiplier = player.InediaInfectedAI_GetPainManager().GetPainArmsDamageMultiplier();
			m_InediaInfectedAI_DamageMultiplier *= painArmsDamageMultiplier;
			m_InediaInfectedAI_DamageMultiplier *= InediaInfectedAI_SyberiaCompatibility.GetSyberiaMeleeDamageMultiplierToAllCreatures(player);
			m_InediaInfectedAI_ShockDamageMultiplier *= painArmsDamageMultiplier;
		}
		
		// Head health damage
		if (dmgZone == "Head") {
			float headHealthAfterDamage = m_InediaInfectedAI_VanillaHeadHealthBeforeEEHitBy - m_InediaInfectedAI_HeadHealthDamage * m_InediaInfectedAI_DamageMultiplier;
			if (headHealthAfterDamage > 0) {
				SetHealth("Head", "Health", headHealthAfterDamage);
			} else {
				InediaInfectedAI_Kill();
			}
		}
		
		// Health damage
		if (!m_InediaInfectedAI_Killed) {
			float healthAfterDamage = m_InediaInfectedAI_VanillaHealthBeforeEEHitBy - m_InediaInfectedAI_HealthDamage * m_InediaInfectedAI_DamageMultiplier;
			if (healthAfterDamage > 0) {
				SetHealth(healthAfterDamage);
			} else {
				InediaInfectedAI_Kill();
			}
		}

		// Shock damage
		SetHealth("", "Shock", m_InediaInfectedAI_VanillaShockBeforeEEHitBy - m_InediaInfectedAI_ShockDamage * m_InediaInfectedAI_ShockDamageMultiplier);
	}

// STUN FROM ZOMBIES HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_EEHitByFromZombieToPlayerStun(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!IsAlive() || !damageResult || !source || !source.IsZombie() || damageType != DT_CLOSE_COMBAT) {
			return;
		}

		ZombieBase zmb = ZombieBase.Cast(source);
		if (!zmb || !zmb.m_InediaInfectedAI_ConfigZombie) {
			return;
		}

		float injuryStunChanceMultiplier = zmb.InediaInfectedAI_GetInjuryStunChanceMultiplier();

		if (!InediaInfectedAI_IsInBlock()) {
			// Stun chance
			float stunChance = zmb.m_InediaInfectedAI_ConfigZombie.StunToPlayerChancePercent * injuryStunChanceMultiplier;
			if (stunChance > 0 && Math.RandomFloatInclusive(0, 100) <= stunChance) {
				InediaInfectedAI_Stun(zmb.GetPosition());
			}
		} else {
			// Stun chance in block
			float stunChanceBlock = zmb.m_InediaInfectedAI_ConfigZombie.StunToPlayerInBlockChancePercent * injuryStunChanceMultiplier;
			if (stunChanceBlock > 0 && Math.RandomFloatInclusive(0, 100) <= stunChanceBlock) {
				InediaInfectedAI_Stun(zmb.GetPosition());
			}
		}
	}
	
// DISARM FROM ENITITES HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_EEHitByFromAnyToPlayerDisarm(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!IsAlive() || !damageResult || !source || (!source.IsZombie() && !source.IsAnimal() && !source.IsInherited(PlayerBase)) || damageType != DT_CLOSE_COMBAT) {
			return;
		}
		
		InediaInfectedAI_ConfigZombie config;

		if (source.IsInherited(ZombieBase)) {
			ZombieBase zmb = ZombieBase.Cast(source);
			if (zmb && zmb.m_InediaInfectedAI_ConfigZombie) {
				config = zmb.m_InediaInfectedAI_ConfigZombie;
			}
		} else if (source.IsInherited(AnimalBase)) {
			AnimalBase animal = AnimalBase.Cast(source);
			if (animal && animal.m_InediaInfectedAI_ConfigZombie) {
				config = animal.m_InediaInfectedAI_ConfigZombie;
			}
		} else if (source.IsInherited(PlayerBase)) {
			PlayerBase player = PlayerBase.Cast(source);
			if (player && player.m_InediaInfectedAI_ConfigZombie) {
				config = player.m_InediaInfectedAI_ConfigZombie;
			}
		}

		if (!config) {
			return;
		}
		
		if (!InediaInfectedAI_IsInBlock() && config.DisarmToPlayerChancePercent > 0 && Math.RandomFloatInclusive(0, 100) <= config.DisarmToPlayerChancePercent) {
			InediaInfectedAI_KnockItemOutOfHands();
		} else if (InediaInfectedAI_IsInBlock() && config.DisarmToPlayerInBlockChancePercent > 0 && Math.RandomFloatInclusive(0, 100) <= config.DisarmToPlayerInBlockChancePercent) {
			InediaInfectedAI_KnockItemOutOfHands();
		}
	}

// DISEASES FROM ENITITES HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_EEHitByFromAnyToPlayerDisease(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!IsAlive() || !damageResult || !source || (!source.IsZombie() && !source.IsAnimal()) || damageType != DT_CLOSE_COMBAT) {
			return;
		}
		
		InediaInfectedAI_ConfigZombie config;

		if (source.IsInherited(ZombieBase)) {
			ZombieBase zmb = ZombieBase.Cast(source);
			if (zmb && zmb.m_InediaInfectedAI_ConfigZombie) {
				config = zmb.m_InediaInfectedAI_ConfigZombie;
			}
		} else if (source.IsInherited(AnimalBase)) {
			AnimalBase animal = AnimalBase.Cast(source);
			if (animal && animal.m_InediaInfectedAI_ConfigZombie) {
				config = animal.m_InediaInfectedAI_ConfigZombie;
			}
		}

		if (!config) {
			return;
		}
		
		if (!config.DiseasesToPlayerHandlerIsActive || !config.DiseasesToPlayerAgents || config.DiseasesToPlayerAgents.Count() == 0) {
			return;
		}
		
		bool isVanillaBleedingActive = GetBleedingSourceCount() > 0;
		
		bool isDeepWoundBleedingActive = false;
		string inediaPainLimb = InediaInfectedAI_GetPainManager().GetInediaPainLimbFromDmgZone(dmgZone);
		if (inediaPainLimb != "") {
			if (InediaInfectedAI_GetPainManager().IsLimbDeepWounded(inediaPainLimb)) {
				if (!InediaInfectedAI_GetPainManager().IsLimbDeepWoundBandaged(inediaPainLimb) || !InediaInfectedAI_GetPainManager().IsLimbDeepWoundBandageEffective(inediaPainLimb)) {
					isDeepWoundBleedingActive = true;
				}
			}
		}
		
		bool isNPC = InediaInfectedAI_IsNPC();
		
		foreach (InediaInfectedAI_ConfigZombieDeseaseAgent agent: config.DiseasesToPlayerAgents) {
			if (agent.AddOnlyIfBleeding && !isVanillaBleedingActive && !isDeepWoundBleedingActive) {
				continue;
			}
			
			if (agent.AddOnlyToPlayers && isNPC) {
				continue;
			}
			
			if (GetSingleAgentCount(agent.AgentId) >= agent.AddAmountNoMoreThan) {
				continue;
			}
			
			if (!InediaInfectedAI_IsInBlock() && agent.AddChance > 0 && Math.RandomFloatInclusive(0, 100) <= agent.AddChance) {
				InsertAgent(agent.AgentId, agent.AddAmount);
			} else if (InediaInfectedAI_IsInBlock() && agent.AddChanceInBlock > 0 && Math.RandomFloatInclusive(0, 100) <= agent.AddChanceInBlock) {
				InsertAgent(agent.AgentId, agent.AddAmountInBlock);
			}
		}
	}

// CAMERA SHAKE FROM ZOMBIES HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_EEHitByFromZombieToPlayerCameraShake(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!IsAlive() || !damageResult || !source || !source.IsZombie() || damageType != DT_CLOSE_COMBAT || InediaInfectedAI_IsNPC()) {
			return;
		}

		ZombieBase zmb = ZombieBase.Cast(source);
		if (!zmb || !zmb.m_InediaInfectedAI_ConfigZombie || zmb.m_InediaInfectedAI_ConfigZombie.CameraShakeToPlayerIntensity <= 0) {
			return;
		}

		float intensity = zmb.m_InediaInfectedAI_ConfigZombie.CameraShakeToPlayerIntensity * zmb.InediaInfectedAI_GetInjuryCameraShakeIntensityMultiplier();

		InediaInfectedAI_CameraShake(0.3, intensity);
	}

// DAMAGE FROM PROJECTILES HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_EEHitByFromThrowingProjectileToPlayerDamage(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (ammo != "InediaInfectedAI_ThrowingProjectile" || !damageResult || !source || !source.IsInherited(ItemBase)) {
			return;
		}
		
		ItemBase projectile = ItemBase.Cast(source);
		if (!projectile || !projectile.InediaInfectedAI_IsThrowingProjectile()) {
			return;
		}
		
		if (projectile.m_InediaInfectedAI_ThrowingProjectile.m_ThrowerEntity) {
			m_InediaInfectedAI_EEHitByLastSource = projectile.m_InediaInfectedAI_ThrowingProjectile.m_ThrowerEntity;
		}
		
		// Prevent the vanilla game or other mods from killing the player.
		InediaInfectedAI_RemoveFatalDamage();
		
		// Retrieve the damage that was previously dealt to the infected by other mods.
		float healthDamageFromOtherMods = m_InediaInfectedAI_HealthDamage - damageResult.GetDamage("", "Health");
		float shockDamageFromOtherMods = m_InediaInfectedAI_ShockDamage - damageResult.GetDamage("", "Shock");

		// Armor multipliers
		float healthArmorMultiplier = damageResult.GetDamage(dmgZone, "Health") / ItemBase.InediaInfectedAI_ThrowingProjectileGetAmmoHealthDamage();
		float shockArmorMultiplier = damageResult.GetDamage(dmgZone, "Shock") / ItemBase.InediaInfectedAI_ThrowingProjectileGetAmmoShockDamage();
		float bloodArmorMultiplier = damageResult.GetDamage(dmgZone, "Blood") / ItemBase.InediaInfectedAI_ThrowingProjectileGetAmmoBloodDamage();

		// Health damage
		m_InediaInfectedAI_HealthDamage = healthArmorMultiplier * projectile.InediaInfectedAI_ThrowingProjectileGetHealthDamage() + healthDamageFromOtherMods;
		
		float healthAfterDamage = m_InediaInfectedAI_VanillaHealthBeforeEEHitBy - m_InediaInfectedAI_HealthDamage;
		if (healthAfterDamage > 0) {
			SetHealth(healthAfterDamage);
		} else {
			InediaInfectedAI_Kill();
		}

		// Shock damage
		m_InediaInfectedAI_ShockDamage = shockArmorMultiplier * projectile.InediaInfectedAI_ThrowingProjectileGetShockDamage() + shockDamageFromOtherMods;
		SetHealth("", "Shock", m_InediaInfectedAI_VanillaShockBeforeEEHitBy - m_InediaInfectedAI_ShockDamage);

		// Stamina damage percent
		float staminaDamagePercent = shockArmorMultiplier * projectile.InediaInfectedAI_ThrowingProjectileGetStaminaDamage();
		if (staminaDamagePercent > 0 && m_StaminaHandler) {
			if (staminaDamagePercent > 100) {
				staminaDamagePercent = 100;
			}
			m_StaminaHandler.SetStamina(m_StaminaHandler.GetStamina() - staminaDamagePercent / 100 * m_StaminaHandler.GetStaminaMax());
		}
		
		// Blood damage percent
		float bloodDamagePercent = shockArmorMultiplier * projectile.InediaInfectedAI_ThrowingProjectileGetBloodDamage();
		if (bloodDamagePercent > 0) {
			DecreaseHealth("GlobalHealth", "Blood", (GetMaxHealth("GlobalHealth", "Blood") - PlayerConstants.BLOOD_THRESHOLD_FATAL) * bloodDamagePercent / 100);
		}

		// Stun chance
		float stunChance = shockArmorMultiplier * projectile.InediaInfectedAI_ThrowingProjectileGetStunChancePercent();
		if (stunChance > 0 && Math.RandomFloatInclusive(0, 100) <= stunChance) {
			// The "-10" trick is required because the contact event is not always triggered on the surface of the body due to the velocity of the projectile and the frame rate
		    InediaInfectedAI_Stun(projectile.GetPosition() - 10 * GetVelocity(projectile).Normalized());
		}

		// Bleeding chance
		float bleedingChance = bloodArmorMultiplier * projectile.InediaInfectedAI_ThrowingProjectileGetBleedingChancePercent();
		if (bleedingChance > 0 && Math.RandomFloatInclusive(0, 100) <= bleedingChance) {
			InediaInfectedAI_AddRandomBleedingSource();
		}
	}
	
// CAR COLLISIONS HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_EEHitByToCarDamage(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!IsAlive() || !damageResult || !source || !m_InediaInfectedAI_ConfigZombie.AttackCarHandlerIsActive || !m_InediaInfectedAI_ConfigZombie.AttackCarElementsByCollisionsIsActive || m_InediaInfectedAI_ConfigZombie.AttackCarElementsByCollisionsGlobalDamageMultiplier <= 0) {
			return;
		}

		if (ammo == "TransportHit") {
			CarScript car = CarScript.Cast(source);
			if (!car) {
				return;
			}

			float carSpeedKmH = GetVelocity(car).Length() * 3.6;
			if (carSpeedKmH >= car.m_InediaInfectedAI_CollisionsDamageSpeedThresholdKmH) {
				car.InediaInfectedAI_AttackedByEntityViaCollision(this, damageResult.GetDamage("", "Health"));
			}
		}
	}

	void InediaInfectedAI_EEHitByCarCollisionActions(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!IsAlive() || !source) {
			return;
		}
		
		if (ammo == "TransportHit") {
			CarScript car = CarScript.Cast(source);
			if (!car) {
				return;
			}

			// Reducing car speed after collision with this zombie
			car.InediaInfectedAI_ReduceSpeedAfterCollision(this);

			// Play collision sound
			car.InediaInfectedAI_PlayCarCollisionSound(this);
		}
	}
	
// REVERSE PAIN HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_EEHitByToTestPlayerReversePain(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!damageResult || !source || !InediaInfectedAI_GetPainManager().PainSystemIsActive()) {
			return;
		}
		
		if (m_InediaInfectedAI_IsTestPlayer && m_InediaInfectedAI_TestPlayerReversePainIsActive) {
	        PlayerBase reversePlayer;
	        reversePlayer = PlayerBase.Cast(source.GetHierarchyRootPlayer());
			
			if (!reversePlayer) {
				if (source.IsInherited(ItemBase)) {
					ItemBase reverseProjectileItem = ItemBase.Cast(source);
					if (reverseProjectileItem && reverseProjectileItem.InediaInfectedAI_IsThrowingProjectile()) {
						reversePlayer = PlayerBase.Cast(reverseProjectileItem.m_InediaInfectedAI_ThrowingProjectile.m_ThrowerEntity);
					}
				} else if (source.IsInherited(CarScript)) {
					CarScript vehicle = CarScript.Cast(source);
					if (vehicle) {
						reversePlayer = vehicle.InediaInfectedAI_GetFirstCrewMember();
					}
				}
			}

	        if (reversePlayer && !reversePlayer.m_InediaInfectedAI_IsTestPlayer && reversePlayer.GetAllowDamage() && reversePlayer.IsAlive()) {
				reversePlayer.m_InediaInfectedAI_WeaponMultiplier = m_InediaInfectedAI_WeaponMultiplier;
				reversePlayer.m_InediaInfectedAI_DamageMultiplier = m_InediaInfectedAI_DamageMultiplier;
				reversePlayer.m_InediaInfectedAI_ShockDamageMultiplier = m_InediaInfectedAI_ShockDamageMultiplier;
				
				reversePlayer.m_InediaInfectedAI_EEHitByBleedingSourceCountBeforeHit = reversePlayer.GetBleedingSourceCount();
				if ((GetBleedingSourceCount() - m_InediaInfectedAI_EEHitByBleedingSourceCountBeforeHit) > 0) {
					reversePlayer.m_InediaInfectedAI_EEHitByBleedingSourceCountBeforeHit -= 1;
				}
				
	            reversePlayer.InediaInfectedAI_GetPainManager().EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
				
	        }
		}
	}
	
// RELATIONSHIPS WITH ENTITIES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override bool CanBeTargetedByAI(EntityAI ai)
	{
#ifdef SERVER
		if (!IsAlive() || IsInVehicle() || !ai) {
			return super.CanBeTargetedByAI(ai);
		}

		if (ai.IsInherited(ZombieBase)) {
			ZombieBase zmb = ZombieBase.Cast(ai);
			if (zmb) {
				zmb.InediaInfectedAI_InitOnceServer();
				
				// If it is a zombie-friendly NPC or player (admin) - do not attack
				if (zmb.m_InediaInfectedAI_ConfigZombie) {
					if (zmb.InediaInfectedAI_IsFriendly(this)) {
						return false;
					}
	
					if (zmb.m_InediaInfectedAI_ConfigZombie.AttackPlayersUnconsciousHandlerIsActive && IsUnconscious()) {
	                    return zmb.InediaInfectedAI_CanAttackUnconsciousPlayer(this);
	                }
				}
			}
		} else if (ai.IsInherited(AnimalBase)) {
			#ifdef Dayz_Dog
			if (InediaInfectedAI_Functions.IsKindOfCached("Dayz_Doggo", ai)) {
				return super.CanBeTargetedByAI(ai);
			}
			#endif
			
			AnimalBase animal = AnimalBase.Cast(ai);
			if (animal) {
				animal.InediaInfectedAI_InitOnceServer();
				
				// If it is a animal-friendly NPC or player (admin) - do not attack
				if (animal.m_InediaInfectedAI_ConfigZombie && animal.InediaInfectedAI_IsFriendly(this)) {
					return false;
				}
			}
		}
#endif

		return super.CanBeTargetedByAI(ai);
	}
	

// TIMERS HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_TimersHandler(float pDt)
	{
		//
	}
	
// STAMINA HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_StaminaHandler(float pDt)
	{
		if (m_InediaInfectedAI_DisableStaminaRegenForSeconds > 0) {
			m_InediaInfectedAI_DisableStaminaRegenForSeconds -= pDt;
			
			if (m_StaminaHandler && m_StaminaHandler.GetStamina() > 0) {
				m_StaminaHandler.SetStamina(0);
			}
		}		
	}

// SMELLS HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	float m_InediaInfectedAI_SmellTracesTimer = 0;
	void InediaInfectedAI_SmellsHandler(float pDt)
	{
		m_InediaInfectedAI_SmellTracesTimer -= pDt;
		if (m_InediaInfectedAI_SmellTracesTimer > 0) return;
		m_InediaInfectedAI_SmellTracesTimer = 10;

		if (!m_InediaInfectedAI_ConfigPlayers.SmellsHandlerIsActive || IsUnconscious() || MiscGameplayFunctions.InediaInfectedAI_IsUnderRoofBuilding(GetPosition() + vector.Up * 0.1)) {
			return;
		}

		m_InediaInfectedAI_SmellsInstance.AddSmell(this);
	}
	
	float InediaInfectedAI_GetSmellRadius()
	{
		if (!m_InediaInfectedAI_ConfigPlayers.SmellsHandlerIsActive) return 0;
		
		float smellRadius = m_InediaInfectedAI_ConfigPlayers.SmellRadiusDefault;
		int woundStatus = 0;
		
		if (HasBloodyHands() || GetBleedingSourceCount() > 0) {
			woundStatus = 2;
		} else {
			foreach (string limb: InediaInfectedAI_GetPainManager().m_InediaPainLimbs) {
				if (InediaInfectedAI_GetPainManager().IsLimbDeepWounded(limb)) {
					if (InediaInfectedAI_GetPainManager().IsLimbDeepWoundBandaged(limb) && InediaInfectedAI_GetPainManager().IsLimbDeepWoundBandageEffective(limb)) {
						woundStatus = 1;
					} else {
						woundStatus = 2;
						break;
					}
				}
			}
		}
		
		if (woundStatus == 1) {
			smellRadius = m_InediaInfectedAI_ConfigPlayers.SmellRadiusBandaged;
		} else if (woundStatus == 2) {
			smellRadius = m_InediaInfectedAI_ConfigPlayers.SmellRadiusWounded;
		}
		
		smellRadius = Math.Clamp(InediaInfectedAI_Restrictions.SmellsSystem_ModifyPlayerSmellRadius(this, woundStatus, smellRadius), 0, 100);
		
		return smellRadius;
	}

// FOOTSTEPS NOISE HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	float m_InediaInfectedAI_FootstepsNoiseHandlerTimer = 0;
	void InediaInfectedAI_FootstepsNoiseHandler(float pDt)
	{
		m_InediaInfectedAI_FootstepsNoiseHandlerTimer -= pDt;
		if (m_InediaInfectedAI_FootstepsNoiseHandlerTimer > 0) return;
		m_InediaInfectedAI_FootstepsNoiseHandlerTimer = 5;

		if (!m_InediaInfectedAI_ConfigPlayers.FootstepsNoiseHandlerIsActive || IsUnconscious() || GetCommand_Ladder()) {
			return;
		}
		
		if (InediaInfectedAI_DayzHorseCompatibility.GetPlayerHorseSpeedKmH(this) > 5) {
			InediaInfectedAI_IrritationActions.HorseHoofstepNoiseIrritation(this);
			
			return;
		}

		GetMovementState(m_MovementState);
		if (!m_MovementState) {
			return;
		}

		InediaInfectedAI_Irritants irritant;
		switch (m_MovementState.m_iMovement)
		{
			case DayZPlayerConstants.MOVEMENTIDX_RUN:
				// JOG
				if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT || m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT) {
					InediaInfectedAI_IrritationActions.PlayerFootstepNoiseIrritation(InediaInfectedAI_Irritants.PLAYER_FOOTSTEPS_JOG_NOISE, this);
				}
			break;

			case DayZPlayerConstants.MOVEMENTIDX_SPRINT:
				if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT || m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT) {
					// SPRINT
					InediaInfectedAI_IrritationActions.PlayerFootstepNoiseIrritation(InediaInfectedAI_Irritants.PLAYER_FOOTSTEPS_SPRINT_NOISE, this);
				} else if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH) {
					// CROUCH SPRINT
					InediaInfectedAI_IrritationActions.PlayerFootstepNoiseIrritation(InediaInfectedAI_Irritants.PLAYER_FOOTSTEPS_CROUCH_SPRINT_NOISE, this);
				}
			break;
		}
	}
	
// VANILLA NOISE HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override void AddNoise(NoiseParams noisePar, float noiseMultiplier = 1.0)
	{
		if (!m_InediaInfectedAI_ConfigPlayers) {
			return;
		}

#ifdef SERVER
		GetMovementState(m_MovementState);
		if (GetCommand_Ladder()) {
			// Player on ladder
			if (!GetGame().GetWorld().IsNight()) {
				if (m_InediaInfectedAI_ConfigPlayers.NoiseMultiplierLadderDay >= 0) {
					noiseMultiplier *= m_InediaInfectedAI_ConfigPlayers.NoiseMultiplierLadderDay;
				}
			} else {
				if (m_InediaInfectedAI_ConfigPlayers.NoiseMultiplierLadderNight >= 0) {
					noiseMultiplier *= m_InediaInfectedAI_ConfigPlayers.NoiseMultiplierLadderNight;
				}
			}
		} else if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH || m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_WALK) {
			// Crouch stance
			if (!GetGame().GetWorld().IsNight()) {
				if (m_InediaInfectedAI_ConfigPlayers.NoiseMultiplierCrouchDay >= 0) {
					noiseMultiplier *= m_InediaInfectedAI_ConfigPlayers.NoiseMultiplierCrouchDay;
				}
			} else {
				if (m_InediaInfectedAI_ConfigPlayers.NoiseMultiplierCrouchNight >= 0) {
					noiseMultiplier *= m_InediaInfectedAI_ConfigPlayers.NoiseMultiplierCrouchNight;
				}
			}
		} else {
			// Normal stance
			if (!GetGame().GetWorld().IsNight()) {
				if (m_InediaInfectedAI_ConfigPlayers.NoiseMultiplierDay >= 0) {
					noiseMultiplier *= m_InediaInfectedAI_ConfigPlayers.NoiseMultiplierDay;
				}
			} else {
				if (m_InediaInfectedAI_ConfigPlayers.NoiseMultiplierNight >= 0) {
					noiseMultiplier *= m_InediaInfectedAI_ConfigPlayers.NoiseMultiplierNight;
				}
			}
		}

		noiseMultiplier *= InediaInfectedAI_GetCamoClothingMultiplier();
#endif

		super.AddNoise(noisePar, noiseMultiplier);
	}

// UNCONSCIOUS HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	float m_InediaInfectedAI_UnconsciousHandlerTimer = 0;
	void InediaInfectedAI_UnconsciousHandler(float pDt)
	{
		m_InediaInfectedAI_UnconsciousHandlerTimer -= pDt;
		if (m_InediaInfectedAI_UnconsciousHandlerTimer > 0) return;
		m_InediaInfectedAI_UnconsciousHandlerTimer = 5;

		if (IsUnconscious()) {
			// If there are other live conscious players nearby - the search mode disable beacon should not work to prevent abuses
			array<PlayerBase> nearPlayers = new array<PlayerBase>;
			MiscGameplayFunctions.InediaInfectedAI_GetPlayersAtPosition3D(GetPosition(), 30, nearPlayers);
		    foreach (PlayerBase nearPlayer : nearPlayers) {
				if (nearPlayer != this && nearPlayer.IsAlive() && !nearPlayer.IsUnconscious()) {
					return;
				}
		    }

			// Switch to vanilla mode for nearby infected that have unconscious player attack disabled
			array<ZombieBase> nearZombies = new array<ZombieBase>;
			MiscGameplayFunctions.InediaInfectedAI_GetZombiesAtPosition3D(GetPosition(), 50, nearZombies);
		    foreach (ZombieBase zmb : nearZombies) {
				if (zmb.m_InediaInfectedAI_ConfigZombie && !zmb.InediaInfectedAI_CanAttackUnconsciousPlayer(this)) {
					zmb.InediaInfectedAI_DisableSmellsHandlerForSeconds(180);
					
					if (zmb.m_InediaInfectedAI_ConfigZombie.ReturnToSpawnHandlerIsActive) {
						zmb.InediaInfectedAI_StopSearchModeByBurstingSphere();
					} else {
						zmb.InediaInfectedAI_StopSearchModeForSeconds(6);
					}
					
					if (this == zmb.InediaInfectedAI_GetLongVisionTarget()) {
						zmb.InediaInfectedAI_SetLongVisionTarget(null);
					}
					
					zmb.InediaInfectedAI_UnsetLastIrritationTime();
				}
		    }
		}
	}
	
// FALL ACTIONS HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
	override bool OnLand(int pCurrentCommandID, FallDamageData fallDamageData)
	{		
#ifdef SERVER
		if (fallDamageData) {
			InediaInfectedAI_SetLastFallHeight(fallDamageData.m_Height);
			InediaInfectedAI_IrritationActions.FallingIrritation(this);
		}
#endif

		return super.OnLand(pCurrentCommandID, fallDamageData);
	}
	
	float m_InediaInfectedAI_ClimbFallHeightStart;
	override bool CanClimb(int climbType, SHumanCommandClimbResult climbRes)
	{
		if (climbRes) {
			m_InediaInfectedAI_ClimbFallHeightStart = GetPosition()[1] + climbRes.m_fClimbHeight;
			
			
			if (InediaInfectedAI_GetPainManager().PainSystemIsActive()) {
				if (climbRes.m_fClimbHeight > 1.5 && (InediaInfectedAI_GetPainManager().IsLimbBroken("arms") || InediaInfectedAI_GetPainManager().GetBulletCountInLimb("arms") > 0 || InediaInfectedAI_GetPainManager().GetPainWithBuffsLevelForLimb("arms") >= InediaInfectedAI_LimbPainLevels.BADLY)) {
					// If a player has a broken arm, they will not be able to jump over obstacles higher than 1.5 meters.
#ifdef SERVER
					InediaInfectedAI_PlayPainSoundServer(5);
#endif
					
					return false;
				} else if (climbRes.m_fClimbHeight > 1 && (InediaInfectedAI_GetPainManager().IsLimbBroken("legs") || InediaInfectedAI_GetPainManager().GetBulletCountInLimb("legs") > 0 || InediaInfectedAI_GetPainManager().GetPainWithBuffsLevelForLimb("legs") >= InediaInfectedAI_LimbPainLevels.BADLY)) {
					// A player with injured legs should make a sound of pain if they are unable to jump over an obstacle.
#ifdef SERVER
					InediaInfectedAI_PlayPainSoundServer(5);
#endif
					return false;
				}
			}
		}

		return super.CanClimb(climbType,climbRes);
	}

	override void OnCommandClimbFinish()
	{
#ifdef SERVER
		InediaInfectedAI_SetLastFallHeight(m_InediaInfectedAI_ClimbFallHeightStart - GetPosition()[1]);
		InediaInfectedAI_IrritationActions.FallingIrritation(this);
#endif
		
		super.OnCommandClimbFinish();
	}
	
	protected float m_InediaInfectedAI_LastFallHeight = 0;
	void InediaInfectedAI_SetLastFallHeight(float height)
	{
		m_InediaInfectedAI_LastFallHeight = height;
	}
	
	float InediaInfectedAI_GetLastFallHeight()
	{
		return m_InediaInfectedAI_LastFallHeight;
	}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::-======-----=======-:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::-===-:.                 .::-====-::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::-==-:                              .:====::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::-==:                                     ..:-==-::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::.-+-    .....    .                             ::.:==::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::.-+:           .::: ...                            .:..==::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::+-                 ::: .::                           .. .+-::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::-+                     .-:  :.          .:...           .-  :+:::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::=+    -.           .:--.   -: .-           .-.             -  .*-:::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::-+    :=         .....:=%#=...:. :..                ....     -   +=::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::*     =.                 =##:-..:   ..:::.              --    -   +=:::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::=-   .+.               :-=--%%::-::..     .--:   .::.           .   *-::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::#   .=             ..::...=*%%% -:.- .       .-:    :--=-:..   ...  .*::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::*    .                .-*%%%%%#-= :. ---:.     .-: ..   .:----::...  -=:::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::-+                     .::-%%%%+. . ..    :-:::   :-  :::   ..---::..  *..:::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::-=              :         .*%%:       .:..::..:--=:.-.   .:::. ..   .  -=.:::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::-=       :.     -:  =     .#%=    ::::.     :--+:.-*--: :.   .-. .:-:   +::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::*        *+ .  =..:+ .   :%%   .+.  ::-::   .:=:.  =*--.-*+-. ...  .  .-*:::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::*::::.    *#:: + .#-  =.+%#.. .+  =%%%%%%#-         .==. +=  :          -+::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::.-+==----=: #%* #.*%:  +%%*::: = .==*%%%##%%#-     :  -=: .# .*  .=-.     :+:::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::+=-#%%%#-:==%%#%=%%=-#%%= .:.-  *-%#-:+*#%%%%#:  .. :-+ ..#..=:  -*#%#*=. -=::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::+-#%%*+=#%=:#*#%%%%%%%%+:  .= + .%%#=++=-=+#%%%%*==::=%*--+#*. +**%%%%#*%%-=-::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::+=%%+::::*%.+= :=+%%*=..   =:=*=*%%%#.. - .-#%%%%%%#%%*+..:=%%#%%%%**##*#%*::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::+-%#:--==+%=-: .:-===:....:-+*++=####++:.:-++%%%%%%%%%#=-:::*%%%%+--:-##*#:::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::*=%=+==+#%-:.::::......  .:   #+:-**###%%##*#*+#%%%#-:::-=-=*%%%+:..:***=.::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::++-+-%%%+==-::::::::::-   .+=-%+=:+*=+*#%%#=*%#++#. :++*+=+-%%#%%#*++*+-.::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::+::-=%%*==-.:=+*=.   .:   ..:+--+++#%%%%##*=-:=-:+.=#%%=**+:-#%%#*++%.+:::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::+::--*#++%+ ..:=#=   ..::.. ..--##*##+-: .---. =+:**#*+=#%%*.:#%%#%*:-=::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::=-. :-+=.%:-....-+-.   .--:-:..:...+ .-=-.::==*-:+=- .:-=%*%=.=+%+#=:=::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::=+:.. .%# --:=*=:-===.  ::-----=+::--+*=-+.%%+#%%%#. :+%*%%+.::=-+=:::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::-=+#%%%-.+*-*%+%*+-**+-..:-:  .++*=:.:+  #%%%%%%#:+%+*#-*+ +-..==:::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::.=%#%%# :#=*%:=%#%*--=*#*--*.+=-...-#.---::%#*=.=##*++.-=.:-=-+::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::-#:#%%.-.**%-:%-#%%--..:##-*.:: .:+--.  -::+- +*+-*=-.--:+=:::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::-* :%%-.-:%%*.%=-%%%-+...*%:*   --:+  .-  =- :-==.*+-:*=+:.:::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::=:= -%= -.-%+-%*.%%%%:+.  #+-+ ::..:  -  .=  :.-:.=++*=+:.::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::+ =.:%+  =::*:##.%%%%=*.  ++-* -::: .:-=--:..::::..+*=+:::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::+  =:%=  -=..##=:*%%#-+-. +-:%..==*##**=+:####%##*##**::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::-=:.* %=  .=+:=%+#*%*#-*=. *.:* =#+*-- =-.%+=:.+:*-+==-::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::.*:-:+.#-   +*. #=*%%++-#+..=.=  #%%%%+=+-.%.:..-.*++-+:.:::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::-======-+--=---*-   .-  -#*#*#++*= +..  -%%#*#%##%#%%%%%%*#..==-.:::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::==.:+**###+ *+=.-=%*=: .   :+#*+**+::+:= =%%***####*##*%%%###::*+:::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::==. .#%%%%%* =-*=. :+%%%%#=.  ..=*###.-.:- +#**#*-=+:#.+:++-=*:# ==:::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::=+.  .#%%%%%#..* *=.   *:*%%#*=-.   .=%-.-=...-+*=+###*#+#***##%%+=*-:::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::+-=   +%%%%%* .*: +=:   *  =%%#%*+=-.  +::.-..-: #=--=---::::-==---#-::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::.-+. -.  *%%%%* .++  ==:   *. =%%%%%%%#*+..-:  . =  =++=-+**#%*+-:.==:+=::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::.-+:    =. :%%%*  +%...==:   =.=%%%%%#+%%%%#:..  . ::-+=:-=:==+*+----=.+++::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::-=-      ::  -%%=.+%+.: =-:  .:.+%%%#%%.-#%%%%*= .+- ::.:=*--=+=*==-=*:=-==::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::.:+  .:     =.  .#*-%%:.=-+-:  :=.+%%%#+%.:*%%%%%%%=     .=:-.-+:*.=.  -*:.::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::=+-  .+:    =    =%%*..+*==.  +::=%%++-*.%%%%%%%%##%= .-:  - :.=.  : . *:.::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::==-  =:   =+.   =    .#-.-+-==.  +:.+#%--:=.%%%%+: :*%%%#=.  :  :-:..    -=::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::-==.  .  =. . .*-   =.    -=:.:-+  :=: =#+-.:-.%%*. :#%%%%+:.=+===+++====*=+-:::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::-=-:     =   =:.-  =*.  -:    .=-:=*  -=. :#--.-..%=  +%%%%%%#+:......::::::+-:::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::-==:        # .:-=::-  .*.  =:     :***  -* .-+=:.::=: :%%%%*%%%%%%-....:::::::+=:::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::.:===:          -#-.   -..-   -   +       -*. -*.:+.%. .#  :%%%+=%%%%%%%#+:..:::::::+==::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::.:-=+==:            -==    .::+..-  .   .=        =====.* *= +: .%%*:+%%%%*%%%%-==::::::::=--::::::::::::::::::::::::::::::::::::::::::::::::::::
// .:-++++-:             :--.   :-+:   +..=      -          =++:+.:#+:  ###.*#%%%%=:%%%%.:*-:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// =-:.               :::   :::: :=-    +..=     :.           =**: %:- ==%==+%%%%%= .*%%#  ++==-:::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//                .:::   .--.            +. -     .             -+.#:+ *-%:-%%%%%%=   :*%+  =- :-====-:::::::::::::::::::::::::::::::::::::::::::::::::::
//     ....     ::.    :::                +: -.                   -*==+:#-:*%%%%%%=     :#-  :#-..   :-==+:::::::::::::::::::::::::::::::::::::::::::::::
// ...     .::-:   .::.                    =- ::             -.     :=**-=.#%#+%%%*    .  -=.  +.        :#=:::::::::::::::::::::::::::::::::::::::::::::
//   :----.    .:-:.                        -= :-        ..:=         .**+:#*=:%%%*     =+=-==  --        +.-=-::::::::::::::::::::::::::::::::::::::::::
//        :--.   ...                      .. .+ .-        :.-         =*.#+-=:.*%%*-.:.*%%%%#=+. .=.      +   :*==-::::::::::::::::::::::::::::::::::::::
//           :-:    :.                  .. .:  =. -.        .        -%: .+=::.=%%*+::*%%%%%%%++: .+:.:   -:    : :+=::::::::::::::::::::::::::::::::::::
//             .--   .:.                     :. =: ::               =%:    .-=::%%+*.*%%%%%#==%#==  :=+    -        #::::::::::::::::::::::::::::::::::::
//                --   .:.                    .. -- .-             =%*-       -=%%%:#%%%%%%:. .#:.=:  *    -       .+=:::::::::::::::::::::::::::::::::::
//                  =:   ::                     . := .-           =%%-=#-       :=-=%%%%%%#   .-   .-.-:   :.      : =+::::::::::::::::::::::::::::::::::
//                   .-.   :.                    . .=  -.        =%%%%::#%*-.    *#=+%+#%%=  .-    . :=:    :     .- -:+:::::::::::::::::::::::::::::::::
//                     :-   .-                    :  += ::      =%%%=+%. =%*=*+-*%%+:#:%%%: .=     -  :.          =: - .*-:::::::::::::::::::::::::::::::
//                       ::   :.                   :  =- ::    +%%+..:-+: .++ :-=#%#.+-#%%  =      +.   ..       :+. =   +-::::::::::::::::::::::::::::::
//                     ..  :.  .:                      :- .-  +%#:    :-=:. :+  . -%:-==#* =.      #    -       .:#  #:   *::::::::::::::::::::::::::::::
//                       ..  .   :.                     .= .-=.=-.      :--==..  .:-*:+--#+       -*    +       =.# :: .  *::::::::::::::::::::::::::::::
//                        .:  ..  ::                      =:-  . -=:       .:       =+#-.+*    -  #.   :=       --=.-  - :#::::::::::::::::::::::::::::::
//                          :.  :  .:                      .    .  :=:      .:     ..=%:.-%.  :- --    -=      .:*:=  :- ==-:::::::::::::::::::::::::::::

	override void DamageAllLegs(float inputDmg)
	{
		if (InediaInfectedAI_GetPainManager().PainSystemIsActive()) {
			return;
		}
		
		super.DamageAllLegs(inputDmg);
	}
	
// INJURY ANIMATIONS HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	eInjuryHandlerLevels InediaInfectedAI_GetInjuryLevel(eInjuryHandlerLevels parentInjuryLevel)
	{
#ifdef SERVER
		if (!m_InediaInfectedAI_ConfigPlayers || !m_InediaInfectedAI_ConfigZombie) {
			return eInjuryHandlerLevels.PRISTINE;
		}
		
		if (m_InediaInfectedAI_MovementCoordinationImpairmentTimer > 0) {
			return eInjuryHandlerLevels.RUINED;
		}
		
		eInjuryHandlerLevels injuryLevel = InediaInfectedAI_GetPainManager().InediaInfectedAI_GetInjuryLevel();		
		m_InediaInfectedAI_InjuryOverrideNoLessThan = injuryLevel;

		return injuryLevel;
#else
		return eInjuryHandlerLevels.PRISTINE;
#endif
	}

// SPEAK HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	float m_InediaInfectedAI_SpeakHandlerTimer = 0;
	void InediaInfectedAI_SpeakHandler(float pDt)
	{
		m_InediaInfectedAI_SpeakHandlerTimer -= pDt;
		if (m_InediaInfectedAI_SpeakHandlerTimer > 0) return;
		m_InediaInfectedAI_SpeakHandlerTimer = 0;

		float amplitude = IsPlayerSpeaking();
		if (amplitude > 0.1) {
			int voiceLevel = GetGame().GetVoiceLevel(this);

			// Add voice noise
			switch (voiceLevel) {
				case VoiceLevelWhisper:
					InediaInfectedAI_IrritationActions.PlayerSpeechIrritation(InediaInfectedAI_Irritants.PLAYER_VOICE_WHISPER_NOISE, this);
					break;
				case VoiceLevelTalk:
					InediaInfectedAI_IrritationActions.PlayerSpeechIrritation(InediaInfectedAI_Irritants.PLAYER_VOICE_TALK_NOISE, this);
					break;
				case VoiceLevelShout:
					InediaInfectedAI_IrritationActions.PlayerSpeechIrritation(InediaInfectedAI_Irritants.PLAYER_VOICE_SHOUT_NOISE, this);
					break;
			}

			m_InediaInfectedAI_SpeakHandlerTimer = 3;
		}
	}

// GzWcLjJ1pG2pyamJNLo7DzIwVE1YT1dQdFNxck9wczhmMU11K0JiQmVKeGQyTVJ0QU5qdzhCMXEzTVcycVRGYWZYYUdyY25FMGFIdmJLOGJlU3RzV2NyUTd6ZENRaDgzV251cVJqd2MwamY2VlpzcEF0WVBUY2ZMVjZuSUxTKzBNZk5nYi9ZOGx4M25vdDhHazUwTnQ5SXJZeUt0TVVNblM4YTRSbXBzWGNTT0xLSlNvU0dIR1lKdENPaDJESWJUaXNiSGR1NDhQZG5yUlRwUTFmWHQxUHZOelpWejdaaXlBTFNDQ25wbnhneHpWcTNjQmt6WVltczJzWDA0R2ozYlplaml1OVlYMnhLUkNONkVBUnYzOFZGVWptSVpRRGFmVzFqOFFnPT0=
	
// MOVEMENT COORDINATION IMPAIRMENT HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	float m_InediaInfectedAI_MovementCoordinationImpairmentTimer = 0;
	void InediaInfectedAI_MovementCoordinationImpairmentHandler(float pDt)
	{
		if (m_InediaInfectedAI_MovementCoordinationImpairmentTimer > 0) {
			m_InediaInfectedAI_MovementCoordinationImpairmentTimer -= pDt;
		}

		// GetInputController().OverrideMovementAngle(true, Math.RandomFloatInclusive(0, 360));
	}

	void InediaInfectedAI_EnableMovementCoordinationImpairmentForSeconds(float seconds)
	{
		m_InediaInfectedAI_MovementCoordinationImpairmentTimer = seconds;
	}

	void InediaInfectedAI_DisableMovementCoordinationImpairment()
	{
		if (m_InediaInfectedAI_MovementCoordinationImpairmentTimer > 0) {
			m_InediaInfectedAI_MovementCoordinationImpairmentTimer = 0;
		}
	}

// BLOOD SPLATS HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Blood wall on receive hit
	float m_InediaInfectedAI_HitTimeWindowStart = 0;
	int m_InediaInfectedAI_HitCountInCurrentSecond = 0;
	override void OnReceivedHit(ImpactEffectsData hitData)
	{
#ifndef SERVER
		if (InediaInfectedAI_GetNetSyncBoolVariable("BloodSplatParticlesIsActive") && hitData && hitData.m_AmmoType) {
		    // Restrict hit triggers to 10 per 10 second
			float currentTime = GetGame().GetTickTime();
		    if (currentTime - m_InediaInfectedAI_HitTimeWindowStart > 10) {
		        m_InediaInfectedAI_HitTimeWindowStart = currentTime;
		        m_InediaInfectedAI_HitCountInCurrentSecond = 0;
		    }
		    if (m_InediaInfectedAI_HitCountInCurrentSecond >= 10) {
				super.OnReceivedHit(hitData);
				
		        return;  // Exit if the limit of hits per second is reached
		    }
			m_InediaInfectedAI_HitCountInCurrentSecond++;
			
			float bleedingChancePercent = GetGame().ConfigGetFloat("CfgAmmo " + hitData.m_AmmoType + " DamageApplied bleedThreshold") * 100;
			if (bleedingChancePercent > 0 && Math.RandomFloatInclusive(0, 100) <= bleedingChancePercent) {
				float healthDamage = GetGame().ConfigGetFloat("CfgAmmo " + hitData.m_AmmoType + " DamageApplied Health damage");
				float bloodDamage = GetGame().ConfigGetFloat("CfgAmmo " + hitData.m_AmmoType + " DamageApplied Blood damage");
				float bloodPowerDamage = healthDamage * bloodDamage / 100;
				
				int power;
				if (bloodPowerDamage >= 120) {
					power = 5;
				} else if (bloodPowerDamage >= 80) {
					power = 4;
				} else if (bloodPowerDamage >= 40) {
					power = 3;
				} else if (bloodPowerDamage > 0) {
					power = 2;
				} else {
					power = 1;
				}
				
				vector startPosition = hitData.m_ExitPosition;
				vector directionSpeed = hitData.m_OutSpeed;
				if (!startPosition || !directionSpeed) {
					startPosition = InediaInfectedAI_GetHeadPosition();
					float rand23 = Math.RandomFloatInclusive(2, 3);
					directionSpeed = vector.RandomDir2D() * rand23;
					directionSpeed = vector.RotateAroundZeroDeg(directionSpeed, directionSpeed.Perpend().Normalized(), Math.RandomFloatInclusive(-30, 30));
				}
				
				// Blood cannot splatter farther than 5-10 meters.
				if (directionSpeed.Length() > 10) {
					float rand510 = Math.RandomFloatInclusive(5, 10);
					directionSpeed = directionSpeed.Normalized() * rand510;
				}
				
				bool hasExited = false;
				if (hitData.m_OutSpeed) {
					hasExited = true;
				}
				
				MiscGameplayFunctions.InediaInfectedAI_ThrowBloodSplatFromPosition(startPosition, directionSpeed, power, hasExited, this, m_InediaInfectedAI_BloodParticlesBirthRateLimit);
			}
		}
#endif
		
		super.OnReceivedHit(hitData);
	}
	
// BLOOD TRAIL HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_BloodTrailHandlerClient()
	{
#ifndef SERVER
		if (m_InediaInfectedAI_BloodLossLevel > 0) {
			// If an creature is bleeding openly, he must leave blood trail.
			
			float rand;
			
			if (IsAlive()) {
				if (InediaInfectedAI_GetNetSyncBoolVariable("BloodTrailParticlesIsActive")) {
					int birthRateLimit = 3;
					if (m_InediaInfectedAI_BloodTrailFrequencySeconds <= 0.5) {
						birthRateLimit = 1;
					} else if (m_InediaInfectedAI_BloodTrailFrequencySeconds <= 1) {
						birthRateLimit = 2;
					}
					
					if (birthRateLimit > m_InediaInfectedAI_BloodParticlesBirthRateLimit) {
						birthRateLimit = m_InediaInfectedAI_BloodParticlesBirthRateLimit;
					}
					
					rand = Math.RandomFloatInclusive(0, 0.2);
					MiscGameplayFunctions.InediaInfectedAI_CreateBloodTrailInPosition(GetCenter() + vector.RandomDir2D() * rand, m_InediaInfectedAI_BloodLossLevel, this, birthRateLimit);
				}
			} else {
				if (InediaInfectedAI_GetNetSyncBoolVariable("BloodPoolParticlesIsActive")) {
					// Death of a creature, leaving a pool of blood if bleeding is active
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InediaInfectedAI_CreateBloodPoolClient, Math.RandomIntInclusive(3000, 5000), false);
					
					rand = Math.RandomFloatInclusive(0, 0.2);
					MiscGameplayFunctions.InediaInfectedAI_CreateBloodTrailInPosition(GetCenter() + vector.RandomDir2D() * rand, m_InediaInfectedAI_BloodLossLevel, this, m_InediaInfectedAI_BloodParticlesBirthRateLimit);
					
					return;
				}
			}
		}

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InediaInfectedAI_BloodTrailHandlerClient, m_InediaInfectedAI_BloodTrailFrequencySeconds * 1000, false);
#endif
	}
	
	void InediaInfectedAI_CreateBloodPoolClient()
	{
#ifndef SERVER
		float rand = Math.RandomFloatInclusive(0, 0.2);
		MiscGameplayFunctions.InediaInfectedAI_CreateBloodPoolInPosition(GetCenter() + vector.RandomDir2D() * rand, m_InediaInfectedAI_BloodLossLevel, this);
#endif
	}

// OVERRIDES OF OTHER PARENT METHODS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// With the DayZ 1.28 update, collisions with throwable projectiles began to be handled by events within the projectiles themselves (ItemBase).
	/*
	override protected void EOnContact(IEntity other, Contact extra)
	{
		super.EOnContact(other, extra);

#ifdef SERVER
		if (other) {
			if (other.IsInherited(ItemBase)) {
				ItemBase projectile = ItemBase.Cast(other);
				if (projectile && projectile.InediaInfectedAI_IsThrowingProjectile()) {
					projectile.InediaInfectedAI_ThrowingProjectileOnContactServer(this);
				}
			}
		}
#endif
	}*/

	override void OnThrowingModeChange(bool change_to_enabled)
	{
#ifdef SERVER
		if (change_to_enabled && m_InediaInfectedAI_ConfigZombie && m_InediaInfectedAI_ConfigZombie.ThrowingProjectilesHandlerIsActive && m_InediaInfectedAI_DefaulThrowingProjectile) {
			ItemBase itemInHands = ItemBase.Cast(GetItemInHands());
			if (itemInHands && !itemInHands.InediaInfectedAI_IsThrowingProjectile()) {
				m_InediaInfectedAI_DefaulThrowingProjectile.m_ThrowerEntity = this;
				itemInHands.InediaInfectedAI_SetThrowingProjectile(m_InediaInfectedAI_DefaulThrowingProjectile);
			}
		}
#endif

		super.OnThrowingModeChange(change_to_enabled);
	}
	
	override string GetHitComponentForAI()
	{
		if (!m_InediaInfectedAI_ConfigPlayers || !m_InediaInfectedAI_ConfigPlayers.OverrideGetHitComponentChances) {
			return super.GetHitComponentForAI();
		}
		
		// Completely override the vanilla functionality of RegisterHitComponentsForAI(),
		// as we need custom logic, for example, to ignore head damage and redirect it to the arms if the player is blocking an attack.
		
		float dmgZoneRandomPercent = Math.RandomFloatInclusive(0, 100);
		
		if (dmgZoneRandomPercent <= 19) {
			// Head damage
			if (!InediaInfectedAI_IsInBlock()) {
				return "dmgZone_head";
			} else {
				// Exclude head damage if the player is blocking, in this case, the damage is redirected to the arms
				if (Math.RandomBool()) {
					return "dmgZone_leftArm";
				} else {
					return "dmgZone_rightArm";
				}
			}
		} else if (dmgZoneRandomPercent <= 46) {
			// Arms damage
			if (Math.RandomBool()) {
				return "dmgZone_leftArm";
			} else {
				return "dmgZone_rightArm";
			}
		} else if (dmgZoneRandomPercent <= 73) {
			// Legs damage
			if (Math.RandomBool()) {
				return "dmgZone_leftLeg";
			} else {
				return "dmgZone_rightLeg";
			}
		} else {
			// Torso damage
			return "dmgZone_torso";
		}
	}
	
	override bool EvaluateDamageHitAnimation(TotalDamageResult pDamageResult, int pDamageType, EntityAI pSource, string pComponent, string pAmmoType, vector pModelPos, out int pAnimType, out float pAnimHitDir, out bool pAnimHitFullbody)
	{
		if (pAmmoType == "InediaInfectedAI_ThrowingProjectile") {
			return true;
		}
		
		return super.EvaluateDamageHitAnimation(pDamageResult, pDamageType, pSource, pComponent, pAmmoType, pModelPos, pAnimType, pAnimHitDir, pAnimHitFullbody);
	}
	
	// Disabling the vanilla broken leg debuff when Inedia stabilization is applied.
	override void UpdateBrokenLegs(int stateId)
	{
		if (InediaInfectedAI_GetPainManager().IsLimbStabilized("legs")) {
			if (m_MovementState.m_iStanceIdx != DayZPlayerConstants.STANCEIDX_PRONE)
			{
				m_InjuryHandler.m_ForceInjuryAnimMask = m_InjuryHandler.m_ForceInjuryAnimMask & ~eInjuryOverrides.PRONE_ANIM_OVERRIDE;
				m_InjuryHandler.m_ForceInjuryAnimMask = m_InjuryHandler.m_ForceInjuryAnimMask & ~eInjuryOverrides.BROKEN_LEGS;
				m_InjuryHandler.m_ForceInjuryAnimMask = m_InjuryHandler.m_ForceInjuryAnimMask | eInjuryOverrides.BROKEN_LEGS_SPLINT;
			}
			m_InjuryHandler.CheckValue(false);
			
			return;
		}
		
		super.UpdateBrokenLegs(stateId);
	}
	
	override void OnUnconsciousStart()
	{
		m_InediaInfectedAI_UnconsciousHealthDamageCounter = 0;
		
		super.OnUnconsciousStart();
	}
	
	override void InsertAgent(int agent, float count = 1) //adds a single agent type to the player agent pool
	{
		if (!GetAllowDamage()) {
			return;
		}

		super.InsertAgent(agent, count);
	}

	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

#ifdef SERVER
		if (IsAlive()) {
			InediaInfectedAI_UpdateCamoClothingMultiplier();
		}
#endif
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

#ifdef SERVER
		if (IsAlive()) {
			InediaInfectedAI_UpdateCamoClothingMultiplier();
		}
#endif
	}
	
// AUXILIARY METHODS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/*
	float m_InediaInfectedAI_ShowFrequentShotIcon_LastTime = -1000000;
	bool m_InediaInfectedAI_ShowFrequentShotIcon_AlertIsActive = false;
	void InediaInfectedAI_ShowFrequentShotIconClient(bool alert = false)
	{
#ifndef SERVER		
		if (alert) {
			m_InediaInfectedAI_ShowFrequentShotIcon_AlertIsActive = alert;
		}

		m_InediaInfectedAI_ShowFrequentShotIcon_LastTime = GetGame().GetTickTime();
#endif
	}
	*/
	
	bool InediaInfectedAI_CannotKillWithBackstab()
	{
		if (!InediaInfectedAI_GetPainManager()) {
			return false;
		}
		
		return InediaInfectedAI_GetPainManager().IsLimbBroken("arms") || InediaInfectedAI_GetPainManager().GetBulletCountInLimb("arms") > 0 || InediaInfectedAI_GetPainManager().GetPainWithBuffsLevelForLimb("arms") >= InediaInfectedAI_LimbPainLevels.BADLY;
	}
	
	float m_InediaInfectedAI_DisableStaminaRegenForSeconds = 0;
	void InediaInfectedAI_DisableStaminaRegenForSeconds(float seconds)
	{
		if (seconds < 0) {
			seconds = 0;
		}
		
		m_InediaInfectedAI_DisableStaminaRegenForSeconds = seconds;
	}
	
	bool InediaInfectedAI_IsStaminaRegenDisabled()
	{
		return m_InediaInfectedAI_DisableStaminaRegenForSeconds > 0;
	}
	
	float m_InediaInfectedAI_DoorDestroyedInfoLastSendTime = -1000000;
	void InediaInfectedAI_SendDoorDestroyedInfoServer()
	{
#ifdef SERVER
		if (!m_InediaInfectedAI_ConfigPlayers) {
			return;
		}
		
		if ((GetGame().GetTickTime() - m_InediaInfectedAI_DoorDestroyedInfoLastSendTime) > 2) {
			GetGame().CreateObject("InediaInfectedAI_AttemptOpenDestroyedDoor", GetPosition());
			InediaInfectedAI_SendNotSpamingImportantMessage("#STR_inediainfectedai_door_destroyed", 2, m_InediaInfectedAI_ConfigPlayers.DestroyedDoorNotificationType, 2);
			m_InediaInfectedAI_DoorDestroyedInfoLastSendTime = GetGame().GetTickTime();
		}
#endif
	}
	
	float InediaInfectedAI_GetNoiseMultiplierByShoes()
	{
		ItemBase shoes = ItemBase.Cast(GetInventory().FindAttachment(InventorySlots.FEET));
		if (!shoes) {
			return 0.6;
		}
		
		if (shoes.InediaInfectedAI_GetSoundType() == "Sneakers") {
			return 0.8;
		}
		
		return 1;
	}
	
	void InediaInfectedAI_RemoveDeepWoundBleedingParticles()
	{
		m_InediaInfectedAI_DeepWoundsBleedingsSyncBits = 0;
	}
	
	float m_InediaInfectedAI_PlayPainSoundLastPlayTime = -1000000;
	void InediaInfectedAI_PlayPainSoundServer(float notMoreFrequentlyThanSeconds = 3)
	{
#ifdef SERVER
		if (notMoreFrequentlyThanSeconds <= 3) {
			notMoreFrequentlyThanSeconds = 3;
		}
		
		if (notMoreFrequentlyThanSeconds <= 0 || (GetGame().GetTickTime() - m_InediaInfectedAI_PlayPainSoundLastPlayTime) > notMoreFrequentlyThanSeconds) {
			InediaInfectedAI_PlaySoundServer(InediaInfectedAI_PlayerBaseSoundTypes.PAIN);
			m_InediaInfectedAI_PlayPainSoundLastPlayTime = GetGame().GetTickTime();
		}
#endif
	}

	float m_InediaInfectedAI_PlayAuditoryHallucinationSoundLastPlayTime = -3;
	void InediaInfectedAI_PlayAuditoryHallucinationSoundServer()
	{
#ifdef SERVER
		if ((GetGame().GetTickTime() - m_InediaInfectedAI_PlayAuditoryHallucinationSoundLastPlayTime) > 3) {
			InediaInfectedAI_PlaySoundServer(InediaInfectedAI_PlayerBaseSoundTypes.AUDIORY_HALLUCINATIONS);
			m_InediaInfectedAI_PlayAuditoryHallucinationSoundLastPlayTime = GetGame().GetTickTime();
		}
#endif
	}
	
	void InediaInfectedAI_KnockItemOutOfHands()
	{
		if (IsRestrained()) {
			return;
		}
		
#ifdef SERVER
		if (GetIdentity()) {
			ScriptRPC rpc = new ScriptRPC();
	    	rpc.Send(this, InediaInfectedAI_PlayerBase_ERPCs.RPC_KNOCK_ITEM_OUT_OF_HANDS, true, GetIdentity());
		}
#else
		ItemBase itemInHands = GetItemInHands();
		if (itemInHands) {
			GetHumanInventory().ThrowEntity(itemInHands, vector.RandomDir(), 10);
		}
#endif
	}
	
	private float m_InediaInfectedAI_smoothedShakeOffsetX = 0;
	private float m_InediaInfectedAI_smoothedShakeOffsetY = 0;
	void InediaInfectedAI_ProcessAimFilters(float pDt, SDayZPlayerAimingModel pModel, int stance_index)
	{
#ifndef SERVER
	    // Hand shake (sway) offset
	    float handShakeMultiplier = InediaInfectedAI_GetPainManager().GetPainArmsHandShakeMultiplier();
	    if (handShakeMultiplier > 0) {
			float currentTime = GetGame().GetTickTime();

			// Frequency (Hz) of motion along different axes
			float frequencyX = 0.6;
			float frequencyY = 0.9;
			
			// Amplitude of motion
			float amplitude = handShakeMultiplier;
			if (IsInIronsights() || IsInOptics()) {
				amplitude /= 3;
			}
			
			amplitude = Math.Clamp(amplitude, 1, 24);
			
			// Smooth offset generation using a sine wave
			float targetShakeOffsetX = amplitude / 2 * Math.Sin(currentTime * 2.0 * Math.PI * frequencyX);
			float targetShakeOffsetY = amplitude * Math.Sin(currentTime * 2.0 * Math.PI * frequencyY + 1.0);
			
			// Smoothing
			float smoothingFactor = 1.0 - Math.Pow(Math.EULER, -1.5 * pDt);
			m_InediaInfectedAI_smoothedShakeOffsetX = Math.Lerp(m_InediaInfectedAI_smoothedShakeOffsetX, targetShakeOffsetX, smoothingFactor);
			m_InediaInfectedAI_smoothedShakeOffsetY = Math.Lerp(m_InediaInfectedAI_smoothedShakeOffsetY, targetShakeOffsetY, smoothingFactor);
			
			// Applying changes to the model
			pModel.m_fAimXHandsOffset += m_InediaInfectedAI_smoothedShakeOffsetX;
			pModel.m_fAimYHandsOffset += m_InediaInfectedAI_smoothedShakeOffsetY;
	    }

		// Camera shake offset
		if (m_InediaInfectedAI_CameraShakeSeconds > 0) {
			if (m_InediaInfectedAI_CameraShakeIntensity > 0) {
				float camera_shake_offset = m_InediaInfectedAI_CameraShakeIntensity * GetRandomGeneratorSyncManager().GetRandomInRange(RandomGeneratorSyncUsage.RGSAimingModel, -0.5, 0.5);
				
		        pModel.m_fAimXCamOffset += camera_shake_offset;
				pModel.m_fAimXCamOffset += camera_shake_offset;
				pModel.m_fAimXHandsOffset += camera_shake_offset;
				pModel.m_fAimYHandsOffset += camera_shake_offset;
			}

			m_InediaInfectedAI_CameraShakeSeconds -= pDt;
		}
#endif
	}

	float m_InediaInfectedAI_CamoClothingMultiplier = 1;
	void InediaInfectedAI_UpdateCamoClothingMultiplier()
	{
		if (!m_InediaInfectedAI_ConfigPlayers) {
			return;
		}

		m_InediaInfectedAI_CamoClothingMultiplier = 1;

		array<string> slots = {"Vest", "Body", "Hips", "Legs", "Back", "Headgear", "Mask", "Eyewear", "Gloves", "Feet", "Armband"};

		foreach(string slot: slots) {
			ItemBase item = ItemBase.Cast(FindAttachmentBySlotName(slot));
			if (!item) {
				continue;
			}

			m_InediaInfectedAI_CamoClothingMultiplier *= m_InediaInfectedAI_ConfigPlayers.GetClothingVisibilityMultiplier(item);
		}
	}

	float InediaInfectedAI_GetCamoClothingMultiplier()
	{
		return m_InediaInfectedAI_CamoClothingMultiplier;
	}
	
	void InediaInfectedAI_Kill()
	{
		m_InediaInfectedAI_Killed = true;
		m_InediaInfectedAI_VanillaHeadHealthBeforeEEHitBy = 0;
		m_InediaInfectedAI_VanillaHealthBeforeEEHitBy = 0;
		
		// CallLater, otherwise ProcessDirectDamage() does not trigger inside EEHitBy()
		// This also resolves the vanilla issue where in EEKilled() the killer is replaced by the entity of the creature itself.
		// This is most noticeable when the creature is killed using a Hammer.
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InediaInfectedAI_ProcessKill, 50, false);
	}
	
	void InediaInfectedAI_ProcessKill()
	{
		if (IsAlive()) {
			if (m_InediaInfectedAI_EEHitByLastSource && !m_InediaInfectedAI_EEHitByLastSource.IsInherited(BuildingSuper)) {
				SetHealth(0.000001);
				ProcessDirectDamage(m_InediaInfectedAI_EEHitByLastDamageType, m_InediaInfectedAI_EEHitByLastSource, m_InediaInfectedAI_EEHitByLastDmgZone, m_InediaInfectedAI_EEHitByLastAmmo, m_InediaInfectedAI_EEHitByLastModelPos);
				
				// Just in case
				if (IsAlive()) {
					SetHealth(0);
				}
			} else {
				SetHealth(0);
			}
		}
	}
	
	bool InediaInfectedAI_IsNPC()
	{
#ifdef ENFUSION_AI_PROJECT
		if (IsAI()) {
			return true;
		}
#endif

		if (!IsPlayer()) {
			return true;
		}
		
		if (m_InediaInfectedAI_IsTestPlayer) {
			return true;
		}

		return false;
	}
	
	float m_InediaInfectedAI_ShockRefillSpeedLastGetTime = -1000000;
	float m_InediaInfectedAI_ShockRefillSpeedLastValue = PlayerConstants.SHOCK_REFILL_CONSCIOUS_SPEED;
	float InediaInfectedAI_GetShockRefillSpeed()
	{
		if ((GetGame().GetTickTime() - m_InediaInfectedAI_ShockRefillSpeedLastGetTime) > 3 && GetModifiersManager()) {
			ShockMdfr shockModifier = ShockMdfr.Cast(GetModifiersManager().GetModifier(eModifiers.MDF_SHOCK));
			if (shockModifier) {
				m_InediaInfectedAI_ShockRefillSpeedLastValue = shockModifier.GetRefillSpeed(this);
				m_InediaInfectedAI_ShockRefillSpeedLastGetTime = GetGame().GetTickTime();
			}
		}
		
		return m_InediaInfectedAI_ShockRefillSpeedLastValue;
	}
	
	// The method is called when the player uses any item, regardless of whether they consume it internally or simply use it.
	void InediaInfectedAI_UseItem(ItemBase item, float consumedPercent = 100)
	{
		if (!item || consumedPercent <= 0) {
			return;
		}
		
		InediaInfectedAI_GetPainManager().UseItem(item, consumedPercent);
	}
	
	// The method is called when the player consume any liquid.
	void InediaInfectedAI_UseLiquid(int liquidId, float consumedPercent = 100)
	{
		if (liquidId <= 0 || consumedPercent <= 0) {
			return;
		}
		
		InediaInfectedAI_GetPainManager().UseLiquid(liquidId, consumedPercent);
	}
	
	void InediaInfectedAI_AddRandomBleedingSource()
	{
		if (PlayerBase.m_BleedingSourcesLow && PlayerBase.m_BleedingSourcesLow.Count() > 0 && GetBleedingManagerServer()) {
			GetBleedingManagerServer().AttemptAddBleedingSourceBySelection(PlayerBase.m_BleedingSourcesLow[Math.RandomIntInclusive(0, PlayerBase.m_BleedingSourcesLow.Count() - 1)]);
		}
	}
	
	ref array<string> InediaInfectedAI_RemoveFatalDamageDmgZones;
	void InediaInfectedAI_RemoveFatalDamage()
	{
		if (!InediaInfectedAI_RemoveFatalDamageDmgZones) {
			if (GetEntityDamageZoneMap()) {
				InediaInfectedAI_RemoveFatalDamageDmgZones = GetEntityDamageZoneMap().GetKeyArray();
			}
		}
		
		// Save the character's health and blood before a full recovery (I don't know of any other way to remove fatal damage).
		map<string, float> dmgZonesHealthBefore = new map<string, float>;
		if (InediaInfectedAI_RemoveFatalDamageDmgZones) {
			foreach (string dmgZoneNameBefore: InediaInfectedAI_RemoveFatalDamageDmgZones) {
				if (dmgZoneNameBefore != "Head" && dmgZoneNameBefore != "Brain") { // The head is a critical zone, so it should be fully restored along with the global zone.
					dmgZonesHealthBefore[dmgZoneNameBefore] = GetHealth(dmgZoneNameBefore, "Health");
				}
			}
		}
		float globalBloodBefore = GetHealth("", "Blood");
		
		// Remove the fatal damage (this is when IsAlive() returns false, for example, with a headshot, even before EEHitBy() is processed).
		SetFullHealth();
		
		// Restore the character's health and blood after removing the fatal damage.
		foreach (string dmgZoneNameAfter, float healthValue: dmgZonesHealthBefore) {
			SetHealth(dmgZoneNameAfter, "Health", healthValue);
			
		}
		SetHealth("", "Blood", globalBloodBefore);
		
		// GetAdditionalHealthTypes()	
	}
	
	void InediaInfectedAI_SetNetSyncBoolVariable(string name, bool value)
	{
		int position = m_InediaInfectedAI_NetSyncBoolVariables.Find(name);
		
		if (position >= 0 && position < 32) {
	        if (value) {
	            m_InediaInfectedAI_NetSyncBoolVariablesSyncBits |= (1 << position);
	        } else {
	            m_InediaInfectedAI_NetSyncBoolVariablesSyncBits &= ~(1 << position);
	        }
		} else {
            ErrorEx("InediaInfectedAI_SetNetSyncBoolVariable(" + name + ", " + value.ToString() + "): incorrect position.");
        }
	}
	
	bool InediaInfectedAI_GetNetSyncBoolVariable(string name)
	{
		int position = m_InediaInfectedAI_NetSyncBoolVariables.Find(name);
		
		if (position >= 0 && position < 32) {
			return (m_InediaInfectedAI_NetSyncBoolVariablesSyncBits & (1 << position)) != 0;
		} else {
			ErrorEx("InediaInfectedAI_GetNetSyncBoolVariable(" + name + "): incorrect position.");
		}
		
		return false;
	}
	
	void InediaInfectedAI_SetNetSyncBoolVariable2(string name, bool value)
	{
		int position = m_InediaInfectedAI_NetSyncBoolVariables2.Find(name);
		
		if (position >= 0 && position < 32) {
	        if (value) {
	            m_InediaInfectedAI_NetSyncBoolVariables2SyncBits |= (1 << position);
	        } else {
	            m_InediaInfectedAI_NetSyncBoolVariables2SyncBits &= ~(1 << position);
	        }
		} else {
            ErrorEx("InediaInfectedAI_SetNetSyncBoolVariable2(" + name + ", " + value.ToString() + "): incorrect position.");
        }
	}
	
	bool InediaInfectedAI_GetNetSyncBoolVariable2(string name)
	{
		int position = m_InediaInfectedAI_NetSyncBoolVariables2.Find(name);
		
		if (position >= 0 && position < 32) {
			return (m_InediaInfectedAI_NetSyncBoolVariables2SyncBits & (1 << position)) != 0;
		} else {
			ErrorEx("InediaInfectedAI_GetNetSyncBoolVariable2(" + name + "): incorrect position.");
		}
		
		return false;
	}
	
	bool InediaInfectedAI_QuietDoorOpeningMechanicAvailable(Object building)
	{
		if (m_InediaInfectedAI_QuietDoorOpeningMechanicDisabledForOpening) {
			return false;
		}

		return InediaInfectedAI_QuietDoorClosingMechanicAvailable(building);
	}
	bool InediaInfectedAI_QuietDoorClosingMechanicAvailable(Object building)
	{
		if (!m_InediaInfectedAI_QuietDoorOpeningMechanicIsActive || !building) {
			return false;
		}

		if (m_InediaInfectedAI_QuietDoorOpeningMechanicRestrictedBuildings && m_InediaInfectedAI_QuietDoorOpeningMechanicRestrictedBuildings.Count() > 0) {
			foreach (string restrictedBuildingClassName: m_InediaInfectedAI_QuietDoorOpeningMechanicRestrictedBuildings) {
				if (InediaInfectedAI_Functions.IsKindOfCached(restrictedBuildingClassName, building)) {
					return false;
				}
			}
		}

		return true;
	}
	
	float InediaInfectedAI_GetPainToPlayerLimbMultiplier(PlayerBase player, string limb, int damageType = DT_CLOSE_COMBAT)
	{
		if (!player || !m_InediaInfectedAI_ConfigZombie || !limb) {
			return 1;
		}
		
		if (player.InediaInfectedAI_IsInBlock() && damageType == DT_CLOSE_COMBAT) {
			// Block
			if (limb == "head") {
				return m_InediaInfectedAI_ConfigZombie.PainToPlayerHeadInBlockMultiplier;
			} else if (limb == "arms") {
				return m_InediaInfectedAI_ConfigZombie.PainToPlayerArmsInBlockMultiplier;
			} else if (limb == "legs") {
				return m_InediaInfectedAI_ConfigZombie.PainToPlayerLegsInBlockMultiplier;
			} else if (limb == "torso") {
				return m_InediaInfectedAI_ConfigZombie.PainToPlayerTorsoInBlockMultiplier;
			}
		} else {
			// No block
			if (limb == "head") {
				return m_InediaInfectedAI_ConfigZombie.PainToPlayerHeadMultiplier;
			} else if (limb == "arms") {
				return m_InediaInfectedAI_ConfigZombie.PainToPlayerArmsMultiplier;
			} else if (limb == "legs") {
				return m_InediaInfectedAI_ConfigZombie.PainToPlayerLegsMultiplier;
			} else if (limb == "torso") {
				return m_InediaInfectedAI_ConfigZombie.PainToPlayerTorsoMultiplier;
			}
		}
		
		return 1;
	}
	
	bool InediaInfectedAI_IsAdmin()
	{
		return GetIdentity() && InediaInfectedAIAdmins_Config.GetInstance().AdminsSteamIds.Find(GetIdentity().GetPlainId()) >= 0;
	}
	
	// Server side
	void InediaInfectedAI_SendInfoMessage(string message, int notificationType = 1, float notificationDurationSeconds = 10) {
#ifdef SERVER
		if (!GetIdentity()) return;
		
		if (notificationType == 1) {
			Param1<string> Msgparam = new Param1<string>(message);
	    	GetGame().RPCSingleParam(this, InediaInfectedAI_PlayerBase_ERPCs.RPC_USER_INFO_MESSAGE, Msgparam, true, GetIdentity());
		} else if (notificationType == 2) {
			NotificationSystem.SendNotificationToPlayerExtended(this, notificationDurationSeconds, "...", message);
		}
#endif
	}
	void InediaInfectedAI_SendActionMessage(string message, int notificationType = 1, float notificationDurationSeconds = 10) {
#ifdef SERVER
		if (!GetIdentity()) return;
		
		if (notificationType == 1) {
			Param1<string> Msgparam = new Param1<string>(message);
	    	GetGame().RPCSingleParam(this, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, GetIdentity());
		} else if (notificationType == 2) {
			NotificationSystem.SendNotificationToPlayerExtended(this, notificationDurationSeconds, "...", message);
		}
#endif
	}
	void InediaInfectedAI_SendImportantMessage(string message, int notificationType = 1, float notificationDurationSeconds = 10) {
#ifdef SERVER
		if (!GetIdentity()) return;
		
		if (notificationType == 1) {
			Param1<string> Msgparam = new Param1<string>(message);
	    	GetGame().RPCSingleParam(this, InediaInfectedAI_PlayerBase_ERPCs.RPC_USER_IMPORTANT_MESSAGE, Msgparam, true, GetIdentity());
		} else if (notificationType == 2) {
			NotificationSystem.SendNotificationToPlayerExtended(this, notificationDurationSeconds, "...", message);
		}
#endif
	}
	
	ref map<string, float> m_InediaInfectedAI_NotSpamingImportantMessagesLastSend = new map<string, float>;
	void InediaInfectedAI_SendNotSpamingImportantMessage(string message, float delaySeconds = 10, int notificationType = 1, float notificationDurationSeconds = 10) {
#ifdef SERVER
		float lastSend;
		if (m_InediaInfectedAI_NotSpamingImportantMessagesLastSend.Find(message, lastSend)) {
			if ((GetGame().GetTickTime() - lastSend) > delaySeconds) {
				InediaInfectedAI_SendImportantMessage(message, notificationType, notificationDurationSeconds);
				m_InediaInfectedAI_NotSpamingImportantMessagesLastSend[message] = GetGame().GetTickTime();
			}
		} else {
			InediaInfectedAI_SendImportantMessage(message, notificationType, notificationDurationSeconds);
			m_InediaInfectedAI_NotSpamingImportantMessagesLastSend[message] = GetGame().GetTickTime();
		}
#endif
	}

	void InediaInfectedAI_SetVomitWithoutMask()
	{
		if (FindAttachmentBySlotName("Mask")) {
			return;
		} else {
			ItemBase itemHeadgear = ItemBase.Cast(FindAttachmentBySlotName("Headgear"));
			if (itemHeadgear && InediaInfectedAI_Functions.IsKindOfCached("MotoHelmet_ColorBase", itemHeadgear)) {
				return;
			}
		}

		InediaInfectedAI_SymptomVomit();
	}
	
	void InediaInfectedAI_SymptomVomit(float durationSeconds = 5, bool emptyingStomach = true)
	{
		SymptomBase symptom = GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_VOMIT);
		if (symptom) {
		    symptom.SetDuration(durationSeconds);

			if (emptyingStomach) {
			    if (GetStatWater().Get() > SalmonellaMdfr.WATER_DRAIN_FROM_VOMIT)
			        GetStatWater().Add(-1 * SalmonellaMdfr.WATER_DRAIN_FROM_VOMIT);
			    if (GetStatEnergy().Get() > SalmonellaMdfr.ENERGY_DRAIN_FROM_VOMIT)
			        GetStatEnergy().Add(-1 * SalmonellaMdfr.ENERGY_DRAIN_FROM_VOMIT);
			}
		}
	}

	void InediaInfectedAI_SetBloodyHandsWithoutGloves()
	{
		if (FindAttachmentBySlotName("Gloves")) {
			return;
		}

		SetBloodyHands(true);
	}

	void InediaInfectedAI_MakeUnconscious()
	{
		m_InediaInfectedAI_VanillaShockBeforeEEHitBy = 0;
		SetHealth("GlobalHealth", "Shock", 0);
	}

	bool InediaInfectedAI_CanPlayerViewCargo(PlayerBase player)
	{
#ifndef SERVER
		if (!InediaInfectedAI_SearchBodyToViewCargoEnabled()) {
			return true;
		}

		if (player.GetIdentity()) {
			return m_InediaInfectedAI_SearchBodyToViewCargoPlayersIds.Find(player.GetIdentity().GetId()) >= 0;
		}
#endif
				
		return false;
	}
	
	bool InediaInfectedAI_SearchBodyToViewCargoEnabled()
	{
#ifndef SERVER
		PlayerBase currentClient = PlayerBase.Cast(GetGame().GetPlayer());
		
		return currentClient && currentClient.m_InediaInfectedAI_SearchBodyToViewCargo;
#endif
		
		return m_InediaInfectedAI_SearchBodyToViewCargo;
	}
	
	bool InediaInfectedAI_SearchBodyOnlyEmptyHands()
	{
#ifndef SERVER
		PlayerBase currentClient = PlayerBase.Cast(GetGame().GetPlayer());
		
		return currentClient && currentClient.m_InediaInfectedAI_SearchBodyOnlyEmptyHands;
#endif
		
		return m_InediaInfectedAI_SearchBodyOnlyEmptyHands;
	}
	
	float InediaInfectedAI_SearchBodyToViewCargoSeconds()
	{
#ifndef SERVER
		PlayerBase currentClient = PlayerBase.Cast(GetGame().GetPlayer());
		
		if (currentClient) {
			return currentClient.m_InediaInfectedAI_SearchBodyToViewCargoSeconds;
		}
#endif
		
		return m_InediaInfectedAI_SearchBodyToViewCargoSeconds;
	}

	void InediaInfectedAI_AllowPlayersToViewCargoServer(array<PlayerBase> players)
	{
#ifdef SERVER
		if (!players || players.Count() == 0) {
			return;
		}
		
		array<PlayerBase> alivePlayers = new array<PlayerBase>;
		array<string> playersIdsForSync = new array<string>;
		foreach (PlayerBase player: players) {
			if (player.IsAlive() && player.GetIdentity() && !player.InediaInfectedAI_IsNPC()) {
				alivePlayers.Insert(player);
				playersIdsForSync.Insert(player.GetIdentity().GetId());
			}
		}
		
		if (alivePlayers.Count() == 0) {
			return;
		}

		foreach (PlayerBase alivePlayer: alivePlayers) {
			ScriptRPC rpc = new ScriptRPC();
		    rpc.Write(playersIdsForSync);
			rpc.Send(this, InediaInfectedAI_PlayerBase_ERPCs.RPC_ALLOW_PLAYERS_TO_VIEW_CARGO, true, alivePlayer.GetIdentity());
		}
#endif
	}

	void InediaInfectedAI_AllowAllPlayersInRadiusToViewCargoServer(float radius)
	{
#ifdef SERVER
		if (radius <= 0) {
			return;
		}

		if (radius > 1000) {
			radius = 1000;
		}
		
		array<PlayerBase> playersWhoAllowedToViewCargo = new array<PlayerBase>;

		array<PlayerBase> players = new array<PlayerBase>;
		MiscGameplayFunctions.InediaInfectedAI_GetPlayersAtPosition3D(GetPosition(), radius, players);
	    foreach (PlayerBase player : players) {
			if (player.IsAlive()) {
				playersWhoAllowedToViewCargo.Insert(player);
			}
		}
		
		InediaInfectedAI_AllowPlayersToViewCargoServer(playersWhoAllowedToViewCargo);
#endif
	}

	void InediaInfectedAI_Stun(vector hitFromPosition)
	{
		if (!hitFromPosition || IsUnconscious() || !IsAlive() || InediaInfectedAI_IsInProneStance() || IsSwimming() || IsInVehicle() || InediaInfectedAI_DayzHorseCompatibility.IsPlayerRidingHorse(this)) {
			return;
		}

		float hitDirectionAngle = InediaInfectedAI_GetToPositionDirectionAngleX(hitFromPosition);

		DayZPlayerSyncJunctures.SendDamageHitEx(this, 0, hitDirectionAngle, true, null, DT_CLOSE_COMBAT, this, GetHitComponentForAI(), "MeleeZombie", vector.Zero);
	}

	float m_InediaInfectedAI_CameraShakeSeconds = 0;
	float m_InediaInfectedAI_CameraShakeIntensity = 0;
	// Server side
	void InediaInfectedAI_CameraShake(float seconds, float intensity)
	{
#ifdef SERVER
		if (IsUnconscious() || !IsAlive()) {
			return;
		}

		if (GetIdentity()) {
			if (seconds > 0 && intensity > 0) {
			    ScriptRPC rpc = new ScriptRPC();
			    rpc.Write(seconds);
			    rpc.Write(intensity);
			    rpc.Send(this, InediaInfectedAI_PlayerBase_ERPCs.RPC_HIT_CAMERA_SHAKE, false, GetIdentity());
			}
		}
#endif
	}

	bool m_InediaInfectedAI_IsInHideMode = false;
	bool InediaInfectedAI_IsInHideMode()
	{
		return m_InediaInfectedAI_IsInHideMode;
	}

	void InediaInfectedAI_SetHideMode()
	{
		m_InediaInfectedAI_IsInHideMode = true;
	}

	void InediaInfectedAI_UnsetHideMode()
	{
		m_InediaInfectedAI_IsInHideMode = false;
	}
	
	bool m_InediaInfectedAI_HasUnlimitedAmmo = false;
	bool InediaInfectedAI_HasUnlimitedAmmo()
	{
		return m_InediaInfectedAI_HasUnlimitedAmmo;
	}

	void InediaInfectedAI_SetUnlimitedAmmo()
	{
		Weapon_Base weapon = Weapon_Base.Cast(GetItemInHands());
		if (weapon) {
			weapon.InediaInfectedAI_SetMaxAmmo();
		}
		
		m_InediaInfectedAI_HasUnlimitedAmmo = true;
		
#ifdef SERVER
		if (GetIdentity()) {
			ScriptRPC rpc = new ScriptRPC();
	    	rpc.Send(this, InediaInfectedAI_PlayerBase_ERPCs.RPC_SET_UNLIMITED_AMMO, true, GetIdentity());
		}
#endif
	}

	void InediaInfectedAI_UnsetUnlimitedAmmo()
	{
		Weapon_Base weapon = Weapon_Base.Cast(GetItemInHands());
		if (weapon) {
			weapon.InediaInfectedAI_SetMaxAmmo();
		}
		
		m_InediaInfectedAI_HasUnlimitedAmmo = false;
		
#ifdef SERVER
		if (GetIdentity()) {
			ScriptRPC rpc = new ScriptRPC();
	    	rpc.Send(this, InediaInfectedAI_PlayerBase_ERPCs.RPC_UNSET_UNLIMITED_AMMO, true, GetIdentity());
		}
#endif
	}

	void InediaInfectedAI_PlaySoundServer(InediaInfectedAI_PlayerBaseSoundTypes type, int variation = 0)
	{
#ifdef SERVER
		if (!IsAlive() || IsUnconscious()) {
			return;
		}

		if (GetIdentity()) {
		    ScriptRPC rpc = new ScriptRPC();

		    rpc.Write(type);
		    rpc.Write(variation);

			rpc.Send(this, InediaInfectedAI_PlayerBase_ERPCs.RPC_PLAY_SOUND, false, GetIdentity());
		}
#endif
	}

	void InediaInfectedAI_PlaySoundClient(InediaInfectedAI_PlayerBaseSoundTypes type, int variation = 0)
	{
#ifndef SERVER
		if (!IsAlive() || IsUnconscious()) {
			return;
		}

		if (type == InediaInfectedAI_PlayerBaseSoundTypes.PAIN) {
			string injurySoundSet;

			if (IsMale()) {
				if (GetVoiceType() == 2) {
					injurySoundSet = "inedia_infectedai_pain_male_2_SoundSet";
				} else {
					injurySoundSet = "inedia_infectedai_pain_male_1_SoundSet";
				}
			} else {
				if (GetVoiceType() == 2) {
					injurySoundSet = "inedia_infectedai_pain_female_2_SoundSet";
				} else {
					injurySoundSet = "inedia_infectedai_pain_female_1_SoundSet";
				}
			}
			EffectSound injurySound = SEffectManager.PlaySoundOnObject(injurySoundSet, this);
			injurySound.SetAutodestroy(true);
		} else if (type == InediaInfectedAI_PlayerBaseSoundTypes.AUDIORY_HALLUCINATIONS) {
            if (!m_InediaInfectedAI_AudiouryHallutinationsSoundsets || !m_InediaInfectedAI_AudiouryHallutinationsSoundsetsGroups) {
                return;
            }

            string audiouryHallutinationsRandomSoundSetGroup = m_InediaInfectedAI_AudiouryHallutinationsSoundsetsGroups.GetRandomElement();
            array<string> audiouryHallutinationsRandomSoundsets = m_InediaInfectedAI_AudiouryHallutinationsSoundsets.Get(audiouryHallutinationsRandomSoundSetGroup);
            string audiouryHallutinationsRandomSoundset = audiouryHallutinationsRandomSoundsets.GetRandomElement();
            if (!audiouryHallutinationsRandomSoundset) {
                return;
            }

            float audiouryHallutinationsSoundRange = 0;
            switch (audiouryHallutinationsRandomSoundSetGroup) {
                case "zmb_move":
                case "zmb_disturb":
                case "zmb_attack":
                case "zmb_hit":
					audiouryHallutinationsSoundRange = Math.RandomFloatInclusive(9, 12);
					break;
				case "zmb_scream":
                    audiouryHallutinationsSoundRange = Math.RandomFloatInclusive(18, 24);
                    break;
                case "rifle_shot":
                    audiouryHallutinationsSoundRange = Math.RandomFloatInclusive(900, 1000);
                    break;
                case "pistol_shot":
                    audiouryHallutinationsSoundRange = Math.RandomFloatInclusive(250, 270);
                    break;
                case "bullet":
                   	audiouryHallutinationsSoundRange = 2;
                    break;
            }

            if (audiouryHallutinationsSoundRange == 0) {
                return;
            }

            vector audiouryHallutinationsSoundPos = GetPosition() + audiouryHallutinationsSoundRange * vector.RotateAroundZeroDeg(GetDirection(), vector.Up, Math.RandomFloatInclusive(0, 360));
            float audiouryHallutinationsSoundPosSurfaceY = GetGame().SurfaceRoadY(audiouryHallutinationsSoundPos[0], audiouryHallutinationsSoundPos[2]);
            audiouryHallutinationsSoundPos = Vector(audiouryHallutinationsSoundPos[0], audiouryHallutinationsSoundPosSurfaceY, audiouryHallutinationsSoundPos[2]) + 1.7 * vector.Up;

            if (!audiouryHallutinationsSoundPos) {
                return;
            }

            EffectSound audiouryHallutinationsSound = SEffectManager.PlaySound(audiouryHallutinationsRandomSoundset, audiouryHallutinationsSoundPos);
            audiouryHallutinationsSound.SetAutodestroy(true);
		} else if (type == InediaInfectedAI_PlayerBaseSoundTypes.FRACTURE) {
			if (!IsAlive() || IsUnconscious()) {
				return;
			}
			
			BreakLegSound();
		} else if (type == InediaInfectedAI_PlayerBaseSoundTypes.DEEP_WOUND) {
			if (!IsAlive() || IsUnconscious()) {
				return;
			}
			
			SEffectManager.PlaySoundOnObject("bleeding_SoundSet", this);
		}
#endif
	}

	void InediaInfectedAI_Heal()
	{
		// Remove Inedia debuffs
		InediaInfectedAI_RemoveInediaDebuffs();

		// Clear stomach
		GetStomach().ClearContents();
		
		// Restore Health, Shock, Blood and zones
		DamageSystem.ResetAllZones(this);
		GetModifiersManager().ResetAll();
		
		// Restore energy and water
		GetStatEnergy().Set(GetStatEnergy().GetMax());
		GetStatWater().Set(GetStatWater().GetMax());
		
		// Remove bleedings
		if (m_BleedingManagerServer) {
			m_BleedingManagerServer.RemoveAllSources();
		}

		// Remove agents
		if (m_AgentPool) {
			m_AgentPool.RemoveAllAgents();
		}

		// Reset stamina
		if (m_StaminaHandler) {
			m_StaminaHandler.SetStamina(GameConstants.STAMINA_MAX);
		}
		
		// Remove bloody hands
		PluginLifespan moduleLifespan = PluginLifespan.Cast(GetPlugin(PluginLifespan));
		if (moduleLifespan) {
			moduleLifespan.UpdateBloodyHandsVisibilityEx(this, eBloodyHandsTypes.CLEAN);
		}
		
		// Remove arrows
		if (GetArrowManager()) {
			GetArrowManager().ClearArrows();
		}
		
		// Remove broken legs
		if (GetModifiersManager().IsModifierActive(eModifiers.MDF_BROKEN_LEGS)) {
		    GetModifiersManager().DeactivateModifier(eModifiers.MDF_BROKEN_LEGS);
		}
		
		// Heat buffer +
		GetStatHeatBuffer().Set(30);
		GetStatHeatComfort().Set(0.1);
	}
	
	void InediaInfectedAI_RemoveInediaDebuffs()
	{
		InediaInfectedAI_GetPainManager().m_BreakHeadLastActivateTime = -1000000;
		InediaInfectedAI_GetPainManager().m_BreakLegsLastActivateTime = -1000000;
		InediaInfectedAI_GetPainManager().m_BreakArmsLastActivateTime = -1000000;
		InediaInfectedAI_GetPainManager().m_BreakTorsoLastActivateTime = -1000000;
		InediaInfectedAI_GetPainManager().m_HeadDeepWoundLastActivateTime = -1000000;
		InediaInfectedAI_GetPainManager().m_ArmsDeepWoundLastActivateTime = -1000000;
		InediaInfectedAI_GetPainManager().m_LegsDeepWoundLastActivateTime = -1000000;
		InediaInfectedAI_GetPainManager().m_TorsoDeepWoundLastActivateTime = -1000000;
		InediaInfectedAI_GetPainManager().DeactivateInternalBleeding();
		foreach (string inediaPainLimb: InediaInfectedAI_PainManager.m_InediaPainLimbs) {
			InediaInfectedAI_GetPainManager().DeactivateBreakForLimb(inediaPainLimb);
			InediaInfectedAI_GetPainManager().DeactivateDeepWoundForLimb(inediaPainLimb);
			InediaInfectedAI_GetPainManager().SetBulletCountInLimb(inediaPainLimb, 0);
			
			InediaInfectedAI_GetPainManager().AddPainPercentForLimb(inediaPainLimb, -100);
		}
	}
	
	void InediaInfectedAI_HealEquip()
	{
		if (!IsAlive()) {
			return;
		}
		
		// Fix up inventory without healing the player
		float healthBefore = GetHealth();
		FixAllInventoryItems();
		SetHealth(healthBefore);
	}
	
	bool m_InediaInfectedAI_HealEquipAuto = false;
	void InediaInfectedAI_HealEquipAuto()
	{
		if (!IsAlive()) {
			return;
		}
		
		// Fix up inventory
		InediaInfectedAI_HealEquip();
		
		if (m_InediaInfectedAI_HealEquipAuto) {
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InediaInfectedAI_HealEquipAuto, 5000, false);
		}
	}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::-======-----=======-:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::-===-:.                 .::-====-::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::-==-:                              .:====::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::-==:                                     ..:-==-::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::.-+-    .....    .                             ::.:==::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::.-+:           .::: ...                            .:..==::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::+-                 ::: .::                           .. .+-::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::-+                     .-:  :.          .:...           .-  :+:::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::=+    -.           .:--.   -: .-           .-.             -  .*-:::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::-+    :=         .....:=%#=...:. :..                ....     -   +=::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::*     =.                 =##:-..:   ..:::.              --    -   +=:::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::=-   .+.               :-=--%%::-::..     .--:   .::.           .   *-::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::#   .=             ..::...=*%%% -:.- .       .-:    :--=-:..   ...  .*::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::*    .                .-*%%%%%#-= :. ---:.     .-: ..   .:----::...  -=:::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::-+                     .::-%%%%+. . ..    :-:::   :-  :::   ..---::..  *..:::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::-=              :         .*%%:       .:..::..:--=:.-.   .:::. ..   .  -=.:::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::-=       :.     -:  =     .#%=    ::::.     :--+:.-*--: :.   .-. .:-:   +::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::*        *+ .  =..:+ .   :%%   .+.  ::-::   .:=:.  =*--.-*+-. ...  .  .-*:::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::*::::.    *#:: + .#-  =.+%#.. .+  =%%%%%%#-         .==. +=  :          -+::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::.-+==----=: #%* #.*%:  +%%*::: = .==*%%%##%%#-     :  -=: .# .*  .=-.     :+:::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::+=-#%%%#-:==%%#%=%%=-#%%= .:.-  *-%#-:+*#%%%%#:  .. :-+ ..#..=:  -*#%#*=. -=::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::+-#%%*+=#%=:#*#%%%%%%%%+:  .= + .%%#=++=-=+#%%%%*==::=%*--+#*. +**%%%%#*%%-=-::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::+=%%+::::*%.+= :=+%%*=..   =:=*=*%%%#.. - .-#%%%%%%#%%*+..:=%%#%%%%**##*#%*::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::+-%#:--==+%=-: .:-===:....:-+*++=####++:.:-++%%%%%%%%%#=-:::*%%%%+--:-##*#:::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::*=%=+==+#%-:.::::......  .:   #+:-**###%%##*#*+#%%%#-:::-=-=*%%%+:..:***=.::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::++-+-%%%+==-::::::::::-   .+=-%+=:+*=+*#%%#=*%#++#. :++*+=+-%%#%%#*++*+-.::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::+::-=%%*==-.:=+*=.   .:   ..:+--+++#%%%%##*=-:=-:+.=#%%=**+:-#%%#*++%.+:::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::+::--*#++%+ ..:=#=   ..::.. ..--##*##+-: .---. =+:**#*+=#%%*.:#%%#%*:-=::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::=-. :-+=.%:-....-+-.   .--:-:..:...+ .-=-.::==*-:+=- .:-=%*%=.=+%+#=:=::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::=+:.. .%# --:=*=:-===.  ::-----=+::--+*=-+.%%+#%%%#. :+%*%%+.::=-+=:::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::-=+#%%%-.+*-*%+%*+-**+-..:-:  .++*=:.:+  #%%%%%%#:+%+*#-*+ +-..==:::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::.=%#%%# :#=*%:=%#%*--=*#*--*.+=-...-#.---::%#*=.=##*++.-=.:-=-+::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::-#:#%%.-.**%-:%-#%%--..:##-*.:: .:+--.  -::+- +*+-*=-.--:+=:::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::-* :%%-.-:%%*.%=-%%%-+...*%:*   --:+  .-  =- :-==.*+-:*=+:.:::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::=:= -%= -.-%+-%*.%%%%:+.  #+-+ ::..:  -  .=  :.-:.=++*=+:.::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::+ =.:%+  =::*:##.%%%%=*.  ++-* -::: .:-=--:..::::..+*=+:::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::+  =:%=  -=..##=:*%%#-+-. +-:%..==*##**=+:####%##*##**::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::-=:.* %=  .=+:=%+#*%*#-*=. *.:* =#+*-- =-.%+=:.+:*-+==-::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::.*:-:+.#-   +*. #=*%%++-#+..=.=  #%%%%+=+-.%.:..-.*++-+:.:::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::-======-+--=---*-   .-  -#*#*#++*= +..  -%%#*#%##%#%%%%%%*#..==-.:::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::==.:+**###+ *+=.-=%*=: .   :+#*+**+::+:= =%%***####*##*%%%###::*+:::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::==. .#%%%%%* =-*=. :+%%%%#=.  ..=*###.-.:- +#**#*-=+:#.+:++-=*:# ==:::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::=+.  .#%%%%%#..* *=.   *:*%%#*=-.   .=%-.-=...-+*=+###*#+#***##%%+=*-:::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::+-=   +%%%%%* .*: +=:   *  =%%#%*+=-.  +::.-..-: #=--=---::::-==---#-::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::.-+. -.  *%%%%* .++  ==:   *. =%%%%%%%#*+..-:  . =  =++=-+**#%*+-:.==:+=::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::.-+:    =. :%%%*  +%...==:   =.=%%%%%#+%%%%#:..  . ::-+=:-=:==+*+----=.+++::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::-=-      ::  -%%=.+%+.: =-:  .:.+%%%#%%.-#%%%%*= .+- ::.:=*--=+=*==-=*:=-==::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::.:+  .:     =.  .#*-%%:.=-+-:  :=.+%%%#+%.:*%%%%%%%=     .=:-.-+:*.=.  -*:.::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::=+-  .+:    =    =%%*..+*==.  +::=%%++-*.%%%%%%%%##%= .-:  - :.=.  : . *:.::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::==-  =:   =+.   =    .#-.-+-==.  +:.+#%--:=.%%%%+: :*%%%#=.  :  :-:..    -=::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::-==.  .  =. . .*-   =.    -=:.:-+  :=: =#+-.:-.%%*. :#%%%%+:.=+===+++====*=+-:::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::-=-:     =   =:.-  =*.  -:    .=-:=*  -=. :#--.-..%=  +%%%%%%#+:......::::::+-:::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::-==:        # .:-=::-  .*.  =:     :***  -* .-+=:.::=: :%%%%*%%%%%%-....:::::::+=:::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::.:===:          -#-.   -..-   -   +       -*. -*.:+.%. .#  :%%%+=%%%%%%%#+:..:::::::+==::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::.:-=+==:            -==    .::+..-  .   .=        =====.* *= +: .%%*:+%%%%*%%%%-==::::::::=--::::::::::::::::::::::::::::::::::::::::::::::::::::
// .:-++++-:             :--.   :-+:   +..=      -          =++:+.:#+:  ###.*#%%%%=:%%%%.:*-:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// =-:.               :::   :::: :=-    +..=     :.           =**: %:- ==%==+%%%%%= .*%%#  ++==-:::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//                .:::   .--.            +. -     .             -+.#:+ *-%:-%%%%%%=   :*%+  =- :-====-:::::::::::::::::::::::::::::::::::::::::::::::::::
//     ....     ::.    :::                +: -.                   -*==+:#-:*%%%%%%=     :#-  :#-..   :-==+:::::::::::::::::::::::::::::::::::::::::::::::
// ...     .::-:   .::.                    =- ::             -.     :=**-=.#%#+%%%*    .  -=.  +.        :#=:::::::::::::::::::::::::::::::::::::::::::::
//   :----.    .:-:.                        -= :-        ..:=         .**+:#*=:%%%*     =+=-==  --        +.-=-::::::::::::::::::::::::::::::::::::::::::
//        :--.   ...                      .. .+ .-        :.-         =*.#+-=:.*%%*-.:.*%%%%#=+. .=.      +   :*==-::::::::::::::::::::::::::::::::::::::
//           :-:    :.                  .. .:  =. -.        .        -%: .+=::.=%%*+::*%%%%%%%++: .+:.:   -:    : :+=::::::::::::::::::::::::::::::::::::
//             .--   .:.                     :. =: ::               =%:    .-=::%%+*.*%%%%%#==%#==  :=+    -        #::::::::::::::::::::::::::::::::::::
//                --   .:.                    .. -- .-             =%*-       -=%%%:#%%%%%%:. .#:.=:  *    -       .+=:::::::::::::::::::::::::::::::::::
//                  =:   ::                     . := .-           =%%-=#-       :=-=%%%%%%#   .-   .-.-:   :.      : =+::::::::::::::::::::::::::::::::::
//                   .-.   :.                    . .=  -.        =%%%%::#%*-.    *#=+%+#%%=  .-    . :=:    :     .- -:+:::::::::::::::::::::::::::::::::
//                     :-   .-                    :  += ::      =%%%=+%. =%*=*+-*%%+:#:%%%: .=     -  :.          =: - .*-:::::::::::::::::::::::::::::::
//                       ::   :.                   :  =- ::    +%%+..:-+: .++ :-=#%#.+-#%%  =      +.   ..       :+. =   +-::::::::::::::::::::::::::::::
//                     ..  :.  .:                      :- .-  +%#:    :-=:. :+  . -%:-==#* =.      #    -       .:#  #:   *::::::::::::::::::::::::::::::
//                       ..  .   :.                     .= .-=.=-.      :--==..  .:-*:+--#+       -*    +       =.# :: .  *::::::::::::::::::::::::::::::
//                        .:  ..  ::                      =:-  . -=:       .:       =+#-.+*    -  #.   :=       --=.-  - :#::::::::::::::::::::::::::::::
//                          :.  :  .:                      .    .  :=:      .:     ..=%:.-%.  :- --    -=      .:*:=  :- ==-:::::::::::::::::::::::::::::

	// Server side
	void InediaInfectedAI_ResetADS()
	{
		if (GetInputController() && GetInputController().IsWeaponRaised()) {
			m_ResetADS = true;
			ExitSights();
#ifdef SERVER
			if (GetIdentity()) {
				ScriptRPC rpc = new ScriptRPC();
		    	rpc.Send(this, InediaInfectedAI_PlayerBase_ERPCs.RPC_RESET_ADS, true, GetIdentity());
			}
#endif
		}
	}

	bool InediaInfectedAI_IsInBlock()
	{
		return m_MeleeFightLogic && m_MeleeFightLogic.IsInBlock();
	}

	bool InediaInfectedAI_IsInMeleeHeavyAttack()
	{
		HumanCommandMelee2 hcm = GetCommand_Melee2();

		return hcm && hcm.GetCurrentHitType() == EMeleeHitType.HEAVY;
	}

	bool InediaInfectedAI_IsInRaisedMeleeStance()
	{
		GetMovementState(m_MovementState);

		if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT) {
			ItemBase itemHands = GetItemInHands();
			if (!itemHands || !itemHands.IsWeapon()) {
				// The player is in a stance with a melee weapon
				return true;
			}
		}

		return false;
	}

	bool InediaInfectedAI_IsInErectStance()
	{
		GetMovementState(m_MovementState);

		return m_MovementState && (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT || m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT);
	}

	bool InediaInfectedAI_IsInCrouchStance()
	{
		GetMovementState(m_MovementState);

		return m_MovementState && (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH || m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDCROUCH);
	}
	
	bool InediaInfectedAI_IsInProneStance()
	{
		GetMovementState(m_MovementState);

		return m_MovementState && (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE || m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE);
	}
	
	bool InediaInfectedAI_IsInIdleMovement()
	{
		GetMovementState(m_MovementState);
		
		return m_MovementState && m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_IDLE;
	}
	
	bool InediaInfectedAI_IsInWalkMovement()
	{
		GetMovementState(m_MovementState);
		
		return m_MovementState && m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_WALK;
	}
	
	bool InediaInfectedAI_IsInRunMovement()
	{
		GetMovementState(m_MovementState);
		
		return m_MovementState && (m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_RUN || m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_CROUCH_RUN || m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_SPRINT);
	}

	vector InediaInfectedAI_GetHeadPosition()
	{
		vector headPos = vector.Zero;

#ifdef SERVER
		// For some reason, this code crashes the client
		if (m_InediaInfectedAI_headBoneIndex != -1) {
			headPos = GetBonePositionWS(m_InediaInfectedAI_headBoneIndex);
		}
#endif

		// Just in case - the old method of getting the head position
		if (!headPos) {
			if (InediaInfectedAI_IsInProneStance() || !IsAlive()) {
				headPos = GetPosition() + 0.4 * vector.Up + 0.6 * GetDirection();
			} else if (InediaInfectedAI_IsInCrouchStance()) {
				headPos = GetPosition() + 1.05 * vector.Up + 0.46 * GetDirection();
			} else {
				headPos = GetPosition() + 1.7 * vector.Up + 0.06 * GetDirection();
			}
		}

		return headPos;
	}

	float InediaInfectedAI_GetToPositionDirectionAngleX(vector position)
	{
		vector creatureHitDirection = vector.Direction(GetPosition(), position);
		return Math.DiffAngle(creatureHitDirection.VectorToAngles()[0], GetDirection().VectorToAngles()[0]);
	}
	
	void InediaInfectedAI_ForceStance(DayZPlayerConstants stance)
	{
		if (GetCommand_Move()) {
			GetCommand_Move().ForceStance(stance);
		}

#ifdef SERVER
		if (GetIdentity()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(stance);
	    	rpc.Send(this, InediaInfectedAI_PlayerBase_ERPCs.RPC_FORCE_STANCE, true, GetIdentity());
		}
#endif
	}
	
	ref array<Particle> m_InediaInfectedAI_ActiveBloodParticles = new array<Particle>;
	void InediaInfectedAI_AddBloodParticleClient(Particle particle)
	{
#ifndef SERVER
		if (!particle) {
			return;
		}
		
		m_InediaInfectedAI_ActiveBloodParticles.Insert(particle);

		int oldParticleIndex = m_InediaInfectedAI_ActiveBloodParticles.Count() - m_InediaInfectedAI_BloodParticlesLimit - 1;
		if (oldParticleIndex >= 0) {
			// If the allowed number of blood particles is exceeded, stop the old ones.
			Particle oldestParticle = m_InediaInfectedAI_ActiveBloodParticles.Get(oldParticleIndex);
			if (oldestParticle && oldestParticle.IsParticlePlaying()) {
				oldestParticle.StopParticle(StopParticleFlags.IMMEDIATE);
			}
		}
#endif
	}
	void InediaInfectedAI_RemoveInactiveBloodParticlesClient()
	{
#ifndef SERVER
		for (int index = m_InediaInfectedAI_ActiveBloodParticles.Count() - 1; index >= 0; index--) {
			Particle currentParticle = m_InediaInfectedAI_ActiveBloodParticles.Get(index);
			if (!currentParticle || !currentParticle.IsParticlePlaying()) {
				m_InediaInfectedAI_ActiveBloodParticles.RemoveOrdered(index);
			}
		}
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InediaInfectedAI_RemoveInactiveBloodParticlesClient, 10000, false);
#endif
	}

// DEBUG METHODS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	int m_InediaInfectedAI_ShowDebugSmellsLastUpdateTime = 0;
	float m_InediaInfectedAI_ShowDebugSmellsTimer = 0;
	bool m_SmellsDebugIsActive = false;
	void InediaInfectedAI_ShowDebugSmellsHandler(float pDt)
	{
		if (!m_InediaInfectedAI_ConfigPlayers.SmellsHandlerIsActive) return;

		m_InediaInfectedAI_ShowDebugSmellsTimer -= pDt;
		if (m_InediaInfectedAI_ShowDebugSmellsTimer > 0) return;
		m_InediaInfectedAI_ShowDebugSmellsTimer = 1;
		
		if (InediaInfectedAI_Debug.IsDebugRuleEnabled("smells")) {
			m_SmellsDebugIsActive = true;
			
			if (m_InediaInfectedAI_ShowDebugSmellsLastUpdateTime == m_InediaInfectedAI_SmellsInstance.m_Smells_LastUpdateTime) {
				return;
			} else {
				m_InediaInfectedAI_ShowDebugSmellsLastUpdateTime = m_InediaInfectedAI_SmellsInstance.m_Smells_LastUpdateTime;
			}
			
			if (m_InediaInfectedAI_SmellsInstance.IsUpdating()) {
				return;
			}
	
			array<vector> positions = new array<vector>;
			array<float> radiuses = new array<float>;
			foreach (InediaInfectedAI_Smell smell: m_InediaInfectedAI_SmellsInstance.m_Smells) {
				if (smell.m_Player == this) {
					positions.Insert(smell.m_Position);
					radiuses.Insert(smell.m_Radius * m_InediaInfectedAI_SmellsInstance.GetRainModifier() * m_InediaInfectedAI_SmellsInstance.GetFogModifier());
				}
			}
	
			InediaInfectedAI_DebugShowSmellsShapes(positions, radiuses);
		} else {
			if (m_SmellsDebugIsActive) {
				m_SmellsDebugIsActive = false;
				
				InediaInfectedAI_DebugHideSmellsShapes();
			}
			
			return;
		}
	}

	void InediaInfectedAI_DebugShowSmellsShapes(array<vector> positions, array<float> radiuses)
	{
#ifdef SERVER
		if (GetIdentity()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(positions);
			rpc.Write(radiuses);
	
			rpc.Send(this, InediaInfectedAI_PlayerBase_ERPCs.RPC_SHOW_SMELLS, false, GetIdentity());
		}
#else
		InediaInfectedAI_DebugHideSmellsShapes();
		
		foreach (int index, vector smellPosition: positions) {
			m_InediaInfectedAI_DebugSmellsShapes.Insert(Debug.DrawSphere(smellPosition, radiuses.Get(index), 0x33AA0000, ShapeFlags.TRANSP));
		}
#endif
	}

	void InediaInfectedAI_DebugHideSmellsShapes()
	{
#ifdef SERVER
		if (GetIdentity()) {
			array<vector> positions = new array<vector>;
			array<float> radiuses = new array<float>;
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(positions);
			rpc.Write(radiuses);
	
			rpc.Send(this, InediaInfectedAI_PlayerBase_ERPCs.RPC_SHOW_SMELLS, true, GetIdentity());
		}
#else
		foreach (Shape shapeForRemove: m_InediaInfectedAI_DebugSmellsShapes) {
			Debug.RemoveShape(shapeForRemove);
		}
		m_InediaInfectedAI_DebugSmellsShapes.Clear();
#endif
	}
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Modding classes that affect various character mechanics,
// which I chose not to move to separate files for convenience,
// as it would be difficult to track all these changes later.

modded class BleedingSourcesManagerServer extends BleedingSourcesManagerBase
{
	// To remove vanilla bleeding without triggering sepsis.
	void InediaInfectedAI_RemoveMostSignificantBleedingSourceWithoutSepsis()
	{
		int bit = GetMostSignificantBleedingSource();
		if (bit != 0) {
			if (m_BleedingSources.Contains(bit)) {
				BleedingSourceZone bsz = GetBleedingSourceMeta(bit);
				int inventory_loc = bsz.GetInvLocation();
				m_BleedingSourcesByLocation.Set(inventory_loc, (m_BleedingSourcesByLocation.Get(inventory_loc) & ~bit));//deactivate the bit
				m_BleedingSources.Remove(bit);

				m_Player.OnBleedingSourceRemovedEx(m_Item);
			}

			int inverse_bit_mask = ~bit;
			m_Player.SetBleedingBits(m_Player.GetBleedingBits() & inverse_bit_mask);

			m_Item = null;//reset, so that next call, if induced by self-healing, will have no item
		}
	}

	override protected void AddBleedingSource(int bit)
	{
		if (m_Player && !m_Player.GetAllowDamage()) {
			return;
		}

		super.AddBleedingSource(bit);
	}
	
	// To update the blood loss level on the client (PlayerBase.m_InediaInfectedAI_BloodLossLevel).
	float m_InediaInfectedAI_BloodLossLevelUpdateTick = 0;
	override void OnTick(float delta_time)
	{
#ifdef SERVER
		m_InediaInfectedAI_BloodLossLevelUpdateTick += delta_time;
		if (m_InediaInfectedAI_BloodLossLevelUpdateTick > TICK_INTERVAL_SEC) {
			if (m_Player) {
				int bloodLossLevelBefore = m_Player.m_InediaInfectedAI_BloodLossLevel;
				
				if (m_Player.IsAlive()) {
					m_Player.m_InediaInfectedAI_BloodLossLevel = MiscGameplayFunctions.InediaInfectedAI_GetBloodLossLevelFromBloodLossRatePerSec(InediaInfectedAI_GetCutBloodLossRatePerSec() + m_Player.InediaInfectedAI_GetPainManager().GetDeepWoundsBleedingRateWithMultipliers());
				} else {
					if (m_Player.m_InediaInfectedAI_BloodLossLevel > 0) {
						m_Player.m_InediaInfectedAI_BloodLossLevel = 0;
					}
				}
				
				if (m_Player.m_InediaInfectedAI_BloodLossLevel != bloodLossLevelBefore) {
					m_Player.SetSynchDirty();
				}
			}
			
			m_InediaInfectedAI_BloodLossLevelUpdateTick = 0;
		}
#endif
		
		super.OnTick(delta_time);
	}
	
	// Gets the bleeding rate per second.
	float InediaInfectedAI_GetCutBloodLossRatePerSec()
	{
		float bloodLossRatePerSec = 0;
		
		if (m_Player && m_BleedingSources && m_BleedingSources.Count() > 0) {
			float blood_scale = Math.InverseLerp(PlayerConstants.BLOOD_THRESHOLD_FATAL, PlayerConstants.BLEEDING_LOW_PRESSURE_BLOOD, m_Player.GetHealth("GlobalHealth", "Blood"));
			blood_scale = Math.Clamp(blood_scale, PlayerConstants.BLEEDING_LOW_PRESSURE_MIN_MOD, 1);
		
			BleedingSource bleedingSource;
			for (int i = 0; i < m_BleedingSources.Count(); i++) {
				if (!m_DisableBloodLoss) {
					bleedingSource = m_BleedingSources.GetElement(i);
					if (bleedingSource) {
						float flow = bleedingSource.m_FlowModifier;
						if (bleedingSource.m_Type == eBleedingSourceType.CONTAMINATED) {
							flow *= PlayerConstants.BLEEDING_SOURCE_BURN_MODIFIER;
						}
						
						bloodLossRatePerSec += -PlayerConstants.BLEEDING_SOURCE_BLOODLOSS_PER_SEC * blood_scale * flow;
					}
				}
			}
		}
		
		if (bloodLossRatePerSec > 0 && m_Player && m_Player.GetModifiersManager().IsModifierActive(InediaInfectedAI_eModifiers.MDF_BLOOD_CLOTTING)) {
			if (m_Player.InediaInfectedAI_GetPainManager().m_BloodClottingItemClottingMultiplier > 1) {
				bloodLossRatePerSec /= m_Player.InediaInfectedAI_GetPainManager().m_BloodClottingItemClottingMultiplier;
			}
		}
		
		if (m_Player && m_Player.InediaInfectedAI_GetPainManager().LimbsDeepWoundSystemIsActive() && m_Player.m_InediaInfectedAI_ConfigPlayers && !m_Player.InediaInfectedAI_IsNPC()) {
			float deepWoundsVanillaCutsMultiplier = Math.Clamp(m_Player.m_InediaInfectedAI_ConfigPlayers.LimbsDeepWoundVanillaCutBleedRateMultiplier, 0.001, 1);
			bloodLossRatePerSec *= deepWoundsVanillaCutsMultiplier;
		}
		
		return bloodLossRatePerSec;
	}
}

modded class BleedingSourcesManagerBase
{
	int InediaInfectedAI_GetBitFromSelectionName(string name)
	{
		name.ToLower();
		
		if (m_BleedingSourceZone.Contains(name)) {
			return m_BleedingSourceZone.Get(name).GetBit();
		}
		
		return 0;
	}
	
	BleedingSourceZone InediaInfectedAI_GetBleedingSourceMeta(int bit)
	{
		return m_BleedingSourceZone.Get(GetSelectionNameFromBit(bit));
	}
	
	int InediaInfectedAI_GetBitFromSelectionID(int id)
	{
		CachedObjectsArrays.ARRAY_STRING.Clear();
		m_Player.GetActionComponentNameList(id, CachedObjectsArrays.ARRAY_STRING, "fire");
		
		for (int i = 0; i < CachedObjectsArrays.ARRAY_STRING.Count(); i++) {
			int bit = GetBitFromSelectionName(CachedObjectsArrays.ARRAY_STRING.Get(i));
			if (bit != 0) {
				return bit;
			}
		}
		
		return 0;
	}
}

// The logic of camera shaking for the player when taking hits (1st person).
modded class DayZPlayerCamera1stPerson extends DayZPlayerCameraBase
{
	PlayerBase m_PlayerPb;
	override void Init()
	{
		m_PlayerPb = PlayerBase.Cast(m_pPlayer);

		super.Init();
	}

	override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		super.OnUpdate(pDt, pOutResult);

		if (m_PlayerPb) {
			if (m_PlayerPb.m_InediaInfectedAI_CameraShakeSeconds > 0) {
				if (m_PlayerPb.m_InediaInfectedAI_CameraShakeIntensity > 0) {
					float camera_shake_offset = m_PlayerPb.m_InediaInfectedAI_CameraShakeIntensity * m_PlayerPb.GetRandomGeneratorSyncManager().GetRandomInRange(RandomGeneratorSyncUsage.RGSAimingModel, -0.5, 0.5);
					vector rot;
			        rot[0] = camera_shake_offset;
			        rot[1] = camera_shake_offset;
			        rot[2] = camera_shake_offset;

					Math3D.YawPitchRollMatrix(GetCurrentOrientation() + rot, pOutResult.m_CameraTM);
				}

				m_PlayerPb.m_InediaInfectedAI_CameraShakeSeconds -= pDt;
			}
		}
	}
};

// The logic of camera shaking for the player when taking hits (3rd person).
modded class DayZPlayerCamera3rdPerson extends DayZPlayerCameraBase
{
	PlayerBase m_PlayerPb;
	override void Init()
	{
		m_PlayerPb = PlayerBase.Cast(m_pPlayer);

		super.Init();
	}

	override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		super.OnUpdate(pDt, pOutResult);

		if (m_PlayerPb) {
			if (m_PlayerPb.m_InediaInfectedAI_CameraShakeSeconds > 0) {
				if (m_PlayerPb.m_InediaInfectedAI_CameraShakeIntensity > 0) {
					float camera_shake_offset = m_PlayerPb.m_InediaInfectedAI_CameraShakeIntensity * m_PlayerPb.GetRandomGeneratorSyncManager().GetRandomInRange(RandomGeneratorSyncUsage.RGSAimingModel, -0.5, 0.5);
					vector rot;
			        rot[0] = camera_shake_offset;
			        rot[1] = camera_shake_offset;
			        rot[2] = camera_shake_offset;

					Math3D.YawPitchRollMatrix(GetCurrentOrientation() + rot, pOutResult.m_CameraTM);
				}

				m_PlayerPb.m_InediaInfectedAI_CameraShakeSeconds -= pDt;
			}
		}
	}
}

// Disabling fall damage in god mode.
modded class DayZPlayerImplementFallDamage
{
	override void HandleFallDamage(FallDamageData pData)
	{
#ifdef SERVER
		PlayerBase playerBase = PlayerBase.Cast(m_Player);
		if (playerBase && !playerBase.GetAllowDamage()) {
			return;
		}
#endif

		super.HandleFallDamage(pData);
	}
}

// Adding and activating modifiers.
modded class ModifiersManager
{
	override void Init()
	{
		super.Init();

		AddModifier(new InediaInfectedAI_PainMdfr);
		AddModifier(new InediaInfectedAI_HealthRegenMdfr);
		AddModifier(new InediaInfectedAI_BoneRegenMdfr);
		AddModifier(new InediaInfectedAI_BloodRegenMdfr);
		AddModifier(new InediaInfectedAI_BloodClottingMdfr);
		AddModifier(new InediaInfectedAI_DeepWoundRegenMdfr);
		AddModifier(new InediaInfectedAI_VomitRelieMdfr);
		AddModifier(new InediaInfectedAI_PainkillerStrongMdfr);
		AddModifier(new InediaInfectedAI_PainkillerMediumMdfr);
		AddModifier(new InediaInfectedAI_PainkillerLightMdfr);
	}
	
	override void ActivateModifier(int modifier_id, bool triggerEvent = EActivationType.TRIGGER_EVENT_ON_ACTIVATION)
	{
		if (m_Player) {
			if (!m_Player.GetAllowDamage()) {
				return;
			}
			
			if (modifier_id == eModifiers.MDF_AREAEXPOSURE || modifier_id == eModifiers.MDF_CONTAMINATION1 || modifier_id == eModifiers.MDF_CONTAMINATION2 || modifier_id == eModifiers.MDF_CONTAMINATION3) {
				if (m_Player.m_InediaInfectedAI_ConfigZombie && m_Player.m_InediaInfectedAI_ConfigZombie.ResistContaminatedEffectHandlerIsActive && m_Player.m_InediaInfectedAI_ConfigZombie.ResistContaminatedEffect) {
					return;
				}
			}
		}

		super.ActivateModifier(modifier_id, triggerEvent);
	}
}

// Hand shaking functionality when experiencing arm pain if "sGunplay" is not used.
// For "sGunplay", a separate implementation is used in the "sgunplay_support.c" class.
#ifndef S_GUNPLAY
    modded class DayZPlayerImplementAiming
    {
        override bool ProcessAimFilters(float pDt, SDayZPlayerAimingModel pModel, int stance_index)
        {
            if (super.ProcessAimFilters(pDt, pModel, stance_index)) {
                if (stance_index >= DayZPlayerConstants.STANCEIDX_RAISEDERECT && m_PlayerPb) {
                    m_PlayerPb.InediaInfectedAI_ProcessAimFilters(pDt, pModel, stance_index);
                }

                return true;
            }

            return false;
        }
    }
#endif


// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::-======-----=======-:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::-===-:.                 .::-====-::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::-==-:                              .:====::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::-==:                                     ..:-==-::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::.-+-    .....    .                             ::.:==::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::.-+:           .::: ...                            .:..==::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::+-                 ::: .::                           .. .+-::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::-+                     .-:  :.          .:...           .-  :+:::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::=+    -.           .:--.   -: .-           .-.             -  .*-:::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::-+    :=         .....:=%#=...:. :..                ....     -   +=::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::*     =.                 =##:-..:   ..:::.              --    -   +=:::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::=-   .+.               :-=--%%::-::..     .--:   .::.           .   *-::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::#   .=             ..::...=*%%% -:.- .       .-:    :--=-:..   ...  .*::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::*    .                .-*%%%%%#-= :. ---:.     .-: ..   .:----::...  -=:::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::-+                     .::-%%%%+. . ..    :-:::   :-  :::   ..---::..  *..:::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::-=              :         .*%%:       .:..::..:--=:.-.   .:::. ..   .  -=.:::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::-=       :.     -:  =     .#%=    ::::.     :--+:.-*--: :.   .-. .:-:   +::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::*        *+ .  =..:+ .   :%%   .+.  ::-::   .:=:.  =*--.-*+-. ...  .  .-*:::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::*::::.    *#:: + .#-  =.+%#.. .+  =%%%%%%#-         .==. +=  :          -+::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::.-+==----=: #%* #.*%:  +%%*::: = .==*%%%##%%#-     :  -=: .# .*  .=-.     :+:::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::+=-#%%%#-:==%%#%=%%=-#%%= .:.-  *-%#-:+*#%%%%#:  .. :-+ ..#..=:  -*#%#*=. -=::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::+-#%%*+=#%=:#*#%%%%%%%%+:  .= + .%%#=++=-=+#%%%%*==::=%*--+#*. +**%%%%#*%%-=-::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::+=%%+::::*%.+= :=+%%*=..   =:=*=*%%%#.. - .-#%%%%%%#%%*+..:=%%#%%%%**##*#%*::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::+-%#:--==+%=-: .:-===:....:-+*++=####++:.:-++%%%%%%%%%#=-:::*%%%%+--:-##*#:::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::*=%=+==+#%-:.::::......  .:   #+:-**###%%##*#*+#%%%#-:::-=-=*%%%+:..:***=.::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::++-+-%%%+==-::::::::::-   .+=-%+=:+*=+*#%%#=*%#++#. :++*+=+-%%#%%#*++*+-.::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::+::-=%%*==-.:=+*=.   .:   ..:+--+++#%%%%##*=-:=-:+.=#%%=**+:-#%%#*++%.+:::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::+::--*#++%+ ..:=#=   ..::.. ..--##*##+-: .---. =+:**#*+=#%%*.:#%%#%*:-=::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::=-. :-+=.%:-....-+-.   .--:-:..:...+ .-=-.::==*-:+=- .:-=%*%=.=+%+#=:=::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::=+:.. .%# --:=*=:-===.  ::-----=+::--+*=-+.%%+#%%%#. :+%*%%+.::=-+=:::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::-=+#%%%-.+*-*%+%*+-**+-..:-:  .++*=:.:+  #%%%%%%#:+%+*#-*+ +-..==:::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::.=%#%%# :#=*%:=%#%*--=*#*--*.+=-...-#.---::%#*=.=##*++.-=.:-=-+::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::-#:#%%.-.**%-:%-#%%--..:##-*.:: .:+--.  -::+- +*+-*=-.--:+=:::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::-* :%%-.-:%%*.%=-%%%-+...*%:*   --:+  .-  =- :-==.*+-:*=+:.:::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::=:= -%= -.-%+-%*.%%%%:+.  #+-+ ::..:  -  .=  :.-:.=++*=+:.::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::+ =.:%+  =::*:##.%%%%=*.  ++-* -::: .:-=--:..::::..+*=+:::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::+  =:%=  -=..##=:*%%#-+-. +-:%..==*##**=+:####%##*##**::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::-=:.* %=  .=+:=%+#*%*#-*=. *.:* =#+*-- =-.%+=:.+:*-+==-::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::.*:-:+.#-   +*. #=*%%++-#+..=.=  #%%%%+=+-.%.:..-.*++-+:.:::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::-======-+--=---*-   .-  -#*#*#++*= +..  -%%#*#%##%#%%%%%%*#..==-.:::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::==.:+**###+ *+=.-=%*=: .   :+#*+**+::+:= =%%***####*##*%%%###::*+:::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::==. .#%%%%%* =-*=. :+%%%%#=.  ..=*###.-.:- +#**#*-=+:#.+:++-=*:# ==:::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::=+.  .#%%%%%#..* *=.   *:*%%#*=-.   .=%-.-=...-+*=+###*#+#***##%%+=*-:::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::+-=   +%%%%%* .*: +=:   *  =%%#%*+=-.  +::.-..-: #=--=---::::-==---#-::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::.-+. -.  *%%%%* .++  ==:   *. =%%%%%%%#*+..-:  . =  =++=-+**#%*+-:.==:+=::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::.-+:    =. :%%%*  +%...==:   =.=%%%%%#+%%%%#:..  . ::-+=:-=:==+*+----=.+++::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::-=-      ::  -%%=.+%+.: =-:  .:.+%%%#%%.-#%%%%*= .+- ::.:=*--=+=*==-=*:=-==::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::.:+  .:     =.  .#*-%%:.=-+-:  :=.+%%%#+%.:*%%%%%%%=     .=:-.-+:*.=.  -*:.::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::=+-  .+:    =    =%%*..+*==.  +::=%%++-*.%%%%%%%%##%= .-:  - :.=.  : . *:.::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::==-  =:   =+.   =    .#-.-+-==.  +:.+#%--:=.%%%%+: :*%%%#=.  :  :-:..    -=::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::-==.  .  =. . .*-   =.    -=:.:-+  :=: =#+-.:-.%%*. :#%%%%+:.=+===+++====*=+-:::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::-=-:     =   =:.-  =*.  -:    .=-:=*  -=. :#--.-..%=  +%%%%%%#+:......::::::+-:::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::-==:        # .:-=::-  .*.  =:     :***  -* .-+=:.::=: :%%%%*%%%%%%-....:::::::+=:::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::.:===:          -#-.   -..-   -   +       -*. -*.:+.%. .#  :%%%+=%%%%%%%#+:..:::::::+==::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::.:-=+==:            -==    .::+..-  .   .=        =====.* *= +: .%%*:+%%%%*%%%%-==::::::::=--::::::::::::::::::::::::::::::::::::::::::::::::::::
// .:-++++-:             :--.   :-+:   +..=      -          =++:+.:#+:  ###.*#%%%%=:%%%%.:*-:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// =-:.               :::   :::: :=-    +..=     :.           =**: %:- ==%==+%%%%%= .*%%#  ++==-:::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//                .:::   .--.            +. -     .             -+.#:+ *-%:-%%%%%%=   :*%+  =- :-====-:::::::::::::::::::::::::::::::::::::::::::::::::::
//     ....     ::.    :::                +: -.                   -*==+:#-:*%%%%%%=     :#-  :#-..   :-==+:::::::::::::::::::::::::::::::::::::::::::::::
// ...     .::-:   .::.                    =- ::             -.     :=**-=.#%#+%%%*    .  -=.  +.        :#=:::::::::::::::::::::::::::::::::::::::::::::
//   :----.    .:-:.                        -= :-        ..:=         .**+:#*=:%%%*     =+=-==  --        +.-=-::::::::::::::::::::::::::::::::::::::::::
//        :--.   ...                      .. .+ .-        :.-         =*.#+-=:.*%%*-.:.*%%%%#=+. .=.      +   :*==-::::::::::::::::::::::::::::::::::::::
//           :-:    :.                  .. .:  =. -.        .        -%: .+=::.=%%*+::*%%%%%%%++: .+:.:   -:    : :+=::::::::::::::::::::::::::::::::::::
//             .--   .:.                     :. =: ::               =%:    .-=::%%+*.*%%%%%#==%#==  :=+    -        #::::::::::::::::::::::::::::::::::::
//                --   .:.                    .. -- .-             =%*-       -=%%%:#%%%%%%:. .#:.=:  *    -       .+=:::::::::::::::::::::::::::::::::::
//                  =:   ::                     . := .-           =%%-=#-       :=-=%%%%%%#   .-   .-.-:   :.      : =+::::::::::::::::::::::::::::::::::
//                   .-.   :.                    . .=  -.        =%%%%::#%*-.    *#=+%+#%%=  .-    . :=:    :     .- -:+:::::::::::::::::::::::::::::::::
//                     :-   .-                    :  += ::      =%%%=+%. =%*=*+-*%%+:#:%%%: .=     -  :.          =: - .*-:::::::::::::::::::::::::::::::
//                       ::   :.                   :  =- ::    +%%+..:-+: .++ :-=#%#.+-#%%  =      +.   ..       :+. =   +-::::::::::::::::::::::::::::::
//                     ..  :.  .:                      :- .-  +%#:    :-=:. :+  . -%:-==#* =.      #    -       .:#  #:   *::::::::::::::::::::::::::::::
//                       ..  .   :.                     .= .-=.=-.      :--==..  .:-*:+--#+       -*    +       =.# :: .  *::::::::::::::::::::::::::::::
//                        .:  ..  ::                      =:-  . -=:       .:       =+#-.+*    -  #.   :=       --=.-  - :#::::::::::::::::::::::::::::::
//                          :.  :  .:                      .    .  :=:      .:     ..=%:.-%.  :- --    -=      .:*:=  :- ==-:::::::::::::::::::::::::::::
