/**
 * Created: 2024-02-06
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

modded class IngameHud
{
	Widget m_InediaInfectedAI_Layout;
	Widget m_InediaInfectedAI_BadgesPanelWidget;
	ref map<string, ref Widget> m_InediaInfectedAI_BadgesWidgetsList = new map<string, ref Widget>;

	override void Init(Widget hud_panel_widget)
	{
		super.Init(hud_panel_widget);

		// Prevents issues with any mods that replace the "BadgesPanel" widget during the initialization of the game HUD
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InediaInfectedAI_InitInediaPainWidgets, 100, false, hud_panel_widget);
	}

	bool m_InediaInfectedAI_InediaPainWidgetsInitialized = false;
	void InediaInfectedAI_InitInediaPainWidgets(Widget hudPanelWidget) {
		if (!hudPanelWidget) {
			return;
		}

		if (!m_InediaInfectedAI_Layout) {
			m_InediaInfectedAI_Layout = GetGame().GetWorkspace().CreateWidgets("InediaInfectedAI/gui/layouts/inediainfectedaihud.layout");
			
			if (!m_InediaInfectedAI_Layout) {
				return;
			}
		}
		
		if (!m_InediaInfectedAI_BadgesPanelWidget) {
			if (m_Badges) {
				m_InediaInfectedAI_BadgesPanelWidget = m_Badges;
			} else {
				m_InediaInfectedAI_BadgesPanelWidget = hudPanelWidget.FindAnyWidget("BadgesPanel");
			}
			
			if (!m_InediaInfectedAI_BadgesPanelWidget) {
				return;
			}
		}
		
		array<string> inediaPainLimbsSequence = {"head", "torso", "legs", "arms"};

		// Internal bleeding badge
		m_InediaInfectedAI_BadgesWidgetsList["internal_bleeding_frame"] = m_InediaInfectedAI_Layout.FindAnyWidget("internal_bleeding_frame");
		m_InediaInfectedAI_BadgesPanelWidget.AddChild(m_InediaInfectedAI_BadgesWidgetsList["internal_bleeding_frame"]);

		// Bullets widgets
		foreach (string inediaBulletLimb: inediaPainLimbsSequence) {
			m_InediaInfectedAI_BadgesWidgetsList["bullet_" + inediaBulletLimb + "_frame"] = m_InediaInfectedAI_Layout.FindAnyWidget("bullet_" + inediaBulletLimb + "_frame");
			m_InediaInfectedAI_BadgesWidgetsList["bullet_" + inediaBulletLimb + "_frame_counter_1"] = m_InediaInfectedAI_BadgesWidgetsList["bullet_" + inediaBulletLimb + "_frame"].FindAnyWidget("Counter1");
			m_InediaInfectedAI_BadgesWidgetsList["bullet_" + inediaBulletLimb + "_frame_counter_2"] = m_InediaInfectedAI_BadgesWidgetsList["bullet_" + inediaBulletLimb + "_frame"].FindAnyWidget("Counter2");
			m_InediaInfectedAI_BadgesWidgetsList["bullet_" + inediaBulletLimb + "_frame_counter_3"] = m_InediaInfectedAI_BadgesWidgetsList["bullet_" + inediaBulletLimb + "_frame"].FindAnyWidget("Counter3");
			m_InediaInfectedAI_BadgesPanelWidget.AddChild(m_InediaInfectedAI_BadgesWidgetsList["bullet_" + inediaBulletLimb + "_frame"]);
		}

		// Breaks widgets
		foreach (string inediaBreakLimb: inediaPainLimbsSequence) {
			m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaBreakLimb + "_frame"] = m_InediaInfectedAI_Layout.FindAnyWidget("break_" + inediaBreakLimb + "_frame");
			m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaBreakLimb + "_frame_limb"] = m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaBreakLimb + "_frame"].FindAnyWidget("Limb");
			m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaBreakLimb + "_frame_bone_break"] = m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaBreakLimb + "_frame"].FindAnyWidget("BoneBreak");
			m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaBreakLimb + "_frame_bone_break_splinted"] = m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaBreakLimb + "_frame"].FindAnyWidget("BoneBreakSplinted");
			m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaBreakLimb + "_frame_tendency_first"] = m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaBreakLimb + "_frame"].FindAnyWidget("TendencyFirst");
			m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaBreakLimb + "_frame_tendency_second"] = m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaBreakLimb + "_frame"].FindAnyWidget("TendencySecond");
			m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaBreakLimb + "_frame_tendency_third"] = m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaBreakLimb + "_frame"].FindAnyWidget("TendencyThird");
			m_InediaInfectedAI_BadgesPanelWidget.AddChild(m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaBreakLimb + "_frame"]);
		}

		// Deep wounds widgets
		foreach (string inediaDeepWoundLimb: inediaPainLimbsSequence) {
			m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaDeepWoundLimb + "_frame"] = m_InediaInfectedAI_Layout.FindAnyWidget("deep_wound_" + inediaDeepWoundLimb + "_frame");
			m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaDeepWoundLimb + "_frame_limb"] = m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaDeepWoundLimb + "_frame"].FindAnyWidget("Limb");
			m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaDeepWoundLimb + "_frame_deep_wound"] = m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaDeepWoundLimb + "_frame"].FindAnyWidget("DeepWound");
			m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaDeepWoundLimb + "_frame_deep_wound_bandaged"] = m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaDeepWoundLimb + "_frame"].FindAnyWidget("DeepWoundBandaged");
			m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaDeepWoundLimb + "_frame_deep_wound_bandaged_ineffective"] = m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaDeepWoundLimb + "_frame"].FindAnyWidget("DeepWoundBandagedIneffective");
			m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaDeepWoundLimb + "_frame_tendency_first"] = m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaDeepWoundLimb + "_frame"].FindAnyWidget("TendencyFirst");
			m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaDeepWoundLimb + "_frame_tendency_second"] = m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaDeepWoundLimb + "_frame"].FindAnyWidget("TendencySecond");
			m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaDeepWoundLimb + "_frame_tendency_third"] = m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaDeepWoundLimb + "_frame"].FindAnyWidget("TendencyThird");
			m_InediaInfectedAI_BadgesPanelWidget.AddChild(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaDeepWoundLimb + "_frame"]);
		}

		// Pain widgets
		foreach (string inediaPainLimb: inediaPainLimbsSequence) {
			m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame"] = m_InediaInfectedAI_Layout.FindAnyWidget("pain_" + inediaPainLimb + "_frame");
			m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame_limb"] = m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame"].FindAnyWidget("Limb");
			m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame_pain_red"] = m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame"].FindAnyWidget("PainRed");
			m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame_pain_orange"] = m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame"].FindAnyWidget("PainOrange");
			m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame_pain_yellow"] = m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame"].FindAnyWidget("PainYellow");
			m_InediaInfectedAI_BadgesPanelWidget.AddChild(m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame"]);
		}
		
		m_InediaInfectedAI_InediaPainWidgetsInitialized = true;
	}

	void InediaInfectedAI_RenderInediaPainLimbWidgetPain(PlayerBase player, string inediaPainLimb)
	{
		if (player.InediaInfectedAI_GetPainManager().PainSystemIsActive() && player.InediaInfectedAI_GetPainManager().m_ShowPainBadges) {
			InediaInfectedAI_LimbPainLevels limbPainLevel = player.InediaInfectedAI_GetPainManager().GetPainWithBuffsLevelForLimb(inediaPainLimb);
			if (limbPainLevel > InediaInfectedAI_LimbPainLevels.ZERO) {
				InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame"]);
				
				/*
					Yellow: ARGB(255, 220, 220, 0)
					Orange: ARGB(255, 255, 128, 0)
					Red: ARGB(255, 255, 0, 0)
				*/

			    if (limbPainLevel >= InediaInfectedAI_LimbPainLevels.CRITICAL) {
					InediaInfectedAI_FlashBadgeWidget("pain_" + inediaPainLimb + "_frame_pain_red");
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame_pain_orange"]);
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame_pain_yellow"]);
			        //m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame_limb"].SetColor(ARGB(255, 255, 0, 0));
			    } else if (limbPainLevel >= InediaInfectedAI_LimbPainLevels.BADLY) {
					InediaInfectedAI_EndFlashBadgeWidget("pain_" + inediaPainLimb + "_frame_pain_red");
					InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame_pain_red"]);
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame_pain_orange"]);
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame_pain_yellow"]);
			        //m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame_limb"].SetColor(ARGB(255, 255, 255, 255));
			    } else if (limbPainLevel >= InediaInfectedAI_LimbPainLevels.MEDIUM) {
					InediaInfectedAI_EndFlashBadgeWidget("pain_" + inediaPainLimb + "_frame_pain_red");
					InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame_pain_orange"]);
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame_pain_red"]);
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame_pain_yellow"]);
					//m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame_limb"].SetColor(ARGB(255, 255, 255, 255));
			    } else {
					InediaInfectedAI_EndFlashBadgeWidget("pain_" + inediaPainLimb + "_frame_pain_red");
					InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame_pain_yellow"]);
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame_pain_red"]);
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame_pain_orange"]);
					//m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame_limb"].SetColor(ARGB(255, 255, 255, 255));
			    }
			} else {
				InediaInfectedAI_EndFlashBadgeWidget("pain_" + inediaPainLimb + "_frame_pain_red");
				InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame"]);
			}
		} else {
			InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["pain_" + inediaPainLimb + "_frame"]);
		}
	}

	void InediaInfectedAI_RenderInediaPainLimbWidgetDeepWound(PlayerBase player, string inediaPainLimb)
	{
		if (player.InediaInfectedAI_GetPainManager().m_ShowPainBadges && player.InediaInfectedAI_GetPainManager().LimbsDeepWoundSystemIsActive()) {
			if (player.InediaInfectedAI_GetPainManager().IsLimbDeepWounded(inediaPainLimb)) {
				InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame"]);

				// Bandage icon
				if (player.InediaInfectedAI_GetPainManager().IsLimbDeepWoundBandaged(inediaPainLimb)) {
					if (player.InediaInfectedAI_GetPainManager().IsLimbDeepWoundBandageEffective(inediaPainLimb)) {
						InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_deep_wound_bandaged"]);
						InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_deep_wound_bandaged_ineffective"]);
					} else {
						InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_deep_wound_bandaged_ineffective"]);
						InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_deep_wound_bandaged"]);
					}

					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_deep_wound"]);
				} else {
					InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_deep_wound"]);

					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_deep_wound_bandaged"]);
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_deep_wound_bandaged_ineffective"]);
				}

				// Regen tendency icons
				if (player.InediaInfectedAI_GetPainManager().GetDeepWoundRegenerationLevelForLimbClient(inediaPainLimb) >= InediaInfectedAI_LimbRegenerationLevels.HIGH) {
					InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_tendency_first"]);
					m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_tendency_first"].SetPos(0, -2);
					InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_tendency_second"]);
					m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_tendency_second"].SetPos(0, 2);
					InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_tendency_third"]);
					m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_tendency_third"].SetPos(0, 6);
				} else if (player.InediaInfectedAI_GetPainManager().GetDeepWoundRegenerationLevelForLimbClient(inediaPainLimb) >= InediaInfectedAI_LimbRegenerationLevels.MEDIUM) {
					InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_tendency_first"]);
					m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_tendency_first"].SetPos(0, -2);
					InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_tendency_second"]);
					m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_tendency_second"].SetPos(0, 2);
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_tendency_third"]);
				} else if (player.InediaInfectedAI_GetPainManager().GetDeepWoundRegenerationLevelForLimbClient(inediaPainLimb) >= InediaInfectedAI_LimbRegenerationLevels.LOW) {
					InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_tendency_first"]);
					m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_tendency_first"].SetPos(0, -2);
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_tendency_second"]);
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_tendency_third"]);
				} else {
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_tendency_first"]);
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_tendency_second"]);
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame_tendency_third"]);
				}
			} else {
				InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame"]);
			}
		} else {
			InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["deep_wound_" + inediaPainLimb + "_frame"]);
		}
	}

	void InediaInfectedAI_RenderInediaPainLimbWidgetBreak(PlayerBase player, string inediaPainLimb)
	{
		if (player.InediaInfectedAI_GetPainManager().m_ShowPainBadges && player.InediaInfectedAI_GetPainManager().LimbsBreakSystemIsActive()) {
			if (player.InediaInfectedAI_GetPainManager().IsLimbBroken(inediaPainLimb)) {
				InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame"]);

				// Stabilization icon
				if (player.InediaInfectedAI_GetPainManager().IsLimbStabilized(inediaPainLimb)) {
					InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_bone_break_splinted"]);
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_bone_break"]);
				} else {
					InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_bone_break"]);
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_bone_break_splinted"]);
				}

                // Regen tendency icons
                if (player.InediaInfectedAI_GetPainManager().GetBreakRegenerationLevelForLimbClient(inediaPainLimb) >= InediaInfectedAI_LimbRegenerationLevels.HIGH) {
                    InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_tendency_first"]);
                    m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_tendency_first"].SetPos(0, -2);
                    InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_tendency_second"]);
                    m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_tendency_second"].SetPos(0, 2);
                    InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_tendency_third"]);
                    m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_tendency_third"].SetPos(0, 6);
                } else if (player.InediaInfectedAI_GetPainManager().GetBreakRegenerationLevelForLimbClient(inediaPainLimb) >= InediaInfectedAI_LimbRegenerationLevels.MEDIUM) {
                    InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_tendency_first"]);
                    m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_tendency_first"].SetPos(0, -2);
                    InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_tendency_second"]);
                    m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_tendency_second"].SetPos(0, 2);
                    InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_tendency_third"]);
                } else if (player.InediaInfectedAI_GetPainManager().GetBreakRegenerationLevelForLimbClient(inediaPainLimb) >= InediaInfectedAI_LimbRegenerationLevels.LOW) {
                    InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_tendency_first"]);
                    m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_tendency_first"].SetPos(0, -2);
                    InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_tendency_second"]);
                    InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_tendency_third"]);
                } else {
                    InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_tendency_first"]);
                    InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_tendency_second"]);
                    InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame_tendency_third"]);
                }
			} else {
				InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame"]);
			}
		} else {
			InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["break_" + inediaPainLimb + "_frame"]);
		}
	}

	void InediaInfectedAI_RenderInediaPainLimbWidgetBullet(PlayerBase player, string inediaPainLimb)
	{
		if (player.InediaInfectedAI_GetPainManager().m_ShowPainBadges && player.InediaInfectedAI_GetPainManager().LimbsBulletSystemIsActive()) {
			int bulletCountInLimb = player.InediaInfectedAI_GetPainManager().GetBulletCountInLimb(inediaPainLimb);
			if (bulletCountInLimb > 0) {
				InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["bullet_" + inediaPainLimb + "_frame"]);

				if (bulletCountInLimb >= 3) {
					InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["bullet_" + inediaPainLimb + "_frame_counter_3"]);
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["bullet_" + inediaPainLimb + "_frame_counter_1"]);
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["bullet_" + inediaPainLimb + "_frame_counter_2"]);
				} else if (bulletCountInLimb == 2) {
					InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["bullet_" + inediaPainLimb + "_frame_counter_2"]);
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["bullet_" + inediaPainLimb + "_frame_counter_1"]);
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["bullet_" + inediaPainLimb + "_frame_counter_3"]);
				} else if (bulletCountInLimb == 1) {
					InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["bullet_" + inediaPainLimb + "_frame_counter_1"]);
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["bullet_" + inediaPainLimb + "_frame_counter_2"]);
					InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["bullet_" + inediaPainLimb + "_frame_counter_3"]);
				}
			} else {
				InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["bullet_" + inediaPainLimb + "_frame"]);
			}
		} else {
			InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["bullet_" + inediaPainLimb + "_frame"]);
		}
	}

	void InediaInfectedAI_RenderInediaWidgetInternalBleeding(PlayerBase player)
	{
		if (player.InediaInfectedAI_GetPainManager().m_ShowPainBadges && player.InediaInfectedAI_GetPainManager().InternalBleedingSystemIsActive()) {
			if (player.InediaInfectedAI_GetPainManager().IsInternalBleedingActive()) {
				InediaInfectedAI_ShowWidget(m_InediaInfectedAI_BadgesWidgetsList["internal_bleeding_frame"]);
			} else {
				InediaInfectedAI_HideWidget(m_InediaInfectedAI_BadgesWidgetsList["internal_bleeding_frame"]);
			}
		}
	}

	void InediaInfectedAI_ShowWidget(Widget widget)
	{
		if (!widget.IsVisible()) {
			widget.Show(true);
		}
	}
	void InediaInfectedAI_HideWidget(Widget widget)
	{
		if (widget.IsVisible()) {
			widget.Show(false);
		}
	}


	override void Update(float timeslice) {
		super.Update(timeslice);

		if (m_InediaInfectedAI_InediaPainWidgetsInitialized) {
			InediaInfectedAI_UpdatePainBadges(timeslice);
			InediaInfectedAI_FlashBadgesWidgetsHandler(timeslice);
		}
	}

// ndAH3lGW5d6CmO9EYAg5tlcxVDVzS0tJYkdhUFJlWkxMUEhGQUZQVzNlUHhIK3NoQ09zRjcxL25iRXFvVEljbEZPd0oyQ000NGplSk04V1NiSHBZdFpicGNUZU1TWC9sdDlPMlkwbGs3aENybWxQQTFXNFFwNnFMKzNuZ1RNWkQ4bEtBL3liSjY1WW9wU0NybU03SitDQ2JtV1RwUmJMSG9jdWdwUmJMYTN4OUZVZ2p1WFZrNkpLR2RxaXc1WG1yMnJGWDM2KzJDcng4QTVUSXMxWmlQbXJHbFhmTjJiWlhWV01mYVAzcDQ0RlJ3MlFZcERqVm1VWXdkQ05UdlhQUmF1Unh3T3F3ZXlaVmlhMFJzMU9lMEYzNlpWamZVbGZFbjN6VnZRPT0=

	float m_InediaInfectedAI_UpdatePainBadgesLastTime = -1000000;
	void InediaInfectedAI_UpdatePainBadges(float pDt)
	{
		if ((GetGame().GetTickTime() - m_InediaInfectedAI_UpdatePainBadgesLastTime) < 1) {
			return;
		}
		m_InediaInfectedAI_UpdatePainBadgesLastTime = GetGame().GetTickTime();

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player) {
			foreach (string inediaPainLimb: InediaInfectedAI_PainManager.m_InediaPainLimbs) {
				InediaInfectedAI_RenderInediaPainLimbWidgetPain(player, inediaPainLimb);
				InediaInfectedAI_RenderInediaPainLimbWidgetDeepWound(player, inediaPainLimb);
				InediaInfectedAI_RenderInediaPainLimbWidgetBreak(player, inediaPainLimb);
				InediaInfectedAI_RenderInediaPainLimbWidgetBullet(player, inediaPainLimb);
			}

			InediaInfectedAI_RenderInediaWidgetInternalBleeding(player);
		}
	}
	
	ref map<string, bool> m_InediaInfectedAI_FlashBadgesWidgetsFlashStatus = new map<string, bool>;
	float m_InediaInfectedAI_FlashBadgesWidgetsHandlerLastTime = -1000000;
	bool m_InediaInfectedAI_FlashBadgesWidgetsHandlerFlashStatus = true;
	void InediaInfectedAI_FlashBadgesWidgetsHandler(float pDt)
	{
		if ((GetGame().GetTickTime() - m_InediaInfectedAI_FlashBadgesWidgetsHandlerLastTime) < 0.35) {
			return;
		}
		m_InediaInfectedAI_FlashBadgesWidgetsHandlerLastTime = GetGame().GetTickTime();
		
		m_InediaInfectedAI_FlashBadgesWidgetsHandlerFlashStatus = !m_InediaInfectedAI_FlashBadgesWidgetsHandlerFlashStatus;
		
		foreach (string badgeWidgetName, bool isFlashing: m_InediaInfectedAI_FlashBadgesWidgetsFlashStatus) {
			if (isFlashing) {
				m_InediaInfectedAI_BadgesWidgetsList[badgeWidgetName].Show(m_InediaInfectedAI_FlashBadgesWidgetsHandlerFlashStatus);
			}
		}
	}
	
	void InediaInfectedAI_FlashBadgeWidget(string badgeWidgetName)
	{
		if (!m_InediaInfectedAI_FlashBadgesWidgetsFlashStatus.Contains(badgeWidgetName)) {
			m_InediaInfectedAI_FlashBadgesWidgetsFlashStatus[badgeWidgetName] = true;
		}
		
		if (!m_InediaInfectedAI_FlashBadgesWidgetsFlashStatus[badgeWidgetName]) {
			m_InediaInfectedAI_FlashBadgesWidgetsFlashStatus[badgeWidgetName] = true;
		}
	}
	
	void InediaInfectedAI_EndFlashBadgeWidget(string badgeWidgetName)
	{
		if (!m_InediaInfectedAI_FlashBadgesWidgetsFlashStatus.Contains(badgeWidgetName)) {
			m_InediaInfectedAI_FlashBadgesWidgetsFlashStatus[badgeWidgetName] = false;
		}
		
		if (m_InediaInfectedAI_FlashBadgesWidgetsFlashStatus[badgeWidgetName]) {
			m_InediaInfectedAI_FlashBadgesWidgetsFlashStatus[badgeWidgetName] = false;
		}
	}
}

// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ..................................................................:=++======++=-:.....................................................................
// ..............................................................:=+=-.   .::::::::-===+=:...............................................................
// ...........................................................:==-.               ....  .-===:...........................................................
// ........................................................:=+- ...                  .      .=+-.........................................................
// ......................................................:+=.:::          .          -+-     .:=+-.......................................................
// ....................................................:+=:=-.         :::.-.          -+=:    ::-+-.....................................................
// ...................................................=+-=:     .     .-  =              .-==-  := :+-...................................................
// .................................................:*--.             ----                 . .%-=#   -*:.................................................
// ................................................-* ..                   -.-        ..      *%%*   .-+=................................................
// ...............................................:* ::       .      .   .: -:          .:.   ==-+     --*...............................................
// ...............................................*.:- .+     :  .=+=.   .--              .- .+ -#:.    =:*..............................................
// ...............................................#:- :#*.:    -##%:  .=-:             .::::-:=  +#=    .=+:.............................................
// ..............................................:#+ .*@% %-  .%*:*.=*+-=          -.    ....:*.. %=    -.-=.............................................
// ....................................   .......:%. -@@@.@@: -#  + .   .+     ..  -:-==---.  *-  *.    = -=.............................................
// ....................................    ......:# :##@@-@@* %*+:-.    :-   .:  :-*=-:..     +   =  +.:- -=.............................................
// ..............................................:# -@@#%++*% %++=*+++=---..:==---:#+.  :..  :-     #* *  +-.............................................
// ...............................................# -+%@@#:=* #-==#:   .::-=-:::.  ::*::.=-:. =.   =@=.+  #-.............................................
// ...............................................# *=#@%:-*+:#==-..----:.     .:::--*---  :::.:* -@@-=:  %:.............................................
// ...............................................#+@#%@#-+=  +.::-==:   .:--==--++=---: :-. .:=+.%@%-+   #..............................................
// ...............................................*@%%@%--::.::  .:: .+==-==--:...:+=.     -::-=:-@@--*   #..............................................
// ...........................................:-:.=@%@#:::::   ::-. .. :*+---=-+#*=:.   :+=....:*+%@#-=. .*..............................................
// ..........................................*=:=*-@+*-     :-=-:.::     -#:.:#=:::  .:+-   .....=.@@@*- ==..............................................
// ..........................................#*#::%@=:=:*%#+-.   :: . .   -+  % .   --.      .:..  +#@@# #--+==+:........................................
// ..........................................**=%.+@#-:*%@@@@@@%#+-  .::   @ :# :  :.    .-*%%*:   .-@@%:@#=.=*:*........................................
// ..........................................*-%@#-%#.-#@@@@@#+==+##=:  ..+% =#       :+#@%@@@@@*-: *@@%#*.=@@@-*........................................
// ..........................................+=@@%#%:-*#%@@@%=-   :=#@@%%@@#=*%#+-=+#%*-. .=+%@@@@@+@@@@*.*@@@%-*........................................
// ...................................  .....-+%-#@=.+=-%@@@%*==-:-=#@@@@@+::-*@@@@@@*=-::--=#@@@@@%@##@*%@+=%=-+........................................
// .......................................... *=-@*##-.=#@@@@@%%##%%%@@@@*=:. *@@@@%@@#++=+*%@@@@%*=*-:@@@@*:%=+-........................................
// .........................................  .+=@-@* -*#%%%%%%%%#%%@@@%*= : -+%##@@##%@@@@@@@@#%*#::+.*@@%% +:#     ....................................
// .......................................     :+*@%=.+##%%%#%%%%%%%@#=--::. +.%*-%%++#*=+##**++=%=: ==-@%+#.-+:   ......................................
// ......................................        *+-+  +%%%%%%%%%%%*+-:+*..  = *+*=%#=-=+++=-: -*:-: .*-@#@+=+.     .....................................
// .....................................         =:.++=:*#**@*++=.  =-*=.   .   -##*=%+-------=+  :  ++%@@=+:        ....................................
// ....................................           =--.#+%@++++-:   :=*=+@%:.- =#+.%%**+*.    :*. . :**@##==-          ...................................
// ....................................               +:=%%#*:.=#==*:%##@@%.+=#@@:%%%+-:=-+*##---=#@= -#..*            ..................................
// ...................................                -=::-%:#+.:-# +==###*=#:%@%=##:*..#@@@#*@@@@@@:  .--             ..................................
// ...................................                :+.#=:#:%%=.:-+=.:=*%@##%@@#:*.# #%#@@*@%@@@*%                    .................................
// ..................................                 .* :%=-++*%+:#.:=.:#+==#%**.:* %:*#.=%@@-@@@-#                    .................................
// ..................................                  #  =#.*-+##.# :=--#= +%=+*.:# +-#+.=@@#:@@%:+                    .................................
// ..................................                 :% :-+-+==*#-=.#-=:#--=% :+:-+#:=+%-#@@=-##**=                    .................................
// ..................................               .*-#  .+.+++=*#%%%%@@%##%%#%@%@%#%*-*+%=@**-*-++=                   .................................
// ...................................             -+  -+:-=.-#.+%%%=%-# *..#. :%:*:*##%-%%-#@%:=*#:.*:                 .................................
// ...................................          :+%+. -%%*==-:%.%@#-.= * =  +  .+ = *=+*%%%##%%=##%%-:*+=.             ..................................
// ....................................     .-+%==-..-@@% ##.=-=#@@%+#*@%%*+#++*%#@#@@@@%%%%%%%%-+%@@.:+:*+:           ..................................
// ......................................-===.+-:: - #@@@ .%-+:*#+%@@%%%@@@@%@@@@%#%@@@#%%%%%+=%%%@@@+ .+ *=-==:.     ...................................
// .................................-===-. :.*= . -  @@@@.:*%--+++#@@+==:#:*:*-*:+=#@%=*%%+**=%@@+%@@#  :: +-  .-===-....................................
// ............................:=+==:..:::. =*    -  @@@@=.#@*..:%%%*#%##%*####%#%#@+-+%%%#%@@@@%:%@@#   =  += .    .-==++=:.............................
// .......................:-===-::::...    .*=   :.  #@@@=.*#@#. +%%%+-=*+%*+*=*--*+-*+*%%@@@@@%=-@@@+   -  :*:...        .=#-...........................
// .....................:=+-::::...  .......+*   -   =@@@=::**@%: -+*=+=+-==*%+===+#+=#*+%@@@@%-**@@@.   =  -+:.....      ...:--=:.......................
// .................::--:::.................+*   =    %@@=- +=+@@+. :=*#==*#+#=...=+*#-=@@@@@@##%@@@*   .+  =+...........    .....---....................
// ................:........................+*.  +    -@@+-.-*:=@@% : ==.  **-:    .-.*@@@@@@@@@@@@%.   .*  =*.................. .:..::::................
// .........................................-%:  *.    *@%*.-:#.=%@=::.-    #:.     :#@@@@@@@@@*@@%:    .% .+#...........................................
// .........................................:@-. #.  .  *@@%.=*#.-@@+------=+--:::-+%@@@@@@@@@+-@@: .    @:.**...........................................
// .........................................:@:.:+.   -  *@@*.#@= +@@@@%%%@@@%%##%@@@@*#@@@@@+ *%: ..    #+.++...........................................
// ..........................................%..*:     .  -%@ +@@: +@@@@@@@@@@@@@@@@@=+@@@@@=:#+.  .     +#.+=...........................................
// ..........................................-.:#          .*#=@@%. *@#@@@@@@@@@@@@%-=*@@@@***.=+        :#=.............................................
// ............................................+*.        .*:.+@@@#  *%=@@@@@@@@@@@-=+%@@@@#::#@#:       =*+.............................................
// ............................................=+*    .- :#@@#=.-#@+ :*=-@@@@@@@@@+-=%@@%=-:+@@@@@= :.   *++.............................................
// ............................................:+%=  ::.+@@%@@- .-@@-.-#:-@@@@@@@*:=%@@@+: .#%%%@@@*.:  -=:=.............................................
// .............................................+*#.-:+%%*+.*@+ .:@@%.=:%:+@@@@@#.+#@@@@=: *-:--:%+#@+. +:...............................................
// ...............................................*-=%#=..:.=@#+*=-=##:-:%.#@@@@:+#@@#=..+#-..*..-=.-#%=+................................................
// ...............................................+#*:......+%::-##+::=*:##:@@@*-%@*:--+###...........-##................................................
// ...............................................=:........=+:...-##*-..=%#*@@:*%--*#%+-.:.............=:...............................................
// ..........................................................:......:-+*= .*%@#.*-+#*-...................................................................
// .....................................................................:-..+##====:.....................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................
// ......................................................................................................................................................
