function()
    local energy = UnitPower("player" , 3) --  SPELL_POWER_ENERGY = 3
    local cp = UnitPower("player" , 4)  -- SPELL_POWER_COMBO_POINTS = 4
    
    local melee = (IsSpellInRange("Отравляющий укол", "target") == 1)
    
    -- sinister strike usable and not on cd
    local sinister_s, sinister_d = GetSpellCooldown(193315)
    local sinister_c = 0
    if sinister_s ~= 0 or sinister_d ~= 0 then
        sinister_c = sinister_s + sinister_d - GetTime()
    end
    local sinister_u = IsUsableSpell(193315) and sinister_c < 0.33
    
    -- ambush usable and not on cd (checking gcd here)
    local ambush_u = IsUsableSpell(8676) and sinister_c < 0.33
    
    -- roll the dice usable and not on cd
    local dice_s, dice_d = GetSpellCooldown(315508)
    local dice_c = 0
    if dice_s ~= 0 or dice_d ~= 0 then
        dice_c = dice_s + dice_d - GetTime()
    end
    local dice_u = IsUsableSpell(315508) and dice_c < 0.33 
    
    -- between the eyes usable and not on cd
    local eyes_s, eyes_d = GetSpellCooldown(315341)
    local eyes_c = 0
    if eyes_s ~= 0 or eyes_d ~= 0 then
        eyes_c = eyes_s + eyes_d - GetTime()
    end
    local eyes_u = IsUsableSpell(315341) and eyes_c < 0.33
    
    -- slice and dice usable, not on cd and uptime == 0 or < 6
    local snd_s, snd_d = GetSpellCooldown(315496)
    local snd_c = 0
    if snd_s ~= 0 or snd_d ~= 0 then
        snd_c = snd_s + snd_d - GetTime()
    end
    local snd_timeleft = 0
    local _, _, _, _, snd_duration, snd_expiration = AuraUtil.FindAuraByName("Мясорубка", "player")
    if snd_duration ~= nil then
        snd_timeleft = snd_expiration - GetTime()
    end
    local snd_u = IsUsableSpell(315496) and snd_c < 0.33 and snd_timeleft < 6 
    
    -- pistol shot usable, not on cd and proc up
    local pistol_s, pistol_d = GetSpellCooldown(185763)
    local pistol_c = 0
    if pistol_s ~= 0 or pistol_d ~= 0 then
        pistol_c = pistol_s + pistol_d - GetTime()
    end
    local pistol_timeleft = 0
    local _, _, _, _, pistol_duration, pistol_expiration = AuraUtil.FindAuraByName("Замечательная возможность", "player")
    if pistol_duration ~= nil then
        pistol_timeleft = pistol_expiration - GetTime()
    end
    local pistol_u = IsUsableSpell(185763) and pistol_c < 0.33 and pistol_timeleft > 0.5
    
    -- blade flurry usable and not on cooldown
    local blade_s, blade_d = GetSpellCooldown(13877)
    local blade_c = 0
    if blade_s ~= 0 or blade_d ~= 0 then
        blade_c = blade_s + blade_d - GetTime()
    end
    local blade_u = IsUsableSpell(13877) and blade_c < 0.33
    
    -- set 8 bits
    local bits = 0
    
    if melee == true then
        bits = bits + 2 ^ 0
    end
    
    if sinister_u == true then
        bits = bits + 2 ^ 1
    end
    
    if ambush_u == true then
        bits = bits + 2 ^ 2
    end
    
    if dice_u == true then
        bits = bits + 2 ^ 3
    end
    
    if eyes_u == true then
        bits = bits + 2 ^ 4
    end
    
    if snd_u == true then
        bits = bits + 2 ^ 5
    end
    
    if pistol_u == true then
        bits = bits + 2 ^ 6
    end
    
    if blade_u == true then
        bits = bits + 2 ^ 7
    end
    
    local R = cp / 255
    local G = energy / 255
    local B = bits / 255
    
    -- print(eyes_u)
    
    -- 1 pixel carries info on 8 spells, energy and active combopoints
    
    return R, G, B, 1
end
