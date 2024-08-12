// Copyright GGBAO 


#include "GAS/Data/CharacterClassInfo.h"


FCharacterClassDefaultInfo UCharacterClassInfo::FindClassDefaultInfo(const ECharacterClass& CharacterClass)
{
	if(CharacterClassInformation.Contains(CharacterClass))
	{
		return CharacterClassInformation[CharacterClass];
	}
	return FCharacterClassDefaultInfo();
}
