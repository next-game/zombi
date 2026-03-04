/**
 * Created: 2023-09-01
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

enum InediaInfectedAI_Irritants
{
	FLASHLIGHTS_VISUAL,
	ROADFLARE_VISUAL,
	ROADFLARE_NOISE,
	CHEMLIGHT_VISUAL,
	PORTABLE_GAS_LAMP_VISUAL,
	FLAMMABLES_VISUAL,
	SPOTLIGHT_VISUAL,
	FIREPLACE_VISUAL,
	CAR_LIGHT_VISUAL,
	SMOKE_GRENADE_VISUAL,
	CAR_HORN_NOISE,
	CAR_ENGINE_NOISE,
	EXPLOSIVE_ITEM_NOISE,
	FIREWORKS_NOISE,
	FUEL_STATION_NOISE,
	POWER_GENERATOR_NOISE,
	ALARM_CLOCK_NOISE,
	WEAPON_SHOT_NOISE,
	FRIENDLY_WEAPON_SHOT_NOISE,
	KITCHEN_TIMER_NOISE,
	HEADTORCHRED_VISUAL,
	CHEMLIGHTRED_VISUAL,
	DOORS_NOISE,
	INFECTED_SCREAM_NOISE,
	INFECTED_BODYFALL_NOISE,
	INFECTED_BODYFALL_BACKSTAB_NOISE,
	HORSE_HOOFSTEPS_NOISE,
	PLAYER_FOOTSTEPS_JOG_NOISE,
	PLAYER_FOOTSTEPS_CROUCH_SPRINT_NOISE,
	PLAYER_FOOTSTEPS_SPRINT_NOISE,
	PLAYER_SYMPTOMS_NOISE,
	PLAYER_VOICE_WHISPER_NOISE,
	PLAYER_VOICE_TALK_NOISE,
	PLAYER_VOICE_SHOUT_NOISE,
	PLAYER_FALL_NOISE,
	PLAYER_MINING_LIGHT_NOISE,
	PLAYER_MINING_HARD_NOISE,
	PLAYER_BUILDING_NOISE,
	PLAYER_SAW_PLANKS_NOISE,
	MOD_MASS_SEARCH_CUSTOM,
	MOD_MASS_SEARCH_VISUAL,
	MOD_MASS_SEARCH_NOISE,
	CUSTOM
};

enum InediaInfectedAI_IrritantsTypes
{
	VISUAL,
	NOISE,
	CUSTOM
};

class InediaInfectedAI_IrritantsManager
{
	static InediaInfectedAI_IrritantsTypes GetIrritantType(InediaInfectedAI_Irritants irritant, EntityAI irritationObject)
	{
		switch (irritant) {
		    case InediaInfectedAI_Irritants.FLASHLIGHTS_VISUAL:
		    case InediaInfectedAI_Irritants.ROADFLARE_VISUAL:
		    case InediaInfectedAI_Irritants.CHEMLIGHT_VISUAL:
		    case InediaInfectedAI_Irritants.PORTABLE_GAS_LAMP_VISUAL:
		    case InediaInfectedAI_Irritants.FLAMMABLES_VISUAL:
		    case InediaInfectedAI_Irritants.SPOTLIGHT_VISUAL:
		    case InediaInfectedAI_Irritants.FIREPLACE_VISUAL:
		    case InediaInfectedAI_Irritants.CAR_LIGHT_VISUAL:
		    case InediaInfectedAI_Irritants.SMOKE_GRENADE_VISUAL:
		    case InediaInfectedAI_Irritants.HEADTORCHRED_VISUAL:
		    case InediaInfectedAI_Irritants.CHEMLIGHTRED_VISUAL:
		    case InediaInfectedAI_Irritants.MOD_MASS_SEARCH_VISUAL:
				return InediaInfectedAI_IrritantsTypes.VISUAL;
			break;

		    case InediaInfectedAI_Irritants.ROADFLARE_NOISE:
		    case InediaInfectedAI_Irritants.CAR_HORN_NOISE:
		    case InediaInfectedAI_Irritants.CAR_ENGINE_NOISE:
		    case InediaInfectedAI_Irritants.EXPLOSIVE_ITEM_NOISE:
		    case InediaInfectedAI_Irritants.FIREWORKS_NOISE:
		    case InediaInfectedAI_Irritants.FUEL_STATION_NOISE:
		    case InediaInfectedAI_Irritants.POWER_GENERATOR_NOISE:
		    case InediaInfectedAI_Irritants.ALARM_CLOCK_NOISE:
		    case InediaInfectedAI_Irritants.INFECTED_SCREAM_NOISE:
		    case InediaInfectedAI_Irritants.WEAPON_SHOT_NOISE:
		    case InediaInfectedAI_Irritants.FRIENDLY_WEAPON_SHOT_NOISE:
		    case InediaInfectedAI_Irritants.KITCHEN_TIMER_NOISE:
		    case InediaInfectedAI_Irritants.HORSE_HOOFSTEPS_NOISE:
		    case InediaInfectedAI_Irritants.PLAYER_FOOTSTEPS_JOG_NOISE:
		    case InediaInfectedAI_Irritants.PLAYER_FOOTSTEPS_CROUCH_SPRINT_NOISE:
		    case InediaInfectedAI_Irritants.PLAYER_FOOTSTEPS_SPRINT_NOISE:
		    case InediaInfectedAI_Irritants.DOORS_NOISE:
		    case InediaInfectedAI_Irritants.PLAYER_FALL_NOISE:
		    case InediaInfectedAI_Irritants.INFECTED_BODYFALL_NOISE:
			case InediaInfectedAI_Irritants.INFECTED_BODYFALL_BACKSTAB_NOISE:
		    case InediaInfectedAI_Irritants.PLAYER_SYMPTOMS_NOISE:
		    case InediaInfectedAI_Irritants.PLAYER_VOICE_WHISPER_NOISE:
		    case InediaInfectedAI_Irritants.PLAYER_VOICE_TALK_NOISE:
		    case InediaInfectedAI_Irritants.PLAYER_VOICE_SHOUT_NOISE:
		    case InediaInfectedAI_Irritants.PLAYER_MINING_LIGHT_NOISE:
		    case InediaInfectedAI_Irritants.PLAYER_MINING_HARD_NOISE:
		    case InediaInfectedAI_Irritants.PLAYER_BUILDING_NOISE:
		    case InediaInfectedAI_Irritants.PLAYER_SAW_PLANKS_NOISE:
		    case InediaInfectedAI_Irritants.MOD_MASS_SEARCH_NOISE:
				return InediaInfectedAI_IrritantsTypes.NOISE;
			break;
			
			case InediaInfectedAI_Irritants.CUSTOM:
				if (irritationObject && irritationObject.IsInherited(ItemBase)) {
					ItemBase irritationItem = ItemBase.Cast(irritationObject);
					if (irritationItem && irritationItem.m_InediaInfectedAI_CustomIrritant) {
						if (irritationItem.m_InediaInfectedAI_CustomIrritant.Type == InediaInfectedAI_IrritantsTypes.VISUAL) {
							return InediaInfectedAI_IrritantsTypes.VISUAL;
						} else if (irritationItem.m_InediaInfectedAI_CustomIrritant.Type == InediaInfectedAI_IrritantsTypes.NOISE) {
							return InediaInfectedAI_IrritantsTypes.NOISE;
						}
					}
				}
			break;
		}

		return InediaInfectedAI_IrritantsTypes.CUSTOM;
	}
	
	static int GetIrritantPriority(InediaInfectedAI_Irritants irritant, EntityAI irritationObject)
	{
		switch (irritant) {
			case InediaInfectedAI_Irritants.CHEMLIGHT_VISUAL:
			case InediaInfectedAI_Irritants.CHEMLIGHTRED_VISUAL:
			case InediaInfectedAI_Irritants.INFECTED_BODYFALL_NOISE:
			case InediaInfectedAI_Irritants.INFECTED_BODYFALL_BACKSTAB_NOISE:
				return 1;
			break;
			
			case InediaInfectedAI_Irritants.FLASHLIGHTS_VISUAL:
			case InediaInfectedAI_Irritants.HEADTORCHRED_VISUAL:
			case InediaInfectedAI_Irritants.KITCHEN_TIMER_NOISE:
			case InediaInfectedAI_Irritants.ALARM_CLOCK_NOISE:
				return 2;
			break;
			
			case InediaInfectedAI_Irritants.ROADFLARE_VISUAL:
			case InediaInfectedAI_Irritants.ROADFLARE_NOISE:
			case InediaInfectedAI_Irritants.PORTABLE_GAS_LAMP_VISUAL:
			case InediaInfectedAI_Irritants.PLAYER_MINING_LIGHT_NOISE:
			case InediaInfectedAI_Irritants.PLAYER_BUILDING_NOISE:
			case InediaInfectedAI_Irritants.PLAYER_SAW_PLANKS_NOISE:
			case InediaInfectedAI_Irritants.SMOKE_GRENADE_VISUAL:
			case InediaInfectedAI_Irritants.FLAMMABLES_VISUAL:
				return 3;
			break;
			
			case InediaInfectedAI_Irritants.FIREPLACE_VISUAL:
			case InediaInfectedAI_Irritants.CAR_LIGHT_VISUAL:
			case InediaInfectedAI_Irritants.CAR_HORN_NOISE:
			case InediaInfectedAI_Irritants.CAR_ENGINE_NOISE:
			case InediaInfectedAI_Irritants.PLAYER_MINING_HARD_NOISE:
			case InediaInfectedAI_Irritants.DOORS_NOISE:
			case InediaInfectedAI_Irritants.POWER_GENERATOR_NOISE:
			case InediaInfectedAI_Irritants.SPOTLIGHT_VISUAL:
				return 4;
			break;
			
			case InediaInfectedAI_Irritants.WEAPON_SHOT_NOISE:
			case InediaInfectedAI_Irritants.FRIENDLY_WEAPON_SHOT_NOISE:
			case InediaInfectedAI_Irritants.HORSE_HOOFSTEPS_NOISE:
			case InediaInfectedAI_Irritants.PLAYER_FOOTSTEPS_JOG_NOISE:
			case InediaInfectedAI_Irritants.PLAYER_FOOTSTEPS_CROUCH_SPRINT_NOISE:
			case InediaInfectedAI_Irritants.PLAYER_FOOTSTEPS_SPRINT_NOISE:
			case InediaInfectedAI_Irritants.PLAYER_VOICE_WHISPER_NOISE:
			case InediaInfectedAI_Irritants.PLAYER_VOICE_TALK_NOISE:
			case InediaInfectedAI_Irritants.PLAYER_VOICE_SHOUT_NOISE:
			case InediaInfectedAI_Irritants.PLAYER_FALL_NOISE:
			case InediaInfectedAI_Irritants.PLAYER_SYMPTOMS_NOISE:
			case InediaInfectedAI_Irritants.INFECTED_SCREAM_NOISE:
			case InediaInfectedAI_Irritants.FIREWORKS_NOISE:
			case InediaInfectedAI_Irritants.EXPLOSIVE_ITEM_NOISE:
			case InediaInfectedAI_Irritants.FUEL_STATION_NOISE:
				return 777;
			break;
			
			case InediaInfectedAI_Irritants.MOD_MASS_SEARCH_CUSTOM:
			case InediaInfectedAI_Irritants.MOD_MASS_SEARCH_VISUAL:
			case InediaInfectedAI_Irritants.MOD_MASS_SEARCH_NOISE:
				return m_ModMassSearchCurrentIrritantPriority;
			break;
			
			case InediaInfectedAI_Irritants.CUSTOM:
				if (irritationObject) {
					ItemBase irritationItem = ItemBase.Cast(irritationObject);
					if (irritationItem && irritationItem.m_InediaInfectedAI_CustomIrritant) {
						return irritationItem.m_InediaInfectedAI_CustomIrritant.Priority;
					}
				}
			break;
		}

		return 1;
	}
	
	static float GetIrritantRadius(InediaInfectedAI_Irritants irritant, EntityAI irritationObject)
	{
		switch (irritant) {
			case InediaInfectedAI_Irritants.PLAYER_FOOTSTEPS_JOG_NOISE:
			case InediaInfectedAI_Irritants.PLAYER_FOOTSTEPS_CROUCH_SPRINT_NOISE:
			case InediaInfectedAI_Irritants.PLAYER_FOOTSTEPS_SPRINT_NOISE:
				if (irritationObject && irritationObject.IsInherited(PlayerBase)) {
					PlayerBase footstepNoisePlayer = PlayerBase.Cast(irritationObject);
					if (footstepNoisePlayer) {
						float playerFootstepIrritationRadius = 0;
	
						if (footstepNoisePlayer.m_MovementState) {
							switch (footstepNoisePlayer.m_MovementState.m_iMovement) {
								case DayZPlayerConstants.MOVEMENTIDX_RUN:
									// JOG
									if (footstepNoisePlayer.m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT || footstepNoisePlayer.m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT) {
										playerFootstepIrritationRadius = 30;
									}
								break;

								case DayZPlayerConstants.MOVEMENTIDX_SPRINT:
									if (footstepNoisePlayer.m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT || footstepNoisePlayer.m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT) {
										// SPRINT
										playerFootstepIrritationRadius = 40;
									} else if (footstepNoisePlayer.m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH) {
										// CROUCH SPRINT
										playerFootstepIrritationRadius = 15;
									}
								break;
							}
					
							if (playerFootstepIrritationRadius > 0) {
								float surfaceNoiseMultiplier =  NoiseAIEvaluate.GetNoiseMultiplierBySurface(footstepNoisePlayer);					
								if (surfaceNoiseMultiplier <= 0.5) {
									playerFootstepIrritationRadius *= 0.8;
								} else if (surfaceNoiseMultiplier >= 1) {
									playerFootstepIrritationRadius *= 1.2;
								}
							
								playerFootstepIrritationRadius *= footstepNoisePlayer.InediaInfectedAI_GetNoiseMultiplierByShoes();
								playerFootstepIrritationRadius *= footstepNoisePlayer.InediaInfectedAI_GetCamoClothingMultiplier();
								playerFootstepIrritationRadius *= InediaInfectedAI_SyberiaCompatibility.GetSyberiaStealthNoiseMultiplier(footstepNoisePlayer);
							}
						}

						return playerFootstepIrritationRadius;
					}
				}
			break;
			
			case InediaInfectedAI_Irritants.HORSE_HOOFSTEPS_NOISE:
				float horseFootstepIrritationRadius =  0;
			
				if (irritationObject && irritationObject.IsInherited(PlayerBase)) {
					PlayerBase horseOwner = PlayerBase.Cast(irritationObject);
					if (horseOwner) {
						horseFootstepIrritationRadius = 60;
						
						float surfaceHorseNoiseMultiplier =  NoiseAIEvaluate.GetNoiseMultiplierBySurface(horseOwner);			
						if (surfaceHorseNoiseMultiplier <= 0.5) {
							horseFootstepIrritationRadius *= 0.8;
						} else if (surfaceHorseNoiseMultiplier >= 1) {
							horseFootstepIrritationRadius *= 1.2;
						}
					}
				}
			
				return horseFootstepIrritationRadius;
			break;

			case InediaInfectedAI_Irritants.FIREPLACE_VISUAL:
				if (GetGame().GetWorld().IsNight()) {
					return 100;
				} else {
					return 50;
				}
			break;
			
			case InediaInfectedAI_Irritants.FLAMMABLES_VISUAL:
				if (irritationObject) {
					if (irritationObject.IsInherited(BroomBase)) {
						return 50;
					} else if (irritationObject.IsInherited(Torch)) {
						return 60;
					}
				}
			break;
			
			case InediaInfectedAI_Irritants.WEAPON_SHOT_NOISE:
			case InediaInfectedAI_Irritants.FRIENDLY_WEAPON_SHOT_NOISE:
				if (irritationObject && irritationObject.IsInherited(Weapon_Base)) {
					Weapon_Base weapon = Weapon_Base.Cast(irritationObject);
					if (weapon) {
						return Math.Clamp(weapon.InediaInfectedAI_GetNoiseShotRadius(), 0, 120);
					}
				}
			break;
			
			case InediaInfectedAI_Irritants.EXPLOSIVE_ITEM_NOISE:
				if (irritationObject) {
					if (irritationObject.IsInherited(FlashGrenade)) {
						if (GetGame().GetWorld().IsNight()) {
							return 120;
						} else {
							return 80;
						}
					} else if (irritationObject.IsInherited(SmokeGrenadeBase)) {
						return 0;
					} else {
						return 120;
					}
				}
			break;
			
			case InediaInfectedAI_Irritants.PLAYER_FALL_NOISE:
				if (irritationObject && irritationObject.IsInherited(PlayerBase)) {
					PlayerBase fallenPlayer = PlayerBase.Cast(irritationObject);
					if (fallenPlayer) {
						float fallIrritationRadius = 40;
						fallIrritationRadius *= Math.Clamp(fallenPlayer.InediaInfectedAI_GetLastFallHeight() / 3, 0, 1);
						fallIrritationRadius *= InediaInfectedAI_SyberiaCompatibility.GetSyberiaStealthNoiseMultiplier(fallenPlayer);
					
						return fallIrritationRadius;
					}
				}
			break;
			
			case InediaInfectedAI_Irritants.INFECTED_SCREAM_NOISE:
				if (irritationObject && irritationObject.IsInherited(ZombieBase)) {
					ZombieBase zombieThatScreamed = ZombieBase.Cast(irritationObject);
					if (zombieThatScreamed && zombieThatScreamed.m_InediaInfectedAI_ConfigZombie) {
						return zombieThatScreamed.m_InediaInfectedAI_ConfigZombie.ScreamAttractsZombiesInRadius;
					}
				}
			break;
			
			case InediaInfectedAI_Irritants.CAR_HORN_NOISE:
				if (irritationObject && InediaInfectedAI_Functions.IsKindOfCached("Bicycle_Base", irritationObject)) {
					return 50;
				}
			
				return 100;
			break;
			
			case InediaInfectedAI_Irritants.CAR_ENGINE_NOISE:
				if (irritationObject && irritationObject.IsInherited(CarScript)) {
					CarScript vehicleThatIrritated = CarScript.Cast(irritationObject);
					if (vehicleThatIrritated) {
						return InediaInfectedAI_Config.GetInstance().Vehicles.GetNoiseRadius(vehicleThatIrritated, vehicleThatIrritated.InediaInfectedAI_EngineGetRPMIdle(), vehicleThatIrritated.InediaInfectedAI_EngineGetRPMMax(), vehicleThatIrritated.InediaInfectedAI_EngineGetRPM());
					}
				}
			break;
			
			case InediaInfectedAI_Irritants.PLAYER_VOICE_WHISPER_NOISE:
			case InediaInfectedAI_Irritants.PLAYER_VOICE_TALK_NOISE:
			case InediaInfectedAI_Irritants.PLAYER_VOICE_SHOUT_NOISE:
				if (irritationObject && irritationObject.IsInherited(PlayerBase)) {
					PlayerBase speakingPlayer = PlayerBase.Cast(irritationObject);
					if (speakingPlayer) {
						float playerVoiceIrritationRadius = 0;
						switch (irritant) {
							case InediaInfectedAI_Irritants.PLAYER_VOICE_WHISPER_NOISE:
								playerVoiceIrritationRadius = 10;
								break;
							case InediaInfectedAI_Irritants.PLAYER_VOICE_TALK_NOISE:
								playerVoiceIrritationRadius = 40;
								break;
							case InediaInfectedAI_Irritants.PLAYER_VOICE_SHOUT_NOISE:
								playerVoiceIrritationRadius = 80;
								break;
						}
				
						if (playerVoiceIrritationRadius > 0) {
							// If a player speaks through a megaphone, the sound is amplified by 40 meters
							ItemMegaphone megaphoneInHands = ItemMegaphone.Cast(speakingPlayer.GetItemInHands());
							if (megaphoneInHands && megaphoneInHands.CanSpeak()) {
								playerVoiceIrritationRadius += 40;
							}
						}
					
						return playerVoiceIrritationRadius;
					}
				}
			break;
			
			case InediaInfectedAI_Irritants.INFECTED_BODYFALL_NOISE:
			case InediaInfectedAI_Irritants.INFECTED_BODYFALL_BACKSTAB_NOISE:
			case InediaInfectedAI_Irritants.DOORS_NOISE:
				return 30;
			break;
			
			case InediaInfectedAI_Irritants.PLAYER_SYMPTOMS_NOISE:
			case InediaInfectedAI_Irritants.CHEMLIGHT_VISUAL:
			case InediaInfectedAI_Irritants.CHEMLIGHTRED_VISUAL:
			case InediaInfectedAI_Irritants.ROADFLARE_NOISE:
				return 40;
			break;
			
			case InediaInfectedAI_Irritants.FLASHLIGHTS_VISUAL:
			case InediaInfectedAI_Irritants.HEADTORCHRED_VISUAL:
			case InediaInfectedAI_Irritants.KITCHEN_TIMER_NOISE:
			case InediaInfectedAI_Irritants.ALARM_CLOCK_NOISE:
				return 50;
			break;
			
			case InediaInfectedAI_Irritants.PORTABLE_GAS_LAMP_VISUAL:
			case InediaInfectedAI_Irritants.ROADFLARE_VISUAL:
			case InediaInfectedAI_Irritants.SMOKE_GRENADE_VISUAL:
				return 60;
			break;
			
			case InediaInfectedAI_Irritants.PLAYER_MINING_LIGHT_NOISE:
			case InediaInfectedAI_Irritants.PLAYER_BUILDING_NOISE:
			case InediaInfectedAI_Irritants.PLAYER_SAW_PLANKS_NOISE:
				return 70;
			break;
			
			case InediaInfectedAI_Irritants.POWER_GENERATOR_NOISE:
				return 80;
			break;
			
			case InediaInfectedAI_Irritants.PLAYER_MINING_HARD_NOISE:
			case InediaInfectedAI_Irritants.SPOTLIGHT_VISUAL:
				return 100;
			break;
			
			case InediaInfectedAI_Irritants.FUEL_STATION_NOISE:
			case InediaInfectedAI_Irritants.FIREWORKS_NOISE:
			case InediaInfectedAI_Irritants.CAR_LIGHT_VISUAL:
				return 120;
			break;
			
			case InediaInfectedAI_Irritants.MOD_MASS_SEARCH_CUSTOM:
			case InediaInfectedAI_Irritants.MOD_MASS_SEARCH_VISUAL:
			case InediaInfectedAI_Irritants.MOD_MASS_SEARCH_NOISE:
				return m_ModMassSearchCurrentIrritantRadius;
			break;
			
			case InediaInfectedAI_Irritants.CUSTOM:
				if (irritationObject) {
					ItemBase irritationItem = ItemBase.Cast(irritationObject);
					if (irritationItem && irritationItem.m_InediaInfectedAI_CustomIrritant) {
						bool indoor = MiscGameplayFunctions.InediaInfectedAI_IsUnderRoofBuilding(irritationItem.GetPosition() + vector.Up * 0.1);
						bool isNight = GetGame().GetWorld().IsNight();
			
						float customIrritantRadius = 0;
						if (!indoor && !isNight) {
							customIrritantRadius = irritationItem.m_InediaInfectedAI_CustomIrritant.RadiusOutdoorDay;
						} else if (!indoor && isNight) {
							customIrritantRadius = irritationItem.m_InediaInfectedAI_CustomIrritant.RadiusOutdoorNight;
						} else if (indoor && !isNight) {
							customIrritantRadius = irritationItem.m_InediaInfectedAI_CustomIrritant.RadiusIndoorDay;
						} else if (indoor && isNight) {
							customIrritantRadius = irritationItem.m_InediaInfectedAI_CustomIrritant.RadiusIndoorNight;
						}

						if (irritationItem.m_InediaInfectedAI_CustomIrritant.Type == InediaInfectedAI_IrritantsTypes.VISUAL) {
							customIrritantRadius *= MiscGameplayFunctions.InediaInfectedAI_GetWeatherVisibilityMultiplier();
						} else if (irritationItem.m_InediaInfectedAI_CustomIrritant.Type == InediaInfectedAI_IrritantsTypes.NOISE) {
							customIrritantRadius *= MiscGameplayFunctions.InediaInfectedAI_GetWeatherSoundMultiplier();
						}
					
						return customIrritantRadius;
					}
				}
			break;
		}
		
		return 0;
	}
	
	static float GetIrritantRadiusMultiplier(InediaInfectedAI_Irritants irritant, EntityAI irritationObject, vector position)
	{
		float radiusMultiplier = 1;
		
		InediaInfectedAI_IrritantsTypes irritantType = InediaInfectedAI_IrritantsManager.GetIrritantType(irritant, irritationObject);

		// The impact of weather and being indoors on noise and visual irritants
		if (irritant == InediaInfectedAI_Irritants.CUSTOM) {
			// For custom irritants, the impact of weather and other conditions is calculated in the method for determining the radius.
			radiusMultiplier *= 1;
		} else if (irritant == InediaInfectedAI_Irritants.DOORS_NOISE) {
			// The noise of doors should not be affected by the player's position inside the building, only by the weather.
			radiusMultiplier *= MiscGameplayFunctions.InediaInfectedAI_GetWeatherSoundMultiplier();
		} else if (irritant == InediaInfectedAI_Irritants.FIREPLACE_VISUAL && !GetGame().GetWorld().IsNight()) {
			// A daytime campfire requires special sensitivity conditions, as it ignores being indoors due to the smoke coming from the chimney,
			// but it does not ignore the weather conditions.
			radiusMultiplier *= MiscGameplayFunctions.InediaInfectedAI_GetWeatherVisibilityMultiplier();
		} else if (irritantType == InediaInfectedAI_IrritantsTypes.VISUAL) {
			radiusMultiplier *= MiscGameplayFunctions.InediaInfectedAI_GetGlobalVisibilityMultiplier(position);
		} else if (irritantType == InediaInfectedAI_IrritantsTypes.NOISE) {
			radiusMultiplier *= MiscGameplayFunctions.InediaInfectedAI_GetGlobalSoundMultiplier(position);
		}
		
		return radiusMultiplier;
	}
	
	static float GetInfectedSensitivityToIrritant(ZombieBase zmb, InediaInfectedAI_Irritants irritant, EntityAI irritationObject = null)
	{
		if (!zmb || !zmb.m_InediaInfectedAI_ConfigZombie) {
			return 0;
		}
		
		float sensitivity = 0;

		switch (irritant) {
			case InediaInfectedAI_Irritants.HEADTORCHRED_VISUAL:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactHeadtorchRedVisual;
				}
			break;

			case InediaInfectedAI_Irritants.FLASHLIGHTS_VISUAL:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactFlashlightsVisual;
				}
			break;

			case InediaInfectedAI_Irritants.ROADFLARE_VISUAL:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactRoadflareVisual;
				}
			break;

			case InediaInfectedAI_Irritants.ROADFLARE_NOISE:
				sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactRoadflareNoise;
			break;

			case InediaInfectedAI_Irritants.CHEMLIGHTRED_VISUAL:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactChemlightRedVisual;
				}
			break;

			case InediaInfectedAI_Irritants.CHEMLIGHT_VISUAL:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactChemlightVisual;
				}
			break;

			case InediaInfectedAI_Irritants.PORTABLE_GAS_LAMP_VISUAL:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactPortableGasLampVisual;
				}
			break;

			case InediaInfectedAI_Irritants.FLAMMABLES_VISUAL:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactFlammablesVisual;
				}
			break;

			case InediaInfectedAI_Irritants.SPOTLIGHT_VISUAL:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactSpotlightVisual;
				}
			break;

			case InediaInfectedAI_Irritants.FIREPLACE_VISUAL:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactFireplaceVisual;
				} else {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactFireplaceDayVisual;
				}
			break;

			case InediaInfectedAI_Irritants.CAR_LIGHT_VISUAL:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactCarLightVisual;
				}
			break;

			case InediaInfectedAI_Irritants.EXPLOSIVE_ITEM_NOISE:
				sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactExplosiveItemNoise;
			break;

	// ....................--................................................................................................................................
// ....................-=::..............................................................................................................................
// .....................-=:............................................----..............................................................................
// ......................:+:........................................:=+==-:--............................................................................
// ........................:::....................................::*++-..--+=++=-.......................................................................
// ...........................-.:...............................-***++=----:---++++:.....................................................................
// ............................:..: ......:....................++=:...  + : . :==*#=.....................................................................
// ............................::: :.......:.................:+=-:..:.  -:.:.:+---:+:....................................................................
// .............................:.- :.......................-:--:-=-.:::--::-+=:. ..==...................................................................
// ................................- : ......:..............*-:-=-===+++*--=+-:-=== =-=..................................................................
// ...............................:.=.: ......:.............#.:.:=*=:....-===--::-===---.................................................................
// ..................................-:. ......:............+...=+.                +#--:.................................................................
// ...................................:+:. .................-:.-+                  .#--..................................................................
// ...................................:.+-: ................-:-*  -==:.        .:=- +*=..................................................................
// ....................................::==: .:....:.......==+==  .:-=+++.  .++*+=-.:*=+:.:..............................................................
// .....................................:.==-. ::  .:......:=-=+  .=+**+.   ::+*++- :+=.-..:.............................................................
// ......................................:.--=: .:. ..:....-*=--:           :       =-= .:..:............................................................
// .......................................:.-=+.. :....:::=-=--.::          ..     ==+=..-..:............................................................
// ..........................................:-*:. .:..-::..+#+= .-.    .::::-   .==##*-:...:............................................................
// ..........................................:.:*-...:::-==*##**   :     ..:.  .--.+##*=::.:.............................................................
// ...........................................:.:++.::=**##+*#+:=      .:---:--:. -##+=-::-..............................................................
// ..............................................:++=+==##-=+#-:#+.     :::...   -#+==:-=.:..............................................................
// ..............................................--==.=#*:==*+:+#-=-:.   ....  .+##-=-.==.:..............................................................
// ............................................::=::=*#=-==+=-+##- . :--..  .-++##*:+:=:+ :.:............................................................
// ...........................................:-+=***+-+-+=-=**##:     .-++==- *#**#+*++.:.-.............................................................
// .........................................::+++--**=+==-+###..-=:.          :#+=###+-::.-..............................................................
// .........................................-*-. :##+#==*###++--: .----:::.  .+*-=#*=-:.::...............................................................
// ........................................:+. .=*=#######*+:::::-:    ..:::::.  =+===-..................................................................
// ........................................-: .*=-######=--=-:-..:::--.         :=:+.:::-....*=--:.......................................................
// ........................................=-=++=-+**-:-==-=-:-::  :::-==-      .+--::.:.-:-=*::+-:-.....................................................
// .......................................=*.   .:.:.::::--:--.=.:    ..::.:    :-=.: -.:.:-=-= -:- ::...................................................
// .....................................-+.......--  .:-:-:=.--:-.-       .-=  ::-= .  ::.:::--- -.-..-..................................................
// ....................................-=   ...::.    ..=.-==--=--::::.  .. .-:::= .    .:+.--+*..- -:.::................................................
// ..................................-- -    .:   .    :-+=+===-:=.:-::::. :::.::  .     -+-:=:*+ :::  .-................................................
// .................................=- :.   :- ...    -:.#: --+---=:  .:::::: :::...   .=-:.  -:#- -::  .+...............................................
// ................................=. -.   :-..      =..*#+  :.-.. -:..  .::::::..-. .-=:  :  .-.*- .-   .=..............................................
// ...............................=. :=:.:-:.      .=..*=:=:  : =.. :. ...  .:::..:.:=:     :  .- *+..=:  -..............................................
// ..............................==  .:..-.       == .#+  .-  - .+:...:   ...   :::+-        :. .-.*#- ::  =.............................................
// .............................=..-           .=+: -=:   .-  : ..:-...:     :....  .:        .:  :.+#*:.: .-............................................
// ..............................=.:-::......:::. :-. :--.=.  : :  .-.: -.    ....::.::         :. .:-##+-::::...........................................
// ..............................:+-:::::::::. :=+:=*####+:  :. :   .=.: :.         .-=          :. .-.*##+..............................................
// ..............................*..-=======+==-..+#-. ++. ::  .:     -::.::         .-           :. .=:##+..............................................
// .............................-=-::::::--:.   :*#= .:: .:    -       :-...-         -            :  :-*=...............................................
// ............................+=-=-------::..:=:-#=:.  ::.    -        .=.: -.      :=.            :  +-................................................
// ...........................-+ ..   -   .. :=. :=. .::: :    -         .=.: :: :::..- .:::        :  *=................................................
// ...........................*::    -    :+*-.. -.+-:. - .-   :          .+:...=   .=-     .::.    - -++:...............................................
// ..........................*-.     =-=====:...-...=.. .-..-   :          :=-.. -.-+.--=:      ....==#-.*...............................................
// ........................-+.  :-=++=-:::-....-....::....:==+. .          -.-*.. =: :.  :=:       :#*-. =-..............................................
// .......................+-:-=+-==:.   .=...::......::.  ..::++:..       :=+=.+:: :: ...  .::::-=+#+=*: .+..............................................
// .....................:**=--:-=:.:::::+.............:   ...:  :--=====+*+-.  :+-..::   ...   .:###==+-  =..............................................
// ....................=-:.:::=-:..:: --...............:   .. ....          ..:-..-:..-    ..:-:-+##*++-  .-.............................................
// ...................-  .-.=+-:==. .-:.................-.::::.  .::::::..:-:::    =:: :.     .-.=###*++ . =.............................................
// .................--  .- =:  .-==.=...................:-   .=:.  ..   ..     .   .=-:..:   .-:.###====:: =.............................................
// ...............--:...: .=      .+.....................--    ::--:..         -..:: .=::.-.:.::=-#*--==+: = -...........................................
// ...............-:.:    :=:      -:.....................=:..     ..-   ::.  .-   :   -=:.--:-.-.+#++=--=:: +:..........................................
// ................+ .:   .*-:..    -:.....................=..   ....:. =    -..   :    -+-.:=:=..:#+==+==- .+::.........................................
// ................:= .:   =*+-:-::. ::.....................=---=-----:.:   -. .   :    ..==:-==...*#+==++ .=+ -:........................................
// .................-=  -   =-::   .. ::....................:=.    .=----:.+   .   :    .  .+:---..:#*+*+=-=:+..-........................................
// ................::=++=--:= :..:.    ==...................::      :.  .-:-:  .   :   .:  :.=-==...+##*+:=::-= :.-......................................
// .....................:+:    :::*:    =-:.................-:       -   : .:. .   :   -  -.  ==--. :###-----.= :.==.....................................
// .......................-=-.   ::-:    =.:...............-*==-:::. .:  :  -      :   -  -  :++--- :##-:::--=.:--.=.....................................
// .........................:=:.   ::-.  :-..::.........-=+=-:.:-:.:----:-  -      :   -  -.===  -+..##+=--:-+:.=-  =....................................
// ...........................--.:. .:::. =..........-+=:-----::--::  ...:-----:   :   = --+-.    :+:=#*=---:  .::  -:...................................
// .............................=:.:  .:::.=.......-**=*=::...----=--:..    .::-=---.  --+-.    .:.+--##.:=:  -  .:  +...................................
// ..............................-= ::  .:-=.....:=+=.:-=::::: :    .--:---::    .::--=-:.     ...=: =##=:=:  :   :. .=..................................
// ................................-: ::  ::+...=++::::-=-   . .:     :   .-=::-..    ..     .. .=:  +###.+=: .    -  -:.................................
// .................................:=..:  .:+..=::::::-:-.     :     :     :+--=....  .:       :-  .=*##:=###=..   -  =.................................
// ..................................:#: -  ::+::+=--:  .:=   . :.     : .   .- .:---:  :::.. ..+   :.*##-:#####-:: .: .=................................
// ....................................::.-. .-*--##=-   .:-     -     .. .   .-     =+=-.   ..+.  .:--#== =#####=.- .: -:...............................
// ......................................----.   .***==-  .-:    .:     :. .    -     : .-:  .+-   : ..#:...:*####=.: :. =...............................
// ........................................-:.:.. .--*--+:--=.    -      :      .-    :.  ===+=    +  .#-:....-####==:.: .=..............................
// .........................................:.  ....:-==-=-::-....:.......:      .:    .. : ....  .=...=-:......==:.:::--:-..............................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................

			case InediaInfectedAI_Irritants.FIREWORKS_NOISE:
				sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactFireworksNoise;
			break;

			case InediaInfectedAI_Irritants.FUEL_STATION_NOISE:
				sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactFuelStationNoise;
			break;

			case InediaInfectedAI_Irritants.POWER_GENERATOR_NOISE:
				sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactPowerGeneratorNoise;
			break;

			case InediaInfectedAI_Irritants.ALARM_CLOCK_NOISE:
				sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactAlarmClockNoise;
			break;

			case InediaInfectedAI_Irritants.KITCHEN_TIMER_NOISE:
				sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactKitchenTimerNoise;
			break;

			case InediaInfectedAI_Irritants.CAR_HORN_NOISE:
				sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactCarHornNoise;
			break;

			case InediaInfectedAI_Irritants.CAR_ENGINE_NOISE:
				sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactCarEngineNoise;
			break;

			case InediaInfectedAI_Irritants.SMOKE_GRENADE_VISUAL:
				if (!GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactSmokeGrenadeVisual;
				}
			break;

			case InediaInfectedAI_Irritants.INFECTED_SCREAM_NOISE:
				sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactScreamNoise;
			break;

			case InediaInfectedAI_Irritants.WEAPON_SHOT_NOISE:
				sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactWeaponShotNoise;
			break;
			
			case InediaInfectedAI_Irritants.FRIENDLY_WEAPON_SHOT_NOISE:
				sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactFriendlyWeaponShotNoise;
			break;
			
			case InediaInfectedAI_Irritants.HORSE_HOOFSTEPS_NOISE:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactHorseHoofstepsNightNoise;
				} else {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactHorseHoofstepsDayNoise;
				}
			break;

			case InediaInfectedAI_Irritants.PLAYER_FOOTSTEPS_JOG_NOISE:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactFootstepsJogNightNoise;
				} else {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactFootstepsJogDayNoise;
				}
			break;

			case InediaInfectedAI_Irritants.PLAYER_FOOTSTEPS_CROUCH_SPRINT_NOISE:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactFootstepsCrouchSprintNightNoise;
				} else {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactFootstepsCrouchSprintDayNoise;
				}
			break;

			case InediaInfectedAI_Irritants.PLAYER_FOOTSTEPS_SPRINT_NOISE:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactFootstepsSprintNightNoise;
				} else {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactFootstepsSprintDayNoise;
				}
			break;

			case InediaInfectedAI_Irritants.DOORS_NOISE:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactDoorsNightNoise;
				} else {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactDoorsDayNoise;
				}
			break;
			
			case InediaInfectedAI_Irritants.PLAYER_FALL_NOISE:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactPlayerFallNightNoise;
				} else {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactPlayerFallDayNoise;
				}
			break;

			case InediaInfectedAI_Irritants.INFECTED_BODYFALL_NOISE:
				if (zmb.m_InediaInfectedAI_DisableReactToBodyfallForSeconds > 0) {
					sensitivity = 0;
				} else {
					if (GetGame().GetWorld().IsNight()) {
						sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactBodyfallNightNoise;
					} else {
						sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactBodyfallDayNoise;
					}
				}
			break;

			case InediaInfectedAI_Irritants.PLAYER_SYMPTOMS_NOISE:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactSymptomsNightNoise;
				} else {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactSymptomsDayNoise;
				}
			break;

			case InediaInfectedAI_Irritants.PLAYER_VOICE_WHISPER_NOISE:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactVoiceWhisperNightNoise;
				} else {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactVoiceWhisperDayNoise;
				}
			break;

			case InediaInfectedAI_Irritants.PLAYER_VOICE_TALK_NOISE:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactVoiceTalkNightNoise;
				} else {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactVoiceTalkDayNoise;
				}
			break;

			case InediaInfectedAI_Irritants.PLAYER_VOICE_SHOUT_NOISE:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactVoiceShoutNightNoise;
				} else {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactVoiceShoutDayNoise;
				}
			break;

			case InediaInfectedAI_Irritants.PLAYER_MINING_LIGHT_NOISE:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactMiningLightNightNoise;
				} else {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactMiningLightDayNoise;
				}
			break;

			case InediaInfectedAI_Irritants.PLAYER_MINING_HARD_NOISE:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactMiningHardNightNoise;
				} else {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactMiningHardDayNoise;
				}
			break;

			case InediaInfectedAI_Irritants.PLAYER_BUILDING_NOISE:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactBuildingNightNoise;
				} else {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactBuildingDayNoise;
				}
			break;

			case InediaInfectedAI_Irritants.PLAYER_SAW_PLANKS_NOISE:
				if (GetGame().GetWorld().IsNight()) {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactSawPlanksNightNoise;
				} else {
					sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactSawPlanksDayNoise;
				}
			break;

			case InediaInfectedAI_Irritants.INFECTED_BODYFALL_BACKSTAB_NOISE:
				if (zmb.m_InediaInfectedAI_DisableReactToBodyfallForSeconds > 0) {
					sensitivity = 0;
				} else {
					if (GetGame().GetWorld().IsNight()) {
						sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactBodyfallBackstabNightNoise;
					} else {
						sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.ReactBodyfallBackstabDayNoise;
					}
				}
			break;
			
			case InediaInfectedAI_Irritants.MOD_MASS_SEARCH_CUSTOM:
			case InediaInfectedAI_Irritants.MOD_MASS_SEARCH_VISUAL:
			case InediaInfectedAI_Irritants.MOD_MASS_SEARCH_NOISE:
				sensitivity = 1;
			break;
			
			case InediaInfectedAI_Irritants.CUSTOM:
				sensitivity = zmb.m_InediaInfectedAI_ConfigZombie.GetSensitivityToCustomIrritantItem(irritationObject);
			break;
		}

		return Math.Clamp(sensitivity, 0, 1);
	}
	
	static float GetInfectedDamageToIrritant(ZombieBase zmb, InediaInfectedAI_Irritants irritant)
	{
		if (!zmb || !zmb.m_InediaInfectedAI_ConfigZombie) {
			return 0;
		}
		
		switch (irritant) {
			case InediaInfectedAI_Irritants.HEADTORCHRED_VISUAL:
				if (InediaInfectedAI_IrritantsManager.GetInfectedSensitivityToIrritant(zmb, InediaInfectedAI_Irritants.HEADTORCHRED_VISUAL) > 0) {
					return zmb.m_InediaInfectedAI_ConfigZombie.ReactAndDestroyHeadtorchRedVisual;
				}
				break;

			case InediaInfectedAI_Irritants.FLASHLIGHTS_VISUAL:
				if (InediaInfectedAI_IrritantsManager.GetInfectedSensitivityToIrritant(zmb, InediaInfectedAI_Irritants.FLASHLIGHTS_VISUAL) > 0) {
					return zmb.m_InediaInfectedAI_ConfigZombie.ReactAndDestroyFlashlightsVisual;
				}
				break;

			case InediaInfectedAI_Irritants.ROADFLARE_VISUAL:
				if (InediaInfectedAI_IrritantsManager.GetInfectedSensitivityToIrritant(zmb, InediaInfectedAI_Irritants.ROADFLARE_VISUAL) > 0) {
					return zmb.m_InediaInfectedAI_ConfigZombie.ReactAndDestroyRoadflareVisual;
				}
				break;

			case InediaInfectedAI_Irritants.ROADFLARE_NOISE:
				if (InediaInfectedAI_IrritantsManager.GetInfectedSensitivityToIrritant(zmb, InediaInfectedAI_Irritants.ROADFLARE_NOISE) > 0) {
					return zmb.m_InediaInfectedAI_ConfigZombie.ReactAndDestroyRoadflareNoise;
				}
				break;

			case InediaInfectedAI_Irritants.CHEMLIGHTRED_VISUAL:
				if (InediaInfectedAI_IrritantsManager.GetInfectedSensitivityToIrritant(zmb, InediaInfectedAI_Irritants.CHEMLIGHTRED_VISUAL) > 0) {
					return zmb.m_InediaInfectedAI_ConfigZombie.ReactAndDestroyChemlightRedVisual;
				}
				break;

			case InediaInfectedAI_Irritants.CHEMLIGHT_VISUAL:
				if (InediaInfectedAI_IrritantsManager.GetInfectedSensitivityToIrritant(zmb, InediaInfectedAI_Irritants.CHEMLIGHT_VISUAL) > 0) {
					return zmb.m_InediaInfectedAI_ConfigZombie.ReactAndDestroyChemlightVisual;
				}
				break;

			case InediaInfectedAI_Irritants.PORTABLE_GAS_LAMP_VISUAL:
				if (InediaInfectedAI_IrritantsManager.GetInfectedSensitivityToIrritant(zmb, InediaInfectedAI_Irritants.PORTABLE_GAS_LAMP_VISUAL) > 0) {
					return zmb.m_InediaInfectedAI_ConfigZombie.ReactAndDestroyPortableGasLampVisual;
				}
				break;

			case InediaInfectedAI_Irritants.FLAMMABLES_VISUAL:
				if (InediaInfectedAI_IrritantsManager.GetInfectedSensitivityToIrritant(zmb, InediaInfectedAI_Irritants.FLAMMABLES_VISUAL) > 0) {
					return zmb.m_InediaInfectedAI_ConfigZombie.ReactAndDestroyFlammablesVisual;
				}
				break;

			case InediaInfectedAI_Irritants.SPOTLIGHT_VISUAL:
				if (InediaInfectedAI_IrritantsManager.GetInfectedSensitivityToIrritant(zmb, InediaInfectedAI_Irritants.SPOTLIGHT_VISUAL) > 0) {
					return zmb.m_InediaInfectedAI_ConfigZombie.ReactAndDestroySpotlightVisual;
				}
				break;

			case InediaInfectedAI_Irritants.FIREPLACE_VISUAL:
				if (InediaInfectedAI_IrritantsManager.GetInfectedSensitivityToIrritant(zmb, InediaInfectedAI_Irritants.FIREPLACE_VISUAL) > 0) {
					if (GetGame().GetWorld().IsNight()) {
						return zmb.m_InediaInfectedAI_ConfigZombie.ReactAndDestroyFireplaceVisual;
					} else {
						return zmb.m_InediaInfectedAI_ConfigZombie.ReactAndDestroyFireplaceDayVisual;
					}
				}
				break;

			case InediaInfectedAI_Irritants.CAR_LIGHT_VISUAL:
				if (InediaInfectedAI_IrritantsManager.GetInfectedSensitivityToIrritant(zmb, InediaInfectedAI_Irritants.CAR_LIGHT_VISUAL) > 0) {
					return zmb.m_InediaInfectedAI_ConfigZombie.ReactAndDestroyCarLightVisual;
				}
				break;

			case InediaInfectedAI_Irritants.FIREWORKS_NOISE:
				if (InediaInfectedAI_IrritantsManager.GetInfectedSensitivityToIrritant(zmb, InediaInfectedAI_Irritants.FIREWORKS_NOISE) > 0) {
					return zmb.m_InediaInfectedAI_ConfigZombie.ReactAndDestroyFireworksNoise;
				}
				break;

			case InediaInfectedAI_Irritants.POWER_GENERATOR_NOISE:
				if (InediaInfectedAI_IrritantsManager.GetInfectedSensitivityToIrritant(zmb, InediaInfectedAI_Irritants.POWER_GENERATOR_NOISE) > 0) {
					return zmb.m_InediaInfectedAI_ConfigZombie.ReactAndDestroyPowerGeneratorNoise;
				}
				break;

			case InediaInfectedAI_Irritants.ALARM_CLOCK_NOISE:
				if (InediaInfectedAI_IrritantsManager.GetInfectedSensitivityToIrritant(zmb, InediaInfectedAI_Irritants.ALARM_CLOCK_NOISE) > 0) {
					return zmb.m_InediaInfectedAI_ConfigZombie.ReactAndDestroyAlarmClockNoise;
				}
				break;

			case InediaInfectedAI_Irritants.KITCHEN_TIMER_NOISE:
				if (InediaInfectedAI_IrritantsManager.GetInfectedSensitivityToIrritant(zmb, InediaInfectedAI_Irritants.KITCHEN_TIMER_NOISE) > 0) {
					return zmb.m_InediaInfectedAI_ConfigZombie.ReactAndDestroyKitchenTimerNoise;
				}
				break;

			case InediaInfectedAI_Irritants.SMOKE_GRENADE_VISUAL:
				if (InediaInfectedAI_IrritantsManager.GetInfectedSensitivityToIrritant(zmb, InediaInfectedAI_Irritants.SMOKE_GRENADE_VISUAL) > 0) {
					return zmb.m_InediaInfectedAI_ConfigZombie.ReactAndDestroySmokeGrenadeVisual;
				}
				break;
		}

		return 0;
	}
	
	static float GetInfectedDamageToIrritantingItem(ZombieBase zmb, ItemBase item)
	{
		if (!zmb || !zmb.m_InediaInfectedAI_ConfigZombie || !item) {
			return 0;
		}
		
		if (item.GetHealth("", "") <= 0 || !item.InediaInfectedAI_IrritationItemIsIrritating()) {
			return 0;
		}


		if (item.IsInherited(Headtorch_Black)) {
			return InediaInfectedAI_IrritantsManager.GetInfectedDamageToIrritant(zmb, InediaInfectedAI_Irritants.HEADTORCHRED_VISUAL);
		}

		if (item.IsInherited(Flashlight) || item.IsInherited(TLRLight) || item.IsInherited(UniversalLight) || item.IsInherited(Headtorch_ColorBase)) {
			return InediaInfectedAI_IrritantsManager.GetInfectedDamageToIrritant(zmb, InediaInfectedAI_Irritants.FLASHLIGHTS_VISUAL);
		}

		if (item.IsInherited(Roadflare)) {
			float roadFlareVisualDamage = InediaInfectedAI_IrritantsManager.GetInfectedDamageToIrritant(zmb, InediaInfectedAI_Irritants.ROADFLARE_VISUAL);
			if (roadFlareVisualDamage > 0) {
				return roadFlareVisualDamage;
			}

			float roadFlareNoiseDamage = InediaInfectedAI_IrritantsManager.GetInfectedDamageToIrritant(zmb, InediaInfectedAI_Irritants.ROADFLARE_NOISE);
			if (roadFlareNoiseDamage > 0) {
				return roadFlareNoiseDamage;
			}
		}

		if (item.IsInherited(Chemlight_Red)) {
			return InediaInfectedAI_IrritantsManager.GetInfectedDamageToIrritant(zmb, InediaInfectedAI_Irritants.CHEMLIGHTRED_VISUAL);
		}

		if (item.IsInherited(Chemlight_ColorBase)) {
			return InediaInfectedAI_IrritantsManager.GetInfectedDamageToIrritant(zmb, InediaInfectedAI_Irritants.CHEMLIGHT_VISUAL);
		}

		if (item.IsInherited(PortableGasLamp)) {
			return InediaInfectedAI_IrritantsManager.GetInfectedDamageToIrritant(zmb, InediaInfectedAI_Irritants.PORTABLE_GAS_LAMP_VISUAL);
		}

		if (item.IsInherited(FlammableBase)) {
			return InediaInfectedAI_IrritantsManager.GetInfectedDamageToIrritant(zmb, InediaInfectedAI_Irritants.FLAMMABLES_VISUAL);
		}

		if (item.IsInherited(Spotlight)) {
			return InediaInfectedAI_IrritantsManager.GetInfectedDamageToIrritant(zmb, InediaInfectedAI_Irritants.SPOTLIGHT_VISUAL);
		}

		if (item.IsInherited(SmokeGrenadeBase)) {
			return InediaInfectedAI_IrritantsManager.GetInfectedDamageToIrritant(zmb, InediaInfectedAI_Irritants.SMOKE_GRENADE_VISUAL);
		}

		if (item.IsInherited(FireplaceBase)) {
			return InediaInfectedAI_IrritantsManager.GetInfectedDamageToIrritant(zmb, InediaInfectedAI_Irritants.FIREPLACE_VISUAL);
		}

		if (item.IsInherited(PowerGenerator)) {
			return InediaInfectedAI_IrritantsManager.GetInfectedDamageToIrritant(zmb, InediaInfectedAI_Irritants.POWER_GENERATOR_NOISE);
		}

		if (item.IsInherited(AlarmClock_ColorBase)) {
			return InediaInfectedAI_IrritantsManager.GetInfectedDamageToIrritant(zmb, InediaInfectedAI_Irritants.ALARM_CLOCK_NOISE);
		}

		if (item.IsInherited(KitchenTimer)) {
			return InediaInfectedAI_IrritantsManager.GetInfectedDamageToIrritant(zmb, InediaInfectedAI_Irritants.KITCHEN_TIMER_NOISE);
		}

		if (item.IsInherited(FireworksLauncher)) {
			return InediaInfectedAI_IrritantsManager.GetInfectedDamageToIrritant(zmb, InediaInfectedAI_Irritants.FIREWORKS_NOISE);
		}

		// Custom irritants
		if (item.m_InediaInfectedAI_CustomIrritant) {
			return zmb.m_InediaInfectedAI_ConfigZombie.GetDamageToCustomIrritantItem(item);
		}

		return 0;
	}
	
	static ItemBase GetInfectedIrritatingItemFromInventory(ZombieBase zmb, EntityAI targetItem)
	{
		if (!zmb || !zmb.m_InediaInfectedAI_ConfigZombie || !targetItem) {
			return null;
		}
		
		if (targetItem.IsPlayer()) {
			PlayerBase targetPlayer = PlayerBase.Cast(targetItem);
			if (targetPlayer && targetPlayer.InediaInfectedAI_IsInBlock()) {
				ItemBase itemInHands = targetPlayer.GetItemInHands();
				if (itemInHands && InediaInfectedAI_IrritantsManager.GetInfectedDamageToIrritantingItem(zmb, itemInHands) > 0) {
					return itemInHands;
				} else {
					return null;
				}
			}
		}
		
		GameInventory inv = targetItem.GetInventory();
		if (!inv) {
			return null;
		}

		array<EntityAI> inventoryItems = new array<EntityAI>;
		inv.EnumerateInventory(InventoryTraversalType.PREORDER, inventoryItems);
		for (int i = 0; i < inventoryItems.Count(); i++) {
			ItemBase inInventoryItem = ItemBase.Cast(inventoryItems.Get(i));
			if (!inInventoryItem) {
				continue;
			}

			if (InediaInfectedAI_IrritantsManager.GetInfectedDamageToIrritantingItem(zmb, inInventoryItem) > 0) {
				return inInventoryItem;
			}
		}

		return null;
	}
	
    static void MassSearchActivation(vector fromPosition, vector toPosition, InediaInfectedAI_Irritants irritant, EntityAI irritationObject)
    {
#ifdef SERVER
        if (!fromPosition) return;

        if (!toPosition) {
            toPosition = fromPosition;
        }
		
		InediaInfectedAI_IrritantsManager.Debug(irritant, irritationObject, fromPosition);
		
		float irritantRadius = Math.Clamp(InediaInfectedAI_IrritantsManager.GetIrritantRadius(irritant, irritationObject) * InediaInfectedAI_IrritantsManager.GetIrritantRadiusMultiplier(irritant, irritationObject, fromPosition), 0, 1000);
		if (irritantRadius <= 0) {
			return;
		}

        int currentTime = GetGame().GetTime();
        InediaInfectedAI_IrritantsTypes irritantType = InediaInfectedAI_IrritantsManager.GetIrritantType(irritant, irritationObject);
        int irritantPriority = InediaInfectedAI_IrritantsManager.GetIrritantPriority(irritant, irritationObject);

        PlayerBase irritatedByPlayer;
        if (irritationObject) {
            if (irritationObject.IsInherited(PlayerBase)) {
                irritatedByPlayer = PlayerBase.Cast(irritationObject);
            } else {
                irritatedByPlayer = PlayerBase.Cast(irritationObject.GetHierarchyRootPlayer());
            }
        }

        array<ZombieBase> nearZombies = new array<ZombieBase>;
		MiscGameplayFunctions.InediaInfectedAI_GetZombiesAtPosition3D(fromPosition, irritantRadius, nearZombies);
		foreach (ZombieBase zmb : nearZombies) {
		    if (zmb.m_InediaInfectedAI_ConfigZombie) {
		        if (!zmb.m_InediaInfectedAI_ConfigZombie.SearchHandlerIsActive || !zmb.m_InediaInfectedAI_ConfigZombie.ReactHandlerIsActive || !zmb.m_InediaInfectedAI_ActiveTarget) {
		            continue;
		        }
		        
		        if (!zmb.IsAlive()) {
		            continue;
		        }
		        
		        // Do not run the logic of activating the search mode for infected in the chase mode
		        if (zmb.m_InediaInfectedAI_MindState == DayZInfectedConstants.MINDSTATE_CHASE || zmb.m_InediaInfectedAI_MindState == DayZInfectedConstants.MINDSTATE_FIGHT) {
		            continue;
		        }
				
				// If the infected is chasing a target using Inedia Long Vision, only irritants with a priority of 777 or higher can distract it.
				if (zmb.InediaInfectedAI_GetLongVisionTarget() && irritantPriority < 777) {
					continue;
				}
				
				// Adjusting the radius taking into account the sensitivity of the zombie to the irritant
		        // If the distance to the zombie is less than the adjusted radius, there will be no irritation
		        float sensitivityRadius;
		        
		        if (irritatedByPlayer && !irritatedByPlayer.CanBeTargetedByAI(zmb)) {
					if (irritant == InediaInfectedAI_Irritants.WEAPON_SHOT_NOISE) {
						// The reaction to the weapon shot noise from friendly NPCs is configured separately from the reaction to weapon shot noise.
						sensitivityRadius = irritantRadius * InediaInfectedAI_IrritantsManager.GetInfectedSensitivityToIrritant(zmb, InediaInfectedAI_Irritants.FRIENDLY_WEAPON_SHOT_NOISE, irritationObject);
					} else {
						// Ignore any other irritants from friendly NPCs.
						continue;
					}
		        } else {
					sensitivityRadius = irritantRadius * InediaInfectedAI_IrritantsManager.GetInfectedSensitivityToIrritant(zmb, irritant, irritationObject);
				}	        
		        
		        // If the irritant is behind the zombie, the radius of irritation changes
		        if (zmb.InediaInfectedAI_IsPositionBehind(fromPosition)) {
		            if (irritantType == InediaInfectedAI_IrritantsTypes.VISUAL) {
		                sensitivityRadius *= zmb.m_InediaInfectedAI_ConfigZombie.ReactBehindVisualMultiplier;
		            } else if (irritantType == InediaInfectedAI_IrritantsTypes.NOISE) {
		                sensitivityRadius *= zmb.m_InediaInfectedAI_ConfigZombie.ReactBehindNoiseMultiplier;
		            }
		        }
		        
		        if (vector.DistanceSq(fromPosition, zmb.GetPosition()) > sensitivityRadius * sensitivityRadius) {
		            continue;
		        }
		        
		        if (irritant == InediaInfectedAI_Irritants.INFECTED_BODYFALL_NOISE || irritant == InediaInfectedAI_Irritants.INFECTED_BODYFALL_BACKSTAB_NOISE) {
		            // Temporarily disable reactions to body fall irritant for abuse prevention
		            zmb.InediaInfectedAI_DisableReactToBodyFallForSeconds(Math.RandomFloatInclusive(240, 480));
		        } else if (irritant == InediaInfectedAI_Irritants.INFECTED_SCREAM_NOISE && !InediaInfectedAI_Functions.IsKindOfCached("ZmbM_usSoldier_Officer_Desert", zmb)) {
		            // Disable the ability to scream for infected who have already been attracted by the scream for performance purposes,
		            // as there is no point in screaming if everyone around has already been drawn in.
		            // However, infected officers are immune to this mechanic and will still be able to scream regardless of "Zombies.ScreamNearbyInfectedSilenceSeconds" parameter setting.
		            zmb.InediaInfectedAI_DisableScreamForSeconds(zmb.m_InediaInfectedAI_ConfigZombie.ScreamNearbyInfectedSilenceSeconds);
		        }
		
		        // Activate irritation only when the irritating object has a higher priority than the current irritating object,
		        // or when the irritating object is already the current one,
		        // or if more than 10 seconds have passed since the last irritation
		        int irritationObjectId = 0;
		        if (irritationObject) {
		            irritationObjectId = irritationObject.GetID();
		        }
		        if (irritantPriority > zmb.m_InediaInfectedAI_IsIrritatedByIrritantPriority || zmb.m_InediaInfectedAI_IsIrritatedByObjectId == irritationObjectId || (currentTime - zmb.m_InediaInfectedAI_IsIrritatedByIrritantLastTime) > 10000) {
		            // Initiate irritation
		            if (irritationObject && irritant != InediaInfectedAI_Irritants.EXPLOSIVE_ITEM_NOISE) {
		                zmb.InediaInfectedAI_StartSearchModeWithTargetCorrection(irritationObject);
		            } else {
		                zmb.InediaInfectedAI_StartSearchMode(toPosition);
		            }
					
					// Create vanilla noise to irritate the infected.
					MiscGameplayFunctions.InediaInfectedAI_MakeNoiseForZombie(zmb);
		
		            // If irritation occurred, update the data for the irritating object
		            zmb.m_InediaInfectedAI_IsIrritatedByObjectId = irritationObjectId;
		            zmb.m_InediaInfectedAI_IsIrritatedByIrritantPriority = irritantPriority;
		            zmb.m_InediaInfectedAI_IsIrritatedByIrritantLastTime = currentTime;
		        }
		    }
		}
#endif
    }
	
	static float m_ModMassSearchCurrentIrritantRadius = 0;
	static int m_ModMassSearchCurrentIrritantPriority = 0;
	static void ModMassSearchActivation(vector fromPosition, float irritantRadius, int irritantType = 0, int irritantPriority = 777, vector toPosition = vector.Zero)
	{
		if (!fromPosition) return;
		
		InediaInfectedAI_Irritants irritant = InediaInfectedAI_Irritants.MOD_MASS_SEARCH_CUSTOM;
		
		if (irritantType == 1) {
			irritant = InediaInfectedAI_Irritants.MOD_MASS_SEARCH_VISUAL;
		} else if (irritantType == 2) {
			irritant = InediaInfectedAI_Irritants.MOD_MASS_SEARCH_NOISE;
		}
		
		m_ModMassSearchCurrentIrritantRadius = irritantRadius;
		m_ModMassSearchCurrentIrritantPriority = irritantPriority;
		
		if (!toPosition) {
			toPosition = fromPosition;
		}
		
		InediaInfectedAI_IrritantsManager.MassSearchActivation(fromPosition, toPosition, irritant, null);
	}
	
// Debug irritants ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static void Debug(InediaInfectedAI_Irritants irritant, EntityAI irritationObject, vector position)
	{
		if (!InediaInfectedAI_Debug.IsDebugRuleEnabled("irr")) {
			return;
		}
		
		int hour;
		int minute;
		int second;
		GetHourMinuteSecond(hour, minute, second);
		
		string irritationObjectString = "None";
		if (irritationObject) {
			irritationObjectString = irritationObject.GetType();
		}
		
		string msg = string.Format("[%1:%2:%3] Irritant: \"%4\" | Object: \"%5\" | Type: \"%6\" | Priority: \"%7\" | Radius: \"%8\"", hour.ToStringLen(2), minute.ToStringLen(2), second.ToStringLen(2), EnumTools.EnumToString(InediaInfectedAI_Irritants, irritant), irritationObjectString, EnumTools.EnumToString(InediaInfectedAI_IrritantsTypes, InediaInfectedAI_IrritantsManager.GetIrritantType(irritant, irritationObject)), InediaInfectedAI_IrritantsManager.GetIrritantPriority(irritant, irritationObject), Math.Round(InediaInfectedAI_IrritantsManager.GetIrritantRadius(irritant, irritationObject) * InediaInfectedAI_IrritantsManager.GetIrritantRadiusMultiplier(irritant, irritationObject, position) * 10) / 10);
		
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
}

class InediaInfectedAI_IrritationActions
{
// Player footstep noise irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static void PlayerFootstepNoiseIrritation(InediaInfectedAI_Irritants irritant, PlayerBase player)
	{
		if (!irritant || !player) return;
		
		InediaInfectedAI_IrritantsManager.MassSearchActivation(player.GetPosition(), player.GetPosition(), irritant, player);
	}
	
// Horse hoofstep noise irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static void HorseHoofstepNoiseIrritation(PlayerBase player)
	{
		if (!player) return;
		
		InediaInfectedAI_IrritantsManager.MassSearchActivation(player.GetPosition(), player.GetPosition(), InediaInfectedAI_Irritants.HORSE_HOOFSTEPS_NOISE, player);
	}
	
// Player speech irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static void PlayerSpeechIrritation(InediaInfectedAI_Irritants irritant, PlayerBase player)
	{
		if (!irritant || !player) return;
		
		InediaInfectedAI_IrritantsManager.MassSearchActivation(player.GetPosition(), player.GetPosition(), irritant, player);
	}
	
// Vehicle engine irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static void VehicleEngineIrritation(CarScript vehicle)
	{
		if (!vehicle) {
			return;
		}
		
		InediaInfectedAI_IrritantsManager.MassSearchActivation(vehicle.GetPosition(), vehicle.GetPosition(), InediaInfectedAI_Irritants.CAR_ENGINE_NOISE, vehicle);
	}
	
// Vehicle car horn irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static void VehicleCarHornIrritation(CarScript vehicle)
	{
		if (!vehicle) {
			return;
		}
		
		InediaInfectedAI_IrritantsManager.MassSearchActivation(vehicle.GetPosition(), vehicle.GetPosition(), InediaInfectedAI_Irritants.CAR_HORN_NOISE, vehicle);
	}
	
// Vehicle car light irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static void VehicleCarLightIrritation(CarScript vehicle)
	{
		if (!vehicle) {
			return;
		}

		vector position;
		if (vehicle.MemoryPointExists("dmgZone_front")) {
			position = vehicle.ModelToWorld(vehicle.GetMemoryPointPos("dmgZone_front"));
		}

		if (!position) {
			position = vehicle.GetPosition();
		}

		InediaInfectedAI_IrritantsManager.MassSearchActivation(position, position, InediaInfectedAI_Irritants.CAR_LIGHT_VISUAL, vehicle);
	}
	
// Scream irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static void ScreamIrritation(ZombieBase zmb, EntityAI target)
	{
		if (!zmb || !zmb.m_InediaInfectedAI_ConfigZombie) {
			return;
		}
		
		if (zmb.m_InediaInfectedAI_ConfigZombie.ScreamAttractsZombiesInRadius <= 0) {
			return;
		}
		
		vector targetPos;
		if (target) {
			targetPos = target.GetPosition();
		} else {
			targetPos = zmb.GetPosition();
		}
		
		InediaInfectedAI_IrritantsManager.MassSearchActivation(zmb.GetPosition(), targetPos, InediaInfectedAI_Irritants.INFECTED_SCREAM_NOISE, zmb);
	}
	
// Open door irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static void OpenDoorIrritation(PlayerBase player)
	{
		if (!player) {
			return;
		}
		
		InediaInfectedAI_IrritantsManager.MassSearchActivation(player.GetPosition(), player.GetPosition(), InediaInfectedAI_Irritants.DOORS_NOISE, player);
	}
	
// Close door irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static void CloseDoorIrritation(PlayerBase player)
	{
		if (!player) {
			return;
		}

		InediaInfectedAI_IrritantsManager.MassSearchActivation(player.GetPosition(), player.GetPosition(), InediaInfectedAI_Irritants.DOORS_NOISE, player);
	}
	
// Falling Irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static void FallingIrritation(PlayerBase player)
	{
		if (!player) {
			return;
		}

		InediaInfectedAI_IrritantsManager.MassSearchActivation(player.GetPosition(), player.GetPosition(), InediaInfectedAI_Irritants.PLAYER_FALL_NOISE, player);
	}
}

// Explosive items irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class ExplosivesBase : ItemBase
{
    override protected void OnExplode()
    {
#ifdef SERVER
		InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.EXPLOSIVE_ITEM_NOISE, this);
#endif

        super.OnExplode();
    }
}

modded class DestructionEffectGasCanister : DestructionEffectBase
{
	override void OnEntityDestroyedOneTimeServer(EntityAI entity, int oldLevel, string zone)
	{
		super.OnEntityDestroyedOneTimeServer(entity, oldLevel, zone);

		if (entity) {
			InediaInfectedAI_IrritantsManager.MassSearchActivation(entity.GetPosition(), entity.GetPosition(), InediaInfectedAI_Irritants.EXPLOSIVE_ITEM_NOISE, entity);
		}
	}
}

modded class LandMineTrap extends TrapBase
{
	override void Explode(int damageType, string ammoType = "")
	{
#ifdef SERVER
		InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.EXPLOSIVE_ITEM_NOISE, this);
#endif
		
		super.Explode(damageType, ammoType);
	}
}

// Smoke Grenade irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class SmokeGrenadeBase extends Grenade_Base
{
	override bool InediaInfectedAI_IsIrritationItem()
	{
		return true;
	}

	override bool InediaInfectedAI_IrritationItemIsIrritating()
	{
		return super.InediaInfectedAI_IrritationItemIsIrritating() && !GetGame().GetWorld().IsNight();
	}

	override void InediaInfectedAI_IrritationAction() {
		InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.SMOKE_GRENADE_VISUAL, this);
	}
}

// Fireworks Launcher irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class FireworksLauncher: FireworksBase
{
	override bool InediaInfectedAI_IsIrritationItem()
	{
		return true;
	}

	override bool InediaInfectedAI_IrritationItemIsIrritating()
	{
		return GetState() == EFireworksState.FIRING;
	}

	override void InediaInfectedAI_IrritationAction() {
        InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.FIREWORKS_NOISE, this);
	}

	void InediaInfectedAI_Stop()
	{
		m_Index = GetMaxShots();
		m_TimerEvent = null;
		SetState(EFireworksState.FINISHED);
	}
}

// FuelStation irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class Land_FuelStation_Feed extends FuelStation
{
    override void EEKilled(Object killer)
    {
#ifdef SERVER
        InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.FUEL_STATION_NOISE, this);
#endif

        super.EEKilled(killer);
    }
}

// PowerGenerator irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class PowerGenerator extends ItemBase
{
	override bool InediaInfectedAI_IsIrritationItem()
	{
		return true;
	}

	override void InediaInfectedAI_IrritationAction() {
		InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.POWER_GENERATOR_NOISE, this);
	}
}

// ....................--................................................................................................................................
// ....................-=::..............................................................................................................................
// .....................-=:............................................----..............................................................................
// ......................:+:........................................:=+==-:--............................................................................
// ........................:::....................................::*++-..--+=++=-.......................................................................
// ...........................-.:...............................-***++=----:---++++:.....................................................................
// ............................:..: ......:....................++=:...  + : . :==*#=.....................................................................
// ............................::: :.......:.................:+=-:..:.  -:.:.:+---:+:....................................................................
// .............................:.- :.......................-:--:-=-.:::--::-+=:. ..==...................................................................
// ................................- : ......:..............*-:-=-===+++*--=+-:-=== =-=..................................................................
// ...............................:.=.: ......:.............#.:.:=*=:....-===--::-===---.................................................................
// ..................................-:. ......:............+...=+.                +#--:.................................................................
// ...................................:+:. .................-:.-+                  .#--..................................................................
// ...................................:.+-: ................-:-*  -==:.        .:=- +*=..................................................................
// ....................................::==: .:....:.......==+==  .:-=+++.  .++*+=-.:*=+:.:..............................................................
// .....................................:.==-. ::  .:......:=-=+  .=+**+.   ::+*++- :+=.-..:.............................................................
// ......................................:.--=: .:. ..:....-*=--:           :       =-= .:..:............................................................
// .......................................:.-=+.. :....:::=-=--.::          ..     ==+=..-..:............................................................
// ..........................................:-*:. .:..-::..+#+= .-.    .::::-   .==##*-:...:............................................................
// ..........................................:.:*-...:::-==*##**   :     ..:.  .--.+##*=::.:.............................................................
// ...........................................:.:++.::=**##+*#+:=      .:---:--:. -##+=-::-..............................................................
// ..............................................:++=+==##-=+#-:#+.     :::...   -#+==:-=.:..............................................................
// ..............................................--==.=#*:==*+:+#-=-:.   ....  .+##-=-.==.:..............................................................
// ............................................::=::=*#=-==+=-+##- . :--..  .-++##*:+:=:+ :.:............................................................
// ...........................................:-+=***+-+-+=-=**##:     .-++==- *#**#+*++.:.-.............................................................
// .........................................::+++--**=+==-+###..-=:.          :#+=###+-::.-..............................................................
// .........................................-*-. :##+#==*###++--: .----:::.  .+*-=#*=-:.::...............................................................
// ........................................:+. .=*=#######*+:::::-:    ..:::::.  =+===-..................................................................
// ........................................-: .*=-######=--=-:-..:::--.         :=:+.:::-....*=--:.......................................................
// ........................................=-=++=-+**-:-==-=-:-::  :::-==-      .+--::.:.-:-=*::+-:-.....................................................
// .......................................=*.   .:.:.::::--:--.=.:    ..::.:    :-=.: -.:.:-=-= -:- ::...................................................
// .....................................-+.......--  .:-:-:=.--:-.-       .-=  ::-= .  ::.:::--- -.-..-..................................................
// ....................................-=   ...::.    ..=.-==--=--::::.  .. .-:::= .    .:+.--+*..- -:.::................................................
// ..................................-- -    .:   .    :-+=+===-:=.:-::::. :::.::  .     -+-:=:*+ :::  .-................................................
// .................................=- :.   :- ...    -:.#: --+---=:  .:::::: :::...   .=-:.  -:#- -::  .+...............................................
// ................................=. -.   :-..      =..*#+  :.-.. -:..  .::::::..-. .-=:  :  .-.*- .-   .=..............................................
// ...............................=. :=:.:-:.      .=..*=:=:  : =.. :. ...  .:::..:.:=:     :  .- *+..=:  -..............................................
// ..............................==  .:..-.       == .#+  .-  - .+:...:   ...   :::+-        :. .-.*#- ::  =.............................................
// .............................=..-           .=+: -=:   .-  : ..:-...:     :....  .:        .:  :.+#*:.: .-............................................
// ..............................=.:-::......:::. :-. :--.=.  : :  .-.: -.    ....::.::         :. .:-##+-::::...........................................
// ..............................:+-:::::::::. :=+:=*####+:  :. :   .=.: :.         .-=          :. .-.*##+..............................................
// ..............................*..-=======+==-..+#-. ++. ::  .:     -::.::         .-           :. .=:##+..............................................
// .............................-=-::::::--:.   :*#= .:: .:    -       :-...-         -            :  :-*=...............................................
// ............................+=-=-------::..:=:-#=:.  ::.    -        .=.: -.      :=.            :  +-................................................
// ...........................-+ ..   -   .. :=. :=. .::: :    -         .=.: :: :::..- .:::        :  *=................................................
// ...........................*::    -    :+*-.. -.+-:. - .-   :          .+:...=   .=-     .::.    - -++:...............................................
// ..........................*-.     =-=====:...-...=.. .-..-   :          :=-.. -.-+.--=:      ....==#-.*...............................................
// ........................-+.  :-=++=-:::-....-....::....:==+. .          -.-*.. =: :.  :=:       :#*-. =-..............................................
// .......................+-:-=+-==:.   .=...::......::.  ..::++:..       :=+=.+:: :: ...  .::::-=+#+=*: .+..............................................
// .....................:**=--:-=:.:::::+.............:   ...:  :--=====+*+-.  :+-..::   ...   .:###==+-  =..............................................
// ....................=-:.:::=-:..:: --...............:   .. ....          ..:-..-:..-    ..:-:-+##*++-  .-.............................................
// ...................-  .-.=+-:==. .-:.................-.::::.  .::::::..:-:::    =:: :.     .-.=###*++ . =.............................................
// .................--  .- =:  .-==.=...................:-   .=:.  ..   ..     .   .=-:..:   .-:.###====:: =.............................................
// ...............--:...: .=      .+.....................--    ::--:..         -..:: .=::.-.:.::=-#*--==+: = -...........................................
// ...............-:.:    :=:      -:.....................=:..     ..-   ::.  .-   :   -=:.--:-.-.+#++=--=:: +:..........................................
// ................+ .:   .*-:..    -:.....................=..   ....:. =    -..   :    -+-.:=:=..:#+==+==- .+::.........................................
// ................:= .:   =*+-:-::. ::.....................=---=-----:.:   -. .   :    ..==:-==...*#+==++ .=+ -:........................................
// .................-=  -   =-::   .. ::....................:=.    .=----:.+   .   :    .  .+:---..:#*+*+=-=:+..-........................................
// ................::=++=--:= :..:.    ==...................::      :.  .-:-:  .   :   .:  :.=-==...+##*+:=::-= :.-......................................
// .....................:+:    :::*:    =-:.................-:       -   : .:. .   :   -  -.  ==--. :###-----.= :.==.....................................
// .......................-=-.   ::-:    =.:...............-*==-:::. .:  :  -      :   -  -  :++--- :##-:::--=.:--.=.....................................
// .........................:=:.   ::-.  :-..::.........-=+=-:.:-:.:----:-  -      :   -  -.===  -+..##+=--:-+:.=-  =....................................
// ...........................--.:. .:::. =..........-+=:-----::--::  ...:-----:   :   = --+-.    :+:=#*=---:  .::  -:...................................
// .............................=:.:  .:::.=.......-**=*=::...----=--:..    .::-=---.  --+-.    .:.+--##.:=:  -  .:  +...................................
// ..............................-= ::  .:-=.....:=+=.:-=::::: :    .--:---::    .::--=-:.     ...=: =##=:=:  :   :. .=..................................
// ................................-: ::  ::+...=++::::-=-   . .:     :   .-=::-..    ..     .. .=:  +###.+=: .    -  -:.................................
// .................................:=..:  .:+..=::::::-:-.     :     :     :+--=....  .:       :-  .=*##:=###=..   -  =.................................
// ..................................:#: -  ::+::+=--:  .:=   . :.     : .   .- .:---:  :::.. ..+   :.*##-:#####-:: .: .=................................
// ....................................::.-. .-*--##=-   .:-     -     .. .   .-     =+=-.   ..+.  .:--#== =#####=.- .: -:...............................
// ......................................----.   .***==-  .-:    .:     :. .    -     : .-:  .+-   : ..#:...:*####=.: :. =...............................
// ........................................-:.:.. .--*--+:--=.    -      :      .-    :.  ===+=    +  .#-:....-####==:.: .=..............................
// .........................................:.  ....:-==-=-::-....:.......:      .:    .. : ....  .=...=-:......==:.:::--:-..............................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................

// AlarmClock irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class AlarmClock_ColorBase: ClockBase
{
	override bool InediaInfectedAI_IsIrritationItem()
	{
		return true;
	}

	override bool InediaInfectedAI_IrritationItemIsIrritating()
	{
		return IsRinging();
	}

	override void InediaInfectedAI_IrritationAction() {
		InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.ALARM_CLOCK_NOISE, this);
	}
}

// KitchenTimer irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class KitchenTimer: ClockBase
{
	override bool InediaInfectedAI_IsIrritationItem()
	{
		return true;
	}

	override bool InediaInfectedAI_IrritationItemIsIrritating()
	{
		return IsRinging();
	}

	override void InediaInfectedAI_IrritationAction() {
		InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.KITCHEN_TIMER_NOISE, this);
	}
}

// Weapons shots noise irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class Weapon_Base extends Weapon
{
	PlayerBase m_InediaInfectedAI_ShooterPlayerPb;
	bool m_InediaInfectedAI_IsFrequentShot = false;
	float m_InediaInfectedAI_LastShotTime = -1000000;
	float m_InediaInfectedAI_NoiseStrength = -1;
	float m_InediaInfectedAI_NoiseStrengthWithSuppressor = -1;
	override void EEFired(int muzzleType, int mode, string ammoType)
	{
		m_InediaInfectedAI_ShooterPlayerPb = PlayerBase.Cast(GetHierarchyRootPlayer());
		if (!m_InediaInfectedAI_ShooterPlayerPb) {
			super.EEFired(muzzleType, mode, ammoType);
			return;
		}
		
		m_InediaInfectedAI_IsFrequentShot = (GetGame().GetTickTime() - m_InediaInfectedAI_LastShotTime) < m_InediaInfectedAI_ShooterPlayerPb.m_InediaInfectedAI_SuppressorShotCooldownSeconds;
		
		if (m_InediaInfectedAI_NoiseStrength == -1) {
			m_InediaInfectedAI_NoiseStrength = 3 * GetGame().ConfigGetFloat("cfgWeapons " + GetType() + " NoiseShoot strength");
		}
		
		float suppressorReduction = m_InediaInfectedAI_NoiseStrength * InediaInfectedAI_GetSuppressorModifier();
		m_InediaInfectedAI_NoiseStrengthWithSuppressor = m_InediaInfectedAI_NoiseStrength + suppressorReduction;
		
		if (m_InediaInfectedAI_NoiseStrengthWithSuppressor < m_InediaInfectedAI_ShooterPlayerPb.m_InediaInfectedAI_SuppressorShotOverfireDistanceMeters) {
			if (m_InediaInfectedAI_IsFrequentShot) {
				m_InediaInfectedAI_NoiseStrengthWithSuppressor = m_InediaInfectedAI_ShooterPlayerPb.m_InediaInfectedAI_SuppressorShotOverfireDistanceMeters;
				
				// m_InediaInfectedAI_ShooterPlayerPb.InediaInfectedAI_ShowFrequentShotIconClient(true);
			} else {
				// m_InediaInfectedAI_ShooterPlayerPb.InediaInfectedAI_ShowFrequentShotIconClient();
			}
		}
		
#ifdef SERVER
		InediaInfectedAI_IrritateZombiesByShotNoise();
#endif

		super.EEFired(muzzleType, mode, ammoType);
		
		if (m_InediaInfectedAI_ShooterPlayerPb.InediaInfectedAI_HasUnlimitedAmmo()) {
			InediaInfectedAI_SetMaxAmmo();
		}
		
		m_InediaInfectedAI_LastShotTime = GetGame().GetTickTime();
	}

	
	float m_InediaInfectedAI_IrritateZombiesByShotNoiseLastTime = -1000000;
	float m_InediaInfectedAI_IrritateZombiesByFrequentShotNoiseLastTime = -1000000;
	
	void InediaInfectedAI_IrritateZombiesByShotNoise()
	{
		float currentTime = GetGame().GetTickTime();
		
		if ((currentTime - m_InediaInfectedAI_IrritateZombiesByShotNoiseLastTime) > 5) {
			InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.WEAPON_SHOT_NOISE, this);
			m_InediaInfectedAI_IrritateZombiesByShotNoiseLastTime = currentTime;
		} else if (m_InediaInfectedAI_IsFrequentShot && (currentTime - m_InediaInfectedAI_IrritateZombiesByFrequentShotNoiseLastTime) > 10) {
			InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.WEAPON_SHOT_NOISE, this);
			m_InediaInfectedAI_IrritateZombiesByShotNoiseLastTime = currentTime;
			
			m_InediaInfectedAI_IrritateZombiesByFrequentShotNoiseLastTime = currentTime;
		}
	}

	float m_InediaInfectedAI_SuppressorNoiseModifier = 0;
	ItemBase m_InediaInfectedAI_SuppressorLast;
	float InediaInfectedAI_GetSuppressorModifier()
	{
		ItemBase suppressor = GetAttachedSuppressor();
		if (suppressor && !suppressor.IsRuined()) {
			if (suppressor != m_InediaInfectedAI_SuppressorLast) {
				m_InediaInfectedAI_SuppressorNoiseModifier = GetGame().ConfigGetFloat("cfgVehicles " + suppressor.GetType() + " noiseShootModifier");
				m_InediaInfectedAI_SuppressorLast = suppressor;
			}
		} else {
			m_InediaInfectedAI_SuppressorNoiseModifier = 0;
			m_InediaInfectedAI_SuppressorLast = null;
		}

		return m_InediaInfectedAI_SuppressorNoiseModifier;
	}

	float InediaInfectedAI_GetNoiseShotRadius()
	{
		return m_InediaInfectedAI_NoiseStrengthWithSuppressor;
	}
	
	void InediaInfectedAI_SetMaxAmmo()
	{
		Magazine magazine = this.GetMagazine(this.GetCurrentMuzzle());
		
		if (magazine) {
#ifdef SERVER
			magazine.ServerSetAmmoMax();
#else
			magazine.LocalSetAmmoMax();
#endif
		}

		if (HasInternalMagazine(-1)) {
			FillInnerMagazine();
		}

		FillChamber();
	}
}

// Flashlights irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class Flashlight extends ItemBase
{
	override bool InediaInfectedAI_IsIrritationItem()
	{
		return true;
	}

	override bool InediaInfectedAI_IrritationItemIsIrritating()
	{
		return GetGame().GetWorld().IsNight() && super.InediaInfectedAI_IrritationItemIsIrritating();
	}

	override void InediaInfectedAI_IrritationAction() {
		InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.FLASHLIGHTS_VISUAL, this);
	}
}
modded class Headtorch_ColorBase extends Clothing
{
	override bool InediaInfectedAI_IsIrritationItem()
	{
		return true;
	}

	override bool InediaInfectedAI_IrritationItemIsIrritating()
	{
		if (!GetGame().GetWorld().IsNight() || !super.InediaInfectedAI_IrritationItemIsIrritating()) {
			return false;
		}

		// If a headtorch is worn on a zombie - that headtorch should not irritate other zombies
		EntityAI owner = GetHierarchyParent();
		if (owner && owner.IsZombie()) {
			return false;
		}

		return true;
	}

	override void InediaInfectedAI_IrritationAction() {
		InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.FLASHLIGHTS_VISUAL, this);
	}
}
modded class Headtorch_Black extends Headtorch_ColorBase
{
	override void InediaInfectedAI_IrritationAction() {
		InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.HEADTORCHRED_VISUAL, this);
	}
}
modded class TLRLight extends Switchable_Base
{
	override bool InediaInfectedAI_IsIrritationItem()
	{
		return true;
	}

	override bool InediaInfectedAI_IrritationItemIsIrritating()
	{
		return GetGame().GetWorld().IsNight() && super.InediaInfectedAI_IrritationItemIsIrritating();
	}

	override void InediaInfectedAI_IrritationAction() {
		InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.FLASHLIGHTS_VISUAL, this);
	}
}
modded class UniversalLight extends Switchable_Base
{
	override bool InediaInfectedAI_IsIrritationItem()
	{
		return true;
	}

	override bool InediaInfectedAI_IrritationItemIsIrritating()
	{
		return GetGame().GetWorld().IsNight() && super.InediaInfectedAI_IrritationItemIsIrritating();
	}

	override void InediaInfectedAI_IrritationAction() {
		InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.FLASHLIGHTS_VISUAL, this);
	}
}

// ....................--................................................................................................................................
// ....................-=::..............................................................................................................................
// .....................-=:............................................----..............................................................................
// ......................:+:........................................:=+==-:--............................................................................
// ........................:::....................................::*++-..--+=++=-.......................................................................
// ...........................-.:...............................-***++=----:---++++:.....................................................................
// ............................:..: ......:....................++=:...  + : . :==*#=.....................................................................
// ............................::: :.......:.................:+=-:..:.  -:.:.:+---:+:....................................................................
// .............................:.- :.......................-:--:-=-.:::--::-+=:. ..==...................................................................
// ................................- : ......:..............*-:-=-===+++*--=+-:-=== =-=..................................................................
// ...............................:.=.: ......:.............#.:.:=*=:....-===--::-===---.................................................................
// ..................................-:. ......:............+...=+.                +#--:.................................................................
// ...................................:+:. .................-:.-+                  .#--..................................................................
// ...................................:.+-: ................-:-*  -==:.        .:=- +*=..................................................................
// ....................................::==: .:....:.......==+==  .:-=+++.  .++*+=-.:*=+:.:..............................................................
// .....................................:.==-. ::  .:......:=-=+  .=+**+.   ::+*++- :+=.-..:.............................................................
// ......................................:.--=: .:. ..:....-*=--:           :       =-= .:..:............................................................
// .......................................:.-=+.. :....:::=-=--.::          ..     ==+=..-..:............................................................
// ..........................................:-*:. .:..-::..+#+= .-.    .::::-   .==##*-:...:............................................................
// ..........................................:.:*-...:::-==*##**   :     ..:.  .--.+##*=::.:.............................................................
// ...........................................:.:++.::=**##+*#+:=      .:---:--:. -##+=-::-..............................................................
// ..............................................:++=+==##-=+#-:#+.     :::...   -#+==:-=.:..............................................................
// ..............................................--==.=#*:==*+:+#-=-:.   ....  .+##-=-.==.:..............................................................
// ............................................::=::=*#=-==+=-+##- . :--..  .-++##*:+:=:+ :.:............................................................
// ...........................................:-+=***+-+-+=-=**##:     .-++==- *#**#+*++.:.-.............................................................
// .........................................::+++--**=+==-+###..-=:.          :#+=###+-::.-..............................................................
// .........................................-*-. :##+#==*###++--: .----:::.  .+*-=#*=-:.::...............................................................
// ........................................:+. .=*=#######*+:::::-:    ..:::::.  =+===-..................................................................
// ........................................-: .*=-######=--=-:-..:::--.         :=:+.:::-....*=--:.......................................................
// ........................................=-=++=-+**-:-==-=-:-::  :::-==-      .+--::.:.-:-=*::+-:-.....................................................
// .......................................=*.   .:.:.::::--:--.=.:    ..::.:    :-=.: -.:.:-=-= -:- ::...................................................
// .....................................-+.......--  .:-:-:=.--:-.-       .-=  ::-= .  ::.:::--- -.-..-..................................................
// ....................................-=   ...::.    ..=.-==--=--::::.  .. .-:::= .    .:+.--+*..- -:.::................................................
// ..................................-- -    .:   .    :-+=+===-:=.:-::::. :::.::  .     -+-:=:*+ :::  .-................................................
// .................................=- :.   :- ...    -:.#: --+---=:  .:::::: :::...   .=-:.  -:#- -::  .+...............................................
// ................................=. -.   :-..      =..*#+  :.-.. -:..  .::::::..-. .-=:  :  .-.*- .-   .=..............................................
// ...............................=. :=:.:-:.      .=..*=:=:  : =.. :. ...  .:::..:.:=:     :  .- *+..=:  -..............................................
// ..............................==  .:..-.       == .#+  .-  - .+:...:   ...   :::+-        :. .-.*#- ::  =.............................................
// .............................=..-           .=+: -=:   .-  : ..:-...:     :....  .:        .:  :.+#*:.: .-............................................
// ..............................=.:-::......:::. :-. :--.=.  : :  .-.: -.    ....::.::         :. .:-##+-::::...........................................
// ..............................:+-:::::::::. :=+:=*####+:  :. :   .=.: :.         .-=          :. .-.*##+..............................................
// ..............................*..-=======+==-..+#-. ++. ::  .:     -::.::         .-           :. .=:##+..............................................
// .............................-=-::::::--:.   :*#= .:: .:    -       :-...-         -            :  :-*=...............................................
// ............................+=-=-------::..:=:-#=:.  ::.    -        .=.: -.      :=.            :  +-................................................
// ...........................-+ ..   -   .. :=. :=. .::: :    -         .=.: :: :::..- .:::        :  *=................................................
// ...........................*::    -    :+*-.. -.+-:. - .-   :          .+:...=   .=-     .::.    - -++:...............................................
// ..........................*-.     =-=====:...-...=.. .-..-   :          :=-.. -.-+.--=:      ....==#-.*...............................................
// ........................-+.  :-=++=-:::-....-....::....:==+. .          -.-*.. =: :.  :=:       :#*-. =-..............................................
// .......................+-:-=+-==:.   .=...::......::.  ..::++:..       :=+=.+:: :: ...  .::::-=+#+=*: .+..............................................
// .....................:**=--:-=:.:::::+.............:   ...:  :--=====+*+-.  :+-..::   ...   .:###==+-  =..............................................
// ....................=-:.:::=-:..:: --...............:   .. ....          ..:-..-:..-    ..:-:-+##*++-  .-.............................................
// ...................-  .-.=+-:==. .-:.................-.::::.  .::::::..:-:::    =:: :.     .-.=###*++ . =.............................................
// .................--  .- =:  .-==.=...................:-   .=:.  ..   ..     .   .=-:..:   .-:.###====:: =.............................................
// ...............--:...: .=      .+.....................--    ::--:..         -..:: .=::.-.:.::=-#*--==+: = -...........................................
// ...............-:.:    :=:      -:.....................=:..     ..-   ::.  .-   :   -=:.--:-.-.+#++=--=:: +:..........................................
// ................+ .:   .*-:..    -:.....................=..   ....:. =    -..   :    -+-.:=:=..:#+==+==- .+::.........................................
// ................:= .:   =*+-:-::. ::.....................=---=-----:.:   -. .   :    ..==:-==...*#+==++ .=+ -:........................................
// .................-=  -   =-::   .. ::....................:=.    .=----:.+   .   :    .  .+:---..:#*+*+=-=:+..-........................................
// ................::=++=--:= :..:.    ==...................::      :.  .-:-:  .   :   .:  :.=-==...+##*+:=::-= :.-......................................
// .....................:+:    :::*:    =-:.................-:       -   : .:. .   :   -  -.  ==--. :###-----.= :.==.....................................
// .......................-=-.   ::-:    =.:...............-*==-:::. .:  :  -      :   -  -  :++--- :##-:::--=.:--.=.....................................
// .........................:=:.   ::-.  :-..::.........-=+=-:.:-:.:----:-  -      :   -  -.===  -+..##+=--:-+:.=-  =....................................
// ...........................--.:. .:::. =..........-+=:-----::--::  ...:-----:   :   = --+-.    :+:=#*=---:  .::  -:...................................
// .............................=:.:  .:::.=.......-**=*=::...----=--:..    .::-=---.  --+-.    .:.+--##.:=:  -  .:  +...................................
// ..............................-= ::  .:-=.....:=+=.:-=::::: :    .--:---::    .::--=-:.     ...=: =##=:=:  :   :. .=..................................
// ................................-: ::  ::+...=++::::-=-   . .:     :   .-=::-..    ..     .. .=:  +###.+=: .    -  -:.................................
// .................................:=..:  .:+..=::::::-:-.     :     :     :+--=....  .:       :-  .=*##:=###=..   -  =.................................
// ..................................:#: -  ::+::+=--:  .:=   . :.     : .   .- .:---:  :::.. ..+   :.*##-:#####-:: .: .=................................
// ....................................::.-. .-*--##=-   .:-     -     .. .   .-     =+=-.   ..+.  .:--#== =#####=.- .: -:...............................
// ......................................----.   .***==-  .-:    .:     :. .    -     : .-:  .+-   : ..#:...:*####=.: :. =...............................
// ........................................-:.:.. .--*--+:--=.    -      :      .-    :.  ===+=    +  .#-:....-####==:.: .=..............................
// .........................................:.  ....:-==-=-::-....:.......:      .:    .. : ....  .=...=-:......==:.:::--:-..............................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................

// Roadflare irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class Roadflare : ItemBase
{
	override bool InediaInfectedAI_IsIrritationItem()
	{
		return true;
	}

	override void InediaInfectedAI_IrritationAction() {
		InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.ROADFLARE_VISUAL, this);
		InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.ROADFLARE_NOISE, this);
	}
}

// Cmemlights irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class Chemlight_ColorBase : ItemBase
{
	override bool InediaInfectedAI_IsIrritationItem()
	{
		return true;
	}

	override bool InediaInfectedAI_IrritationItemIsIrritating()
	{
		if (!GetGame().GetWorld().IsNight() || !super.InediaInfectedAI_IrritationItemIsIrritating()) {
			return false;
		}

		// If the current chemlight is inside some item (is a child),
		// except for the slot specially allocated for chemlight and player hands - the condition should not be triggered.
		EntityAI owner = GetHierarchyParent();
		if (owner && !owner.IsPlayer()) {
			EntityAI chemlightInSlot = owner.FindAttachmentBySlotName("Chemlight");
			if (!chemlightInSlot || chemlightInSlot != this) {
				return false;
			}
		}

		return true;
	}

	override void InediaInfectedAI_IrritationAction() {
		InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.CHEMLIGHT_VISUAL, this);
	}
}
modded class Chemlight_Red : Chemlight_ColorBase
{
	override void InediaInfectedAI_IrritationAction() {
		InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.CHEMLIGHTRED_VISUAL, this);
	}
}

// PortableGasLamp irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class PortableGasLamp extends ItemBase
{
	override bool InediaInfectedAI_IsIrritationItem()
	{
		return true;
	}

	override bool InediaInfectedAI_IrritationItemIsIrritating()
	{
		return GetGame().GetWorld().IsNight() && super.InediaInfectedAI_IrritationItemIsIrritating();
	}

	override void InediaInfectedAI_IrritationAction() {
		InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.PORTABLE_GAS_LAMP_VISUAL, this);
	}
}

// FlammableBase irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class Torch : FlammableBase
{
	override bool InediaInfectedAI_IsIrritationItem()
	{
		return true;
	}

	override bool InediaInfectedAI_IrritationItemIsIrritating()
	{
		return GetGame().GetWorld().IsNight() && super.InediaInfectedAI_IrritationItemIsIrritating();
	}

	override void InediaInfectedAI_IrritationAction() {
		InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.FLAMMABLES_VISUAL, this);
	}
}
modded class BroomBase : FlammableBase
{
	override bool InediaInfectedAI_IsIrritationItem()
	{
		return true;
	}

	override bool InediaInfectedAI_IrritationItemIsIrritating()
	{
		return GetGame().GetWorld().IsNight() && super.InediaInfectedAI_IrritationItemIsIrritating();
	}

	override void InediaInfectedAI_IrritationAction() {
		InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.FLAMMABLES_VISUAL, this);
	}
}

// Spotlight irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class Spotlight extends ItemBase
{
	override bool InediaInfectedAI_IsIrritationItem()
	{
		return true;
	}

	override bool InediaInfectedAI_IrritationItemIsIrritating()
	{
		return GetGame().GetWorld().IsNight() && super.InediaInfectedAI_IrritationItemIsIrritating();
	}

	override void InediaInfectedAI_IrritationAction() {
		InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.SPOTLIGHT_VISUAL, this);
	}
}

// Fireplace irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class FireplaceBase extends ItemBase
{
	override bool InediaInfectedAI_IsIrritationItem()
	{
		return true;
	}

	override bool InediaInfectedAI_IrritationItemIsIrritating()
	{
		return IsBurning();
	}

	override void InediaInfectedAI_IrritationAction() {
		InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.FIREPLACE_VISUAL, this);
	}

	override void StartFire(bool force_start = false)
	{
		super.StartFire(force_start);

		// Disable navmesh update when starting fire
		SetAffectPathgraph(false, false);
	}
}

modded class ZombieBase extends DayZInfected
{
// Zombie bodyfall irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);

		if (IsCrawling()) {
			return;
		}

		EntityAI killerEntity = EntityAI.Cast(killer);
		if (killerEntity) {
			Man killerSurvivor = killerEntity.GetHierarchyRootPlayer();
			if (killerSurvivor) {
				if (IsBeingBackstabbed()) {
					InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.INFECTED_BODYFALL_BACKSTAB_NOISE, this);
				} else {
					InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.INFECTED_BODYFALL_NOISE, this);
				}
			}
		}
	}
}

// Player symptoms irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class CoughSymptom extends SymptomBase
{
	override void OnGetActivatedServer(PlayerBase player)
	{
		super.OnGetActivatedServer(player);

		if (!player) return;

		InediaInfectedAI_IrritantsManager.MassSearchActivation(player.GetPosition(), player.GetPosition(), InediaInfectedAI_Irritants.PLAYER_SYMPTOMS_NOISE, player);
	}
}
modded class SneezeSymptom extends SymptomBase
{
	override void OnGetActivatedServer(PlayerBase player)
	{
		super.OnGetActivatedServer(player);

		if (!player) return;

		InediaInfectedAI_IrritantsManager.MassSearchActivation(player.GetPosition(), player.GetPosition(), InediaInfectedAI_Irritants.PLAYER_SYMPTOMS_NOISE, player);
	}
}
modded class VomitSymptom extends SymptomBase
{
	override void OnGetActivatedServer(PlayerBase player)
	{
		super.OnGetActivatedServer(player);

		if (!player) return;

		InediaInfectedAI_IrritantsManager.MassSearchActivation(player.GetPosition(), player.GetPosition(), InediaInfectedAI_Irritants.PLAYER_SYMPTOMS_NOISE, player);
	}
}
modded class LaughterSymptom extends SymptomBase
{
	override void OnGetActivatedServer(PlayerBase player)
	{
		super.OnGetActivatedServer(player);

		if (!player) return;

		InediaInfectedAI_IrritantsManager.MassSearchActivation(player.GetPosition(), player.GetPosition(), InediaInfectedAI_Irritants.PLAYER_SYMPTOMS_NOISE, player);
	}
}
modded class PainHeavySymptom extends SymptomBase
{
	override void OnGetActivatedServer(PlayerBase player)
	{
		super.OnGetActivatedServer(player);

		if (!player) return;

		InediaInfectedAI_IrritantsManager.MassSearchActivation(player.GetPosition(), player.GetPosition(), InediaInfectedAI_Irritants.PLAYER_SYMPTOMS_NOISE, player);
	}
}

// oAwJgmtR2g295WD+2t3OqU5MaENPdy93M3d4NVFVcDdMcmJlbllhbUMrL205SHZJTmx6WmxvM2E1Ky93KzN1aEE5TXZuUTVCK1piYzJ3dmtLcFhacWkxbjMrWHU1cDJzTXpoSlJadGVseUQ2bHVnWFF6RlNnd0psMGFXR1l4Tk9MaTIrbjJ6aFlTa0lQdkFFY1BYUlFwQUd1Nmh3UDdZTWJwY213S3FrVDU4a21YWnBhZ2NBMkVnUHZlQmJ1T2VrTTlRWXJsUUFkdjNob1pPWE84NEpUMVVUS21ISTJLa2hJYnJBNHpFYzh3cGpFMTQ5T2h4SGZySzk5eU5tVXNPUlhwbGQvcXhNc1Z4aURDTnJkYlN4bFF0YjIweDRRUzJBNXl0dU5nPT0=

// Mine rock irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class ActionMineRock
{
	override void OnFinishProgressServer(ActionData action_data)
	{
		super.OnFinishProgressServer(action_data);

		if (this.ClassName() == "ActionMineRock" || this.ClassName() == "ActionMineRock1H") {
			if (!action_data.m_Player || !action_data.m_Target || !action_data.m_MainItem) {
				return;
			}

			string workType = "light";
			RockBase rock = RockBase.Cast(action_data.m_Target.GetObject());
			if (rock) {
				workType = InediaInfectedAI_GetWorkTypeForRock(rock, action_data.m_MainItem);
			}

			InediaInfectedAI_Irritants irritant = InediaInfectedAI_Irritants.PLAYER_MINING_LIGHT_NOISE;

			if (workType == "hard") {
				irritant = InediaInfectedAI_Irritants.PLAYER_MINING_HARD_NOISE;
			}

			InediaInfectedAI_IrritantsManager.MassSearchActivation(action_data.m_Player.GetPosition(), action_data.m_Player.GetPosition(), irritant, action_data.m_Player);
		}
	}

	string InediaInfectedAI_GetWorkTypeForRock(RockBase rock, ItemBase tool)
	{
		map<string,int> output_map = new map<string,int>;
		rock.GetMaterialAndQuantityMap(tool, output_map);
		if (output_map.Count() > 0) {
			if (output_map.GetKey(0) == "Stone") {
				return "hard";
			}
		}

		return "light";
	}
};

// Mine tree irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class ActionMineTree
{
	override void OnFinishProgressServer(ActionData action_data)
	{
		super.OnFinishProgressServer(action_data);

		if (this.ClassName() == "ActionMineTree") {
			if (!action_data.m_Player || !action_data.m_Target || !action_data.m_MainItem) {
				return;
			}

			string workType = "light";
			WoodBase tree = WoodBase.Cast(action_data.m_Target.GetObject());
			if (tree) {
				workType = InediaInfectedAI_GetWorkTypeForTree(tree, action_data.m_MainItem);
			}

			InediaInfectedAI_Irritants irritant = InediaInfectedAI_Irritants.PLAYER_MINING_LIGHT_NOISE;

			if (workType == "hard") {
				irritant = InediaInfectedAI_Irritants.PLAYER_MINING_HARD_NOISE;
			}

			InediaInfectedAI_IrritantsManager.MassSearchActivation(action_data.m_Player.GetPosition(), action_data.m_Player.GetPosition(), irritant, action_data.m_Player);
		}
	}

	string InediaInfectedAI_GetWorkTypeForTree(WoodBase tree, ItemBase tool)
	{
		map<string,int> output_map = new map<string,int>;
		tree.GetMaterialAndQuantityMapEx(tool, output_map, m_HarvestType);
		if (output_map.Count() > 0) {
			if (output_map.GetKey(0) == "WoodenLog") {
				return "hard";
			}
		}

		return "light";
	}
};

// Building irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class ActionBuildPart: ActionContinuousBase
{
    override void OnFinishProgressServer(ActionData action_data)
    {
        super.OnFinishProgressServer(action_data);

		if (!action_data.m_Player || !action_data.m_MainItem) {
			return;
		}

		// If the building is done with these tools, we don't attract the infected, as it's a quiet operation.
		if (action_data.m_MainItem.IsInherited(Pliers) || action_data.m_MainItem.IsInherited(Shovel) || action_data.m_MainItem.IsInherited(Pickaxe) || action_data.m_MainItem.IsInherited(FarmingHoe)) {
			return;
		}

		InediaInfectedAI_IrritantsManager.MassSearchActivation(action_data.m_Player.GetPosition(), action_data.m_Player.GetPosition(), InediaInfectedAI_Irritants.PLAYER_BUILDING_NOISE, action_data.m_Player);
    }
}

modded class ActionDismantlePart: ActionContinuousBase
{
	override void OnFinishProgressServer(ActionData action_data)
	{
		super.OnFinishProgressServer(action_data);

		if (!action_data.m_Player || !action_data.m_MainItem) {
			return;
		}

		// If the building is done with these tools, we don't attract the infected, as it's a quiet operation.
		if (action_data.m_MainItem.IsInherited(Pliers) || action_data.m_MainItem.IsInherited(Shovel) || action_data.m_MainItem.IsInherited(Pickaxe) || action_data.m_MainItem.IsInherited(FarmingHoe)) {
			return;
		}

		InediaInfectedAI_IrritantsManager.MassSearchActivation(action_data.m_Player.GetPosition(), action_data.m_Player.GetPosition(), InediaInfectedAI_Irritants.PLAYER_BUILDING_NOISE, action_data.m_Player);
	}
};

// Saw planks irritation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class ActionSawPlanks: ActionContinuousBase
{
	override void OnFinishProgressServer(ActionData action_data)
	{
		super.OnFinishProgressServer(action_data);

		if (!action_data.m_Player) {
			return;
		}

		InediaInfectedAI_IrritantsManager.MassSearchActivation(action_data.m_Player.GetPosition(), action_data.m_Player.GetPosition(), InediaInfectedAI_Irritants.PLAYER_SAW_PLANKS_NOISE, action_data.m_Player);
	}
};

// PetrolLighter (SimpleMatch mod) ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class PetrolLighter extends ItemBase
{
	override bool InediaInfectedAI_IrritationItemIsIrritating()
	{
#ifdef SimpleMatch
		return IsLighterOn();
#else
		return super.InediaInfectedAI_IrritationItemIsIrritating();
#endif
	}
}