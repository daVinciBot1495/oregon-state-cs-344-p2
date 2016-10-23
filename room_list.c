#include <stdlib.h>
#include "room_list.h"
#include "CuTest.h"

/*
 * A structure that stores a pointer to a Room and a pointer to the next
 * RoomLink.
 */
struct RoomLink {
    struct Room *room;
    struct RoomLink *next;
};

/*
 * Constructor.
 *
 * @param room A pointer to a Room.
 * @return A pointer to a new RoomLink.
 */
struct RoomLink *new_room_link(struct Room *room) {
    struct RoomLink *link = (struct RoomLink*) malloc(sizeof(struct RoomLink));

    link->room = room;
    link->next = NULL;

    return link;
}

/*
 * Deletes the given RoomLink and returns a pointer to the next RoomLink.
 *
 * @param room_link A pointer to a RoomLink.
 * @return A pointer to the next RoomLink.
 */
struct RoomLink *del_room_link(struct RoomLink *room_link) {
    struct RoomLink *next = room_link->next;
    free(room_link);
    return next;
}

/*
 * Constructor.
 *
 * @return A pointer to a new RoomList.
 */
struct RoomList *new_room_list() {
    struct RoomList *room_list = (struct RoomList*) malloc(
            sizeof(struct RoomList));

    room_list->size = 0;
    room_list->head = NULL;
    room_list->tail = NULL;

    return room_list;
}

/*
 * Deletes the given RoomList.
 *
 * @param room_list A pointer to a RoomList.
 */
void del_room_list(struct RoomList *room_list) {
    struct RoomLink *curr = room_list->head;

    while (curr != NULL) {
        curr = del_room_link(curr);
    }

    free(room_list);
}

/*
 * Adds a Room to the given RoomList.
 *
 * @param room_list A pointer to a RoomList.
 * @param room A pointer to a Room.
 */
void add_room(struct RoomList *room_list, struct Room *room) {
    struct RoomLink *link = new_room_link(room);
   
    if (room_list->head == NULL && room_list->tail == NULL) {
        // If the list is empty set the head and the tail to link
        room_list->head = link;
        room_list->tail = link;
    } else {
        // Else set the current tail's next to link and update tail to link
        room_list->tail->next = link;
        room_list->tail = link;
    }

    room_list->size++;
}

////////////////////////////////////////////////////////////////////////////////
// Unit tests
////////////////////////////////////////////////////////////////////////////////

void new_room_link_should_create_new_room_link(CuTest *tc) {
    // Given
    const char *name = "name";
    const room_t type = START_ROOM;
    struct Room *room = new_room(name, type);

    // When
    struct RoomLink *link = new_room_link(room);

    // Then
    CuAssertPtrNotNull(tc, link);
    CuAssertPtrEquals(tc, room, link->room);
    CuAssertPtrEquals(tc, NULL, link->next);

    // Clean up
    del_room(room);
    del_room_link(link);
}

void del_room_link_should_return_next(CuTest *tc) {
    // Given
    const char *name1 = "name1";
    const room_t type1 = START_ROOM;
    struct Room *room1 = new_room(name1, type1);

    const char *name2 = "name2";
    const room_t type2 = END_ROOM;
    struct Room *room2 = new_room(name2, type2);

    struct RoomLink *link1 = new_room_link(room1);
    struct RoomLink *link2 = new_room_link(room2);

    link1->next = link2;

    // When
    struct RoomLink *next = del_room_link(link1);

    // Then
    CuAssertPtrEquals(tc, link2, next);

    // Clean up
    del_room(room1);
    del_room(room2);
    del_room_link(link2);
}

void new_room_list_should_return_new_room_list(CuTest *tc) {
    // When
    struct RoomList *list = new_room_list();

    // Then
    CuAssertIntEquals(tc, 0, list->size);
    CuAssertPtrEquals(tc, NULL, list->head);
    CuAssertPtrEquals(tc, NULL, list->tail);

    // Clean up
    del_room_list(list);
}

void add_room_should_add_to_list(CuTest *tc) {
    // Given
    const char *name1 = "name1";
    const room_t type1 = START_ROOM;
    struct Room *room1 = new_room(name1, type1);

    const char *name2 = "name2";
    const room_t type2 = END_ROOM;
    struct Room *room2 = new_room(name2, type2);

    struct RoomList *list = new_room_list();

    // When
    add_room(list, room1);
    add_room(list, room2);

    // Then
    CuAssertIntEquals(tc, 2, list->size);
    CuAssertPtrEquals(tc, room1, list->head->room);
    CuAssertPtrEquals(tc, room2, list->tail->room);
    CuAssertPtrEquals(tc, list->tail, list->head->next);

    // Clean up
    del_room_list(list);
    del_room(room1);
    del_room(room2);
}

CuSuite *get_room_list_suite() {
    CuSuite *suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, new_room_link_should_create_new_room_link);
    SUITE_ADD_TEST(suite, del_room_link_should_return_next);
    SUITE_ADD_TEST(suite, new_room_list_should_return_new_room_list);
    SUITE_ADD_TEST(suite, add_room_should_add_to_list);

    return suite;
}
