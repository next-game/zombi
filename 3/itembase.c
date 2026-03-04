/**
 * Created: 2023-09-01
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

modded class ItemBase extends InventoryItem
{
	InediaInfectedAI_CustomIrritant m_InediaInfectedAI_CustomIrritant;
	int m_InediaInfectedAI_IsDeepWoundClosureItem = 0;
	int m_InediaInfectedAI_IsExtractBulletItem = 0;
	int m_InediaInfectedAI_IsInternalBleedingStoppingItem = 0;
	int m_InediaInfectedAI_IsDeepWoundBandageItem = 0;
	int m_InediaInfectedAI_IsFractureStabilizationItem = 0;
	int m_InediaInfectedAI_IsCutClosureItem = 0;
	int m_InediaInfectedAI_IsUsableItem = 0;
	int m_InediaInfectedAI_IsHealthRegenItem = 0;
	int m_InediaInfectedAI_IsBoneRegenItem = 0;
	int m_InediaInfectedAI_IsBloodRegenItem = 0;
	int m_InediaInfectedAI_IsBloodClottingItem = 0;
	int m_InediaInfectedAI_IsDeepWoundRegenItem = 0;
	int m_InediaInfectedAI_IsVomitReliefItem = 0;
	int m_InediaInfectedAI_IsPainkillerLightItem = 0;
	int m_InediaInfectedAI_IsPainkillerMediumItem = 0;
	int m_InediaInfectedAI_IsPainkillerStrongItem = 0;

	void ItemBase()
	{
#ifdef SERVER
		m_InediaInfectedAI_CustomIrritant = InediaInfectedAI_Config.GetInstance().Zombies.GetCustomIrritantByItem(this);

		if (InediaInfectedAI_IsIrritationItem()) {
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InediaInfectedAI_IrritationWorkflow, Math.RandomIntInclusive(0, 5000), false);
		}
#endif
		
		m_InediaInfectedAI_IsDeepWoundClosureItem = GetGame().ConfigGetInt("CfgVehicles " + GetType() + " InediaPain IsDeepWoundClosureItem");
		m_InediaInfectedAI_IsExtractBulletItem = GetGame().ConfigGetInt("CfgVehicles " + GetType() + " InediaPain IsExtractBulletItem");
		m_InediaInfectedAI_IsInternalBleedingStoppingItem = GetGame().ConfigGetInt("CfgVehicles " + GetType() + " InediaPain IsInternalBleedingStoppingItem");
		m_InediaInfectedAI_IsDeepWoundBandageItem = GetGame().ConfigGetInt("CfgVehicles " + GetType() + " InediaPain IsDeepWoundBandageItem");
		m_InediaInfectedAI_IsFractureStabilizationItem = GetGame().ConfigGetInt("CfgVehicles " + GetType() + " InediaPain IsFractureStabilizationItem");
		m_InediaInfectedAI_IsCutClosureItem = GetGame().ConfigGetInt("CfgVehicles " + GetType() + " InediaPain IsCutClosureItem");
		m_InediaInfectedAI_IsUsableItem = GetGame().ConfigGetInt("CfgVehicles " + GetType() + " InediaPain IsUsableItem");
		m_InediaInfectedAI_IsHealthRegenItem = GetGame().ConfigGetInt("CfgVehicles " + GetType() + " InediaPain IsHealthRegenItem");
		m_InediaInfectedAI_IsBoneRegenItem = GetGame().ConfigGetInt("CfgVehicles " + GetType() + " InediaPain IsBoneRegenItem");
		m_InediaInfectedAI_IsBloodRegenItem = GetGame().ConfigGetInt("CfgVehicles " + GetType() + " InediaPain IsBloodRegenItem");
		m_InediaInfectedAI_IsBloodClottingItem = GetGame().ConfigGetInt("CfgVehicles " + GetType() + " InediaPain IsBloodClottingItem");
		m_InediaInfectedAI_IsDeepWoundRegenItem = GetGame().ConfigGetInt("CfgVehicles " + GetType() + " InediaPain IsDeepWoundRegenItem");
		m_InediaInfectedAI_IsVomitReliefItem = GetGame().ConfigGetInt("CfgVehicles " + GetType() + " InediaPain IsVomitReliefItem");
		m_InediaInfectedAI_IsPainkillerLightItem = GetGame().ConfigGetInt("CfgVehicles " + GetType() + " InediaPain IsPainkillerLightItem");
		m_InediaInfectedAI_IsPainkillerMediumItem = GetGame().ConfigGetInt("CfgVehicles " + GetType() + " InediaPain IsPainkillerMediumItem");
		m_InediaInfectedAI_IsPainkillerStrongItem = GetGame().ConfigGetInt("CfgVehicles " + GetType() + " InediaPain IsPainkillerStrongItem");
	}
	
	override void InitializeActions()
	{
		if (GetGame().ConfigGetInt("CfgVehicles " + GetType() + " InediaPain EnableFlugegeheimen") == 1) {
			m_InputActionMap = m_ItemTypeActionsMap.Get(this.Type());
			if (!m_InputActionMap) {
				TInputActionMap iam = new TInputActionMap;
				m_InputActionMap = iam;
				
				AddAction(ActionTakeItem);
				AddAction(ActionTakeItemToHands);
				AddAction(ActionWorldCraft);
				AddAction(ActionDropItem);
				AddAction(ActionAttachWithSwitch);
				
				InediaInfectedAI_SetActions();
				
				m_ItemTypeActionsMap.Insert(this.Type(), m_InputActionMap);
			}
		} else {
			super.InitializeActions();
		}
	}

// IRRITATION LOGIC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// Irritation cycle, 5-second interval
	bool InediaInfectedAI_IrritationItemIsIrritatingLastState = false;
	float m_InediaInfectedAI_IrritationTimer;
	void InediaInfectedAI_IrritationWorkflow()
	{
#ifdef SERVER
		if (IsDamageDestroyed()) return;

		bool isIrritating = InediaInfectedAI_IrritationItemIsIrritating();

		if (isIrritating) {
			if (!InediaInfectedAI_IrritationItemIsIrritatingLastState) {
				InediaInfectedAI_SetWithoutMovementSeconds(0);
			}

			if (InediaInfectedAI_GetWithoutMovementSeconds() <= InediaInfectedAI_Config.GetInstance().LossInterestToIrritantAfterSeconds) {
				InediaInfectedAI_IrritationAction();
			}
		}

		InediaInfectedAI_IrritationItemIsIrritatingLastState = isIrritating;

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InediaInfectedAI_IrritationWorkflow, 5000, false);
#endif
	}

	// If "true" - logic of zombie irritation will be applied to this item
	bool InediaInfectedAI_IsIrritationItem()
	{
		if (m_InediaInfectedAI_CustomIrritant) {
			return true;
		}

		return false;
	}

	// Determines if irritating item is currently irritating
	bool InediaInfectedAI_IrritationItemIsIrritating()
	{
		return GetCompEM() && GetCompEM().IsWorking();
	}

	// Actions that attract zombies are triggered here
	void InediaInfectedAI_IrritationAction()
	{
		if (m_InediaInfectedAI_CustomIrritant) {
			InediaInfectedAI_IrritantsManager.MassSearchActivation(GetPosition(), GetPosition(), InediaInfectedAI_Irritants.CUSTOM, this);
		}
	}

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

// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ....................................................................--.::::...........................................................................
// ...................................................................=+=+=-=====-:......................................................................
// ...................................................................#+===--=----==:....................................................................
// ...................................................................+:::-=::-----=+-...................................................................
// ..................................................................=.--:..=+=====++*-..................................................................
// ...................................................................+.-:...-+***+===+=:................................................................
// ...................................................................:. ...   -#**+=+=*+----:.:.........................................................
// ...................................................................--:.....=.-%*-=+*+#:..:::..........................................................
// ....................................................................=::-=:.  :+-==-=+++::::...........................................................
// .....................................................................:.:=---::::..=-:++++=====---=-::::...............................................
// ....................................................................:===-:.::.--:::--=**==----..::.::..-:.............................................
// .................................................................-::=-.+::. :=:::::::: :*==----=--===--::-............................................
// ...............................................................-:. .-+-::::-+......   ::.+*+------======-=:...........................................
// ..............................................................-.:=*+=*:.     .*=:...    :-*-**=--=-::-:=-=+...........................................
// .............................................................--+=:  .        :*-+==-::...:=+:-=-...:::::=-++::::......................................
// .............................................................:*    ..       .::-:=.-++=:..:++----::...:::-::-.........................................
// ..............................................................*.   .     ..::...=*+...:=+-. ++=+====-::...:-:-:.......................................
// ..............................................................+-:.  :..:::.....-.+-=:   :-=+==.:-+=====+==-:..........................................
// .............................................................-=*+ ..- ..     . :++*+=-.   :-:--:..--======+++=-.......................................
// .............................................................-  =   :=..      --..::=*+:   :=.:-:::........:-:---.....................................
// ............................................................=.  :+-:.=..:===+*:.......:=-   .-+.::.::........::..-....................................
// ..........................................................-+:   -*::::::..::.=:.........-:  ::-- .:-::::..........:...................................
// .........................................................=.==..=*=........::-=+=.........=:-  :=.   ::=::.::..........................................
// ........................................................=   ==-:+ .::..:::-===-:=........-+   :--.     ::=-::::.......................................
// ......................................................:-    =-.=+   .:--::. :::::*-.......=.  -=.::       ..:--::.:...................................
// .....................................................:-   :-.--.+++-::.   .::-=-: :=-......+:=+=  .::... ......::---:::...............................
// ...................................................:++   -:.-* =+ :==-:---==+..     :--....*:.==:.              ...-===-::............................
// ..................................................-=--==-:==-=-.#  ...::=-===. :     .-:::=-:..+:.:                 ..:+*+=::.........................
// .................................................=:=..*=-+* += ++ . :   -  :.:: :.     .:.:=.=--=:.::        ..::::.......--==-:......................
// ................................................:*-# =--:+.+= .== ....  :.  : :-  ..     .:.:*#:*:...:.     :.................:-......................
// ..............................................:::--=-*:.:-=-  = =  : -   :   : .+- .:      ...-=-:.....:. .:..........................................
// ...........................................-::::===+-...+-=  :=:+  :.    :   :.  --:.:       .::  .::.....:...........................................
// .......................................:-::::=--:..:...-*=   *--+  .-     - . -   -:--:        .-::  .::..............................................
// ....................................-::::-=--.........:#+   -*-:+   -     :. : -   ::.=:  .      ..:::  .:::..........................................
// ................................:--:::===:............**:  .#. .+.  :      - .. -   .- =:  :.      .  :-:   .:::..........:+:.........................
// .............................:-::::===:..............+==:=:+=   -..  .      - : .:    - =:  .:           :::    ...:::--==-  .:.......................
// ..........................-:::::---:................=:*.  -#     =  .:       : : .:    -.+:   :: ..         :::              .:.......................
// .......................:--::=-=-...................:=.*   ==      =.  .       :.. :.  . -:+-    :..:.          :::.          :........................
// ......................=--=--:......................* -=  -=.      -+           .:  -   - ::=-     :..-.            .:.       :........................
// ....................:==-:.......................+.:= +  :.=     .=:.-.          .:  -   - ---=     .: :=:              ...  :.........................
// ................................................=+= -  -..-    --....-: .         .  :   -.:=.=.     :: :=:.                :.........................
// ...............................................::*=+  =. -   .=........- :         ....   -::=.=.      :. .--:              :.........................
// ...............................................-::.===*  :  -:..........-:..          :    --:-.=:      .:  .:-:.           :.........................
// ...............................................=-.  -.#:  --.............=+.          ..   .::-- --       .:.  .:-:.        -.........................
// ...............................................+:.  :.*--+:...............==:.         -    -..-: :-.        .:.  .:--.     -.........................
// ...............................................-:-  --    -................=-:         .-...::..:: :-.           ..   .:-: -..........................
// ...............................................=-==+=.  :-.................:-+:   .     =......:.-: .:.                  :=:..........................
// ..............................................:+:+  -.::=...................-=.::  :.   .-......:.-:  -.               .::............................
// ..............................................-=:  .-  .=....................+...:.  :.  =.........-:  :..           .:...............................
// ..............................................=.   =   =.....................+.....:. .: ::..:....-.*.  : ..        :.:...............................
// ..............................................+   -   :-.....................=.......:.:-.=.-..    ===   .  :.     :::.::.............................
// ..............................................+  .:.:-=......................-...........-=:.=..   =-:=       .. .=:..:...............................
// ..............................................=  +:..=......................:.................-..  .:-.=        :=.:::................................
// .............................................:: =   =.........................................-.:   -=.-: :     =::...................................
// .............................................=.-   =...........................................= .  ==..-: :   =......................................
// .............................................+.:  -:...........................................:::  ==...-. : -:......................................
// .............................................+-  .=.............................................-:. -=....-. ::.......................................
// .............................................++---=..............................................-: :+.....:::........................................
// ............................................-+=...+--.............................................+.:*................................................
// ............................................+-+::-: =.............................................*::+................................................
// ............................................+ =  .-:+.............................................=: =-...............................................
// ...........................................=. :. =+ =............................................:-.  =-..............................................
// ..........................................:*::.-:--.-............................................:-:..-=..............................................
// .........................................:=    -=..--............................................:-    =-.............................................
// .........................................-=====++..:..............................................+---=*=.............................................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................

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

// THROWING PROJECTILE LOGIC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	const float INEDIA_INFECTEDAI_MIN_DAMAGE_WEIGHT = 200;
	const float INEDIA_INFECTEDAI_MAX_DAMAGE_WEIGHT = 3000;

	override protected void EOnContact(IEntity other, Contact extra)
	{
		super.EOnContact(other, extra);

#ifdef SERVER
		if (other) {
			InediaInfectedAI_ThrowingProjectileOnContactServer(other);
		}
#endif
	}

	ref InediaInfectedAI_ThrowingProjectile m_InediaInfectedAI_ThrowingProjectile;
	void InediaInfectedAI_SetThrowingProjectile(InediaInfectedAI_ThrowingProjectile throwingProjectile)
	{
#ifdef SERVER
		m_InediaInfectedAI_ThrowingProjectile = throwingProjectile;
#endif
	}

	InediaInfectedAI_ThrowingProjectile InediaInfectedAI_GetThrowingProjectile()
	{
		return m_InediaInfectedAI_ThrowingProjectile;
	}

	bool InediaInfectedAI_IsThrowingProjectile()
	{
		if (m_InediaInfectedAI_ThrowingProjectile) {
			return true;
		}

		return false;
	}

	void InediaInfectedAI_ThrowingProjectileOnContactServer(notnull IEntity other)
	{
		if (InediaInfectedAI_IsThrowingProjectile()) {
			// Ignoring creature own projectiles, as some modded items trigger the ItemBase.EOnContact() handler immediately after being thrown.
			if (m_InediaInfectedAI_ThrowingProjectile.m_ThrowerEntity && m_InediaInfectedAI_ThrowingProjectile.m_ThrowerEntity == other) {
			    return;
			}
			
			if (other.IsInherited(PlayerBase)) {
				PlayerBase player = PlayerBase.Cast(other);
				if (player) {
					if (InediaInfectedAI_ThrowingProjectileGetHealthDamage() > 0 || InediaInfectedAI_ThrowingProjectileGetShockDamage() > 0 || InediaInfectedAI_ThrowingProjectileGetStaminaDamage() > 0 || InediaInfectedAI_ThrowingProjectileGetBloodDamage() > 0 || InediaInfectedAI_ThrowingProjectileGetStunChancePercent() > 0 || InediaInfectedAI_ThrowingProjectileGetBleedingChancePercent() > 0) {
						DamageSystem.CloseCombatDamageName(this, player, player.GetHitComponentForAI(), "InediaInfectedAI_ThrowingProjectile", GetPosition());
						SetVelocity(this, vector.RandomDir() * 5);
						//GetGame().CreateObject("InediaInfectedAI_ItemHit", GetPosition());
					}
				}
			} else if (other.IsInherited(ZombieBase)) {
				ZombieBase zombie = ZombieBase.Cast(other);
				if (zombie) {
					if (InediaInfectedAI_ThrowingProjectileGetHealthDamage() > 0 || InediaInfectedAI_ThrowingProjectileGetStunChancePercent() > 0) {
						DamageSystem.CloseCombatDamageName(this, zombie, zombie.InediaInfectedAI_GetRandomDmgComponentName(), "InediaInfectedAI_ThrowingProjectile", GetPosition());
						SetVelocity(this, vector.RandomDir() * 5);
						//GetGame().CreateObject("InediaInfectedAI_ItemHit", GetPosition());
					}
				}
			} else if (other.IsInherited(AnimalBase)) {
				AnimalBase animal = AnimalBase.Cast(other);
				if (animal) {
					if (InediaInfectedAI_ThrowingProjectileGetHealthDamage() > 0 || InediaInfectedAI_ThrowingProjectileGetStunChancePercent() > 0) {
						DamageSystem.CloseCombatDamageName(this, animal, animal.InediaInfectedAI_GetRandomDmgComponentName(), "InediaInfectedAI_ThrowingProjectile", GetPosition());
						
						SetVelocity(this, vector.RandomDir() * 5);
						//GetGame().CreateObject("InediaInfectedAI_ItemHit", GetPosition());
					}
					
					if (Math.RandomBool() && InediaInfectedAI_DayzHorseCompatibility.IsHorse(animal)) {
						// Since the player model on the horse has no collision with projectiles,
						// the player riding the horse has a certain chance (50%) of taking damage that was intended for the horse.
						PlayerBase horseRiderPb = InediaInfectedAI_DayzHorseCompatibility.GetHorseRider(animal);
						if (horseRiderPb) {
							if (InediaInfectedAI_ThrowingProjectileGetHealthDamage() > 0 || InediaInfectedAI_ThrowingProjectileGetShockDamage() > 0 || InediaInfectedAI_ThrowingProjectileGetStaminaDamage() > 0 || InediaInfectedAI_ThrowingProjectileGetBloodDamage() > 0 || InediaInfectedAI_ThrowingProjectileGetStunChancePercent() > 0 || InediaInfectedAI_ThrowingProjectileGetBleedingChancePercent() > 0) {
								DamageSystem.CloseCombatDamageName(this, horseRiderPb, horseRiderPb.GetHitComponentForAI(), "InediaInfectedAI_ThrowingProjectile", GetPosition());
							}
						}
					}
				}
			} else if (other.IsInherited(CarScript)) {
				CarScript vehicle = CarScript.Cast(other);
				if (vehicle) {
					vehicle.InediaInfectedAI_AttackedByEntity(this);
				}
			}

			if (m_InediaInfectedAI_ThrowingProjectile.m_DeleteProjectileAfterSeconds == 0) {
				DeleteSafe();
			}

			if (m_InediaInfectedAI_ThrowingProjectile.m_DestroyProjectileAfterSeconds == 0) {
				InediaInfectedAI_ThrowingProjectileDestroy();
			}

			if (m_InediaInfectedAI_ThrowingProjectile.m_ActivateGrenadeAfterSeconds == 0) {
				InediaInfectedAI_ThrowingProjectileActivateGrenade();
			}

			if (m_InediaInfectedAI_ThrowingProjectile.m_StartWorkingAfterSeconds == 0) {
				InediaInfectedAI_ThrowingProjectileStartWorking();
			}

			// After contact, the projectile loses its damage properties
			InediaInfectedAI_SetThrowingProjectile(null);
		}
	}

	float InediaInfectedAI_ThrowingProjectileGetHealthDamage()
	{
		float damage = 0;

		if (InediaInfectedAI_IsThrowingProjectile()) {
			if (m_InediaInfectedAI_ThrowingProjectile.m_TargetHealthDamage >= 0) {
				damage = m_InediaInfectedAI_ThrowingProjectile.m_TargetHealthDamage;
			} else if (GetWeightEx() >= INEDIA_INFECTEDAI_MIN_DAMAGE_WEIGHT) {
				damage = MiscGameplayFunctions.InediaInfectedAI_GetItemMeleeHeavyDamage(this, "Health");
				if (damage <= 0) {
					damage = Math.Clamp(30 * GetWeightEx() / INEDIA_INFECTEDAI_MAX_DAMAGE_WEIGHT, 0, 30);
				}
			}
		}

		if (damage <= 0) {
			return 0;
		} else {
			return damage * InediaInfectedAI_ThrowingProjectileGetThrowerDamageMultiplier();
		}
	}

	float InediaInfectedAI_ThrowingProjectileGetShockDamage()
	{
		float damage = 0;

		if (InediaInfectedAI_IsThrowingProjectile()) {
			if (m_InediaInfectedAI_ThrowingProjectile.m_TargetShockDamage >= 0) {
				damage = m_InediaInfectedAI_ThrowingProjectile.m_TargetShockDamage;
			} else if (GetWeightEx() >= INEDIA_INFECTEDAI_MIN_DAMAGE_WEIGHT) {
				damage = MiscGameplayFunctions.InediaInfectedAI_GetItemMeleeHeavyDamage(this, "Shock");
				if (damage <= 0) {
					damage = Math.Clamp(60 * GetWeightEx() / INEDIA_INFECTEDAI_MAX_DAMAGE_WEIGHT, 0, 60);
				}
			}
		}

		if (damage <= 0) {
			return 0;
		} else {
			return damage * InediaInfectedAI_ThrowingProjectileGetThrowerDamageMultiplier();
		}
	}
	
	float InediaInfectedAI_ThrowingProjectileGetPainMultiplier()
	{
		if (InediaInfectedAI_IsThrowingProjectile() && m_InediaInfectedAI_ThrowingProjectile.m_TargetPainMultiplier >= 0) {
			return m_InediaInfectedAI_ThrowingProjectile.m_TargetPainMultiplier;
		}
		
		return 1;
	}

	float InediaInfectedAI_ThrowingProjectileGetStaminaDamage()
	{
		float damage = 0;

		if (InediaInfectedAI_IsThrowingProjectile()) {
			if (m_InediaInfectedAI_ThrowingProjectile.m_TargetStaminaDamage >= 0) {
				damage = m_InediaInfectedAI_ThrowingProjectile.m_TargetStaminaDamage;
			} else if (GetWeightEx() >= INEDIA_INFECTEDAI_MIN_DAMAGE_WEIGHT) {
				damage = MiscGameplayFunctions.InediaInfectedAI_GetItemMeleeHeavyDamage(this, "Shock");
				if (damage <= 0) {
					damage = Math.Clamp(30 * GetWeightEx() / INEDIA_INFECTEDAI_MAX_DAMAGE_WEIGHT, 0, 30);
				}
			}
		}

		if (damage <= 0) {
			return 0;
		} else {
			return damage * InediaInfectedAI_ThrowingProjectileGetThrowerDamageMultiplier();
		}
	}
	
	float InediaInfectedAI_ThrowingProjectileGetBloodDamage()
	{
		float damage = 0;

		if (InediaInfectedAI_IsThrowingProjectile()) {
			if (m_InediaInfectedAI_ThrowingProjectile.m_TargetBloodDamage >= 0) {
				damage = m_InediaInfectedAI_ThrowingProjectile.m_TargetBloodDamage;
			} else if (GetWeightEx() >= INEDIA_INFECTEDAI_MIN_DAMAGE_WEIGHT) {
				damage = MiscGameplayFunctions.InediaInfectedAI_GetItemMeleeHeavyDamage(this, "Health");
				if (damage <= 0) {
					damage = Math.Clamp(30 * GetWeightEx() / INEDIA_INFECTEDAI_MAX_DAMAGE_WEIGHT, 0, 30);
				}
			}
		}

		if (damage <= 0) {
			return 0;
		} else {
			return damage * InediaInfectedAI_ThrowingProjectileGetThrowerDamageMultiplier();
		}
	}

// 0ezCbkPVZcwFVszHDnSa7lVRMnRCQ0t5Q0dDclhsRmUvbnRGMGRkejFEZWhhaHRPa2c4alVIcXhHaWtBZ3piNkZJTmdxM3puUEpUWmM0a1JsUkVkVjNwTHA4Vi9kQjVGc3YxbjZwNDF0VytQREFVcE9LYXlqODZDRE5HbzZuNWpOUEdGNml2VGgyejRRTVRYTXg3RXl1NmtwWVR0ZTFrWGR1U0dMOERRNGppZEt4Wk1BTDM5cXlCNjdKQ0Zkc2xLclNVeGhFbHRNVUlhOTJyUUVTYW04ZXBubnJtNW9QbDNZOXRFSGhKbHNPOFFNYy9Pd2ZOTjc1a3VGWDl1b0RaMWd6OTF4Z0xLUGdpT29EQ2IzMHhkR0F1K0wzNHlQWEFLRklHWCtRPT0=

	float InediaInfectedAI_ThrowingProjectileGetStunChancePercent()
	{
		float chancePercent = 0;

		if (InediaInfectedAI_IsThrowingProjectile()) {
			if (m_InediaInfectedAI_ThrowingProjectile.m_TargetStunChancePercent >= 0) {
				chancePercent = m_InediaInfectedAI_ThrowingProjectile.m_TargetStunChancePercent;
			} else if (GetWeightEx() >= INEDIA_INFECTEDAI_MIN_DAMAGE_WEIGHT) {
				chancePercent = MiscGameplayFunctions.InediaInfectedAI_GetItemMeleeHeavyDamage(this, "Shock");
				if (chancePercent <= 0) {
					chancePercent = Math.Clamp(100 * GetWeightEx() / INEDIA_INFECTEDAI_MAX_DAMAGE_WEIGHT, 0, 100);
				}
			}
		}

		if (chancePercent <= 0) {
			return 0;
		} else {
			return chancePercent * InediaInfectedAI_ThrowingProjectileGetThrowerDamageMultiplier();
		}
	}

	float InediaInfectedAI_ThrowingProjectileGetBleedingChancePercent()
	{
		float chancePercent = 0;

		if (InediaInfectedAI_IsThrowingProjectile()) {
			if (m_InediaInfectedAI_ThrowingProjectile.m_TargetBleedingChancePercent >= 0) {
				chancePercent = m_InediaInfectedAI_ThrowingProjectile.m_TargetBleedingChancePercent;
			} else {
				if (GetWeightEx() >= INEDIA_INFECTEDAI_MIN_DAMAGE_WEIGHT) {
					string projectileMeleeHeavyAmmo = GetGame().ConfigGetTextOut("CfgVehicles " + GetType() + " MeleeModes Heavy ammo");
					if (projectileMeleeHeavyAmmo) {
						chancePercent = Math.Clamp(100 * GetGame().ConfigGetFloat("CfgAmmo " + projectileMeleeHeavyAmmo + " DamageApplied bleedThreshold"), 0, 100);
					}
				}
			}
		}

		if (chancePercent <= 0) {
			return 0;
		} else {
			return chancePercent * InediaInfectedAI_ThrowingProjectileGetThrowerDamageMultiplier();
		}
	}

	float InediaInfectedAI_ThrowingProjectileGetThrowerDamageMultiplier()
	{
		float multiplier = 1;

		if (InediaInfectedAI_IsThrowingProjectile() && m_InediaInfectedAI_ThrowingProjectile.m_ThrowerEntity) {
			if (m_InediaInfectedAI_ThrowingProjectile.m_ThrowerEntity.IsInherited(ZombieBase)) {
				ZombieBase zmbThrower = ZombieBase.Cast(m_InediaInfectedAI_ThrowingProjectile.m_ThrowerEntity);
				if (zmbThrower && zmbThrower.m_InediaInfectedAI_ConfigZombie) {
					multiplier = zmbThrower.m_InediaInfectedAI_ConfigZombie.ThrowingProjectilesHandlerDamageMultiplier;
				}
			} else if (m_InediaInfectedAI_ThrowingProjectile.m_ThrowerEntity.IsInherited(PlayerBase)) {
				PlayerBase playerThrower = PlayerBase.Cast(m_InediaInfectedAI_ThrowingProjectile.m_ThrowerEntity);
				if (playerThrower && playerThrower.m_InediaInfectedAI_ConfigZombie) {
					multiplier = playerThrower.m_InediaInfectedAI_ConfigZombie.ThrowingProjectilesHandlerDamageMultiplier;
				}
			}
		}

		return multiplier;
	}

	void InediaInfectedAI_ThrowingProjectileDestroy()
	{
		SetHealth("", "", GetMaxHealth("", "") * GameConstants.DAMAGE_RUINED_VALUE);
	}

	void InediaInfectedAI_ThrowingProjectileActivateGrenade()
	{
		if (this.IsInherited(Grenade_Base)) {
			Grenade_Base grenadeProjectile = Grenade_Base.Cast(this);
			if (grenadeProjectile) {
				grenadeProjectile.ActivateImmediate();
			}
		}
	}

	void InediaInfectedAI_ThrowingProjectileStartWorking()
	{
		if (GetCompEM() && !GetCompEM().IsWorking()) {
			GetCompEM().SwitchOn();
		}
	}
	
// DEEP WOUNDS MEDICINE LOGIC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override void SetActions()
	{
		super.SetActions();
		
		InediaInfectedAI_SetActions();
	}
	
	void InediaInfectedAI_SetActions()
	{
		// Arms self actions
		if (m_InediaInfectedAI_IsDeepWoundBandageItem == 1) AddAction(InediaInfectedAI_ActionDeepWoundBandageArmsSelf);
		if (m_InediaInfectedAI_IsExtractBulletItem == 1) AddAction(InediaInfectedAI_ActionExtractBulletArmsSelf);
		if (m_InediaInfectedAI_IsDeepWoundClosureItem == 1) AddAction(InediaInfectedAI_ActionDeepWoundClosureArmsSelf);
		if (m_InediaInfectedAI_IsFractureStabilizationItem == 1) AddAction(InediaInfectedAI_ActionFractureStabilizationArmsSelf);
		
		// Legs self actions
		if (m_InediaInfectedAI_IsDeepWoundBandageItem == 1) AddAction(InediaInfectedAI_ActionDeepWoundBandageLegsSelf);
		if (m_InediaInfectedAI_IsExtractBulletItem == 1) AddAction(InediaInfectedAI_ActionExtractBulletLegsSelf);
		if (m_InediaInfectedAI_IsDeepWoundClosureItem == 1) AddAction(InediaInfectedAI_ActionDeepWoundClosureLegsSelf);
		if (m_InediaInfectedAI_IsFractureStabilizationItem == 1) AddAction(InediaInfectedAI_ActionFractureStabilizationLegsSelf);
		
		// Torso self actions
		if (m_InediaInfectedAI_IsDeepWoundBandageItem == 1) AddAction(InediaInfectedAI_ActionDeepWoundBandageTorsoSelf);
		if (m_InediaInfectedAI_IsExtractBulletItem == 1) AddAction(InediaInfectedAI_ActionExtractBulletTorsoSelf);
		if (m_InediaInfectedAI_IsInternalBleedingStoppingItem == 1) AddAction(InediaInfectedAI_ActionInternalBleedingStopSelf);
		if (m_InediaInfectedAI_IsDeepWoundClosureItem == 1) AddAction(InediaInfectedAI_ActionDeepWoundClosureTorsoSelf);
		if (m_InediaInfectedAI_IsFractureStabilizationItem == 1) AddAction(InediaInfectedAI_ActionFractureStabilizationTorsoSelf);
		
		// Head self actions
		if (m_InediaInfectedAI_IsDeepWoundBandageItem == 1) AddAction(InediaInfectedAI_ActionDeepWoundBandageHeadSelf);
		if (m_InediaInfectedAI_IsExtractBulletItem == 1) AddAction(InediaInfectedAI_ActionExtractBulletHeadSelf);
		if (m_InediaInfectedAI_IsDeepWoundClosureItem == 1) AddAction(InediaInfectedAI_ActionDeepWoundClosureHeadSelf);
		if (m_InediaInfectedAI_IsFractureStabilizationItem == 1) AddAction(InediaInfectedAI_ActionFractureStabilizationHeadSelf);
		
		// Usable item self
		if (m_InediaInfectedAI_IsUsableItem == 1) AddAction(InediaInfectedAI_ActionUseSelf);
		
		// Cut closure item self
		if (m_InediaInfectedAI_IsCutClosureItem == 1) AddAction(InediaInfectedAI_ActionCutClosureSelf);
		
		// Arms target actions
		if (m_InediaInfectedAI_IsDeepWoundBandageItem == 1) AddAction(InediaInfectedAI_ActionDeepWoundBandageArmsTarget);
		if (m_InediaInfectedAI_IsExtractBulletItem == 1) AddAction(InediaInfectedAI_ActionExtractBulletArmsTarget);
		if (m_InediaInfectedAI_IsDeepWoundClosureItem == 1) AddAction(InediaInfectedAI_ActionDeepWoundClosureArmsTarget);
		if (m_InediaInfectedAI_IsFractureStabilizationItem == 1) AddAction(InediaInfectedAI_ActionFractureStabilizationArmsTarget);
		
		// Legs target actions
		if (m_InediaInfectedAI_IsDeepWoundBandageItem == 1) AddAction(InediaInfectedAI_ActionDeepWoundBandageLegsTarget);
		if (m_InediaInfectedAI_IsExtractBulletItem == 1) AddAction(InediaInfectedAI_ActionExtractBulletLegsTarget);
		if (m_InediaInfectedAI_IsDeepWoundClosureItem == 1) AddAction(InediaInfectedAI_ActionDeepWoundClosureLegsTarget);
		if (m_InediaInfectedAI_IsFractureStabilizationItem == 1) AddAction(InediaInfectedAI_ActionFractureStabilizationLegsTarget);
		
		// Torso target actions
		if (m_InediaInfectedAI_IsDeepWoundBandageItem == 1) AddAction(InediaInfectedAI_ActionDeepWoundBandageTorsoTarget);
		if (m_InediaInfectedAI_IsExtractBulletItem == 1) AddAction(InediaInfectedAI_ActionExtractBulletTorsoTarget);
		if (m_InediaInfectedAI_IsInternalBleedingStoppingItem == 1) AddAction(InediaInfectedAI_ActionInternalBleedingStopTarget);
		if (m_InediaInfectedAI_IsDeepWoundClosureItem == 1) AddAction(InediaInfectedAI_ActionDeepWoundClosureTorsoTarget);
		if (m_InediaInfectedAI_IsFractureStabilizationItem == 1) AddAction(InediaInfectedAI_ActionFractureStabilizationTorsoTarget);
		
		// Head target actions
		if (m_InediaInfectedAI_IsDeepWoundBandageItem == 1) AddAction(InediaInfectedAI_ActionDeepWoundBandageHeadTarget);
		if (m_InediaInfectedAI_IsExtractBulletItem == 1) AddAction(InediaInfectedAI_ActionExtractBulletHeadTarget);
		if (m_InediaInfectedAI_IsDeepWoundClosureItem == 1) AddAction(InediaInfectedAI_ActionDeepWoundClosureHeadTarget);
		if (m_InediaInfectedAI_IsFractureStabilizationItem == 1) AddAction(InediaInfectedAI_ActionFractureStabilizationHeadTarget);		
		
		// Usable item target
		if (m_InediaInfectedAI_IsUsableItem == 1) AddAction(InediaInfectedAI_ActionUseTarget);
		
		// Cut closure item target
		if (m_InediaInfectedAI_IsCutClosureItem == 1) AddAction(InediaInfectedAI_ActionCutClosureTarget);
	}
	
	override bool CanBeDisinfected()
	{
		if (m_InediaInfectedAI_IsDeepWoundClosureItem == 1 || m_InediaInfectedAI_IsExtractBulletItem == 1 || m_InediaInfectedAI_IsInternalBleedingStoppingItem == 1 || m_InediaInfectedAI_IsDeepWoundBandageItem == 1 || m_InediaInfectedAI_IsCutClosureItem == 1) {
			return true;
		}
		
		return super.CanBeDisinfected();
	}

// AUXILIARY METHODS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	string m_InediaInfectedAI_SoundType = "";
	string InediaInfectedAI_GetSoundType()
	{
		if (m_InediaInfectedAI_SoundType == "") {
			string attType = "None";
	
			if (ConfigIsExisting("soundAttType")) {
				attType = ConfigGetString("soundAttType");
			}
			
			m_InediaInfectedAI_SoundType = attType;
		}
		
		return m_InediaInfectedAI_SoundType;
	}
	
	string InediaInfectedAI_GetDrugInfoClient()
	{
#ifndef SERVER		
		string infoText = "";
		
		array<string> cfgPrefixes = new array<string>;
		cfgPrefixes.Insert("CfgVehicles " + GetType());
		
		if (IsLiquidContainer()) {
			int liquidId = GetLiquidType();
			if (liquidId > 0) {
				string liquidClassName = Liquid.GetLiquidClassname(liquidId);
				if (liquidClassName) {
					cfgPrefixes.Insert("cfgLiquidDefinitions " + liquidClassName);
				}
			}
		}
		
		foreach (string cfgPrefix: cfgPrefixes) {
			infoText += ItemBase.InediaInfectedAI_GetDrugInfoByCfgPrefixClient(cfgPrefix);
		}

		
		return infoText;
#else
		return "";
#endif
	}
	
	static string InediaInfectedAI_GetDrugInfoByCfgPrefixClient(string cfgPrefix)
	{
#ifndef SERVER
		if (!cfgPrefix) {
			return "";
		}
		
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player || !player.InediaInfectedAI_GetPainManager().PainSystemIsActive() || !player.InediaInfectedAI_GetPainManager().m_ShowMedicationInfo) {
			return "";
		}
		
		string infoText = "";
		
		string secondsFormat = "s";
		if (cfgPrefix.Contains("cfgLiquidDefinitions")) {
			secondsFormat = "s/1L";
		}
		
        if (GetGame().ConfigGetInt(cfgPrefix + " InediaPain ShowTips") == 1) {
            if (GetGame().ConfigGetInt(cfgPrefix + " InediaPain IsPainkillerLightItem") == 1) {
                float painkillerLightDurationSeconds = GetGame().ConfigGetInt(cfgPrefix + " InediaPain PainkillerLightItemDurationSeconds");
                infoText += " [#STR_inediainfectedai_painkiller_light / " + painkillerLightDurationSeconds + secondsFormat + "]";
            }
            if (GetGame().ConfigGetInt(cfgPrefix + " InediaPain IsPainkillerMediumItem") == 1) {
                float painkillerMediumDurationSeconds = GetGame().ConfigGetInt(cfgPrefix + " InediaPain PainkillerMediumItemDurationSeconds");
                infoText += " [#STR_inediainfectedai_painkiller_medium / " + painkillerMediumDurationSeconds + secondsFormat + "]";
            }
            if (GetGame().ConfigGetInt(cfgPrefix + " InediaPain IsPainkillerStrongItem") == 1) {
                float painkillerStrongDurationSeconds = GetGame().ConfigGetInt(cfgPrefix + " InediaPain PainkillerStrongItemDurationSeconds");
                infoText += " [#STR_inediainfectedai_painkiller_strong / " + painkillerStrongDurationSeconds + secondsFormat + "]";
            }

            if (player.InediaInfectedAI_GetPainManager().LimbsDeepWoundSystemIsActive() && GetGame().ConfigGetInt(cfgPrefix + " InediaPain IsDeepWoundRegenItem") == 1) {
                float deepWoundRegenMultiplier = GetGame().ConfigGetFloat(cfgPrefix + " InediaPain DeepWoundRegenItemRegenMultiplier");
                if (deepWoundRegenMultiplier < 1) deepWoundRegenMultiplier = 1;
                float deepWoundRegenDurationSeconds = GetGame().ConfigGetInt(cfgPrefix + " InediaPain DeepWoundRegenItemDurationSeconds");
                
                if (deepWoundRegenMultiplier > 1) {
                    infoText += " [#STR_inediainfectedai_regeneration_wound: x" + deepWoundRegenMultiplier + " / " + deepWoundRegenDurationSeconds + secondsFormat + "]";
                }
            }
			if (GetGame().ConfigGetInt(cfgPrefix + " InediaPain IsVomitReliefItem") == 1) {
                float vomitRelieDurationSeconds = GetGame().ConfigGetInt(cfgPrefix + " InediaPain VomitReliefItemDurationSeconds");
                infoText += " [#STR_inediainfectedai_prevents_vomiting / " + vomitRelieDurationSeconds + secondsFormat + "]";
            }
            if (player.InediaInfectedAI_GetPainManager().LimbsBreakSystemIsActive() && GetGame().ConfigGetInt(cfgPrefix + " InediaPain IsBoneRegenItem") == 1) {
                float boneRegenMultiplier = GetGame().ConfigGetFloat(cfgPrefix + " InediaPain BoneRegenItemRegenMultiplier");
                if (boneRegenMultiplier < 1) boneRegenMultiplier = 1;
                float boneRegenDurationSeconds = GetGame().ConfigGetInt(cfgPrefix + " InediaPain BoneRegenItemDurationSeconds");
                
                if (boneRegenMultiplier > 1) {
                    infoText += " [#STR_inediainfectedai_regeneration_bone: x" + boneRegenMultiplier + " / " + boneRegenDurationSeconds + secondsFormat + "]";
                }
            }
            if (GetGame().ConfigGetInt(cfgPrefix + " InediaPain IsBloodClottingItem") == 1) {
                float bloodClottingMultiplier = GetGame().ConfigGetFloat(cfgPrefix + " InediaPain BloodClottingItemClottingMultiplier");
                if (bloodClottingMultiplier < 1) bloodClottingMultiplier = 1;
                float bloodClottingRegenDurationSeconds = GetGame().ConfigGetInt(cfgPrefix + " InediaPain BloodClottingItemDurationSeconds");
                
                infoText += " [#STR_inediainfectedai_bleeding_reduction: x" + bloodClottingMultiplier + " / " + bloodClottingRegenDurationSeconds + secondsFormat + "]";
            }
            if (GetGame().ConfigGetInt(cfgPrefix + " InediaPain IsBloodRegenItem") == 1) {
                float bloodRegenMultiplier = GetGame().ConfigGetFloat(cfgPrefix + " InediaPain BloodRegenItemRegenMultiplier");
                if (bloodRegenMultiplier < 1) bloodRegenMultiplier = 1;
                float bloodRegenDurationSeconds = GetGame().ConfigGetInt(cfgPrefix + " InediaPain BloodRegenItemDurationSeconds");
                
                if (bloodRegenMultiplier > 1) {
                    infoText += " [#STR_inediainfectedai_regeneration_blood: x" + bloodRegenMultiplier + " / " + bloodRegenDurationSeconds + secondsFormat + "]";
                }
            }
            if (GetGame().ConfigGetInt(cfgPrefix + " InediaPain IsHealthRegenItem") == 1) {
                float healthRegenMultiplier = GetGame().ConfigGetFloat(cfgPrefix + " InediaPain HealthRegenItemRegenMultiplier");
                if (healthRegenMultiplier < 1) healthRegenMultiplier = 1;
                float healthRegenDurationSeconds = GetGame().ConfigGetInt(cfgPrefix + " InediaPain HealthRegenItemDurationSeconds");
                if (healthRegenDurationSeconds < 0) healthRegenDurationSeconds = 0;
                
                if (healthRegenMultiplier > 1) {
                    infoText += " [#STR_inediainfectedai_regeneration_health: x" + healthRegenMultiplier + " / " + healthRegenDurationSeconds + secondsFormat + "]";
                }
            }
        }
		
		return infoText;
#else
		return "";
#endif
	}
	
	bool m_InediaInfectedAI_DestroyItemInPlayerInventoryWhenDisconnected = false;
	void InediaInfectedAI_DestroyItemInPlayerInventoryWhenDisconnected(bool pDestroy)
	{
		if (m_InediaInfectedAI_DestroyItemInPlayerInventoryWhenDisconnected != pDestroy) {
			m_InediaInfectedAI_DestroyItemInPlayerInventoryWhenDisconnected = pDestroy;
		}
	}

	void InediaInfectedAI_SetQuantityPercent(float percent)
	{
		if (percent <= 0) {
			return;
		}

		int inStackQuantity = 0;

		Magazine magazine = Magazine.Cast(this);
		if (magazine) {
			inStackQuantity = magazine.GetAmmoCount();
		} else {
			inStackQuantity = GetQuantityMax();
		}

		int newQuantity = (int) Math.Round((float) inStackQuantity * percent / 100);
		if (newQuantity <= 0) {
			newQuantity = 1;
		}

		if (magazine) {
			magazine.ServerSetAmmoCount(newQuantity);
		} else {
			SetQuantity(newQuantity, false);
		}
	}
	
	static float m_InediaInfectedAI_ThrowingProjectileGetAmmoHealthDamage = -1;
	static float InediaInfectedAI_ThrowingProjectileGetAmmoHealthDamage()
	{
		if (m_InediaInfectedAI_ThrowingProjectileGetAmmoHealthDamage == -1) {
			m_InediaInfectedAI_ThrowingProjectileGetAmmoHealthDamage = GetGame().ConfigGetFloat("CfgAmmo InediaInfectedAI_ThrowingProjectile DamageApplied Health damage");
			if (m_InediaInfectedAI_ThrowingProjectileGetAmmoHealthDamage <= 0) {
				m_InediaInfectedAI_ThrowingProjectileGetAmmoHealthDamage = 10;
			}
		}
		
		return m_InediaInfectedAI_ThrowingProjectileGetAmmoHealthDamage;
	}
	
	static float m_InediaInfectedAI_ThrowingProjectileGetAmmoShockDamage = -1;
	static float InediaInfectedAI_ThrowingProjectileGetAmmoShockDamage()
	{
		if (m_InediaInfectedAI_ThrowingProjectileGetAmmoShockDamage == -1) {
			m_InediaInfectedAI_ThrowingProjectileGetAmmoShockDamage = GetGame().ConfigGetFloat("CfgAmmo InediaInfectedAI_ThrowingProjectile DamageApplied Shock damage");
			if (m_InediaInfectedAI_ThrowingProjectileGetAmmoShockDamage <= 0) {
				m_InediaInfectedAI_ThrowingProjectileGetAmmoShockDamage = 10;
			}
		}
		
		return m_InediaInfectedAI_ThrowingProjectileGetAmmoShockDamage;
	}
	
	static float m_InediaInfectedAI_ThrowingProjectileGetAmmoBloodDamage = -1;
	static float InediaInfectedAI_ThrowingProjectileGetAmmoBloodDamage()
	{
		if (m_InediaInfectedAI_ThrowingProjectileGetAmmoBloodDamage == -1) {
			m_InediaInfectedAI_ThrowingProjectileGetAmmoBloodDamage = GetGame().ConfigGetFloat("CfgAmmo InediaInfectedAI_ThrowingProjectile DamageApplied Blood damage");
			if (m_InediaInfectedAI_ThrowingProjectileGetAmmoBloodDamage <= 0) {
				m_InediaInfectedAI_ThrowingProjectileGetAmmoBloodDamage = 10;
			}
		}
		
		return m_InediaInfectedAI_ThrowingProjectileGetAmmoBloodDamage;
	}
}