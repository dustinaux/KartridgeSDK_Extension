package;


import lime.system.CFFI;


class Kartridge
{
	private static function notifyListeners(inEvent:Dynamic)
	{
		trace(inEvent);
	}

	private static var set_event_handle = CFFI.load("kartridge", "kartridge_set_event_handle", 1);
	
	public static function initialize(settings_json:String):Bool
	{
		set_event_handle(notifyListeners);
		return kartridge_initialize(settings_json);
	}
	private static var kartridge_initialize = CFFI.load ("kartridge", "kartridge_initialize", 1);
	
	public static function shutdown():Void
	{
		kartridge_shutdown();
	}
	private static var kartridge_shutdown = CFFI.load ("kartridge", "kartridge_shutdown", 0);
	
	public static function update():Void
	{
		kartridge_update();
	}
	private static var kartridge_update = CFFI.load ("kartridge", "kartridge_update", 0);
	
	public static function isConnected():Bool
	{
		return kartridge_isConnected();
	}
	private static var kartridge_isConnected = CFFI.load ("kartridge", "kartridge_isConnected", 0);
	
	public static function isReady():Bool
	{
		return kartridge_isReady();
	}
	private static var kartridge_isReady = CFFI.load ("kartridge", "kartridge_isReady", 0);
	
	public static function restartWithKartridgeIfNeeded(game_id:Int):Bool
	{
		return kartridge_restartWithKartridgeIfNeeded(game_id);
	}
	private static var kartridge_restartWithKartridgeIfNeeded = CFFI.load ("kartridge", "kartridge_restartWithKartridgeIfNeeded", 1);
	
	public static function getUsername():String
	{
		return kartridge_getUsername();
	}
	private static var kartridge_getUsername = CFFI.load ("kartridge", "kartridge_getUsername", 0);
	
	public static function getUserId():String
	{
		return kartridge_getUserId();
	}
	private static var kartridge_getUserId = CFFI.load ("kartridge", "kartridge_getUserId", 0);
	
	public static function getGameAuthToken():String
	{
		return kartridge_getGameAuthToken();
	}
	private static var kartridge_getGameAuthToken = CFFI.load ("kartridge", "kartridge_getGameAuthToken", 0);
	
	public static function submit(name:String,value:Int):Void
	{
		kartridge_submit(name,value);
	}
	private static var kartridge_submit = CFFI.load ("kartridge", "kartridge_submit", 2);
}