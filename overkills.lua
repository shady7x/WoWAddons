local function OnEvent(self, event, ...)
	local _, subevent, _, sourceName, _, _, destName, _, prefixParam1, prefixParam2, _, suffixParam1, suffixParam2 = ...
 
	if (subevent == "SPELL_DAMAGE" or subevent == "SPELL_PERIODIC_DAMAGE" or subevent == "RANGE_DAMAGE") and sourceName == "Sxtn" and suffixParam2~=nil and suffixParam2 > 0 and prefixParam1 == 67485 then
		print(Killed ["..destName.."] with "..suffixParam1.." "..GetSpellLink(prefixParam1))
	end
end
 
local f = CreateFrame("Frame")
f:RegisterEvent("COMBAT_LOG_EVENT_UNFILTERED")
f:SetScript("OnEvent", OnEvent)