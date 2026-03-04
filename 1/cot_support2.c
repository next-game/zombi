#ifdef JM_CommunityOnlineTools

modded class JMPlayerModule: JMRenderableModuleBase
{
    override private void Exec_Heal(array<string> guids, PlayerIdentity ident, JMPlayerInstance instance = null)
    {
        array<JMPlayerInstance> players = GetPermissionsManager().GetPlayers(guids);

        for (int i = 0; i < players.Count(); i++) {
            PlayerBase player = PlayerBase.Cast(players[i].PlayerObject);
            if (!player) {
                continue;
            }

            player.InediaInfectedAI_RemoveInediaDebuffs();
        }

        super.Exec_Heal(guids, ident, instance);
    }
}

#endif
