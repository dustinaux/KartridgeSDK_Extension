#ifndef STATIC_LINK
#define IMPLEMENT_API
#endif

#if defined(HX_WINDOWS) || defined(HX_MACOS) || defined(HX_LINUX)
#define NEKO_COMPATIBLE
#endif


#include <hx/CFFI.h>
#include <string>
#include "Utils.h"
#include "../lib/kartridge-sdk/kongregate.h"

using namespace kartridge;

AutoGCRoot* kartridgeEventHandle = 0;

static void kartridge_set_event_handle(value onEvent)
{
  kartridgeEventHandle = new AutoGCRoot(onEvent);
}
DEFINE_PRIM(kartridge_set_event_handle, 1);

extern "C" void sendKartridgeEvent(const char* type, const char* data)
{
    value o = alloc_empty_object();
    alloc_field(o,val_id("type"),alloc_string(type));
    alloc_field(o,val_id("data"),alloc_string(data));
    val_call1(kartridgeEventHandle->get(), o);
}

void event_handler(const char *event, const char* const payload) {
	if (event == KONGREGATE_EVENT_READY) {
		sendKartridgeEvent(event,"Kongregate API is READY");
	} else if (event == KONGREGATE_EVENT_CONNECTED) {
		sendKartridgeEvent(event,"Kongregate API is CONNECTED");
	} else if (event == KONGREGATE_EVENT_USER) {
		std::string buf("Kongregate API user info change, username=");
		buf.append(KongregateServices_GetUsername());
		sendKartridgeEvent(event,buf.c_str());
	}
}

static value kartridge_initialize(value settings_json)
{
	KongregateAPI_SetEventListener(event_handler);
	return alloc_bool(KongregateAPI_Initialize(val_string(settings_json)));
}
DEFINE_PRIM (kartridge_initialize, 1);

static void kartridge_shutdown()
{
	KongregateAPI_Shutdown();
}
DEFINE_PRIM (kartridge_shutdown, 0);

static void kartridge_update()
{
	KongregateAPI_Update();
}
DEFINE_PRIM (kartridge_update, 0);

static value kartridge_isConnected()
{
	return alloc_bool(KongregateAPI_IsConnected());
}
DEFINE_PRIM (kartridge_isConnected, 0);

static value kartridge_isReady()
{
	return alloc_bool(KongregateAPI_IsReady());
}
DEFINE_PRIM (kartridge_isReady, 0);

static value kartridge_restartWithKartridgeIfNeeded(value game_id)
{
	return alloc_bool(KongregateAPI_RestartWithKartridgeIfNeeded(val_int(game_id)));
}
DEFINE_PRIM (kartridge_restartWithKartridgeIfNeeded, 1);

static value kartridge_getUsername()
{
	return alloc_string(KongregateServices_GetUsername());
}
DEFINE_PRIM (kartridge_getUsername, 0);

static value kartridge_getUserId()
{
	return alloc_int(KongregateServices_GetUserId());
}
DEFINE_PRIM (kartridge_getUserId, 0);

static value kartridge_getGameAuthToken()
{
	return alloc_string(KongregateServices_GetGameAuthToken());
}
DEFINE_PRIM (kartridge_getGameAuthToken, 0);

static void kartridge_submit(value name, value _value)
{
	KongregateStats_Submit(val_string(name), val_int(_value));
}
DEFINE_PRIM (kartridge_submit, 2);



extern "C" void kartridge_main () {
	
	val_int(0); // Fix Neko init
	
}
DEFINE_ENTRY_POINT (kartridge_main);



extern "C" int kartridge_register_prims () { return 0; }