/**
 * Created: 2023-09-01
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

modded class AnimalBase extends DayZAnimal
{
	ref InediaInfectedAI_Config m_InediaInfectedAI_ConfigInstance;
	ref InediaInfectedAI_ConfigZombie m_InediaInfectedAI_ConfigZombie;
	
	ref array<string> m_InediaInfectedAI_NetSyncBoolVariables = {
		"BloodSplatParticlesIsActive", 
		"BloodTrailParticlesIsActive",
		"BloodPoolParticlesIsActive"
	};
	int m_InediaInfectedAI_NetSyncBoolVariablesSyncBits;

	int m_InediaInfectedAI_headBoneIndex;

	int m_InediaInfectedAI_BleedingComponent = 0;
	int m_InediaInfectedAI_BleedingCurrentComponent = 0;
	float m_InediaInfectedAI_BloodTrailFrequencySeconds = 1;
	int m_InediaInfectedAI_BloodParticlesBirthRateLimit = 3;
	int m_InediaInfectedAI_BloodLossLevel = 0; // 0 - 5, for the intensity of the blood trails on the client
	
	string m_InediaInfectedAI_PredatorFirstAmmo;

	void ~AnimalBase()
	{
#ifndef SERVER
		InediaInfectedAI_DeactivateBleeding();
#endif
	}

	void AnimalBase()
	{
		RegisterNetSyncVariableInt("m_InediaInfectedAI_NetSyncBoolVariablesSyncBits");
		
		RegisterNetSyncVariableInt("m_InediaInfectedAI_BleedingComponent");
		RegisterNetSyncVariableInt("m_InediaInfectedAI_BloodLossLevel");
	
#ifdef SERVER
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InediaInfectedAI_BloodHandlerServer, 3000, false);
#else
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InediaInfectedAI_BloodTrailHandlerClient, 3000, false);
#endif
	}

	override void EEInit()
	{
		super.EEInit();

#ifdef SERVER
		// ~~~ Server side ~~~

		// Delayed initialization to avoid performance issues when spawning a large number of infected simultaneously.
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InediaInfectedAI_InitOnceServer, Math.RandomIntInclusive(0, 3000), false);
#endif

		m_InediaInfectedAI_headBoneIndex = GetBoneIndexByName("Head");
		
		array<string> predatorAmmos = new array<string>;
		GetGame().ConfigGetTextArray("CfgVehicles " + GetType() + " DamageSphereAmmos", predatorAmmos);
		if (predatorAmmos && predatorAmmos.Count() > 0) {
			m_InediaInfectedAI_PredatorFirstAmmo = predatorAmmos[0];
		}
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
		
		// Init vanilla health
		m_InediaInfectedAI_VanillaHealthBeforeEEHitBy = GetHealth();
#endif
	}
	
	float m_InediaInfectedAI_ConfigInitLastTime = -1;
	void InediaInfectedAI_InitConfig()
	{
#ifdef SERVER
		// Config instances
		m_InediaInfectedAI_ConfigInstance = InediaInfectedAI_Config.GetInstance();
		m_InediaInfectedAI_ConfigInitLastTime = InediaInfectedAI_Config.m_InstanceInitLastTime;
		m_InediaInfectedAI_ConfigZombie = m_InediaInfectedAI_ConfigInstance.Zombies.GetConfigZombie(this);
		
		// Blood handler
		if (m_InediaInfectedAI_ConfigZombie.BloodVolumeMl <= 0) {
			float weight = dBodyGetMass(this);
			if (weight > 0) {
				m_InediaInfectedAI_BloodVolumeMaxMl = weight * 1000 * 0.07;
			} else {
				m_InediaInfectedAI_BloodVolumeMaxMl = 5000;
			}
		} else {
			m_InediaInfectedAI_BloodVolumeMaxMl = m_InediaInfectedAI_ConfigZombie.BloodVolumeMl;
		}

		m_InediaInfectedAI_BloodVolumeCurrentMl = m_InediaInfectedAI_BloodVolumeMaxMl;
		
		if (m_InediaInfectedAI_ConfigZombie.BloodHandlerIsActive) {
			InediaInfectedAI_SetNetSyncBoolVariable("BloodSplatParticlesIsActive", m_InediaInfectedAI_ConfigZombie.BloodSplatParticlesIsActive);
			InediaInfectedAI_SetNetSyncBoolVariable("BloodTrailParticlesIsActive", m_InediaInfectedAI_ConfigZombie.BloodTrailParticlesIsActive);
			InediaInfectedAI_SetNetSyncBoolVariable("BloodPoolParticlesIsActive", m_InediaInfectedAI_ConfigZombie.BloodPoolParticlesIsActive);
		} else {
			InediaInfectedAI_SetNetSyncBoolVariable("BloodSplatParticlesIsActive", false);
			InediaInfectedAI_SetNetSyncBoolVariable("BloodTrailParticlesIsActive", false);
			InediaInfectedAI_SetNetSyncBoolVariable("BloodPoolParticlesIsActive", false);
		}
		
		// Disable the bleeding handler for dogs from the "Dayz-Dog" modification.
	#ifdef Dayz_Dog
		if (InediaInfectedAI_Functions.IsKindOfCached("Dayz_Doggo", this)) {
			m_InediaInfectedAI_ConfigZombie.BloodHandlerIsActive = false;
		}
	#endif
		
		// Disable vanilla wounds and bleeding if needed.
		ComponentAnimalBleeding animalBleeding = ComponentAnimalBleeding.Cast(GetComponent(COMP_TYPE_ANIMAL_BLEEDING));
		if (animalBleeding) {
			if (m_InediaInfectedAI_ConfigZombie.DamageToZombieHandlerIsActive) {
				animalBleeding.m_InediaInfectedAI_DisableVanillaWoundsProcessing = true;
			}
		}
#endif
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

#ifndef SERVER
		// Client side bleeding sync
		if (m_InediaInfectedAI_BleedingComponent != m_InediaInfectedAI_BleedingCurrentComponent) {
			if (m_InediaInfectedAI_BleedingComponent > 0) {
				InediaInfectedAI_ActivateBleedingForComponent(m_InediaInfectedAI_BleedingComponent);
			} else {
				InediaInfectedAI_DeactivateBleeding();
			}

			m_InediaInfectedAI_BleedingCurrentComponent = m_InediaInfectedAI_BleedingComponent;
		}
#endif
	}
	
	override bool ModCommandHandlerBefore(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
#ifdef SERVER
		// This variable is necessary to correctly obtain HP before executing EEHitBy(), as EEHitBy is triggered after HP has already been reduced.
		m_InediaInfectedAI_VanillaHealthBeforeEEHitBy = GetHealth();
		
		// m_InediaInfectedAI_Killed sync
		if (!m_InediaInfectedAI_Killed && !IsAlive()) {
			m_InediaInfectedAI_Killed = true;
		}
			
		if (m_InediaInfectedAI_ConfigZombie) {
			if (IsAlive()) {
				// Handlers without animations
				InediaInfectedAI_ConfigReloadHandler(pDt);
				
				InediaInfectedAI_TimersHandler(pDt);
				InediaInfectedAI_SpeedHandler(pDt);
			}
		}
#endif

		return super.ModCommandHandlerBefore(pDt, pCurrentCommandID, pCurrentCommandFinished);
	}

	override bool ModCommandHandlerAfter(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
	    if (super.ModCommandHandlerAfter(pDt, pCurrentCommandID, pCurrentCommandFinished)) {
	        return true;
		}

#ifdef SERVER
		if (m_InediaInfectedAI_ConfigZombie) {
			
			if (IsAlive()) {
				InediaInfectedAI_CarAttackHandler(pDt);
			}
		}
#endif

		return false;
	}

// EI6dCcKyWQ4sZkiCwvINDGFSVDU5MEVsU0poNlozT05wREtrd0VFSENRZ1dYdWQyTkZlRzRMdy9JRjRRazdFc2tMQjMvK2lTbVpBaU5zMTdmNWU4Sm0yNnpqV1NCa3BUSnhtbXlGQnoyZHBjdGpEMlI1WStDWkNkV0QxaWRpTlJIdjZVMHBZbDJ2OEN5YzVQSjNiN3IrN01mUHpPQWNhbFZaaG5IZjA3eTRpNEFwZFlQWWNZcmNQNUR0R2Z2TCttR2pzK2RUYVlhdEp5cWJFWE9LRDcveW4yb281eml0RS9rck5nNG1GUUtkZGpPajgvaE1yQVdvWkVmRnFnRjhaZE4zaHV2RUY3bHVkdUJRSGoyZDZ0R3JETnNXUkwreWI3Y3VacGdRPT0=

	float m_InediaInfectedAI_WeaponMultiplier = 1;
	float m_InediaInfectedAI_DamageMultiplier = 1;
	
	bool m_InediaInfectedAI_Killed = false;
	
	float m_InediaInfectedAI_HealthDamage;
	
	float m_InediaInfectedAI_VanillaHealthBeforeEEHitBy;
	
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
#ifdef SERVER
		InediaInfectedAI_InitOnceServer();
		
		m_InediaInfectedAI_WeaponMultiplier = 1;
		m_InediaInfectedAI_DamageMultiplier = 1;
		
		m_InediaInfectedAI_EEHitByLastDamageResult = damageResult;
		m_InediaInfectedAI_EEHitByLastDamageType = damageType;
		m_InediaInfectedAI_EEHitByLastSource = source;
		m_InediaInfectedAI_EEHitByLastComponent = component;
		m_InediaInfectedAI_EEHitByLastDmgZone = dmgZone;
		m_InediaInfectedAI_EEHitByLastAmmo = ammo;
		m_InediaInfectedAI_EEHitByLastModelPos = modelPos;
		m_InediaInfectedAI_EEHitByLastSpeedCoef = speedCoef;
		
		if (!damageResult || !source || !m_InediaInfectedAI_ConfigZombie || m_InediaInfectedAI_Killed) {
			super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
			return;
		}
		
		m_InediaInfectedAI_HealthDamage = damageResult.GetDamage("", "Health");
		
		InediaInfectedAI_EEHitByFromAnyToAnimalDamage(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		InediaInfectedAI_EEHitByBleedingActions(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		InediaInfectedAI_EEHitByInjuryActions(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		InediaInfectedAI_EEHitByCarCollisionActions(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		InediaInfectedAI_EEHitByToCarDamage(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		
		float healthDamageFromOtherModsAfter = GetHealth();

		if (!m_InediaInfectedAI_Killed) {
			super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		}
		
		healthDamageFromOtherModsAfter -= GetHealth();
		
		if (InediaInfectedAI_Debug.IsDebugRuleEnabled("dmg")) {
			float logHealth = GetHealth();
			if (m_InediaInfectedAI_Killed) {
				logHealth = 0;
			}
			MiscGameplayFunctions.InediaInfectedAI_DmgDebug(GetType(), source.GetType(), ammo, dmgZone, m_InediaInfectedAI_HealthDamage * m_InediaInfectedAI_DamageMultiplier + healthDamageFromOtherModsAfter, -1, logHealth);
		}
		
		if (!m_InediaInfectedAI_Killed) {
			m_InediaInfectedAI_VanillaHealthBeforeEEHitBy = GetHealth();
		}
		
#else
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
#endif
	}
	
	void InediaInfectedAI_Kill()
	{
		m_InediaInfectedAI_Killed = true;
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
	
// TIMERS HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_TimersHandler(float pDt)
	{
		if (m_InediaInfectedAI_ForbidMovementForSeconds <= 0) {
			InediaInfectedAI_DisableForbidMovementForSeconds();
		} else {
			m_InediaInfectedAI_ForbidMovementForSeconds -= pDt;
		}

		if (m_InediaInfectedAI_OverrideMovementSpeedForSeconds <= 0) {
			InediaInfectedAI_DisableOverrideMovementSpeedForSeconds();
		} else {
			m_InediaInfectedAI_OverrideMovementSpeedForSeconds -= pDt;
		}
	}

// BLOOD HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	float m_InediaInfectedAI_BloodVolumeMaxMl;
	float m_InediaInfectedAI_BloodVolumeCurrentMl;
	float m_InediaInfectedAI_BloodLossRateCurrentMl = 0;
	void InediaInfectedAI_BloodHandlerServer()
	{
#ifdef SERVER
		if (m_InediaInfectedAI_ConfigZombie && m_InediaInfectedAI_ConfigZombie.BloodHandlerIsActive) {
			int bloodLossLevelBefore = m_InediaInfectedAI_BloodLossLevel;
			
			if (IsAlive()) {
				// Blood loss
				if (m_InediaInfectedAI_BloodLossRateCurrentMl > 0) {
					m_InediaInfectedAI_BloodVolumeCurrentMl -= m_InediaInfectedAI_BloodLossRateCurrentMl;
					m_InediaInfectedAI_BloodLossRateCurrentMl -= m_InediaInfectedAI_ConfigZombie.BloodLossRateRegenMl;
				} else {
					InediaInfectedAI_ClearBleeding();
					
					if (m_InediaInfectedAI_BloodLossLevel > 0) {
						m_InediaInfectedAI_BloodLossLevel = 0;
					}
				}
		
				// Blood regen
				if (m_InediaInfectedAI_BloodVolumeCurrentMl < m_InediaInfectedAI_BloodVolumeMaxMl) {
					m_InediaInfectedAI_BloodVolumeCurrentMl += 0.5;
				}
	
				// Death handler
				if (InediaInfectedAI_IsBloodVolumeCritical()) {
					InediaInfectedAI_Kill();
					m_InediaInfectedAI_EEHitByBleedingActionsIsActive = false;
				}
			}
	
			m_InediaInfectedAI_BloodLossLevel = MiscGameplayFunctions.InediaInfectedAI_GetBloodLossLevelFromBloodLossRatePerSec(m_InediaInfectedAI_BloodLossRateCurrentMl);
			
			if (m_InediaInfectedAI_BloodLossLevel != bloodLossLevelBefore) {
				SetSynchDirty();
			}
		}
		
		if (IsAlive()) {
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InediaInfectedAI_BloodHandlerServer, 1000, false);
		} else {
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InediaInfectedAI_ClearBleeding, Math.RandomIntInclusive(20000, 40000), false);
		}
#endif
	}
	
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

	bool m_InediaInfectedAI_EEHitByBleedingActionsIsActive = true;
	void InediaInfectedAI_EEHitByBleedingActions(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!m_InediaInfectedAI_EEHitByBleedingActionsIsActive || !m_InediaInfectedAI_ConfigZombie.BloodHandlerIsActive || !damageResult || !source || m_InediaInfectedAI_BloodLossRateCurrentMl >= m_InediaInfectedAI_ConfigZombie.BloodLossRateMaxMl) {
			return;
		}

		float bloodDamage = 0;
		float bleedingChance = 0;
		
		if (ammo == "InediaInfectedAI_ThrowingProjectile") {
			// For throwing projectiles, damage and bleeding chance is determined from the projectile item
			ItemBase projectile = ItemBase.Cast(source);
			if (!projectile || !projectile.InediaInfectedAI_IsThrowingProjectile()) {
				return;
			}

			float bloodArmorMultiplier = damageResult.GetDamage(dmgZone, "Blood") / ItemBase.InediaInfectedAI_ThrowingProjectileGetAmmoBloodDamage();

			bloodDamage = bloodArmorMultiplier * projectile.InediaInfectedAI_ThrowingProjectileGetHealthDamage();
			bleedingChance = bloodArmorMultiplier * projectile.InediaInfectedAI_ThrowingProjectileGetBleedingChancePercent();
		} else if (damageType == DT_EXPLOSION) {
			// When an infected takes damage from an explosion, activate the bleed with chance
			if (m_InediaInfectedAI_ConfigZombie.BloodOnExplosiveDamageChancePercent > 0) {
				bloodDamage = damageResult.GetDamage(dmgZone, "");
				bleedingChance = m_InediaInfectedAI_ConfigZombie.BloodOnExplosiveDamageChancePercent;
			} else {
				return;
			}
		} else {
			bloodDamage = damageResult.GetDamage(dmgZone, "") * damageResult.GetDamage(dmgZone, "Blood") / 100;
			bleedingChance = 100 * GetGame().ConfigGetFloat("CfgAmmo " + ammo + " DamageApplied bleedThreshold");
			
			if (damageType == DT_CLOSE_COMBAT) {
				bloodDamage *= 0.5;
			}
		}

		if (bloodDamage <= 0 || bleedingChance <= 0) {
			return;
		}

		if (dmgZone == "Head") {
			// In the head, the chance of bleeding increases 3 times
			bleedingChance *= 3;
		}
		
		bloodDamage *= m_InediaInfectedAI_WeaponMultiplier;
		
		bleedingChance = Math.Clamp(bleedingChance, 0, 100);

		if (bleedingChance > 0 && Math.RandomFloatInclusive(0, 100) <= bleedingChance) {
			if (component <= 0) {
				component = 1;
			}
			
			InediaInfectedAI_SetBleedingForComponent(component);

			m_InediaInfectedAI_BloodLossRateCurrentMl += 1 * bloodDamage;

			if (m_InediaInfectedAI_ConfigZombie.BloodLossRateMaxMl < m_InediaInfectedAI_ConfigZombie.BloodLossRateMinMl) {
				m_InediaInfectedAI_ConfigZombie.BloodLossRateMaxMl =  m_InediaInfectedAI_ConfigZombie.BloodLossRateMinMl;
			}

			m_InediaInfectedAI_BloodLossRateCurrentMl = Math.Clamp(m_InediaInfectedAI_BloodLossRateCurrentMl, m_InediaInfectedAI_ConfigZombie.BloodLossRateMinMl, m_InediaInfectedAI_ConfigZombie.BloodLossRateMaxMl);
		}
		
		if (!IsAlive()) {
			m_InediaInfectedAI_EEHitByBleedingActionsIsActive = false;
		}
	}

	// Server side
	bool InediaInfectedAI_IsBloodVolumeLow()
	{
		if (!m_InediaInfectedAI_ConfigZombie.BloodHandlerIsActive) return false;

		if (m_InediaInfectedAI_BloodVolumeMaxMl <= 0) {
			return false;
		}

		float volumeRatio = m_InediaInfectedAI_BloodVolumeCurrentMl / m_InediaInfectedAI_BloodVolumeMaxMl;

		return (volumeRatio >= 0.7 && volumeRatio < 0.8);
	}

	// Server side
	bool InediaInfectedAI_IsBloodVolumeVeryLow()
	{
		if (!m_InediaInfectedAI_ConfigZombie.BloodHandlerIsActive) return false;

		if (m_InediaInfectedAI_BloodVolumeMaxMl <= 0) {
			return false;
		}

		float volumeRatio = m_InediaInfectedAI_BloodVolumeCurrentMl / m_InediaInfectedAI_BloodVolumeMaxMl;

		return (volumeRatio >= 0.6 && volumeRatio < 0.7);
	}

	// Server side
	bool InediaInfectedAI_IsBloodVolumeCritical()
	{
		if (!m_InediaInfectedAI_ConfigZombie.BloodHandlerIsActive) return false;

		if (m_InediaInfectedAI_BloodVolumeMaxMl <= 0) {
			return false;
		}

		return (m_InediaInfectedAI_BloodVolumeCurrentMl / m_InediaInfectedAI_BloodVolumeMaxMl) < 0.6;
	}

	// Server side
	void InediaInfectedAI_SetBleedingForComponent(int component)
	{
		if (!m_InediaInfectedAI_ConfigZombie.BloodHandlerIsActive || component <= 0) return;

		if (m_InediaInfectedAI_BleedingComponent != component) {
			m_InediaInfectedAI_BleedingComponent = component;
			SetSynchDirty();
		}
	}

	// Server side
	void InediaInfectedAI_ClearBleeding()
	{
		if (!m_InediaInfectedAI_ConfigZombie.BloodHandlerIsActive) return;

		if (m_InediaInfectedAI_BleedingComponent > 0) {
			m_InediaInfectedAI_BleedingComponent = 0;
			SetSynchDirty();
		}
	}

	// Client side
	ref array<ref EffectParticle> InediaInfectedAI_BleedingEffects = new array<ref EffectParticle>;
	void InediaInfectedAI_ActivateBleedingForComponent(int component)
	{
		CachedObjectsArrays.ARRAY_STRING.Clear();
		GetActionComponentNameList(component, CachedObjectsArrays.ARRAY_STRING, "fire");

		int boneIdx = -1;
		for (int i = 0; i < CachedObjectsArrays.ARRAY_STRING.Count() && boneIdx == -1; i++) {
			boneIdx = GetBoneIndexByName(CachedObjectsArrays.ARRAY_STRING.Get(i));
		}

		if (!boneIdx) {
			return;
		}

		EffectParticle effect = new BleedingSourceEffect();
		if (!effect) {
			return;
		}

		InediaInfectedAI_BleedingEffects.Insert(effect);

		vector orientation = "0 0 0";
		vector offset = "0 0 0";

	    SEffectManager.PlayInWorld(effect, "0 0 0");
	    Particle m_BloodParticle = effect.GetParticle();
	    m_BloodParticle.SetOrientation(orientation);
	    vector pos;
	    pos += offset;
	    m_BloodParticle.SetPosition(pos);
	    float time = Math.RandomFloat01() * 2;
	    m_BloodParticle.SetParameter(-1, EmitorParam.CURRENT_TIME, time);

	    AddChild(m_BloodParticle, boneIdx);
	}

	// Client side
	void InediaInfectedAI_DeactivateBleeding()
	{
		if (InediaInfectedAI_BleedingEffects && InediaInfectedAI_BleedingEffects.Count() > 0) {
			foreach (EffectParticle effect: InediaInfectedAI_BleedingEffects) {
				if (effect) {
					SEffectManager.DestroyEffect(effect);
				}
			}
		}
	}
	
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

// ......................................................................................................................................................
// ......................................................................................................................................................
// .....................................................................:::::::::........................................................................
// ...........................................................:------====--===%+:-**++--:................................................................
// ......................................................:-=---=--===-.        +=  -***+=++=-:...........................................................
// ...................................................:=-:....        .--:      =+   :**#=--=++=:........................................................
// ................................................:==: ...              .+=     -=    +-+=-...:+*=:.....................................................
// ..............................................-=-  ::.                  :*.    +:    +.-+-:....-++-...................................................
// ............................................-++-::--:::::.                *-    *     +.-::-.....:=*-:................................................
// ..........................................:+**-          .---              +-   -=    .* =.-=.......=+-...............................................
// .........................................:***.               -=-            *:   #     -= + ==.......:+=:.............................................
// ........................................:**%                    -=:          #.  :=     * .-:.=........-+:............................................
// .......................................:#=#%-====--:..            :=:        .#   *     -- - -.-........-+:...........................................
// .......................................*-# *.     ..:--======-:.    .=:       ==  =-     * . = -.........:+:..........................................
// ......................................-@.* .*.                .:-===: .=:      #.  *     *   = .-...:=++++#*..........................................
// .....................................-=#-=   +=                     :==--+     .*  +.    =:  =  =.++=:.....#*.........................................
// ....................................--:**.     --.                     .=+*-    +- :+    :=  =  **:........-#-........................................
// ...................................:+. #*: ::    :-::.                    =*=.   #. *    .+  +  #=.........-%#:.......................................
// ...................................= * .%::. :-:     :---:                  ++-   * :-   .+  *  %:.........+@-=.......................................
// ...................................= +- -+ .:::.:--.     .----.              *+=  :+ -   :+  # .%.......:=-:%.=.......................................
// ...................................= ==- #=   ::.-+++-::      :---:           *==. --.   -- :+ +=....:--:..:*.=.......................................
// ...................................= -=.==@--++=-=-..==+*+--:      :--:        +-=: -:   + .* :#..:--:....:*=.=.......................................
// ...................................= :=-:=#%*-:+%@@@#*+-:-=+**=--:     :-:      ==:- =.  +.= .*::-:.....-=-*:.=.......................................
// ...................................=. =..-:=*%@@#==--=*@%##+=-===+**+-::  --:    .=:::+ :+- :*:-.....-==-..*..=.......................................
// ...................................:= =..:#%@@*:  .:  -%@@#%*:-=+==---==**+==+=:   :--:++: -+-:..:===:.:--#=.--.......................................
// ....................................+.+...=@@*:-.   :.#-.:-:     ...-==---===+*##+-. -#:+-=:--===-:===---+@:.=........................................
// ....................................*=+-..=%@-.:-:.  =#::==      :-=++=-:. .:---+*#%#=:+-========++++=-@%%@-==........................................
// ....................................=:-#*-=#@+:  :=:=*--=#    .--:...:=+#@@#+=:    .:-----===-=*@@@%#%*@%*@+=-........................................
// ..................................:==+ -:=**@%:   .=#%.:*:   .:.          -*@@@%#.        .*@@%*=:    :%%+@@-:........................................
// .................................-=.:*- -  =@@+. : :-#--#      -###%@%%#*+-: -=-=        .**-:::::--. .@%+@*-.........................................
// ................................=::=-.+ =::-*@%:. .=.-*:*       :#@+:%%**#=*+ .-        .:.=+#*%%%@@%-:@%*#*-.........................................
// ...............................---=...:+.* -%@@=::=  :=:+        .=+==+=+-::+# .:      =:-+: ==**:=+:.:@%#*#..........................................
// ...............................+:=......-+%@@@#%. -  ::=-: .                          .+   ....:::.....@*#+*..........................................
// ...............................=+.........:+*#-%@*-. . # *.:.......                   .=.              %=%++..........................................
// ...............................:+:.......--:#==%*@+ .  +:-: .::.   ........            +..            .#*=*+..........................................
// ................................:+-..:---.-+=-*##+% -.  # =     .....      .........   +..            ***-*-..........................................
// ...................:-........::------:..-+*:.:+##.@.- = +=::         ....           ...=-..          ==%=:#...........................................
// ....................-+----=--::.....:==++-..-===*-+=- .+ #.+             ....:-         .=.         .+%=:*-...........................................
// .....................:----:.....:===--=-...--+=.** *-  --.*:=                -.:=--     .=-         ++=-+=............................................
// ..............................:==.:==-...:=:*=.:+*=.*  - --==:                ::: .:..:==-         :*+=-+.............................................
// .............................=-.-+=....:=--*-.:=+::+=.:. .:+:+                      .:= .-         *+=.*..............................................
// ...........................:+:-+-....--==*=..-++:..-###   ..*:+                  .::  -:          +*=:*:..............................................
// ...........................+:+-...:-===*-..:*+-..-=:.+#+:  :.+.+             :::.  .:...:::      :%--+:...............................................
// ..........................:==:..:==-:-+..=++:..-=:..+=+*+-- ::+.+        .---:.::::-::-----=-.  .*-+:+................................................
// ..........................-+-...*-..:*-++-...-=:..=*:+-+=+---:.+:=         ...     :     ::.   :+-*=%=:...............................................
// ..........................:*-..-=..-+*=.....=-...+=.=-=..*-====-*:=           ..:::-::::--....=++#*#@@%+:.............................................
// ...........................+=...=:-.=-.....+:..:*:.--+...+-- :=--==+           .::...:.....:-+%**%@@@@@++.............................................
// ............................==:.=....+....*:..:*:..+=....:*-  ..:=-::=.                 .:::=+***%@@@@%--:............................................
// .............................-*+.=....=--+-...*:..=:-....=%:. .....:==@+:                .:---++..#@@%.=:=............................................
// ...............................-+=*=.....*...==...-=:...+=@-= ........-%@%=:            .===..:*..%@#.=..=............................................
// ..................................:-=----+..:+-:...=-:-++=@#=..........:*@@@%#+=====+=--: .....*=#@+:=..:=............................................
// ........................................:=..=:..::==++:*- #@#............:+#%@@@#*=-.     ....:+@%:--...:=............................................
// ........................................:-..+----:...=-*.-.#@-...........-**+:.         .:.-+*#@+:=......=............................................
// ........................................-:..+.........=*  -.#@-........:*=.        ....-===::#%=--........*...........................................
// ........................................=:..+..........*   -:@@:......-*.       :::=++=:...:*=:=..........#...........................................
// ........................................=:..--..:-.....*    -+@%:....-*       .--+=:.....:==:=-.........-#=-----:..::-----------:.....................
// ........................................:+...=-...-----#     =@@*...=+       ::=-.......---.::.......:=*##*#*==-=:::. .::----:-=***+==-:..............
// .........................................:==-.-==-:::-=*:    -+@@=.-+      .--+.......:+:- :.......:+#***#+=:.  :::-==--=::::::....:-=+*#*-...........
// ........................................::--=+==**+==-+--     =@@%:*      ::+-.......:=:-  -.....-*#*#*=:  .:::-:::.:::.     ...:--:..:::-+*+:........
// .................................:-------:.:-++=:::-::=-:.    -#@@#     .-=+.........+::  ....:-#%#*=.  :::.... .:..    ...::.... ....::----=+*=:.....
// ............................::---:......-=**+.:::::. -%* +    :=@@.    :=+:.......:==+#::.. .+%*++: .::..... ::.     .::... ...:-=-----:......::==-...
// ............................:.:---::::=*+--::-.::..-#@#+=:-    =@+    -*=.....:::--:==:: .+##=--. ::. ..  .::     ::.... ..-===-.           ...:::--:.
// .........................:----:....:=*+-.=:-.:-..:*@#=-:-=.-.  -#:   +*:.---=-:---:::::=+%*-=-.:-: ..   .:.    ::......:===-                    ..::--
// .......................-=-........-=#= .=.:.-:..=@%-:-.=..+ .: -*:  +#---..----..:::-++++-==.:=-      ::     ::... .:-+=.                           .:
// .....................:=:.........=+*..-=...=...#@*..::.-..:+  ::*.:++:.:---. .:::.=*==+:==..--.     ::     ::.. ..:++:  ....       ...:::::::::::::...
// ....................=-.........:=*=:=-....:..:%@=..-.=..=..=-   :-=----:..::::..=+-=+:-=:.---     .:     ::   ..:+=. :::     .:::::--=***#%@@@@@@@@%#=
// ...................=:........:=-*-==......:.-@%:...=.::.--..+=-:+=--...:-:.:::--.=+:-+:..-:-    .:      -.  .:.+=  ::    .:::.:=#%#+-:::----+*%@@@@@@@
// ..................=:........:=-#=+:.....:=.:@%:....-:.=..+..........:-:.:::.--.-=::=:...-::    :.     :-   :.-+. ::    ::.:=#@#+::-+**+*----=*%##@@@@@
// .................=:........--=##-......-=.:%%:......=..-..=......:::.:::..:-.-=::=-.-:+=:.   .-      -.  .:.+: ::   ::.:+%@*=--++=-:....--....=+*#@@@@
// ................--........=:-#+=......=*..%@:.......:-.-..-:...--..:-....-:--::=:.-: :+*.   ::     .=   -.:= ::   :-.-#@%=:=++-:.........:=....=.-#@@@
// ...............:=........-:.#*+......=+..*@=.........-..-..=:-:..::....--:=::=:.-.   +:*:  :.     .=   -.=:.-   .-.:#@#-=+=-..............-:...--.:%@@
// ..............:+........:=.+#=.....:=+..*@=...........-:.-::::::..:::::--:-=:.-:    --=.= -.     .-   =.-.-.   :::*@#===:..................=....=..-+%
// ..............+.........+.:@=.....-=+.-%%-..............-:::::-:::...--.-=..::      % + =-      .-   =  ::     .=@%==-.....................--....=..=-
// .............=:........--.*#....:==-:*#-...........................--.==..::       =*.- .::::::--   =:..      .*@+=-........................=....=..:-
// ............--.........+.:@-...===:--............................-:.==...:.        %+::       .=.  =-        .#%=-..........................:-....=..-

// INJURY HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool m_InediaInfectedAI_LegsInjured = false;
	float m_InediaInfectedAI_LegsCumulativeDamage = 0;
	void InediaInfectedAI_EEHitByInjuryActions(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!IsAlive() || !m_InediaInfectedAI_ConfigZombie.InjuryHandlerIsActive || !damageResult || !source) {
			return;
		}

		// When an animal takes damage from an explosion, injury legs with chance
		if (damageType == DT_EXPLOSION && m_InediaInfectedAI_ConfigZombie.InjuryHandlerInjuryOnExplosiveDamageChancePercent > 0 && Math.RandomFloatInclusive(0, 100) <= m_InediaInfectedAI_ConfigZombie.InjuryHandlerInjuryOnExplosiveDamageChancePercent) {
			dmgZone = "Zone_Legs";
		}

		if (dmgZone == "Zone_Legs") {
			if (m_InediaInfectedAI_LegsInjured) {
				// Legs is already injured
				return;
			}

			float damage = m_InediaInfectedAI_DamageMultiplier * damageResult.GetDamage(dmgZone, "");
			if (damage <= 0) {
				return;
			}

			m_InediaInfectedAI_LegsCumulativeDamage += damage;

			if (m_InediaInfectedAI_LegsCumulativeDamage > m_InediaInfectedAI_ConfigZombie.InjuryHandlerDamageThresholdToInjuryLeg) {
				m_InediaInfectedAI_LegsInjured = true;

				if (m_InediaInfectedAI_ConfigZombie.InjuryHandlerRestoreInjuredLegAfterSeconds > 0) {
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InediaInfectedAI_RestoreLegs, m_InediaInfectedAI_ConfigZombie.InjuryHandlerRestoreInjuredLegAfterSeconds * 1000, false);
				}
			}
		}
	}

	void InediaInfectedAI_RestoreLegs()
	{
		if (!m_InediaInfectedAI_LegsInjured) return;

		m_InediaInfectedAI_LegsInjured = false;
		m_InediaInfectedAI_LegsCumulativeDamage = 0;
	}

// SPEED HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	float m_InediaInfectedAI_SpeedHandlerTimer = 0;
	protected void InediaInfectedAI_SpeedHandler(float pDt)
	{
		if (!m_InediaInfectedAI_ConfigZombie.SpeedHandlerIsActive) return;

		m_InediaInfectedAI_SpeedHandlerTimer -= pDt;
        if (m_InediaInfectedAI_SpeedHandlerTimer > 0) return;
        m_InediaInfectedAI_SpeedHandlerTimer = 5;

		// If a animal has lost a lot of blood or his legs are damaged and alerted, we limit its speed to jogging
		if ((InediaInfectedAI_IsBloodVolumeVeryLow() || m_InediaInfectedAI_LegsInjured) && GetInputController().GetAlertLevel() > 0) {
			float movementSpeedLimit = 3;

			if (this.IsInherited(Animal_GallusGallusDomesticus)) {
				movementSpeedLimit = 2;
			}

			if (GetInputController().GetMovementSpeed() >= movementSpeedLimit) {
				InediaInfectedAI_OverrideMovementSpeedForSeconds(movementSpeedLimit, 10);
			}
		}
	}

	bool m_InediaInfectedAI_ForbidMovement = false;
	float m_InediaInfectedAI_ForbidMovementForSeconds = 0;
	void InediaInfectedAI_ForbidMovementForSeconds(float seconds)
	{
		if (seconds > m_InediaInfectedAI_ForbidMovementForSeconds && !m_InediaInfectedAI_ForbidMovement) {
			GetInputController().OverrideMovementSpeed(true, 0);
			m_InediaInfectedAI_ForbidMovement = true;
			m_InediaInfectedAI_ForbidMovementForSeconds = seconds;
		}
	}

	void InediaInfectedAI_DisableForbidMovementForSeconds()
	{
		if (m_InediaInfectedAI_ForbidMovement) {
			GetInputController().OverrideMovementSpeed(false, 0);
			m_InediaInfectedAI_ForbidMovement = false;
			m_InediaInfectedAI_ForbidMovementForSeconds = 0;
		}
	}

	bool m_InediaInfectedAI_MovementSpeedOverrided = false;
	float m_InediaInfectedAI_OverrideMovementSpeedForSeconds = 0;
	void InediaInfectedAI_OverrideMovementSpeedForSeconds(float speed, float seconds)
	{
		if (m_InediaInfectedAI_ForbidMovement) return;

		GetInputController().OverrideMovementSpeed(true, speed);
		m_InediaInfectedAI_MovementSpeedOverrided = true;
		m_InediaInfectedAI_OverrideMovementSpeedForSeconds = seconds;
	}

	void InediaInfectedAI_DisableOverrideMovementSpeedForSeconds()
	{
		if (m_InediaInfectedAI_ForbidMovement) return;

		if (m_InediaInfectedAI_MovementSpeedOverrided) {
			GetInputController().OverrideMovementSpeed(false, 0);
			m_InediaInfectedAI_MovementSpeedOverrided = false;
			m_InediaInfectedAI_OverrideMovementSpeedForSeconds = 0;
		}
	}

// CAR ATTACK HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    float m_InediaInfectedAI_CarAttackHandlerTimer = 0;
    protected void InediaInfectedAI_CarAttackHandler(float pDt)
    {
        if (!m_InediaInfectedAI_ConfigZombie.AttackCarHandlerIsActive) {
            return;
        }

        m_InediaInfectedAI_CarAttackHandlerTimer -= pDt;
        if (m_InediaInfectedAI_CarAttackHandlerTimer > 0) return;
        m_InediaInfectedAI_CarAttackHandlerTimer = 2;

		if (!m_InediaInfectedAI_PredatorFirstAmmo) {
			return;
		}

		int alertLevel = 0;
		if (GetInputController()) {
			alertLevel = GetInputController().GetAlertLevel();
		}

		if (alertLevel < 2) {
			return;
		}

        // Looking for a vehicle nearby to attack
		CarScript vehicle;
        array<CarScript> nearVehicles = new array<CarScript>;
		MiscGameplayFunctions.InediaInfectedAI_GetVehiclesAtPosition3D(GetPosition(), 5, nearVehicles);
		if (nearVehicles.Count() > 0) {
			vehicle = nearVehicles.Get(0);
		}

        if (vehicle) {
            vehicle.InediaInfectedAI_AttackedByEntity(this);
        }
    }

// ......................................................................................................................................................
// ......................................................................................................................................................
// .....................................................................:::::::::........................................................................
// ...........................................................:------====--===%+:-**++--:................................................................
// ......................................................:-=---=--===-.        +=  -***+=++=-:...........................................................
// ...................................................:=-:....        .--:      =+   :**#=--=++=:........................................................
// ................................................:==: ...              .+=     -=    +-+=-...:+*=:.....................................................
// ..............................................-=-  ::.                  :*.    +:    +.-+-:....-++-...................................................
// ............................................-++-::--:::::.                *-    *     +.-::-.....:=*-:................................................
// ..........................................:+**-          .---              +-   -=    .* =.-=.......=+-...............................................
// .........................................:***.               -=-            *:   #     -= + ==.......:+=:.............................................
// ........................................:**%                    -=:          #.  :=     * .-:.=........-+:............................................
// .......................................:#=#%-====--:..            :=:        .#   *     -- - -.-........-+:...........................................
// .......................................*-# *.     ..:--======-:.    .=:       ==  =-     * . = -.........:+:..........................................
// ......................................-@.* .*.                .:-===: .=:      #.  *     *   = .-...:=++++#*..........................................
// .....................................-=#-=   +=                     :==--+     .*  +.    =:  =  =.++=:.....#*.........................................
// ....................................--:**.     --.                     .=+*-    +- :+    :=  =  **:........-#-........................................
// ...................................:+. #*: ::    :-::.                    =*=.   #. *    .+  +  #=.........-%#:.......................................
// ...................................= * .%::. :-:     :---:                  ++-   * :-   .+  *  %:.........+@-=.......................................
// ...................................= +- -+ .:::.:--.     .----.              *+=  :+ -   :+  # .%.......:=-:%.=.......................................
// ...................................= ==- #=   ::.-+++-::      :---:           *==. --.   -- :+ +=....:--:..:*.=.......................................
// ...................................= -=.==@--++=-=-..==+*+--:      :--:        +-=: -:   + .* :#..:--:....:*=.=.......................................
// ...................................= :=-:=#%*-:+%@@@#*+-:-=+**=--:     :-:      ==:- =.  +.= .*::-:.....-=-*:.=.......................................
// ...................................=. =..-:=*%@@#==--=*@%##+=-===+**+-::  --:    .=:::+ :+- :*:-.....-==-..*..=.......................................
// ...................................:= =..:#%@@*:  .:  -%@@#%*:-=+==---==**+==+=:   :--:++: -+-:..:===:.:--#=.--.......................................
// ....................................+.+...=@@*:-.   :.#-.:-:     ...-==---===+*##+-. -#:+-=:--===-:===---+@:.=........................................
// ....................................*=+-..=%@-.:-:.  =#::==      :-=++=-:. .:---+*#%#=:+-========++++=-@%%@-==........................................
// ....................................=:-#*-=#@+:  :=:=*--=#    .--:...:=+#@@#+=:    .:-----===-=*@@@%#%*@%*@+=-........................................
// ..................................:==+ -:=**@%:   .=#%.:*:   .:.          -*@@@%#.        .*@@%*=:    :%%+@@-:........................................
// .................................-=.:*- -  =@@+. : :-#--#      -###%@%%#*+-: -=-=        .**-:::::--. .@%+@*-.........................................
// ................................=::=-.+ =::-*@%:. .=.-*:*       :#@+:%%**#=*+ .-        .:.=+#*%%%@@%-:@%*#*-.........................................
// ...............................---=...:+.* -%@@=::=  :=:+        .=+==+=+-::+# .:      =:-+: ==**:=+:.:@%#*#..........................................
// ...............................+:=......-+%@@@#%. -  ::=-: .                          .+   ....:::.....@*#+*..........................................
// ...............................=+.........:+*#-%@*-. . # *.:.......                   .=.              %=%++..........................................
// ...............................:+:.......--:#==%*@+ .  +:-: .::.   ........            +..            .#*=*+..........................................
// ................................:+-..:---.-+=-*##+% -.  # =     .....      .........   +..            ***-*-..........................................
// ...................:-........::------:..-+*:.:+##.@.- = +=::         ....           ...=-..          ==%=:#...........................................
// ....................-+----=--::.....:==++-..-===*-+=- .+ #.+             ....:-         .=.         .+%=:*-...........................................
// .....................:----:.....:===--=-...--+=.** *-  --.*:=                -.:=--     .=-         ++=-+=............................................
// ..............................:==.:==-...:=:*=.:+*=.*  - --==:                ::: .:..:==-         :*+=-+.............................................
// .............................=-.-+=....:=--*-.:=+::+=.:. .:+:+                      .:= .-         *+=.*..............................................
// ...........................:+:-+-....--==*=..-++:..-###   ..*:+                  .::  -:          +*=:*:..............................................
// ...........................+:+-...:-===*-..:*+-..-=:.+#+:  :.+.+             :::.  .:...:::      :%--+:...............................................
// ..........................:==:..:==-:-+..=++:..-=:..+=+*+-- ::+.+        .---:.::::-::-----=-.  .*-+:+................................................
// ..........................-+-...*-..:*-++-...-=:..=*:+-+=+---:.+:=         ...     :     ::.   :+-*=%=:...............................................
// ..........................:*-..-=..-+*=.....=-...+=.=-=..*-====-*:=           ..:::-::::--....=++#*#@@%+:.............................................
// ...........................+=...=:-.=-.....+:..:*:.--+...+-- :=--==+           .::...:.....:-+%**%@@@@@++.............................................
// ............................==:.=....+....*:..:*:..+=....:*-  ..:=-::=.                 .:::=+***%@@@@%--:............................................
// .............................-*+.=....=--+-...*:..=:-....=%:. .....:==@+:                .:---++..#@@%.=:=............................................
// ...............................-+=*=.....*...==...-=:...+=@-= ........-%@%=:            .===..:*..%@#.=..=............................................
// ..................................:-=----+..:+-:...=-:-++=@#=..........:*@@@%#+=====+=--: .....*=#@+:=..:=............................................
// ........................................:=..=:..::==++:*- #@#............:+#%@@@#*=-.     ....:+@%:--...:=............................................
// ........................................:-..+----:...=-*.-.#@-...........-**+:.         .:.-+*#@+:=......=............................................
// ........................................-:..+.........=*  -.#@-........:*=.        ....-===::#%=--........*...........................................
// ........................................=:..+..........*   -:@@:......-*.       :::=++=:...:*=:=..........#...........................................
// ........................................=:..--..:-.....*    -+@%:....-*       .--+=:.....:==:=-.........-#=-----:..::-----------:.....................
// ........................................:+...=-...-----#     =@@*...=+       ::=-.......---.::.......:=*##*#*==-=:::. .::----:-=***+==-:..............
// .........................................:==-.-==-:::-=*:    -+@@=.-+      .--+.......:+:- :.......:+#***#+=:.  :::-==--=::::::....:-=+*#*-...........
// ........................................::--=+==**+==-+--     =@@%:*      ::+-.......:=:-  -.....-*#*#*=:  .:::-:::.:::.     ...:--:..:::-+*+:........
// .................................:-------:.:-++=:::-::=-:.    -#@@#     .-=+.........+::  ....:-#%#*=.  :::.... .:..    ...::.... ....::----=+*=:.....
// ............................::---:......-=**+.:::::. -%* +    :=@@.    :=+:.......:==+#::.. .+%*++: .::..... ::.     .::... ...:-=-----:......::==-...
// ............................:.:---::::=*+--::-.::..-#@#+=:-    =@+    -*=.....:::--:==:: .+##=--. ::. ..  .::     ::.... ..-===-.           ...:::--:.
// .........................:----:....:=*+-.=:-.:-..:*@#=-:-=.-.  -#:   +*:.---=-:---:::::=+%*-=-.:-: ..   .:.    ::......:===-                    ..::--
// .......................-=-........-=#= .=.:.-:..=@%-:-.=..+ .: -*:  +#---..----..:::-++++-==.:=-      ::     ::... .:-+=.                           .:
// .....................:=:.........=+*..-=...=...#@*..::.-..:+  ::*.:++:.:---. .:::.=*==+:==..--.     ::     ::.. ..:++:  ....       ...:::::::::::::...
// ....................=-.........:=*=:=-....:..:%@=..-.=..=..=-   :-=----:..::::..=+-=+:-=:.---     .:     ::   ..:+=. :::     .:::::--=***#%@@@@@@@@%#=
// ...................=:........:=-*-==......:.-@%:...=.::.--..+=-:+=--...:-:.:::--.=+:-+:..-:-    .:      -.  .:.+=  ::    .:::.:=#%#+-:::----+*%@@@@@@@
// ..................=:........:=-#=+:.....:=.:@%:....-:.=..+..........:-:.:::.--.-=::=:...-::    :.     :-   :.-+. ::    ::.:=#@#+::-+**+*----=*%##@@@@@
// .................=:........--=##-......-=.:%%:......=..-..=......:::.:::..:-.-=::=-.-:+=:.   .-      -.  .:.+: ::   ::.:+%@*=--++=-:....--....=+*#@@@@
// ................--........=:-#+=......=*..%@:.......:-.-..-:...--..:-....-:--::=:.-: :+*.   ::     .=   -.:= ::   :-.-#@%=:=++-:.........:=....=.-#@@@
// ...............:=........-:.#*+......=+..*@=.........-..-..=:-:..::....--:=::=:.-.   +:*:  :.     .=   -.=:.-   .-.:#@#-=+=-..............-:...--.:%@@
// ..............:+........:=.+#=.....:=+..*@=...........-:.-::::::..:::::--:-=:.-:    --=.= -.     .-   =.-.-.   :::*@#===:..................=....=..-+%
// ..............+.........+.:@=.....-=+.-%%-..............-:::::-:::...--.-=..::      % + =-      .-   =  ::     .=@%==-.....................--....=..=-
// .............=:........--.*#....:==-:*#-...........................--.==..::       =*.- .::::::--   =:..      .*@+=-........................=....=..:-
// ............--.........+.:@-...===:--............................-:.==...:.        %+::       .=.  =-        .#%=-..........................:-....=..-

// CAR COLLISIONS HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
	
// DAMAGE FROM ENTITIES HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_EEHitByFromAnyToAnimalDamage(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
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
		
		if (ammo == "InediaInfectedAI_ThrowingProjectile") {
			// Damage from thrown projectiles
			InediaInfectedAI_EEHitByFromThrowingProjectileToAnimalDamage(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		} else {
			InediaInfectedAI_EEHitByFromCreatureToAnimalDamage(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		}
	}
	
// DAMAGE FROM PLAYERS ATTACKS HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_EEHitByFromCreatureToAnimalDamage(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{		
		if (!damageResult || !source || !m_InediaInfectedAI_ConfigZombie.DamageToZombieHandlerIsActive) {
			return;
		}
		
		// Prevent the vanilla game or other mods from killing the animal.
		SetFullHealth();
		
		m_InediaInfectedAI_WeaponMultiplier *= m_InediaInfectedAI_ConfigZombie.GetWeaponMultiplierByWeapon(source, ammo, damageType);		
		m_InediaInfectedAI_DamageMultiplier *= m_InediaInfectedAI_WeaponMultiplier;
		
		if (dmgZone == "Zone_Head") {
			m_InediaInfectedAI_DamageMultiplier *= 4;
		}
	
		if (source.IsInherited(ZombieBase)) {
			ZombieBase zmb = ZombieBase.Cast(source);
			if (zmb) {
				m_InediaInfectedAI_DamageMultiplier *= zmb.InediaInfectedAI_GetInjuryDamageMultiplier() * zmb.InediaInfectedAI_GetBloodDamageMultiplier() * zmb.InediaInfectedAI_GetScaleDamageMultiplier();
			}
		} else if (source.IsInherited(PlayerBase)) {
			PlayerBase player = PlayerBase.Cast(source);
			if (player) {
				if (damageType == DT_CLOSE_COMBAT) {
					m_InediaInfectedAI_DamageMultiplier *= player.InediaInfectedAI_GetPainManager().GetPainArmsDamageMultiplier();
					m_InediaInfectedAI_DamageMultiplier *= InediaInfectedAI_SyberiaCompatibility.GetSyberiaMeleeDamageMultiplierToAllCreatures(player);
					m_InediaInfectedAI_DamageMultiplier *= InediaInfectedAI_SyberiaCompatibility.GetSyberiaMeleeDamageMultiplierToAnimals(player);
				} else if (damageType == DT_FIRE_ARM) {
					m_InediaInfectedAI_DamageMultiplier *= InediaInfectedAI_SyberiaCompatibility.GetSyberiaRangeDamageMultiplierToAnimals(player);
				}
			}
		}
		
		// Health damage
		float healthAfterDamage = m_InediaInfectedAI_VanillaHealthBeforeEEHitBy - m_InediaInfectedAI_HealthDamage * m_InediaInfectedAI_DamageMultiplier;
		if (healthAfterDamage > 0) {
			SetHealth(healthAfterDamage);
		} else {
			InediaInfectedAI_Kill();
		}
	}

// DAMAGE FROM PROJECTILE HANDLER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_EEHitByFromThrowingProjectileToAnimalDamage(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (ammo != "InediaInfectedAI_ThrowingProjectile" || !damageResult || !source || !source.IsInherited(ItemBase)) {
			return;
		}
		
		ItemBase projectile = ItemBase.Cast(source);
		if (!projectile || !projectile.InediaInfectedAI_IsThrowingProjectile()) {
			return;
		}
		
		damageType = DT_CUSTOM;

		if (projectile.m_InediaInfectedAI_ThrowingProjectile.m_ThrowerEntity) {
			m_InediaInfectedAI_EEHitByLastSource = projectile.m_InediaInfectedAI_ThrowingProjectile.m_ThrowerEntity;
		}
		
		// Prevent the vanilla game or other mods from killing the animal.
		SetFullHealth();
		
		// Retrieve the damage that was previously dealt to the infected by other mods.
		float healthDamageFromOtherMods = m_InediaInfectedAI_HealthDamage - damageResult.GetDamage("", "Health");

		// Armor multipliers
		float healthArmorMultiplier = damageResult.GetDamage(dmgZone, "Health") / ItemBase.InediaInfectedAI_ThrowingProjectileGetAmmoHealthDamage();
		float shockArmorMultiplier = damageResult.GetDamage(dmgZone, "Shock") / ItemBase.InediaInfectedAI_ThrowingProjectileGetAmmoShockDamage();

		m_InediaInfectedAI_HealthDamage = healthArmorMultiplier * projectile.InediaInfectedAI_ThrowingProjectileGetHealthDamage() + healthDamageFromOtherMods;
		
		if (m_InediaInfectedAI_HealthDamage > 0) {
			float healthAfterDamage = m_InediaInfectedAI_VanillaHealthBeforeEEHitBy - m_InediaInfectedAI_HealthDamage;
			if (healthAfterDamage > 0) {
				SetHealth(healthAfterDamage);
			} else {
				InediaInfectedAI_Kill();
			}
		}

		// Stun chance
		float stunChance = shockArmorMultiplier * projectile.InediaInfectedAI_ThrowingProjectileGetStunChancePercent();
		if (stunChance > 0 && Math.RandomFloatInclusive(0, 100) <= stunChance) {
			// The "-10" trick is required because the contact event is not always triggered on the surface of the body due to the velocity of the projectile and the frame rate
			float hitDirectionAngle = InediaInfectedAI_GetToPositionDirectionAngleX(projectile.GetPosition() - 10 * GetVelocity(projectile).Normalized());
			StartCommand_Hit(m_DamageHitType, hitDirectionAngle);
		}
	}

	// With the DayZ 1.28 update, collisions with throwable projectiles began to be handled by events within the projectiles themselves (ItemBase).
	/*
	override protected void EOnContact(IEntity other, Contact extra)
	{
		super.EOnContact(other, extra);

#ifdef SERVER
		if (other && other.IsInherited(ItemBase)) {
			ItemBase projectile = ItemBase.Cast(other);
			if (projectile && projectile.InediaInfectedAI_IsThrowingProjectile()) {
				projectile.InediaInfectedAI_ThrowingProjectileOnContactServer(this);
			}
		}
#endif
	}
	*/

// RELATIONSHIPS WITH ENTITIES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override bool CanBeTargetedByAI(EntityAI ai)
	{
		if (!ai) {
			return super.CanBeTargetedByAI(ai);
		}
		
#ifdef Dayz_Dog
		if (InediaInfectedAI_Functions.IsKindOfCached("Dayz_Doggo", this)) {
			return super.CanBeTargetedByAI(ai);
		}
#endif

#ifdef SERVER
		InediaInfectedAI_InitOnceServer();
		
		if (m_InediaInfectedAI_ConfigZombie && IsAlive()) {
			if (ai.IsInherited(ZombieBase)) {
				ZombieBase zmb = ZombieBase.Cast(ai);
				if (zmb) {
					zmb.InediaInfectedAI_InitOnceServer();
					if (zmb.m_InediaInfectedAI_ConfigZombie && zmb.m_InediaInfectedAI_ConfigZombie.AttackAnimalsHandlerIsActive) {
						return zmb.InediaInfectedAI_CanAttackAnimal(this);
					}
				}
			}
		}
#endif

		return super.CanBeTargetedByAI(ai);
	}

	bool InediaInfectedAI_IsFriendly(PlayerBase player)
	{
		if (!m_InediaInfectedAI_ConfigZombie || !player) {
			return false;
		}

		return InediaInfectedAI_IsFriendlyNPC(player) || InediaInfectedAI_IsFriendlyPlayer(player);
	}

	bool InediaInfectedAI_IsFriendlyNPC(PlayerBase npc)
	{
		if (!m_InediaInfectedAI_ConfigZombie || !npc || !npc.InediaInfectedAI_IsNPC()) {
			return false;
		}
		
		if (InediaInfectedAI_ExpansionAICompatibility.IsEAIPlayerInCreatureFriendlyFaction(npc, this)) {
			return true;
		}

		foreach (string NPCClassName: m_InediaInfectedAI_ConfigZombie.FriendlyNPC) {
			if (InediaInfectedAI_Functions.IsKindOfCached(NPCClassName, npc)) {
				return true;
			}
		}

		return false;
	}

	bool InediaInfectedAI_IsFriendlyPlayer(PlayerBase player)
	{
		if (!m_InediaInfectedAI_ConfigZombie || !player) {
			return false;
		}

		return player.InediaInfectedAI_IsInHideMode();
	}
	
	override bool ResistContaminatedEffect()
	{
#ifdef SERVER
		InediaInfectedAI_InitOnceServer();
		
		if (m_InediaInfectedAI_ConfigZombie && m_InediaInfectedAI_ConfigZombie.ResistContaminatedEffectHandlerIsActive) {
			return m_InediaInfectedAI_ConfigZombie.ResistContaminatedEffect;
		}
#endif

		return super.ResistContaminatedEffect();
	}

// ......................................................................................................................................................
// ......................................................................................................................................................
// .....................................................................:::::::::........................................................................
// ...........................................................:------====--===%+:-**++--:................................................................
// ......................................................:-=---=--===-.        +=  -***+=++=-:...........................................................
// ...................................................:=-:....        .--:      =+   :**#=--=++=:........................................................
// ................................................:==: ...              .+=     -=    +-+=-...:+*=:.....................................................
// ..............................................-=-  ::.                  :*.    +:    +.-+-:....-++-...................................................
// ............................................-++-::--:::::.                *-    *     +.-::-.....:=*-:................................................
// ..........................................:+**-          .---              +-   -=    .* =.-=.......=+-...............................................
// .........................................:***.               -=-            *:   #     -= + ==.......:+=:.............................................
// ........................................:**%                    -=:          #.  :=     * .-:.=........-+:............................................
// .......................................:#=#%-====--:..            :=:        .#   *     -- - -.-........-+:...........................................
// .......................................*-# *.     ..:--======-:.    .=:       ==  =-     * . = -.........:+:..........................................
// ......................................-@.* .*.                .:-===: .=:      #.  *     *   = .-...:=++++#*..........................................
// .....................................-=#-=   +=                     :==--+     .*  +.    =:  =  =.++=:.....#*.........................................
// ....................................--:**.     --.                     .=+*-    +- :+    :=  =  **:........-#-........................................
// ...................................:+. #*: ::    :-::.                    =*=.   #. *    .+  +  #=.........-%#:.......................................
// ...................................= * .%::. :-:     :---:                  ++-   * :-   .+  *  %:.........+@-=.......................................
// ...................................= +- -+ .:::.:--.     .----.              *+=  :+ -   :+  # .%.......:=-:%.=.......................................
// ...................................= ==- #=   ::.-+++-::      :---:           *==. --.   -- :+ +=....:--:..:*.=.......................................
// ...................................= -=.==@--++=-=-..==+*+--:      :--:        +-=: -:   + .* :#..:--:....:*=.=.......................................
// ...................................= :=-:=#%*-:+%@@@#*+-:-=+**=--:     :-:      ==:- =.  +.= .*::-:.....-=-*:.=.......................................
// ...................................=. =..-:=*%@@#==--=*@%##+=-===+**+-::  --:    .=:::+ :+- :*:-.....-==-..*..=.......................................
// ...................................:= =..:#%@@*:  .:  -%@@#%*:-=+==---==**+==+=:   :--:++: -+-:..:===:.:--#=.--.......................................
// ....................................+.+...=@@*:-.   :.#-.:-:     ...-==---===+*##+-. -#:+-=:--===-:===---+@:.=........................................
// ....................................*=+-..=%@-.:-:.  =#::==      :-=++=-:. .:---+*#%#=:+-========++++=-@%%@-==........................................
// ....................................=:-#*-=#@+:  :=:=*--=#    .--:...:=+#@@#+=:    .:-----===-=*@@@%#%*@%*@+=-........................................
// ..................................:==+ -:=**@%:   .=#%.:*:   .:.          -*@@@%#.        .*@@%*=:    :%%+@@-:........................................
// .................................-=.:*- -  =@@+. : :-#--#      -###%@%%#*+-: -=-=        .**-:::::--. .@%+@*-.........................................
// ................................=::=-.+ =::-*@%:. .=.-*:*       :#@+:%%**#=*+ .-        .:.=+#*%%%@@%-:@%*#*-.........................................
// ...............................---=...:+.* -%@@=::=  :=:+        .=+==+=+-::+# .:      =:-+: ==**:=+:.:@%#*#..........................................
// ...............................+:=......-+%@@@#%. -  ::=-: .                          .+   ....:::.....@*#+*..........................................
// ...............................=+.........:+*#-%@*-. . # *.:.......                   .=.              %=%++..........................................
// ...............................:+:.......--:#==%*@+ .  +:-: .::.   ........            +..            .#*=*+..........................................
// ................................:+-..:---.-+=-*##+% -.  # =     .....      .........   +..            ***-*-..........................................
// ...................:-........::------:..-+*:.:+##.@.- = +=::         ....           ...=-..          ==%=:#...........................................
// ....................-+----=--::.....:==++-..-===*-+=- .+ #.+             ....:-         .=.         .+%=:*-...........................................
// .....................:----:.....:===--=-...--+=.** *-  --.*:=                -.:=--     .=-         ++=-+=............................................
// ..............................:==.:==-...:=:*=.:+*=.*  - --==:                ::: .:..:==-         :*+=-+.............................................
// .............................=-.-+=....:=--*-.:=+::+=.:. .:+:+                      .:= .-         *+=.*..............................................
// ...........................:+:-+-....--==*=..-++:..-###   ..*:+                  .::  -:          +*=:*:..............................................
// ...........................+:+-...:-===*-..:*+-..-=:.+#+:  :.+.+             :::.  .:...:::      :%--+:...............................................
// ..........................:==:..:==-:-+..=++:..-=:..+=+*+-- ::+.+        .---:.::::-::-----=-.  .*-+:+................................................
// ..........................-+-...*-..:*-++-...-=:..=*:+-+=+---:.+:=         ...     :     ::.   :+-*=%=:...............................................
// ..........................:*-..-=..-+*=.....=-...+=.=-=..*-====-*:=           ..:::-::::--....=++#*#@@%+:.............................................
// ...........................+=...=:-.=-.....+:..:*:.--+...+-- :=--==+           .::...:.....:-+%**%@@@@@++.............................................
// ............................==:.=....+....*:..:*:..+=....:*-  ..:=-::=.                 .:::=+***%@@@@%--:............................................
// .............................-*+.=....=--+-...*:..=:-....=%:. .....:==@+:                .:---++..#@@%.=:=............................................
// ...............................-+=*=.....*...==...-=:...+=@-= ........-%@%=:            .===..:*..%@#.=..=............................................
// ..................................:-=----+..:+-:...=-:-++=@#=..........:*@@@%#+=====+=--: .....*=#@+:=..:=............................................
// ........................................:=..=:..::==++:*- #@#............:+#%@@@#*=-.     ....:+@%:--...:=............................................
// ........................................:-..+----:...=-*.-.#@-...........-**+:.         .:.-+*#@+:=......=............................................
// ........................................-:..+.........=*  -.#@-........:*=.        ....-===::#%=--........*...........................................
// ........................................=:..+..........*   -:@@:......-*.       :::=++=:...:*=:=..........#...........................................
// ........................................=:..--..:-.....*    -+@%:....-*       .--+=:.....:==:=-.........-#=-----:..::-----------:.....................
// ........................................:+...=-...-----#     =@@*...=+       ::=-.......---.::.......:=*##*#*==-=:::. .::----:-=***+==-:..............
// .........................................:==-.-==-:::-=*:    -+@@=.-+      .--+.......:+:- :.......:+#***#+=:.  :::-==--=::::::....:-=+*#*-...........
// ........................................::--=+==**+==-+--     =@@%:*      ::+-.......:=:-  -.....-*#*#*=:  .:::-:::.:::.     ...:--:..:::-+*+:........
// .................................:-------:.:-++=:::-::=-:.    -#@@#     .-=+.........+::  ....:-#%#*=.  :::.... .:..    ...::.... ....::----=+*=:.....
// ............................::---:......-=**+.:::::. -%* +    :=@@.    :=+:.......:==+#::.. .+%*++: .::..... ::.     .::... ...:-=-----:......::==-...
// ............................:.:---::::=*+--::-.::..-#@#+=:-    =@+    -*=.....:::--:==:: .+##=--. ::. ..  .::     ::.... ..-===-.           ...:::--:.
// .........................:----:....:=*+-.=:-.:-..:*@#=-:-=.-.  -#:   +*:.---=-:---:::::=+%*-=-.:-: ..   .:.    ::......:===-                    ..::--
// .......................-=-........-=#= .=.:.-:..=@%-:-.=..+ .: -*:  +#---..----..:::-++++-==.:=-      ::     ::... .:-+=.                           .:
// .....................:=:.........=+*..-=...=...#@*..::.-..:+  ::*.:++:.:---. .:::.=*==+:==..--.     ::     ::.. ..:++:  ....       ...:::::::::::::...
// ....................=-.........:=*=:=-....:..:%@=..-.=..=..=-   :-=----:..::::..=+-=+:-=:.---     .:     ::   ..:+=. :::     .:::::--=***#%@@@@@@@@%#=
// ...................=:........:=-*-==......:.-@%:...=.::.--..+=-:+=--...:-:.:::--.=+:-+:..-:-    .:      -.  .:.+=  ::    .:::.:=#%#+-:::----+*%@@@@@@@
// ..................=:........:=-#=+:.....:=.:@%:....-:.=..+..........:-:.:::.--.-=::=:...-::    :.     :-   :.-+. ::    ::.:=#@#+::-+**+*----=*%##@@@@@
// .................=:........--=##-......-=.:%%:......=..-..=......:::.:::..:-.-=::=-.-:+=:.   .-      -.  .:.+: ::   ::.:+%@*=--++=-:....--....=+*#@@@@
// ................--........=:-#+=......=*..%@:.......:-.-..-:...--..:-....-:--::=:.-: :+*.   ::     .=   -.:= ::   :-.-#@%=:=++-:.........:=....=.-#@@@
// ...............:=........-:.#*+......=+..*@=.........-..-..=:-:..::....--:=::=:.-.   +:*:  :.     .=   -.=:.-   .-.:#@#-=+=-..............-:...--.:%@@
// ..............:+........:=.+#=.....:=+..*@=...........-:.-::::::..:::::--:-=:.-:    --=.= -.     .-   =.-.-.   :::*@#===:..................=....=..-+%
// ..............+.........+.:@=.....-=+.-%%-..............-:::::-:::...--.-=..::      % + =-      .-   =  ::     .=@%==-.....................--....=..=-
// .............=:........--.*#....:==-:*#-...........................--.==..::       =*.- .::::::--   =:..      .*@+=-........................=....=..:-
// ............--.........+.:@-...===:--............................-:.==...:.        %+::       .=.  =-        .#%=-..........................:-....=..-

// AUXILIARY METHODS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void InediaInfectedAI_SetNetSyncBoolVariable(string name, bool value)
	{
		int position = m_InediaInfectedAI_NetSyncBoolVariables.Find(name);
		
		if (position >= 0 && position < 32) {
	        if (value) {
	            m_InediaInfectedAI_NetSyncBoolVariablesSyncBits |= (1 << position);
	        } else {
	            m_InediaInfectedAI_NetSyncBoolVariablesSyncBits &= ~(1 << position);
	        }
		}
	}
	
	bool InediaInfectedAI_GetNetSyncBoolVariable(string name)
	{
		int position = m_InediaInfectedAI_NetSyncBoolVariables.Find(name);
		
		if (position >= 0 && position < 32) {
			return (m_InediaInfectedAI_NetSyncBoolVariablesSyncBits & (1 << position)) != 0;
		}
		
		return false;
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
	
	string InediaInfectedAI_GetRandomDmgComponentName()
	{
		if (m_DamageZoneMap) {
			array<string> dmgZones = m_DamageZoneMap.GetKeyArray();
			if (dmgZones && dmgZones.Count() > 0) {
				string dmgZone = dmgZones.GetRandomElement();
				if (m_DamageZoneMap.Get(dmgZone).Count() > 0) {
					return m_DamageZoneMap.Get(dmgZone).GetRandomElement();
				}
			}
		}

		return "";
	}

	float InediaInfectedAI_GetToPositionDirectionAngleX(vector position)
	{
		vector creatureHitDirection = vector.Direction(GetPosition(), position);
		return Math.DiffAngle(creatureHitDirection.VectorToAngles()[0], GetDirection().VectorToAngles()[0]);
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
			headPos = GetPosition() + 0.5 * vector.Up;
		}

		return headPos;
	}
}
