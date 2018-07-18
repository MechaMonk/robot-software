#ifndef RANGING_THREAD_H
#define RANGING_THREAD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct {
    uint32_t timestamp;   ///< Time at which the ranging solution was found (in us since boot)
    uint16_t anchor_addr; ///< Address of the anchor with which the measurement was done
    float range;          ///< Distance to the anchor, in meters
    uint32_t ranging_time;  /* time it takes for the whole ranging procedure in nano secs */
} range_msg_t;

typedef struct {
    uint32_t timestamp;
    uint16_t anchor_addr;
    float x;
    float y;
    float z;
} anchor_position_msg_t;

typedef struct {
    uint32_t timestamp;
    uint16_t tag_addr;
    float x;
    float y;
} tag_position_msg_t;

void ranging_start(void);

#ifdef __cplusplus
}
#endif

#endif

