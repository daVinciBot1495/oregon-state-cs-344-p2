#include <stdio.h>
#include "room.h"

const size_t MAX_CONNECTIONS = 6;

/*
 * Constructs a new Room structure with the given name and type.
 */
struct Room *new_room(const char *name, const room_t type) {
    // Create a new Room struct
    struct Room *room = (struct Room*) malloc(sizeof(struct Room));

    // Copy the provided name and type into the struct
    room->name = new_str_from(name);
    room->type = type;

    // Since there is a maximum number of outgoing connections from a room go
    // and create an array of pointers to Room structs of size MAX_CONNECTIONS.
    room->num_connections = 0;
    room->connections = (struct Room**) malloc(MAX_CONNECTIONS *
            sizeof(struct Room*));

    return room;
}

/*
 * Deletes the given Room structure.
 */
void del_room(struct Room *room) {
    free(room->name);
    free(room->connections);
    free(room);
}

/*
 * Prints the given Room structure.
 */
void print_room(const struct Room *room) {
    printf("ROOM NAME: %s\n", room->name);

    size_t i;

    for (i = 0; i < room->num_connections; ++i) {
        printf("CONNECTION %zu: %s\n", i + 1, room->connections[i]->name);
    }

    switch (room->type) {
        case START_ROOM:
            printf("ROOM TYPE: START_ROOM\n");
            break;
        case MID_ROOM:
            printf("ROOM TYPE: MID_ROOM\n");
            break;
        case END_ROOM:
            printf("ROOM TYPE: END_ROOM\n");
            break;
        default:
            printf("ROOM TYPE: UNKNOWN\n");
            break;
    }
}

/*
 * Returns whether or not the given Room structure has any connections
 * available.
 */
bool has_connection_available(const struct Room *room) {
    return room->num_connections < MAX_CONNECTIONS;
}

/*
 * Tries to add a connection between two Room structures. If the connection was
 * added then true is returned. Otherwise, false is returned.
 */
bool add_connection(struct Room *room1, struct Room *room2) {
    if (room1 != room2) {
        // If room1 and room2 point to the same Room then don't add a
        // connection since self connections are not allowed
        return false;
    } else if (has_connection_available(room1) && has_connection_available(room2)) {
        // If both rooms have connections available then connect them
        size_t last_index = room1->num_connections++;
        room1->connections[last_index] = room2;

        last_index = room2->num_connections++;
        room2->connections[last_index] = room1;

        return true;
    } else {
        // If both rooms don't have connections available
        return false;
    }
}
