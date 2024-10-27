//////////////////////////////////////////////////////
// Copyright (C) Betide Studio. 2023. All rights reserved.
//////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FPlayFabIntegrationKitModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};
