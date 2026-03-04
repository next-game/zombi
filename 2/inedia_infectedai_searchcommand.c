/**
 * Created: 2023-09-01
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

class InediaInfectedAI_SearchCommand extends InediaInfectedAI_BaseCommand
{
	ref PGFilter m_InediaInfectedAI_PGFilter;

	EntityAI m_InediaInfectedAI_Target;
	EntityAI m_InediaInfectedAI_LastTarget;
	
	float m_InertiaSpeedMultiplier = 1;

	void InediaInfectedAI_SearchCommand(DayZInfected pInfected)
    {
		if (!GetGame().GetWorld().GetAIWorld()) {
			SetFlagFinished(true);
			return;
		}
		
		m_InediaInfectedAI_PGFilter = new PGFilter();
		int inFlags = PGPolyFlags.WALK | PGPolyFlags.DOOR;
		//int inFlags = PGPolyFlags.WALK | PGPolyFlags.DOOR | PGPolyFlags.JUMP_OVER | PGPolyFlags.CLIMB;
		int excludeFlags = PGPolyFlags.NONE;
		int exclusiveFlags = PGPolyFlags.NONE;
		m_InediaInfectedAI_PGFilter.SetFlags(inFlags, excludeFlags, exclusiveFlags);

		m_InediaInfectedAI_PGFilter.SetCost(PGAreaType.NONE, 1);
		m_InediaInfectedAI_PGFilter.SetCost(PGAreaType.DOOR_OPENED, 0);
		m_InediaInfectedAI_PGFilter.SetCost(PGAreaType.TREE, 0);
    }

	void InediaInfectedAI_StopScript()
	{
		SetFlagFinished(true);
	}

// eEBThWJ3PUl86D2LOy2X201sNURaSzVJNlZzcFRqMGtrbUlRNVE5TTMrRVNOU2c1NThzSlVlNmtZOWJSS0tGaWZWYkh5aWw2alZueVV2ajV5VENUdkw2MFBLcFdSMlcxcWRvL0tXbElSWDE4bDJFQjA2NXNxM09WR1N6YURpcy9zSTNyWU5nbnNDLzk1bmIwUGIyeFRHMXhxdTRpSlVrQ1lwTGdYRHBiMmJHalp4d2NGT2VRcTI4ai9TdkpJT1Y3OWJYYXhRN2M0MFdmaWh2aVRqb3FVYTkvdDRYbEdZcWE1QmovaTlEYy8wVG5VYW01WkVPMExYRkRUaW5qdFpSTTQwdWVBWGM4dEcrOE9LdU9rV0plZHg4MFQ1aC80eFNxZG40eUV3PT0=

	override void OnActivate()
    {
		m_InediaInfectedAI_Target = m_InediaInfectedAI_Infected.GetInputController().GetTargetEntity();
		m_InediaInfectedAI_LastTarget = m_InediaInfectedAI_Target;
    }

	override bool PostPhysUpdate(float pDt)
    {
        return true;
    }

	override void PreAnimUpdate(float pDt)
    {
		if (!m_InediaInfectedAI_Infected) {
			SetFlagFinished(true);
			return;
		}

		InediaInfectedAI_TimersHandler(pDt);
		InediaInfectedAI_OrientationToPositionHandler(pDt);
		InediaInfectedAI_BreakingDoorHandler(pDt);
		InediaInfectedAI_VariationStanceHandler(pDt);
		InediaInfectedAI_MainHandler(pDt);
		InediaInfectedAI_LookAtTargetHandler(pDt);
    }
	
	protected void InediaInfectedAI_TimersHandler(float pDt)
	{
		if (m_InediaInfectedAI_SearchModeAlertedSpeedTimer > 0) {
			m_InediaInfectedAI_SearchModeAlertedSpeedTimer -= pDt;
		}
		
		if (m_InediaInfectedAI_LongVisionChaseSpeedTimer > 0) {
			m_InediaInfectedAI_LongVisionChaseSpeedTimer -= pDt;
		}
	}

	vector m_InediaInfectedAI_SearchGoal;
	vector m_InediaInfectedAI_LastSearchSpherePosition;
	ZombieBase m_InediaInfectedAI_NearestAliveZmbInPath;

	float m_InediaInfectedAI_MainTimer = 0;
	protected void InediaInfectedAI_MainHandler(float pDt)
	{	
		m_InediaInfectedAI_MainTimer -= pDt;
		if (m_InediaInfectedAI_MainTimer > 0) return;
		m_InediaInfectedAI_MainTimer = 0.2;

		if (!m_InediaInfectedAI_Infected.IsAlive()) {
			InediaInfectedAI_StopScript();
			return;
		}

		// If there is no surface under the infected, pause the script.
		// Otherwise, the infected will pass through doors due to the specifics of the vanilla mechanics,
		// which push them onto each other's heads during collisions.
		// When they are higher than the usual level, they lose collision with doors and walls and pass through them.
		if (!m_InediaInfectedAI_Infected.InediaInfectedAI_HasSurface()) {
			m_InediaInfectedAI_Infected.InediaInfectedAI_StopSearchModeForSeconds(3, true);
			return;
		}
		
		m_InediaInfectedAI_NearestAliveZmbInPath = InediaInfectedAI_GetNearestAliveZombieInPath();
		
		// If another infected is in the way, disable the jumping functionality until the infected moves out of the other infected's path.
		// This is to avoid engine collisions during these jumps and prevent infected from passing through walls.
		if (m_InediaInfectedAI_NearestAliveZmbInPath) {
			m_InediaInfectedAI_Infected.InediaInfectedAI_DisableUpJumpHandlerForSeconds(5);
			m_InediaInfectedAI_Infected.InediaInfectedAI_DisableDownJumpHandlerForSeconds(5);
			m_InediaInfectedAI_Infected.InediaInfectedAI_DisableStuckJumpHandlerForSeconds(5);
		}

		if (m_InediaInfectedAI_Infected.m_InediaInfectedAI_MindState == DayZInfectedConstants.MINDSTATE_CHASE || m_InediaInfectedAI_Infected.m_InediaInfectedAI_MindState == DayZInfectedConstants.MINDSTATE_FIGHT) {
			InediaInfectedAI_ChaseHandler();
		} else {
			InediaInfectedAI_SearchHandler();
		}
	}

	int m_ProblemsInReachingSearchGoalCounter = 0;
	protected void InediaInfectedAI_SearchHandler()
	{
		bool needTurn = false;
		
		vector zmbPos = m_InediaInfectedAI_Infected.GetPosition();
		
		if (m_InediaInfectedAI_SearchGoal) {
			float distanceToSearchGoalVertical = Math.AbsFloat(zmbPos[1] - m_InediaInfectedAI_SearchGoal[1]);
			float distanceToSearchGoalHorizontalSq = vector.DistanceSq(zmbPos, Vector(m_InediaInfectedAI_SearchGoal[0], zmbPos[1], m_InediaInfectedAI_SearchGoal[2]));
			
			if (distanceToSearchGoalHorizontalSq <= 1 * 1 && distanceToSearchGoalVertical <= 2 && !InediaInfectedAI_InBreakingDoor()) {
				InediaInfectedAI_ChangeSearchGoal();
				needTurn = true;
			} else if (distanceToSearchGoalHorizontalSq <= 1 * 1 && distanceToSearchGoalVertical <= 4 && !InediaInfectedAI_InBreakingDoor()) {
				// If the infected cannot reach the search goal within 5 seconds (this is possible because of a bad navmesh) - change the search goal.
				// If the goal is not changed in such situations, the infected  will get stuck and keep spinning around one point.
				m_ProblemsInReachingSearchGoalCounter++;
				if (m_ProblemsInReachingSearchGoalCounter >= 20) {
					InediaInfectedAI_ChangeSearchGoal();
					needTurn = true;
				}
			} else {
				m_ProblemsInReachingSearchGoalCounter = 0;
			}
			
			// This multiplier is applied to the movement speed to slow down the infected as they approach to search goal, creating an inertia effect.
			if (distanceToSearchGoalHorizontalSq <= 3 * 3 && distanceToSearchGoalVertical <= 2) {
				m_InertiaSpeedMultiplier = Math.Clamp(Math.Sqrt(distanceToSearchGoalHorizontalSq) / 3, 0, 1);
			} else if (m_InertiaSpeedMultiplier != 1) {
				m_InertiaSpeedMultiplier = 1;
			}
		}

		// SearchPosition has changed, change searchGoal
		if (m_InediaInfectedAI_Infected.m_InediaInfectedAI_SearchSpherePosition != m_InediaInfectedAI_LastSearchSpherePosition) {
			InediaInfectedAI_ChangeSearchGoal();
			InediaInfectedAI_StopBreakingDoorHandlerForSeconds(0);

			if (!m_InediaInfectedAI_LastSearchSpherePosition) {
				needTurn = true;
			}

			m_InediaInfectedAI_LastSearchSpherePosition = m_InediaInfectedAI_Infected.m_InediaInfectedAI_SearchSpherePosition;
		}

		// Get first or next search goal
		if (!m_InediaInfectedAI_SearchGoal) {
			m_InediaInfectedAI_SearchGoal = InediaInfectedAI_GetSearchGoalInSphere(m_InediaInfectedAI_Infected.m_InediaInfectedAI_SearchSpherePosition, m_InediaInfectedAI_Infected.m_InediaInfectedAI_SearchSphereRadius);
			if (!m_InediaInfectedAI_SearchGoal) {
				m_InediaInfectedAI_Infected.InediaInfectedAI_StopSearchMode();
				return;
			}

			if (needTurn && !m_InediaInfectedAI_Infected.IsCrawling()) {
				vector currentWaypoint = InediaInfectedAI_GetCurrentWaypoint(m_InediaInfectedAI_SearchGoal);
				if (currentWaypoint) {
					float turnAngleRel = Math.DiffAngle(vector.Direction(zmbPos, currentWaypoint).VectorToAngles()[0], m_InediaInfectedAI_Infected.GetDirection().VectorToAngles()[0]);
					
					m_InediaInfectedAI_Infected.m_InediaInfectedAI_InfectedAnimationST.SetMovementSpeed(this, 0);
					m_InediaInfectedAI_Infected.InediaInfectedAI_ForbidMovementForSeconds(1);
					m_InediaInfectedAI_Infected.InediaInfectedAI_SetIsInTurnForSeconds(2);
					m_InediaInfectedAI_Infected.m_InediaInfectedAI_InfectedAnimationST.ExecTurn(this, 0, turnAngleRel);
					
					m_InediaInfectedAI_MainTimer = 1;
					return;
				}
			}
		}

		// Moving to goal
		InediaInfectedAI_SpeedHandler();
		InediaInfectedAI_GoToSearchGoal(m_InediaInfectedAI_SearchGoal);
		InediaInfectedAI_StuckHandler();
	}

	protected void InediaInfectedAI_ChaseHandler()
	{
		m_InediaInfectedAI_Target = m_InediaInfectedAI_Infected.GetInputController().GetTargetEntity();
		if (!m_InediaInfectedAI_Target) {
			return;
		}
		
		if (m_InertiaSpeedMultiplier != 1) {
			m_InertiaSpeedMultiplier = 1;
		}

		float distanceToTargetSq = vector.DistanceSq(m_InediaInfectedAI_Infected.GetPosition(), m_InediaInfectedAI_Target.GetPosition());
		if (distanceToTargetSq <= 2 * 2) {
			if (MiscGameplayFunctions.InediaInfectedAI_IsObjectOrNothingOnLine(m_InediaInfectedAI_Infected.GetPosition() + vector.Up * 0.9, m_InediaInfectedAI_Target.GetPosition() + vector.Up * 0.9, m_InediaInfectedAI_Target, m_InediaInfectedAI_Infected, 0.15)) {
				m_InediaInfectedAI_Infected.InediaInfectedAI_StopSearchModeForSeconds(3);
				return;
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

		if (m_InediaInfectedAI_Target != m_InediaInfectedAI_LastTarget) {
			m_InediaInfectedAI_LastTarget = m_InediaInfectedAI_Target;
		}

		// In chase mode - search goal is target player
		m_InediaInfectedAI_SearchGoal = m_InediaInfectedAI_Target.GetPosition();

		// Moving to target player
		InediaInfectedAI_SpeedHandler();

		vector searchGoalNavmesh = InediaInfectedAI_GoToSearchGoal(m_InediaInfectedAI_SearchGoal);
		// To prevent situations when a zombie reaches navmesh search goal but does not reach the player, for example he is sitting on a tower.
		// This prevents the zombie from spinning around this point.
		if (searchGoalNavmesh && vector.DistanceSq(m_InediaInfectedAI_Infected.GetPosition(), searchGoalNavmesh) <= 0.5 * 0.5) {
			m_InediaInfectedAI_Infected.InediaInfectedAI_StopSearchModeForSeconds(3);
		}

		InediaInfectedAI_StuckHandler();
	}

	protected void InediaInfectedAI_SpeedHandler()
	{
		if (m_InediaInfectedAI_Infected.m_InediaInfectedAI_ForbidMovement) {
			m_InediaInfectedAI_Infected.m_InediaInfectedAI_InfectedAnimationST.SetMovementSpeed(this, 0);
			return;
		}

		float movementSpeed = m_InediaInfectedAI_Infected.GetInputController().GetMovementSpeed();
		bool unlimitedSpeed = false;

		// If speed handler is enabled
		if (m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.SpeedHandlerIsActive) {
			if (m_InediaInfectedAI_Infected.m_InediaInfectedAI_MindState == DayZInfectedConstants.MINDSTATE_CHASE || m_InediaInfectedAI_Infected.m_InediaInfectedAI_MindState == DayZInfectedConstants.MINDSTATE_FIGHT) {
				// If the zombie is IN CHASE mode
				if (m_InediaInfectedAI_Target) {
					unlimitedSpeed = vector.DistanceSq(m_InediaInfectedAI_Infected.GetPosition(), m_InediaInfectedAI_Target.GetPosition()) < m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.SpeedNoLimitsFromDistanceMeters * m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.SpeedNoLimitsFromDistanceMeters;
					if (!unlimitedSpeed) {
						if (movementSpeed > m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.SpeedLimitInChaseMode) {
							movementSpeed = m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.SpeedLimitInChaseMode;
						} else if (movementSpeed < m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.SpeedMinimumInChaseMode) {
							movementSpeed = m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.SpeedMinimumInChaseMode;
						}
					}
				}
			} else if (m_InediaInfectedAI_Infected.InediaInfectedAI_GetLongVisionTarget()) {
				// If the zombie is IN "Inedia Long Vision" CHASE mode
				movementSpeed = InediaInfectedAI_GetLongVisionChaseSpeed();
				
				if (movementSpeed > m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.SpeedLimitInChaseMode) {
					movementSpeed = m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.SpeedLimitInChaseMode;
				} else if (movementSpeed < m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.SpeedMinimumInChaseMode) {
					movementSpeed = m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.SpeedMinimumInChaseMode;
				}
			} else {
				// If the zombie is IN SEARCH MODE
				if (m_InediaInfectedAI_Infected.m_InediaInfectedAI_MindState > DayZInfectedConstants.MINDSTATE_CALM) {
					movementSpeed = InediaInfectedAI_GetSearchModeAlertedSpeed();
				}
				
				if (movementSpeed > m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.SpeedLimitInSearchMode) {
					movementSpeed = m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.SpeedLimitInSearchMode;
				} else if (movementSpeed < m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.SpeedMinimumInSearchMode) {
					movementSpeed = m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.SpeedMinimumInSearchMode;
				}
			}

			if (!unlimitedSpeed) {
				// If a zombie has lost a lot of blood, we limit its speed to walking
				if (m_InediaInfectedAI_Infected.InediaInfectedAI_IsBloodVolumeVeryLow()) {
					if (movementSpeed > 1) {
						movementSpeed = 1;
					}
				} else if (m_InediaInfectedAI_Infected.InediaInfectedAI_IsBloodVolumeLow()) {
					if (movementSpeed > 2.1) {
						movementSpeed = 2.1;
					}
				}

				// If a zombie's limbs are damaged, limit its speed
				if (!m_InediaInfectedAI_Infected.IsCrawling()) {
					float injurySpeedLimit = m_InediaInfectedAI_Infected.InediaInfectedAI_GetInjurySpeedLimit();
					if (movementSpeed > injurySpeedLimit) {
						movementSpeed = injurySpeedLimit;
					}
				}
			}
		}

		if (movementSpeed < 1) {
			// Speed in search mode cannot be less than "1"
			movementSpeed = 1;
		}
		
		if (movementSpeed > 1 && m_InediaInfectedAI_NearestAliveZmbInPath) {
			movementSpeed = 1;
		}
		
		movementSpeed *= m_InertiaSpeedMultiplier;
		
		if (movementSpeed < 0.6) {
			movementSpeed = 0.6;
		}

		m_InediaInfectedAI_Infected.m_InediaInfectedAI_InfectedAnimationST.SetMovementSpeed(this, movementSpeed);
	}
	
	protected float m_InediaInfectedAI_SearchModeAlertedSpeed = -1;
	protected float m_InediaInfectedAI_SearchModeAlertedSpeedTimer = -1;
	protected float InediaInfectedAI_GetSearchModeAlertedSpeed()
	{
		if (m_InediaInfectedAI_SearchModeAlertedSpeedTimer <= 0) {
			float rand100 = Math.RandomFloatInclusive(0, 100);
			
			if (rand100 <= 50) {
				// 50% light jog
				m_InediaInfectedAI_SearchModeAlertedSpeed = 2.1;
				m_InediaInfectedAI_SearchModeAlertedSpeedTimer = Math.RandomFloatInclusive(15, 25);
			} else if (rand100 <= 85) {
				// 35% fast jog
				m_InediaInfectedAI_SearchModeAlertedSpeed = 2.6;
				m_InediaInfectedAI_SearchModeAlertedSpeedTimer = Math.RandomFloatInclusive(10, 20);
			} else {
				// 15% sprint
				m_InediaInfectedAI_SearchModeAlertedSpeed = 3;
				m_InediaInfectedAI_SearchModeAlertedSpeedTimer = Math.RandomFloatInclusive(5, 15);
			}
		}
		
		return m_InediaInfectedAI_SearchModeAlertedSpeed;
	}
	
	protected float m_InediaInfectedAI_LongVisionChaseSpeed = -1;
	protected float m_InediaInfectedAI_LongVisionChaseSpeedTimer = -1;
	protected float InediaInfectedAI_GetLongVisionChaseSpeed()
	{
		if (m_InediaInfectedAI_LongVisionChaseSpeedTimer <= 0) {
			float rand100 = Math.RandomFloatInclusive(0, 100);
			
			if (rand100 <= 60) {
				// 60% fast jog
				m_InediaInfectedAI_LongVisionChaseSpeed = 2.6;
				m_InediaInfectedAI_LongVisionChaseSpeedTimer = Math.RandomFloatInclusive(15, 25);
			} else if (rand100 <= 85) {
				// 25% light jog
				m_InediaInfectedAI_LongVisionChaseSpeed = 2.1;
				m_InediaInfectedAI_LongVisionChaseSpeedTimer = Math.RandomFloatInclusive(10, 20);
			} else {
				// 15% sprint
				m_InediaInfectedAI_LongVisionChaseSpeed = 3;
				m_InediaInfectedAI_LongVisionChaseSpeedTimer = Math.RandomFloatInclusive(5, 15);
			}
		}
		
		return m_InediaInfectedAI_LongVisionChaseSpeed;
	}

	vector m_InediaInfectedAI_LastPosition;
	int m_InediaInfectedAI_LastPositionEqualsCounter = 0;
	protected void InediaInfectedAI_StuckHandler()
	{
		// If zombie is stuck - switch it to vanilla mode for a while

		if (!m_InediaInfectedAI_Infected.InediaInfectedAI_IsInAnimation() && !m_InediaInfectedAI_Infected.InediaInfectedAI_IsInTurn() && !m_InediaInfectedAI_Infected.m_InediaInfectedAI_ForbidMovement && !InediaInfectedAI_InBreakingDoor() && vector.DistanceSq(m_InediaInfectedAI_Infected.GetPosition(), m_InediaInfectedAI_LastPosition) <= 0.08 * 0.08) {
			m_InediaInfectedAI_LastPositionEqualsCounter++;

			if (m_InediaInfectedAI_LastPositionEqualsCounter >= 10) {
				// Stuck
				m_InediaInfectedAI_Infected.InediaInfectedAI_StopSearchModeForSeconds(10, true);
				return;
			}
		} else {
			m_InediaInfectedAI_LastPositionEqualsCounter = 0;
		}

		m_InediaInfectedAI_LastPosition = m_InediaInfectedAI_Infected.GetPosition();
	}

	bool m_InediaInfectedAI_InBreakingDoor = false;
	Building m_InediaInfectedAI_NearestBuilding;
	int m_InediaInfectedAI_NearestClosedDoorIndex = -1;
	vector m_InediaInfectedAI_NearestClosedDoorPosition = vector.Zero;
	int m_InediaInfectedAI_NearestOpenDoorIndex = -1;
	vector m_InediaInfectedAI_NearestOpenDoorPosition = vector.Zero;
	float m_InediaInfectedAI_BreakingDoorHandlerTimer = 0;
	protected void InediaInfectedAI_BreakingDoorHandler(float pDt)
	{
		if (!m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.BreakingDoorsHandlerIsActive) return;
		
		if (m_InediaInfectedAI_InBreakingDoor) {
			// If the infected is currently in the process of breaking down a door,
			// meaning they are not allowed to move, and someone opens that door,
			// the infected should be allowed to move again.
			if (m_InediaInfectedAI_NearestBuilding && m_InediaInfectedAI_NearestClosedDoorIndex >= 0) {
				if (m_InediaInfectedAI_NearestBuilding.IsDoorOpen(m_InediaInfectedAI_NearestClosedDoorIndex)) {
					m_InediaInfectedAI_Infected.InediaInfectedAI_DisableForbidMovementForSeconds();
				}
			}
		}

		m_InediaInfectedAI_BreakingDoorHandlerTimer -= pDt;
		if (m_InediaInfectedAI_BreakingDoorHandlerTimer > 0) return;
		m_InediaInfectedAI_BreakingDoorHandlerTimer = 1;

		if (m_InediaInfectedAI_Infected.InediaInfectedAI_IsInAnimation()) {
			// Waiting for the end of the animations
			m_InediaInfectedAI_BreakingDoorHandlerTimer = 0;
			return;
		}

		m_InediaInfectedAI_InBreakingDoor = false;
		m_InediaInfectedAI_NearestBuilding = null;
		m_InediaInfectedAI_NearestClosedDoorIndex = -1;
		m_InediaInfectedAI_NearestClosedDoorPosition = vector.Zero;
		m_InediaInfectedAI_NearestOpenDoorIndex = -1;
		m_InediaInfectedAI_NearestOpenDoorPosition = vector.Zero;
		
		bool inaccessibleDoor = false;

		m_InediaInfectedAI_NearestBuilding = InediaInfectedAI_GetBuildingOnZombiePath();
		if (!m_InediaInfectedAI_NearestBuilding) {
			inaccessibleDoor = true;
		} else {
			string lowerBuildingType = m_InediaInfectedAI_NearestBuilding.GetType();
			lowerBuildingType.ToLower();
			if (InediaInfectedAI_IsWreckBuilding(lowerBuildingType)) {
				inaccessibleDoor = true;
			} else {
				InediaInfectedAI_GetNearestDoorsInfo(m_InediaInfectedAI_NearestBuilding, m_InediaInfectedAI_NearestClosedDoorIndex, m_InediaInfectedAI_NearestClosedDoorPosition, m_InediaInfectedAI_NearestOpenDoorIndex, m_InediaInfectedAI_NearestOpenDoorPosition);
				if (m_InediaInfectedAI_NearestClosedDoorIndex < 0 || !m_InediaInfectedAI_NearestClosedDoorPosition) {
					inaccessibleDoor = true;
				}
			}
		}

		if (inaccessibleDoor) {
			m_InediaInfectedAI_NearestBuilding = null;
			m_InediaInfectedAI_NearestClosedDoorIndex = -1;
			m_InediaInfectedAI_NearestClosedDoorPosition = vector.Zero;
			return;
		}

		m_InediaInfectedAI_InBreakingDoor = true;

		InediaInfectedAI_SetOrientationToPosition(m_InediaInfectedAI_NearestClosedDoorPosition);

		float zombieYPosition = m_InediaInfectedAI_Infected.GetPosition()[1];
		float perpendDistanceToDoorSq = vector.DistanceSq(m_InediaInfectedAI_Infected.GetPosition(), Vector(m_InediaInfectedAI_NearestClosedDoorPosition[0], zombieYPosition, m_InediaInfectedAI_NearestClosedDoorPosition[2]));
		if (perpendDistanceToDoorSq < 1.2 * 1.2) {
			m_InediaInfectedAI_Infected.SetPosition(m_InediaInfectedAI_Infected.GetPosition() - (1.2 - Math.Sqrt(perpendDistanceToDoorSq)) * m_InediaInfectedAI_Infected.GetDirection());
		}

		m_InediaInfectedAI_Infected.InediaInfectedAI_SetIsInAnimationForSeconds(1.5);
		array<int> attackTypes = {9, 11, 13};
		m_InediaInfectedAI_Infected.m_InediaInfectedAI_InfectedAnimationST.ExecAttack(this, attackTypes.GetRandomElement());

		// MiscGameplayFunctions.InediaInfectedAI_MakeNoiseForZombie(m_InediaInfectedAI_Infected);

		// Door sounds
		string doorHit;
		string doorOpen;
		string doorDestroy;

		string doorType = InediaInfectedAI_GetDoorType(lowerBuildingType, m_InediaInfectedAI_NearestClosedDoorIndex);
		if (doorType == "metal") {
			doorHit = "InediaInfectedAI_MetalDoorHit";
			doorOpen = "InediaInfectedAI_MetalDoorOpen";
			doorDestroy = "InediaInfectedAI_MetalDoorDestroy";
		} else if (doorType == "wallgate") {
			doorHit = "InediaInfectedAI_WallgateDoorHit";
			doorOpen = "InediaInfectedAI_WallgateDoorOpen";
			doorDestroy = "InediaInfectedAI_WallgateDoorDestroy";
		} else {
			doorHit = "InediaInfectedAI_WoodenDoorHit";
			doorOpen = "InediaInfectedAI_WoodenDoorOpen";
			doorDestroy = "InediaInfectedAI_WoodenDoorDestroy";
		}

		GetGame().CreateObject(doorHit, m_InediaInfectedAI_NearestClosedDoorPosition);
		
		float breakChance = 0;
		bool isDoorLocked = m_InediaInfectedAI_NearestBuilding.IsDoorLocked(m_InediaInfectedAI_NearestClosedDoorIndex);
		
		// If door is restricted - loss interest to door and goal after hit
		if (InediaInfectedAI_IsRestrictedDoor(m_InediaInfectedAI_NearestBuilding, m_InediaInfectedAI_NearestClosedDoorIndex)) {
			breakChance = 0;
		} else if (!isDoorLocked && m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.BreakingDoorsLossInterestAfterHitChancePercent > 0 && Math.RandomFloatInclusive(0, 100) <= m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.BreakingDoorsLossInterestAfterHitChancePercent) {
			// The infected loses interest in the door not locked with a lockpick
			breakChance = 0;
		} else if (isDoorLocked && m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.BreakingDoorsLossInterestAfterHitLockPickChancePercent > 0 && Math.RandomFloatInclusive(0, 100) <= m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.BreakingDoorsLossInterestAfterHitLockPickChancePercent) {
			// The infected loses interest in the door locked with a lockpick
			breakChance = 0;
		} else if (m_InediaInfectedAI_Infected.InediaInfectedAI_IsBloodVolumeVeryLow()) {
			// If a zombie has a very small blood volume, it cannot break down doors
			breakChance = 0;
		} else {
			if (!isDoorLocked) {
				breakChance = m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.BreakingDoorsOpenChancePercent;
			} else {
				breakChance = m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.BreakingDoorsOpenLockPickChancePercent;
			}
			
			// Influence of injury system on break breakChance
			breakChance *= m_InediaInfectedAI_Infected.InediaInfectedAI_GetInjuryBreakingDoorChanceMultiplier();
			
			// Chance multiplier for crawlers
			if (m_InediaInfectedAI_Infected.IsCrawling()) {
				breakChance *= m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.BreakingDoorsCrawlersChanceMultiplier;
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

		if (breakChance <= 0 && m_InediaInfectedAI_Infected.m_InediaInfectedAI_MindState != DayZInfectedConstants.MINDSTATE_CHASE && m_InediaInfectedAI_Infected.m_InediaInfectedAI_MindState != DayZInfectedConstants.MINDSTATE_FIGHT) {
			// The probability of kicking the door is zero, so the infected loses interest in the door
			// Howewer, loss of interest in the door is not possible in chase mode.
			InediaInfectedAI_LossInterestToDoor();
		} else if (breakChance > 0 && Math.RandomFloatInclusive(0, 100) <= breakChance) {
			// The infected kicked in the door
			string currentDoorSound = doorOpen;

			if (m_InediaInfectedAI_NearestBuilding.IsDoorLocked(m_InediaInfectedAI_NearestClosedDoorIndex)) {
				m_InediaInfectedAI_NearestBuilding.UnlockDoor(m_InediaInfectedAI_NearestClosedDoorIndex);
			}
			m_InediaInfectedAI_NearestBuilding.OpenDoor(m_InediaInfectedAI_NearestClosedDoorIndex);

			if (m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.BreakingDoorsDestroyAfterOpenChancePercent > 0 && Math.RandomFloatInclusive(0, 100) <= m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.BreakingDoorsDestroyAfterOpenChancePercent) {
				// The infected breaks down the door
				House house = House.Cast(m_InediaInfectedAI_NearestBuilding);
				if (house) {
					currentDoorSound = doorDestroy;
					house.InediaInfectedAI_DestroyDoor(m_InediaInfectedAI_NearestClosedDoorIndex);
				}
			}

			GetGame().CreateObject(currentDoorSound, m_InediaInfectedAI_NearestClosedDoorPosition);
		} else {
			// The infected has failed to break down the door and he did not lose interest in it, disable movement, so that he does not penetrate the door with part of his body
			m_InediaInfectedAI_Infected.InediaInfectedAI_ForbidMovementForSeconds(2);
		}
	}

	bool InediaInfectedAI_InBreakingDoor()
	{
		return m_InediaInfectedAI_InBreakingDoor;
	}

	void InediaInfectedAI_StopBreakingDoorHandlerForSeconds(float seconds)
	{
		m_InediaInfectedAI_InBreakingDoor = false;
		m_InediaInfectedAI_BreakingDoorHandlerTimer = seconds;
	}

	/*
		"wooden" - wooden door
		"metal" - metal door
		"wallgate" - wall gate door
	*/
	string InediaInfectedAI_GetDoorType(string lowerBuildingType, int doorNumber)
	{
		if (lowerBuildingType == "land_water_station" || lowerBuildingType == "land_coalplant_main" || lowerBuildingType == "land_power_station" || lowerBuildingType == "land_mil_atc_big" || lowerBuildingType == "land_repair_center") {
			return "metal";
		} else if (lowerBuildingType == "land_mil_aircraftshelter" && (doorNumber == 0 || doorNumber == 2)) {
			return "metal";
		} else if (lowerBuildingType == "land_mil_barracks5" && doorNumber == 1) {
			return "wallgate";
		} else if (lowerBuildingType == "land_village_policestation" && doorNumber == 7) {
			return "wallgate";
		} else if (lowerBuildingType == "land_prison_side" && (doorNumber == 12 || doorNumber == 13 || doorNumber == 14)) {
			return "wallgate";
		} else if (lowerBuildingType == "land_garage_small" && doorNumber == 0) {
			return "metal";
		} else if ((lowerBuildingType == "land_city_firestation" || lowerBuildingType == "land_mil_firestation") && (doorNumber == 0 || doorNumber == 1 || doorNumber == 2 || doorNumber == 3)) {
			return "metal";
		} else if (lowerBuildingType.Contains("land_shed_closed")) {
			return "metal";
		} else if (lowerBuildingType.Contains("land_workshop")) {
			return "metal";
		} else if (lowerBuildingType.Contains("land_cementworks")) {
			return "metal";
		} else if (lowerBuildingType.Contains("land_barn_metal")) {
			return "metal";
		} else if (lowerBuildingType.Contains("garage")) {
			return "metal";
		} else if (lowerBuildingType.Contains("container")) {
			return "metal";
		} else if (lowerBuildingType.Contains("wall_gate_wood")) {
			return "wooden";
		} else if (lowerBuildingType.Contains("wall_gate")) {
			return "wallgate";
		}

		return "wooden";
	}

	bool InediaInfectedAI_IsWreckBuilding(string lowerBuildingType)
	{
		// Exclude these buildings from wrecked
		if (lowerBuildingType.Contains("land_wreck_caravan")) {
			return false;
		}

		// Wreck buildings
		if (lowerBuildingType.Contains("wreck")) {
			return true;
		}

		return false;
	}

	bool InediaInfectedAI_IsRestrictedDoor(Building building, int doorIndex)
	{
		if (!building) {
			return false;
		}

		foreach (string buildingConfigClass, array<int> doorIndexes: m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.BreakingDoorsRestrictedDoors) {
			if (InediaInfectedAI_Functions.IsKindOfCached(buildingConfigClass, building)) {
				if (doorIndexes.Count() == 0 || doorIndexes.Find(doorIndex) >= 0) {
					return true;
				}

				break;
			}
		}

		return false;
	}

	void InediaInfectedAI_LossInterestToDoor()
	{
		float lossInterestSearchSphereRadius = 0.5 * m_InediaInfectedAI_Infected.m_InediaInfectedAI_ConfigZombie.SearchSphereRadiusMax;
		if (m_InediaInfectedAI_Infected.m_InediaInfectedAI_SearchSphereRadius < lossInterestSearchSphereRadius) {
			m_InediaInfectedAI_Infected.m_InediaInfectedAI_SearchSphereRadius = lossInterestSearchSphereRadius;
		}

		m_InediaInfectedAI_Infected.m_InediaInfectedAI_SearchSpherePosition = m_InediaInfectedAI_Infected.GetPosition();
		InediaInfectedAI_ChangeSearchGoal();
		InediaInfectedAI_StopBreakingDoorHandlerForSeconds(2.5);
	}

	Building InediaInfectedAI_GetBuildingOnZombiePath()
	{
		if (InediaInfectedAI_CurrentWaypoint) {
			vector zmbPosition = m_InediaInfectedAI_Infected.GetPosition();
			float zmbWaypointDirectionAngleX = vector.Direction(zmbPosition, InediaInfectedAI_CurrentWaypoint).VectorToAngles()[0];
			vector zmbWaypointDirectionHorizontal = Vector(zmbWaypointDirectionAngleX, 0, 0).AnglesToVector().Normalized();

			float hitFraction;
			vector hitPosition, hitNormal;
			Object hitObject;
			PhxInteractionLayers collisionLayerMask = PhxInteractionLayers.ITEM_LARGE|PhxInteractionLayers.BUILDING|PhxInteractionLayers.FENCE;

			vector fromPosition = zmbPosition + vector.Up * 0.9 - zmbWaypointDirectionHorizontal * 0.2;
			vector toPosition = zmbPosition + vector.Up * 0.9 + zmbWaypointDirectionHorizontal * 1.5;
			float radius = 0.2;
			
			DayZPhysics.SphereCastBullet(fromPosition, toPosition, radius, collisionLayerMask, null, hitObject, hitPosition, hitNormal, hitFraction);
			if (hitObject && hitObject.IsInherited(Building)) {
				Building building = Building.Cast(hitObject);
				return building;
			}
		}

		return null;
	}

	void InediaInfectedAI_GetNearestDoorsInfo(Building pBuilding, out int pNearestClosedDoorIndex, out vector pNearestClosedDoorPosition, out int pNearestOpenDoorIndex, out vector pNearestOpenDoorPosition)
	{
		pNearestClosedDoorIndex = -1;
		pNearestClosedDoorPosition = vector.Zero;
		pNearestOpenDoorIndex = -1;
		pNearestOpenDoorPosition = vector.Zero;
		
		if (!pBuilding) {
			return;
		}

	 	vector zmbPos = m_InediaInfectedAI_Infected.GetPosition();
		int doorsCount = GetGame().ConfigGetChildrenCount("CfgVehicles " + pBuilding.GetType() + " Doors");
		
		for (int i = 0; i < doorsCount; i++) {
			vector doorPosition = pBuilding.GetDoorSoundPos(i);
			InediaInfectedAI_FixDoorPosition(pBuilding, i, doorPosition);

			if (vector.DistanceSq(zmbPos + vector.Up, doorPosition) <= 2 * 2) {
				// For ignore stove doors that are also in the list of building doors
				float doorHeight = doorPosition[1] - zmbPos[1];
				if (doorHeight < 0.7) {
					continue;
				}
				
				// Ignore the door if it is behind the infected
				if (m_InediaInfectedAI_Infected.InediaInfectedAI_IsPositionBehind(doorPosition)) {
					continue;
				}
				
				if (pBuilding.IsDoorOpen(i)) {
					if (!pNearestOpenDoorPosition || vector.DistanceSq(zmbPos + vector.Up, doorPosition) < vector.DistanceSq(zmbPos + vector.Up, pNearestOpenDoorPosition)) {
						pNearestOpenDoorPosition = doorPosition;
						pNearestOpenDoorIndex = i;
					}
				} else {
					if (!pNearestClosedDoorPosition || vector.DistanceSq(zmbPos + vector.Up, doorPosition) < vector.DistanceSq(zmbPos + vector.Up, pNearestClosedDoorPosition)) {
						pNearestClosedDoorPosition = doorPosition;
						pNearestClosedDoorIndex = i;
					}
				}
			}
		}
	}

	void InediaInfectedAI_FixDoorPosition(notnull Building building, int doorNumber, out vector doorPosition)
	{
		vector directionToDoor;
		float angleToDoor;

		string lowerBuildingType = building.GetType();
		lowerBuildingType.ToLower();

		// Land_Mil_AircraftShelter door position fix
		if (lowerBuildingType == "land_mil_aircraftshelter") {
			if (doorNumber == 0) {
				directionToDoor = Vector(-23.8994,1.06689,-5.20898);
				angleToDoor = 142.207;
			} else if (doorNumber == 1) {
				directionToDoor = Vector(-18.5562,1.10162,-7.29688);
				angleToDoor = 133.037;
			} else if (doorNumber == 2) {
				directionToDoor = Vector(9.21582,1.10162,-9.98438);
                angleToDoor = 21.7952;
			}
		} else if (lowerBuildingType == "land_village_pub") {
            if (doorNumber == 4) {
                directionToDoor = Vector(-4.59814,-2.45171,-1.67725);
                angleToDoor = 51.6542;
            }
		} else if (lowerBuildingType == "land_factory_lathes") {
            if (doorNumber == 0) {
                directionToDoor = Vector(11.6084,-4.38376,3.67969);
                angleToDoor = -231.588;
            }
		} else if (lowerBuildingType == "land_prison_side") {
            if (doorNumber == 12) {
                directionToDoor = Vector(10.5903,-4.93613,-10.6978);
                angleToDoor = 92.874;
            } else if (doorNumber == 13) {
                directionToDoor = Vector(11.155,-4.93659,-10.0874);
                angleToDoor = 89.7077;
            }
		} else if (lowerBuildingType == "land_garage_small") {
            if (doorNumber == 0) {
                directionToDoor = Vector(1.38184,-0.185608,2.62354);
                angleToDoor = -200.523;
            }
		} else if (lowerBuildingType == "land_rail_warehouse_small") {
            if (doorNumber == 1) {
                directionToDoor = Vector(-1.0918,-0.493837,1.43262);
                angleToDoor = 267.414;
            } else if (doorNumber == 2) {
                directionToDoor = Vector(-0.662109,-0.493884,-4.27051);
                angleToDoor = 133.538;
            }
		} else if (lowerBuildingType == "land_rail_station_big") {
            if (doorNumber == 0) {
                directionToDoor = Vector(-0.656738,-3.98749,5.21216);
                angleToDoor = 220.458;
            }
		} else if (lowerBuildingType == "land_rail_station_small") {
            if (doorNumber == 0) {
                directionToDoor = Vector(-0.124512,-0.215072,-2.25879);
                angleToDoor = -19.2224;
            } else if (doorNumber == 1) {
                directionToDoor = Vector(-1.67432,-0.215074,1.74634);
                angleToDoor = 113.829;
            } else if (doorNumber == 2) {
                directionToDoor = Vector(-2.06543,-0.215017,4.21436);
                angleToDoor = 131.513;
            }
		} else if (lowerBuildingType == "land_tisy_barracks") {
			if (doorNumber == 0) {
				directionToDoor = Vector(3.53662,-2.72079,0.0244141);
                angleToDoor = -206.038;
			} else if (doorNumber == 1) {
                directionToDoor = Vector(3.96521,-2.75214,3.28418);
                angleToDoor = -245.276;
            } else if (doorNumber == 2) {
                directionToDoor = Vector(2.49866,4.39743,2.63184);
                angleToDoor = -252.129;
            } else if (doorNumber == 3) {
                directionToDoor = Vector(0.646362,1.3981,0.115234);
                angleToDoor = -215.751;
            }
		} else if (lowerBuildingType == "land_mil_atc_small") {
			if (doorNumber == 0) {
				directionToDoor = Vector(3.7041,-8.60431,-2.75879);
                angleToDoor = -7.98483;
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

		if (directionToDoor) {
			float directionToDoorRotateAngle = building.GetDirection().VectorToAngles()[0] + angleToDoor - directionToDoor.VectorToAngles()[0];
			doorPosition = building.GetPosition() + vector.RotateAroundZeroDeg(directionToDoor, vector.Up, directionToDoorRotateAngle);
		}
	}

	float m_InediaInfectedAI_VariationStanceTimer = 0;
	void InediaInfectedAI_VariationStanceHandler(float pDt)
	{
		m_InediaInfectedAI_VariationStanceTimer -= pDt;
		if (m_InediaInfectedAI_VariationStanceTimer > 0) return;
		m_InediaInfectedAI_VariationStanceTimer = Math.RandomFloatInclusive(30, 90);

		m_InediaInfectedAI_Infected.m_InediaInfectedAI_InfectedAnimationST.SetVariationStance(this, Math.RandomIntInclusive(0, 1));
	}

	bool m_InediaInfectedAI_LookAtTargetHandlerInitialized = false;
	float m_InediaInfectedAI_CurrentLookAtX = 0;
	float m_InediaInfectedAI_CurrentLookAtY = 0;
	float m_InediaInfectedAI_LookAtTargetHandlerTimer = 0;
	void InediaInfectedAI_LookAtTargetHandler(float pDt)
	{
		if (m_InediaInfectedAI_Infected.InediaInfectedAI_IsInAnimation()) {
			// Waiting for the end of the animations
			return;
		}
		
		m_InediaInfectedAI_LookAtTargetHandlerTimer -= pDt;
		if (m_InediaInfectedAI_LookAtTargetHandlerTimer > 0) return;
		m_InediaInfectedAI_LookAtTargetHandlerTimer = 0.5;
		
		if (!m_InediaInfectedAI_LookAtTargetHandlerInitialized) {
			m_InediaInfectedAI_Infected.m_InediaInfectedAI_InfectedAnimationST.SetLook(this, true);
			m_InediaInfectedAI_Infected.m_InediaInfectedAI_InfectedAnimationST.SetLookDirX(this, 0);
			m_InediaInfectedAI_Infected.m_InediaInfectedAI_InfectedAnimationST.SetLookDirY(this, 0);
			m_InediaInfectedAI_LookAtTargetHandlerInitialized = true;
		}
		
		m_InediaInfectedAI_Target = m_InediaInfectedAI_Infected.GetInputController().GetTargetEntity();
		if (m_InediaInfectedAI_Target) {
			vector zombieTargetDirection = vector.Direction(m_InediaInfectedAI_Infected.GetPosition(), m_InediaInfectedAI_Target.GetPosition());
			m_InediaInfectedAI_CurrentLookAtX = Math.Clamp(Math.DiffAngle(zombieTargetDirection.VectorToAngles()[0], m_InediaInfectedAI_Infected.GetDirection().VectorToAngles()[0]), -90, 90);
			m_InediaInfectedAI_CurrentLookAtY = Math.Clamp(Math.DiffAngle(zombieTargetDirection.VectorToAngles()[1], m_InediaInfectedAI_Infected.GetDirection().VectorToAngles()[1]), -90, 90);
			m_InediaInfectedAI_Infected.m_InediaInfectedAI_InfectedAnimationST.SetLookDirX(this, m_InediaInfectedAI_CurrentLookAtX);
			m_InediaInfectedAI_Infected.m_InediaInfectedAI_InfectedAnimationST.SetLookDirY(this, m_InediaInfectedAI_CurrentLookAtY);
		} else {
			if (m_InediaInfectedAI_CurrentLookAtX != 0) {
				m_InediaInfectedAI_CurrentLookAtX = 0;
				m_InediaInfectedAI_Infected.m_InediaInfectedAI_InfectedAnimationST.SetLookDirX(this, m_InediaInfectedAI_CurrentLookAtX);
			}
			
			if (m_InediaInfectedAI_CurrentLookAtY != 0) {
				m_InediaInfectedAI_CurrentLookAtY = 0;
				m_InediaInfectedAI_Infected.m_InediaInfectedAI_InfectedAnimationST.SetLookDirY(this, m_InediaInfectedAI_CurrentLookAtY);
			}
		}
	}

    vector InediaInfectedAI_CurrentWaypoint;
    // Returns search goal on navmesh
    protected vector InediaInfectedAI_GoToSearchGoal(vector searchGoal)
    {
        if (!searchGoal) return vector.Zero;

        vector zmbPos = m_InediaInfectedAI_Infected.GetPosition();
        vector zmbDir = m_InediaInfectedAI_Infected.GetDirection();
        
        TVectorArray waypoints = new TVectorArray;
        if (GetGame().GetWorld().GetAIWorld().FindPath(zmbPos, searchGoal, m_InediaInfectedAI_PGFilter, waypoints)) {
            int waypointsCount = waypoints.Count();
            if (waypointsCount < 2) return vector.Zero;

            InediaInfectedAI_CurrentWaypoint = waypoints.Get(1);

            if (m_InediaInfectedAI_NearestAliveZmbInPath && m_InediaInfectedAI_NearestOpenDoorIndex < 0) {
                // If there is an infected in the way, try to go around them.
				vector nearestZmbPos = m_InediaInfectedAI_NearestAliveZmbInPath.GetPosition();
    			vector avoidanceDirection = (zmbPos - nearestZmbPos).Normalized();
    			vector newDirection = (zmbDir + avoidanceDirection).Normalized();
                
                InediaInfectedAI_CurrentWaypoint = zmbPos + newDirection;
            }
            
            InediaInfectedAI_SetOrientationToPosition(InediaInfectedAI_CurrentWaypoint);

            return waypoints.Get(waypointsCount - 1);
        }

        return vector.Zero;
    }
    
    ref array<ZombieBase> m_InediaInfectedAI_NearestAliveZombies = new array<ZombieBase>;
    protected ZombieBase InediaInfectedAI_GetNearestAliveZombieInPath()
    {		
        vector zmbPos = m_InediaInfectedAI_Infected.GetPosition();
        vector zmbDir = m_InediaInfectedAI_Infected.GetDirection();

        MiscGameplayFunctions.InediaInfectedAI_GetZombiesAtPosition3D(zmbPos, 1, m_InediaInfectedAI_NearestAliveZombies);
        
        foreach (ZombieBase nearZmb: m_InediaInfectedAI_NearestAliveZombies) {
            if (nearZmb.IsAlive() && nearZmb != m_InediaInfectedAI_Infected) {
                vector nearestZmbPos = nearZmb.GetPosition();
                vector directionToNearestZmb = vector.Direction(zmbPos, nearestZmbPos);
                float toNearestZmbDiffAngle = Math.DiffAngle(zmbDir.VectorToAngles()[0], directionToNearestZmb.VectorToAngles()[0]);
                float toNearestZmbDistance = vector.Distance(zmbPos, nearestZmbPos);
                float toNearestZmbHeightDiff = Math.AbsFloat(zmbPos[1] - nearestZmbPos[1]);
                
                // If the other infected is more than 1.8 meters above or below our infected, we ignore them.
                if (toNearestZmbHeightDiff > 1.8) {
                    continue;
                }
                
                // To determine whether our infected can pass by another,
                // we consider the other infected as a circle with a radius of 0.4 meters
                // and find the angle needed to pass by it tangentially at a distance of 0.2 meters.
                // Then we compare this angle with the current rotation angle of our infected relative to the other,
                // and if it is greater or equal, passing is possible.
                
                float tangentCircleRadius = 0.3 + 0.15;
                
                if (toNearestZmbDistance < tangentCircleRadius || toNearestZmbDistance == 0) {
                    // Our infected is inside the tangent circle, therefore, the other infected character is already in their way.
                    return nearZmb;
                } else {
	                float toNearestZmbTangentAngle = Math.Asin(tangentCircleRadius / toNearestZmbDistance) * Math.RAD2DEG;
	                if (Math.AbsFloat(toNearestZmbDiffAngle) < toNearestZmbTangentAngle) {
	                    return nearZmb;
	                }
				}
            }
        }
        
        return null;
    }
	
	protected vector InediaInfectedAI_GetCurrentWaypoint(vector goal)
	{
		if (!goal) return vector.Zero;

		TVectorArray waypoints = new TVectorArray;
		if (GetGame().GetWorld().GetAIWorld().FindPath(m_InediaInfectedAI_Infected.GetPosition(), goal, m_InediaInfectedAI_PGFilter, waypoints)) {
			if (waypoints.Count() >= 2) {
				return waypoints.Get(1);
			}
		}

		return vector.Zero;
	}

	protected vector InediaInfectedAI_GetSearchGoalInSphere(vector sphereCenter, float sphereRadius)
	{
		vector goal;

		bool goalFound = false;
		for (int try = 1; try <= 10; try++) {
			vector findpathNavmeshGoal;
			float randSphereRadius = Math.RandomFloatInclusive(0, sphereRadius);
			
			goal = sphereCenter + vector.RandomDir() * randSphereRadius;
			TVectorArray waypoints = new TVectorArray;
			if (!GetGame().GetWorld().GetAIWorld().FindPath(m_InediaInfectedAI_Infected.GetPosition(), goal, m_InediaInfectedAI_PGFilter, waypoints)) continue;
			if (waypoints.Count() < 2) continue;

			findpathNavmeshGoal = waypoints.Get(waypoints.Count() - 1);

			goal = findpathNavmeshGoal;
			goalFound = true;

			break;
		}

		if (!goalFound) {
			goal = vector.Zero;
		} /*else {
			InediaInfectedAI_ApplyGoalNavmeshFixes(goal);
		}*/

		return goal;
	}

	void InediaInfectedAI_ChangeSearchGoal()
	{
		m_InediaInfectedAI_SearchGoal = vector.Zero;
		m_ProblemsInReachingSearchGoalCounter = 0;
	}

	protected void InediaInfectedAI_ApplyGoalNavmeshFixes(out vector searchGoal)
	{
		string worldName;
		GetGame().GetWorldName(worldName);
		// worldName.ToLower();

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

		InediaInfectedAI_Config config = InediaInfectedAI_Config.GetInstance();

		if (worldName == "chernarusplus") {
			array<ref InediaInfectedAI_ConfigNavMeshFix> navmeshChernarusFixes = config.GetNavmeshChernarusFixes();
			foreach (InediaInfectedAI_ConfigNavMeshFix navmeshfix : navmeshChernarusFixes) {
				if (vector.DistanceSq(searchGoal, navmeshfix.m_Position) <= navmeshfix.m_Radius * navmeshfix.m_Radius) {
					if (navmeshfix.m_Goals.Count() > 0) {
						vector nearestGoal;
						float nearestDistanceSq;
						foreach (vector goal: navmeshfix.m_Goals) {
							float currentGoalDistanceSq = vector.DistanceSq(searchGoal, goal);
							if (!nearestGoal || currentGoalDistanceSq < nearestDistanceSq) {
								nearestGoal = goal;
								nearestDistanceSq = currentGoalDistanceSq;
							}
						}
						searchGoal = nearestGoal;
						break;
					}
				}
			}
		} else if (worldName == "enoch") {
			//
		}
	}
}
