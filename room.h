#ifndef ROOM_H
#define ROOM_H

#include <stdlib.h>
#include "utils.h"

/*
 * The maximum number of rooms a single room can be connected to.
 */
extern const size_t MAX_CONNECTIONS; 

/*
 * An enumeration for room types.
 */
typedef enum { START_ROOM, MID_ROOM, END_ROOM } room_t;

/*
 * A structure that stores the data associated with a room.
 */
struct Room {
    char *name;
    room_t type;
    size_t num_connections;
    struct Room **connections;
};

struct Room *new_room(const char *name, const room_t type);

void del_room(struct Room *room);

void print_room(const struct Room *room);

bool has_connection_available(const struct Room *room);

bool add_connection(struct Room *room1, struct Room *room2);

#endif
