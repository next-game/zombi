/**
 * Created: 2024-12-06
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

enum InediaInfectedAI_LimbPainLevels {
	ZERO,
	LOW,
	MEDIUM,
	BADLY,
	CRITICAL,
}

enum InediaInfectedAI_LimbPainLevelsBorders {
	ZERO = 10,
	LOW = 30,
	MEDIUM = 50,
	BADLY = 90,
	CRITICAL = 100,
}

enum InediaInfectedAI_BulletInBodyStages {
	FIRST,
	SECOND,
}

enum InediaInfectedAI_LimbRegenerationLevels {
	BASE,
	LOW,
	MEDIUM,
	HIGH,
}

class InediaInfectedAI_PainManager
{
	static const float DEEP_WOUND_BLEEDING_RATE_MIN = 10;
	static const float DEEP_WOUND_BLEEDING_RATE_MAX = 30;
	static const float DEEP_WOUND_BLEEDING_RATE_HIT = 20;
	static const float DEEP_WOUND_BLEEDING_RATE_REGEN_ML_SEC = 0.05;
	static const float DEEP_WOUND_PAIN_TO_BLEEDING_MULTIPLIER = 0.03;
	
	static const int BULLET_COUNT_IN_LIMB_MAX = 3;
	
	PlayerBase m_Player;
	
	static ref array<string> m_InediaPainLimbs = {"head", "arms", "legs", "torso"};
	
	float m_InediaInfectedAI_PainHeadPercent = 0;
	float m_InediaInfectedAI_PainArmsPercent = 0;
	float m_InediaInfectedAI_PainLegsPercent = 0;
	float m_InediaInfectedAI_PainTorsoPercent = 0;
	InediaInfectedAI_LimbPainLevels m_PainHeadLevel = InediaInfectedAI_LimbPainLevels.ZERO;
	InediaInfectedAI_LimbPainLevels m_PainArmsLevel = InediaInfectedAI_LimbPainLevels.ZERO;
	InediaInfectedAI_LimbPainLevels m_PainLegsLevel = InediaInfectedAI_LimbPainLevels.ZERO;
	InediaInfectedAI_LimbPainLevels m_PainTorsoLevel = InediaInfectedAI_LimbPainLevels.ZERO;
	float m_PainHeadWithBuffsPercent = 0;
	float m_PainArmsWithBuffsPercent = 0;
	float m_PainLegsWithBuffsPercent = 0;
	float m_PainTorsoWithBuffsPercent = 0;
	InediaInfectedAI_LimbPainLevels m_PainHeadWithBuffsLevel = InediaInfectedAI_LimbPainLevels.ZERO;
	InediaInfectedAI_LimbPainLevels m_PainArmsWithBuffsLevel = InediaInfectedAI_LimbPainLevels.ZERO;
	InediaInfectedAI_LimbPainLevels m_PainLegsWithBuffsLevel = InediaInfectedAI_LimbPainLevels.ZERO;
	InediaInfectedAI_LimbPainLevels m_PainTorsoWithBuffsLevel = InediaInfectedAI_LimbPainLevels.ZERO;
	ref map<string, float> m_AllowBreakPainDropBelowBadlySeconds = new map<string, float>;
	ref map<string, float> m_AllowDeepWoundPainDropBelowBadlySeconds = new map<string, float>;
	
    float m_BreakHeadTimeRemainingSeconds = 0;
    float m_BreakArmsTimeRemainingSeconds = 0;
    float m_BreakTorsoTimeRemainingSeconds = 0;
	float m_HeadBreakStabilizationItemShockReductionMultiplier = 1;
	float m_ArmsBreakStabilizationItemShockReductionMultiplier = 1;
	float m_LegsBreakStabilizationItemShockReductionMultiplier = 1;
	float m_TorsoBreakStabilizationItemShockReductionMultiplier = 1;
	float m_HeadBreakStabilizationItemRegenerationMultiplier = 1;
	float m_ArmsBreakStabilizationItemRegenerationMultiplier = 1;
	float m_LegsBreakStabilizationItemRegenerationMultiplier = 1;
	float m_TorsoBreakStabilizationItemRegenerationMultiplier = 1;
	ref map<string, int> m_LimbsBreakRegenerationLevels = new map<string, int>;
	
    float m_HeadDeepWoundTimeRemainingSeconds = 0;
    float m_ArmsDeepWoundTimeRemainingSeconds = 0;
    float m_LegsDeepWoundTimeRemainingSeconds = 0;
    float m_TorsoDeepWoundTimeRemainingSeconds = 0;
    float m_HeadDeepWoundBleedingRate = 0;
    float m_ArmsDeepWoundBleedingRate = 0;
    float m_LegsDeepWoundBleedingRate = 0;
    float m_TorsoDeepWoundBleedingRate = 0;
	float m_HeadDeepWoundBandageItemRegenerationMultiplier = 1;
	float m_ArmsDeepWoundBandageItemRegenerationMultiplier = 1;
	float m_LegsDeepWoundBandageItemRegenerationMultiplier = 1;
	float m_TorsoDeepWoundBandageItemRegenerationMultiplier = 1;
	float m_HeadDeepWoundBandageItemEffectivenessTimeRemainingSeconds = 0;
	float m_ArmsDeepWoundBandageItemEffectivenessTimeRemainingSeconds = 0;
	float m_LegsDeepWoundBandageItemEffectivenessTimeRemainingSeconds = 0;
	float m_TorsoDeepWoundBandageItemEffectivenessTimeRemainingSeconds = 0;
	ref map<string, int> m_LimbsDeepWoundRegenerationLevels = new map<string, int>;
	
	float m_HeadBulletInBodySeconds = 0;
	float m_ArmsBulletInBodySeconds = 0;
	float m_LegsBulletInBodySeconds = 0;
	float m_TorsoBulletInBodySeconds = 0;
	
	bool m_ShowPainBadges = true;
	bool m_ShowMedicationInfo = true;
	bool m_ShowPainBlur = true;
	bool m_TinnitusWhenHeadReceivingHits = true;
	bool m_BlurWhenHeadReceivingHits = true;
	bool m_SoundAttenuationWhenHeadReceivingHits = true;
	bool m_WeaponSwayWhenHeadReceivingHits = true;
	
	float m_HealthRegenItemDurationSeconds;
	float m_HealthRegenItemRegenMultiplier;
	float m_BoneRegenItemDurationSeconds;
	float m_BoneRegenItemRegenMultiplier;
	float m_BloodRegenItemDurationSeconds;
	float m_BloodRegenItemRegenMultiplier;
	float m_BloodClottingItemDurationSeconds;
	float m_BloodClottingItemClottingMultiplier;
	float m_DeepWoundRegenItemDurationSeconds;
	float m_DeepWoundRegenItemRegenMultiplier;
	float m_VomitRelieItemDurationSeconds;
	float m_PainkillerLightItemDurationSeconds;
	float m_PainkillerMediumItemDurationSeconds;
	float m_PainkillerStrongItemDurationSeconds;
	
	float m_WeaponHoldingWhileArmsPainEnergyPercent = 100;
	
	ref map<string, ref array<string>> m_InediaPainLimbsEquipSlots = new map<string, ref array<string>>;
	
	ref map<string, string> m_InediaPainLimbMainBleedingZoneName = new map<string, string>;
	ref map<string, ref array<string>> m_InediaPainLimbAllBleedingZonesNames = new map<string, ref array<string>>;
	ref map<string, ref EffectParticle> m_InediaBleedingZonesBloodParticlesClient = new map<string, ref EffectParticle>;
	bool m_BleedingLayerIsActiveClient = false;
	
    void InediaInfectedAI_PainManager(PlayerBase player)
	{
		m_Player = player;
		
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("HeadBreakIsActive");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("ArmsBreakIsActive");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("TorsoBreakIsActive");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("HeadStabilizeIsActive");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("ArmsStabilizeIsActive");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("TorsoStabilizeIsActive");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("HeadDeepWoundIsActive");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("ArmsDeepWoundIsActive");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("LegsDeepWoundIsActive");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("TorsoDeepWoundIsActive");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("HeadDeepWoundBandageIsActive");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("ArmsDeepWoundBandageIsActive");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("LegsDeepWoundBandageIsActive");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("TorsoDeepWoundBandageIsActive");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("HeadDeepWoundBandageIsEffective");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("ArmsDeepWoundBandageIsEffective");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("LegsDeepWoundBandageIsEffective");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("TorsoDeepWoundBandageIsEffective");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("HeadBulletCountBit1");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("HeadBulletCountBit2");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("ArmsBulletCountBit1");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("ArmsBulletCountBit2");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("LegsBulletCountBit1");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("LegsBulletCountBit2");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("TorsoBulletCountBit1");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("TorsoBulletCountBit2");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables.Insert("IsInternalBleedingActive");
		
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables2.Insert("PainSystemIsActive");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables2.Insert("LimbsBreakSystemIsActive");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables2.Insert("LimbsDeepWoundSystemIsActive");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables2.Insert("LimbsBulletSystemIsActive");
		m_Player.m_InediaInfectedAI_NetSyncBoolVariables2.Insert("InternalBleedingSystemIsActive");
		
		m_InediaPainLimbsEquipSlots["head"] = {"Headgear", "Mask", "Eyewear"};
		m_InediaPainLimbsEquipSlots["arms"] = {"Body", "Gloves"};
		m_InediaPainLimbsEquipSlots["legs"] = {"Legs", "Feet"};
		m_InediaPainLimbsEquipSlots["torso"] = {"Vest", "Body"};
		
		m_InediaPainLimbMainBleedingZoneName["head"] = "Neck";
		m_InediaPainLimbMainBleedingZoneName["arms"] = "RightForeArm";
		m_InediaPainLimbMainBleedingZoneName["legs"] = "RightUpLegRoll";
		m_InediaPainLimbMainBleedingZoneName["torso"] = "Pelvis";
		
		m_InediaPainLimbAllBleedingZonesNames["head"] = {"Head", "Neck"};
		m_InediaPainLimbAllBleedingZonesNames["arms"] = {"LeftShoulder", "LeftArm", "LeftArmRoll", "LeftForeArm", "RightShoulder", "RightArm", "RightArmRoll", "RightForeArm", "LeftForeArmRoll", "RightForeArmRoll"};
		m_InediaPainLimbAllBleedingZonesNames["legs"] = {"LeftLeg", "LeftLegRoll", "LeftUpLeg", "LeftUpLegRoll", "RightLeg", "RightLegRoll", "RightUpLeg", "RightUpLegRoll", "LeftFoot", "LeftToeBase", "RightFoot", "RightToeBase"};
		m_InediaPainLimbAllBleedingZonesNames["torso"] = {"Pelvis", "Spine", "Spine1", "Spine2", "Spine3"};
		
		foreach (array<string> bleedingZones: m_InediaPainLimbAllBleedingZonesNames) {
			foreach (string bleedingZoneName: bleedingZones) {
				m_InediaBleedingZonesBloodParticlesClient[bleedingZoneName] = null;
			}
		}
		
		foreach (string inediaPainLimb: InediaInfectedAI_PainManager.m_InediaPainLimbs) {
			m_LimbsBreakRegenerationLevels[inediaPainLimb] = 0;
			m_LimbsDeepWoundRegenerationLevels[inediaPainLimb] = 0;
			m_AllowBreakPainDropBelowBadlySeconds[inediaPainLimb] = 0;
			m_AllowDeepWoundPainDropBelowBadlySeconds[inediaPainLimb] = 0;
		}
	}
	
	void InitConfig()
	{
		if (!m_Player.m_InediaInfectedAI_ConfigPlayers) {
			return;
		}

		m_Player.InediaInfectedAI_SetNetSyncBoolVariable2("PainSystemIsActive", m_Player.m_InediaInfectedAI_ConfigPlayers.PainSystemIsActive);
		
		if (m_Player.m_InediaInfectedAI_ConfigPlayers.PainSystemIsActive) {
			m_Player.InediaInfectedAI_SetNetSyncBoolVariable2("LimbsBreakSystemIsActive", m_Player.m_InediaInfectedAI_ConfigPlayers.LimbsBreakSystemIsActive);
			m_Player.InediaInfectedAI_SetNetSyncBoolVariable2("LimbsDeepWoundSystemIsActive", m_Player.m_InediaInfectedAI_ConfigPlayers.LimbsDeepWoundSystemIsActive);
			m_Player.InediaInfectedAI_SetNetSyncBoolVariable2("LimbsBulletSystemIsActive", m_Player.m_InediaInfectedAI_ConfigPlayers.LimbsBulletSystemIsActive);
			m_Player.InediaInfectedAI_SetNetSyncBoolVariable2("InternalBleedingSystemIsActive", m_Player.m_InediaInfectedAI_ConfigPlayers.InternalBleedingSystemIsActive);
			
			m_ShowPainBadges = m_Player.m_InediaInfectedAI_ConfigPlayers.ShowPainBadges;
			m_ShowMedicationInfo = m_Player.m_InediaInfectedAI_ConfigPlayers.ShowMedicationInfo;
			m_ShowPainBlur = m_Player.m_InediaInfectedAI_ConfigPlayers.ShowPainBlur;
			m_TinnitusWhenHeadReceivingHits = m_Player.m_InediaInfectedAI_ConfigPlayers.TinnitusWhenHeadReceivingHits;
			m_BlurWhenHeadReceivingHits = m_Player.m_InediaInfectedAI_ConfigPlayers.BlurWhenHeadReceivingHits;
			m_SoundAttenuationWhenHeadReceivingHits = m_Player.m_InediaInfectedAI_ConfigPlayers.SoundAttenuationWhenHeadReceivingHits;
			m_WeaponSwayWhenHeadReceivingHits = m_Player.m_InediaInfectedAI_ConfigPlayers.WeaponSwayWhenHeadReceivingHits;
		} else {
			m_Player.InediaInfectedAI_SetNetSyncBoolVariable2("LimbsBreakSystemIsActive", false);
			m_Player.InediaInfectedAI_SetNetSyncBoolVariable2("LimbsDeepWoundSystemIsActive", false);
			m_Player.InediaInfectedAI_SetNetSyncBoolVariable2("LimbsBulletSystemIsActive", false);
			m_Player.InediaInfectedAI_SetNetSyncBoolVariable2("InternalBleedingSystemIsActive", false);
			
			m_ShowPainBadges = false;
			m_ShowMedicationInfo = false;
			m_ShowPainBlur = false;
			m_TinnitusWhenHeadReceivingHits = false;
			m_BlurWhenHeadReceivingHits = false;
			m_SoundAttenuationWhenHeadReceivingHits = false;
			m_WeaponSwayWhenHeadReceivingHits = false;
		}
	}
	
	void SyncConfigVariablesServer(ParamsWriteContext ctx)
	{
	    ctx.Write(m_ShowPainBadges);
	    ctx.Write(m_ShowMedicationInfo);
	    ctx.Write(m_ShowPainBlur);
	    ctx.Write(m_TinnitusWhenHeadReceivingHits);
	    ctx.Write(m_BlurWhenHeadReceivingHits);
	    ctx.Write(m_SoundAttenuationWhenHeadReceivingHits);
	    ctx.Write(m_WeaponSwayWhenHeadReceivingHits);
	}
	
	void SyncConfigVariablesClient(ParamsReadContext ctx)
	{
		ctx.Read(m_ShowPainBadges);
		ctx.Read(m_ShowMedicationInfo);
		ctx.Read(m_ShowPainBlur);
		ctx.Read(m_TinnitusWhenHeadReceivingHits);
		ctx.Read(m_BlurWhenHeadReceivingHits);
		ctx.Read(m_SoundAttenuationWhenHeadReceivingHits);
		ctx.Read(m_WeaponSwayWhenHeadReceivingHits);
	}
	
	void OnStoreSave(InediaInfectedAI_PlayerStorage playerStorage)
	{
		if (!playerStorage) {
			return;
		}
		
		playerStorage.Set("m_InediaInfectedAI_PainHeadPercent", m_InediaInfectedAI_PainHeadPercent);
		playerStorage.Set("m_InediaInfectedAI_PainArmsPercent", m_InediaInfectedAI_PainArmsPercent);
		playerStorage.Set("m_InediaInfectedAI_PainLegsPercent", m_InediaInfectedAI_PainLegsPercent);
		playerStorage.Set("m_InediaInfectedAI_PainTorsoPercent", m_InediaInfectedAI_PainTorsoPercent);
		
		playerStorage.Set("m_BreakHeadTimeRemainingSeconds", m_BreakHeadTimeRemainingSeconds);
		playerStorage.Set("m_BreakArmsTimeRemainingSeconds", m_BreakArmsTimeRemainingSeconds);
		playerStorage.Set("m_BreakTorsoTimeRemainingSeconds", m_BreakTorsoTimeRemainingSeconds);
		
		playerStorage.Set("HeadStabilizeIsActive", IsLimbStabilized("head"));
		playerStorage.Set("ArmsStabilizeIsActive", IsLimbStabilized("arms"));
		playerStorage.Set("TorsoStabilizeIsActive", IsLimbStabilized("torso"));
		
		playerStorage.Set("m_HeadBreakStabilizationItemShockReductionMultiplier", m_HeadBreakStabilizationItemShockReductionMultiplier);
		playerStorage.Set("m_ArmsBreakStabilizationItemShockReductionMultiplier", m_ArmsBreakStabilizationItemShockReductionMultiplier);
		playerStorage.Set("m_LegsBreakStabilizationItemShockReductionMultiplier", m_LegsBreakStabilizationItemShockReductionMultiplier);
		playerStorage.Set("m_TorsoBreakStabilizationItemShockReductionMultiplier", m_TorsoBreakStabilizationItemShockReductionMultiplier);
		
		playerStorage.Set("m_HeadBreakStabilizationItemRegenerationMultiplier", m_HeadBreakStabilizationItemRegenerationMultiplier);
		playerStorage.Set("m_ArmsBreakStabilizationItemRegenerationMultiplier", m_ArmsBreakStabilizationItemRegenerationMultiplier);
		playerStorage.Set("m_LegsBreakStabilizationItemRegenerationMultiplier", m_LegsBreakStabilizationItemRegenerationMultiplier);
		playerStorage.Set("m_TorsoBreakStabilizationItemRegenerationMultiplier", m_TorsoBreakStabilizationItemRegenerationMultiplier);
		
		playerStorage.Set("m_HeadDeepWoundTimeRemainingSeconds", m_HeadDeepWoundTimeRemainingSeconds);
		playerStorage.Set("m_ArmsDeepWoundTimeRemainingSeconds", m_ArmsDeepWoundTimeRemainingSeconds);
		playerStorage.Set("m_LegsDeepWoundTimeRemainingSeconds", m_LegsDeepWoundTimeRemainingSeconds);
		playerStorage.Set("m_TorsoDeepWoundTimeRemainingSeconds", m_TorsoDeepWoundTimeRemainingSeconds);
		
		playerStorage.Set("m_HeadDeepWoundBleedingRate", m_HeadDeepWoundBleedingRate);
		playerStorage.Set("m_ArmsDeepWoundBleedingRate", m_ArmsDeepWoundBleedingRate);
		playerStorage.Set("m_LegsDeepWoundBleedingRate", m_LegsDeepWoundBleedingRate);
		playerStorage.Set("m_TorsoDeepWoundBleedingRate", m_TorsoDeepWoundBleedingRate);
		
		playerStorage.Set("HeadDeepWoundBandageIsActive", IsLimbDeepWoundBandaged("head"));
		playerStorage.Set("ArmsDeepWoundBandageIsActive", IsLimbDeepWoundBandaged("arms"));
		playerStorage.Set("LegsDeepWoundBandageIsActive", IsLimbDeepWoundBandaged("legs"));
		playerStorage.Set("TorsoDeepWoundBandageIsActive", IsLimbDeepWoundBandaged("torso"));
		
		playerStorage.Set("m_HeadDeepWoundBandageItemEffectivenessTimeRemainingSeconds", m_HeadDeepWoundBandageItemEffectivenessTimeRemainingSeconds);
		playerStorage.Set("m_ArmsDeepWoundBandageItemEffectivenessTimeRemainingSeconds", m_ArmsDeepWoundBandageItemEffectivenessTimeRemainingSeconds);
		playerStorage.Set("m_LegsDeepWoundBandageItemEffectivenessTimeRemainingSeconds", m_LegsDeepWoundBandageItemEffectivenessTimeRemainingSeconds);
		playerStorage.Set("m_TorsoDeepWoundBandageItemEffectivenessTimeRemainingSeconds", m_TorsoDeepWoundBandageItemEffectivenessTimeRemainingSeconds);
		playerStorage.Set("m_HeadDeepWoundBandageItemRegenerationMultiplier", m_HeadDeepWoundBandageItemRegenerationMultiplier);
		playerStorage.Set("m_ArmsDeepWoundBandageItemRegenerationMultiplier", m_ArmsDeepWoundBandageItemRegenerationMultiplier);
		playerStorage.Set("m_LegsDeepWoundBandageItemRegenerationMultiplier", m_LegsDeepWoundBandageItemRegenerationMultiplier);
		playerStorage.Set("m_TorsoDeepWoundBandageItemRegenerationMultiplier", m_TorsoDeepWoundBandageItemRegenerationMultiplier);
		
		playerStorage.Set("HeadBulletCount", GetBulletCountInLimb("head"));
		playerStorage.Set("ArmsBulletCount", GetBulletCountInLimb("arms"));
		playerStorage.Set("LegsBulletCount", GetBulletCountInLimb("legs"));
		playerStorage.Set("TorsoBulletCount", GetBulletCountInLimb("torso"));
		playerStorage.Set("m_HeadBulletInBodySeconds", GetBulletInBodySecondsForLimb("head"));
		playerStorage.Set("m_ArmsBulletInBodySeconds", GetBulletInBodySecondsForLimb("arms"));
		playerStorage.Set("m_LegsBulletInBodySeconds", GetBulletInBodySecondsForLimb("legs"));
		playerStorage.Set("m_TorsoBulletInBodySeconds", GetBulletInBodySecondsForLimb("torso"));
		
		playerStorage.Set("IsInternalBleedingActive", IsInternalBleedingActive());
		
		playerStorage.Set("m_HealthRegenItemDurationSeconds", m_HealthRegenItemDurationSeconds);
		playerStorage.Set("m_HealthRegenItemRegenMultiplier", m_HealthRegenItemRegenMultiplier);
		playerStorage.Set("m_BoneRegenItemDurationSeconds", m_BoneRegenItemDurationSeconds);
		playerStorage.Set("m_BoneRegenItemRegenMultiplier", m_BoneRegenItemRegenMultiplier);
		playerStorage.Set("m_BloodRegenItemDurationSeconds", m_BloodRegenItemDurationSeconds);
		playerStorage.Set("m_BloodRegenItemRegenMultiplier", m_BloodRegenItemRegenMultiplier);
		playerStorage.Set("m_BloodClottingItemDurationSeconds", m_BloodClottingItemDurationSeconds);
		playerStorage.Set("m_BloodClottingItemClottingMultiplier", m_BloodClottingItemClottingMultiplier);
		playerStorage.Set("m_DeepWoundRegenItemDurationSeconds", m_DeepWoundRegenItemDurationSeconds);
		playerStorage.Set("m_DeepWoundRegenItemRegenMultiplier", m_DeepWoundRegenItemRegenMultiplier);
		playerStorage.Set("m_VomitRelieItemDurationSeconds", m_VomitRelieItemDurationSeconds);
		playerStorage.Set("m_PainkillerLightItemDurationSeconds", m_PainkillerLightItemDurationSeconds);
		playerStorage.Set("m_PainkillerMediumItemDurationSeconds", m_PainkillerMediumItemDurationSeconds);
		playerStorage.Set("m_PainkillerStrongItemDurationSeconds", m_PainkillerStrongItemDurationSeconds);
	}
	
	void OnStoreLoad(InediaInfectedAI_PlayerStorage playerStorage)
	{
		if (!playerStorage) {
			return;
		}
		
        m_InediaInfectedAI_PainHeadPercent = playerStorage.Get("m_InediaInfectedAI_PainHeadPercent");
        m_InediaInfectedAI_PainArmsPercent = playerStorage.Get("m_InediaInfectedAI_PainArmsPercent");
        m_InediaInfectedAI_PainLegsPercent = playerStorage.Get("m_InediaInfectedAI_PainLegsPercent");
        m_InediaInfectedAI_PainTorsoPercent = playerStorage.Get("m_InediaInfectedAI_PainTorsoPercent");
        
        m_BreakHeadTimeRemainingSeconds = playerStorage.Get("m_BreakHeadTimeRemainingSeconds");
        m_BreakArmsTimeRemainingSeconds = playerStorage.Get("m_BreakArmsTimeRemainingSeconds");
        m_BreakTorsoTimeRemainingSeconds = playerStorage.Get("m_BreakTorsoTimeRemainingSeconds");
        
        m_Player.InediaInfectedAI_SetNetSyncBoolVariable("HeadStabilizeIsActive", playerStorage.Get("HeadStabilizeIsActive") != 0);
        m_Player.InediaInfectedAI_SetNetSyncBoolVariable("ArmsStabilizeIsActive", playerStorage.Get("ArmsStabilizeIsActive") != 0);
        m_Player.InediaInfectedAI_SetNetSyncBoolVariable("TorsoStabilizeIsActive", playerStorage.Get("TorsoStabilizeIsActive") != 0);
		
		m_HeadBreakStabilizationItemShockReductionMultiplier = playerStorage.Get("m_HeadBreakStabilizationItemShockReductionMultiplier");
        m_ArmsBreakStabilizationItemShockReductionMultiplier = playerStorage.Get("m_ArmsBreakStabilizationItemShockReductionMultiplier");
        m_LegsBreakStabilizationItemShockReductionMultiplier = playerStorage.Get("m_LegsBreakStabilizationItemShockReductionMultiplier");
        m_TorsoBreakStabilizationItemShockReductionMultiplier = playerStorage.Get("m_TorsoBreakStabilizationItemShockReductionMultiplier");
		
		m_HeadBreakStabilizationItemRegenerationMultiplier = playerStorage.Get("m_HeadBreakStabilizationItemRegenerationMultiplier");
        m_ArmsBreakStabilizationItemRegenerationMultiplier = playerStorage.Get("m_ArmsBreakStabilizationItemRegenerationMultiplier");
        m_LegsBreakStabilizationItemRegenerationMultiplier = playerStorage.Get("m_LegsBreakStabilizationItemRegenerationMultiplier");
        m_TorsoBreakStabilizationItemRegenerationMultiplier = playerStorage.Get("m_TorsoBreakStabilizationItemRegenerationMultiplier");
        
        m_HeadDeepWoundTimeRemainingSeconds = playerStorage.Get("m_HeadDeepWoundTimeRemainingSeconds");
        m_ArmsDeepWoundTimeRemainingSeconds = playerStorage.Get("m_ArmsDeepWoundTimeRemainingSeconds");
        m_LegsDeepWoundTimeRemainingSeconds = playerStorage.Get("m_LegsDeepWoundTimeRemainingSeconds");
        m_TorsoDeepWoundTimeRemainingSeconds = playerStorage.Get("m_TorsoDeepWoundTimeRemainingSeconds");
		
		m_HeadDeepWoundBleedingRate = playerStorage.Get("m_HeadDeepWoundBleedingRate");
        m_ArmsDeepWoundBleedingRate = playerStorage.Get("m_ArmsDeepWoundBleedingRate");
        m_LegsDeepWoundBleedingRate = playerStorage.Get("m_LegsDeepWoundBleedingRate");
        m_TorsoDeepWoundBleedingRate = playerStorage.Get("m_TorsoDeepWoundBleedingRate");
        
        m_Player.InediaInfectedAI_SetNetSyncBoolVariable("HeadDeepWoundBandageIsActive", playerStorage.Get("HeadDeepWoundBandageIsActive") != 0);
        m_Player.InediaInfectedAI_SetNetSyncBoolVariable("ArmsDeepWoundBandageIsActive", playerStorage.Get("ArmsDeepWoundBandageIsActive") != 0);
        m_Player.InediaInfectedAI_SetNetSyncBoolVariable("LegsDeepWoundBandageIsActive", playerStorage.Get("LegsDeepWoundBandageIsActive") != 0);
        m_Player.InediaInfectedAI_SetNetSyncBoolVariable("TorsoDeepWoundBandageIsActive", playerStorage.Get("TorsoDeepWoundBandageIsActive") != 0);
		
        m_HeadDeepWoundBandageItemEffectivenessTimeRemainingSeconds = playerStorage.Get("m_HeadDeepWoundBandageItemEffectivenessTimeRemainingSeconds");
        m_ArmsDeepWoundBandageItemEffectivenessTimeRemainingSeconds = playerStorage.Get("m_ArmsDeepWoundBandageItemEffectivenessTimeRemainingSeconds");
        m_LegsDeepWoundBandageItemEffectivenessTimeRemainingSeconds = playerStorage.Get("m_LegsDeepWoundBandageItemEffectivenessTimeRemainingSeconds");
        m_TorsoDeepWoundBandageItemEffectivenessTimeRemainingSeconds = playerStorage.Get("m_TorsoDeepWoundBandageItemEffectivenessTimeRemainingSeconds");
        m_HeadDeepWoundBandageItemRegenerationMultiplier = playerStorage.Get("m_HeadDeepWoundBandageItemRegenerationMultiplier");
        m_ArmsDeepWoundBandageItemRegenerationMultiplier = playerStorage.Get("m_ArmsDeepWoundBandageItemRegenerationMultiplier");
        m_LegsDeepWoundBandageItemRegenerationMultiplier = playerStorage.Get("m_LegsDeepWoundBandageItemRegenerationMultiplier");
        m_TorsoDeepWoundBandageItemRegenerationMultiplier = playerStorage.Get("m_TorsoDeepWoundBandageItemRegenerationMultiplier");
        
		SetBulletCountInLimb("head", playerStorage.Get("HeadBulletCount"));
		SetBulletCountInLimb("arms", playerStorage.Get("ArmsBulletCount"));
		SetBulletCountInLimb("legs", playerStorage.Get("LegsBulletCount"));
		SetBulletCountInLimb("torso", playerStorage.Get("TorsoBulletCount"));
		
        m_HeadBulletInBodySeconds = playerStorage.Get("m_HeadBulletInBodySeconds");
        m_ArmsBulletInBodySeconds = playerStorage.Get("m_ArmsBulletInBodySeconds");
        m_LegsBulletInBodySeconds = playerStorage.Get("m_LegsBulletInBodySeconds");
        m_TorsoBulletInBodySeconds = playerStorage.Get("m_TorsoBulletInBodySeconds");
		
		m_Player.InediaInfectedAI_SetNetSyncBoolVariable("IsInternalBleedingActive", playerStorage.Get("IsInternalBleedingActive") != 0);
		
        m_HealthRegenItemDurationSeconds = playerStorage.Get("m_HealthRegenItemDurationSeconds");
        m_HealthRegenItemRegenMultiplier = playerStorage.Get("m_HealthRegenItemRegenMultiplier");
        m_BoneRegenItemDurationSeconds = playerStorage.Get("m_BoneRegenItemDurationSeconds");
        m_BoneRegenItemRegenMultiplier = playerStorage.Get("m_BoneRegenItemRegenMultiplier");
        m_BloodRegenItemDurationSeconds = playerStorage.Get("m_BloodRegenItemDurationSeconds");
        m_BloodRegenItemRegenMultiplier = playerStorage.Get("m_BloodRegenItemRegenMultiplier");
        m_BloodClottingItemDurationSeconds = playerStorage.Get("m_BloodClottingItemDurationSeconds");
        m_BloodClottingItemClottingMultiplier = playerStorage.Get("m_BloodClottingItemClottingMultiplier");
        m_DeepWoundRegenItemDurationSeconds = playerStorage.Get("m_DeepWoundRegenItemDurationSeconds");
        m_DeepWoundRegenItemRegenMultiplier = playerStorage.Get("m_DeepWoundRegenItemRegenMultiplier");
        m_VomitRelieItemDurationSeconds = playerStorage.Get("m_VomitRelieItemDurationSeconds");
        m_PainkillerLightItemDurationSeconds = playerStorage.Get("m_PainkillerLightItemDurationSeconds");
        m_PainkillerMediumItemDurationSeconds = playerStorage.Get("m_PainkillerMediumItemDurationSeconds");
        m_PainkillerStrongItemDurationSeconds = playerStorage.Get("m_PainkillerStrongItemDurationSeconds");
	}
	
	void OnPlayerBaseRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
#ifndef SERVER		
		switch (rpc_type) {
			case InediaInfectedAI_PlayerBase_ERPCs.RPC_SYNC_LIMBS_DEEP_WOUND_REGENERATION_LEVELS:
				ctx.Read(m_LimbsDeepWoundRegenerationLevels);
			break;
			
			case InediaInfectedAI_PlayerBase_ERPCs.RPC_SYNC_BREAK_REGENERATION_LEVELS:
				ctx.Read(m_LimbsBreakRegenerationLevels);
			break;
			
			case InediaInfectedAI_PlayerBase_ERPCs.RPC_SYNC_PAIN_LEVELS:
				ctx.Read(m_PainHeadLevel);
				ctx.Read(m_PainArmsLevel);
				ctx.Read(m_PainLegsLevel);
				ctx.Read(m_PainTorsoLevel);
				ctx.Read(m_PainHeadWithBuffsLevel);
				ctx.Read(m_PainArmsWithBuffsLevel);
				ctx.Read(m_PainLegsWithBuffsLevel);
				ctx.Read(m_PainTorsoWithBuffsLevel);
			break;
			
			case InediaInfectedAI_PlayerBase_ERPCs.RPC_SYNC_STABILIZATION_SHOCK_REDUCTION_MULTIPLIER:
				string limb;
				float shockReductionMultiplier;
			
				ctx.Read(limb);
				ctx.Read(shockReductionMultiplier);
				
				SetBreakStabilizationShockReductionMultiplierForLimb(limb, shockReductionMultiplier);
			break;
			
			case InediaInfectedAI_PlayerBase_ERPCs.RPC_HEAD_HIT_ACTION:
				if (!m_Player.IsAlive() || m_Player.IsUnconscious()) {
					return;
				}
			
				float powerMultiplier = 0;
				if (ctx.Read(powerMultiplier) && powerMultiplier > 0) {
					if (m_SoundAttenuationWhenHeadReceivingHits) {
						GetGame().GetSoundScene().SetSoundVolume(0.3, 1);
						m_Player.SetMasterAttenuation("FlashbangAttenuation");
						GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RestoreHeadHitSoundEffects, powerMultiplier * 10 * 1000, false);
					}
			
					if (m_TinnitusWhenHeadReceivingHits) {
						EffectSound tinnitusSound = SEffectManager.PlaySoundOnObject("inedia_infectedai_tinnitus_SoundSet", m_Player);
			           	tinnitusSound.SetAutodestroy(true);
					}
				
					if (m_BlurWhenHeadReceivingHits) {
						m_HeadHitBlurRequesterIsActive = true;
						GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(HeadHitBlurRequesterStopClient, powerMultiplier * 10 * 1000, false);
					}
				
					if (m_WeaponSwayWhenHeadReceivingHits) {
						m_HeadHitWeaponSwayIsActive = true;
						GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(HeadHitWeaponSwayStopClient, powerMultiplier * 15 * 1000, false);
					}
				}
			break;
		}
#endif
	}
	
	int m_InediaInfectedAI_NetSyncBoolVariablesSyncBitsLocal = 0;
	int m_InediaInfectedAI_NetSyncBoolVariables2SyncBitsLocal = 0;
	int m_InediaInfectedAI_DeepWoundsBleedingsSyncBitsLocal = 0;
	void OnVariablesSynchronized()
	{
#ifndef SERVER
		if (m_InediaInfectedAI_NetSyncBoolVariablesSyncBitsLocal != m_Player.m_InediaInfectedAI_NetSyncBoolVariablesSyncBits || m_InediaInfectedAI_NetSyncBoolVariables2SyncBitsLocal != m_Player.m_InediaInfectedAI_NetSyncBoolVariables2SyncBits || m_InediaInfectedAI_DeepWoundsBleedingsSyncBitsLocal != m_Player.m_InediaInfectedAI_DeepWoundsBleedingsSyncBits) {
			m_InediaInfectedAI_NetSyncBoolVariablesSyncBitsLocal = m_Player.m_InediaInfectedAI_NetSyncBoolVariablesSyncBits;
			m_InediaInfectedAI_NetSyncBoolVariables2SyncBitsLocal = m_Player.m_InediaInfectedAI_NetSyncBoolVariables2SyncBits;
			m_InediaInfectedAI_DeepWoundsBleedingsSyncBitsLocal = m_Player.m_InediaInfectedAI_DeepWoundsBleedingsSyncBits;

			UpdateBloodParticlesClient();
		}
#endif
	}
	
	void OnTickServer(float pDt)
	{
		foreach (string inediaPainLimb: InediaInfectedAI_PainManager.m_InediaPainLimbs) {
			if (m_AllowBreakPainDropBelowBadlySeconds[inediaPainLimb] > 0) {
				m_AllowBreakPainDropBelowBadlySeconds[inediaPainLimb] = m_AllowBreakPainDropBelowBadlySeconds[inediaPainLimb] - pDt;
			}
			
			if (m_AllowDeepWoundPainDropBelowBadlySeconds[inediaPainLimb] > 0) {
				m_AllowDeepWoundPainDropBelowBadlySeconds[inediaPainLimb] = m_AllowDeepWoundPainDropBelowBadlySeconds[inediaPainLimb] - pDt;
			}
		}
	}
	
	void OnTickClient(float pDt)
	{
		/*if (IsControlledPlayer()) {
			//
		}*/
	}
	
	bool PainSystemIsActive()
	{
		return m_Player.InediaInfectedAI_GetNetSyncBoolVariable2("PainSystemIsActive");
	}
	
	bool LimbsBreakSystemIsActive()
	{
		return m_Player.InediaInfectedAI_GetNetSyncBoolVariable2("LimbsBreakSystemIsActive");
	}
	
	bool LimbsDeepWoundSystemIsActive()
	{
		return m_Player.InediaInfectedAI_GetNetSyncBoolVariable2("LimbsDeepWoundSystemIsActive");
	}
	
	bool LimbsBulletSystemIsActive()
	{
		return m_Player.InediaInfectedAI_GetNetSyncBoolVariable2("LimbsBulletSystemIsActive");
	}
	
	bool InternalBleedingSystemIsActive()
	{
		return m_Player.InediaInfectedAI_GetNetSyncBoolVariable2("InternalBleedingSystemIsActive");
	}
	
	eInjuryHandlerLevels InediaInfectedAI_GetInjuryLevel()
	{
		if (!PainSystemIsActive()) {
			return eInjuryHandlerLevels.PRISTINE;
		}

		eInjuryHandlerLevels maxInjuryLevel = eInjuryHandlerLevels.PRISTINE;
		
		foreach (string inediaPainLimb: InediaInfectedAI_PainManager.m_InediaPainLimbs) {
			InediaInfectedAI_LimbPainLevels painLevel = GetPainWithBuffsLevelForLimb(inediaPainLimb);
			eInjuryHandlerLevels currentInjuryLevel = eInjuryHandlerLevels.PRISTINE;
			
			if (inediaPainLimb == "legs") {
				// A separate slowing logic is used for legs.
				if (painLevel >= InediaInfectedAI_LimbPainLevels.CRITICAL) {
					currentInjuryLevel = eInjuryHandlerLevels.RUINED;
				} else if (painLevel >= InediaInfectedAI_LimbPainLevels.BADLY) {
					currentInjuryLevel = eInjuryHandlerLevels.BADLY_DAMAGED;
				} else if (painLevel >= InediaInfectedAI_LimbPainLevels.MEDIUM) {
					currentInjuryLevel = eInjuryHandlerLevels.DAMAGED;
				} else if (painLevel >= InediaInfectedAI_LimbPainLevels.LOW) {
					currentInjuryLevel = eInjuryHandlerLevels.DAMAGED;
				}
			} else {
				if (painLevel >= InediaInfectedAI_LimbPainLevels.BADLY) {
				    currentInjuryLevel = eInjuryHandlerLevels.WORN;
				} else if (painLevel >= InediaInfectedAI_LimbPainLevels.MEDIUM) {
				    currentInjuryLevel = eInjuryHandlerLevels.WORN;
				} /*else if (painLevel >= InediaInfectedAI_LimbPainLevels.LOW) {
				    currentInjuryLevel = eInjuryHandlerLevels.DAMAGED;
				}*/
			}

			if (painLevel >= InediaInfectedAI_LimbPainLevels.LOW) {
				if (inediaPainLimb == "torso" && IsInternalBleedingActive()) {
					currentInjuryLevel = Math.Clamp(currentInjuryLevel + 1, eInjuryHandlerLevels.PRISTINE, eInjuryHandlerLevels.RUINED);
				}

				if (IsLimbBroken(inediaPainLimb) && !IsLimbStabilized(inediaPainLimb)) {
					currentInjuryLevel = Math.Clamp(currentInjuryLevel + 1, eInjuryHandlerLevels.PRISTINE, eInjuryHandlerLevels.RUINED);
				}

				if (GetBulletCountInLimb(inediaPainLimb) > 0) {
					currentInjuryLevel = Math.Clamp(currentInjuryLevel + 1, eInjuryHandlerLevels.PRISTINE, eInjuryHandlerLevels.RUINED);
				}
			}

			if (currentInjuryLevel > maxInjuryLevel) {
				maxInjuryLevel = currentInjuryLevel;
			}
		}

		return maxInjuryLevel;
	}
	
	float m_EEHitByLimbsBreakChancesMultiplier = 1;
	void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		m_EEHitByLimbsBreakChancesMultiplier = 1;
		
		ProcessPainDamage(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		
		m_EEHitByLimbsBreakChancesMultiplier = 1;
	}
	
	void ProcessPainDamage(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!m_Player.IsAlive() || !damageResult || !source || !PainSystemIsActive() || !m_Player.m_InediaInfectedAI_ConfigZombie || !m_Player.m_InediaInfectedAI_ConfigPlayers) {
			return;
		}
		
		bool isBleedingHit = (m_Player.GetBleedingSourceCount() - m_Player.m_InediaInfectedAI_EEHitByBleedingSourceCountBeforeHit) > 0;
		
		// Heal the character's legs since the leg injury is processed by the Inedia pain system.
		if (!m_Player.GetModifiersManager().IsModifierActive(eModifiers.MDF_BROKEN_LEGS)) {
			if (dmgZone == "LeftLeg" || dmgZone == "RightLeg" || dmgZone == "LeftFoot" || dmgZone == "RightFoot") {
				m_Player.SetHealth(dmgZone, "Health", m_Player.GetMaxHealth(dmgZone, "Health"));
			}
		}

		string inediaPainLimb = GetInediaPainLimbFromDmgZone(dmgZone);

		float painDamage = damageResult.GetDamage("", "Shock") * m_Player.m_InediaInfectedAI_WeaponMultiplier;

		if (source.IsInherited(ZombieBase)) {
			// Pain from zombies

			ZombieBase zmb = ZombieBase.Cast(source);
			if (!zmb || !zmb.m_InediaInfectedAI_ConfigZombie || !zmb.m_InediaInfectedAI_ConfigZombie.PainToPlayerHandlerIsActive) {
				return;
			}

			float zmbLimbPainMultiplier = zmb.InediaInfectedAI_GetPainToPlayerLimbMultiplier(m_Player, inediaPainLimb, damageType);
			if (zmbLimbPainMultiplier <= 0) {
				return;
			}
			
			m_EEHitByLimbsBreakChancesMultiplier = zmb.m_InediaInfectedAI_ConfigZombie.PainToPlayerLimbsBreakChancesMultiplier;

			if (inediaPainLimb == "head") {
				// Crawlers should not attack the head, except when the player is lying down
				if (zmb.IsCrawling() && !m_Player.InediaInfectedAI_IsInProneStance()) {
					inediaPainLimb = "legs";
					painDamage = GetInediaLimbAmmoShockDamage("legs", ammo, damageType) * m_Player.m_InediaInfectedAI_WeaponMultiplier;
				}
			}

			painDamage *= zmb.InediaInfectedAI_GetInjuryPainMultiplier();
			painDamage *= zmbLimbPainMultiplier;
			
			AddPainPercentForLimb(inediaPainLimb, painDamage, true, source, damageType);
			
			// The logic of inflicting a deep wound
			if (CanGetDeepWoundFromHit(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef)) {
				if (ActivateDeepWoundForLimb(inediaPainLimb, painDamage * DEEP_WOUND_PAIN_TO_BLEEDING_MULTIPLIER, component)) {
					if (isBleedingHit && m_Player.GetBleedingManagerServer()) {
						m_Player.GetBleedingManagerServer().InediaInfectedAI_RemoveMostSignificantBleedingSourceWithoutSepsis();
					}
				}
			}

			return;
		} else if (source.IsInherited(AnimalBase)) {
			// Pain from animals

			AnimalBase animal = AnimalBase.Cast(source);
			if (!animal || !animal.m_InediaInfectedAI_ConfigZombie || !animal.m_InediaInfectedAI_ConfigZombie.PainToPlayerHandlerIsActive) {
				return;
			}
				
			float animalLimpPainMultiplier = animal.InediaInfectedAI_GetPainToPlayerLimbMultiplier(m_Player, inediaPainLimb, damageType);
			if (animalLimpPainMultiplier <= 0) {
				return;
			}
			
			m_EEHitByLimbsBreakChancesMultiplier = animal.m_InediaInfectedAI_ConfigZombie.PainToPlayerLimbsBreakChancesMultiplier;

			if (inediaPainLimb == "head") {
				if (m_Player.InediaInfectedAI_IsInBlock()) {
					// Head cannot be injured by melee attacks if the player is in a block,
					// redirecting damage to arms
					inediaPainLimb = "arms";
					painDamage = GetInediaLimbAmmoShockDamage("arms", ammo, damageType) * m_Player.m_InediaInfectedAI_WeaponMultiplier;
				}
			}

			painDamage *= animalLimpPainMultiplier;
			AddPainPercentForLimb(inediaPainLimb, painDamage, true, source, damageType);
			
			// The logic of inflicting a deep wound
			if (CanGetDeepWoundFromHit(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef)) {
				if (ActivateDeepWoundForLimb(inediaPainLimb, painDamage * DEEP_WOUND_PAIN_TO_BLEEDING_MULTIPLIER, component)) {
					if (isBleedingHit && m_Player.GetBleedingManagerServer()) {
						m_Player.GetBleedingManagerServer().InediaInfectedAI_RemoveMostSignificantBleedingSourceWithoutSepsis();
					}
				}
			}

			return;
		} else if (damageType == DT_EXPLOSION) {
			// Pain from explosions

			if (m_Player.m_InediaInfectedAI_ConfigPlayers.ExplosionsPainMultiplier <= 0) {
				return;
			}

			// From the grenade object, you cannot obtain the object that threw it, and grenades also do not have a dmgZone,
			// so in the case of grenade damage, if permitted, we damage a random dmgZone.
			// The explosion can hit multiple limbs at once.
			foreach (string EDPainLimb: InediaInfectedAI_PainManager.m_InediaPainLimbs) {
				if (Math.RandomBool() || EDPainLimb == "head" || (EDPainLimb == "legs" && ammo == "LandMineExplosion")) {
					painDamage = GetInediaLimbAmmoShockDamage(EDPainLimb, ammo, damageType) * m_Player.m_InediaInfectedAI_ConfigPlayers.ExplosionsPainMultiplier * m_Player.m_InediaInfectedAI_WeaponMultiplier;
					AddPainPercentForLimb(EDPainLimb, painDamage, true, source, damageType);
					
					if (painDamage >= 10) {
						float deepWoundChanceExplosions = painDamage / 3 * GetInediaLimbDamageMultiplier(EDPainLimb, "Blood", "FragGrenade");
						if (deepWoundChanceExplosions > 0 && Math.RandomFloatInclusive(0, 100) <= deepWoundChanceExplosions) {
							ActivateDeepWoundForLimb(EDPainLimb, painDamage * DEEP_WOUND_PAIN_TO_BLEEDING_MULTIPLIER);
						}
					}
				}
			}

			return;
		} else if (ammo == "InediaInfectedAI_ThrowingProjectile") {
			// Pain from throwing projectile

			ItemBase projectile = ItemBase.Cast(source);
			if (!projectile || !projectile.InediaInfectedAI_IsThrowingProjectile()) {
				return;
			}
			
			m_EEHitByLimbsBreakChancesMultiplier *= projectile.m_InediaInfectedAI_ThrowingProjectile.m_TargetBreakChanceMultiplier;

			float shockArmorMultiplier = damageResult.GetDamage("", "Shock") / ItemBase.InediaInfectedAI_ThrowingProjectileGetAmmoShockDamage();
			painDamage = shockArmorMultiplier * projectile.InediaInfectedAI_ThrowingProjectileGetShockDamage() * projectile.InediaInfectedAI_ThrowingProjectileGetPainMultiplier();

			AddPainPercentForLimb(inediaPainLimb, painDamage, true, source, damageType);
			
			// The logic of inflicting a deep wound
			if (CanGetDeepWoundFromHit(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef)) {
				if (ActivateDeepWoundForLimb(inediaPainLimb, painDamage * DEEP_WOUND_PAIN_TO_BLEEDING_MULTIPLIER, component)) {
					if (isBleedingHit && m_Player.GetBleedingManagerServer()) {
						m_Player.GetBleedingManagerServer().InediaInfectedAI_RemoveMostSignificantBleedingSourceWithoutSepsis();
					}
				}
			}

			return;
		} else if (ammo == "TransportHit") {
			// Pain from collision with car
			
			if (m_Player.m_InediaInfectedAI_ConfigPlayers.VehicleCollisionsPainMultiplier <= 0) {
				return;
			}
			
			painDamage = damageResult.GetDamage("", "Health") * 1.7 * m_Player.m_InediaInfectedAI_ConfigPlayers.VehicleCollisionsPainMultiplier * m_Player.m_InediaInfectedAI_WeaponMultiplier;

			// The car can hit multiple limbs at once
			// The limbs are damaged in random patterns
			foreach (string carHitLimb: InediaInfectedAI_PainManager.m_InediaPainLimbs) {
				if (Math.RandomBool()) {
					AddPainPercentForLimb(carHitLimb, painDamage, true, source, damageType);

					if (painDamage >= 3) {
						float deepWoundChanceVehicleCollisions = painDamage / 3 * GetInediaLimbDamageMultiplier(carHitLimb, "Blood", "Melee");
						if (deepWoundChanceVehicleCollisions > 0 && Math.RandomFloatInclusive(0, 100) <= deepWoundChanceVehicleCollisions) {
							ActivateDeepWoundForLimb(carHitLimb, painDamage * DEEP_WOUND_PAIN_TO_BLEEDING_MULTIPLIER);
						}
					}
				}
			}
			
			return;
		} else if (ammo == "FallDamageShock") {
			// Pain from falls
			
			if (m_Player.m_InediaInfectedAI_ConfigPlayers.FallPainMultiplier <= 0) {
				return;
			}
			
			painDamage *= m_Player.m_InediaInfectedAI_ConfigPlayers.FallPainMultiplier;
			
			// If a character receives more than 30% shock damage from a fall, they have a chance to break a leg equal to the shock damage received.
			if (m_Player.m_InediaInfectedAI_ConfigPlayers.LimbsBreakFromFalls && painDamage > 30) {
				float fallLegsFractureChance = painDamage;
				fallLegsFractureChance *= InediaInfectedAI_SyberiaCompatibility.GetSyberiaFractureChanceMultiplier(m_Player);

				if (fallLegsFractureChance > 0 && Math.RandomFloatInclusive(0, 100) <= fallLegsFractureChance) {
					ActivateBreakForLimb("legs", 0);
				}
			}
			
			if (m_Player.GetHealth("", "Shock") > PlayerConstants.UNCONSCIOUS_THRESHOLD) {
				// After the fall, the player remains conscious.
				
				AddPainPercentForLimb("legs", painDamage);
				
				if (m_Player.m_InediaInfectedAI_ConfigPlayers.LimbsDeepWoundFromFalls && painDamage > 30) {
					float deepWoundChanceFallLegs = painDamage / 3 * GetInediaLimbDamageMultiplier("legs", "Blood", "Melee");
					if (deepWoundChanceFallLegs > 0 && Math.RandomFloatInclusive(0, 100) <= deepWoundChanceFallLegs) {
						ActivateDeepWoundForLimb("legs", painDamage * DEEP_WOUND_PAIN_TO_BLEEDING_MULTIPLIER);
					}
				}
				
				// If a player is conscious after a fall and receives more than 30% shock damage from the fall,
				// their arms receive random shock damage ranging from zero to the shock damage received by the legs.
				if (painDamage > 30) {
					AddPainPercentForLimb("arms", Math.RandomFloatInclusive(0, painDamage), m_Player.m_InediaInfectedAI_ConfigPlayers.LimbsBreakFromFalls);
					
					if (m_Player.m_InediaInfectedAI_ConfigPlayers.LimbsDeepWoundFromFalls) {
						float deepWoundChanceFallArms = painDamage / 3 * GetInediaLimbDamageMultiplier("arms", "Blood", "Melee");
						if (deepWoundChanceFallArms > 0 && Math.RandomFloatInclusive(0, 100) <= deepWoundChanceFallArms) {
							ActivateDeepWoundForLimb("arms", painDamage * DEEP_WOUND_PAIN_TO_BLEEDING_MULTIPLIER);
						}
					}
				}
			} else {
				// After the fall, the player lost consciousness.
				
				// If a player loses consciousness upon falling, besides their legs,
				// all other limbs receive random shock damage ranging from zero to the shock damage received by the legs.
				foreach (string inediaPainLimbFall: InediaInfectedAI_PainManager.m_InediaPainLimbs) {
					AddPainPercentForLimb(inediaPainLimbFall, Math.RandomFloatInclusive(0, painDamage), m_Player.m_InediaInfectedAI_ConfigPlayers.LimbsBreakFromFalls);
					
					if (m_Player.m_InediaInfectedAI_ConfigPlayers.LimbsDeepWoundFromFalls && painDamage > 30) {
						float deepWoundChanceFallUnc = painDamage / 3 * GetInediaLimbDamageMultiplier(inediaPainLimbFall, "Blood", "Melee");
						if (deepWoundChanceFallUnc > 0 && Math.RandomFloatInclusive(0, 100) <= deepWoundChanceFallUnc) {
							ActivateDeepWoundForLimb(inediaPainLimbFall, painDamage * DEEP_WOUND_PAIN_TO_BLEEDING_MULTIPLIER);
						}
					}
				}
			}

			return;
		} else if (ammo == "BearTrapHit") {
			// Pain from bear trap
			
			AddPainPercentForLimb("legs", 100);
			
			if (m_Player.m_InediaInfectedAI_ConfigPlayers.LegsBreakFromTraps) {
				ActivateBreakForLimb("legs");
			}
			
			if (m_Player.m_InediaInfectedAI_ConfigPlayers.LegsDeepWoundFromTraps) {
				ActivateDeepWoundForLimb("legs");
			}
			
			return;
		} else if (ammo == "FireDamage") {
			// Pain from fireplace
			
			painDamage = 15 * m_Player.m_InediaInfectedAI_ConfigPlayers.FireDamagePainMultiplier * m_Player.m_InediaInfectedAI_WeaponMultiplier;
			
			if (painDamage > 0) {
				if (inediaPainLimb) {
					AddPainPercentForLimb(inediaPainLimb, painDamage);
				} else {
					AddPainPercentForLimb("legs", painDamage);
				}
			}
			
			return;
		} else {
			// Pain from other players
			
			if (!inediaPainLimb) {
				return;
			}

			PlayerBase player = PlayerBase.Cast(source.GetHierarchyRootPlayer());
			if (!player || !player.m_InediaInfectedAI_ConfigZombie || !player.m_InediaInfectedAI_ConfigZombie.PainToPlayerHandlerIsActive) {
				return;
			}
				
			float playerLimpPainMultiplier = player.InediaInfectedAI_GetPainToPlayerLimbMultiplier(m_Player, inediaPainLimb, damageType);
			if (playerLimpPainMultiplier <= 0) {
				return;
			}
			
			m_EEHitByLimbsBreakChancesMultiplier = player.m_InediaInfectedAI_ConfigZombie.PainToPlayerLimbsBreakChancesMultiplier;

			if (damageType == DT_CLOSE_COMBAT) {
				painDamage *= player.InediaInfectedAI_GetPainManager().GetPainArmsDamageMultiplier();

				if (inediaPainLimb == "head" && m_Player.InediaInfectedAI_IsInBlock()) {
					// Head cannot be injured by melee attacks if the player is in a block,
					// redirecting damage to arms
					inediaPainLimb = "arms";
					painDamage = GetInediaLimbAmmoShockDamage("arms", ammo, damageType) * m_Player.m_InediaInfectedAI_WeaponMultiplier;
				}
			}
			
			painDamage *= playerLimpPainMultiplier;
			AddPainPercentForLimb(inediaPainLimb, painDamage, true, player, damageType);
			
			// The logic of inflicting a bloody injuries
			bool runBloodyInjuriesLogic = false;
			
			if (!LimbsDeepWoundSystemIsActive()) {
				if (isBleedingHit) {
					runBloodyInjuriesLogic = true;
				}	
			} else if (CanGetDeepWoundFromHit(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef)) {
				if (ActivateDeepWoundForLimb(inediaPainLimb, painDamage * DEEP_WOUND_PAIN_TO_BLEEDING_MULTIPLIER, component)) {
					if (isBleedingHit && m_Player.GetBleedingManagerServer()) {
						m_Player.GetBleedingManagerServer().InediaInfectedAI_RemoveMostSignificantBleedingSourceWithoutSepsis();
					}
					
					runBloodyInjuriesLogic = true;
				}
			}
			
			if (runBloodyInjuriesLogic) {
				if (CanGetBulletFromHit(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef)) {
					SetBulletCountInLimb(inediaPainLimb, GetBulletCountInLimb(inediaPainLimb) + 1, 0.1);
					if (GetBulletCountInLimb(inediaPainLimb) == 1) {
						// If this is the first bullet in the body, start the countdown to the second stage.
						SetBulletInBodySecondsForLimb(inediaPainLimb, 0);
					}
				}
				
				if (CanGetInternalBleedingFromHit(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef)) {
					ActivateInternalBleeding();
				}
			}

			return;
		}
	}
	
	string GetInediaPainLimbFromDmgZone(string dmgZone)
	{
		if (dmgZone == "Head" || dmgZone == "Brain") {
			return "head";
		} else if (dmgZone == "LeftArm" || dmgZone == "RightArm" || dmgZone == "LeftHand" || dmgZone == "RightHand") {
			return "arms";
		} else if (dmgZone == "LeftLeg" || dmgZone == "RightLeg" || dmgZone == "LeftFoot" || dmgZone == "RightFoot") {
			return "legs";
		} else if (dmgZone == "Torso") {
			return "torso";
		}

		return "";
	}
	
	float GetInediaLimbAmmoShockDamage(string inediaLimb, string ammo, int damageType = DT_CLOSE_COMBAT)
	{
		if (!inediaLimb || !ammo || !m_InediaPainLimbsEquipSlots) {
		    return 0;
		}

		string dmgSource = "Melee";
		if (damageType == DT_FIRE_ARM) {
		    dmgSource = "Projectile";
		} else if (damageType == DT_EXPLOSION) {
		    dmgSource = "FragGrenade";
		}

		float damage = GetGame().ConfigGetFloat("CfgAmmo " + ammo + " DamageApplied Shock damage");
		if (damage <= 0) {
			if (damageType == DT_EXPLOSION || ammo == "TransportHit") {
				damage = GetGame().ConfigGetFloat("CfgAmmo " + ammo + " DamageApplied Health damage") * 1.7;
			}
			
			if (damage <= 0) {
				return 0;
			}
		}

		damage *= GetInediaLimbDamageMultiplier(inediaLimb, "Shock", dmgSource);

		return damage;
	}
	
	float GetInediaLimbDamageMultiplier(string inediaLimb, string dmgType = "Health", string dmgSource = "Melee" /* Melee|Projectile|FragGrenade|etc. */)
	{
		float damageMultiplier = 1;
		
		array<string> inediaPainLimbEquipSlots = m_InediaPainLimbsEquipSlots.Get(inediaLimb);
		if (inediaPainLimbEquipSlots && inediaPainLimbEquipSlots.Count() > 0) {
		    foreach(string equipSlotName: inediaPainLimbEquipSlots) {
		        ItemBase itemInEquipSlot = ItemBase.Cast(m_Player.FindAttachmentBySlotName(equipSlotName));
		        if (!itemInEquipSlot || itemInEquipSlot.IsDamageDestroyed()) {
		            continue;
		        }

		        string configPath = "CfgVehicles " + itemInEquipSlot.GetType() + " DamageSystem GlobalArmor " + dmgSource + " " + dmgType + " damage";
		        if (GetGame().ConfigGetType(configPath) >= 0) {
		            damageMultiplier *= GetGame().ConfigGetFloat(configPath);
		        }
		    }
		}
		
		return damageMultiplier;
	}
	
	void AddPainPercentForLimb(string limb, float painDamage, bool painActions = false, EntityAI source = null, int damageType = DT_CLOSE_COMBAT)
	{
		if (!PainSystemIsActive() || !m_Player.m_InediaInfectedAI_ConfigPlayers || !m_Player.GetAllowDamage() || !limb || painDamage == 0) {
			return;
		}
		
		float limbPainPercentNewValue = Math.Clamp(GetPainPercentForLimb(limb) + painDamage, 0, 100);
		
		float minimumPainPercent = GetMinimumPainPercentForLimb(limb);
		if (limbPainPercentNewValue < minimumPainPercent) {
			limbPainPercentNewValue = minimumPainPercent;
		}

		switch (limb) {
			case "head":
				m_InediaInfectedAI_PainHeadPercent = limbPainPercentNewValue;
			break;

			case "arms":
				m_InediaInfectedAI_PainArmsPercent = limbPainPercentNewValue;
			break;

			case "legs":
				m_InediaInfectedAI_PainLegsPercent = limbPainPercentNewValue;
			break;

			case "torso":
				m_InediaInfectedAI_PainTorsoPercent = limbPainPercentNewValue;
			break;
		
		    default:
	        	return;
	        break;
		}
		
		// Execute limb pain actions if necessary.
		if (painActions) {
			RunPainActionsForLimb(limb, painDamage, source, damageType);
		}
	}

	float GetPainPercentForLimb(string limb)
	{
		if (!PainSystemIsActive() || !limb) {
			return 0;
		}

		switch (limb) {
			case "head":
				return m_InediaInfectedAI_PainHeadPercent;
			break;

			case "arms":
				return m_InediaInfectedAI_PainArmsPercent;
			break;

			case "legs":
				return m_InediaInfectedAI_PainLegsPercent;
			break;

			case "torso":
				return m_InediaInfectedAI_PainTorsoPercent;
			break;
		}

		return 0;
	}
	
	float GetMinimumPainPercentForLimb(string limb)
	{
		if (!PainSystemIsActive() || !limb) {
			return 0;
		}
		
		float minPercent = 0;
		
		// Pain from the injured limbs does not drop below 50% (splint) or 80% (no splint) if a fracture is active.
		if (IsLimbBroken(limb)) {
			if (IsLimbStabilized(limb)) {
				if (minPercent < 50) {
					minPercent = 50;
				}
			} else {
				if (minPercent < 80) {
					if (m_AllowBreakPainDropBelowBadlySeconds[limb] > 0) {
						minPercent = 50;
					} else {
						minPercent = 80;
					}
				}
			}
		}
		
		// Pain from the injured limbs does not drop below 50% (bandage) or 80% (no bandage) if a deep wound is active.
		if (IsLimbDeepWounded(limb)) {
			if (IsLimbDeepWoundBandaged(limb)) {
				if (minPercent < 50) {
					minPercent = 50;
				}
			} else {
				if (minPercent < 80) {
					if (m_AllowDeepWoundPainDropBelowBadlySeconds[limb] > 0) {
						minPercent = 50;
					} else {
						minPercent = 80;
					}
				}
			}
		}
		
		if (GetBulletCountInLimb(limb) > 0) {
			if (LimbsDeepWoundSystemIsActive()) {
				InediaInfectedAI_BulletInBodyStages bulletInBodyStage = GetBulletInBodyStageForLimb(limb);
				
				if (bulletInBodyStage == InediaInfectedAI_BulletInBodyStages.SECOND) {
					if (minPercent < 80) {
						minPercent = 80;
					}
				} else {
					if (minPercent < 50) {
						minPercent = 50;
					}
				}
			} else {
				// If the deep wound system is disabled, but the bullet system is enabled, the bullet should lock the pain level at a high threshold.
				if (minPercent < 80) {
					minPercent = 80;
				}
			}
		}
		
		if (limb == "torso" && IsInternalBleedingActive()) {
			if (minPercent < 30) {
				minPercent = 30;
			}
		}
		
		return minPercent;
	}

	void SetPainLevelForLimb(string limb, InediaInfectedAI_LimbPainLevels level)
	{
		if (!PainSystemIsActive()) {
			return;
		}

		switch (limb) {
			case "head":
				m_PainHeadLevel = level;
			break;

			case "arms":
				m_PainArmsLevel = level;
			break;

			case "legs":
				m_PainLegsLevel = level;
			break;

			case "torso":
				m_PainTorsoLevel = level;
			break;
		}
	}

	InediaInfectedAI_LimbPainLevels GetPainLevelForLimb(string limb)
	{
		if (!PainSystemIsActive()) {
			return InediaInfectedAI_LimbPainLevels.ZERO;
		}

		switch (limb) {
			case "head":
				return m_PainHeadLevel;
			break;

			case "arms":
				return m_PainArmsLevel;
			break;

			case "legs":
				return m_PainLegsLevel;
			break;

			case "torso":
				return m_PainTorsoLevel;
			break;
		}

		return InediaInfectedAI_LimbPainLevels.ZERO;
	}

	void SetPainWithBuffsPercentForLimb(string limb, float value)
	{
		if (!PainSystemIsActive()) {
			return;
		}

		value = Math.Clamp(value, 0, 100);

		switch (limb) {
			case "head":
				m_PainHeadWithBuffsPercent = value;
			break;

			case "arms":
				m_PainArmsWithBuffsPercent = value;
			break;

			case "legs":
				m_PainLegsWithBuffsPercent = value;
			break;

			case "torso":
				m_PainTorsoWithBuffsPercent = value;
			break;
		}
	}

	float GetPainWithBuffsPercentForLimb(string limb)
	{
		if (!PainSystemIsActive()) {
			return 0;
		}

		switch (limb) {
			case "head":
				return m_PainHeadWithBuffsPercent;
			break;

			case "arms":
				return m_PainArmsWithBuffsPercent;
			break;

			case "legs":
				return m_PainLegsWithBuffsPercent;
			break;

			case "torso":
				return m_PainTorsoWithBuffsPercent;
			break;
		}

		return 0;
	}

	void SetPainWithBuffsLevelForLimb(string limb, InediaInfectedAI_LimbPainLevels level)
	{
		if (!PainSystemIsActive()) {
			return;
		}

		switch (limb) {
			case "head":
				m_PainHeadWithBuffsLevel = level;
			break;

			case "arms":
				m_PainArmsWithBuffsLevel = level;
			break;

			case "legs":
				m_PainLegsWithBuffsLevel = level;
			break;

			case "torso":
				m_PainTorsoWithBuffsLevel = level;
			break;
		}
	}

	InediaInfectedAI_LimbPainLevels GetPainWithBuffsLevelForLimb(string limb)
	{
		if (!PainSystemIsActive()) {
			return InediaInfectedAI_LimbPainLevels.ZERO;
		}

		switch (limb) {
			case "head":
				return m_PainHeadWithBuffsLevel;
			break;

			case "arms":
				return m_PainArmsWithBuffsLevel;
			break;

			case "legs":
				return m_PainLegsWithBuffsLevel;
			break;

			case "torso":
				return m_PainTorsoWithBuffsLevel;
			break;
		}

		return InediaInfectedAI_LimbPainLevels.ZERO;
	}
	
	void UpdatePainLevelsForAllLimbs()
	{
		if (!PainSystemIsActive() || !m_Player.m_InediaInfectedAI_ConfigPlayers) {
			return;
		}
		
		bool needSync = false;
		
		foreach (string limb: InediaInfectedAI_PainManager.m_InediaPainLimbs) {
			float limbPainPercent = GetPainPercentForLimb(limb);
			InediaInfectedAI_LimbPainLevels limbPainLevelBefore = GetPainLevelForLimb(limb);
			InediaInfectedAI_LimbPainLevels limbPainLevelAfter;
	
	       	if (limbPainPercent <= InediaInfectedAI_LimbPainLevelsBorders.ZERO) {
	            limbPainLevelAfter = InediaInfectedAI_LimbPainLevels.ZERO;
	        } else if (limbPainPercent <= InediaInfectedAI_LimbPainLevelsBorders.LOW) {
	            limbPainLevelAfter = InediaInfectedAI_LimbPainLevels.LOW;
	        } else if (limbPainPercent <= InediaInfectedAI_LimbPainLevelsBorders.MEDIUM) {
	            limbPainLevelAfter = InediaInfectedAI_LimbPainLevels.MEDIUM;
	        } else if (limbPainPercent <= InediaInfectedAI_LimbPainLevelsBorders.BADLY) {
	            limbPainLevelAfter = InediaInfectedAI_LimbPainLevels.BADLY;
	        } else {
				limbPainLevelAfter = InediaInfectedAI_LimbPainLevels.CRITICAL;
			}
	
			if (limbPainLevelBefore != limbPainLevelAfter) {
				SetPainLevelForLimb(limb, limbPainLevelAfter);
				needSync = true;
			}
	
			// Update limb pain with buffs level for limb
			float limbPainWithBuffsPercent = limbPainPercent;
			InediaInfectedAI_LimbPainLevels limbPainWithBuffsLevelBefore = GetPainWithBuffsLevelForLimb(limb);
			InediaInfectedAI_LimbPainLevels limbPainWithBuffsLevelAfter;
	
			if (PainkillerStrongEffectIsActive()) {
				limbPainWithBuffsPercent = limbPainPercent - 100;
			} else if (PainkillerMediumEffectIsActive()) {
				limbPainWithBuffsPercent = limbPainPercent - 50;
			} else if (PainkillerLightEffectIsActive()) {
				limbPainWithBuffsPercent = limbPainPercent - 30;
			}
	
	        if (limbPainWithBuffsPercent <= InediaInfectedAI_LimbPainLevelsBorders.ZERO) {
	            limbPainWithBuffsLevelAfter = InediaInfectedAI_LimbPainLevels.ZERO;
	        } else if (limbPainWithBuffsPercent <= InediaInfectedAI_LimbPainLevelsBorders.LOW) {
	            limbPainWithBuffsLevelAfter = InediaInfectedAI_LimbPainLevels.LOW;
	        } else if (limbPainWithBuffsPercent <= InediaInfectedAI_LimbPainLevelsBorders.MEDIUM) {
	            limbPainWithBuffsLevelAfter = InediaInfectedAI_LimbPainLevels.MEDIUM;
	        } else if (limbPainWithBuffsPercent <= InediaInfectedAI_LimbPainLevelsBorders.BADLY) {
	            limbPainWithBuffsLevelAfter = InediaInfectedAI_LimbPainLevels.BADLY;
	        } else {
				limbPainWithBuffsLevelAfter = InediaInfectedAI_LimbPainLevels.CRITICAL;
			}
	
			SetPainWithBuffsPercentForLimb(limb, limbPainWithBuffsPercent);
	
			if (limbPainWithBuffsLevelBefore != limbPainWithBuffsLevelAfter) {
				SetPainWithBuffsLevelForLimb(limb, limbPainWithBuffsLevelAfter);
				needSync = true;
			}
		}
		
		if (needSync) {
			SyncPainLevelsForAllLimbsServer();
		}
	}
	
	void SyncPainLevelsForAllLimbsServer()
	{
#ifdef SERVER
		if (m_Player.GetIdentity()) {
		    ScriptRPC rpc = new ScriptRPC();
			rpc.Write(m_PainHeadLevel);
			rpc.Write(m_PainArmsLevel);
			rpc.Write(m_PainLegsLevel);
			rpc.Write(m_PainTorsoLevel);
			rpc.Write(m_PainHeadWithBuffsLevel);
			rpc.Write(m_PainArmsWithBuffsLevel);
			rpc.Write(m_PainLegsWithBuffsLevel);
			rpc.Write(m_PainTorsoWithBuffsLevel);
			rpc.Send(m_Player, InediaInfectedAI_PlayerBase_ERPCs.RPC_SYNC_PAIN_LEVELS, true, m_Player.GetIdentity());
		}
#endif
	}
	
	void RunPainActionsForLimb(string limb, float painDamage, EntityAI source = null, int damageType = DT_CLOSE_COMBAT)
	{
		if (!PainSystemIsActive() || !m_Player.m_InediaInfectedAI_ConfigPlayers || painDamage <= 0) {
			return;
		}
		
		UpdatePainLevelsForAllLimbs();
		InediaInfectedAI_LimbPainLevels limbPainLevel = GetPainLevelForLimb(limb);
		InediaInfectedAI_LimbPainLevels limbPainWithBuffsLevel = GetPainWithBuffsLevelForLimb(limb);

		if (limb == "head") {
			// If the head injury is more critical, the player loses consciousness
			if (limbPainLevel >= InediaInfectedAI_LimbPainLevels.CRITICAL && m_Player.m_InediaInfectedAI_ConfigPlayers.UnconsciousWhenOverPainedHeadReceivingHits) {
				m_Player.InediaInfectedAI_MakeUnconscious();
			}
			
			// Actions on impact to the head (tinitus, blurring, temporary slowdown)
			if (limbPainLevel >= InediaInfectedAI_LimbPainLevels.LOW) {	
				float powerMultiplier = Math.Clamp(painDamage / 20, 0, 1);
				HeadHitAction(powerMultiplier, 60);
			}
			
			// Break limb if necessary
			if (CanLimbBeBrokenAfterHit(limb, painDamage)) {
				ActivateBreakForLimb("head", 60);
			}
		} else if (limb == "arms") {
			if (painDamage >= 2) {
				// Player exits ADS (Aim Down Sights) mode if arms damage becomes critical after taking damage.
				if (limbPainLevel >= InediaInfectedAI_LimbPainLevels.CRITICAL && m_Player.m_InediaInfectedAI_ConfigPlayers.ArmsLowerWhenOverPainedArmsReceivingHits) {
					m_Player.InediaInfectedAI_ResetADS();
				}
				
				// Logic for knocking an item out of the player's arms when taking damage to the arms
				if (limbPainWithBuffsLevel >= InediaInfectedAI_LimbPainLevels.LOW) {
					float attackerInjuryMultiplier = 1;
					float disarmMultiplier = 0;
					float inBlockDisarmMultiplier = 0;
					
					if (source) {
						if (source.IsInherited(ZombieBase)) {
							ZombieBase zmb = ZombieBase.Cast(source);
							if (zmb && zmb.m_InediaInfectedAI_ConfigZombie) {
								attackerInjuryMultiplier = zmb.InediaInfectedAI_GetInjuryPainMultiplier();
								disarmMultiplier = zmb.m_InediaInfectedAI_ConfigZombie.PainToPlayerArmsDisarmMultiplier;
								inBlockDisarmMultiplier = zmb.m_InediaInfectedAI_ConfigZombie.PainToPlayerArmsInBlockDisarmMultiplier;
							}
						} else if (source.IsInherited(AnimalBase)) {
							AnimalBase animal = AnimalBase.Cast(source);
							if (animal && animal.m_InediaInfectedAI_ConfigZombie) {
								disarmMultiplier = animal.m_InediaInfectedAI_ConfigZombie.PainToPlayerArmsDisarmMultiplier;
								inBlockDisarmMultiplier = animal.m_InediaInfectedAI_ConfigZombie.PainToPlayerArmsInBlockDisarmMultiplier;
							}
						} else if (source.IsInherited(ItemBase)) {
							ItemBase projectile = ItemBase.Cast(source);
							if (projectile && projectile.InediaInfectedAI_IsThrowingProjectile()) {
								disarmMultiplier = projectile.InediaInfectedAI_GetThrowingProjectile().m_TargetArmsPainDisarmMultiplier;
								inBlockDisarmMultiplier = projectile.InediaInfectedAI_GetThrowingProjectile().m_TargetArmsPainDisarmMultiplier;
							}
						} else if (damageType == DT_CLOSE_COMBAT) {
							PlayerBase player = PlayerBase.Cast(source.GetHierarchyRootPlayer());
							if (player && player.m_InediaInfectedAI_ConfigZombie) {
								attackerInjuryMultiplier = player.InediaInfectedAI_GetPainManager().GetPainArmsDamageMultiplier();
								disarmMultiplier = player.m_InediaInfectedAI_ConfigZombie.PainToPlayerArmsDisarmMultiplier;
								inBlockDisarmMultiplier = player.m_InediaInfectedAI_ConfigZombie.PainToPlayerArmsInBlockDisarmMultiplier;
							}
						}
					}
					
					float actionsShockMultiplier = GetActionsShockMultiplierForLimb("arms");
					disarmMultiplier *= actionsShockMultiplier;
					inBlockDisarmMultiplier *= actionsShockMultiplier;
					
					if (!m_Player.InediaInfectedAI_IsInBlock() || damageType != DT_CLOSE_COMBAT) {
						// No block
						float itemKnockoutChance = GetPainWithBuffsPercentForLimb("arms") * disarmMultiplier * attackerInjuryMultiplier;
						if (itemKnockoutChance > 0 && Math.RandomFloatInclusive(0, 100) <= itemKnockoutChance) {
							m_Player.InediaInfectedAI_KnockItemOutOfHands();
						}
					} else {
						// Block
						float itemKnockoutInBlockChance = GetPainWithBuffsPercentForLimb("arms") * inBlockDisarmMultiplier * attackerInjuryMultiplier;
						if (itemKnockoutInBlockChance > 0 && Math.RandomFloatInclusive(0, 100) <= itemKnockoutInBlockChance) {
							m_Player.InediaInfectedAI_KnockItemOutOfHands();
						}
					}
				}
			}
			
			// Break limb if necessary
			if (CanLimbBeBrokenAfterHit(limb, painDamage)) {
				ActivateBreakForLimb("arms", 60);
			}
		} else if (limb == "legs") {
			if (CanLimbBeBrokenAfterHit(limb, painDamage)) {
				ActivateBreakForLimb("legs", 60);
			}
		} else if (limb == "torso") {
			// Player crouching if torso injury is critical after taking damage.
			if (painDamage >= 2 && limbPainLevel >= InediaInfectedAI_LimbPainLevels.CRITICAL && m_Player.m_InediaInfectedAI_ConfigPlayers.CrouchWhenOverPainedTorsoReceivingHits) {
				if (m_Player.m_MovementState && (m_Player.m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT || m_Player.m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT)) {
					m_Player.InediaInfectedAI_ForceStance(DayZPlayerConstants.STANCEIDX_CROUCH);
				}
			}
			
			// Break limb if necessary
			if (CanLimbBeBrokenAfterHit(limb, painDamage)) {
				ActivateBreakForLimb("torso", 60);
			}
		}
		
		// If the character receives damage to a limb with a deep wound, the wound duration will increase by 5 minutes,
		// and if it was bandaged, the bandage will lose its effectiveness.
		if (painDamage >= 3 && Math.RandomFloatInclusive(0, 100) <= 20 && IsLimbDeepWounded(limb)) {
			float deepWoundIncreasedTime = Math.Clamp(GetDeepWoundTimeRemainingSecondsForLimb(limb) + 300, 0, GetDeepWoundMaxDurationSecondsForLimb(limb));
			SetDeepWoundTimeRemainingSecondsForLimb(limb, deepWoundIncreasedTime);
			
			if (IsLimbDeepWoundBandaged(limb) && IsLimbDeepWoundBandageEffective(limb)) {
				SetDeepWoundBandageEffectivenessTimeRemainingSecondsForLimb(limb, 0);
			}
		}
	}
	
	void SetBreakTimeRemainingSecondsForLimb(string limb, float seconds)
	{
		if (limb == "legs") {
			// At the moment, the vanilla leg fracture is being used.
			return;
		}
		
		if (!LimbsBreakSystemIsActive()) {
			return;
		}

		switch (limb) {
			case "head":
				m_BreakHeadTimeRemainingSeconds = seconds;
			break;

			case "arms":
				m_BreakArmsTimeRemainingSeconds = seconds;
			break;

			case "torso":
				m_BreakTorsoTimeRemainingSeconds = seconds;
			break;
		}
	}
	
	float GetBreakTimeRemainingSecondsForLimb(string limb)
	{
		if (limb == "legs") {
			// At the moment, the vanilla leg fracture is being used.
			return 0;
		}
		
		if (!LimbsBreakSystemIsActive()) {
			return 0;
		}

		switch (limb) {
			case "head":
				return m_BreakHeadTimeRemainingSeconds;
			break;

			case "arms":
				return m_BreakArmsTimeRemainingSeconds;
			break;

			case "torso":
				return m_BreakTorsoTimeRemainingSeconds;
			break;
		}

		return 0;
	}
	
	float m_BreakHeadLastActivateTime = -1000000;
	float m_BreakLegsLastActivateTime = -1000000;
	float m_BreakArmsLastActivateTime = -1000000;
	float m_BreakTorsoLastActivateTime = -1000000;
	bool ActivateBreakForLimb(string limb, float notMoreFrequentlyThanSeconds = 0, bool enableSound = true, bool spawnUsedItemIfStabilizationActive = true)
	{
		if (!m_Player.m_InediaInfectedAI_ConfigPlayers || !m_Player.GetAllowDamage()) {
			return false;
		}
		
		if (limb == "legs") {
			// At the moment, the vanilla leg fracture is being used.

			if (notMoreFrequentlyThanSeconds > 0 && (GetGame().GetTickTime() - m_BreakLegsLastActivateTime) <= notMoreFrequentlyThanSeconds) {
				return false;
			}
			m_BreakLegsLastActivateTime = GetGame().GetTickTime();
			
			if (m_Player.GetModifiersManager()) {
				if (m_Player.GetModifiersManager().IsModifierActive(eModifiers.MDF_BROKEN_LEGS)) {
				    m_Player.GetModifiersManager().DeactivateModifier(eModifiers.MDF_BROKEN_LEGS);
				}
			    m_Player.GetModifiersManager().ActivateModifier(eModifiers.MDF_BROKEN_LEGS);
				
				//DeactivateBreakStabilizeForLimb("legs", spawnUsedItemIfStabilizationActive);
				
				return true;
			}
		} else {
			if (!LimbsBreakSystemIsActive()) {
				return false;
			}
			
			float fractureTimeRemainingSeconds = 0;
			
			switch (limb) {
				case "head":
					if (m_Player.m_InediaInfectedAI_ConfigPlayers.HeadBreakDurationSeconds <= 0) {
						return false;
					}
				
					if (notMoreFrequentlyThanSeconds > 0 && (GetGame().GetTickTime() - m_BreakHeadLastActivateTime) <= notMoreFrequentlyThanSeconds) {
						return false;
					}
					m_BreakHeadLastActivateTime = GetGame().GetTickTime();
				
					fractureTimeRemainingSeconds = m_Player.m_InediaInfectedAI_ConfigPlayers.HeadBreakDurationSeconds;
					m_Player.InediaInfectedAI_SetNetSyncBoolVariable("HeadBreakIsActive", true);
					
				break;
	
				case "arms":
					if (m_Player.m_InediaInfectedAI_ConfigPlayers.ArmsBreakDurationSeconds <= 0) {
						return false;
					}
				
					if (notMoreFrequentlyThanSeconds > 0 && (GetGame().GetTickTime() - m_BreakArmsLastActivateTime) <= notMoreFrequentlyThanSeconds) {
						return false;
					}
					m_BreakArmsLastActivateTime = GetGame().GetTickTime();
				
					fractureTimeRemainingSeconds = m_Player.m_InediaInfectedAI_ConfigPlayers.ArmsBreakDurationSeconds;
					m_Player.InediaInfectedAI_SetNetSyncBoolVariable("ArmsBreakIsActive", true);
				break;
	
				case "torso":
					if (m_Player.m_InediaInfectedAI_ConfigPlayers.TorsoBreakDurationSeconds <= 0) {
						return false;
					}
				
					if (notMoreFrequentlyThanSeconds > 0 && (GetGame().GetTickTime() - m_BreakTorsoLastActivateTime) <= notMoreFrequentlyThanSeconds) {
						return false;
					}
					m_BreakTorsoLastActivateTime = GetGame().GetTickTime();
				
					fractureTimeRemainingSeconds = m_Player.m_InediaInfectedAI_ConfigPlayers.TorsoBreakDurationSeconds;
					m_Player.InediaInfectedAI_SetNetSyncBoolVariable("TorsoBreakIsActive", true);
				break;
			}
			
			if (fractureTimeRemainingSeconds > 0) {
				if (enableSound) {
					m_Player.InediaInfectedAI_PlaySoundServer(InediaInfectedAI_PlayerBaseSoundTypes.FRACTURE);
				}
				
				DeactivateBreakStabilizeForLimb(limb, spawnUsedItemIfStabilizationActive);
				SetBreakTimeRemainingSecondsForLimb(limb, fractureTimeRemainingSeconds);
				return true;
			}
		}
		
		return false;
	}
	
	void DeactivateBreakForLimb(string limb, bool spawnUsedItemIfStabilizationActive = true)
	{
		if (limb == "legs") {
			// At the moment, the vanilla leg fracture is being used.
			if (m_Player.GetModifiersManager()) {
				if (m_Player.GetModifiersManager().IsModifierActive(eModifiers.MDF_BROKEN_LEGS)) {
				    m_Player.GetModifiersManager().DeactivateModifier(eModifiers.MDF_BROKEN_LEGS);
				}
			}
			//DeactivateBreakStabilizeForLimb("legs", spawnUsedItemIfStabilizationActive);
		} else {
			if (!LimbsBreakSystemIsActive()) {
				return;
			}
			
			switch (limb) {
				case "head":
					m_Player.InediaInfectedAI_SetNetSyncBoolVariable("HeadBreakIsActive", false);
				break;
	
				case "arms":
					m_Player.InediaInfectedAI_SetNetSyncBoolVariable("ArmsBreakIsActive", false);
				break;
	
				case "torso":
					m_Player.InediaInfectedAI_SetNetSyncBoolVariable("TorsoBreakIsActive", false);
				break;
			}
			
			DeactivateBreakStabilizeForLimb(limb, spawnUsedItemIfStabilizationActive);
			SetBreakTimeRemainingSecondsForLimb(limb, 0);
		}
	}
	
	bool IsLimbBroken(string limb)
	{
		if (limb == "legs") {
			// At the moment, the vanilla leg fracture is being used.
			return m_Player.GetBrokenLegs() != eBrokenLegs.NO_BROKEN_LEGS;
		} else {
			if (!LimbsBreakSystemIsActive()) {
				return false;
			}
			
			switch (limb) {
				case "head":
					return m_Player.InediaInfectedAI_GetNetSyncBoolVariable("HeadBreakIsActive");
				break;
	
				case "arms":
					return m_Player.InediaInfectedAI_GetNetSyncBoolVariable("ArmsBreakIsActive");
				break;
	
				case "torso":
					return m_Player.InediaInfectedAI_GetNetSyncBoolVariable("TorsoBreakIsActive");
				break;
			}
		}
	
		return false;
	}
	
    void ActivateBreakStabilizeForLimb(string limb, float shockReductionMultiplier = 1, float regenerationMultiplier = 1, EntityAI item = null)
    {
        if (!LimbsBreakSystemIsActive()) {
            return;
        }
		
		if (IsLimbStabilized(limb)) {
			return;
		}
        
        switch (limb) {
            case "head":
                m_Player.InediaInfectedAI_SetNetSyncBoolVariable("HeadStabilizeIsActive", true);
				SetBreakStabilizationRegenerationMultiplierForLimb(limb, regenerationMultiplier);
				SetBreakStabilizationShockReductionMultiplierForLimb(limb, shockReductionMultiplier);
            break;

            case "arms":
                m_Player.InediaInfectedAI_SetNetSyncBoolVariable("ArmsStabilizeIsActive", true);
				SetBreakStabilizationRegenerationMultiplierForLimb(limb, regenerationMultiplier);
				SetBreakStabilizationShockReductionMultiplierForLimb(limb, shockReductionMultiplier);
            break;
            
            case "legs":
				m_Player.SetBrokenLegs(eBrokenLegs.BROKEN_LEGS_SPLINT);
				SetBreakStabilizationRegenerationMultiplierForLimb(limb, regenerationMultiplier);
				SetBreakStabilizationShockReductionMultiplierForLimb(limb, shockReductionMultiplier);
			
				// Apply vanilla splint model
				if (item && InediaInfectedAI_Functions.IsKindOfCached("Splint", item) && !m_Player.IsWearingSplint()) {
					m_Player.SetBrokenLegs(eBrokenLegs.BROKEN_LEGS_SPLINT);

					ItemBase newItem = ItemBase.Cast(m_Player.GetInventory().CreateInInventory("Splint_Applied"));
					if (newItem) {
						MiscGameplayFunctions.TransferItemProperties(item, newItem, true, false, true);
					}
				}
            break;

            case "torso":
                m_Player.InediaInfectedAI_SetNetSyncBoolVariable("TorsoStabilizeIsActive", true);
				SetBreakStabilizationRegenerationMultiplierForLimb(limb, regenerationMultiplier);
				SetBreakStabilizationShockReductionMultiplierForLimb(limb, shockReductionMultiplier);
            break;
        }
    }
    
    void DeactivateBreakStabilizeForLimb(string limb, bool spawnUsedItem = true)
    {
        if (!LimbsBreakSystemIsActive()) {
            return;
        }
		
		if (!IsLimbStabilized(limb)) {
			return;
		}
		
		bool splintDeactivated = false;
        
        switch (limb) {
            case "head":
                m_Player.InediaInfectedAI_SetNetSyncBoolVariable("HeadStabilizeIsActive", false);
				splintDeactivated = true;
            break;

            case "arms":
                m_Player.InediaInfectedAI_SetNetSyncBoolVariable("ArmsStabilizeIsActive", false);
				splintDeactivated = true;
            break;
            
            case "legs":
				m_Player.SetBrokenLegs(eBrokenLegs.BROKEN_LEGS);
			
				// Remove vanilla splint model if needed
				if (m_Player.IsWearingSplint()) {
					MiscGameplayFunctions.RemoveSplint(m_Player);
					spawnUsedItem = false;
				}

				splintDeactivated = true;
            break;

            case "torso":
                m_Player.InediaInfectedAI_SetNetSyncBoolVariable("TorsoStabilizeIsActive", false);
				splintDeactivated = true;
            break;
        }
		
		if (splintDeactivated && spawnUsedItem) {
			// When the splint is deactivated, bloody rags appear on the ground.
			ItemBase rag = ItemBase.Cast(m_Player.SpawnEntityOnGroundPos("Rag", m_Player.GetPosition() + 0.6 * m_Player.GetDirection()));
			if (rag) {
				rag.SetQuantity(1);
				rag.SetHealth("", "", rag.GetMaxHealth("", "") * GameConstants.DAMAGE_RUINED_VALUE);
			}
		}
    }
    
    bool IsLimbStabilized(string limb)
    {
        if (!LimbsBreakSystemIsActive()) {
            return false;
        }
        
        switch (limb) {
            case "head":
                return m_Player.InediaInfectedAI_GetNetSyncBoolVariable("HeadStabilizeIsActive");
            break;

            case "arms":
                return m_Player.InediaInfectedAI_GetNetSyncBoolVariable("ArmsStabilizeIsActive");
            break;
            
            case "legs":
				return m_Player.GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS_SPLINT;
            break;

            case "torso":
                return m_Player.InediaInfectedAI_GetNetSyncBoolVariable("TorsoStabilizeIsActive");
            break;
        }
    
        return false;
    }
	
	void SetBreakStabilizationShockReductionMultiplierForLimb(string limb, float shockReductionMultiplier = 1)
	{
		if (!LimbsBreakSystemIsActive()) {
			return;
		}
		
		if (shockReductionMultiplier < 1) {
			shockReductionMultiplier = 1;
		}
		
		switch (limb) {
			case "head":
				m_HeadBreakStabilizationItemShockReductionMultiplier = shockReductionMultiplier;
			break;

			case "arms":
				m_ArmsBreakStabilizationItemShockReductionMultiplier = shockReductionMultiplier;
			break;
			
			case "legs":
				m_LegsBreakStabilizationItemShockReductionMultiplier = shockReductionMultiplier;
			break;

			case "torso":
				m_TorsoBreakStabilizationItemShockReductionMultiplier = shockReductionMultiplier;
			break;
		}
		
#ifdef SERVER
		// This multiplier needs to be synchronized with the client, as it affects the client's hand shake during aiming.
		if (m_Player.GetIdentity()) {
		    ScriptRPC rpc = new ScriptRPC();
			rpc.Write(limb);
			rpc.Write(shockReductionMultiplier);
			rpc.Send(m_Player, InediaInfectedAI_PlayerBase_ERPCs.RPC_SYNC_STABILIZATION_SHOCK_REDUCTION_MULTIPLIER, true, m_Player.GetIdentity());
		}
#endif
	}
	
	float GetBreakStabilizationShockReductionMultiplierForLimb(string limb)
	{
		if (!LimbsBreakSystemIsActive()) {
			return 1;
		}
		
		switch (limb) {
			case "head":
				return m_HeadBreakStabilizationItemShockReductionMultiplier;
			break;

			case "arms":
				return m_ArmsBreakStabilizationItemShockReductionMultiplier;
			break;
			
			case "legs":
				return m_LegsBreakStabilizationItemShockReductionMultiplier;
			break;

			case "torso":
				return m_TorsoBreakStabilizationItemShockReductionMultiplier;
			break;
		}
	
		return 1;
	}
	
	void SetBreakStabilizationRegenerationMultiplierForLimb(string limb, float regenerationMultiplier = 1)
	{
		if (!LimbsBreakSystemIsActive()) {
			return;
		}
		
		if (regenerationMultiplier < 1) {
			regenerationMultiplier = 1;
		}
		
		switch (limb) {
			case "head":
				m_HeadBreakStabilizationItemRegenerationMultiplier = regenerationMultiplier;
			break;

			case "arms":
				m_ArmsBreakStabilizationItemRegenerationMultiplier = regenerationMultiplier;
			break;
			
			case "legs":
				m_LegsBreakStabilizationItemRegenerationMultiplier = regenerationMultiplier;
			break;

			case "torso":
				m_TorsoBreakStabilizationItemRegenerationMultiplier = regenerationMultiplier;
			break;
		}
	}
	
	float GetBreakStabilizationRegenerationMultiplierForLimb(string limb)
	{
		if (!LimbsBreakSystemIsActive()) {
			return 1;
		}
		
		switch (limb) {
			case "head":
				return m_HeadBreakStabilizationItemRegenerationMultiplier;
			break;

			case "arms":
				return m_ArmsBreakStabilizationItemRegenerationMultiplier;
			break;
			
			case "legs":
				return m_LegsBreakStabilizationItemRegenerationMultiplier;
			break;

			case "torso":
				return m_TorsoBreakStabilizationItemRegenerationMultiplier;
			break;
		}
	
		return 1;
	}
	
	void SyncBreakRegenerationLevelForLimbServer(string limb, float regenerationMultiplier)
	{
#ifdef SERVER
		if (!LimbsBreakSystemIsActive()) {
			return;
		}
		
		if (regenerationMultiplier < 1) {
			regenerationMultiplier = 1;
		}
		
		bool needSync = false;
		
		int newLevel;
		if (regenerationMultiplier > 4) {
			newLevel = InediaInfectedAI_LimbRegenerationLevels.HIGH;
		} else if (regenerationMultiplier > 2) {
			newLevel = InediaInfectedAI_LimbRegenerationLevels.MEDIUM;
		} else if (regenerationMultiplier > 1) {
			newLevel = InediaInfectedAI_LimbRegenerationLevels.LOW;
		} else {
			newLevel = InediaInfectedAI_LimbRegenerationLevels.BASE;
		}
		
		if (m_LimbsBreakRegenerationLevels.Contains(limb)) {
			if (m_LimbsBreakRegenerationLevels[limb] != newLevel) {
				m_LimbsBreakRegenerationLevels[limb] = newLevel;
				needSync = true;
			}
		}

		
		if (needSync) {
			if (m_Player.GetIdentity()) {
			    ScriptRPC rpc = new ScriptRPC();
			    rpc.Write(m_LimbsBreakRegenerationLevels);
				rpc.Send(m_Player, InediaInfectedAI_PlayerBase_ERPCs.RPC_SYNC_BREAK_REGENERATION_LEVELS, true, m_Player.GetIdentity());
			}
		}
#endif
	}
	InediaInfectedAI_LimbRegenerationLevels GetBreakRegenerationLevelForLimbClient(string limb)
	{
#ifndef SERVER
		if (m_LimbsBreakRegenerationLevels) {
			if (m_LimbsBreakRegenerationLevels && m_LimbsBreakRegenerationLevels.Contains(limb)) {
				return m_LimbsBreakRegenerationLevels[limb];
			}
		}
#endif
		
		return InediaInfectedAI_LimbRegenerationLevels.BASE;
	}
	
	bool CanLimbBeBrokenAfterHit(string limb, float receivedDamage)
	{
		if (!LimbsBreakSystemIsActive() || !m_Player.m_InediaInfectedAI_ConfigPlayers || m_Player.m_InediaInfectedAI_ConfigPlayers.LimbsBreakPainThresholdPercent > 100 || receivedDamage < 3) {
			return false;
		}
		
		if (limb == "head") {
			if (m_Player.m_InediaInfectedAI_ConfigPlayers.HeadBreakDurationSeconds <= 0) {
				return false;
			}
		} else if (limb == "arms") {
			if (m_Player.m_InediaInfectedAI_ConfigPlayers.ArmsBreakDurationSeconds <= 0) {
				return false;
			}
		} else if (limb == "torso") {
			if (m_Player.m_InediaInfectedAI_ConfigPlayers.TorsoBreakDurationSeconds <= 0) {
				return false;
			}
		} else if (limb == "legs") {
			if (m_Player.m_InediaInfectedAI_ConfigPlayers.LegsBreakDurationSeconds <= 0) {
				return false;
			}
		}
		
		float limbPainPercent = GetPainPercentForLimb(limb);
		if (limbPainPercent < m_Player.m_InediaInfectedAI_ConfigPlayers.LimbsBreakPainThresholdPercent) {
			return false;
		}
		
		float fractureChance = 0.03125 * Math.Pow(2, Math.Clamp(limbPainPercent / 10, 0, 10));
		
		if (receivedDamage < 5) {
			fractureChance *= 0.5;
		} else if (receivedDamage < 20) {
			fractureChance *= 1;
		} else {
			fractureChance *= 2;
		}
		
		fractureChance *= m_EEHitByLimbsBreakChancesMultiplier;
		fractureChance *= InediaInfectedAI_SyberiaCompatibility.GetSyberiaFractureChanceMultiplier(m_Player);

		if (fractureChance <= 0) {
			return false;
		}
		
		return Math.RandomFloatInclusive(0, 100) <= fractureChance;
	}
	
	void SetDeepWoundTimeRemainingSecondsForLimb(string limb, float seconds)
	{		
		if (!LimbsDeepWoundSystemIsActive()) {
			return;
		}

		switch (limb) {
			case "head":
				m_HeadDeepWoundTimeRemainingSeconds = seconds;
			break;

			case "arms":
				m_ArmsDeepWoundTimeRemainingSeconds = seconds;
			break;
			
			case "legs":
				m_LegsDeepWoundTimeRemainingSeconds = seconds;
			break;

			case "torso":
				m_TorsoDeepWoundTimeRemainingSeconds = seconds;
			break;
		}
	}
	
	float GetDeepWoundTimeRemainingSecondsForLimb(string limb)
	{		
		if (!LimbsDeepWoundSystemIsActive()) {
			return 0;
		}

		switch (limb) {
			case "head":
				return m_HeadDeepWoundTimeRemainingSeconds;
			break;

			case "arms":
				return m_ArmsDeepWoundTimeRemainingSeconds;
			break;
			
			case "legs":
				return m_LegsDeepWoundTimeRemainingSeconds;
			break;

			case "torso":
				return m_TorsoDeepWoundTimeRemainingSeconds;
			break;
		}

		return 0;
	}
	
	float GetDeepWoundMaxDurationSecondsForLimb(string limb)
	{		
		if (!m_Player.m_InediaInfectedAI_ConfigPlayers || !LimbsDeepWoundSystemIsActive()) {
			return 0;
		}

		switch (limb) {
			case "head":
				return m_Player.m_InediaInfectedAI_ConfigPlayers.HeadDeepWoundDurationSeconds;
			break;

			case "arms":
				return m_Player.m_InediaInfectedAI_ConfigPlayers.ArmsDeepWoundDurationSeconds;
			break;
			
			case "legs":
				return m_Player.m_InediaInfectedAI_ConfigPlayers.LegsDeepWoundDurationSeconds;
			break;

			case "torso":
				return m_Player.m_InediaInfectedAI_ConfigPlayers.TorsoDeepWoundDurationSeconds;
			break;
		}

		return 0;
	}
	
	float m_HeadDeepWoundLastActivateTime = -1000000;
	float m_ArmsDeepWoundLastActivateTime = -1000000;
	float m_LegsDeepWoundLastActivateTime = -1000000;
	float m_TorsoDeepWoundLastActivateTime = -1000000;
	float m_DeepWoundLastSoundPlay = -1000000;
	bool ActivateDeepWoundForLimb(string limb, float bleedingRateMultiplier = 1, int bleedingZoneIndex = -1, float notMoreFrequentlyThanSeconds = 0, bool enableSound = true, bool spawnUsedRagIfBandageActive = true)
	{
		if (!LimbsDeepWoundSystemIsActive() || m_Player.InediaInfectedAI_IsNPC() || !m_Player.GetAllowDamage()) {
			return false;
		}

		// Blood rate logic
		if (bleedingRateMultiplier > 0) {
			float bloodRate = bleedingRateMultiplier * DEEP_WOUND_BLEEDING_RATE_HIT;
			
			if (IsLimbDeepWounded(limb)) {
				bloodRate += GetDeepWoundBleedingRateForLimb(limb);
			}
			
			SetDeepWoundBleedingRateForLimb(limb, Math.Clamp(bloodRate, DEEP_WOUND_BLEEDING_RATE_MIN, DEEP_WOUND_BLEEDING_RATE_MAX));
		}
				
		float deepWoundMaxDurationSeconds = GetDeepWoundMaxDurationSecondsForLimb(limb);
		if (deepWoundMaxDurationSeconds <= 0) {
			return false;
		}
		
		switch (limb) {
			case "head":
				if (notMoreFrequentlyThanSeconds > 0 && (GetGame().GetTickTime() - m_HeadDeepWoundLastActivateTime) <= notMoreFrequentlyThanSeconds) {
					return false;
				}
				m_HeadDeepWoundLastActivateTime = GetGame().GetTickTime();
			
				m_Player.InediaInfectedAI_SetNetSyncBoolVariable("HeadDeepWoundIsActive", true);
				
			break;

			case "arms":
				if (notMoreFrequentlyThanSeconds > 0 && (GetGame().GetTickTime() - m_ArmsDeepWoundLastActivateTime) <= notMoreFrequentlyThanSeconds) {
					return false;
				}
				m_ArmsDeepWoundLastActivateTime = GetGame().GetTickTime();

				m_Player.InediaInfectedAI_SetNetSyncBoolVariable("ArmsDeepWoundIsActive", true);
			break;
			
			case "legs":
				if (notMoreFrequentlyThanSeconds > 0 && (GetGame().GetTickTime() - m_LegsDeepWoundLastActivateTime) <= notMoreFrequentlyThanSeconds) {
					return false;
				}
				m_LegsDeepWoundLastActivateTime = GetGame().GetTickTime();

				m_Player.InediaInfectedAI_SetNetSyncBoolVariable("LegsDeepWoundIsActive", true);
			break;

			case "torso":
				if (notMoreFrequentlyThanSeconds > 0 && (GetGame().GetTickTime() - m_TorsoDeepWoundLastActivateTime) <= notMoreFrequentlyThanSeconds) {
					return false;
				}
				m_TorsoDeepWoundLastActivateTime = GetGame().GetTickTime();

				m_Player.InediaInfectedAI_SetNetSyncBoolVariable("TorsoDeepWoundIsActive", true);
			break;
		}
		
		if (enableSound && (m_DeepWoundLastSoundPlay <= 0 || (GetGame().GetTickTime() - m_DeepWoundLastSoundPlay) > 5)) {
			m_Player.InediaInfectedAI_PlaySoundServer(InediaInfectedAI_PlayerBaseSoundTypes.DEEP_WOUND);
			m_DeepWoundLastSoundPlay = GetGame().GetTickTime();
		}
		
		// If pain was previously allowed to drop below the "badly" level with an open wound, this is now revoked.
		m_AllowDeepWoundPainDropBelowBadlySeconds[limb] = 0;
		
		// Visual bleeding source
		if (bleedingZoneIndex >= 0) {
			int bleedingBit = m_Player.GetBleedingManagerServer().InediaInfectedAI_GetBitFromSelectionID(bleedingZoneIndex);
			if (bleedingBit > 0) {
				m_Player.m_InediaInfectedAI_DeepWoundsBleedingsSyncBits = m_Player.m_InediaInfectedAI_DeepWoundsBleedingsSyncBits | bleedingBit;
			}
		} else if (bleedingZoneIndex == -1) {
			ActivateVisualBleedingForLimbServer(limb);
		}
		
		// Activate/Reactivate deep wound logic
		DeactivateDeepWoundBandageForLimb(limb, spawnUsedRagIfBandageActive);
		SetDeepWoundTimeRemainingSecondsForLimb(limb, deepWoundMaxDurationSeconds);
		
		return true;
	}
	
	void DeactivateDeepWoundForLimb(string limb, bool spawnUsedRagIfBandageActive = true)
	{
		if (!LimbsDeepWoundSystemIsActive()) {
			return;
		}
		
		switch (limb) {
			case "head":
				m_Player.InediaInfectedAI_SetNetSyncBoolVariable("HeadDeepWoundIsActive", false);
			break;

			case "arms":
				m_Player.InediaInfectedAI_SetNetSyncBoolVariable("ArmsDeepWoundIsActive", false);
			break;
			
			case "legs":
				m_Player.InediaInfectedAI_SetNetSyncBoolVariable("LegsDeepWoundIsActive", false);
			break;

			case "torso":
				m_Player.InediaInfectedAI_SetNetSyncBoolVariable("TorsoDeepWoundIsActive", false);
			break;
		}
		
		DeactivateVisualBleedingForLimbServer(limb);
		DeactivateDeepWoundBandageForLimb(limb, spawnUsedRagIfBandageActive);
		SetDeepWoundTimeRemainingSecondsForLimb(limb, 0);
	}
	
	void SetDeepWoundBleedingRateForLimb(string limb, float bleedingRate)
	{
		if (!LimbsDeepWoundSystemIsActive()) {
			return;
		}
		
		switch (limb) {
			case "head":
				m_HeadDeepWoundBleedingRate = bleedingRate;
			break;

			case "arms":
				m_ArmsDeepWoundBleedingRate = bleedingRate;
			break;
			
			case "legs":
				m_LegsDeepWoundBleedingRate = bleedingRate;
			break;

			case "torso":
				m_TorsoDeepWoundBleedingRate = bleedingRate;
			break;
		}
	}
	
	float GetDeepWoundBleedingRateForLimb(string limb)
	{
		if (!LimbsDeepWoundSystemIsActive()) {
			return 0;
		}
		
		switch (limb) {
			case "head":
				return m_HeadDeepWoundBleedingRate;
			break;

			case "arms":
				return m_ArmsDeepWoundBleedingRate;
			break;
			
			case "legs":
				return m_LegsDeepWoundBleedingRate;
			break;

			case "torso":
				return m_TorsoDeepWoundBleedingRate;
			break;
		}
		
		return 0;
	}
	
	float GetDeepWoundsBleedingRateWithMultipliers()
	{
		if (!LimbsDeepWoundSystemIsActive()) {
			return 0;
		}

		float bloodLossRatePerSec = 0;
		
		foreach (string inediaPainLimb: InediaInfectedAI_PainManager.m_InediaPainLimbs) {			
			if (IsLimbDeepWounded(inediaPainLimb)) {
				if (!IsLimbDeepWoundBandaged(inediaPainLimb)) {
					bloodLossRatePerSec += GetDeepWoundBleedingRateForLimb(inediaPainLimb);
				} else if (!IsLimbDeepWoundBandageEffective(inediaPainLimb)) {
					bloodLossRatePerSec += 0.5 * GetDeepWoundBleedingRateForLimb(inediaPainLimb);
				}
			}
		}
		
		if (bloodLossRatePerSec > 0) {
			float bloodScale = Math.InverseLerp(PlayerConstants.BLOOD_THRESHOLD_FATAL, PlayerConstants.BLEEDING_LOW_PRESSURE_BLOOD, m_Player.GetHealth("GlobalHealth", "Blood"));
			bloodScale = Math.Clamp(bloodScale, PlayerConstants.BLEEDING_LOW_PRESSURE_MIN_MOD, 1);
			bloodLossRatePerSec *= bloodScale;
			
			if (m_Player.GetModifiersManager() && m_Player.GetModifiersManager().IsModifierActive(InediaInfectedAI_eModifiers.MDF_BLOOD_CLOTTING)) {
				if (m_BloodClottingItemClottingMultiplier > 1) {
					bloodLossRatePerSec /= m_BloodClottingItemClottingMultiplier;
				}
			}
				
			if (!m_Player.IsUnconscious()) {
				bloodLossRatePerSec *= m_Player.m_InediaInfectedAI_ConfigPlayers.LimbsDeepWoundConsciousBleedRateMultiplier;
			} else {
				bloodLossRatePerSec *= m_Player.m_InediaInfectedAI_ConfigPlayers.LimbsDeepWoundUnconsciousBleedRateMultiplier;
			}
		}
		
		return bloodLossRatePerSec;
	}
	
	bool IsLimbDeepWounded(string limb)
	{
		if (!LimbsDeepWoundSystemIsActive()) {
			return false;
		}
		
		switch (limb) {			
			case "head":
				return m_Player.InediaInfectedAI_GetNetSyncBoolVariable("HeadDeepWoundIsActive");
			break;

			case "arms":
				return m_Player.InediaInfectedAI_GetNetSyncBoolVariable("ArmsDeepWoundIsActive");
			break;
			
			case "legs":
				return m_Player.InediaInfectedAI_GetNetSyncBoolVariable("LegsDeepWoundIsActive");
			break;

			case "torso":
				return m_Player.InediaInfectedAI_GetNetSyncBoolVariable("TorsoDeepWoundIsActive");
			break;
		}
	
		return false;
	}
	
	bool CanGetDeepWoundFromHit(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!damageResult || !source || !LimbsDeepWoundSystemIsActive()) {
			return false;
		}
			
		string inediaPainLimb = GetInediaPainLimbFromDmgZone(dmgZone);
		if (!inediaPainLimb) {
			return false;
		}
		
		float healthDamage = damageResult.GetDamage(dmgZone, "Health") * m_Player.m_InediaInfectedAI_DamageMultiplier;
		if (inediaPainLimb == "head") {
			healthDamage *= 3;
		}

		float minSensitivityDamage = 3;
		if (damageType == DT_FIRE_ARM) {
			minSensitivityDamage = 1;
		}
			
		float bloodDamage = damageResult.GetDamage(dmgZone, "Blood");
		if (bloodDamage <= 0 || (ammo != "InediaInfectedAI_ThrowingProjectile" && healthDamage < minSensitivityDamage)) {
			return false;
		}

		float bloodArmor = 1;
		if (ammo == "InediaInfectedAI_ThrowingProjectile") {
			bloodArmor = Math.Clamp(bloodDamage / ItemBase.InediaInfectedAI_ThrowingProjectileGetAmmoBloodDamage(), 0, 1);
		} else {
			if (damageType == DT_CLOSE_COMBAT || bloodDamage <= 50) {
				bloodArmor = Math.Clamp(bloodDamage / 100, 0, 1);
			}
		}
		
		if (bloodArmor == 0) {
			return false;
		}
		
		float deepWoundChancePercent;
		if (ammo == "InediaInfectedAI_ThrowingProjectile") {
			deepWoundChancePercent = 100;
		} else {
			if (damageType == DT_FIRE_ARM && source.IsInherited(Archery_Base)) {
				// Since archery base weapons do not cause bleeding - use a special logic for such weapons.
				deepWoundChancePercent = 100;
			} else {
				deepWoundChancePercent = 100 * GetGame().ConfigGetFloat("CfgAmmo " + ammo + " DamageApplied bleedThreshold");
			}
		}
		
		// Creatures chance multipliers
		float creatureDeepWoundChancesMultiplier = 1;
		
		if (ammo == "InediaInfectedAI_ThrowingProjectile") {
			ItemBase projectile = ItemBase.Cast(source);
			if (!projectile || !projectile.InediaInfectedAI_IsThrowingProjectile()) {
				return false;
			}
			
			creatureDeepWoundChancesMultiplier = projectile.InediaInfectedAI_ThrowingProjectileGetBleedingChancePercent() / 100 * projectile.m_InediaInfectedAI_ThrowingProjectile.m_TargetDeepWoundChanceMultiplier;
		} else if (source.IsInherited(ZombieBase)) {
			ZombieBase zmb = ZombieBase.Cast(source);
			if (!zmb || !zmb.m_InediaInfectedAI_ConfigZombie || !zmb.m_InediaInfectedAI_ConfigZombie.PainToPlayerHandlerIsActive) {
				return false;
			}
			
			creatureDeepWoundChancesMultiplier *= 0.1 * zmb.m_InediaInfectedAI_ConfigZombie.PainToPlayerLimbsDeepWoundChancesMultiplier * zmb.InediaInfectedAI_GetInjuryBleedingChanceMultiplier();
		} else if (source.IsInherited(AnimalBase)) {
			AnimalBase animal = AnimalBase.Cast(source);
			if (!animal || !animal.m_InediaInfectedAI_ConfigZombie || !animal.m_InediaInfectedAI_ConfigZombie.PainToPlayerHandlerIsActive) {
				return false;
			}
			
			creatureDeepWoundChancesMultiplier *= animal.m_InediaInfectedAI_ConfigZombie.PainToPlayerLimbsDeepWoundChancesMultiplier;
		} else {
			PlayerBase player = PlayerBase.Cast(source.GetHierarchyRootPlayer());
			if (!player || !player.m_InediaInfectedAI_ConfigZombie || !player.m_InediaInfectedAI_ConfigZombie.PainToPlayerHandlerIsActive) {
				return false;
			}
			
			if (damageType == DT_CLOSE_COMBAT) {
				creatureDeepWoundChancesMultiplier *= player.m_InediaInfectedAI_ConfigZombie.PainToPlayerLimbsDeepWoundChancesMultiplier;
				if (ammo.Contains("MeleeFist") && inediaPainLimb != "head") {
					// Blunt strikes with fists or objects in hand should not cause deep wounds to any part of the body except the head.
					
					return false;
				}
			}
		}
		
		deepWoundChancePercent *= creatureDeepWoundChancesMultiplier;
		
		// When receiving damage to the head, the chance of sustaining a deep wound increases by 3 times.
		if (inediaPainLimb == "head") {
			deepWoundChancePercent *= 3;
		}
		
		deepWoundChancePercent = Math.Clamp(deepWoundChancePercent, 0, 100);
		
		// The armor and block multipliers must be applied to the resulting chance after the clamp.
		deepWoundChancePercent *= bloodArmor;
		
		if (damageType == DT_CLOSE_COMBAT && m_Player.InediaInfectedAI_IsInBlock()) {
			deepWoundChancePercent *= 0.5;
		}

		if (deepWoundChancePercent > 0 && Math.RandomFloatInclusive(0, 100) <= deepWoundChancePercent) {
			return true;
		}
		
		return false;
	}
	
	void ActivateDeepWoundBandageForLimb(string limb, float effectiveDuration, float regenerationMultiplier = 1)
	{
		if (!LimbsDeepWoundSystemIsActive()) {
			return;
		}
		
		switch (limb) {
			case "head":
				m_Player.InediaInfectedAI_SetNetSyncBoolVariable("HeadDeepWoundBandageIsActive", true);
				SetDeepWoundBandageEffectivenessTimeRemainingSecondsForLimb(limb, effectiveDuration);
				SetDeepWoundBandageRegenerationMultiplierForLimb(limb, regenerationMultiplier);
			break;

			case "arms":
				m_Player.InediaInfectedAI_SetNetSyncBoolVariable("ArmsDeepWoundBandageIsActive", true);
				SetDeepWoundBandageEffectivenessTimeRemainingSecondsForLimb(limb, effectiveDuration);
				SetDeepWoundBandageRegenerationMultiplierForLimb(limb, regenerationMultiplier);
			break;
			
			case "legs":
				m_Player.InediaInfectedAI_SetNetSyncBoolVariable("LegsDeepWoundBandageIsActive", true);
				SetDeepWoundBandageEffectivenessTimeRemainingSecondsForLimb(limb, effectiveDuration);
				SetDeepWoundBandageRegenerationMultiplierForLimb(limb, regenerationMultiplier);
			break;

			case "torso":
				m_Player.InediaInfectedAI_SetNetSyncBoolVariable("TorsoDeepWoundBandageIsActive", true);
				SetDeepWoundBandageEffectivenessTimeRemainingSecondsForLimb(limb, effectiveDuration);
				SetDeepWoundBandageRegenerationMultiplierForLimb(limb, regenerationMultiplier);
			break;
		}
	}
	
	void DeactivateDeepWoundBandageForLimb(string limb, bool spawnUsedRag = true)
	{
		if (!LimbsDeepWoundSystemIsActive()) {
			return;
		}
		
		if (!IsLimbDeepWoundBandaged(limb)) {
			return;
		}
		
		if (spawnUsedRag) {
			// When the bandage is deactivated, bloody rags appear on the ground.
			ItemBase rag = ItemBase.Cast(m_Player.SpawnEntityOnGroundPos("Rag", m_Player.GetPosition() + 0.6 * m_Player.GetDirection()));
			if (rag) {
				rag.SetQuantity(1);
				rag.SetHealth("", "", rag.GetMaxHealth("", "") * GameConstants.DAMAGE_RUINED_VALUE);
			}
		}
		
		switch (limb) {
			case "head":
				m_Player.InediaInfectedAI_SetNetSyncBoolVariable("HeadDeepWoundBandageIsActive", false);
			break;

			case "arms":
				m_Player.InediaInfectedAI_SetNetSyncBoolVariable("ArmsDeepWoundBandageIsActive", false);
			break;
			
			case "legs":
				m_Player.InediaInfectedAI_SetNetSyncBoolVariable("LegsDeepWoundBandageIsActive", false);
			break;

			case "torso":
				m_Player.InediaInfectedAI_SetNetSyncBoolVariable("TorsoDeepWoundBandageIsActive", false);
			break;
		}
	}
	
	bool IsLimbDeepWoundBandaged(string limb)
	{
		if (!LimbsDeepWoundSystemIsActive()) {
			return false;
		}
		
		switch (limb) {
			case "head":
				return m_Player.InediaInfectedAI_GetNetSyncBoolVariable("HeadDeepWoundBandageIsActive");
			break;

			case "arms":
				return m_Player.InediaInfectedAI_GetNetSyncBoolVariable("ArmsDeepWoundBandageIsActive");
			break;
			
			case "legs":
				return m_Player.InediaInfectedAI_GetNetSyncBoolVariable("LegsDeepWoundBandageIsActive");
			break;

			case "torso":
				return m_Player.InediaInfectedAI_GetNetSyncBoolVariable("TorsoDeepWoundBandageIsActive");
			break;
		}
	
		return false;
	}
	
	void SetDeepWoundBandageEffectivenessTimeRemainingSecondsForLimb(string limb, float effectiveDuration)
	{
		if (!LimbsDeepWoundSystemIsActive()) {
			return;
		}
		
		if (effectiveDuration < 0) {
			effectiveDuration = 0;
		}
		
		switch (limb) {
			case "head":
				m_HeadDeepWoundBandageItemEffectivenessTimeRemainingSeconds = effectiveDuration;
			break;

			case "arms":
				m_ArmsDeepWoundBandageItemEffectivenessTimeRemainingSeconds = effectiveDuration;
			break;
			
			case "legs":
				m_LegsDeepWoundBandageItemEffectivenessTimeRemainingSeconds = effectiveDuration;
			break;

			case "torso":
				m_TorsoDeepWoundBandageItemEffectivenessTimeRemainingSeconds = effectiveDuration;
			break;
		}
	}
	
	float GetDeepWoundBandageEffectivenessTimeRemainingSecondsForLimb(string limb)
	{
		if (!LimbsDeepWoundSystemIsActive()) {
			return 0;
		}
		
		switch (limb) {
			case "head":
				return m_HeadDeepWoundBandageItemEffectivenessTimeRemainingSeconds;
			break;

			case "arms":
				return m_ArmsDeepWoundBandageItemEffectivenessTimeRemainingSeconds;
			break;
			
			case "legs":
				return m_LegsDeepWoundBandageItemEffectivenessTimeRemainingSeconds;
			break;

			case "torso":
				return m_TorsoDeepWoundBandageItemEffectivenessTimeRemainingSeconds;
			break;
		}
	
		return 0;
	}
	
	void SetDeepWoundBandageEffectivenessForLimb(string limb, bool isEffective)
	{
		if (!LimbsDeepWoundSystemIsActive()) {
			return;
		}
		
		if (!isEffective && IsLimbDeepWoundBandageEffective(limb)) {
			m_Player.InediaInfectedAI_PlaySoundServer(InediaInfectedAI_PlayerBaseSoundTypes.DEEP_WOUND);
		}
		
		switch (limb) {
			case "head":
				m_Player.InediaInfectedAI_SetNetSyncBoolVariable("HeadDeepWoundBandageIsEffective", isEffective);
			break;

			case "arms":
				m_Player.InediaInfectedAI_SetNetSyncBoolVariable("ArmsDeepWoundBandageIsEffective", isEffective);
			break;
			
			case "legs":
				m_Player.InediaInfectedAI_SetNetSyncBoolVariable("LegsDeepWoundBandageIsEffective", isEffective);
			break;

			case "torso":
				m_Player.InediaInfectedAI_SetNetSyncBoolVariable("TorsoDeepWoundBandageIsEffective", isEffective);
			break;
		}
	}
	
	bool IsLimbDeepWoundBandageEffective(string limb)
	{
		if (!LimbsDeepWoundSystemIsActive()) {
			return false;
		}
		
		switch (limb) {
			case "head":
				return m_Player.InediaInfectedAI_GetNetSyncBoolVariable("HeadDeepWoundBandageIsEffective");
			break;

			case "arms":
				return m_Player.InediaInfectedAI_GetNetSyncBoolVariable("ArmsDeepWoundBandageIsEffective");
			break;
			
			case "legs":
				return m_Player.InediaInfectedAI_GetNetSyncBoolVariable("LegsDeepWoundBandageIsEffective");
			break;

			case "torso":
				return m_Player.InediaInfectedAI_GetNetSyncBoolVariable("TorsoDeepWoundBandageIsEffective");
			break;
		}
		
		return false;
	}
	
	void SetDeepWoundBandageRegenerationMultiplierForLimb(string limb, float regenerationMultiplier = 1)
	{
		if (!LimbsDeepWoundSystemIsActive()) {
			return;
		}
		
		if (regenerationMultiplier < 1) {
			regenerationMultiplier = 1;
		}
		
		switch (limb) {
			case "head":
				m_HeadDeepWoundBandageItemRegenerationMultiplier = regenerationMultiplier;
			break;

			case "arms":
				m_ArmsDeepWoundBandageItemRegenerationMultiplier = regenerationMultiplier;
			break;
			
			case "legs":
				m_LegsDeepWoundBandageItemRegenerationMultiplier = regenerationMultiplier;
			break;

			case "torso":
				m_TorsoDeepWoundBandageItemRegenerationMultiplier = regenerationMultiplier;
			break;
		}
	}
	
	float GetDeepWoundBandageRegenerationMultiplierForLimb(string limb)
	{
		if (!LimbsDeepWoundSystemIsActive()) {
			return 1;
		}
		
		switch (limb) {
			case "head":
				return m_HeadDeepWoundBandageItemRegenerationMultiplier;
			break;

			case "arms":
				return m_ArmsDeepWoundBandageItemRegenerationMultiplier;
			break;
			
			case "legs":
				return m_LegsDeepWoundBandageItemRegenerationMultiplier;
			break;

			case "torso":
				return m_TorsoDeepWoundBandageItemRegenerationMultiplier;
			break;
		}
	
		return 1;
	}

	void SyncDeepWoundRegenerationLevelForLimbServer(string limb, float regenerationMultiplier)
	{
#ifdef SERVER
		if (!LimbsDeepWoundSystemIsActive()) {
			return;
		}
		
		if (regenerationMultiplier < 1) {
			regenerationMultiplier = 1;
		}
		
		bool needSync = false;
		
		int newLevel;
		if (regenerationMultiplier > 4) {
			newLevel = InediaInfectedAI_LimbRegenerationLevels.HIGH;
		} else if (regenerationMultiplier > 2) {
			newLevel = InediaInfectedAI_LimbRegenerationLevels.MEDIUM;
		} else if (regenerationMultiplier > 1) {
			newLevel = InediaInfectedAI_LimbRegenerationLevels.LOW;
		} else {
			newLevel = InediaInfectedAI_LimbRegenerationLevels.BASE;
		}
		
		if (m_LimbsDeepWoundRegenerationLevels.Contains(limb)) {
			if (m_LimbsDeepWoundRegenerationLevels[limb] != newLevel) {
				m_LimbsDeepWoundRegenerationLevels[limb] = newLevel;
				needSync = true;
			}
		}

		if (needSync) {
			if (m_Player.GetIdentity()) {
			    ScriptRPC rpc = new ScriptRPC();
			    rpc.Write(m_LimbsDeepWoundRegenerationLevels);
				rpc.Send(m_Player, InediaInfectedAI_PlayerBase_ERPCs.RPC_SYNC_LIMBS_DEEP_WOUND_REGENERATION_LEVELS, true, m_Player.GetIdentity());
			}
		}
#endif
	}
	InediaInfectedAI_LimbRegenerationLevels GetDeepWoundRegenerationLevelForLimbClient(string limb)
	{
#ifndef SERVER
		if (LimbsDeepWoundSystemIsActive()) {
			if (m_LimbsDeepWoundRegenerationLevels && m_LimbsDeepWoundRegenerationLevels.Contains(limb)) {
				return m_LimbsDeepWoundRegenerationLevels[limb];
			}
		}
#endif
		
		return InediaInfectedAI_LimbRegenerationLevels.BASE;
	}
	
	float m_SetBulletLastTime = -1000000;
	void SetBulletCountInLimb(string limb, int count, float notMoreFrequentlyThanSeconds = 0)
	{
		if (!LimbsBulletSystemIsActive() || m_Player.InediaInfectedAI_IsNPC() || !m_Player.GetAllowDamage()) {
			return;
		}
		
		if (notMoreFrequentlyThanSeconds <= 0 || (GetGame().GetTickTime() - m_SetBulletLastTime) > notMoreFrequentlyThanSeconds) {
			m_SetBulletLastTime = GetGame().GetTickTime();
			
			count = Math.Clamp(count, 0, BULLET_COUNT_IN_LIMB_MAX);
			
			if (count != GetBulletCountInLimb(limb)) {
				int bit1 = (count >> 1) & 1;
				int bit2 = count & 1;
				
				switch (limb) {
					case "head":
						m_Player.InediaInfectedAI_SetNetSyncBoolVariable("HeadBulletCountBit1", bit1 != 0);
						m_Player.InediaInfectedAI_SetNetSyncBoolVariable("HeadBulletCountBit2", bit2 != 0);
					break;
		
					case "arms":
						m_Player.InediaInfectedAI_SetNetSyncBoolVariable("ArmsBulletCountBit1", bit1 != 0);
						m_Player.InediaInfectedAI_SetNetSyncBoolVariable("ArmsBulletCountBit2", bit2 != 0);
					break;
					
					case "legs":
						m_Player.InediaInfectedAI_SetNetSyncBoolVariable("LegsBulletCountBit1", bit1 != 0);
						m_Player.InediaInfectedAI_SetNetSyncBoolVariable("LegsBulletCountBit2", bit2 != 0);
					break;
		
					case "torso":
						m_Player.InediaInfectedAI_SetNetSyncBoolVariable("TorsoBulletCountBit1", bit1 != 0);
						m_Player.InediaInfectedAI_SetNetSyncBoolVariable("TorsoBulletCountBit2", bit2 != 0);
					break;
				}
			}
		}
	}
	
	int GetBulletCountInLimb(string limb)
	{
		if (!LimbsBulletSystemIsActive()) {
			return 0;
		}
		
		int bit1 = 0;
		int bit2 = 0;
		
		switch (limb) {
			case "head":
				if (m_Player.InediaInfectedAI_GetNetSyncBoolVariable("HeadBulletCountBit1")) {
					bit1 = 1;
				}
				if (m_Player.InediaInfectedAI_GetNetSyncBoolVariable("HeadBulletCountBit2")) {
					bit2 = 1;
				}
			break;

			case "arms":
				if (m_Player.InediaInfectedAI_GetNetSyncBoolVariable("ArmsBulletCountBit1")) {
					bit1 = 1;
				}
				if (m_Player.InediaInfectedAI_GetNetSyncBoolVariable("ArmsBulletCountBit2")) {
					bit2 = 1;
				}
			break;
			
			case "legs":
				if (m_Player.InediaInfectedAI_GetNetSyncBoolVariable("LegsBulletCountBit1")) {
					bit1 = 1;
				}
				if (m_Player.InediaInfectedAI_GetNetSyncBoolVariable("LegsBulletCountBit2")) {
					bit2 = 1;
				}
			break;

			case "torso":
				if (m_Player.InediaInfectedAI_GetNetSyncBoolVariable("TorsoBulletCountBit1")) {
					bit1 = 1;
				}
				if (m_Player.InediaInfectedAI_GetNetSyncBoolVariable("TorsoBulletCountBit2")) {
					bit2 = 1;
				}
			break;
		}
		
		return ((bit1 << 1) | bit2);
	}
	
	void SetBulletInBodySecondsForLimb(string limb, float time)
	{
		if (!LimbsBulletSystemIsActive()) {
			return;
		}
		
		switch (limb) {
			case "head":
				m_HeadBulletInBodySeconds = time;
			break;

			case "arms":
				m_ArmsBulletInBodySeconds = time;
			break;
			
			case "legs":
				m_LegsBulletInBodySeconds = time;
			break;

			case "torso":
				m_TorsoBulletInBodySeconds = time;
			break;
		}
	}
	
	float GetBulletInBodySecondsForLimb(string limb)
	{
		if (!LimbsBulletSystemIsActive()) {
			return 0;
		}
		
		switch (limb) {
			case "head":
				return m_HeadBulletInBodySeconds;
			break;

			case "arms":
				return m_ArmsBulletInBodySeconds;
			break;
			
			case "legs":
				return m_LegsBulletInBodySeconds;
			break;

			case "torso":
				return m_TorsoBulletInBodySeconds;
			break;
		}
		
		return 0;
	}
	
	bool CanGetBulletFromHit(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!damageResult || damageType != DT_FIRE_ARM || !source || !source.IsWeapon() || !LimbsBulletSystemIsActive()) {
			return false;
		}
		
		float healthDamage = damageResult.GetDamage(dmgZone, "Health") * m_Player.m_InediaInfectedAI_DamageMultiplier;
		float minSensitivityDamage = 3;
		if (damageType == DT_FIRE_ARM) {
			minSensitivityDamage = 1;
		}
		
		if (damageResult.GetDamage(dmgZone, "Blood") <= 0 || healthDamage < minSensitivityDamage) {
			return false;
		}
		
		if (source.IsInherited(Archery_Base)) {
			// Crossbows and bows should not cause bullet wounds.
			return false;
		}
		
		string ammoLower = ammo;
		ammoLower.ToLower();
		if (!ammoLower.Contains("bullet")) {
			return false;
		}
		
		float bulletRetentionChancePercent = Math.Clamp(100 - (healthDamage - minSensitivityDamage) / 2, 30, 100);
		
		if (bulletRetentionChancePercent > 0 && Math.RandomFloatInclusive(0, 100) <= bulletRetentionChancePercent) {
			return true;
		}
		
		return false;
	}
	
	void ActivateInternalBleeding()
	{
		if (!InternalBleedingSystemIsActive() || m_Player.InediaInfectedAI_IsNPC()) {
			return;
		}
		
		m_Player.InediaInfectedAI_SetNetSyncBoolVariable("IsInternalBleedingActive", true);
	}
	
	void DeactivateInternalBleeding()
	{
		if (!InternalBleedingSystemIsActive()) {
			return;
		}
		
		m_Player.InediaInfectedAI_SetNetSyncBoolVariable("IsInternalBleedingActive", false);
	}
	
	bool IsInternalBleedingActive()
	{
		if (!InternalBleedingSystemIsActive()) {
			return false;
		}
		
		return m_Player.InediaInfectedAI_GetNetSyncBoolVariable("IsInternalBleedingActive");
	}
	
// 23w3USpN6JQk+TjsiB1eplhHSjFjcy9vZDh6RFo4RzdRWDZhYnd6NW94VEpNRzJVdnlqN2JRMGFBOExDQTNVWTdVdlZWdUJNb3VSc2tVQ0JqenVqTU9sa29nYWhMVmtadUVPTGRjYXg5N3MyNkNBSEovR3R1Um5sZVYzRHhyTmx4eW5oWlk2U2ZhTDFKb2dQdEZXVEdxM3R1TzJ3QktRQjN6TDdSNTFHbDMrUDdrNENmZUpjcVBvZ0FoN1I4VU14dDFqdGE2bjA5T2FSNVl4R044bmJsdnNITWowcmk3dmVOMDRXcWhWNGJuOGZTMEc5VzhIS21BOUUyaVNtVUxvdm13MlVaczlUdnRQY0oyUTBxSUhpR21YVE5aYXlvUjQ1QUZGeTlnPT0=
	
	bool CanGetInternalBleedingFromHit(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!damageResult || !source || !InternalBleedingSystemIsActive() || IsInternalBleedingActive()) {
			return false;
		}
			
		string inediaPainLimb = GetInediaPainLimbFromDmgZone(dmgZone);
		if (!inediaPainLimb || inediaPainLimb != "torso") {
			return false;
		}
		
		float healthDamage = damageResult.GetDamage(dmgZone, "Health") * m_Player.m_InediaInfectedAI_DamageMultiplier;
		float minSensitivityDamage = 3;
		if (damageType == DT_FIRE_ARM) {
			minSensitivityDamage = 1;
		}
		
		if (damageResult.GetDamage(dmgZone, "Blood") <= 0 || healthDamage < minSensitivityDamage) {
			return false;
		}
		
		float internalBleedingChance;
		if (damageType == DT_FIRE_ARM && source.IsInherited(Archery_Base)) {
			// Since archery base weapons do not cause bleeding - use a special logic for such weapons.
			internalBleedingChance = 100;
		} else {
			internalBleedingChance = 100 * GetGame().ConfigGetFloat("CfgAmmo " + ammo + " DamageApplied bleedThreshold");
		}

		internalBleedingChance *= Math.Clamp(healthDamage, minSensitivityDamage, 100) / 100;
		
		if (internalBleedingChance > 0 && Math.RandomFloatInclusive(0, 100) <= internalBleedingChance) {
			return true;
		}
		
		return false;
	}
	
	InediaInfectedAI_BulletInBodyStages GetBulletInBodyStageForLimb(string limb)
	{
		float bulletInBodySeconds = GetBulletInBodySecondsForLimb(limb);
		
		if (bulletInBodySeconds >= m_Player.m_InediaInfectedAI_ConfigPlayers.LimbsBulletTimeToSecondStageSeconds) {
			return InediaInfectedAI_BulletInBodyStages.SECOND;
		} else {
			return InediaInfectedAI_BulletInBodyStages.FIRST;
		}
	}

	float GetPainArmsHandShakeMultiplier()
	{
		if (!PainSystemIsActive()) {
			return 0;
		}

		m_Player.GetMovementState(m_Player.m_MovementState);
		if (m_Player.m_MovementState && m_Player.m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE) {
			// When lying down - hands don't shake.
			return 0;
		}
		
		// If the character is stunned (blur, tinnitus, etc.), their weapon will sway at a fixed multiplier.
		if (m_HeadHitWeaponSwayIsActive) {
			return 24;
		}

		InediaInfectedAI_LimbPainLevels painArmsLevel = GetPainWithBuffsLevelForLimb("arms");

		float shakeMultiplier = 0;
		
		if (painArmsLevel >= InediaInfectedAI_LimbPainLevels.BADLY) {
			shakeMultiplier = 8;
		} else if (painArmsLevel >= InediaInfectedAI_LimbPainLevels.MEDIUM) {
			shakeMultiplier = 4;
		} else if (painArmsLevel >= InediaInfectedAI_LimbPainLevels.LOW) {
			shakeMultiplier = 2;
		}
		
		if (shakeMultiplier > 0) {
			shakeMultiplier *= GetActionsShockMultiplierForLimb("arms");
		}

		return shakeMultiplier;
	}
	
	float m_InediaInfectedAI_PainBlurRequesterStrength = 0;
	float m_InediaInfectedAI_PainBlurRequesterStrengthLastGetTime = 0;
	float InediaInfectedAI_GetPainBlurRequesterStrength()
	{
		if ((GetGame().GetTickTime() - m_InediaInfectedAI_PainBlurRequesterStrengthLastGetTime) <= 3) {
			return m_InediaInfectedAI_PainBlurRequesterStrength;
		}
		m_InediaInfectedAI_PainBlurRequesterStrengthLastGetTime = GetGame().GetTickTime();
			
		m_InediaInfectedAI_PainBlurRequesterStrength = 0;

		float lowIncrease = 0.05;
		float mediumIncrease = 0.15;
		float badlyIncrease = 0.25;

		foreach (string inediaPainLimb: InediaInfectedAI_PainManager.m_InediaPainLimbs) {
			InediaInfectedAI_LimbPainLevels painLevel = GetPainWithBuffsLevelForLimb(inediaPainLimb);
			float limbIncreaseStrength = 0;

			if (painLevel >= InediaInfectedAI_LimbPainLevels.BADLY) {
				limbIncreaseStrength += badlyIncrease;
			} else if (painLevel >= InediaInfectedAI_LimbPainLevels.MEDIUM) {
				limbIncreaseStrength += mediumIncrease;
			} else if (painLevel >= InediaInfectedAI_LimbPainLevels.LOW) {
				limbIncreaseStrength += lowIncrease;
			}

			if (painLevel >= InediaInfectedAI_LimbPainLevels.LOW) {
				if (inediaPainLimb == "head") {
					limbIncreaseStrength += badlyIncrease;
				} else if (inediaPainLimb == "torso") {
					if (IsInternalBleedingActive()) {
						limbIncreaseStrength += badlyIncrease;
					}
				}
					
				if (IsLimbBroken(inediaPainLimb) && !IsLimbStabilized(inediaPainLimb)) {
					limbIncreaseStrength += badlyIncrease;
				}
	
				if (GetBulletCountInLimb(inediaPainLimb) > 0) {
					limbIncreaseStrength += badlyIncrease;
				}
			}

			m_InediaInfectedAI_PainBlurRequesterStrength += limbIncreaseStrength;
		}

		m_InediaInfectedAI_PainBlurRequesterStrength = Math.Clamp(m_InediaInfectedAI_PainBlurRequesterStrength, 0, 1);
			
		return m_InediaInfectedAI_PainBlurRequesterStrength;
	}

	float GetPainArmsDamageMultiplier()
	{
		if (!PainSystemIsActive()) {
			return 1;
		}

		InediaInfectedAI_LimbPainLevels painArmsLevel = GetPainWithBuffsLevelForLimb("arms");

		if (painArmsLevel >= InediaInfectedAI_LimbPainLevels.BADLY) {
			return 0.4;
		} else if (painArmsLevel >= InediaInfectedAI_LimbPainLevels.MEDIUM) {
			return 0.6;
		} else if (painArmsLevel >= InediaInfectedAI_LimbPainLevels.LOW) {
			return 0.8;
		}

		return 1;
	}
	
	// The method is called when the player uses any item, regardless of whether they consume it internally or simply use it.
	void UseItem(ItemBase item, float consumedPercent = 100)
	{
		if (!PainSystemIsActive() || !item || consumedPercent <= 0) {
			return;
		}
		
        // Health regen buff logic
        if (item.m_InediaInfectedAI_IsHealthRegenItem == 1) {
            ReloadMedicalModifier(InediaInfectedAI_eModifiers.MDF_HEALTH_REGEN, consumedPercent / 100 * GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain HealthRegenItemDurationSeconds"), GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain HealthRegenItemRegenMultiplier"));
        }

        // Bone regen buff logic
        if (LimbsBreakSystemIsActive() && item.m_InediaInfectedAI_IsBoneRegenItem == 1) {
            ReloadMedicalModifier(InediaInfectedAI_eModifiers.MDF_BONE_REGEN, consumedPercent / 100 * GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain BoneRegenItemDurationSeconds"), GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain BoneRegenItemRegenMultiplier"));
        }

        // Blood regen buff logic
        if (item.m_InediaInfectedAI_IsBloodRegenItem == 1) {
            ReloadMedicalModifier(InediaInfectedAI_eModifiers.MDF_BLOOD_REGEN, consumedPercent / 100 * GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain BloodRegenItemDurationSeconds"), GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain BloodRegenItemRegenMultiplier"));
        }

        // Blood clotting buff logic
        if (item.m_InediaInfectedAI_IsBloodClottingItem == 1) {
            ReloadMedicalModifier(InediaInfectedAI_eModifiers.MDF_BLOOD_CLOTTING, consumedPercent / 100 * GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain BloodClottingItemDurationSeconds"), GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain BloodClottingItemClottingMultiplier"));
        }

        // Deep wound regen buff logic
        if (LimbsDeepWoundSystemIsActive() && item.m_InediaInfectedAI_IsDeepWoundRegenItem == 1) {
            ReloadMedicalModifier(InediaInfectedAI_eModifiers.MDF_DEEP_WOUND_REGEN, consumedPercent / 100 * GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain DeepWoundRegenItemDurationSeconds"), GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain DeepWoundRegenItemRegenMultiplier"));
        }
		
		// Vomit relie buff logic
        if (item.m_InediaInfectedAI_IsVomitReliefItem == 1) {
            ReloadMedicalModifier(InediaInfectedAI_eModifiers.MDF_VOMIT_RELIE, consumedPercent / 100 * GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain VomitReliefItemDurationSeconds"));
        }

        // Painkiller light buff logic
        if (item.m_InediaInfectedAI_IsPainkillerLightItem == 1) {
            ReloadMedicalModifier(InediaInfectedAI_eModifiers.MDF_PAINKILLER_LIGHT, consumedPercent / 100 * GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain PainkillerLightItemDurationSeconds"));
        }

        // Painkiller medium buff logic
        if (item.m_InediaInfectedAI_IsPainkillerMediumItem == 1) {
            ReloadMedicalModifier(InediaInfectedAI_eModifiers.MDF_PAINKILLER_MEDIUM, consumedPercent / 100 * GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain PainkillerMediumItemDurationSeconds"));
        }

        // Painkiller strong buff logic
        if (item.m_InediaInfectedAI_IsPainkillerStrongItem == 1) {
            ReloadMedicalModifier(InediaInfectedAI_eModifiers.MDF_PAINKILLER_STRONG, consumedPercent / 100 * GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " InediaPain PainkillerStrongItemDurationSeconds"));
        }
	}
	
	// The method is called when the player consume any liquid.
	void UseLiquid(int liquidId, float consumedPercent = 100)
	{
		if (!PainSystemIsActive() || liquidId <= 0 || consumedPercent <= 0) {
			return;
		}
		
		string liquidClassName = Liquid.GetLiquidClassname(liquidId);
		if (!liquidClassName) {
			return;
		}
		
        // Health regen buff logic
        if (GetGame().ConfigGetInt("cfgLiquidDefinitions " + liquidClassName + " InediaPain IsHealthRegenItem") == 1) {
            ReloadMedicalModifier(InediaInfectedAI_eModifiers.MDF_HEALTH_REGEN, consumedPercent / 100 * GetGame().ConfigGetFloat("cfgLiquidDefinitions " + liquidClassName + " InediaPain HealthRegenItemDurationSeconds"), GetGame().ConfigGetFloat("cfgLiquidDefinitions " + liquidClassName + " InediaPain HealthRegenItemRegenMultiplier"));
        }
        
        // Bone regen buff logic
        if (LimbsBreakSystemIsActive() && GetGame().ConfigGetInt("cfgLiquidDefinitions " + liquidClassName + " InediaPain IsBoneRegenItem") == 1) {
            ReloadMedicalModifier(InediaInfectedAI_eModifiers.MDF_BONE_REGEN, consumedPercent / 100 * GetGame().ConfigGetFloat("cfgLiquidDefinitions " + liquidClassName + " InediaPain BoneRegenItemDurationSeconds"), GetGame().ConfigGetFloat("cfgLiquidDefinitions " + liquidClassName + " InediaPain BoneRegenItemRegenMultiplier"));
        }
        
        // Blood regen buff logic
        if (GetGame().ConfigGetInt("cfgLiquidDefinitions " + liquidClassName + " InediaPain IsBloodRegenItem") == 1) {
            ReloadMedicalModifier(InediaInfectedAI_eModifiers.MDF_BLOOD_REGEN, consumedPercent / 100 * GetGame().ConfigGetFloat("cfgLiquidDefinitions " + liquidClassName + " InediaPain BloodRegenItemDurationSeconds"), GetGame().ConfigGetFloat("cfgLiquidDefinitions " + liquidClassName + " InediaPain BloodRegenItemRegenMultiplier"));
        }
        
        // Blood clotting buff logic
        if (GetGame().ConfigGetInt("cfgLiquidDefinitions " + liquidClassName + " InediaPain IsBloodClottingItem") == 1) {
            ReloadMedicalModifier(InediaInfectedAI_eModifiers.MDF_BLOOD_CLOTTING, consumedPercent / 100 * GetGame().ConfigGetFloat("cfgLiquidDefinitions " + liquidClassName + " InediaPain BloodClottingItemDurationSeconds"), GetGame().ConfigGetFloat("cfgLiquidDefinitions " + liquidClassName + " InediaPain BloodClottingItemClottingMultiplier"));
        }
        
        // Deep wound regen buff logic
        if (LimbsDeepWoundSystemIsActive() && GetGame().ConfigGetInt("cfgLiquidDefinitions " + liquidClassName + " InediaPain IsDeepWoundRegenItem") == 1) {
            ReloadMedicalModifier(InediaInfectedAI_eModifiers.MDF_DEEP_WOUND_REGEN, consumedPercent / 100 * GetGame().ConfigGetFloat("cfgLiquidDefinitions " + liquidClassName + " InediaPain DeepWoundRegenItemDurationSeconds"), GetGame().ConfigGetFloat("cfgLiquidDefinitions " + liquidClassName + " InediaPain DeepWoundRegenItemRegenMultiplier"));
        }
		
		// Vomit relief buff logic
        if (GetGame().ConfigGetInt("cfgLiquidDefinitions " + liquidClassName + " InediaPain IsVomitReliefItem") == 1) {
            ReloadMedicalModifier(InediaInfectedAI_eModifiers.MDF_VOMIT_RELIE, consumedPercent / 100 * GetGame().ConfigGetFloat("cfgLiquidDefinitions " + liquidClassName + " InediaPain VomitReliefItemDurationSeconds"));
        }
        
        // Painkiller light buff logic
        if (GetGame().ConfigGetInt("cfgLiquidDefinitions " + liquidClassName + " InediaPain IsPainkillerLightItem") == 1) {
            ReloadMedicalModifier(InediaInfectedAI_eModifiers.MDF_PAINKILLER_LIGHT, consumedPercent / 100 * GetGame().ConfigGetFloat("cfgLiquidDefinitions " + liquidClassName + " InediaPain PainkillerLightItemDurationSeconds"));
        }
        
        // Painkiller medium buff logic
        if (GetGame().ConfigGetInt("cfgLiquidDefinitions " + liquidClassName + " InediaPain IsPainkillerMediumItem") == 1) {
            ReloadMedicalModifier(InediaInfectedAI_eModifiers.MDF_PAINKILLER_MEDIUM, consumedPercent / 100 * GetGame().ConfigGetFloat("cfgLiquidDefinitions " + liquidClassName + " InediaPain PainkillerMediumItemDurationSeconds"));
        }
        
        // Painkiller strong buff logic
        if (GetGame().ConfigGetInt("cfgLiquidDefinitions " + liquidClassName + " InediaPain IsPainkillerStrongItem") == 1) {
            ReloadMedicalModifier(InediaInfectedAI_eModifiers.MDF_PAINKILLER_STRONG, consumedPercent / 100 * GetGame().ConfigGetFloat("cfgLiquidDefinitions " + liquidClassName + " InediaPain PainkillerStrongItemDurationSeconds"));
        }
	}
	
	void ReloadMedicalModifier(InediaInfectedAI_eModifiers modifierId, float newDuration, float param1 = 0)
	{
		ModifierBase modifier = m_Player.GetModifiersManager().GetModifier(modifierId);
		if (!modifier || newDuration <= 0) {
			return;
		}
		
		bool modifierIsActive = m_Player.GetModifiersManager().IsModifierActive(modifierId);
		bool reloadModifier = false;
		
		switch (modifierId) {
			case InediaInfectedAI_eModifiers.MDF_HEALTH_REGEN:
				float healthRegenMultiplierImpactCurrent = (m_HealthRegenItemDurationSeconds - modifier.GetAttachedTime()) * m_HealthRegenItemRegenMultiplier;
				float healthRegenMultiplierImpactNew = newDuration * param1;
				if (!modifierIsActive || healthRegenMultiplierImpactNew > healthRegenMultiplierImpactCurrent) {
					m_HealthRegenItemDurationSeconds = newDuration;
					m_HealthRegenItemRegenMultiplier = param1;
					reloadModifier = true;
				}
			break;
			
			case InediaInfectedAI_eModifiers.MDF_BONE_REGEN:
				float boneRegenMultiplierImpactCurrent = (m_BoneRegenItemDurationSeconds - modifier.GetAttachedTime()) * m_BoneRegenItemRegenMultiplier;
				float boneRegenMultiplierImpactNew = newDuration * param1;
				if (!modifierIsActive || boneRegenMultiplierImpactNew > boneRegenMultiplierImpactCurrent) {
					m_BoneRegenItemDurationSeconds = newDuration;
					m_BoneRegenItemRegenMultiplier = param1;
					reloadModifier = true;
				}
			break;

			case InediaInfectedAI_eModifiers.MDF_BLOOD_REGEN:
				float bloodRegenMultiplierImpactCurrent = (m_BloodRegenItemDurationSeconds - modifier.GetAttachedTime()) * m_BloodRegenItemRegenMultiplier;
				float bloodRegenMultiplierImpactNew = newDuration * param1;
				if (!modifierIsActive || bloodRegenMultiplierImpactNew > bloodRegenMultiplierImpactCurrent) {
					m_BloodRegenItemDurationSeconds = newDuration;
					m_BloodRegenItemRegenMultiplier = param1;
					reloadModifier = true;
				}
			break;
			
			case InediaInfectedAI_eModifiers.MDF_BLOOD_CLOTTING:
				float bloodClottingMultiplierImpactCurrent = (m_BloodClottingItemDurationSeconds - modifier.GetAttachedTime()) * m_BloodClottingItemClottingMultiplier;
				float bloodClottingMultiplierImpactNew = newDuration * param1;
				if (!modifierIsActive || bloodClottingMultiplierImpactNew > bloodClottingMultiplierImpactCurrent) {
					m_BloodClottingItemDurationSeconds = newDuration;
					m_BloodClottingItemClottingMultiplier = param1;
					reloadModifier = true;
				}
			break;
			
			case InediaInfectedAI_eModifiers.MDF_DEEP_WOUND_REGEN:
				float deepWoundRegenMultiplierImpactCurrent = (m_DeepWoundRegenItemDurationSeconds - modifier.GetAttachedTime()) * m_DeepWoundRegenItemRegenMultiplier;
				float deepWoundRegenMultiplierImpactNew = newDuration * param1;
				if (!modifierIsActive || deepWoundRegenMultiplierImpactNew > deepWoundRegenMultiplierImpactCurrent) {
					m_DeepWoundRegenItemDurationSeconds = newDuration;
					m_DeepWoundRegenItemRegenMultiplier = param1;
					reloadModifier = true;
				}
			break;
			
			case InediaInfectedAI_eModifiers.MDF_VOMIT_RELIE:
				if (!modifierIsActive || newDuration > (m_VomitRelieItemDurationSeconds - modifier.GetAttachedTime())) {
					m_VomitRelieItemDurationSeconds = newDuration;
					reloadModifier = true;
				}
			break;
			
			case InediaInfectedAI_eModifiers.MDF_PAINKILLER_LIGHT:
				if (!modifierIsActive || newDuration > (m_PainkillerLightItemDurationSeconds - modifier.GetAttachedTime())) {
					m_PainkillerLightItemDurationSeconds = newDuration;
					reloadModifier = true;
				}
			break;
			
			case InediaInfectedAI_eModifiers.MDF_PAINKILLER_MEDIUM:
				if (!modifierIsActive || newDuration > (m_PainkillerMediumItemDurationSeconds - modifier.GetAttachedTime())) {
					m_PainkillerMediumItemDurationSeconds = newDuration;
					reloadModifier = true;
				}
			break;
			
			case InediaInfectedAI_eModifiers.MDF_PAINKILLER_STRONG:
				if (!modifierIsActive || newDuration > (m_PainkillerStrongItemDurationSeconds - modifier.GetAttachedTime())) {
					m_PainkillerStrongItemDurationSeconds = newDuration;
					reloadModifier = true;
				}
			break;
		}
		
		if (reloadModifier) {
			if (m_Player.GetModifiersManager().IsModifierActive(modifierId)) {
			    m_Player.GetModifiersManager().DeactivateModifier(modifierId);
			}
			m_Player.GetModifiersManager().ActivateModifier(modifierId);
		}
	}
	
	float m_HeadHitActionLastTime = -60;
	bool m_HeadHitBlurRequesterIsActive = false;
	bool m_HeadHitWeaponSwayIsActive = false;
	void HeadHitAction(float powerMultiplier = 1, float notMoreFrequentlyThanSeconds = 0)
	{
#ifdef SERVER
		if (!m_Player.m_InediaInfectedAI_ConfigPlayers || !m_Player.IsAlive() || m_Player.IsUnconscious() || powerMultiplier < 0.25) {
			return;
		}
		
		if (notMoreFrequentlyThanSeconds <= 0 || (GetGame().GetTickTime() - m_HeadHitActionLastTime) > notMoreFrequentlyThanSeconds) {
			if (m_Player.m_InediaInfectedAI_ConfigPlayers.MovementCoordinationImpairmentWhenHeadReceivingHits) {
				m_Player.InediaInfectedAI_EnableMovementCoordinationImpairmentForSeconds(20 * powerMultiplier);
			}
			
			if (m_Player.GetIdentity()) {
			    ScriptRPC rpc = new ScriptRPC();
				
			    rpc.Write(powerMultiplier);
				
				rpc.Send(m_Player, InediaInfectedAI_PlayerBase_ERPCs.RPC_HEAD_HIT_ACTION, false, m_Player.GetIdentity());
			}

			m_HeadHitActionLastTime = GetGame().GetTickTime();
		}
#endif
	}
	
	void RestoreHeadHitSoundEffects()
	{
#ifndef SERVER
		if (m_Player.IsAlive() && !m_Player.IsUnconscious()) {
			GetGame().GetSoundScene().SetSoundVolume(g_Game.m_volume_sound, 1);
		}

		m_Player.SetMasterAttenuation("");
#endif
	}
	
	void HeadHitBlurRequesterStopClient()
	{
#ifndef SERVER
		m_HeadHitBlurRequesterIsActive = false;
#endif
	}
	
	void HeadHitWeaponSwayStopClient()
	{
#ifndef SERVER
		m_HeadHitWeaponSwayIsActive = false;
#endif
	}
	
	float GetActionsShockMultiplierForLimb(string limb)
	{
		float additionalMultiplier = 0;
		
		if (IsLimbBroken(limb)) {
			additionalMultiplier += 2;
			
			if (IsLimbStabilized(limb)) {
				float stabilizationShockReductionMultiplier = GetBreakStabilizationShockReductionMultiplierForLimb(limb);
				if (stabilizationShockReductionMultiplier < 1) stabilizationShockReductionMultiplier = 1;
				additionalMultiplier /= stabilizationShockReductionMultiplier;
			}
		}
		
		if (GetBulletCountInLimb(limb) > 0) {
			additionalMultiplier += 2;
		}
		
		additionalMultiplier = Math.Clamp(additionalMultiplier, 0, 2);
		
		return 1 + additionalMultiplier;
	}
	
	bool PainkillerStrongEffectIsActive()
	{
		if (m_Player.GetModifiersManager().IsModifierActive(InediaInfectedAI_eModifiers.MDF_PAINKILLER_STRONG)) {
			return true;
		}
		
		if (InediaInfectedAI_SyberiaCompatibility.SyberiaPainkillerStrongEffectIsActive(m_Player)) {
			return true;
		}
		
		if (m_Player.GetModifiersManager().IsModifierActive(eModifiers.MDF_MORPHINE) && !InediaInfectedAI_SyberiaCompatibility.IsSyberiaConnected()) {
			return true;
		}

#ifdef AdditionalMedicSupplies
		if (m_Player.GetModifiersManager().IsModifierActive(AMS_eModifiers.MDF_METHADONE)) {
			return true;
		}
#endif

		if (InediaInfectedAI_Restrictions.PainSystem_PainkillerStrongEffectIsActive(m_Player) || InediaInfectedAI_Restrictions.PainSystem_MorphineEffectIsActive(m_Player)) {
			return true;
		}
		
		return false;
	}
	
	bool PainkillerMediumEffectIsActive()
	{
		if (m_Player.GetModifiersManager().IsModifierActive(InediaInfectedAI_eModifiers.MDF_PAINKILLER_MEDIUM)) {
			return true;
		}
		
		if (InediaInfectedAI_SyberiaCompatibility.SyberiaPainkillerMediumEffectIsActive(m_Player)) {
			return true;
		}
		
		if (InediaInfectedAI_Restrictions.PainSystem_PainkillerMediumEffectIsActive(m_Player)) {
			return true;
		}

		return false;
	}

	bool PainkillerLightEffectIsActive()
	{
		if (m_Player.GetModifiersManager().IsModifierActive(InediaInfectedAI_eModifiers.MDF_PAINKILLER_LIGHT)) {
			return true;
		}
		
		if (InediaInfectedAI_SyberiaCompatibility.SyberiaPainkillerLightEffectIsActive(m_Player)) {
			return true;
		}
		
		if ((m_Player.GetModifiersManager().IsModifierActive(eModifiers.MDF_EPINEPHRINE) || m_Player.GetModifiersManager().IsModifierActive(eModifiers.MDF_PAINKILLERS)) && !InediaInfectedAI_SyberiaCompatibility.IsSyberiaConnected()) {
			return true;
		}
		
		if (InediaInfectedAI_Restrictions.PainSystem_PainkillerLightEffectIsActive(m_Player) || InediaInfectedAI_Restrictions.PainSystem_PainkillersEffectIsActive(m_Player)) {
			return true;
		}

		return false;
	}
	
//                                                                                                                                       -**= == ::=-
//                                                                                                                                      :#-= :#.:: =
//                                                                      .:-==+====-:.                                                 .=+++ :#.:: ::
//       .                                                        .-=+*%*=+==++*=--++##+-.                                            =+-* :#:::.:.
//       ==                                                 .-=++#%%##**::=+##%#++++++*+*#*=.                                        =**+-.#:::--
//       +%.                              :.            :=+#%%%#*==++**=*:%*+=::.::. :==-::=#=.                                     =#%+=:%-:.--
//       +#=                             :.          :+#%%*====+**=+#=#*#*=:::.    .::.  :=++**.                                   -#%++:%-::-:
//       +*+                                      :=##++**+++=---=*==#*#*=+++--+=-:   .---. .=#+                                  :*%#*:#=.:=-
//       +*=-                                  .=*#*%#*=-==++*+====**#***++=-:-=-::=-:    :-: .**.                               .+%##:#+.+:=
//       ==-*                                .++-*%*:.:---:::.#+*==%#*##+=-=*%*--=--.:=:    .-: -#:                             .+%##:#+.+.=
//       =-=:-:                            .++.-#*=*#%*+++=+*#%###%*--::.::::-#%#--=-- .=:    .:.:#-                            =%##.#* = -.
//       =-+ +=.                         .+=:=#%+*%***%%#==-*+-..   ::=-..::--:-+##:===- .--    ..:#=                          =%##:##:=.-.
//       -==.:=+                        ==.+%**+#++%%%%%+             .-*+=-::::-=%%+.+==-.:=:     :*=                        =#%#:##----.
//       .*-= +*.                     .+:+*+#=%=-#%%%%%%=                .-+##+--+%%%*.++.===-.      =+:                     -#%#:*%-:+-:
//        #.# :*=                    .=+++:+*%++=%%%%%%%:                    =*###%%%%#:++=. :-====-:..--:.                 -###:*%+.=:-
//        #.#: ##.                   +*-+=*#+%--#%#%%%%+.          .:-::.      :=**+*%%%-*+===-.   .:======::::            :*##:*%*.=--
//        +.== -%-            .     +==#**: *++-%%%%%%%=       .-+#%*=-----:      =#%%%%%-+=+-:-====-:   .:=+- -.         :+%*-+%* =--
//        -:-+  +#             ::..=:=*=-::+%#%+*%%%%%##%#=   =%%+:::.....         -#%#%%%+**+*+==-:::-----. :*+:        :*%*-=%* -=-
//        .---:  *:         .     .:++::+**%%%%%-%%%%%%*#%%#.   .**#*+=#+.          *+=##%%#++*+=:-=++===:.-++:-*.      :*%+==%%.:=-
//        :-.:=  -=         ..    =:#.-%##%*%%%%=#%%%%%%%#%#=     =---:              :*%#%%%%#+=-===--=--==+:.=*=*     .+%*+-%%-.:-
//        -- :+   #         .... ..:+ **=#%%%%%%%+%%%%%%+. +:                         =*.%%%%%#%%%#***+==-=-=+:.**-   .+%**:%%=.:-
//        =: :+   +:         .....  +=:*%##%%*%%#+%%%%%%:  =   .                      #.. .:%%%*+%#*%%%%%%*+==*+=*#  .+%+#-%%* --.
//        =. :=:  :+            -:  *=+***%*+%%%#%%%%%%%- +   :::-                   :-=::+%%%%%%++**#***##*%%*%+#%.:*%%%-#%%.-+.
//        =: .-=   *          :. .-==*=#**---#=#%%%%%%%%%.+%=*++:-                   :.#+##%%%%%%%%*++**+--+*#%%#%%+%%%%**@%:=+.
//        --  :*   =-          ::::=+###+:.  **%%%%%%%%%%**%#+.                     - :=  :+#%%%%%#%%%%###%****@%#%%%%%#+%%-++.
//        .+  :#   .*           ...:-*+#+==+**==*#%%%%%%%%%%+*--::.               ..  +.  .#:%%%%%%##%%%%%%%%#*+%%%%%%%#%%*+#.
//         *  :*.   *.              -+-+....  ::+:#%%%%%%%%%%##**===-:              -=    -# -%%%%%%#*#%%####%%#%%%*=-::::::-+++:
//         +. .=:   :+             .:=:+-      ::-.*%%%%%%%%%+-:---.             .==+.    *=  =%%%%%%%#*%%%%%%%#=..-----::::-:-:=*=
//         -=  ==    #.            . =:-*---:.  .::.-+%%%%%%%%*+==-          .:+#+:+.    -#.:-=##**##%%%%%%%%*:.--- .:=*#####*=-=:=#:
//         .*  .+    =-            . .+. ----==-:.:-..:=##%%%%+          .-+#%%*: +.  .-+#==---===+#%%%%%%%+.:--.-+**++===+*#%%%*=-:++.
//          *.  *    .*             . .++: .....   :==-::--+*%%*:. .:-+*%%%%%#:  :==+*=:  .::::-==++*%%%%*::=:-+=----------::.-*%%%+:-*-
//          =:  +    -+              ..:+=++=-:..    ..:=+*#%%%%%%%%%%%%%%%%%+.=+*+-:.  .--=++=---=*%%%%- -:-=:--:           .. .+%%#---+=:
//          --  =: :- =                 :===++*#%%%%%%%%%%###*%%%%%%%%%%%%%%%#*+--:..:-+=::.  .:-==+#%*.:-:---:                   .*%%::::-=+:
//          --  -- +  =                 .::.:=+#########*+***%%%%%%%%%%%%%%#++=-:.:-+-:  .=+****###%%:.=--==.     .----:::::::::::::+*%*=.:=:+*
//          :=  .= =: =                 .: .::..::----===*%#*-.  ..+%%%%%%+**=..-=-:  .-**=++=**+*%%.:=-++.    :--:      ........    -=+%%=.=:=+
//           *   + .+ -.                 .:.  ..:::-+++++-:        .#%%%%%#=.::::..:=+++-=++++*-#%%::=-*:     +:                     .=-=%%*:*.#-
//           +.  *  + ::                    .:::----::.       :-=+++#%-==- .: .--=+=-+*#%=:.=*:#+=* + =-     .%                       -: -*%*-=:%.
//           --  +. --.-                                  .=*%%+=+#%%%*===+=++==+**%#+*#*+*#%:** *# + =-:    :*                        .-  %%:* *=
//           .*  =:  * =                               .=#%%*+-*%%%%%%%%%*++*#%%#+==****=--.*:%- %%#+=---::---+====-:.                   - -%*-=:#
//            *  :=  -:=                             .=#%*=--+%%%%#:%%%%%%%%%#*+=+++=-::--+-*-%: #%%%%#*+++=--======--=====:.             - *%.+ #:
//            =-  *  :.+                             =*+::--#%##+##=.+%%#*****=--------.  -=*=+- =+*%%%%%%*====+=-:-=-----::-===-:.       ..:@%*-+=
//            .*  *  ::+                            :+::==-%#*+**:    :=+++=--::::.       :**-:+  =  :=+#%%%%%%*++=--==-::-:----:.:--::.::-=+#*#*:*
//             *. =: .:+                           +=:==.+%#+*#*                          .##+- =..+     #*-=*%%%%%%%*+=-:--:--:.::::. ::-=+. :+*.%
//             =-::=  -+.                        .+=--: *%#+#:+*:                  ..:   :--:%+=.-:.=    ++    #%%*%%%%%%%*++--:---:        :-:+++#.
//             .+: +  -=:                       .+=-:: *####  .*=- .:--+#+=---=+*#****==+=+*%%%*=-:-:=   -+.   .##%==+*#%%%%%%###+--::.       =.*%*.
//              *:.=  --=                       *=--- *%%%==+  .=++=-=*+=-:-++==:..::----*: .*%%%+==-:+.  -+    -*#%#-::-=+=*#%%%%%%#+=-.   ..::*%*:
//              +.::- --+                      ==-=- +%%*- -:-    -*+-.:--=-:-----:..  ..-#=  :%%%#=*+ --  -*:   %=*@%%+=-:=-===*#%%%%#*+=--.:=*.*#:
//              --: + -:+                     .#::-.*%%%%:= +=  =*=:::-++==:.              *=  =%*%%*-=-.=:  -=: *=+@+=#%%#**=-==-=*#%%%%%*+=+=--*#.
//              :+: + -.*                     +=.***=-.*%== =-=+-::-+*+:     :::::::.      .%.  #*=%%%*--::=:  ::#:=#:::.:=*%%%%#*=-==+*%%%**#*==*=
//              .* .=.- #                     #+*=...  =%+= -*-..-+*-  .::::.        ....   =*  =%:-%%%%*=-::-: :-:*+:......:-=+#%%%#*+==+##=
//               * .:=: #                   -##-..     +@-= +- .=+- .::.                    .%. -@- .*%%%%%+-:.:--**.:::...  :::=+++*%%%#+==#=
//               +.: *:.#                 .**+         *% =:* .=+..:.      .     ...         %: +%..  -#%%%%%*+-+#*--::  :--=-:: .:--.:+%%#+*%:
//               =-: +..#                :#=+  .       %#.=+. ++...       :    .:            %..%*  -   -#%%%%%%%*-.  :---::  :--.:-++++*#%%*#+
//               :+:.-:.#               .#-#.   ..     %*:-* ==- .       -    :             -* +%-   =-   :#%%%%:  .---:.  :-==+++-:      .+%%%.
//                # -.=:#.              -#-*           ##:=* *=..       -.   :             .+ -%+     .=----=%%%%-::--  .-==*+:             :#@:
//                * - =:#.              =*=+           +%:*+:+=:        +    .            .+.=#*            .*%%%%+:  .==+#-             ..   +=
//                =:- ==*.              -%-*           :@**=*:*=        +                -=:*#+   ...      .#:+%%%%#.=*#%+       ... .:----==++*#.
//                --- -+#.               +%#=:    .:.   *%#%#+===.      +              -+-**#-.----::===.  -= -%%%%%%%%%--+    ..:---+++***+===*#:
//                .+- :**                 -%#+=.    .:-=-*%%%%*==::.    :=           :+**+*#=-:         -:.-=  %%%%%%%%.   =..--=++*+=-.       .:#.
//                 +:::#:                  .*#+===:     :-=%%%%%=+=:     -=       .-*#+=+#+:  .-===*+::.  :=%: :%%%%%%%%-  -*++*+-....           :#.
//                 +.=-#-:+-                 :+#*=-----=++=-#-= =+:-+++-:..-::--+*+-.:+*-. .=+=.    :==-::. :#= :#%%%%%%%#**#+:  ..             . :#.
//               :=+==-::##*                    :*%%#*+===+*--%+..==:..-=*##*+=-. .===:.:=+-.          :=-:--:-#*+%%%%%%%%%+:+. .          :-::.   :*.
//             -=-:..   :+#%+=:                 :%%%%%#%%-:- :# -:  .::::.     :==-..:==:     .::---------*-.:==+%%%%%%#:+#-+.+:        :--.   .-=--=+--:
//           -=:..:-:   -*.:=:*-                 *%%%*.*-=- .-=- =..         -=:.. =+:    ..:::.   .....  .-%%#+%%%%%%#:  =#:+.+.    .==. .:=+=+++#%%%%%+
//          :*::==:==+==%:  .+=*                 *%%%%+=:  . +.* :: ......  --:  .*-    ..  .-====--:::--:-+*%%%%%%%%+.    -#:*.+  -*=:=*##+=+%%%%%#=--%-
//          *--. ...    =%++*: *.                *%==%%+*  . * +  =      : -:.  :*.      .==-.   :-----=*%%%%%%#%%%#-       -#-+=##%#%%%#*+**+=-:.:-:  -#
//          *%.    .::::%##%*  --                *% =*%%+- . * =. +      :.-:  .*      .+=      ..:--=#%%%%%%%*=%*-          =%*%#%%##+#-. .....:-   :  *
//          *=  .:--:+.:-+**#  :+                *%.+:#%++ . * -: *     ..-..  *.     =+     :===-:..  .:+%%%%-=.             +%%%#*#%++:        =:  .*..
//          *=--:   :-+   :%#:-#+.               =%=+.+%+#  .* :- *     . -:  -+     +:   .=+-   :-====*%%%%%==.             -##++**=-====+::----.   +:
//          =*: .:---:+==+###=##*=-::.           :%*=.:%**: :=:.= *    . ..-  *:   .+.   ++.  .++: :=++#%%%%=-.            -##*%*-.   :--+-++       =: .+
//           :+-  +.:-+  .%=%%%+: :--::::::.      #%:+ %#:* .:= + *      -.:  #    +.  .*:   ==  -=-:.*%%%#--.           .*%%#=.    ..:.::-+-*:    =-:+%%
//            .###+=+-*===#%%%#+.... :=-: .:---==-=%+--*%.== :+ + *      =:.  +   -.  :*.  .+.  -..:=%%%%%*-.           -%%#-  ..      .- -:+=+-  +##%%#-
//              *%%=:   .-%%%###%#=..:. .----:. =%%%%-=#%+ +::-:-:+     .:-  :-  .-   #.   +  .: :=#%#*+=-.           .*%#: ..         --  :-=++=*%%%*-
//               =##++===*%%%%%%%%%%#=::...  .::-%%%%%:#%% :+:-=.==:    = -  -.  =   -=   +: ::.++%#=*               :%%-..        :--:    :* #:%%%*:
//                -*.-   .##%%%%%%%%%%%%*=:.    =%%%%%%+#%..*.-= +-:   :: -  =   =   +=:-=*--=---+:-%%=            .+%+..    .:::::.  .::--:+:#=%+.
	
	void UpdateBloodParticlesClient()
	{
#ifndef SERVER
		if (!m_Player.GetBleedingManagerRemote()) {
			return;
		}
		
		bool activateBleedingLayer = false;
		
		foreach (string inediaPainLimb: InediaInfectedAI_PainManager.m_InediaPainLimbs) {
			foreach (string bleedingZone: m_InediaPainLimbAllBleedingZonesNames[inediaPainLimb]) {
				if (IsBleedingInBleedingZoneActiveClient(bleedingZone) && IsLimbDeepWounded(inediaPainLimb) && (!IsLimbDeepWoundBandaged(inediaPainLimb) || !IsLimbDeepWoundBandageEffective(inediaPainLimb))) {
					AddBloodParticleForBleedingZoneClient(bleedingZone);
					activateBleedingLayer = true;
				} else {
					RemoveBloodParticleForBleedingZoneClient(bleedingZone);
				}
			}
		}
		
		if (m_Player.IsControlledPlayer()) {
			if (activateBleedingLayer) {
				if (!m_BleedingLayerIsActiveClient) {
					// Activating bleeding layer
					
					Param4<bool,int,vector,bool> par1 = new Param4<bool,int,vector,bool>(true, 1, "0 0 0", false);
					GetGame().GetMission().GetEffectWidgets().AddActiveEffects({EffectWidgetsTypes.BLEEDING_LAYER});
					GetGame().GetMission().GetEffectWidgets().UpdateWidgets(EffectWidgetsTypes.BLEEDING_LAYER, 0, par1);
					
					m_BleedingLayerIsActiveClient = true;
				}
			} else {
				if (m_BleedingLayerIsActiveClient) {
					// Deactivating bleeding layer
					
					Param4<bool,int,vector,bool> par2 = new Param4<bool,int,vector,bool>(false, 1, "0 0 0", false);
					GetGame().GetMission().GetEffectWidgets().UpdateWidgets(EffectWidgetsTypes.BLEEDING_LAYER, 0, par2);
					
					m_BleedingLayerIsActiveClient = false;
				}
			}
		}
#endif
	}
	
	bool IsBleedingInBleedingZoneActiveClient(string bleedingZone)
	{
#ifndef SERVER
		if (!m_Player.GetBleedingManagerRemote()) {
			return false;
		}

		return m_Player.m_InediaInfectedAI_DeepWoundsBleedingsSyncBits & m_Player.GetBleedingManagerRemote().InediaInfectedAI_GetBitFromSelectionName(bleedingZone);
#else
		return false;
#endif
	}
	
	void AddBloodParticleForBleedingZoneClient(string bleedingZone)
	{
#ifndef SERVER
		if (!m_InediaBleedingZonesBloodParticlesClient[bleedingZone]) {
			if (!m_Player.GetBleedingManagerRemote()) {
				return;
			}
			
			int zoneBit = m_Player.GetBleedingManagerRemote().InediaInfectedAI_GetBitFromSelectionName(bleedingZone);
			if (zoneBit > 0) {
				BleedingSourceZone bsz = m_Player.GetBleedingManagerRemote().InediaInfectedAI_GetBleedingSourceMeta(zoneBit);
				if (bsz) {
					int boneIdx = m_Player.GetBoneIndexByName(bsz.GetBoneName());
					if (boneIdx >= 0) {
						m_InediaBleedingZonesBloodParticlesClient[bleedingZone] = EffectParticle.Cast(bsz.GetParticleName().ToType().Spawn());
						if (m_InediaBleedingZonesBloodParticlesClient[bleedingZone]) {
							SEffectManager.PlayInWorld(m_InediaBleedingZonesBloodParticlesClient[bleedingZone], "0 0 0");
							Particle bloodParticle = m_InediaBleedingZonesBloodParticlesClient[bleedingZone].GetParticle();
							bloodParticle.SetOrientation(bsz.GetOrientation());
							vector pos;
							pos += bsz.GetOffset();
							bloodParticle.SetPosition(pos);
							float time = Math.RandomFloat01() * 2;
							bloodParticle.SetParameter(-1, EmitorParam.CURRENT_TIME, time);
							
							m_Player.AddChild(bloodParticle, boneIdx);
						}
					}
				}
			}
		}	
#endif
	}
	
	void RemoveBloodParticleForBleedingZoneClient(string bleedingZone)
	{
#ifndef SERVER
		if (m_InediaBleedingZonesBloodParticlesClient[bleedingZone]) {
			SEffectManager.DestroyEffect(m_InediaBleedingZonesBloodParticlesClient[bleedingZone]);
		}
#endif
	}
	
	void ActivateVisualBleedingForLimbServer(string limb)
	{
		if (!m_Player.GetBleedingManagerServer()) {
			return;
		}
		
		string mainBleedingZone = m_InediaPainLimbMainBleedingZoneName[limb];
		int bitMask = m_Player.GetBleedingManagerServer().InediaInfectedAI_GetBitFromSelectionName(mainBleedingZone);
		m_Player.m_InediaInfectedAI_DeepWoundsBleedingsSyncBits = m_Player.m_InediaInfectedAI_DeepWoundsBleedingsSyncBits | bitMask;
	}
	
	void DeactivateVisualBleedingForLimbServer(string limb)
	{
		if (!m_Player.GetBleedingManagerServer()) {
			return;
		}
		
		foreach (string bleedingZone: m_InediaPainLimbAllBleedingZonesNames[limb]) {
			int bitMask = m_Player.GetBleedingManagerServer().InediaInfectedAI_GetBitFromSelectionName(bleedingZone);
			m_Player.m_InediaInfectedAI_DeepWoundsBleedingsSyncBits = m_Player.m_InediaInfectedAI_DeepWoundsBleedingsSyncBits & ~bitMask;
		}
	}
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Modding classes that affect pain mechanic,
// which I chose not to move to separate files for convenience,
// as it would be difficult to track all these changes later.

// Disabling the vanilla broken leg badge if the Inedia fracture system is active.
modded class FracturedLegNotfr: NotifierBase
{
	override void DisplayBadge()
	{
		if (m_Player && m_Player.InediaInfectedAI_GetPainManager().LimbsBreakSystemIsActive()) {
			return;
		}
		
		super.DisplayBadge();
	}
}

// Disabling the vanilla injured leg badge if the Inedia pain system is active.
modded class InjuredLegNotfr: NotifierBase
{
	override void DisplayBadge()
	{
		if (m_Player && m_Player.InediaInfectedAI_GetPainManager().PainSystemIsActive()) {
			return;
		}
		
		super.DisplayBadge();
	}
};

// The vanilla modifier for broken arms is disabled when using the Inedia fracture system, just in case.
modded class BrokenArmsMdfr: ModifierBase
{
	override bool ActivateCondition(PlayerBase player)
	{
		if (player && player.InediaInfectedAI_GetPainManager().LimbsBreakSystemIsActive()) {
			return false;
		}

		return super.ActivateCondition(player);
	}
};

// The shock refill speed for a player with active stabilization of broken legs should be normal.
modded class ShockMdfr: ModifierBase
{
	override float GetRefillSpeed(PlayerBase player)
	{
		if (player && player.GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS && !player.m_IsDrowning && !player.IsUnconscious()) {
			if (player.InediaInfectedAI_GetPainManager().IsLimbStabilized("legs")) {
				return CfgGameplayHandler.GetShockRefillSpeedConscious();
			}
		}
		
		return super.GetRefillSpeed(player);
	}
};

// Modifier for reducing vanilla blood loss, as well as an overall reduction of vanilla blood loss by X times
// if the deep wounds system is active (in this case, vanilla cuts are treated as scratches).
modded class BleedingSource
{
	override void OnUpdateServer(float deltatime, float blood_scale, bool no_blood_loss )
	{
		super.OnUpdateServer(deltatime, blood_scale, no_blood_loss);
		
		if (!no_blood_loss && m_Player && m_Player.m_InediaInfectedAI_ConfigPlayers) {
			float bloodClottingMultiplier = 1;
			
			if (m_Player.GetModifiersManager() && m_Player.GetModifiersManager().IsModifierActive(InediaInfectedAI_eModifiers.MDF_BLOOD_CLOTTING) && m_Player.InediaInfectedAI_GetPainManager().m_BloodClottingItemClottingMultiplier > 1) {
				bloodClottingMultiplier *= m_Player.InediaInfectedAI_GetPainManager().m_BloodClottingItemClottingMultiplier;
			}
			
			float deepWoundsVanillaCutsMultiplier = Math.Clamp(m_Player.m_InediaInfectedAI_ConfigPlayers.LimbsDeepWoundVanillaCutBleedRateMultiplier, 0.001, 1);
			if (m_Player.InediaInfectedAI_GetPainManager().LimbsDeepWoundSystemIsActive() && !m_Player.InediaInfectedAI_IsNPC()) {
				bloodClottingMultiplier /= deepWoundsVanillaCutsMultiplier;
			}
			
			if (bloodClottingMultiplier > 1) {
				float flow = m_FlowModifier;
				switch (m_Type) {
					case eBleedingSourceType.NORMAL:
					{
						//do nothing
						break;
					}
					case eBleedingSourceType.CONTAMINATED:
					{
						flow *= PlayerConstants.BLEEDING_SOURCE_BURN_MODIFIER;
					}
				}
				
				float bloodLossRate = -PlayerConstants.BLEEDING_SOURCE_BLOODLOSS_PER_SEC * blood_scale * deltatime * flow;
				
				float recoveredBlood = bloodLossRate * (1.0 - 1.0 / bloodClottingMultiplier);

				if (recoveredBlood > 0) {
					m_Player.AddHealth("GlobalHealth", "Blood", recoveredBlood);
				}				
			}
		}
	}
}

// Various restrictions on attack methods that the player receives due to pain limbs,
// as well as shock damage the player takes when attacking with painful arms or bare hands.
modded class DayZPlayerMeleeFightLogic_LightHeavy
{
	PlayerBase m_InediaInfectedAI_DZPlayerPb;

	override void Init(DayZPlayerImplement player)
	{
		super.Init(player);

		m_InediaInfectedAI_DZPlayerPb = PlayerBase.Cast(player);
	}

	/*
	override protected EMeleeHitType GetAttackTypeFromInputs(HumanInputController pInputs)
	{
		if (m_InediaInfectedAI_DZPlayerPb && m_InediaInfectedAI_DZPlayerPb.InediaInfectedAI_GetPainManager().GetPainWithBuffsLevelForLimb("arms") >= InediaInfectedAI_LimbPainLevels.MEDIUM) {
			return EMeleeHitType.LIGHT;
		}

		return super.GetAttackTypeFromInputs(pInputs);
	}
	*/

	override protected bool HandleComboHit(int pCurrentCommandID, HumanInputController pInputs, InventoryItem itemInHands, HumanMovementState pMovementState, out bool pContinueAttack)
	{
		if (m_InediaInfectedAI_DZPlayerPb && m_InediaInfectedAI_DZPlayerPb.InediaInfectedAI_GetPainManager().GetPainWithBuffsLevelForLimb("arms") >= InediaInfectedAI_LimbPainLevels.LOW) {
			return false;
		}

		return super.HandleComboHit(pCurrentCommandID, pInputs, itemInHands, pMovementState, pContinueAttack);
	}
	
	override protected void DamageHands(DayZPlayer DZPlayer, ClothingBase gloves, InventoryItem itemInHands)
	{
		if (DZPlayer) {
			PlayerBase player = PlayerBase.Cast(DZPlayer);
			if (player && player.m_InediaInfectedAI_ConfigPlayers && player.InediaInfectedAI_GetPainManager().PainSystemIsActive()) {
				EntityAI target = m_MeleeCombat.GetTargetEntity();
				if (target) {
					// This logic is executed only if the character hits the target.
					if (player.m_InediaInfectedAI_ConfigPlayers.PainArmsWhenHitWithoutGloves && !itemInHands && (!gloves || gloves.GetHealthLevel() == GameConstants.STATE_RUINED) && m_HitType != EMeleeHitType.KICK) {
						player.InediaInfectedAI_GetPainManager().AddPainPercentForLimb("arms", 5, true);
					}
				}
				
				if (player.m_InediaInfectedAI_ConfigPlayers.ShockIfAttackingWithPainedArms) {
					float painArmsShockMultiplier = player.InediaInfectedAI_GetPainManager().GetActionsShockMultiplierForLimb("arms");
					
					if (player.InediaInfectedAI_GetPainManager().GetPainWithBuffsLevelForLimb("arms") >= InediaInfectedAI_LimbPainLevels.BADLY) {
						player.DecreaseHealth("GlobalHealth", "Shock", 6 * painArmsShockMultiplier);
					} else if (player.InediaInfectedAI_GetPainManager().GetPainWithBuffsLevelForLimb("arms") >= InediaInfectedAI_LimbPainLevels.MEDIUM) {
						player.DecreaseHealth("GlobalHealth", "Shock", 3 * painArmsShockMultiplier);
					}
				}
			}
		}

		super.DamageHands(DZPlayer, gloves, itemInHands);
	}

	/*
	override protected bool HandleSprintAttack(int pCurrentCommandID, HumanInputController pInputs, InventoryItem itemInHands, HumanMovementState pMovementState, out bool pContinueAttack)
	{
		return super.HandleSprintAttack(pCurrentCommandID, pInputs, itemInHands, pMovementState, pContinueAttack);
	}
	*/
}


// When the character experiences torso pain, they should not frequently make stamina sounds, as high frequency is very irritating.
modded class StaminaSoundEventBase extends PlayerSoundEventBase
{
	float m_InediaInfectedAI_LastSoundTime = -1000000;
	float m_InediaInfectedAI_SoundNotMoreFrequentlyThanSeconds;
	override bool CanPlay(PlayerBase player)
	{
		if (player) {
			if (player.InediaInfectedAI_GetPainManager().GetPainWithBuffsLevelForLimb("torso") >= InediaInfectedAI_LimbPainLevels.LOW) {
				if (m_InediaInfectedAI_SoundNotMoreFrequentlyThanSeconds > 0 && (GetGame().GetTickTime() - m_InediaInfectedAI_LastSoundTime) <= m_InediaInfectedAI_SoundNotMoreFrequentlyThanSeconds) {
					return false;
				} else {
					m_InediaInfectedAI_LastSoundTime = GetGame().GetTickTime();
					m_InediaInfectedAI_SoundNotMoreFrequentlyThanSeconds = Math.RandomFloatInclusive(10, 30);
				}
			}
		}
		
		return super.CanPlay(player);
	}
}

// Disabling backstab if the player has a broken arm.
modded class DayZPlayerImplementMeleeCombat
{
	override protected int TrySelectFinisherType(InventoryItem weapon, EntityAI target)
	{
		if (target && target.IsInherited(ZombieBase)) {
			ZombieBase zmbTarget = ZombieBase.Cast(target);
			
			// This functionality disables the ability to perform a backstab, for example when the character’s hands are damaged.
			// However, it will not work if zombie damage handling is enabled, because in that case a backstab is possible but does not kill the infected.
			if (zmbTarget && !zmbTarget.InediaInfectedAI_GetNetSyncBoolVariable("DamageToZombieHandlerIsActive")) {
				PlayerBase playerPb = PlayerBase.Cast(m_DZPlayer);
				if (playerPb && playerPb.InediaInfectedAI_CannotKillWithBackstab()) {
					return -1;
				}
			}
		}
		
		return super.TrySelectFinisherType(weapon, target);
	}
}

// When a vanilla fracture is deactivated, but a non-vanilla splint was used, rags are spawned.
modded class BrokenLegsMdfr: ModifierBase
{
	override void OnDeactivate(PlayerBase player)
	{
		if (player && player.InediaInfectedAI_GetPainManager().IsLimbStabilized("legs") && !player.IsWearingSplint()) {
			ItemBase rag = ItemBase.Cast(m_Player.SpawnEntityOnGroundPos("Rag", player.GetPosition() + 0.6 * player.GetDirection()));
			if (rag) {
				rag.SetQuantity(1);
				rag.SetHealth("", "", rag.GetMaxHealth("", "") * GameConstants.DAMAGE_RUINED_VALUE);
			}
		}
				
		super.OnDeactivate(player);
	}
};

