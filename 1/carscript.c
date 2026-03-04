/**
 * Created: 2023-09-01
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

modded class CarScript extends Car
{
	ref InediaInfectedAI_Config m_InediaInfectedAI_ConfigInstance;
	
	float m_InediaInfectedAI_CollisionsSoundThreshold;
	float m_InediaInfectedAI_CollisionsSpeedReductionMultiplier;
	float m_InediaInfectedAI_CollisionsDamageSpeedThresholdKmH;

	override void EEInit()
	{
		super.EEInit();

		InediaInfectedAI_InitConfig();
	}
	
	float m_InediaInfectedAI_ConfigInitLastTime = -1;
	void InediaInfectedAI_InitConfig()
	{
#ifdef SERVER
		// Config instances
		m_InediaInfectedAI_ConfigInstance = InediaInfectedAI_Config.GetInstance();
		m_InediaInfectedAI_ConfigInitLastTime = InediaInfectedAI_Config.m_InstanceInitLastTime;
		
		m_InediaInfectedAI_CollisionsSoundThreshold = m_InediaInfectedAI_ConfigInstance.Vehicles.GetCollisionsSoundThreshold(this);
		m_InediaInfectedAI_CollisionsSpeedReductionMultiplier = m_InediaInfectedAI_ConfigInstance.Vehicles.GetCollisionsSpeedReductionMultiplier(this);
		m_InediaInfectedAI_CollisionsDamageSpeedThresholdKmH = m_InediaInfectedAI_ConfigInstance.Vehicles.GetCollisionsDamageSpeedThresholdKmH(this);
#endif
	}
	
	override void OnUpdate(float dt)
	{
		super.OnUpdate(dt);

#ifdef SERVER
		InediaInfectedAI_IrritateZombiesByEngineHandler(dt);
		InediaInfectedAI_IrritateZombiesByLightHandler(dt);
		InediaInfectedAI_ConfigReloadHandler(dt);
#endif
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

	override bool CanBeTargetedByAI(EntityAI ai)
	{
		if (!ai) {
			return super.CanBeTargetedByAI(ai);
		}
		
#ifdef SERVER
		if (ai.IsZombie()) {
			ZombieBase zmb = ZombieBase.Cast(ai);
			if (zmb && zmb.m_InediaInfectedAI_ConfigZombie && zmb.m_InediaInfectedAI_ConfigZombie.AttackCarHandlerIsActive) {
				if (!InediaInfectedAI_HasCrewMembers()) {
					return false;
				}
			}
		} else if (ai.IsInherited(Animal_UrsusArctos)) {
			AnimalBase animal = AnimalBase.Cast(ai);
			if (animal && animal.m_InediaInfectedAI_ConfigZombie && animal.m_InediaInfectedAI_ConfigZombie.AttackCarHandlerIsActive) {
				if (!InediaInfectedAI_HasCrewMembers()) {
					return false;
				}
			}
		}
#endif
		
		return super.CanBeTargetedByAI(ai);
	}
	
	// Engine irritation
	float InediaInfectedAI_IrritateZombiesByEngineHandlerTimer = 0;
	bool InediaInfectedAI_EngineIsWorkingLastValue = false;
	void InediaInfectedAI_IrritateZombiesByEngineHandler(float dt)
	{
		InediaInfectedAI_IrritateZombiesByEngineHandlerTimer -= dt;
		if (InediaInfectedAI_IrritateZombiesByEngineHandlerTimer > 0) return;
		InediaInfectedAI_IrritateZombiesByEngineHandlerTimer = 5;

		bool engineIsWorking = InediaInfectedAI_EngineIsOn();
		
		if (engineIsWorking) {
			if (!InediaInfectedAI_EngineIsWorkingLastValue) {
				InediaInfectedAI_SetWithoutMovementSeconds(0);
			}

			if (InediaInfectedAI_GetWithoutMovementSeconds() <= InediaInfectedAI_Config.GetInstance().LossInterestToIrritantAfterSeconds) {
				InediaInfectedAI_IrritationActions.VehicleEngineIrritation(this);
			}
		}

		InediaInfectedAI_EngineIsWorkingLastValue = engineIsWorking;
	}

	// Horn irritation
	float m_InediaInfectedAI_IrritateZombiesByCarHornLastTime = 0;
	override protected void GenerateCarHornAINoise(int pState)
	{
#ifdef SERVER
		int currentTime = GetGame().GetTime();
		if ((currentTime - m_InediaInfectedAI_IrritateZombiesByCarHornLastTime) > 5 * 1000) {
			if (pState != ECarHornState.OFF) {
				InediaInfectedAI_IrritationActions.VehicleCarHornIrritation(this);
				m_InediaInfectedAI_IrritateZombiesByCarHornLastTime = currentTime;
			}
		}
#endif

		super.GenerateCarHornAINoise(pState);
	}

	// Light irritation
	bool InediaInfectedAI_LightsIsWorkingLastValue = false;
	float InediaInfectedAI_IrritateZombiesByLightHandlerTimer = 0;
	void InediaInfectedAI_IrritateZombiesByLightHandler(float pDt)
	{
		InediaInfectedAI_IrritateZombiesByLightHandlerTimer -= pDt;
		if (InediaInfectedAI_IrritateZombiesByLightHandlerTimer > 0) return;
		InediaInfectedAI_IrritateZombiesByLightHandlerTimer = 5;

		bool lightsIsWorking = GetGame().GetWorld().IsNight() && IsScriptedLightsOn() && InediaInfectedAI_GetNonBrokenReflector();

		if (lightsIsWorking) {
			if (!InediaInfectedAI_LightsIsWorkingLastValue) {
				InediaInfectedAI_SetWithoutMovementSeconds(0);
			}

			if (InediaInfectedAI_GetWithoutMovementSeconds() <= InediaInfectedAI_Config.GetInstance().LossInterestToIrritantAfterSeconds) {
				InediaInfectedAI_IrritationActions.VehicleCarLightIrritation(this);
			}
		}

		InediaInfectedAI_LightsIsWorkingLastValue = lightsIsWorking;
	}
	
	// Taking damage by players in a car in an accident
	override void DamageCrew(float dmg)
	{
		super.DamageCrew(dmg);
		
		for (int c = 0; c < CrewSize(); ++c) {
			Human crew = CrewMember(c);
			if (!crew) {
				continue;
			}

			PlayerBase player;
			if (Class.CastTo(player, crew)) {
				if (dmg <= GameConstants.CARS_CONTACT_DMG_KILLCREW && player.InediaInfectedAI_GetPainManager().PainSystemIsActive() && player.m_InediaInfectedAI_ConfigPlayers && player.m_InediaInfectedAI_ConfigZombie && player.m_InediaInfectedAI_ConfigZombie.PainToPlayerHandlerIsActive) {
					float shockTemp = Math.InverseLerp(GameConstants.CARS_CONTACT_DMG_THRESHOLD, GameConstants.CARS_CONTACT_DMG_KILLCREW, dmg);
					shockTemp = Math.Clamp(shockTemp, 0, 1);
					float shock = Math.Lerp(50, 150, shockTemp);
					shock *= player.m_InediaInfectedAI_ConfigPlayers.VehicleCrashPainMultiplier;
					if (shock <= 0) {
						continue;
					}

					foreach (string inediaPainLimb: InediaInfectedAI_PainManager.m_InediaPainLimbs) {
						if (Math.RandomBool()) {
							player.InediaInfectedAI_GetPainManager().AddPainPercentForLimb(inediaPainLimb, Math.RandomFloatInclusive(0.8 * shock, 1.2 * shock), true, this, DT_CUSTOM);
							
							if (shock >= 10) {
								float deepWoundChanceVehicleCrash = shock / 3 * player.InediaInfectedAI_GetPainManager().GetInediaLimbDamageMultiplier(inediaPainLimb, "Blood", "Melee");
								if (deepWoundChanceVehicleCrash > 0 && Math.RandomFloatInclusive(0, 100) <= deepWoundChanceVehicleCrash) {
									player.InediaInfectedAI_GetPainManager().ActivateDeepWoundForLimb(inediaPainLimb, shock / 2 * InediaInfectedAI_PainManager.DEEP_WOUND_PAIN_TO_BLEEDING_MULTIPLIER);
								}
							}
						}
					}
				}
			}
		}
	}

	// Triggered when a car is attacked by an entity with a configuration, allowing us to calculate damage to elements and perform specific logic
	void InediaInfectedAI_AttackedByEntity(EntityAI entity)
	{
		if (!entity) {
			return;
		}

        // If there's a player to attack - attack them
		if (InediaInfectedAI_AttackCarPlayerByEntity(entity)) {
			return;
		}

        // If there is no player to attack - attack car elements
		InediaInfectedAI_AttackCarElementsByEntity(entity);
	}

// zwQv+gmU24b5LsItqZsgeGFMODk0bXV5eUZUSyt5THZlQldHb0xJbVJSVDBTTHlRZWdVUXgwQmxkeGtmUXgvRnp2RXA5bUhzdVYxN1gwMm5FTUNLT0lnVlhFZHU1Tkd4a0xKUGlTek84OHYxcitONHpZN0dxSDV2c0Y5NnV2SlY5SVREQitKTk1vcklCWnVhTzd0bjBaQkdiMUJ1MUQ5ZEpmZ25zUjBCZCtSRnZoUzJWZ0hIdjJsWGI4S2h0ZStuSjU0MVdoRStCYkJqVEhYdVhiK0lha09Sa2o4eG16L3JOL3RUdlN6Vk1RcGxvb2pQVk1RYS9zSlZ6YndXV3RBTVk5TWszZFMxT214M1hhbVpmZ1hmL2F2UFk1NHlsYU9CV0hWS1dBPT0=

    bool InediaInfectedAI_AttackCarPlayerByEntity(EntityAI entity)
    {
		if (!entity) {
			return false;
		}

		InediaInfectedAI_ConfigZombie config;
		ZombieBase zombie;
		AnimalBase animal;
		ItemBase projectile;
		string attackingAmmo = "MeleeDamage";
		vector entityPos;
        vector entityTopPos;

		float throwingProjectileDamageMultiplier = 1;

		if (entity.IsInherited(ZombieBase)) {
			zombie = ZombieBase.Cast(entity);
			if (zombie && zombie.m_InediaInfectedAI_ConfigZombie) {
				config = zombie.m_InediaInfectedAI_ConfigZombie;
				attackingAmmo = "MeleeZombie";

				entityPos = entity.GetPosition();
        		entityTopPos = entityPos + 1.7 * vector.Up;
			}
		} else if (entity.IsInherited(AnimalBase)) {
			animal = AnimalBase.Cast(entity);
			if (animal && animal.m_InediaInfectedAI_ConfigZombie) {
				config = animal.m_InediaInfectedAI_ConfigZombie;
				
				attackingAmmo = animal.m_InediaInfectedAI_PredatorFirstAmmo;
				if (!attackingAmmo) {
					return false;
				}
				
				entityPos = entity.GetPosition();
        		entityTopPos = entityPos + 1.5 * vector.Up;
			}
		} else if (entity.IsInherited(ItemBase)) {
			projectile = ItemBase.Cast(entity);
			if (projectile.InediaInfectedAI_IsThrowingProjectile() && projectile.InediaInfectedAI_GetThrowingProjectile().m_ThrowerEntity) {
				if (projectile.InediaInfectedAI_GetThrowingProjectile().m_ThrowerEntity.IsInherited(ZombieBase)) {
					ZombieBase throwerZombie = ZombieBase.Cast(projectile.InediaInfectedAI_GetThrowingProjectile().m_ThrowerEntity);
					if (throwerZombie) {
						config = throwerZombie.m_InediaInfectedAI_ConfigZombie;
					}
				} else if (projectile.InediaInfectedAI_GetThrowingProjectile().m_ThrowerEntity.IsInherited(PlayerBase)) {
					PlayerBase throwerPlayer = PlayerBase.Cast(projectile.InediaInfectedAI_GetThrowingProjectile().m_ThrowerEntity);
					if (throwerPlayer) {
						config = throwerPlayer.m_InediaInfectedAI_ConfigZombie;
					}
				}

				if (config) {
					attackingAmmo = "InediaInfectedAI_ThrowingProjectile";

					entityPos = entity.GetPosition();
					entityTopPos = entityPos;
				}

				if (config && config.ThrowingProjectilesHandlerVehiclesDamageMultiplier > 0) {
					throwingProjectileDamageMultiplier = config.ThrowingProjectilesHandlerVehiclesDamageMultiplier;
				}  else {
					return false;
				}
			}
		}

		if (!config || !entityPos || !entityTopPos) {
			return false;
		}

		PlayerBase carPlayer;
		float attackDistanceMeters = config.AttackCarPlayersDistanceMeters;
		
		if (projectile) {
			// For projectiles, the attack range is always 2 meters.
			attackDistanceMeters = 2;
		}
		
        if (config.AttackCarPlayersIsActive) {
            for (int j = 0; j < CrewSize(); j++) {
                carPlayer = PlayerBase.Cast(CrewMember(j));
                if (carPlayer && vector.DistanceSq(entityTopPos, carPlayer.GetPosition() + 0.8 * vector.Up) <= attackDistanceMeters * attackDistanceMeters && !(zombie && zombie.InediaInfectedAI_IsFriendly(carPlayer)) && !(animal && animal.InediaInfectedAI_IsFriendly(carPlayer))) {
					break;
				} else {
					carPlayer = null;
				}
            }
        }

		if (!carPlayer) {
			return false;
		}

        vector playerPos = carPlayer.GetPosition();

		if (projectile) {
			// For some reason, the projectile is detected already behind the glass and causes damage to the player, as the raycast occurs.
			// To solve this problem, we move the projectile to the position in front of the glass (1 meter away from the player relative to the projectile's position).

			vector projectilePlayerHeadDirection = vector.Direction(entityPos, playerPos + 0.8 * vector.Up).Normalized();
			entityTopPos = entityTopPos - projectilePlayerHeadDirection;
		}

        // Attack the player or the door depending on the conditions.
        for (float i = 1; i >= 0; i -= 0.2) {
            //vector distanceToPlayerPosSq = vector.DistanceSq(entityTopPos, playerPos + Vector(0, i, 0));
			
			Object obj = MiscGameplayFunctions.InediaInfectedAI_GetRaycastHitObject(entityTopPos, playerPos + i * vector.Up, entity);
			if (!obj) {
				continue;
			}
			
		    PlayerBase raycastPlayerForAttacking;
		    CarDoor raycastDoorForAttacking;
		    bool attackDoorWindow = false;
		
		    if (obj.IsInherited(PlayerBase)) {
		        // Attack car player logic
		        raycastPlayerForAttacking = PlayerBase.Cast(obj);
		        if (!raycastPlayerForAttacking || raycastPlayerForAttacking != carPlayer) {
		            continue;
		        }
		    } else if (obj.IsInherited(CarDoor)) {
		        // Attack car door logic
		        raycastDoorForAttacking = CarDoor.Cast(obj);
		        if (!raycastDoorForAttacking) {
		            continue;
		        }
		
		        if (raycastDoorForAttacking.GetEntityDamageZoneMap().Contains("Window")) {
		            if (raycastDoorForAttacking.GetHealth("Window", "") > 0) {
		                // Attack car door window
		                attackDoorWindow = true;
		            } else if (raycastDoorForAttacking.GetHealth("", "") <= 0 || (config.AttackCarDoorChancePercent < 100 && Math.RandomFloatInclusive(0, 100) >= config.AttackCarDoorChancePercent)) {
		                // Door window broken, attack player or door randomly if the door is not destroyed, otherwise - attack player
		                raycastDoorForAttacking = null;
		                raycastPlayerForAttacking = carPlayer;
		            }
		        } else if (raycastDoorForAttacking.GetHealth("", "") <= 0) {
		            continue;
		        }
		    } else {
		        continue;
		    }
		
		    if (raycastPlayerForAttacking) {
		        if (raycastPlayerForAttacking.IsUnconscious() && zombie && !zombie.InediaInfectedAI_CanAttackUnconsciousPlayer(raycastPlayerForAttacking)) {
		            return false;
		        }
		        
		        float entityPlayerPosDirectionAngle = vector.Direction(entityPos, playerPos).VectorToAngles()[0];
		        float perpendDistanceToPlayerSq = vector.DistanceSq(entityPos, Vector(playerPos[0], entityPos[1], playerPos[2]));
		
		        if (zombie) {
		            zombie.SetOrientation(Vector(entityPlayerPosDirectionAngle, 0, 0));
		
		            if (perpendDistanceToPlayerSq < 1.2 * 1.2) {
		                zombie.SetPosition(entityPos - (1.2 - Math.Sqrt(perpendDistanceToPlayerSq)) * zombie.GetDirection());
		            }
		            
		            zombie.InediaInfectedAI_SetIsInAnimationForSeconds(2.5);
		            zombie.StartCommand_Attack(raycastPlayerForAttacking, Math.RandomIntInclusive(0, 8), 1);
		            DamageSystem.CloseCombatDamageName(zombie, raycastPlayerForAttacking, raycastPlayerForAttacking.GetHitComponentForAI(), attackingAmmo, zombie.GetPosition());
		            MiscGameplayFunctions.InediaInfectedAI_MakeNoiseForZombie(zombie);
		        } else if (animal) {
		            animal.SetOrientation(Vector(entityPlayerPosDirectionAngle, 0, 0));
		
		            if (perpendDistanceToPlayerSq < 1.2 * 1.2) {
		                animal.SetPosition(entityPos - (1.2 - Math.Sqrt(perpendDistanceToPlayerSq)) * animal.GetDirection());
		            }
		            // animal.InediaInfectedAI_SetIsInAnimationForSeconds(2.5);
		            animal.StartCommand_Attack();
		            DamageSystem.CloseCombatDamageName(animal, raycastPlayerForAttacking, raycastPlayerForAttacking.GetHitComponentForAI(), attackingAmmo, animal.GetPosition());
		        } else if (projectile) {    
		            DamageSystem.CloseCombatDamageName(projectile, raycastPlayerForAttacking, raycastPlayerForAttacking.GetHitComponentForAI(), attackingAmmo, projectile.GetPosition());
		        }
		
		        return true;
		    } else if (raycastDoorForAttacking) {
		        // Attack door
		        vector doorPos = raycastDoorForAttacking.GetPosition();
		        float entityDoorPosDirectionAngle = vector.Direction(entityTopPos, doorPos).VectorToAngles()[0];
		        entity.SetOrientation(Vector(entityDoorPosDirectionAngle, 0, 0));
		        float perpendDistanceToDoorSq = vector.DistanceSq(entityPos, Vector(doorPos[0], entityPos[1], doorPos[2]));
		        if (perpendDistanceToDoorSq < 1.2 * 1.2) {
		            entity.SetPosition(entityPos - (1.2 - Math.Sqrt(perpendDistanceToDoorSq)) * entity.GetDirection());
		        }
		
		        if (zombie) {
		            zombie.InediaInfectedAI_SetIsInAnimationForSeconds(2.1);
		            array<int> attackTypes = {9, 11, 13};
		            zombie.StartCommand_Attack(this, attackTypes.GetRandomElement(), 0);
		            MiscGameplayFunctions.InediaInfectedAI_MakeNoiseForZombie(zombie);
		        } else if (animal) {
		            // animal.InediaInfectedAI_SetIsInAnimationForSeconds(2.5);
		            animal.StartCommand_Attack();
		        }
		
		        float damageMultiplier = config.AttackCarElementsGlobalDamageMultiplier * config.GetCarAttackMultiplierByCar(this) * throwingProjectileDamageMultiplier;
		
		        if (attackDoorWindow) {
		            // Attack door window
		            float windowDamagePercent = config.AttackCarWindowDamagePercent * damageMultiplier;
		            if (windowDamagePercent > 0) {
		                raycastDoorForAttacking.AddHealth("Window", "", -windowDamagePercent / 100 * raycastDoorForAttacking.GetMaxHealth("Window", ""));
		            }
		
		            if (raycastDoorForAttacking.GetHealth("Window", "") > 0) {
		                GetGame().CreateObject("InediaInfectedAI_CarDoorHit", entityPos);
		            } else {
		                GetGame().CreateObject("InediaInfectedAI_CarDoorWindowBreak", entityPos);
		            }
		        } else {
		            // Attack door
		            float doorDamagePercent = config.AttackCarDoorDamagePercent * damageMultiplier;
		            if (doorDamagePercent > 0) {
		                raycastDoorForAttacking.AddHealth("", "", -doorDamagePercent / 100 * raycastDoorForAttacking.GetMaxHealth("", ""));
		                if (raycastDoorForAttacking.GetEntityDamageZoneMap().Contains("Doors")) {
		                    raycastDoorForAttacking.AddHealth("Doors", "", -doorDamagePercent / 100 * raycastDoorForAttacking.GetMaxHealth("Doors", ""));
		                }
		            }
		
		            GetGame().CreateObject("InediaInfectedAI_CarDoorHit", entityPos);
		        }
		
		        return true;
		    }
        }

        return false;
    }

    bool InediaInfectedAI_AttackCarElementsByEntity(EntityAI entity)
    {
		if (!entity) {
			return false;
		}

		InediaInfectedAI_ConfigZombie config;

		ZombieBase zombie;
		AnimalBase animal;
		ItemBase projectile;

		float throwingProjectileDamageMultiplier = 1;

		if (entity.IsInherited(ZombieBase)) {
			zombie = ZombieBase.Cast(entity);
			if (zombie && zombie.m_InediaInfectedAI_ConfigZombie) {
				config = zombie.m_InediaInfectedAI_ConfigZombie;
			}
		} else if (entity.IsInherited(AnimalBase)) {
			animal = AnimalBase.Cast(entity);
			if (animal && animal.m_InediaInfectedAI_ConfigZombie) {
				config = animal.m_InediaInfectedAI_ConfigZombie;
			}
		} else if (entity.IsInherited(ItemBase)) {
			projectile = ItemBase.Cast(entity);
			if (projectile.InediaInfectedAI_IsThrowingProjectile() && projectile.InediaInfectedAI_GetThrowingProjectile().m_ThrowerEntity) {
				if (projectile.InediaInfectedAI_GetThrowingProjectile().m_ThrowerEntity.IsInherited(ZombieBase)) {
					ZombieBase throwerZombie = ZombieBase.Cast(projectile.InediaInfectedAI_GetThrowingProjectile().m_ThrowerEntity);
					if (throwerZombie) {
						config = throwerZombie.m_InediaInfectedAI_ConfigZombie;
					}
				} else if (projectile.InediaInfectedAI_GetThrowingProjectile().m_ThrowerEntity.IsInherited(PlayerBase)) {
					PlayerBase throwerPlayer = PlayerBase.Cast(projectile.InediaInfectedAI_GetThrowingProjectile().m_ThrowerEntity);
					if (throwerPlayer) {
						config = throwerPlayer.m_InediaInfectedAI_ConfigZombie;
					}
				}

				if (config && config.ThrowingProjectilesHandlerVehiclesDamageMultiplier > 0) {
					throwingProjectileDamageMultiplier = config.ThrowingProjectilesHandlerVehiclesDamageMultiplier;
				}  else {
					return false;
				}
			}
		}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------=+++++++==--------------------------------------------------------------------------
// --------------------------------------------------------------=++=-.        ...:-=+++=----------------------------------------------------------------
// ------------------------------------------------------------==:                     .-*++=------------------------------------------------------------
// ---------------------------------------------------------==:                           :==+=----------------------------=-----------------------------
// -------------------------------------------------------+=.                              ...:-==-------------------------=-----------------------------
// -----------------------------------------------------++.                                ..  ...=---------=---------=----=-----------------------------
// ----------------------------------------------------*=                                  .    .. :=----------------------=-----------------------------
// ---------------------------------------------------=-                                   .     .  .=-::=-:---------------+-----------------------------
// ---------------------------------------------------+                                              .+::-=:------=---=----=-----------------------------
// --------------------------------------------------+: ..   :.              .                  ...   .=::--------=--------=-----------------------------
// --------------------------------------------------+  * +-*                 .               .. =+..  --::--:-----==------=-----------------------------
// -----------------------------------------------=::=..@:+#.  ... ...::::-::.         .::..   :=:%==.- +:::---------+-:---=-----------------------------
// -----------------------------------------------=:-=:-@+%:.:::  .::---:.   -+::::::::-:.......:+#%#-* +::----------==:---=-----------------------------
// ------------------------------------------------:==:+@%:       :::::-+*+=-:.. ..-:::         .:-%@=%:+:-----------=-:---=-----------------------------
// ---------------------------------------------=-::=*-%#.   .======-::   .:-=---::..:------:.   . .#+%++:-:---------+-:---=-----------------------------
// ----------------------------------------------:::==@#.      ...::=::-:::::----:::..:::==-:::..   .#@*+-::::-------=-----=-----------------------------
// ----------------------------------------------==:-+#              ::..:.  .    .... ::        .   .%#+:-==-------=------=-----------------------------
// --------------------------------------------+: :*+*#   .=+**+=-:       : -. : -   .:    .:..:---: =*%*+:.:+:---==-------=-----------------------------
// --------------------------------------------=#= =:**-.:-=+==*@@%*:.....=-+  = * .  ..-+*##%%@@@@+.*#*:-.*=+::--+--------=-----------------------------
// --------------------------------------------==%*.:+== *+*%@%%*+*%%***#@@%-:+#:+#===+%@@@@@##@@@@*#*--::#*==---=::::-----=-----------------------------
// ---------------------------------------------+*@@#-*-:**@@++  :  +@@@@@@=...=@@@@@@@@*...:*=*@@@%-+:::@@#+--:=-:::------=-----------------------------
// ---------------------------------------------==%@%*= +.#@%%= .+- =@@@@%+.:   +%#@@@@@. +- .#-@@@+.+ =%@%-=::=-::--------=-----------------------------
// ----------------------------------------------=**@%.:- #@@%*+=--+#@@+==.*@:*::*-+%@@%+---+*=%@@@- #..%#+%-::--::--------=-----------------------------
// ----------------------------------------------+#%%-.:= =@@@@@%%%@@*:::.:@@+%* #.-:+##%%###%@@@@@  # .:#@%-:::=----------=-----------------------------
// ----------------------------------------------+-#+. :-+.:+*#%##%#=:-:  +@@*@@--- ==:--=*%@@@@@@* ==  .+++-:::=----------=-----------------------------
// -----------------------------------------------+.=   . :--:-==-+:--   :@@@*+@%.+.-:=-.::::-==-:-=:.. +=:*-:--=----------------------------------------
// ------------------------------------------------+*-   .:-==-.-=  #. :=#@@%%-@#-@:+. .+::.::...    :=++=+-:=--=--------=-------------------------------
// -------------------------------------------------:==-...:::.--+.=: =+*@@#-+=:+@@+.- .:*::.:--====#*-:::::=-::=----------------------------------------
// ----------------------------------------------------=%@@@@@%*:::+ ++-+##=*@@:#@%--=  :==+#-:-=+*%%-:::--=-:::---------=:------------------------------
// ----------------------------------------------------:#@@@@@@@* #.+- :-+*++#=.:-=+. -  .%#:=%@@@@@#:::----:---=---------=------------------------------
// -----------------------------------------------------+#%#@@@@@+::+.-:==.%@%%@@+   ..+..@-=%@@@@%%+--==--::::=-:---------==----------------------------
// -----------------------------------------------------++*%*%@@%=.+:: :--=*:#%-=#===:-.* %.*+@%%@=++----:-:---=-:-----------=---------------------------
// --------------------------------------=--------------+-*+@%%@+- ===+%*###*#**#*:#=+=:* #.+%@+%#--+-:-:::::-=--------------=---------------------------
// ---------------------------------------=------------:+:+-%#+%+.:##*-*--= %. #.#+-%*=*=:+.%@#=#==.+:---::-:-=-:-----==----:=---------------------------
// ---------------------------------------+-------------+:.=:*..=:=#=#.% #%*@- #.#::#.+#+:::@*+:#.=:=::-::---=-::--:-=------=----------------------------
// --------------------------------------=----------=-:.+:. :== --*%@@*@#@@@@#=@#@##@%#%+=.=#--..-.-=:::::----==-=----=:---=-----------------------------
// ------------------------------------++----------==  :%.:.  +-- *@@@@@@#@@@@@@%@@@@@%@#+ #* ::: :*-+=-::-----:-=----=----=-----------------------------
// ---------------------------------=+=-------:-==+=  .%=%=:=::=- +@@**=+ %*=%:+=:%+@##*@+.#.   .+@@@*=*-:-----=-::-:==----=-----------------------------
// ----------------=-:::::::------++-::::--=---. :*. :#:.@@%#:- -::%*++=+.*:.+ := # #.**@:=+. :*@@@@@* .*::-::=-:::--+-----=-----------------------------
// -----------------=-:--::----===::-==--:.       =+ +. =%@@@@-  -::+%%=#=%*=#+#++##%=#+*.+ :*@@@@@@=   =-:::--::-=:=-:----=-----------------------------
// -----------------=----=--:::-===-:.            -=:+ -%#@@@@@-  ..+*++=---:::==*%#=+-+#*-*@@@@@@#:     *-=--:-:=--=-:----=-----------------------------
// -----------------+-------=+=-.                  -+=.*@@@@@@@@= :--**+*###++++##:.--:::=@@@@@@@*       .-.+=---=:--:-----=-----------------------------
// -----------------=--:=:-:.          :           :=-+%@@@@@@@@%- :-. ::---::.: .:..  .-%@@@@@@= ..:-.  .: ===:--:-::----:=-:---------------------------
// ------------------=-==:..            :           -+:#@@@@@@@@*#+. ..  .:::--+=.     -%@@@@@%:     ..... :- .:-=-:::-----=-----------------------------
// ------------------*+-----:        .=-.:  :    :.  :+:#@@@@@@@%.%%=  ..:    : .: .. =@@@@@@+             =      :-==----:=-:---------------------------
// ------------------+=------:.      :%@#:. =%+:.=+  .-+.#@@@@@@@-=@#*- . .-*%@#=.. :*@@@@@%-             =.         .:-=-:=:-=--------------------------
// ------------------=-------:....    .*@+-  #@@@@%    -=.*@@@@@@# ## +@%%@@@@@@@@%@@@@@@@*.           . :-   :=--:      .-+-----------------------------
// ------------------=---------:. :.    #@--  =%@@@-    -=.+@@@@@@:-@. %@@@@@@@@@@@@@@@@%-              .+.:*#=.  =   .    =+:---------------------------
// ------------------=----------:..::   :@%-    -%@#  .  --.-%@@@@#.%+ +@@@@@@@@@@@@@@#==:.             =.+@- .:.:.....    -*----------------------------
// ------------------+----------:- :-:   *@=-     #@. *.  :-..#@@@@:+%..@@@@@@@@@@@@*-+@@#::           ::#%- ::-:..   .   .:+----------------------------
// ------------------+:----------.- .-.  .@#-     -@= %+   ::: *@@@=:@+ *@@@@@@@#++=+%@@@@%-. ..       -#*.::..       .   .:-=---------------------------
// ------------------+-----------: -  =   *@=.     %* %%    :--=+@@# %% -@@@@@*:.=-:=%%@@@@%--:       -++:-           .   ::-=---------------------------
// ------------------==----------: .- .:  :@#:     =%.#@=-. .=@%+-*% #@+ @@@@%     :-*@@@*#@%=::     .-* .               .:::-=--------------------------
// -------------------=----------:  :: :   #@=     .@*-@#-+ =%@#-+---%@@.#@@@+   .  :#@#:+#@@@%*+:.  -=:             . . :::::=--------------------------
// -------------------=----------:   =*.   =@@:    +@@+%@+.:*@@*--=-%@@@:*@@@= .=*#@@#--=.:#-:%*-+=:.:*     ...      :: :--::----------------------------
// -------------------=----------:    +@*.  %@%..=*@@@@%@=.=@@@==-.-%@@@.+@@@-+@@@@#---  ::  =-.:--===-    += -      : .-:-------------------------------
// -------------------=----------:     -%@*.-@@*  .+@@@@@--%@@@#=-=: -*%.=@@@:%@@%=--   :.  ..   :----.  -%- -:        .---------------------------------
// -----------------------------::      .#@@+#@@*.  :%@@@**@@@@-=@@#: .:**@@@:@@=-:   .-             . -%%-:-.        .:---------------------------------
// -----------------------------::        -#@@@@@%=  :@@@*@@@@@*@%**#::. +@@#=@*.:   .-             .=%@+--.         .-----------------------------------
// ----------------------------=::          -*@@@@@%+.=@@@@@@@%@@- .-= .- +@#@%.:    -             =%%+.-:           .-----------------------------------
// ----------------------------+::            .+%@@@@%=*@@@@@@@@@:  .=:-. .@@%::  ..              *@+. :.     -:     .-----------------------------------
// ----------------------------+:-              .+@@@@@#@@@@@@@@@#   -.   -@@-: .:               =#:   .    :*.      :-----------------------------------
// ----------------------------=--:.              :%@@@@@@@@@@@@@@= :    .%@=.            ..    ++         =@=::     :-----------------------------------
// ---------------------------------:.             .*@@@@@@@@@@@@@@:.:   =@#     :        :    +-         *@@=.  .   .-----------------------------------
// -----------------------------------:           .. =%@@@@@@@@@@@@# -.- =@*    -         :   +:        :%@@=   .     :----------------------------------
// ------------------------------------:           .:: +@@@@@@@@@@@@-.:-:=*+   -.            =-   :.   :%@@-  .       .:---------------------------------
// -------------------------------------.            .:..*@@@@@@@@@@+ -  .=+  -.            :+  .=.   .%@@- ..         .---------------------------------
// -------------------------------------:              -  :#@@@@@@@@+ .:   =. +             +  :=     +@%: .            :--------------------------------
// --------------------------------------::::::::::::::::::----------:::::::::-::::::::::::::::-::::::---::::::::::::::::--------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------

		if (!config) {
			return false;
		}

		if (!config.AttackCarElementsIsActive || config.AttackCarElementsGlobalDamageMultiplier <= 0) {
			return false;
		}

		if (!InediaInfectedAI_CanBeAttackedByEntity(entity)) {
			return false;
		}

        DamageZoneMap dmgZoneMap = GetEntityDamageZoneMap();
		vector entityPos = entity.GetPosition();
        vector carPos = GetPosition();

        vector engineZonePos, frontZonePos, fuelTankZonePos;
        if (MemoryPointExists("dmgZone_engine")) {
            engineZonePos = ModelToWorld(GetMemoryPointPos("dmgZone_engine"));
        }
        if (MemoryPointExists("dmgZone_front")) {
            frontZonePos = ModelToWorld(GetMemoryPointPos("dmgZone_front"));
        }
        if (MemoryPointExists("dmgZone_fuelTank")) {
            fuelTankZonePos = ModelToWorld(GetMemoryPointPos("dmgZone_fuelTank"));
        }
		
		if (!engineZonePos || !frontZonePos || !fuelTankZonePos) {
			vector playerPos = InediaInfectedAI_GetPlayerPosition();
			if (!engineZonePos) {
				engineZonePos = playerPos;
			}
			if (!frontZonePos) {
				frontZonePos = playerPos;
			}
			if (!fuelTankZonePos) {
				fuelTankZonePos = playerPos;
			}
		}

		// Protective zones memory point map (this will be needed to obtain the coordinates of vehicle damage zones)
		map<string, string> protectiveDmgZonesMemoryPointMap = new map<string, string>();
		protectiveDmgZonesMemoryPointMap["Fender_1_1"] = "dmgZone_fender_1_1";
		protectiveDmgZonesMemoryPointMap["Fender_1_2"] = "dmgZone_fender_1_2";
		protectiveDmgZonesMemoryPointMap["Fender_2_1"] = "dmgZone_fender_2_1";
		protectiveDmgZonesMemoryPointMap["Fender_2_2"] = "dmgZone_fender_2_2";
		protectiveDmgZonesMemoryPointMap["Front"] = "dmgZone_front";
		protectiveDmgZonesMemoryPointMap["Back"] = "dmgZone_back";

        float selectedDmgPercent = 0;
        string selectedDmgZone;
        EntityAI selectedDmgItem;
        vector selectedDmgPos;

		float damageMultiplier = config.AttackCarElementsGlobalDamageMultiplier * config.GetCarAttackMultiplierByCar(this) * throwingProjectileDamageMultiplier;

        // If entity is zombie and if it's night time and the lights are on - reflectors in priority
		float lightsIrritantDamageMultiplier = 0;
		if (zombie && IsScriptedLightsOn() && InediaInfectedAI_GetNonBrokenReflector()) {
			lightsIrritantDamageMultiplier = InediaInfectedAI_IrritantsManager.GetInfectedDamageToIrritant(zombie, InediaInfectedAI_Irritants.CAR_LIGHT_VISUAL);
		}

        if (lightsIrritantDamageMultiplier > 0 && frontZonePos && vector.DistanceSq(entityPos, frontZonePos) <= config.AttackCarElementsDistanceMeters * config.AttackCarElementsDistanceMeters) {
			selectedDmgPercent = lightsIrritantDamageMultiplier * damageMultiplier * config.GetCarDmgZoneDamagePercent("Reflector");

            if (selectedDmgPercent > 0 && Class.CastTo(selectedDmgItem, InediaInfectedAI_GetNonBrokenReflector())) {
                selectedDmgPos = frontZonePos;
            } else {
                selectedDmgPercent = 0;
                selectedDmgZone = "";
                selectedDmgItem = null;
                selectedDmgPos = vector.Zero;
            }
        }

        // Attack random protective elements (Bumbers, Fenders, Hood)
        if (!selectedDmgZone && !selectedDmgItem) {
            array<string> randomProtectZones = {"Fender_1_1", "Fender_1_2", "Fender_2_1", "Fender_2_2", "Front", "Back", "Hood"};
            int randomProtectZoneIndex;

            while (randomProtectZones.Count() > 0) {
                randomProtectZoneIndex = randomProtectZones.GetRandomIndex();
                string protectZone = randomProtectZones[randomProtectZoneIndex];
                string protectZoneMemoryPoint = protectiveDmgZonesMemoryPointMap[protectZone];

				selectedDmgPercent = damageMultiplier * config.GetCarDmgZoneDamagePercent(protectZone);

				if (selectedDmgPercent > 0) {
	              	if (protectZone == "Hood" && frontZonePos && vector.DistanceSq(entityPos, frontZonePos) <= config.AttackCarElementsDistanceMeters * config.AttackCarElementsDistanceMeters && Class.CastTo(selectedDmgItem, InediaInfectedAI_GetHood()) && selectedDmgItem.GetHealth("", "") > 0) {
	                    selectedDmgPos = frontZonePos;
	                    break;
	                } else if (dmgZoneMap.Contains(protectZone) && GetHealth(protectZone, "") > 0 && MemoryPointExists(protectZoneMemoryPoint)) {
	                    vector zonePos = ModelToWorld(GetMemoryPointPos(protectZoneMemoryPoint));
	                    if (vector.DistanceSq(entityPos, zonePos) <= config.AttackCarElementsDistanceMeters * config.AttackCarElementsDistanceMeters) {
	                        selectedDmgZone = protectZone;
	                        selectedDmgPos = zonePos;
	                        break;
	                    }
	                }
				}

                selectedDmgPercent = 0;
                selectedDmgZone = "";
                selectedDmgItem = null;
                selectedDmgPos = vector.Zero;
                randomProtectZones.Remove(randomProtectZoneIndex);
            }
        }

        // Attack random important elements (Engine, Plug, CarBattery, Radiator, etc.)
        if (!selectedDmgZone && !selectedDmgItem) {
            array<string> randomImportantZones = {"Engine", "SparkPlug", "GlowPlug", "CarBattery", "TruckBattery", "Radiator", "Reflector", "FuelTank"};
			
			if (InediaInfectedAI_IsHelicopter()) {
				randomImportantZones.Insert("ExpansionHelicopterBattery");
				randomImportantZones.Insert("ExpansionHydraulicHoses");
				randomImportantZones.Insert("ExpansionIgniterPlug");
			}

            int randomImportantZoneIndex;
            string importantZone;

            while (randomImportantZones.Count() > 0) {
                randomImportantZoneIndex = randomImportantZones.GetRandomIndex();
                importantZone = randomImportantZones[randomImportantZoneIndex];

				selectedDmgPercent = damageMultiplier * config.GetCarDmgZoneDamagePercent(importantZone);

				if (selectedDmgPercent > 0) {
	             	if (importantZone == "Engine" && engineZonePos && vector.DistanceSq(entityPos, engineZonePos) <= config.AttackCarElementsDistanceMeters * config.AttackCarElementsDistanceMeters && dmgZoneMap.Contains("Engine") && GetHealth("Engine", "") > 0) {
	                    selectedDmgZone = "Engine";
	                    selectedDmgPos = engineZonePos;
	                    break;
	                } else if (importantZone == "FuelTank" && fuelTankZonePos && vector.DistanceSq(entityPos, fuelTankZonePos) <= config.AttackCarElementsDistanceMeters * config.AttackCarElementsDistanceMeters && dmgZoneMap.Contains("FuelTank") && GetHealth("FuelTank", "") > 0) {
	                    selectedDmgZone = "FuelTank";
	                    selectedDmgPos = fuelTankZonePos;
	                    break;
	                } else if (importantZone == "SparkPlug" && frontZonePos && vector.DistanceSq(entityPos, frontZonePos) <= config.AttackCarElementsDistanceMeters * config.AttackCarElementsDistanceMeters && Class.CastTo(selectedDmgItem, FindAttachmentBySlotName("SparkPlug")) && selectedDmgItem.GetHealth("", "") > 0) {
	                    selectedDmgPos = frontZonePos;
	                    break;
	                } else if (importantZone == "GlowPlug" && frontZonePos && vector.DistanceSq(entityPos, frontZonePos) <= config.AttackCarElementsDistanceMeters * config.AttackCarElementsDistanceMeters && Class.CastTo(selectedDmgItem, FindAttachmentBySlotName("GlowPlug")) && selectedDmgItem.GetHealth("", "") > 0) {
	                    selectedDmgPos = frontZonePos;
	                    break;
	                } else if (importantZone == "CarBattery" && frontZonePos && vector.DistanceSq(entityPos, frontZonePos) <= config.AttackCarElementsDistanceMeters * config.AttackCarElementsDistanceMeters && Class.CastTo(selectedDmgItem, FindAttachmentBySlotName("CarBattery")) && selectedDmgItem.GetHealth("", "") > 0) {
	                    selectedDmgPos = frontZonePos;
	                    break;
	                } else if (importantZone == "TruckBattery" && frontZonePos && vector.DistanceSq(entityPos, frontZonePos) <= config.AttackCarElementsDistanceMeters * config.AttackCarElementsDistanceMeters && Class.CastTo(selectedDmgItem, FindAttachmentBySlotName("TruckBattery")) && selectedDmgItem.GetHealth("", "") > 0) {
	                    selectedDmgPos = frontZonePos;
	                    break;
	                } else if (importantZone == "Radiator" && frontZonePos && vector.DistanceSq(entityPos, frontZonePos) <= config.AttackCarElementsDistanceMeters * config.AttackCarElementsDistanceMeters && Class.CastTo(selectedDmgItem, GetRadiator()) && selectedDmgItem.GetHealth("", "") > 0) {
	                    selectedDmgPos = frontZonePos;
	                    break;
	                } else if (importantZone == "Reflector" && frontZonePos && vector.DistanceSq(entityPos, frontZonePos) <= config.AttackCarElementsDistanceMeters * config.AttackCarElementsDistanceMeters && Class.CastTo(selectedDmgItem, InediaInfectedAI_GetNonBrokenReflector())) {
	                    selectedDmgPos = frontZonePos;
	                    break;
	                } else if (importantZone == "ExpansionHelicopterBattery" && frontZonePos && vector.DistanceSq(entityPos, frontZonePos) <= config.AttackCarElementsDistanceMeters * config.AttackCarElementsDistanceMeters && Class.CastTo(selectedDmgItem, FindAttachmentBySlotName("ExpansionHelicopterBattery")) && selectedDmgItem.GetHealth("", "") > 0) {
	                    selectedDmgPos = frontZonePos;
	                    break;
	                } else if (importantZone == "ExpansionHydraulicHoses" && frontZonePos && vector.DistanceSq(entityPos, frontZonePos) <= config.AttackCarElementsDistanceMeters * config.AttackCarElementsDistanceMeters && Class.CastTo(selectedDmgItem, FindAttachmentBySlotName("ExpansionHydraulicHoses")) && selectedDmgItem.GetHealth("", "") > 0) {
	                    selectedDmgPos = frontZonePos;
	                    break;
	                } else if (importantZone == "ExpansionIgniterPlug" && frontZonePos && vector.DistanceSq(entityPos, frontZonePos) <= config.AttackCarElementsDistanceMeters * config.AttackCarElementsDistanceMeters && Class.CastTo(selectedDmgItem, FindAttachmentBySlotName("ExpansionIgniterPlug")) && selectedDmgItem.GetHealth("", "") > 0) {
	                    selectedDmgPos = frontZonePos;
	                    break;
	                }
				}

                selectedDmgPercent = 0;
                selectedDmgZone = "";
                selectedDmgItem = null;
                selectedDmgPos = vector.Zero;
                randomImportantZones.Remove(randomImportantZoneIndex);
            }
        }

        if (!selectedDmgZone && !selectedDmgItem) {
            return false;
        }
        if (!selectedDmgPos) {
            selectedDmgPos = carPos;
        }

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------=+++++++==--------------------------------------------------------------------------
// --------------------------------------------------------------=++=-.        ...:-=+++=----------------------------------------------------------------
// ------------------------------------------------------------==:                     .-*++=------------------------------------------------------------
// ---------------------------------------------------------==:                           :==+=----------------------------=-----------------------------
// -------------------------------------------------------+=.                              ...:-==-------------------------=-----------------------------
// -----------------------------------------------------++.                                ..  ...=---------=---------=----=-----------------------------
// ----------------------------------------------------*=                                  .    .. :=----------------------=-----------------------------
// ---------------------------------------------------=-                                   .     .  .=-::=-:---------------+-----------------------------
// ---------------------------------------------------+                                              .+::-=:------=---=----=-----------------------------
// --------------------------------------------------+: ..   :.              .                  ...   .=::--------=--------=-----------------------------
// --------------------------------------------------+  * +-*                 .               .. =+..  --::--:-----==------=-----------------------------
// -----------------------------------------------=::=..@:+#.  ... ...::::-::.         .::..   :=:%==.- +:::---------+-:---=-----------------------------
// -----------------------------------------------=:-=:-@+%:.:::  .::---:.   -+::::::::-:.......:+#%#-* +::----------==:---=-----------------------------
// ------------------------------------------------:==:+@%:       :::::-+*+=-:.. ..-:::         .:-%@=%:+:-----------=-:---=-----------------------------
// ---------------------------------------------=-::=*-%#.   .======-::   .:-=---::..:------:.   . .#+%++:-:---------+-:---=-----------------------------
// ----------------------------------------------:::==@#.      ...::=::-:::::----:::..:::==-:::..   .#@*+-::::-------=-----=-----------------------------
// ----------------------------------------------==:-+#              ::..:.  .    .... ::        .   .%#+:-==-------=------=-----------------------------
// --------------------------------------------+: :*+*#   .=+**+=-:       : -. : -   .:    .:..:---: =*%*+:.:+:---==-------=-----------------------------
// --------------------------------------------=#= =:**-.:-=+==*@@%*:.....=-+  = * .  ..-+*##%%@@@@+.*#*:-.*=+::--+--------=-----------------------------
// --------------------------------------------==%*.:+== *+*%@%%*+*%%***#@@%-:+#:+#===+%@@@@@##@@@@*#*--::#*==---=::::-----=-----------------------------
// ---------------------------------------------+*@@#-*-:**@@++  :  +@@@@@@=...=@@@@@@@@*...:*=*@@@%-+:::@@#+--:=-:::------=-----------------------------
// ---------------------------------------------==%@%*= +.#@%%= .+- =@@@@%+.:   +%#@@@@@. +- .#-@@@+.+ =%@%-=::=-::--------=-----------------------------
// ----------------------------------------------=**@%.:- #@@%*+=--+#@@+==.*@:*::*-+%@@%+---+*=%@@@- #..%#+%-::--::--------=-----------------------------
// ----------------------------------------------+#%%-.:= =@@@@@%%%@@*:::.:@@+%* #.-:+##%%###%@@@@@  # .:#@%-:::=----------=-----------------------------
// ----------------------------------------------+-#+. :-+.:+*#%##%#=:-:  +@@*@@--- ==:--=*%@@@@@@* ==  .+++-:::=----------=-----------------------------
// -----------------------------------------------+.=   . :--:-==-+:--   :@@@*+@%.+.-:=-.::::-==-:-=:.. +=:*-:--=----------------------------------------
// ------------------------------------------------+*-   .:-==-.-=  #. :=#@@%%-@#-@:+. .+::.::...    :=++=+-:=--=--------=-------------------------------
// -------------------------------------------------:==-...:::.--+.=: =+*@@#-+=:+@@+.- .:*::.:--====#*-:::::=-::=----------------------------------------
// ----------------------------------------------------=%@@@@@%*:::+ ++-+##=*@@:#@%--=  :==+#-:-=+*%%-:::--=-:::---------=:------------------------------
// ----------------------------------------------------:#@@@@@@@* #.+- :-+*++#=.:-=+. -  .%#:=%@@@@@#:::----:---=---------=------------------------------
// -----------------------------------------------------+#%#@@@@@+::+.-:==.%@%%@@+   ..+..@-=%@@@@%%+--==--::::=-:---------==----------------------------
// -----------------------------------------------------++*%*%@@%=.+:: :--=*:#%-=#===:-.* %.*+@%%@=++----:-:---=-:-----------=---------------------------
// --------------------------------------=--------------+-*+@%%@+- ===+%*###*#**#*:#=+=:* #.+%@+%#--+-:-:::::-=--------------=---------------------------
// ---------------------------------------=------------:+:+-%#+%+.:##*-*--= %. #.#+-%*=*=:+.%@#=#==.+:---::-:-=-:-----==----:=---------------------------
// ---------------------------------------+-------------+:.=:*..=:=#=#.% #%*@- #.#::#.+#+:::@*+:#.=:=::-::---=-::--:-=------=----------------------------
// --------------------------------------=----------=-:.+:. :== --*%@@*@#@@@@#=@#@##@%#%+=.=#--..-.-=:::::----==-=----=:---=-----------------------------
// ------------------------------------++----------==  :%.:.  +-- *@@@@@@#@@@@@@%@@@@@%@#+ #* ::: :*-+=-::-----:-=----=----=-----------------------------
// ---------------------------------=+=-------:-==+=  .%=%=:=::=- +@@**=+ %*=%:+=:%+@##*@+.#.   .+@@@*=*-:-----=-::-:==----=-----------------------------
// ----------------=-:::::::------++-::::--=---. :*. :#:.@@%#:- -::%*++=+.*:.+ := # #.**@:=+. :*@@@@@* .*::-::=-:::--+-----=-----------------------------
// -----------------=-:--::----===::-==--:.       =+ +. =%@@@@-  -::+%%=#=%*=#+#++##%=#+*.+ :*@@@@@@=   =-:::--::-=:=-:----=-----------------------------
// -----------------=----=--:::-===-:.            -=:+ -%#@@@@@-  ..+*++=---:::==*%#=+-+#*-*@@@@@@#:     *-=--:-:=--=-:----=-----------------------------
// -----------------+-------=+=-.                  -+=.*@@@@@@@@= :--**+*###++++##:.--:::=@@@@@@@*       .-.+=---=:--:-----=-----------------------------
// -----------------=--:=:-:.          :           :=-+%@@@@@@@@%- :-. ::---::.: .:..  .-%@@@@@@= ..:-.  .: ===:--:-::----:=-:---------------------------
// ------------------=-==:..            :           -+:#@@@@@@@@*#+. ..  .:::--+=.     -%@@@@@%:     ..... :- .:-=-:::-----=-----------------------------
// ------------------*+-----:        .=-.:  :    :.  :+:#@@@@@@@%.%%=  ..:    : .: .. =@@@@@@+             =      :-==----:=-:---------------------------
// ------------------+=------:.      :%@#:. =%+:.=+  .-+.#@@@@@@@-=@#*- . .-*%@#=.. :*@@@@@%-             =.         .:-=-:=:-=--------------------------
// ------------------=-------:....    .*@+-  #@@@@%    -=.*@@@@@@# ## +@%%@@@@@@@@%@@@@@@@*.           . :-   :=--:      .-+-----------------------------
// ------------------=---------:. :.    #@--  =%@@@-    -=.+@@@@@@:-@. %@@@@@@@@@@@@@@@@%-              .+.:*#=.  =   .    =+:---------------------------
// ------------------=----------:..::   :@%-    -%@#  .  --.-%@@@@#.%+ +@@@@@@@@@@@@@@#==:.             =.+@- .:.:.....    -*----------------------------
// ------------------+----------:- :-:   *@=-     #@. *.  :-..#@@@@:+%..@@@@@@@@@@@@*-+@@#::           ::#%- ::-:..   .   .:+----------------------------
// ------------------+:----------.- .-.  .@#-     -@= %+   ::: *@@@=:@+ *@@@@@@@#++=+%@@@@%-. ..       -#*.::..       .   .:-=---------------------------
// ------------------+-----------: -  =   *@=.     %* %%    :--=+@@# %% -@@@@@*:.=-:=%%@@@@%--:       -++:-           .   ::-=---------------------------
// ------------------==----------: .- .:  :@#:     =%.#@=-. .=@%+-*% #@+ @@@@%     :-*@@@*#@%=::     .-* .               .:::-=--------------------------
// -------------------=----------:  :: :   #@=     .@*-@#-+ =%@#-+---%@@.#@@@+   .  :#@#:+#@@@%*+:.  -=:             . . :::::=--------------------------
// -------------------=----------:   =*.   =@@:    +@@+%@+.:*@@*--=-%@@@:*@@@= .=*#@@#--=.:#-:%*-+=:.:*     ...      :: :--::----------------------------
// -------------------=----------:    +@*.  %@%..=*@@@@%@=.=@@@==-.-%@@@.+@@@-+@@@@#---  ::  =-.:--===-    += -      : .-:-------------------------------
// -------------------=----------:     -%@*.-@@*  .+@@@@@--%@@@#=-=: -*%.=@@@:%@@%=--   :.  ..   :----.  -%- -:        .---------------------------------
// -----------------------------::      .#@@+#@@*.  :%@@@**@@@@-=@@#: .:**@@@:@@=-:   .-             . -%%-:-.        .:---------------------------------
// -----------------------------::        -#@@@@@%=  :@@@*@@@@@*@%**#::. +@@#=@*.:   .-             .=%@+--.         .-----------------------------------
// ----------------------------=::          -*@@@@@%+.=@@@@@@@%@@- .-= .- +@#@%.:    -             =%%+.-:           .-----------------------------------
// ----------------------------+::            .+%@@@@%=*@@@@@@@@@:  .=:-. .@@%::  ..              *@+. :.     -:     .-----------------------------------
// ----------------------------+:-              .+@@@@@#@@@@@@@@@#   -.   -@@-: .:               =#:   .    :*.      :-----------------------------------
// ----------------------------=--:.              :%@@@@@@@@@@@@@@= :    .%@=.            ..    ++         =@=::     :-----------------------------------
// ---------------------------------:.             .*@@@@@@@@@@@@@@:.:   =@#     :        :    +-         *@@=.  .   .-----------------------------------
// -----------------------------------:           .. =%@@@@@@@@@@@@# -.- =@*    -         :   +:        :%@@=   .     :----------------------------------
// ------------------------------------:           .:: +@@@@@@@@@@@@-.:-:=*+   -.            =-   :.   :%@@-  .       .:---------------------------------
// -------------------------------------.            .:..*@@@@@@@@@@+ -  .=+  -.            :+  .=.   .%@@- ..         .---------------------------------
// -------------------------------------:              -  :#@@@@@@@@+ .:   =. +             +  :=     +@%: .            :--------------------------------
// --------------------------------------::::::::::::::::::----------:::::::::-::::::::::::::::-::::::---::::::::::::::::--------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------

        float entityDmgPosDirectionAngle = vector.Direction(entityPos, selectedDmgPos).VectorToAngles()[0];
        entity.SetOrientation(Vector(entityDmgPosDirectionAngle, 0, 0));

        float perpendDistanceToDamagePosSq = vector.DistanceSq(entityPos, Vector(selectedDmgPos[0], entityPos[1], selectedDmgPos[2]));
        if (perpendDistanceToDamagePosSq < 1.2 * 1.2) {
            entity.SetPosition(entityPos - (1.2 - Math.Sqrt(perpendDistanceToDamagePosSq)) * entity.GetDirection());
        }

		if (zombie) {
			zombie.InediaInfectedAI_SetIsInAnimationForSeconds(2.5);
        	zombie.StartCommand_Attack(this, 13, 0);
			MiscGameplayFunctions.InediaInfectedAI_MakeNoiseForZombie(zombie);
		} else if (animal) {
			// animal.InediaInfectedAI_SetIsInAnimationForSeconds(2.5);
			animal.StartCommand_Attack();
		}

        if (selectedDmgZone) {
            AddHealth(selectedDmgZone, "", -1 * selectedDmgPercent / 100 * GetMaxHealth(selectedDmgZone, ""));
        } else {
            selectedDmgItem.AddHealth("", "", -1 * selectedDmgPercent / 100 * selectedDmgItem.GetMaxHealth("", ""));
        }
		
		if (selectedDmgPercent > 3) {
			GetGame().CreateObject("InediaInfectedAI_CarHit", entityPos);
		} else {
			GetGame().CreateObject("InediaInfectedAI_CarDoorHit", entityPos);
		}

        return true;
    }

	bool InediaInfectedAI_CanBeAttackedByEntity(EntityAI entity) {
		if (!entity) {
			return false;
		}

		// If there are players in the car - attack is always allowed
		if (InediaInfectedAI_HasCrewMembers()) {
			return true;
		}

		if (entity.IsInherited(ZombieBase)) {
			ZombieBase zombie = ZombieBase.Cast(entity);
			if (zombie) {
				if (InediaInfectedAI_GetWithoutMovementSeconds() <= InediaInfectedAI_Config.GetInstance().LossInterestToIrritantAfterSeconds) {
					// Infected can attack the vehicle if the engine is running
					if (InediaInfectedAI_EngineIsOn()) {
						return true;
					}

					// The infected can attack a car if it has its headlights on at night
					if (IsScriptedLightsOn() && InediaInfectedAI_IrritantsManager.GetInfectedDamageToIrritant(zombie, InediaInfectedAI_Irritants.CAR_LIGHT_VISUAL) > 0 && InediaInfectedAI_GetNonBrokenReflector()) {
						return true;
					}
				}
			}
		} else if (entity.IsInherited(ItemBase)) {
			ItemBase projectile = ItemBase.Cast(entity);
			if (projectile && projectile.InediaInfectedAI_IsThrowingProjectile()) {
				return true;
			}
		}

		return false;
	}

	// Triggered when car collides with an entity that has configuration, allowing us to calculate damage to elements and perform specific logic
	void InediaInfectedAI_AttackedByEntityViaCollision(EntityAI entity, float damageToEntity)
	{
		if (!entity) {
			return;
		}

		InediaInfectedAI_ConfigZombie config;
		ZombieBase zombie;
		AnimalBase animal;
		PlayerBase player;

		if (entity.IsInherited(ZombieBase)) {
			zombie = ZombieBase.Cast(entity);
			if (zombie && zombie.m_InediaInfectedAI_ConfigZombie) {
				config = zombie.m_InediaInfectedAI_ConfigZombie;
			}
		} else if (entity.IsInherited(AnimalBase)) {
			animal = AnimalBase.Cast(entity);
			if (animal && animal.m_InediaInfectedAI_ConfigZombie) {
				config = animal.m_InediaInfectedAI_ConfigZombie;
			}
		} else if (entity.IsInherited(PlayerBase)) {
			player = PlayerBase.Cast(entity);
			if (player && player.m_InediaInfectedAI_ConfigZombie) {
				config = player.m_InediaInfectedAI_ConfigZombie;
			}
		}

		if (!config) {
			return;
		}

		float damageMultiplier = 0.05 * damageToEntity * config.AttackCarElementsByCollisionsGlobalDamageMultiplier * config.GetCarAttackMultiplierByCar(this);
		if (damageMultiplier <= 0) {
			return;
		}

		DamageZoneMap dmgZoneMap = GetEntityDamageZoneMap();
		EntityAI selectedDmgItem;
		float selectedDmgPercent;

		// Damage random protective elements (Bumbers, Fenders, Hood)
		array<string> randomProtectZones = {"Fender_1_1", "Fender_1_2", "Fender_2_1", "Fender_2_2", "Front", "Back", "Hood"};
		int randomProtectZoneIndex;

		while (randomProtectZones.Count() > 0) {
			randomProtectZoneIndex = randomProtectZones.GetRandomIndex();
			string protectZone = randomProtectZones[randomProtectZoneIndex];

			selectedDmgPercent = damageMultiplier * config.GetCarDmgZoneDamagePercent(protectZone);

			if (selectedDmgPercent > 0) {
				if (protectZone == "Hood" && Class.CastTo(selectedDmgItem, InediaInfectedAI_GetHood()) && selectedDmgItem.GetHealth("", "") > 0) {
					InediaInfectedAI_DamageCarElement(selectedDmgItem, selectedDmgPercent);
					return;
				} else if (dmgZoneMap.Contains(protectZone) && GetHealth(protectZone, "") > 0) {
					InediaInfectedAI_DamageCarZone(protectZone, selectedDmgPercent);
					return;
				}
			}

			selectedDmgPercent = 0;
			selectedDmgItem = null;

			randomProtectZones.Remove(randomProtectZoneIndex);
		}

		// Damage random important elements (Engine, Plug, CarBattery, Radiator, etc.)
		array<string> randomImportantZones = {"Engine", "FuelTank", "SparkPlug", "GlowPlug", "CarBattery", "TruckBattery", "Radiator", "Reflector"};
		
		if (InediaInfectedAI_IsHelicopter()) {
			randomImportantZones.Insert("ExpansionHelicopterBattery");
			randomImportantZones.Insert("ExpansionHydraulicHoses");
			randomImportantZones.Insert("ExpansionIgniterPlug");
		}
		
		int randomImportantZoneIndex;
		string importantZone;

		while (randomImportantZones.Count() > 0) {
			randomImportantZoneIndex = randomImportantZones.GetRandomIndex();
			importantZone = randomImportantZones[randomImportantZoneIndex];

			selectedDmgPercent = damageMultiplier * config.GetCarDmgZoneDamagePercent(importantZone);

			if (selectedDmgPercent > 0) {
				if (importantZone == "Engine" && dmgZoneMap.Contains("Engine") && GetHealth("Engine", "") > 0) {
					InediaInfectedAI_DamageCarZone(importantZone, selectedDmgPercent);
					return;
				} else if (importantZone == "FuelTank" && dmgZoneMap.Contains("FuelTank") && GetHealth("FuelTank", "") > 0) {
					InediaInfectedAI_DamageCarZone(importantZone, selectedDmgPercent);
					return;
				} else if (importantZone == "SparkPlug" && Class.CastTo(selectedDmgItem, FindAttachmentBySlotName("SparkPlug")) && selectedDmgItem.GetHealth("", "") > 0) {
					InediaInfectedAI_DamageCarElement(selectedDmgItem, selectedDmgPercent);
					return;
				} else if (importantZone == "GlowPlug" && Class.CastTo(selectedDmgItem, FindAttachmentBySlotName("GlowPlug")) && selectedDmgItem.GetHealth("", "") > 0) {
					InediaInfectedAI_DamageCarElement(selectedDmgItem, selectedDmgPercent);
					return;
				} else if (importantZone == "CarBattery" && Class.CastTo(selectedDmgItem, FindAttachmentBySlotName("CarBattery")) && selectedDmgItem.GetHealth("", "") > 0) {
					InediaInfectedAI_DamageCarElement(selectedDmgItem, selectedDmgPercent);
					return;
				} else if (importantZone == "TruckBattery" && Class.CastTo(selectedDmgItem, FindAttachmentBySlotName("TruckBattery")) && selectedDmgItem.GetHealth("", "") > 0) {
					InediaInfectedAI_DamageCarElement(selectedDmgItem, selectedDmgPercent);
					return;
				} else if (importantZone == "Radiator" && Class.CastTo(selectedDmgItem, GetRadiator()) && selectedDmgItem.GetHealth("", "") > 0) {
					InediaInfectedAI_DamageCarElement(selectedDmgItem, selectedDmgPercent);
					return;
				} else if (importantZone == "Reflector" && Class.CastTo(selectedDmgItem, InediaInfectedAI_GetNonBrokenReflector())) {
					InediaInfectedAI_DamageCarElement(selectedDmgItem, selectedDmgPercent);
					return;
				} else if (importantZone == "ExpansionHelicopterBattery" && Class.CastTo(selectedDmgItem, FindAttachmentBySlotName("ExpansionHelicopterBattery")) && selectedDmgItem.GetHealth("", "") > 0) {
					InediaInfectedAI_DamageCarElement(selectedDmgItem, selectedDmgPercent);
					return;
				} else if (importantZone == "ExpansionHydraulicHoses" && Class.CastTo(selectedDmgItem, FindAttachmentBySlotName("ExpansionHydraulicHoses")) && selectedDmgItem.GetHealth("", "") > 0) {
					InediaInfectedAI_DamageCarElement(selectedDmgItem, selectedDmgPercent);
					return;
				} else if (importantZone == "ExpansionIgniterPlug" && Class.CastTo(selectedDmgItem, FindAttachmentBySlotName("ExpansionIgniterPlug")) && selectedDmgItem.GetHealth("", "") > 0) {
					InediaInfectedAI_DamageCarElement(selectedDmgItem, selectedDmgPercent);
					return;
				}
			}

			selectedDmgPercent = 0;
			selectedDmgItem = null;

			randomImportantZones.Remove(randomImportantZoneIndex);
		}
	}

	void InediaInfectedAI_DamageCarZone(string dmgZone, float dmgPercent)
	{
		if (!dmgZone || dmgPercent <= 0) {
			return;
		}
		
		AddHealth(dmgZone, "", -dmgPercent / 100 * GetMaxHealth(dmgZone, ""));
	}

	void InediaInfectedAI_DamageCarElement(EntityAI dmgElement, float dmgPercent)
	{
		if (!dmgElement || dmgPercent <= 0) {
			return;
		}

		dmgElement.AddHealth("", "", -dmgPercent / 100 * dmgElement.GetMaxHealth("", ""));
	}

	// Play car collision sound
	int InediaInfectedAI_PlayCarCollisionSoundLastPlayedTime;
	void InediaInfectedAI_PlayCarCollisionSound(EntityAI target)
	{
		if (m_InediaInfectedAI_CollisionsSoundThreshold == 0 || !target) {
			return;
		}

		if ((GetGame().GetTime() - InediaInfectedAI_PlayCarCollisionSoundLastPlayedTime) <= 1000) {
			return;
		}

		vector targetPosition = target.GetPosition();

		// Play collision sound
		float speedKmH = GetVelocity(this).Length() * 3.6;

		if (speedKmH > m_InediaInfectedAI_CollisionsSoundThreshold) {
			// Play loud sound
			GetGame().CreateObject("InediaInfectedAI_CarHit", targetPosition);
		} else {
			// Play quiet sound
			GetGame().CreateObject("InediaInfectedAI_CarDoorHit", targetPosition);
		}

		InediaInfectedAI_PlayCarCollisionSoundLastPlayedTime = GetGame().GetTime();
	}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------=+++++++==--------------------------------------------------------------------------
// --------------------------------------------------------------=++=-.        ...:-=+++=----------------------------------------------------------------
// ------------------------------------------------------------==:                     .-*++=------------------------------------------------------------
// ---------------------------------------------------------==:                           :==+=----------------------------=-----------------------------
// -------------------------------------------------------+=.                              ...:-==-------------------------=-----------------------------
// -----------------------------------------------------++.                                ..  ...=---------=---------=----=-----------------------------
// ----------------------------------------------------*=                                  .    .. :=----------------------=-----------------------------
// ---------------------------------------------------=-                                   .     .  .=-::=-:---------------+-----------------------------
// ---------------------------------------------------+                                              .+::-=:------=---=----=-----------------------------
// --------------------------------------------------+: ..   :.              .                  ...   .=::--------=--------=-----------------------------
// --------------------------------------------------+  * +-*                 .               .. =+..  --::--:-----==------=-----------------------------
// -----------------------------------------------=::=..@:+#.  ... ...::::-::.         .::..   :=:%==.- +:::---------+-:---=-----------------------------
// -----------------------------------------------=:-=:-@+%:.:::  .::---:.   -+::::::::-:.......:+#%#-* +::----------==:---=-----------------------------
// ------------------------------------------------:==:+@%:       :::::-+*+=-:.. ..-:::         .:-%@=%:+:-----------=-:---=-----------------------------
// ---------------------------------------------=-::=*-%#.   .======-::   .:-=---::..:------:.   . .#+%++:-:---------+-:---=-----------------------------
// ----------------------------------------------:::==@#.      ...::=::-:::::----:::..:::==-:::..   .#@*+-::::-------=-----=-----------------------------
// ----------------------------------------------==:-+#              ::..:.  .    .... ::        .   .%#+:-==-------=------=-----------------------------
// --------------------------------------------+: :*+*#   .=+**+=-:       : -. : -   .:    .:..:---: =*%*+:.:+:---==-------=-----------------------------
// --------------------------------------------=#= =:**-.:-=+==*@@%*:.....=-+  = * .  ..-+*##%%@@@@+.*#*:-.*=+::--+--------=-----------------------------
// --------------------------------------------==%*.:+== *+*%@%%*+*%%***#@@%-:+#:+#===+%@@@@@##@@@@*#*--::#*==---=::::-----=-----------------------------
// ---------------------------------------------+*@@#-*-:**@@++  :  +@@@@@@=...=@@@@@@@@*...:*=*@@@%-+:::@@#+--:=-:::------=-----------------------------
// ---------------------------------------------==%@%*= +.#@%%= .+- =@@@@%+.:   +%#@@@@@. +- .#-@@@+.+ =%@%-=::=-::--------=-----------------------------
// ----------------------------------------------=**@%.:- #@@%*+=--+#@@+==.*@:*::*-+%@@%+---+*=%@@@- #..%#+%-::--::--------=-----------------------------
// ----------------------------------------------+#%%-.:= =@@@@@%%%@@*:::.:@@+%* #.-:+##%%###%@@@@@  # .:#@%-:::=----------=-----------------------------
// ----------------------------------------------+-#+. :-+.:+*#%##%#=:-:  +@@*@@--- ==:--=*%@@@@@@* ==  .+++-:::=----------=-----------------------------
// -----------------------------------------------+.=   . :--:-==-+:--   :@@@*+@%.+.-:=-.::::-==-:-=:.. +=:*-:--=----------------------------------------
// ------------------------------------------------+*-   .:-==-.-=  #. :=#@@%%-@#-@:+. .+::.::...    :=++=+-:=--=--------=-------------------------------
// -------------------------------------------------:==-...:::.--+.=: =+*@@#-+=:+@@+.- .:*::.:--====#*-:::::=-::=----------------------------------------
// ----------------------------------------------------=%@@@@@%*:::+ ++-+##=*@@:#@%--=  :==+#-:-=+*%%-:::--=-:::---------=:------------------------------
// ----------------------------------------------------:#@@@@@@@* #.+- :-+*++#=.:-=+. -  .%#:=%@@@@@#:::----:---=---------=------------------------------
// -----------------------------------------------------+#%#@@@@@+::+.-:==.%@%%@@+   ..+..@-=%@@@@%%+--==--::::=-:---------==----------------------------
// -----------------------------------------------------++*%*%@@%=.+:: :--=*:#%-=#===:-.* %.*+@%%@=++----:-:---=-:-----------=---------------------------
// --------------------------------------=--------------+-*+@%%@+- ===+%*###*#**#*:#=+=:* #.+%@+%#--+-:-:::::-=--------------=---------------------------
// ---------------------------------------=------------:+:+-%#+%+.:##*-*--= %. #.#+-%*=*=:+.%@#=#==.+:---::-:-=-:-----==----:=---------------------------
// ---------------------------------------+-------------+:.=:*..=:=#=#.% #%*@- #.#::#.+#+:::@*+:#.=:=::-::---=-::--:-=------=----------------------------
// --------------------------------------=----------=-:.+:. :== --*%@@*@#@@@@#=@#@##@%#%+=.=#--..-.-=:::::----==-=----=:---=-----------------------------
// ------------------------------------++----------==  :%.:.  +-- *@@@@@@#@@@@@@%@@@@@%@#+ #* ::: :*-+=-::-----:-=----=----=-----------------------------
// ---------------------------------=+=-------:-==+=  .%=%=:=::=- +@@**=+ %*=%:+=:%+@##*@+.#.   .+@@@*=*-:-----=-::-:==----=-----------------------------
// ----------------=-:::::::------++-::::--=---. :*. :#:.@@%#:- -::%*++=+.*:.+ := # #.**@:=+. :*@@@@@* .*::-::=-:::--+-----=-----------------------------
// -----------------=-:--::----===::-==--:.       =+ +. =%@@@@-  -::+%%=#=%*=#+#++##%=#+*.+ :*@@@@@@=   =-:::--::-=:=-:----=-----------------------------
// -----------------=----=--:::-===-:.            -=:+ -%#@@@@@-  ..+*++=---:::==*%#=+-+#*-*@@@@@@#:     *-=--:-:=--=-:----=-----------------------------
// -----------------+-------=+=-.                  -+=.*@@@@@@@@= :--**+*###++++##:.--:::=@@@@@@@*       .-.+=---=:--:-----=-----------------------------
// -----------------=--:=:-:.          :           :=-+%@@@@@@@@%- :-. ::---::.: .:..  .-%@@@@@@= ..:-.  .: ===:--:-::----:=-:---------------------------
// ------------------=-==:..            :           -+:#@@@@@@@@*#+. ..  .:::--+=.     -%@@@@@%:     ..... :- .:-=-:::-----=-----------------------------
// ------------------*+-----:        .=-.:  :    :.  :+:#@@@@@@@%.%%=  ..:    : .: .. =@@@@@@+             =      :-==----:=-:---------------------------
// ------------------+=------:.      :%@#:. =%+:.=+  .-+.#@@@@@@@-=@#*- . .-*%@#=.. :*@@@@@%-             =.         .:-=-:=:-=--------------------------
// ------------------=-------:....    .*@+-  #@@@@%    -=.*@@@@@@# ## +@%%@@@@@@@@%@@@@@@@*.           . :-   :=--:      .-+-----------------------------
// ------------------=---------:. :.    #@--  =%@@@-    -=.+@@@@@@:-@. %@@@@@@@@@@@@@@@@%-              .+.:*#=.  =   .    =+:---------------------------
// ------------------=----------:..::   :@%-    -%@#  .  --.-%@@@@#.%+ +@@@@@@@@@@@@@@#==:.             =.+@- .:.:.....    -*----------------------------
// ------------------+----------:- :-:   *@=-     #@. *.  :-..#@@@@:+%..@@@@@@@@@@@@*-+@@#::           ::#%- ::-:..   .   .:+----------------------------
// ------------------+:----------.- .-.  .@#-     -@= %+   ::: *@@@=:@+ *@@@@@@@#++=+%@@@@%-. ..       -#*.::..       .   .:-=---------------------------
// ------------------+-----------: -  =   *@=.     %* %%    :--=+@@# %% -@@@@@*:.=-:=%%@@@@%--:       -++:-           .   ::-=---------------------------
// ------------------==----------: .- .:  :@#:     =%.#@=-. .=@%+-*% #@+ @@@@%     :-*@@@*#@%=::     .-* .               .:::-=--------------------------
// -------------------=----------:  :: :   #@=     .@*-@#-+ =%@#-+---%@@.#@@@+   .  :#@#:+#@@@%*+:.  -=:             . . :::::=--------------------------
// -------------------=----------:   =*.   =@@:    +@@+%@+.:*@@*--=-%@@@:*@@@= .=*#@@#--=.:#-:%*-+=:.:*     ...      :: :--::----------------------------
// -------------------=----------:    +@*.  %@%..=*@@@@%@=.=@@@==-.-%@@@.+@@@-+@@@@#---  ::  =-.:--===-    += -      : .-:-------------------------------
// -------------------=----------:     -%@*.-@@*  .+@@@@@--%@@@#=-=: -*%.=@@@:%@@%=--   :.  ..   :----.  -%- -:        .---------------------------------
// -----------------------------::      .#@@+#@@*.  :%@@@**@@@@-=@@#: .:**@@@:@@=-:   .-             . -%%-:-.        .:---------------------------------
// -----------------------------::        -#@@@@@%=  :@@@*@@@@@*@%**#::. +@@#=@*.:   .-             .=%@+--.         .-----------------------------------
// ----------------------------=::          -*@@@@@%+.=@@@@@@@%@@- .-= .- +@#@%.:    -             =%%+.-:           .-----------------------------------
// ----------------------------+::            .+%@@@@%=*@@@@@@@@@:  .=:-. .@@%::  ..              *@+. :.     -:     .-----------------------------------
// ----------------------------+:-              .+@@@@@#@@@@@@@@@#   -.   -@@-: .:               =#:   .    :*.      :-----------------------------------
// ----------------------------=--:.              :%@@@@@@@@@@@@@@= :    .%@=.            ..    ++         =@=::     :-----------------------------------
// ---------------------------------:.             .*@@@@@@@@@@@@@@:.:   =@#     :        :    +-         *@@=.  .   .-----------------------------------
// -----------------------------------:           .. =%@@@@@@@@@@@@# -.- =@*    -         :   +:        :%@@=   .     :----------------------------------
// ------------------------------------:           .:: +@@@@@@@@@@@@-.:-:=*+   -.            =-   :.   :%@@-  .       .:---------------------------------
// -------------------------------------.            .:..*@@@@@@@@@@+ -  .=+  -.            :+  .=.   .%@@- ..         .---------------------------------
// -------------------------------------:              -  :#@@@@@@@@+ .:   =. +             +  :=     +@%: .            :--------------------------------
// --------------------------------------::::::::::::::::::----------:::::::::-::::::::::::::::-::::::---::::::::::::::::--------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------

	// Reduce car speed after collision
	ref map<int, int> InediaInfectedAI_ReduceSpeedAfterCollisionTargetLastCollisionTime = new map<int, int>;
	void InediaInfectedAI_ReduceSpeedAfterCollision(EntityAI target)
	{
		if (m_InediaInfectedAI_CollisionsSpeedReductionMultiplier <= 0 || !target) {
			return;
		}

		int targetId = target.GetID();
		int targetLastCollisionTime = 0;
		InediaInfectedAI_ReduceSpeedAfterCollisionTargetLastCollisionTime.Find(targetId, targetLastCollisionTime);

		if ((GetGame().GetTime() - targetLastCollisionTime) <= 50) {
			return;
		}

		float carSpeedKmH = GetVelocity(this).Length() * 3.6;
		float carWeightKg = dBodyGetMass(this);
		float targetWeightKg = dBodyGetMass(target);
		float newCarSpeedKmH = (carWeightKg - targetWeightKg) * carSpeedKmH / (carWeightKg + targetWeightKg);
		InediaInfectedAI_AddVelocity(m_InediaInfectedAI_CollisionsSpeedReductionMultiplier * (newCarSpeedKmH - carSpeedKmH));

		InediaInfectedAI_ReduceSpeedAfterCollisionTargetLastCollisionTime[targetId] = GetGame().GetTime();
	}

	// Add velocity (km/H) to current vehicle velocity
	void InediaInfectedAI_AddVelocity(float addedVelocityKmH)
	{
		if (addedVelocityKmH == 0) {
			return;
		}

		vector currentVelocity = GetVelocity(this);
		float newVelocityLength = currentVelocity.Length() + addedVelocityKmH / 3.6;

		if (newVelocityLength < 0) {
			newVelocityLength = 0;
		}

		vector newVelocity = currentVelocity.Normalized() * newVelocityLength;
		SetVelocity(this, newVelocity);
	}

	// Get car hood item element
    CarDoor InediaInfectedAI_GetHood()
	{
		float attachmentCount = GetInventory().AttachmentCount();
		for (int i = 0; i < attachmentCount; i++) {
			EntityAI attachment = GetInventory().GetAttachmentFromIndex(i);
			if (attachment.IsInherited(CarDoor)) {
				string lowerAttachmentType = attachment.GetType();
				lowerAttachmentType.ToLower();
				if (lowerAttachmentType.Contains("hood")) {
					return CarDoor.Cast(attachment);
				}
			}
		}

		return null;
	}

	CarDoor InediaInfectedAI_GetNonBrokenDoor()
	{
		float attachmentCount = GetInventory().AttachmentCount();
		for (int i = 0; i < attachmentCount; i++) {
			EntityAI attachment = GetInventory().GetAttachmentFromIndex(i);
			if (attachment.IsInherited(CarDoor) && attachment.GetHealth("", "") > 0) {
				string lowerAttachmentType = attachment.GetType();
				lowerAttachmentType.ToLower();
				if (!lowerAttachmentType.Contains("hood")) {
					return CarDoor.Cast(attachment);
				}
			}
		}

		return null;
	}

	PlayerBase InediaInfectedAI_GetFirstCrewMember()
	{
		PlayerBase firstCrewMember;

		for (int i = 0; i < CrewSize(); i++) {
			firstCrewMember = PlayerBase.Cast(CrewMember(i));
			if (firstCrewMember) {
				return firstCrewMember;
			}
		}

		return null;
	}

	vector InediaInfectedAI_GetPlayerHeadPosition()
	{
		vector playerHeadPos = vector.Zero;

		PlayerBase vehicleCrewMember = InediaInfectedAI_GetFirstCrewMember();
		if (vehicleCrewMember) {
			playerHeadPos = vehicleCrewMember.InediaInfectedAI_GetHeadPosition();
		}

		if (!playerHeadPos) {
			playerHeadPos = GetPosition() + vector.Up;
		}

		return playerHeadPos;
	}
	
	vector InediaInfectedAI_GetPlayerPosition()
	{
		vector playerPos = vector.Zero;

		PlayerBase vehicleCrewMember = InediaInfectedAI_GetFirstCrewMember();
		if (vehicleCrewMember) {
			playerPos = vehicleCrewMember.GetPosition();
		}

		if (!playerPos) {
			playerPos = GetPosition() + vector.Up;
		}

		return playerPos;
	}
	
	HeadlightH7 InediaInfectedAI_GetNonBrokenReflector()
	{
		float attachmentCount = GetInventory().AttachmentCount();
		for (int i = 0; i < attachmentCount; i++) {
			EntityAI attachment = GetInventory().GetAttachmentFromIndex(i);
			if (attachment.IsInherited(HeadlightH7) && attachment.GetHealth("", "") > 0) {
				return HeadlightH7.Cast(attachment);
			}
		}

		return null;
	}

	bool InediaInfectedAI_HasCrewMembers()
	{
		for (int i = 0; i < CrewSize(); i++) {
			if (CrewMember(i) && CrewMember(i).IsInherited(PlayerBase)) {
				return true;
			}
		}

		return false;
	}
	
	bool InediaInfectedAI_IsHelicopter()
	{
#ifdef EXPANSIONMODVEHICLE
		return IsInherited(ExpansionHelicopterScript);
#endif
		
		return false;
	}
	
	bool InediaInfectedAI_EngineIsOn()
	{
#ifdef EXPANSIONMODVEHICLE
		if (IsInherited(ExpansionHelicopterScript)) {
			ExpansionHelicopterScript helicopter = ExpansionHelicopterScript.Cast(this);
			if (helicopter) {
				return helicopter.Expansion_EngineIsSpinning();
			}
		}
#endif
		
		return EngineIsOn();
	}
	
	float InediaInfectedAI_EngineGetRPMIdle()
	{
#ifdef EXPANSIONMODVEHICLE
		if (IsInherited(ExpansionHelicopterScript)) {
			ExpansionHelicopterScript helicopter = ExpansionHelicopterScript.Cast(this);
			if (helicopter && helicopter.m_Simulation) {
				return 0;
			}
		}
#endif
		
		return EngineGetRPMIdle();
	}
	
	float InediaInfectedAI_EngineGetRPMMax()
	{
#ifdef EXPANSIONMODVEHICLE
		if (IsInherited(ExpansionHelicopterScript)) {
			ExpansionHelicopterScript helicopter = ExpansionHelicopterScript.Cast(this);
			if (helicopter && helicopter.m_Simulation) {
				return 1;
			}
		}
#endif
		
		return EngineGetRPMMax();
	}
	
	float InediaInfectedAI_EngineGetRPM()
	{
#ifdef EXPANSIONMODVEHICLE
		if (IsInherited(ExpansionHelicopterScript)) {
			ExpansionHelicopterScript helicopter = ExpansionHelicopterScript.Cast(this);
			if (helicopter && helicopter.m_Simulation) {
				return helicopter.m_Simulation.m_RotorSpeed;
			}
		}
#endif
		
		return EngineGetRPM();
	}

	// This is a workaround, a necessary duplication of functionality from "ItemBase",
	// as there's no option to place these functions in an object shared between "ItemBase" and "CarScript"
	float m_InediaInfectedAI_WithoutMovementSecondsLastUpdateTime = GetGame().GetTickTime();
	vector m_InediaInfectedAI_WithoutMovementSecondsLastPosition;
	float m_InediaInfectedAI_WithoutMovementSeconds = 0;
	float InediaInfectedAI_GetWithoutMovementSeconds()
	{
		float deltaTime = GetGame().GetTickTime() - m_InediaInfectedAI_WithoutMovementSecondsLastUpdateTime;

		if (vector.DistanceSq(GetPosition(), m_InediaInfectedAI_WithoutMovementSecondsLastPosition) <= 0.1 * 0.1) {
			InediaInfectedAI_SetWithoutMovementSeconds(m_InediaInfectedAI_WithoutMovementSeconds + deltaTime);
		} else {
			InediaInfectedAI_SetWithoutMovementSeconds(0);
		}

		return m_InediaInfectedAI_WithoutMovementSeconds;
	}

	void InediaInfectedAI_SetWithoutMovementSeconds(float value)
	{
#ifdef SERVER
		m_InediaInfectedAI_WithoutMovementSeconds = value;
		m_InediaInfectedAI_WithoutMovementSecondsLastUpdateTime = GetGame().GetTickTime();
		m_InediaInfectedAI_WithoutMovementSecondsLastPosition = GetPosition();
#endif
	}
}
