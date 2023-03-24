function()
    local rbits = 0
    local gbits = 0
    local bbits = 0
    
    local energy = UnitPower("player", 3)
    local cp = UnitPower("player" , 4)  -- SPELL_POWER_COMBO_POINTS
    
    local gTime = GetTime()
    
    -- garrote
    local garrote_cd_start, garrote_cd_duration = GetSpellCooldown(703)
    local garrote_cd = 0
    if garrote_cd_start ~= 0 or garrote_cd_duration ~= 0 then
        garrote_cd = garrote_cd_start + garrote_cd_duration - gTime
    end
    local _, _, _, _, garrote_duration, garrote_expiration = AuraUtil.FindAuraByName("Гаррота", "target", "PLAYER|HARMFUL")
    local garrote_timeleft = 0
    if garrote_duration ~= nil then
        garrote_timeleft = garrote_expiration - gTime
    end
    
    local garrote_u = garrote_cd < 0.3 and garrote_timeleft < 5.8 and IsUsableSpell(703) 
    
    -- bonespike
    local bonespike_cd_start, bonespike_cd_duration = GetSpellCooldown(385424)
    local bonespike_cd = 0
    if bonespike_cd_start ~= 0 or bonespike_cd_duration ~= 0 then
        bonespike_cd = bonespike_cd_start + bonespike_cd_duration - gTime
    end
    
    local bonespike_u = bonespike_cd < 0.3 and AuraUtil.FindAuraByName("Зазубренный костяной шип", "target", "PLAYER|HARMFUL") == nil and IsUsableSpell(385424) -- TODO: test if (not on target or has 3 charges), maybe better than muti
    
    -- rapture
    local rapture_cd_start, rapture_cd_duration = GetSpellCooldown(1943)
    local rapture_cd = 0
    if rapture_cd_start ~= 0 or rapture_cd_duration ~= 0 then
        rapture_cd = rapture_cd_start + rapture_cd_duration - gTime
    end
    local _, _, _, _, rapture_duration, rapture_expiration = AuraUtil.FindAuraByName("Рваная рана", "target", "PLAYER|HARMFUL")
    local rapture_timeleft = 0
    if rapture_duration ~= nil then
        rapture_timeleft = rapture_expiration - gTime
    end
    local rapture_empty = rapture_timeleft < 5.8
    
    local rapture_u = rapture_cd < 0.3 and rapture_empty and IsUsableSpell(1943)
    
    -- mutilate
    local mutilate_cd_start, mutilate_cd_duration = GetSpellCooldown(1329)
    local mutilate_cd = 0
    if mutilate_cd_start ~= 0 or mutilate_cd_duration ~= 0 then
        mutilate_cd = mutilate_cd_start + mutilate_cd_duration - gTime
    end
    
    local mutilate_u = mutilate_cd < 0.3 and IsUsableSpell(1329) 
    
    -- envenom
    local envenom_cd_start, envenom_cd_duration = GetSpellCooldown(32645)
    local envenom_cd = 0
    if envenom_cd_start ~= 0 or envenom_cd_duration ~= 0 then
        envenom_cd = envenom_cd_start + envenom_cd_duration - gTime
    end
    
    local envenom_u = envenom_cd < 0.3 and IsUsableSpell(32645)
    
    -- slice and dice
    local snd_cd_start, snd_cd_duration = GetSpellCooldown(315496)
    local snd_cd = 0
    if snd_cd_start ~= 0 or snd_cd_duration ~= 0 then
        snd_cd = snd_cd_start + snd_cd_duration - gTime
    end
    local snd_empty = C_UnitAuras.GetPlayerAuraBySpellID(315496) == nil
    
    local snd_u = snd_cd < 0.3 and snd_empty and IsUsableSpell(315496)
    
    -- echoing reprimand
    local echo_cd_start, echo_cd_duration = GetSpellCooldown(385616)
    local echo_cd = 0
    if echo_cd_start ~= 0 or echo_cd_duration ~= 0 then
        echo_cd = echo_cd_start + echo_cd_duration - gTime
    end
    
    local echo_u = echo_cd < 0.3 and IsUsableSpell(385616)
    
    -- reprimand charged
    local echo2 = C_UnitAuras.GetPlayerAuraBySpellID(323558) ~= nil
    local echo3 = C_UnitAuras.GetPlayerAuraBySpellID(323559) ~= nil
    local echo4 = C_UnitAuras.GetPlayerAuraBySpellID(323560) ~= nil
    local charged2 = cp == 2 and echo2
    local charged3 = cp == 3 and echo3
    local charged4 = cp == 4 and echo4
    local echo_charged = charged2 or charged3 or charged4
    
    -- death mark
    local death = AuraUtil.FindAuraByName("Знак смерти", "target", "PLAYER|HARMFUL") ~= nil
    
    -- shiv
    local shiv_cd_start, shiv_cd_duration = GetSpellCooldown(5938)
    local shiv_cd = 0
    if shiv_cd_start ~= 0 or shiv_cd_duration ~= 0 then
        shiv_cd = shiv_cd_start + shiv_cd_duration - gTime
    end
    
    local shiv_u = shiv_cd < 0.3 and AuraUtil.FindAuraByName("Отравляющий укол", "target", "PLAYER|HARMFUL") == nil and IsUsableSpell(5938)
    local shiv_reprimand_1_off = shiv_u and echo_charged == false and ((echo2 and cp == 1) or (echo3 and cp == 2) or (echo4 and cp == 3))
    local shiv_death = shiv_u and (death or UnitHealth("target") < UnitHealthMax("target") * 0.6)
    
    -- kidney
    local kidney_cd_start, kidney_cd_duration = GetSpellCooldown(408)
    local kidney_cd = 0
    if kidney_cd_start ~= 0 or kidney_cd_duration ~= 0 then
        kidney_cd = kidney_cd_start + kidney_cd_duration - gTime
    end
    
    local kidney_u = kidney_cd < 2.5
    
    -- kicks
    local tKick = false
    local kick1 = false
    local kick2 = false
    local kick3 = false
    if GetSpellCooldown(1766) == 0 and IsUsableSpell(1766) then
        -- target kick
        local bChanneling, _, _, _, _, _, iChanneling = UnitChannelInfo("target")
        local bCasting, _, _, _, eCasting, _, _, iCasting = UnitCastingInfo("target")
        tKick = (iChanneling == false or (iCasting == false and eCasting - gTime * 1000 < 240))
        -- arena1 kick
        local bChanneling1, _, _, _, _, _, iChanneling1 = UnitChannelInfo("arena1")
        local bCasting1, _, _, _, eCasting1, _, _, iCasting1 = UnitCastingInfo("arena1")
        kick1 = (iChanneling1 == false or (iCasting1 == false and eCasting1 - gTime * 1000 < 240))
        -- arena2 kick
        local bChanneling2, _, _, _, _, _, iChanneling2 = UnitChannelInfo("arena2")
        local bCasting2, _, _, _, eCasting2, _, _, iCasting2 = UnitCastingInfo("arena2")
        kick2 = (iChanneling2 == false or (iCasting2 == false and eCasting2 - gTime * 1000 < 240))
        -- arena3 kick
        local bChanneling3, _, _, _, _, _, iChanneling3 = UnitChannelInfo("arena3")
        local bCasting3, _, _, _, eCasting3, _, _, iCasting3 = UnitCastingInfo("arena3")
        kick3 = (iChanneling3 == false or (iCasting3 == false and eCasting3 - gTime * 1000 < 240))
    end
    
    -- in melee range
    local melee = (IsSpellInRange("Отравляющий укол", "target") == 1)
    local autoatk = melee and IsCurrentSpell(6603)
    -- tea
    local tea_death = death and C_UnitAuras.GetPlayerAuraBySpellID(381623) == nil
    local tea_u = autoatk and (energy < 55 or tea_death) and GetSpellCharges(381623) > 0 and IsUsableSpell(381623)
    -- mark for death
    local mark_u = GetSpellCooldown(137619) == 0 and IsUsableSpell(137619)
    local mark_filler_u = autoatk and cp == 0 and snd_empty == false and mark_u
    local mark_kidney_u = melee and kidney_u and mark_u
    
    if garrote_u then
        rbits = rbits + 1
    end
    if bonespike_u then
        rbits = rbits + 2
    end
    if rapture_u then
        rbits = rbits + 4
    end
    if mutilate_u then
        rbits = rbits + 8
    end
    if envenom_u then
        rbits = rbits + 16
    end
    if snd_u then
        rbits = rbits + 32
    end
    if rapture_empty then
        rbits = rbits + 64
    end
    if melee then
        rbits = rbits + 128
    end
    
    if echo_u then
        gbits = gbits + 1
    end
    if echo_charged then
        gbits = gbits + 2
    end
    if tKick then
        gbits = gbits + 4
    end
    if kick1 then
        gbits = gbits + 8
    end
    if kick2 then
        gbits = gbits + 16
    end
    if kick3 then
        gbits = gbits + 32
    end
    if shiv_reprimand_1_off or shiv_death then
        gbits = gbits + 64
    end
    if tea_u then
        gbits = gbits + 128
    end
    
    bbits = cp -- can take up first 3 bits max, other 5 bits from 2^3(8) are free
    if kidney_u then
        bbits = bbits + 8
    end
    if mark_filler_u then
        bbits = bbits + 16
    end
    if mark_kidney_u then
        bbits = bbits + 32
    end
    
    return rbits / 255, gbits / 255, bbits / 255, 1
end

