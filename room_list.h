#ifndef ROOM_LIST_H
#define ROOM_LIST_H

#include <stddef.h>
#include "room.h"

/*
 * A structure that stores a pointer to a room and a link to the next room.
 */
struct RoomLink {
    struct Room *room;
    struct RoomLink *next;
};

struct RoomLink *new_room_link(struct Room *room);
struct RoomLink *del_room_link(struct RoomLink *room_link);

/*
 * A structure that stores a linked list of rooms.
 */
struct RoomList {
    size_t size;
    struct RoomLink *head;
    struct RoomLink *tail;
};

struct RoomList *new_room_list();
void del_room_list(struct RoomList *room_list);
void add_room(struct RoomList *room_list, struct Room *room);

#endif
