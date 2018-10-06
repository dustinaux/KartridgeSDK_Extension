#ifndef KARTRIDGE_H
#define KARTRIDGE_H


namespace kartridge 
{
	bool initialize(const char* settings_json);
	void shutdown();
	void update();
	bool isConnected();
	bool isReady();
	bool restartWithKartridgeIfNeeded(int game_id);
	const char* getUsername();
	int getUserId();
	const char* getGameAuthToken();
	void submit(const char* name, int _value);
}


#endif