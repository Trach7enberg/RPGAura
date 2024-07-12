// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HighLightInterface.generated.h"

// 高亮任何Actor的接口
UINTERFACE(MinimalAPI)
class UHighLightInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPGAURA_API IHighLightInterface
{
	GENERATED_BODY()

public:
	virtual void HighLightActor() = 0;
	virtual void UnHighLightActor() = 0;
};
