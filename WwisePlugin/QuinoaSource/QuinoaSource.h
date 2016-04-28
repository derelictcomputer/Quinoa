#pragma once
#include <AK/SoundEngine/Common/IAkPlugin.h>

class QuinoaSource : AK::IAkSourcePlugin
{
public:
	QuinoaSource();
	~QuinoaSource();

	AKRESULT Term(AK::IAkPluginMemAlloc* in_pAllocator) override;
	AKRESULT Reset() override;
	AKRESULT GetPluginInfo(AkPluginInfo& out_rPluginInfo) override;
	AKRESULT Init(AK::IAkPluginMemAlloc* in_pAllocator, AK::IAkSourcePluginContext* in_pSourcePluginContext, AK::IAkPluginParam* in_pParams, AkAudioFormat& io_rFormat) override;
	AkReal32 GetDuration() const override;
	void Execute(AkAudioBuffer* io_pBuffer) override;
};

