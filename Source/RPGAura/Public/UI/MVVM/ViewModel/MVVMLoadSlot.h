// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVMLoadSlot.generated.h"

/**
 * 用于加载屏幕widget里加载槽位的MVVM架构的ViewModel
 */
UCLASS()
class RPGAURA_API UMVVMLoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	FString GetTestStr() const { return TestStr; }
	void SetTestStr(const FString& NewTestStr)
	{
		UE_MVVM_SET_PROPERTY_VALUE(TestStr,NewTestStr);
		this->TestStr = NewTestStr;
	}
private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(Allowprivateaccess="true"))
	FString TestStr;
};
