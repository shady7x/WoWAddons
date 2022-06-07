/*
Chain Heal is a 'smart' heal and always jumps to the eligible target with the largest health deficit in range. 
The bounce radius is 12.5 yards. Since the targets are linked into a chain, 
targets must be within range of each other, in order of "bounce". 
The chain cannot jump back to heal the same target more than once.
*/

unsigned long long best_chain_target()
{
	unsigned int max_cr = 0;
	unsigned long long btarget = 0;

	unsigned int total = raid.size();

	for (unsigned int i = 0; i < total; ++i)
	{
		Unit* raid_i = new Unit(raid[i].BaseAddress);
		if (raid_i && raid[i].CurrentHealth > 1 && f.xcastrange(Spell(CHAIN), raid[i].guid))
		{
			std::vector < unsigned int > cratings;
			cratings.clear();
			cratings.push_back(raid[i].MissingHealth);
			for (unsigned int j = 0; j < total; ++j)
			{
				if (i != j)
				{
					Unit* raid_j = new Unit(raid[j].BaseAddress);
					if (raid_j && raid[j].CurrentHealth > 1)
					{
						if (raid_i->DistanceTo(raid_j) < 12.5f)
						{
							cratings.push_back(raid[j].MissingHealth);
						}
					}
					delete raid_j;
				}
			}
			std::sort(cratings.begin(), cratings.end());
			while (cratings.size() > 4)
				cratings.pop_back();

			raid[i].crating = 0;
			for (unsigned int j = 0; j < cratings.size(); ++j)
				raid[i].crating += cratings[j];

			if (raid[i].crating > max_cr)
			{
				max_cr = raid[i].crating;
				btarget = raid[i].guid;
				//chat("new target: " + std::to_string(max_cr) + " | " + raid[i].Name);
			}
		}
		delete raid_i;
	}
	//chat("best target: " + std::to_string(max_cr));
	return btarget;
}