// uxplay_api.h
#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int start_uxplay(int argc, char *argv[]);
void stop_uxplay();

/* ---- Event API (additive; used by GUI front-ends) ----------------------
 * Events are delivered synchronously from engine threads. The callback must
 * return quickly and must copy anything it wants to keep: every pointer in
 * uxp_event is valid only for the duration of the call. */
typedef enum {
    UXP_EVT_CLIENT_CONNECTED = 1, /* device_name, device_model, device_id */
    UXP_EVT_CLIENT_DISCONNECTED,  /* no payload; all connections are gone */
    UXP_EVT_METADATA,             /* title, artist, album (any may be NULL) */
    UXP_EVT_COVERART,             /* data/data_len: JPEG bytes */
    UXP_EVT_PROGRESS,             /* rtp_start, rtp_curr, rtp_end (44100 Hz ticks) */
    UXP_EVT_VOLUME,               /* volume_db: -30..0, -144 = mute */
    UXP_EVT_DACP,                 /* dacp_id, active_remote */
    UXP_EVT_AUDIO_FLUSH,          /* audio stream flushed (seek / track change) */
    UXP_EVT_AUDIO_ACTIVITY        /* audio packets are arriving; sent at most 4x/s */
} uxp_event_type;

typedef struct {
    uxp_event_type type;
    const char *device_name;
    const char *device_model;
    const char *device_id;
    const char *title;
    const char *artist;
    const char *album;
    const void *data;
    size_t data_len;
    uint32_t rtp_start;
    uint32_t rtp_curr;
    uint32_t rtp_end;
    float volume_db;
    const char *dacp_id;
    const char *active_remote;
} uxp_event;

typedef void (*uxp_event_cb)(const uxp_event *ev, void *user);

/* Register (or clear with cb == NULL) the event callback. Thread-safe. After
 * this call returns with cb == NULL no further callback invocation is in progress. */
void uxplay_set_event_callback(uxp_event_cb cb, void *user);

#ifdef __cplusplus
}
#endif
