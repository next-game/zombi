/**
 * Created: 2024-03-04
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

class InediaInfectedAI_ThrowingProjectileAtEntityCommand extends InediaInfectedAI_BaseCommand
{
	ref InediaInfectedAI_ThrowingProjectile m_InediaInfectedAI_ThrowingProjectile;
	EntityAI m_InediaInfectedAI_TargetEntity;

	void InediaInfectedAI_SetOptions(InediaInfectedAI_ThrowingProjectile throwingProjectile, EntityAI targetEntity)
	{
		m_InediaInfectedAI_ThrowingProjectile = throwingProjectile;
		m_InediaInfectedAI_TargetEntity = targetEntity;

		if (m_InediaInfectedAI_TargetEntity) {
			InediaInfectedAI_SetOrientationToPosition(m_InediaInfectedAI_TargetEntity.GetPosition());
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

	/*
	override void OnDeactivate()
	{
		//
	}
	*/

	float m_InediaInfectedAI_StopScriptTimer = 10;
	override void PreAnimUpdate(float pDt)
    {
		if (!m_InediaInfectedAI_Infected) {
			SetFlagFinished(true);
			return;
		}

		m_InediaInfectedAI_StopScriptTimer -= pDt;
		if (m_InediaInfectedAI_StopScriptTimer <= 0) {
			SetFlagFinished(true);
			return;
		}

		InediaInfectedAI_OrientationToPositionHandler(pDt);
		InediaInfectedAI_ThrowHandler(pDt);
    }

	float m_InediaInfectedAI_ThrowHandlerTimer = 0;
	bool m_InediaInfectedAI_InMovement = true;
	protected void InediaInfectedAI_ThrowHandler(float pDt)
	{
		m_InediaInfectedAI_ThrowHandlerTimer -= pDt;
		if (m_InediaInfectedAI_ThrowHandlerTimer > 0) return;
		m_InediaInfectedAI_ThrowHandlerTimer = 0.25;

		if (!m_InediaInfectedAI_ThrowingProjectile || !m_InediaInfectedAI_TargetEntity) {
			SetFlagFinished(true);
			return;
		}
		
		// Waiting for the completion of other animations and turn
		if (m_InediaInfectedAI_Infected.InediaInfectedAI_IsInTurn()) {
			return;
		}

		// Stop movement
		if (m_InediaInfectedAI_InMovement) {
			m_InediaInfectedAI_Infected.m_InediaInfectedAI_InfectedAnimationST.SetMovementSpeed(this, 0);
			m_InediaInfectedAI_InMovement = false;
		}

		// Waiting for zombie to turn in jump direction
		if (InediaInfectedAI_InOrientationToPosition()) {
			return;
		}

		// Checking if we can still throw the ammo
		if (!m_InediaInfectedAI_Infected.InediaInfectedAI_CanThrowProjectileAtEntity(m_InediaInfectedAI_ThrowingProjectile, m_InediaInfectedAI_TargetEntity) || !m_InediaInfectedAI_Infected.InediaInfectedAI_CanSeeEntity(m_InediaInfectedAI_TargetEntity, 0.3 * vector.Up)) {
			SetFlagFinished(true);
			return;
		}

		if (!m_InediaInfectedAI_ThrowingProjectile.m_ItemClasses || m_InediaInfectedAI_ThrowingProjectile.m_ItemClasses.Count() == 0) {
			SetFlagFinished(true);
			return;
		}

		// Throw
		vector throwFromPosition = m_InediaInfectedAI_Infected.InediaInfectedAI_GetHeadPosition();
		vector throwToPosition;
		
		if (m_InediaInfectedAI_TargetEntity.IsInherited(PlayerBase)) {
			PlayerBase targetPlayerPb = PlayerBase.Cast(m_InediaInfectedAI_TargetEntity);
			if (targetPlayerPb) {
				vector playerHorseChestPosition = InediaInfectedAI_DayzHorseCompatibility.GetPlayerHorseChestPosition(targetPlayerPb);
				if (playerHorseChestPosition) {
					throwToPosition = playerHorseChestPosition;
				} else {
					throwToPosition = MiscGameplayFunctions.InediaInfectedAI_GetCreatureHeadPosition(targetPlayerPb);
				}
			}
		} else {
			throwToPosition = MiscGameplayFunctions.InediaInfectedAI_GetCreatureHeadPosition(m_InediaInfectedAI_TargetEntity);
		}

		if (!throwToPosition) {
			SetFlagFinished(true);
			return;
		}

		float projectileInitialSpeed = m_InediaInfectedAI_ThrowingProjectile.m_InitialSpeed;

		// Apply accuracy
		if (m_InediaInfectedAI_ThrowingProjectile.m_AccuracySpread > 0) {
			float toPositionDistance = vector.Distance(throwFromPosition, throwToPosition);
			throwToPosition = throwToPosition + vector.RandomDir() * toPositionDistance * m_InediaInfectedAI_ThrowingProjectile.m_AccuracySpread;
		}

		float throwAngle = MiscGameplayFunctions.InediaInfectedAI_CalculateThrowAngle(throwFromPosition, throwToPosition, projectileInitialSpeed);

		if (throwAngle > -90 && throwAngle < 90) {
			int throwAnimationType = 1;
			int throwAnimationSubType = 1;
			if (throwAngle > 15) {
				throwAnimationType = 1;
				throwAnimationSubType = 0;
			} else if (throwAngle < -15) {
				throwAnimationType = 1;
				throwAnimationSubType = 2;
			}
			m_InediaInfectedAI_Infected.InediaInfectedAI_SetIsInAnimationForSeconds(2.5);
			m_InediaInfectedAI_Infected.m_InediaInfectedAI_InfectedAnimationST.ExecAttack(this, throwAnimationType, throwAnimationSubType);

			vector horizontalTargetDirection = vector.Direction(throwFromPosition, Vector(throwToPosition[0], throwFromPosition[1], throwToPosition[2])).Normalized();
			vector throwDirection = vector.RotateAroundZeroDeg(horizontalTargetDirection, horizontalTargetDirection.Perpend(), throwAngle);

			ItemBase throwingItem = ItemBase.Cast(GetGame().CreateObject(m_InediaInfectedAI_ThrowingProjectile.m_ItemClasses.GetRandomElement(), throwFromPosition));
			if (throwingItem) {
				// Throw projectile
				throwingItem.ThrowPhysically(null, vector.Zero);
				
				// Set velocity
				SetVelocity(throwingItem, throwDirection * projectileInitialSpeed);
				
				// Set torque impulse
				vector spinAxis = MiscGameplayFunctions.InediaInfectedAI_VectorCross(vector.Up, throwDirection);
				if (spinAxis.LengthSq() < 0.001) {
					spinAxis = MiscGameplayFunctions.InediaInfectedAI_VectorCross(vector.Aside, throwDirection);
				}
				
				spinAxis = spinAxis.Normalized();
				vector randomOffset = vector.RandomDir() * 0.1;
				spinAxis = (spinAxis + randomOffset).Normalized();
				float throwingItemWeightKg = Math.Clamp(throwingItem.GetWeightEx(), 500, 50000) / 1000;
				float torqueStrength = (12 / Math.Pow(throwingItemWeightKg, 0.1)) * Math.RandomFloatInclusive(0.8, 1.2);
				dBodyApplyTorqueImpulse(throwingItem, spinAxis * torqueStrength);

				m_InediaInfectedAI_ThrowingProjectile.m_ThrowerEntity = m_InediaInfectedAI_Infected;
				m_InediaInfectedAI_ThrowingProjectile.m_TargetEntity = m_InediaInfectedAI_TargetEntity;
				throwingItem.InediaInfectedAI_SetThrowingProjectile(m_InediaInfectedAI_ThrowingProjectile);

				if (m_InediaInfectedAI_ThrowingProjectile.m_ThrowConditionAmountOfProjectiles > 0) {
					m_InediaInfectedAI_ThrowingProjectile.m_ThrowConditionAmountOfProjectiles--;
				}
				m_InediaInfectedAI_ThrowingProjectile.m_TimeOfLastUse = GetGame().GetTickTime();

				// Delete projectile logic
				if (m_InediaInfectedAI_ThrowingProjectile.m_DeleteProjectileAfterSeconds > 0) {
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(throwingItem.DeleteSafe, m_InediaInfectedAI_ThrowingProjectile.m_DeleteProjectileAfterSeconds * 1000, false);
					throwingItem.InediaInfectedAI_DestroyItemInPlayerInventoryWhenDisconnected(true);
				}

				// Destroy projectile logic
				if (m_InediaInfectedAI_ThrowingProjectile.m_DestroyProjectileAfterSeconds > 0) {
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(throwingItem.InediaInfectedAI_ThrowingProjectileDestroy, m_InediaInfectedAI_ThrowingProjectile.m_DestroyProjectileAfterSeconds * 1000, false);
					throwingItem.InediaInfectedAI_DestroyItemInPlayerInventoryWhenDisconnected(true);
				}

				// Activate grenade logic
				if (m_InediaInfectedAI_ThrowingProjectile.m_ActivateGrenadeAfterSeconds > 0) {
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(throwingItem.InediaInfectedAI_ThrowingProjectileActivateGrenade, m_InediaInfectedAI_ThrowingProjectile.m_ActivateGrenadeAfterSeconds * 1000, false);
					throwingItem.InediaInfectedAI_DestroyItemInPlayerInventoryWhenDisconnected(true);
				}

				// Start working logic
				if (m_InediaInfectedAI_ThrowingProjectile.m_StartWorkingAfterSeconds > 0) {
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(throwingItem.InediaInfectedAI_ThrowingProjectileStartWorking, m_InediaInfectedAI_ThrowingProjectile.m_StartWorkingAfterSeconds * 1000, false);
					throwingItem.InediaInfectedAI_DestroyItemInPlayerInventoryWhenDisconnected(true);
				}
			}
		} else {
			SetFlagFinished(true);
			return;
		}

// VFg248/lIqRAYRe9Iy1qpE13WDR5K0dyUFJIMzRvd0YrcDJSTUdJRGRZdmxRSmQ4Yjg5LzNhemJnWlA2QkRpTGUySXZSQ214RzR3enMrcCtXTi9zMFlJcVNoNzd4bldTTlR4bnRCN0VsWTRCYVVPcnRyc2dKNk04TVUxd3ZxN1M5ZDZMSFRGRW16MzIvd0pLTHVCNXpSY0p2S1NvcDUxNzFxTGR1Z2lCQ2tDa1VPN1hodmxlRmtXQXg0UUJnN2gva2lMOHpQa2dkNFFFQmp4SHplYUFiOXFDNGhxb3ZmbFFLN2hSMnJMRTBlK3AxRDN0dGtRc3lFaVE1UjFYWU01b2xicmxkS1FEV1NjcnFZT2xQMUZOeHZRSm9OVHRYUFE1Q2FlZnNRPT0=

		m_InediaInfectedAI_ThrowHandlerTimer = 100500;
		m_InediaInfectedAI_StopScriptTimer = 1;
	}

	override bool PostPhysUpdate(float pDt)
    {
        return true;
    }
}