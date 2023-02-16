function()
    -- local energy = UnitPower("player" , 3) --  SPELL_POWER_ENERGY = 3
    local cp = UnitPower("player" , 4)  -- SPELL_POWER_COMBO_POINTS = 4
    local cp_mx = UnitPowerMax("player", 4)
    
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
    
    -- dispatch usable and not on cooldown
    local dispatch_s, dispatch_d = GetSpellCooldown(2098)
    local dispatch_c = 0
    if dispatch_s ~= 0 or dispatch_d ~= 0 then
        dispatch_c = dispatch_s + dispatch_d - GetTime()
    end
    local dispatch_u = IsUsableSpell(2098) and dispatch_c < 0.33
    
    -- normal invis active
    local invis_a = (AuraUtil.FindAuraByName("Незаметность", "player") ~= nil)
    -- vanish active
    local vanish_a = (AuraUtil.FindAuraByName("Исчезновение", "player") ~= nil)
    -- subterfuge active
    local subterfuge_a = (AuraUtil.FindAuraByName("Увертка", "player") ~= nil) -- might wanna check remaining time as in pistol proc
    -- shadow dance active
    local shadowdance_a = (AuraUtil.FindAuraByName("Танец теней", "player") ~= nil) -- same
    -- in melee range
    local melee = (IsSpellInRange("Отравляющий укол", "target") == 1)
    -- audacity proc
    local audacity_a = (AuraUtil.FindAuraByName("Дерзость", "player") ~= nil) -- same
    
    
    if dice_u == true then
        
        -- prevent buffs from shuffling when not needed
        local cnt = 0
        
        local truebearing_a = (AuraUtil.FindAuraByName("Истинный азимут", "player") ~= nil) -- True Bearing 
        local skullbones_a = (AuraUtil.FindAuraByName("Череп с костями", "player") ~= nil) -- Skull and Crossbones
        local broadside_a = (AuraUtil.FindAuraByName("Бортовой залп", "player") ~= nil) -- Broadside
        
        if truebearing_a == true then
            cnt = cnt + 1
            dice_u = false
        end
        if skullbones_a == true then
            cnt = cnt + 1
            dice_u = false
        end
        if broadside_a == true then
            cnt = cnt + 1
            dice_u = false
        end
        
        if dice_u == true then -- still true after ^ checks
            
            local grandmelee_a = (AuraUtil.FindAuraByName("Великая битва", "player") ~= nil) -- Grand Melee 
            local treasure_a = (AuraUtil.FindAuraByName("Зарытое сокровище", "player") ~= nil) -- Buried Treasure
            local precision_a = (AuraUtil.FindAuraByName("Беспощадная точность", "player") ~= nil) -- Ruthless Precision
            
            if grandmelee_a == true then
                cnt = cnt + 1
            end
            if treasure_a == true then
                cnt = cnt + 1
            end
            if precision == true then
                cnt = cnt + 1
            end
            
            if cnt > 1 then
                dice_u = false
            end
            
        end
        
    end
    
    -- set 8 bits
    local bits = 0
    
    if dispatch_u == true then
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
    
    -- second set of bits
    local bits2 = 0
    
    if melee == true then
        bits2 = bits2 + 2 ^ 0
    end
    
    if invis_a == true then
        bits2 = bits2 + 2 ^ 1
    end
    
    if vanish_a == true then
        bits2 = bits2 + 2 ^ 2
    end
    
    if subterfuge_a == true then
        bits2 = bits2 + 2 ^ 3
    end
    
    if shadowdance_a == true then
        bits2 = bits2 + 2 ^ 4
    end
    
    if audacity_a == true then
        bits2 = bits2 + 2 ^ 5
    end
    
    
    cp = cp + (cp_mx - 5) * 10
    
    local R = cp / 255
    local G = bits / 255
    local B = bits2 / 255 -- or * 0.00392156862745098
    
    -- 1 pixel carries info on 16 spells, current and max combopoints 
    
    return R, G, B, 1
end