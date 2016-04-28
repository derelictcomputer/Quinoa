#include "QuinoaPlugin.h"

/////////////// DLL exports ///////////////////

// Plugin creation
AK::Wwise::IPluginBase* __stdcall AkCreatePlugin(unsigned short in_usCompanyID, unsigned short in_usPluginID)
{
	if (in_usCompanyID == ToneGenPlugin::CompanyID && (in_usPluginID == ToneGenPlugin::PluginID || in_usPluginID == ToneGenPlugin::MotionPluginID))
	{
		return new ToneGenPlugin(in_usPluginID);
	}

	return NULL;
}

// Sound Engine callbacks
bool __stdcall AkGetSoundEngineCallbacks(unsigned short in_usCompanyID, unsigned short in_usPluginID, AkCreatePluginCallback & out_funcEffect, AkCreateParamCallback & out_funcParam)
{
	if (in_usCompanyID == ToneGenPlugin::CompanyID && (in_usPluginID == ToneGenPlugin::PluginID || in_usPluginID == ToneGenPlugin::MotionPluginID))
	{
		out_funcEffect = CreateToneSource;
		out_funcParam = CreateToneSourceParams;

		return true;
	}

	return false;
}


QuinoaPlugin::QuinoaPlugin()
{
}


QuinoaPlugin::~QuinoaPlugin()
{
}
