/*
 * Copyright (c) 2018 Elastos Foundation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __ELA_CARRIER_H__
#define __ELA_CARRIER_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

#if defined(__APPLE__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdocumentation"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(CARRIER_STATIC)
  #define CARRIER_API
#elif defined(CARRIER_DYNAMIC)
  #ifdef CARRIER_BUILD
    #if defined(_WIN32) || defined(_WIN64)
      #define CARRIER_API        __declspec(dllexport)
    #else
      #define CARRIER_API        __attribute__((visibility("default")))
    #endif
  #else
    #if defined(_WIN32) || defined(_WIN64)
      #define CARRIER_API        __declspec(dllimport)
    #else
      #define CARRIER_API
    #endif
  #endif
#else
  #define CARRIER_API
#endif

/**
 * \~English
 * Carrier User address max length.
 */
#define ELA_MAX_ADDRESS_LEN             52

/**
 * \~English
 * Carrier Node/User ID max length.
 */
#define ELA_MAX_ID_LEN                  45

/**
 * \~English
 * Carrier user name max length.
 */
#define ELA_MAX_USER_NAME_LEN           63

/**
 * \~English
 * Carrier user password max length.
 */
#define ELA_MAX_PASSWORD_LEN            63

/**
 * \~English
 * Carrier user description max length.
 */
#define ELA_MAX_USER_DESCRIPTION_LEN    127

/**
 * \~English
 * Carrier user phone number max length.
 */
#define ELA_MAX_PHONE_LEN               31

/**
 * \~English
 * Carrier user email address max length.
 */
#define ELA_MAX_EMAIL_LEN               127

/**
 * \~English
 * Carrier user region max length.
 */
#define ELA_MAX_REGION_LEN              127

/**
 * \~English
 * Carrier user gender max length.
 */
#define ELA_MAX_GENDER_LEN              31

/**
 * \~English
 * Carrier node name max length.
 */
#define ELA_MAX_NODE_NAME_LEN           63

/**
 * \~English
 * Carrier node description max length.
 */
#define ELA_MAX_NODE_DESCRIPTION_LEN    127

/**
 * \~English
 * Carrier App message max length.
 */
#define ELA_MAX_APP_MESSAGE_LEN         1024

typedef struct ElaCarrier ElaCarrier;

/******************************************************************************
 * Creation & destruction
 *****************************************************************************/

/**
 * \~English
 * Bootstrap defines a couple of perperities to provide for Carrier nodes
 * to connect with. The bootstrap nodes help Carrier nodes be connected to
 * the others with more higher possibilities.
 */
typedef struct BootstrapNode {
    /**
     * \~English
     * The ip address supported with ipv4 protocol.
     */
    const char *ipv4;

    /**
     * \~English
     * The ip address supported with ipv6 protocol.
     */
    const char *ipv6;

    /**
     * \~English
     * The ip port.
     * The default value is 33445.
     */
    const char *port;

    /**
     * \~English
     * The unique public key to provide for Carrier nodes, terminated
     * by null-string.
     * The length of public key is about 45 bytes.
     */
    const char *public_key;
} BootstrapNode;

/**
 * \~English
 * ElaOptions defines several settings that control the way the Carrier
 * node connects to others.
 *
 * @remark
 *      Default values are not defined for persistent_location of Carrier-
 *      Options, so application should be set persistent_location clearly.
 *      If the ElaOptions structure is defined as a static variable,
 *      initialization (in compliant compilers) sets all values to 0 (NULL
 *      for pointers).
 */
typedef struct ElaOptions {
    /**
     * \~English
     * The application defined persistent data location.
     * The location must be set.
     */
    const char *persistent_location;

    /**
     * \~English
     * The option to decide to use udp transport or not. Setting this option
     * to false will force Carrier node to use TCP only, which will potentially
     * slow down the message to run through.
     */
    bool udp_enabled; 

    /**
     * \~English
     * The total number of bootstrap nodes to connect.
     * There must have at least one bootstrap node for the very first time
     * to create carrier instance.
     */
    size_t bootstraps_size;

    /**
     * \~English
     * The array of bootstrap nodes.
     */
    BootstrapNode *bootstraps;
} ElaOptions;

/**
 * \~English
 * Get the current version of Carrier node.
 */
CARRIER_API
const char *ela_get_version(void);

/**
 * \~English
 * Carrier log level to control or filter log output.
 */
typedef enum ElaLogLevel {
    /**
     * \~English
     * Log level None
     * Indicate disable log output.
     */
    ElaLogLevel_None = 0,
    /**
     * \~English
     * Log level fatal.
     * Indicate output log with level 'Fatal' only.
     */
    ElaLogLevel_Fatal = 1,
    /**
     * \~English
     * Log level error.
     * Indicate output log above 'Error' level.
     */
    ElaLogLevel_Error = 2,
    /**
     * \~English
     * Log level warning.
     * Indicate output log above 'Warning' level.
     */
    ElaLogLevel_Warning = 3,
    /**
     * \~English
     * Log level info.
     * Indicate output log above 'Info' level.
     */
    ElaLogLevel_Info = 4,
    /*
     * \~English
     * Log level debug.
     * Indicate output log above 'Debug' level.
     */
    ElaLogLevel_Debug = 5,
    /*
     * \~English
     * Log level trace.
     * Indicate output log above 'Trace' level.
     */
    ElaLogLevel_Trace = 6,
    /*
     * \~English
     * Log level verbose.
     * Indicate output log above 'Verbose' level.
     */
    ElaLogLevel_Verbose = 7
} ElaLogLevel;

/**
 * \~English
 * Carrier node connection status to Carrier network.
 */
typedef enum ElaConnectionStatus {
    /**
     * \~English
     * Carrier node connected to Carrier network.
     * Indicate the Carrier node is online.
     */
    ElaConnectionStatus_Connected,
    /**
     * \~English
     * There is no connection to Carrier network.
     * Indicate the Carrier node is offline.
     */
    ElaConnectionStatus_Disconnected,
} ElaConnectionStatus;

/**
 * \~English
 * Carrier node presence status to Carrier network.
 */
typedef enum ElaPresenceStatus {
    /**
     * \~English
     * Carrier node is online and available.
     */
    ElaPresenceStatus_None,
    /**
     * \~English
     * Carrier node is being away.
     * Carrier node can set this value with an user defined inactivity time.
     */
    ElaPresenceStatus_Away,
    /**
     * \~English
     * Carrier node is being busy.
     * Carrier node can set this value to tell friends that it can not
     * currently wish to commincate.
     */
    ElaPresenceStatus_Busy,
} ElaPresenceStatus;

/**
 * \~English
 * A structure representing the Carrier user information.
 *
 * In Carrier SDK, self and all friends are carrier user, and have
 * same user attributes.
 */
typedef struct ElaUserInfo {
    /**
     * \~English
     * User ID. Read only to application.
     */
    char userid[ELA_MAX_ID_LEN+1];
    /**
     * \~English
     * Nickname, also known as display name.
     */
    char name[ELA_MAX_USER_NAME_LEN+1];
    /**
     * \~English
     * User's description, also known as what's up.
     */
    char description[ELA_MAX_USER_DESCRIPTION_LEN+1];
    /**
     * \~English
     * If user has an avatar.
     */
    int has_avatar;
    /**
     * \~English
     * User's gender.
     */
    char gender[ELA_MAX_GENDER_LEN+1];
    /**
     * \~English
     * User's phone number.
     */
    char phone[ELA_MAX_PHONE_LEN+1];
    /**
     * \~English
     * User's email address.
     */
    char email[ELA_MAX_EMAIL_LEN+1];
    /**
     * \~English
     * User's region information.
     */
    char region[ELA_MAX_REGION_LEN+1];
} ElaUserInfo;

/**
 * \~English
 * A structure representing the Carrier friend information.
 *
 * Include the basic user information and the extra friend information.
 */
typedef struct ElaFriendInfo {
    /**
     * \~English
     * Friend's user information.
     */
    ElaUserInfo user_info;
    /**
     * \~English
     * Your label for the friend.
     */
    char label[ELA_MAX_USER_NAME_LEN+1];
    /**
     * \~English
     * Friend's connection status.
     */
    ElaConnectionStatus status;
    /**
     * \~English
     * Friend's presence status.
     */
    ElaPresenceStatus presence;
} ElaFriendInfo;

/**
 * \~English
 * Carrier callbacks, include all global callbacks for Carrier.
 */
typedef struct ElaCallbacks {
    /**
     * \~English
     * An application-defined function that perform idle work.
     *
     * @param
     *      carrier     [in] A handle to the Carrier node instance.
     * @param
     *      context     [in] The application defined context data.
     */
    void (*idle)(ElaCarrier *carrier, void *context);

    /**
     * \~English
     * An application-defined function that process the self connection status.
     *
     * @param
     *      carrier     [in] A handle to the Carrier node instance.
     * @param
     *      status      [in] Current connection status. @see ElaConnectionStatus.
     * @param
     *      context     [in] The application defined context data.
     */
    void (*connection_status)(ElaCarrier *carrier,
                              ElaConnectionStatus status, void *context);

    /**
     * \~English
     * An application-defined function that process the ready notification.
     * Notice: application should wait this callback invoked before calling any
     * carrier function to interact with friends.
     *
     * @param
     *      carrier     [in] A handle to the Carrier node instance.
     * @param
     *      context     [in] The application defined context data.
     */
    void (*ready)(ElaCarrier *carrier, void *context);

    /**
     * \~English
     * An application-defined function that process the self info change event.
     * This callback is reserved for future compatibility.
     *
     * @param
     *      carrier     [in] A handle to the Carrier node instance.
     * @param
     *      info        [in] The ElaUserInfo pointer to the new user info.
     * @param
     *      context     [in] The application defined context data.
     */
    void (*self_info)(ElaCarrier *carrier, const ElaUserInfo *info, void *context);

    /**
     * \~English
     * An application-defined function that iterate the each friends list item.
     *
     * @param
     *      carrier     [in] A handle to the Carrier node instance.
     * @param
     *      info        [in] A pointer to ElaFriendInfo structure that
     *                       representing a friend
     * @param
     *      context     [in] The application defined context data.
     *
     * @return
     *      Return true to continue iterate next friend user info,
     *      false to stop iterate.
     */
    bool (*friend_list)(ElaCarrier *carrier, const ElaFriendInfo* info,
                        void* context);

    /**
     * \~English
     * An application-defined function that process the friend connection
     * change event.
     *
     * @param
     *      carrier     [in] A handle to the Carrier node instance.
     * @param
     *      friendid    [in] The friend's user id.
     * @param
     *      status      [in] Connection status. @see ElaConnectionStatus
     * @param
     *      context     [in] The application defined context data.
     */
    void (*friend_connection)(ElaCarrier *carrier,const char *friendid,
                              ElaConnectionStatus status, void *context);

    /**
     * \~English
     * An application-defined function that process the friend information
     * change event.
     *
     * @param
     *      carrier     [in] A handle to the Carrier node instance.
     * @param
     *      friendid    [in] The friend's user id.
     * @param
     *      info        [in] The ElaFriendInfo pointer to the new friend info.
     * @param
     *      context     [in] The application defined context data.
     */
    void (*friend_info)(ElaCarrier *carrier, const char *friendid,
                        const ElaFriendInfo *info, void *context);

    /**
     * \~English
     * An application-defined function that process the friend presence
     * change event.
     *
     * @param
     *      carrier     [in] A handle to the Carrier node instance.
     * @param
     *      friendid    [in] The friend's user id.
     * @param
     *      presence    [in] The presence status of the friend.
     * @param
     *      context     [in] The application defined context data.
     */
    void (*friend_presence)(ElaCarrier *carrier, const char *friendid,
                            ElaPresenceStatus status, void *context);

    /**
     * \~English
     * An application-defined function that process the friend request.
     *
     * @param
     *      carrier     [in] A handle to the Carrier node instance.
     * @param
     *      userid      [in] The user id who wants friend with us.
     * @param
     *      info        [in] The basic user info who wants to be friend.
     * @param
     *      hello       [in] PIN for target user, or any application defined
     *                       content.
     * @param
     *      context     [in] The application defined context data.
     */
    void (*friend_request)(ElaCarrier *carrier, const char *userid,
                           const ElaUserInfo *info,
                           const char *hello, void *context);

    /**
     * \~English
     * An application-defined function that process the new friend added
     * event.
     *
     * @param
     *      carrier     [in] A handle to the Carrier node instance.
     * @param
     *      info        [in] The firend's information.
     * @param
     *      context     [in] The application defined context data.
     */
    void (*friend_added)(ElaCarrier *carrier, const ElaFriendInfo *info,
                         void *context);

    /**
     * \~English
     * An application-defined function that process the friend removed
     * event.
     *
     * @param
     *      carrier     [in] A handle to the Carrier node instance.
     * @param
     *      friendid    [in] The friend's user id.
     * @param
     *      context     [in] The application defined context data.
     */
    void (*friend_removed)(ElaCarrier *carrier, const char *friendid,
                           void *context);

    /**
     * \~English
     * An application-defined function that process the friend messages.
     *
     * @param
     *      carrier     [in] A handle to the Carrier node instance.
     * @param
     *      from        [in] The id(userid@nodeid) from who send the message.
     * @param
     *      msg         [in] The message content.
     * @param
     *      len         [in] The message length in bytes.
     * @param
     *      context     [in] The application defined context data.
     */
    void (*friend_message)(ElaCarrier *carrier, const char *from,
                           const char *msg, size_t len, void *context);

    /**
     * \~English
     * An application-defined function that process the friend invite request.
     *
     * @param
     *      carrier     [in] A handle to the Carrier node instance.
     * @param
     *      from        [in] The user id from who send the invite request.
     * @param
     *      data        [in] The application defined data send from friend.
     * @param
     *      len         [in] The data length in bytes.
     * @param
     *      context     [in] The application defined context data.
     */
    void (*friend_invite)(ElaCarrier *carrier, const char *from,
                          const char *data, size_t len, void *context);

} ElaCallbacks;

/**
 * \~English
 * initialize log options for Carrier. The default level to control log output
 * is 'Info'.
 *
 * @param
 *      level       [in] The log level to control internal log output.
 * @param
 *      log_file    [in] the log file name.
 *                       If the log_file is NULL, Carrier will not write
 *                       log to file.
 * @param
 *      log_printer [in] the user defined log printer. can be NULL.
 */
CARRIER_API
void ela_log_init(ElaLogLevel level, const char *log_file,
                  void (*log_printer)(const char *format, va_list args));

/**
 * \~English
 * Check if the carrier address is valid.
 *
 * @param
 *      address     [in] the carrier address to be check.
 *
 * @return
 *      true if address is valid, or false if address is not valid.
 */
CARRIER_API
bool ela_address_is_valid(const char *address);

/**
 * \~English
 * Check if the carrier ID is valid.
 *
 * @param
 *      id          [in] the carrier id to be check.
 *
 * @return
 *      true if id is valid, or false if id is not valid.
 */
CARRIER_API
bool ela_id_is_valid(const char *id);

/**
 * \~English
 * Create a new Carrier node instance. after creating the instance, it's
 * ready for connection to Carrier network.
 *
 * @param
 *      options     [in] A pointer to a valid ElaOptions structure.
 * @param
 *      callbacks   [in] The Application defined callback functions.
 * @param
 *      context     [in] The application defined context data.
 *
 * @return
 *      If no error occurs, return the pointer of Carrier node instance.
 *      Otherwise, return NULL, and a specific error code can be
 *      retrieved by calling ela_get_error().
 */
CARRIER_API
ElaCarrier *ela_new(const ElaOptions *options,
                    ElaCallbacks *callbacks, void *context);

/**
 * \~English
 * Disconnect from Carrier network, and destroy all associated resources
 * with the Carrier node instance.
 *
 * After calling the function, the Carrier pointer becomes invalid.
 * No other functions can be called.
 *
 * @param
 *      carrier     [in] A handle identifying the Carrier node instance
 *                       to kill.
 */
CARRIER_API
void ela_kill(ElaCarrier *carrier);

/******************************************************************************
 * \~English
 * Connection & event loop
 *****************************************************************************/
/**
 * \~English
 * Attempts to connect the node to Carrier network. If the node successfully
 * connects to Carrier network, then it starts the node's main event loop.
 * The connect options was specified by previously create options.
 * @see ela_new().
 *
 * @param
 *      carrier     [in] A handle identifying the Carrier node instance.
 * @param
 *      interval    [in] Internal loop interval, in milliseconds.
 *
 * @return
 *      0 if the client successfully connected to Carrier network and start the
 *      event loop. Otherwise, return -1, and a specific error code can be
 *      retrieved by calling ela_get_error().
 */
CARRIER_API
int ela_run(ElaCarrier *carrier, int interval);

/******************************************************************************
 * Internal node information
 *****************************************************************************/

/**
 * \~English
 * Get user address associated with the Carrier node.
 *
 * @param
 *      carrier     [in] A handle to the Carrier node instance.
 * @param
 *      address     [out] The buffer that will receive the address.
 *                        The buffer size should at least
 *                        (ELA_MAX_ADDRESS_LEN + 1) bytes.
 * @param
 *      len         [in] The buffer size of address.
 *
 * @return
 *      The address string pointer, or NULL if buffer is too small.
 */
CARRIER_API
char *ela_get_address(ElaCarrier *carrier, char *address, size_t len);

/**
 * \~English
 * Get node identifier associated with this Carrier node.
 *
 * @param
 *      carrier     [in] A handle to the Carrier node instance.
 * @param
 *      nodeid      [out] The buffer that will receive the identifier.
 *                        The buffer size should at least
 *                        (ELA_MAX_ID_LEN + 1) bytes.
 * @param
 *      len         [in] The buffer size of nodeid.
 *
 * @return
 *      The nodeId string pointer, or NULL if buffer is too small.
 */
CARRIER_API
char *ela_get_nodeid(ElaCarrier *carrier, char *nodeid, size_t len);

/**
 * \~English
 * Get user identifier associated with this Carrier node.
 *
 * @param
 *      carrier     [in] A handle to the Carrier node instance.
 * @param
 *      userid      [out] The buffer that will receive the identifier.
 *                        The buffer size should at least
 *                        (ELA_MAX_ID_LEN + 1) bytes.
 * @param
 *      len         [in] The buffer size of userid.
 *
 * @return
 *      The userId string pointer, or NULL if buffer is too small.
 */
CARRIER_API
char *ela_get_userid(ElaCarrier *carrier, char *userid, size_t len);

/******************************************************************************
 * Client information
 *****************************************************************************/

/**
 * \~Egnlish
 * Update the nospam for Carrier address.
 *
 * Update the 4-byte nospam part of the Carrier address with host byte order
 * expected. Nospam for Carrier address is used to eliminate spam friend
 * request.
 *
 * @param
 *      carrier     [in] A handle to the Carrier node instance.
 * @param
 *      nospam      [in] An 4-bytes unsigned integer.
 *
 * @return
 *      0 on success, or -1 if an error occurred. The specific error code
 *      can be retrieved by calling ela_get_error().
 */
CARRIER_API
int ela_set_self_nospam(ElaCarrier *carrier, uint32_t nospam);

/**
 * \~Egnlish
 * Get the nospam for Carrier address.
 *
 * Get the 4-byte nospam part of the Carrier address with host byte order
 * expected. Nospam for Carrier address is used to eliminate spam friend
 * request.
 *
 * @param
 *      carrier     [in] A handle to the Carrier node instance.
 * @param
 *      nospam      [in] An unsigned integer pointer to receive nospam value.
 * @return
 *      0 on success, or -1 if an error occurred. The specific error code
 *      can be retrieved by calling ela_get_error().
 */
CARRIER_API
int ela_get_self_nospam(ElaCarrier *carrier, uint32_t *nospam);

/**
 * \~English
 * Update self information.
 *
 * As self information changed, Carrier node would update itself information
 * to Carrier network, which would forward the change to all friends.
 * nodes.
 *
 * @param
 *      carrier     [in] A handle to the Carrier node instance.
 * @param
 *      info        [in] The ElaUserInfo pointer to the updated user info.
 *
 * @return
 *      0 on success, or -1 if an error occurred. The specific error code
 *      can be retrieved by calling ela_get_error().
 */
CARRIER_API
int ela_set_self_info(ElaCarrier *carrier, const ElaUserInfo *info);

/**
 * \~English
 * Get self information.
 *
 * @param
 *      carrier     [in] A handle to the Carrier node instance.
 * @param
 *      info        [in] The ElaUserInfo pointer to receive user info.
 *
 * @return
 *      0 on success, or -1 if an error occurred. The specific error code
 *      can be retrieved by calling ela_get_error().
 */
CARRIER_API
int ela_get_self_info(ElaCarrier *carrier, ElaUserInfo *info);

/**
 * \~English
 * Set self presence status.
 *
 * @param
 *      carrier     [in] A handle to the Carrier node instance.
 * @param
 *      presence    [in] the new presence status.
 *
 * @return
 *      0 on success, or -1 if an error occurred. The specific error code
 *      can be retrieved by calling ela_get_error().
 */
CARRIER_API
int ela_set_self_presence(ElaCarrier *carrier, ElaPresenceStatus status);

/**
 * \~English
 * Get self presence status.
 *
 * @param
 *      carrier     [in] A handle to the Carrier node instance.
 * @param
 *      presence    [in] A pointer to receive presence status value.
 *
 * @return
 *      0 on success, or -1 if an error occurred. The specific error code
 *      can be retrieved by calling ela_get_error().
 */
CARRIER_API
int ela_get_self_presence(ElaCarrier *carrier, ElaPresenceStatus *status);

/**
 * \~English
 * Check if Carrier node instance is being ready.
 *
 * All carrier interactive APIs should be called only if carrier instance
 * is being ready.
 *
 * @param
 *      carrier     [in] A handle to the Carrier node instance.
 *
 * @return
 *      true if the carrier node instance is ready, or false if not.
 */
CARRIER_API
bool ela_is_ready(ElaCarrier *carrier);

/******************************************************************************
 * Friend information
 *****************************************************************************/

/**
 * \~English
 * An application-defined function that iterate the each friends list item.
 *
 * ElaFriendsIterateCallback is the callback function type.
 *
 * @param
 *      info        [in] A pointer to ElaFriendInfo structure that
 *                       representing a friend
 * @param
 *      context     [in] The application defined context data.
 *
 * @return
 *      Return true to continue iterate next friend user info,
 *      false to stop iterate.
 */
typedef bool ElaFriendsIterateCallback(const ElaFriendInfo *info,
                                       void *context);
/**
 * \~English
 * Get friends list. For each friend will call the application defined
 * iterate callback.
 *
 * @param
 *      carrier     [in] a handle to the Carrier node instance.
 * @param
 *      callback    [in] a pointer to ElaFriendsIterateCallback function.
 * @param
 *      context     [in] the application defined context data.
 *
 * @return
 *      0 on success, or -1 if an error occurred. The specific error code
 *      can be retrieved by calling ela_get_error().
 */
CARRIER_API
int ela_get_friends(ElaCarrier *carrier,
                    ElaFriendsIterateCallback *callback, void *context);

/**
 * \~English
 * Get friend information.
 *
 * @param
 *      carrier     [in] A handle to the Carrier node instance.
 * @param
 *      friendid    [in] The friend's user id.
 * @param
 *      info        [in] The ElaFriendInfo pointer to receive the friend
 *                       information.
 *
 * @return
 *      0 on success, or -1 if an error occurred. The specific error code
 *      can be retrieved by calling ela_get_error().
 */
CARRIER_API
int ela_get_friend_info(ElaCarrier *carrier, const char *friendid,
                        ElaFriendInfo *info);

/**
 * \~English
 * Set the label of the specified friend.
 *
 * If the value length is 0 or value is NULL, the attribute will be cleared.
 *
 * @param
 *      carrier     [in] A handle to the Carrier node instance.
 * @param
 *      friendid    [in] The friend's user id.
 * @param
 *      label       [in] the new label of the specified friend.
 *
 * @return
 *      0 on success, or -1 if an error occurred. The specific error code
 *      can be retrieved by calling ela_get_error().
 *
 * @remarks
 *      The label of a friend is a private alias named by yourself. It can be
 *      seen by yourself only, and has no impact to the target friend.
 */
CARRIER_API
int ela_set_friend_label(ElaCarrier *carrier,
                         const char *friendid, const char *label);

/**
 * \~English
 * Check if the user ID is friend.
 *
 * @param
 *      carrier     [in] A handle to the Carrier node instance.
 * @param
 *      userid      [in] The userid to check.
 *
 * @return
 *      true if the user id is friend, or false if not;
 */
CARRIER_API
bool ela_is_friend(ElaCarrier* carrier, const char* userid);

/******************************************************************************
 * Friend add & remove
 *****************************************************************************/

/**
 * \~English
 * Attempt to add friend by sending a new friend request.
 *
 * This function will add a new friend with specific address, and then
 * send a friend request to the target node.
 *
 * @param
 *      carrier     [in] A handle to the Carrier node instance.
 * @param
 *      address     [in] The target user address.
 * @param
 *      hello       [in] PIN for target user, or any application defined
 *                       content.
 *
 * @return
 *      0 if adding friend is successful. Otherwise, return -1, and a specific
 *      error code can be retrieved by calling ela_get_error().
 */
CARRIER_API
int ela_add_friend(ElaCarrier *carrier, const char *address, const char *hello);

/**
 * \~English
 * Accept the friend request.
 *
 * This function is used to add a friend in response to a friend request.
 *
 * @param
 *      carrier     [in] A handle to the Carrier node instance.
 * @param
 *      userid      [in] The user id to who wants to be friend with us.
 *
 * @return
 *      0 if adding friend successfully.
 *      Otherwise, return -1, and a specific error code can be
 *      retrieved by calling ela_get_error().
 */
CARRIER_API
int ela_accept_friend(ElaCarrier *carrier, const char *userid);

/**
 * \~English
 * Remove a friend.
 *
 * This function will send a remove friend indicator to Carrier network.
 *
 * If all correct, Carrier network will clean the friend relationship, and
 * send friend removed message to both.
 *
 * @param
 *      carrier     [in] A handle to the Carrier node instance.
 * @param
 *      userid      [in] The target user id.
 *
 * @return
 *      0 if the indicator successfully sent.
 *      Otherwise, return -1, and a specific error code can be
 *      retrieved by calling ela_get_error().
 */
CARRIER_API
int ela_remove_friend(ElaCarrier *carrier, const char *userid);

/******************************************************************************
 * Application transactions between friends
 *****************************************************************************/

/**
 * \~English
 * Send a message to a friend.
 *
 * The message length may not exceed ELA_MAX_APP_MESSAGE_LEN, and message
 * itself should be text-formatted. Larger messages must be split by application
 * and sent as separate messages. Other carrier nodes can reassemble the
 * fragments.
 *
 * Message may not be empty or NULL.
 * @param
 *      carrier     [in] A handle to the Carrier node instance.
 * @param
 *      to          [in] The target userid.
 * @param
 *      msg         [in] The message content defined by application.
 * @param
 *      len         [in] The message length in bytes.
 *
 * @return
 *      0 if the text message successfully sent.
 *      Otherwise, return -1, and a specific error code can be
 *      retrieved by calling ela_get_error().
 */
CARRIER_API
int ela_send_friend_message(ElaCarrier *carrier, const char *to,
                            const char *msg, size_t len);

/**
 * \~English
 * An application-defined function that process the friend invite response.
 *
 * CarrierFriendInviteResponseCallback is the callback function type.
 *
 * @param
 *      carrier     [in] A handle to the Carrier node instance.
 * @param
 *      from        [in] The target user id.
 * @param
 *      status      [in] The status code of the response.
 *                       0 is success, otherwise is error.
 * @param
 *      reason      [in] The error message if status is error, or NULL
 * @param
 *      data        [in] The application defined data return by target user.
 * @param
 *      len         [in] The data length in bytes.
 * @param
 *      context      [in] The application defined context data.
 */
typedef void ElaFriendInviteResponseCallback(ElaCarrier *carrier,
                                             const char *from,
                                             int status, const char *reason,
                                             const char *data, size_t len,
                                             void *context);

/**
 * \~English
 * Send invite request to a friend.
 *
 * Application can attach the application defined data within the invite
 * request, and the data will send to target friend.
 *
 * @param
 *      carrier     [in] A handle to the Carrier node instance.
 * @param
 *      to          [in] The target userid.
 * @param
 *      data        [in] The application defined data send to target user.
 * @param
 *      len         [in] The data length in bytes.
 * @param
 *      callback    [in] A pointer to ElaFriendInviteResponseCallback
 *                       function to receive the invite response.
 * @param
 *      context      [in] The application defined context data.
 *
 * @return
 *      0 if the invite request successfully send to the friend.
 *      Otherwise, return -1, and a specific error code can be
 *      retrieved by calling ela_get_error().
 */
CARRIER_API
int ela_invite_friend(ElaCarrier *carrier, const char *to,
                      const char *data, size_t len,
                      ElaFriendInviteResponseCallback *callback,
                      void *context);

/**
 * \~English
 * Reply the friend invite request.
 *
 * This function will send a invite response to friend.
 *
 * @param
 *      carrier     [in] A handle to the Carrier node instance.
 * @param
 *      to          [in] The userid who send invite request.
 * @param
 *      status      [in] The status code of the response.
 *                       0 is success, otherwise is error.
 * @param
 *      reason      [in] The error message if status is error, or NULL
 *                       if success.
 * @param
 *      data        [in] The application defined data send to target user.
 *                       If the status is error, this will be ignored.
 * @param
 *      len         [in] The data length in bytes.
 *                       If the status is error, this will be ignored.
 *
 * @return
 *      0 if the invite response successfully send to the friend.
 *      Otherwise, return -1, and a specific error code can be
 *      retrieved by calling ela_get_error().
 */
CARRIER_API
int ela_reply_friend_invite(ElaCarrier *carrier, const char *to,
                            int status, const char *reason,
                            const char *data, size_t len);
/******************************************************************************
 * Error handling
 *****************************************************************************/

#define ELASUCCESS                                0

// Facility code
#define ELAF_GENERAL                              1
#define ELAF_SYS                                  2
#define ELAF_HTTP                                 3
#define ELAF_RESERVED2                            4
#define ELAF_ICE                                  5
#define ELAF_DHT                                  6


#define ELAERR_INVALID_ARGS                       1
#define ELAERR_OUT_OF_MEMORY                      2
#define ELAERR_BUFFER_TOO_SMALL                   3
#define ELAERR_BAD_PERSISTENT_DATA                4
#define ELAERR_INVALID_PERSISTENCE_FILE           5
#define ELAERR_INVALID_CONTROL_PACKET             6
#define ELAERR_INVALID_CREDENTIAL                 7
#define ELAERR_SERVER_FAILED                      8
#define ELAERR_ALREADY_RUN                        9
#define ELAERR_NOT_READY                          10
#define ELAERR_NOT_EXIST                          11
#define ELAERR_ALREADY_EXIST                      12
#define ELAERR_NO_MATCHED_REQUEST                 13
#define ELAERR_INVALID_USERID                     14
#define ELAERR_INVALID_NODEID                     15
#define ELAERR_INVALID_APPID                      16
#define ELAERR_INVALID_DESCRIPTOR                 17
#define ELAERR_WRONG_STATE                        18
#define ELAERR_BUSY                               19
#define ELAERR_LANGUAGE_BINDING                   20
#define ELAERR_ENCRYPT                            21
#define ELAERR_SDP_TOO_LONG                       22
#define ELAERR_INVALID_SDP                        23
#define ELAERR_NOT_IMPLEMENTED                    24
#define ELAERR_LIMIT_EXCEEDED                     25
#define ELAERR_UNKNOWN                            26

#define ELA_MK_ERROR(facility, code)  (0x80000000 | ((facility) << 24) | \
                    ((((code) & 0x80000000) >> 8) | ((code) & 0x7FFFFFFF)))

#define ELA_GENERAL_ERROR(code)       ELA_MK_ERROR(ELAF_GENERAL, code)
#define ELA_SYS_ERROR(code)           ELA_MK_ERROR(ELAF_SYS, code)
#define ELA_HTTP_ERROR(code)          ELA_MK_ERROR(ELAF_HTTP, code)
#define ELA_ICE_ERROR(code)           ELA_MK_ERROR(ELAF_ICE, code)
#define ELA_DHT_ERROR(code)           ELA_MK_ERROR(ELAF_DHT, code)

/*
 * \~English
 * Retrieves the last-error code value. The last-error code is maintained on a
 * per-instance basis. Multiple instance do not overwrite each other's
 * last-error code.
 *
 * @return
 *      The return value is the last-error code.
 */
CARRIER_API
int ela_get_error(void);

/**
 * \~English
 * Clear the last-error code of a Carrier instance.
 */
CARRIER_API
void ela_clear_error(void);

#ifdef __cplusplus
}
#endif

#if defined(__APPLE__)
#pragma GCC diagnostic pop
#endif

#endif /* __ELA_CARRIER_H_ */
