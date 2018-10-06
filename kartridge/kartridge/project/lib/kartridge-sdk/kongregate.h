/**
 * @file   kongregate.h
 * @brief  The C interface for the Kartridge library
 *
 * This file includes methods for interacting with the Kartridge application.
 * All methods use the cdecl calling convention and are not thread safe. Boolean
 * values are one byte, and need to be marshalled accordingly when wrapped using a
 * different language or framework.
 */

#ifndef Kongregate_H
#define Kongregate_H

#ifdef __APPLE__
#include <stdint.h>
#define KONG_EXPORT __attribute__((visibility("default")))
#define KONG_CCONV __attribute__((cdecl))
#elif _WIN32
#include <cstdint>
#define KONG_CCONV __cdecl
#ifdef KONG_DLL_EXPORT
#define KONG_EXPORT __declspec(dllexport)
#elif KONG_STATIC_LIBRARY
#define KONG_EXPORT
#else
#define KONG_EXPORT __declspec(dllimport)
#endif
#endif

extern "C" {
/**
 * Function pointer type used for handling Kongregate API events. The callback should use the cdecl
 * calling convention. This callback will be called from #KongregateAPI_Update when events are
 * processed.
 *
 * @param event_name The name of the event as a NULL-terminated string
 * @param event_payload A NULL-terminated string containing a JSON object with event parameters
 * @see KongregateAPI_Update
 */
typedef void (KONG_CCONV *kongregate_event_listener)
             (const char* const event_name, const char* const event_payload);

/**
 * Event broadcast when the API has been fully initialized and it has successfully communicated with the
 * Kartridge application for the first time. After this event is fired, #KongregateAPI_IsReady will return
 * true.
 *
 * Generally, you will want to wait for #KONGREGATE_EVENT_USER or #KONGREGATE_EVENT_AUTH_TOKEN if you need
 * information about the current user.
 *
 * @see KongregateAPI_IsReady
 * @see KONGREGATE_EVENT_USER
 * @see KONGREGATE_EVENT_AUTH_TOKEN
 */
KONG_EXPORT extern const char* const KONGREGATE_EVENT_READY;

/**
 * Event broadcast once user information is available. Before this event is broadcast for the first
 * time, users will be treated as guests. Once this event is received, the user ID and username returned
 * will match the user that is logged into Kartridge (if any).
 *
 * This event can be fired again if the user signs out and back with a different Kongregate account, so
 * you should handle it accordingly.
 *
 * The game authentication token may not be available when this event is fired. If you need the authentication
 * token, you should wait for the #KONGREGATE_EVENT_AUTH_TOKEN event instead.
 *
 * @see KongregateServices_GetUsername
 * @see KongregateServices_GetUserId
 * @see KONGREGATE_EVENT_AUTH_TOKEN
 */
KONG_EXPORT extern const char* const KONGREGATE_EVENT_USER;

/**
 * Event broadcast when the Kongregate authentication token is received or changed. If your game uses the
 * token, it should authenticate with your server when this event is fired. This event can be fired if
 * either the user or token changes. It will follow the #KONGREGATE_EVENT_USER event if the user
 * has changed as well. In general, if your application needs to use auth tokens, you can just listen
 * for this event and ignore #KONGREGATE_EVENT_USER.
 *
 * @see KongregateServices_GetGameAuthToken
 * @see KONGREGATE_EVENT_USER
 */
KONG_EXPORT extern const char* const KONGREGATE_EVENT_AUTH_TOKEN;

/**
 * Event broadcast when the API has connected to the Kartridge application process.
 * This event is for advanced usage and can typically be ignored.
 *
 * @see KONGREGATE_EVENT_DISCONNECTED
 */
KONG_EXPORT extern const char* const KONGREGATE_EVENT_CONNECTED;

/**
 * Event broadcast when the API has disconnected from the Kartridge application process.
 * This event is for advanced usage and can typically be ignored.
 *
 * @see KONGREGATE_EVENT_CONNECTED
 */
KONG_EXPORT extern const char* const KONGREGATE_EVENT_DISCONNECTED;

/**
 * Event broadcast when the API is shutting down.
 * This event is for advanced usage and can typically be ignored.
 */
KONG_EXPORT extern const char* const KONGREGATE_EVENT_SHUTDOWN;

/**
 * An empty JSON event payload
 */
KONG_EXPORT extern const char* const KONGREGATE_EMPTY_EVENT_PAYLOAD;

/**
 * An empty NULL-terminated string
 */
KONG_EXPORT extern const char* const KONGREGATE_EMPTY_STRING;

/**
 * Username representing a guest user - "Guest"
 */
KONG_EXPORT extern const char* const KONGREGATE_GUEST_USERNAME;

/**
 * Checks to see if the game was launched from Kartridge, and requests Kartridge launch the game
 * if not. If you have a kong_gameid.txt file present with your game ID in it, this will always
 * return false. The kong_gameid.txt should not be packaged with your release build. If this method
 * returns true, you should exit your game as soon as possible, and not initialize the API, as that
 * means that the Kartridge application will attempt to launch a duplicate copy of your game.
 *
 * This method is optional but suggested if your game depends on Kartridge API functionality. You
 * should call it immediately before #KongregateAPI_Initialize.
 *
 * @param game_id The Kongregate game ID for your game
 * @return true if the game will be re-launched via Kartridge, or false if not.
 * @see KongregateAPI_Initialize
 */
KONG_EXPORT bool KONG_CCONV KongregateAPI_RestartWithKartridgeIfNeeded(uint32_t game_id);

/**
 * Initialize the Kongregate API. You must call this before most other API methods. If this method returns
 * false, it can be because the game was not launched via Kartridge or because Kartridge is not installed,
 * and all further API calls will be stubbed out.
 *
 * This method will use the game ID passed in from Kartridge, or the one present in the kong_gameid.txt
 * file in the working directory the game was launched from if present. The kong_gameid.txt file should
 * only be used for testing, and you should not ship it with your game.
 *
 * @param settings_json An optional string representation of a JSON object containing API settings. Can be
 *        NULL to use all defaults.
 * @return true if the API was successfully initialized, which means your game ID was successfully
 *         detected. If this method returns false, it means the game was not launched from Kartridge
 *         or the kong_gameid.txt file is not present.
 * @see KongregateAPI_RestartWithKartridgeIfNeeded
 */
KONG_EXPORT bool KONG_CCONV KongregateAPI_Initialize(const char *settings_json);

/**
 * Shuts down the Kongregate API after it has been initialized. After this method is called, you must call
 * KongregateAPI_Initialize() again for the API to function.
 */
KONG_EXPORT void KONG_CCONV KongregateAPI_Shutdown();

/**
 * Updates the internal state of the Kongregate API. Must be called periodically to maintain a connection
 * to the Kongregate application and process events. Typically you would call this every frame or once
 * every few frames.
 *
 * This call can mutate the state of the API (username, user ID, etc), and it is not thread safe. You should
 * always call all API functions from the same thread, or use an external synchronization mechanism.
 *
 * Additionally, strings returned from the various API functions are only guaranteed to be valid until the
 * next call to #KongregateAPI_Update, which means you should make a copy of the string if you need it for
 * a longer amount of time.
 */
KONG_EXPORT void KONG_CCONV KongregateAPI_Update();

/**
 * @return true if the API is connected to the Kongregate application. You can use this method to determine
 * if the Kartridge application is running or not.
 */
KONG_EXPORT bool KONG_CCONV KongregateAPI_IsConnected();

/**
 * @return true if the API has been fully initialized and has successfully communicated with the Kartridge
 * application for the first time. Will always return true after #KONGREGATE_EVENT_READY has been fired.
 *
 * @see KONGREGATE_EVENT_READY
 */
KONG_EXPORT bool KONG_CCONV KongregateAPI_IsReady();

/**
 * Sets the event listener function pointer to listen for API events.
 * @param listener The function that will be called when an event notification is fired
 */
KONG_EXPORT void KONG_CCONV KongregateAPI_SetEventListener(kongregate_event_listener listener);

/**
 * @return the username for the active user within Kartridge. Result is not valid until
 * #KONGREGATE_EVENT_USER has been fired. Will return an empty string if the user information has
 * not yet been fetched, and "Guest" for guest users.
 *
 * @see KONGREGATE_EVENT_USER
 */
KONG_EXPORT const char* KONG_CCONV KongregateServices_GetUsername();

/**
 * @return the user ID for the active user. Result is not valid until
 * #KONGREGATE_EVENT_USER has been fired. Will return 0 for guest users.
 *
 * @see KONGREGATE_EVENT_USER
 */
KONG_EXPORT uint32_t KONG_CCONV KongregateServices_GetUserId();

/**
 * @return the game authorization token for the active user for your game. Use the Kongregate
 * REST API from your server to validate the user.
 *
 * Will return an empty string if the user is a guest or the key has not been fetched yet.
 *
 * As with all strings returned from the API, this pointer can be deallocated after the
 * next call to #KongregateAPI_Update, so it is important to make a copy if you need it.
 *
 * @see KONGREGATE_EVENT_AUTH_TOKEN
 */
KONG_EXPORT const char* KONG_CCONV KongregateServices_GetGameAuthToken();

/**
 * Submit a statistic with the given name and value
 * @param name The statistic name
 * @param value The value to submit
 */
KONG_EXPORT void KONG_CCONV KongregateStats_Submit(const char *name, int64_t value);
}

#endif
// Kongregate Kartridge SDK - [1.0.2] - 06/19/2018
