#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "room.h"
#include "CuTest.h"

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
    if (room1 == room2) {
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

/*
 * Finds the connection of a given room by name. If the connection cannot bei
 * found NULL is returned.
 */
struct Room *find_connection(const struct Room *room, const char *name) {
    int i = 0;

    for (i = 0; i < room->num_connections; ++i) {
        if (strcmp(name, room->connections[i]->name) == 0) {
            return room->connections[i];
        }
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
// Unit tests
////////////////////////////////////////////////////////////////////////////////

void new_room_should_create_new_room(CuTest *tc) {
    // Given
    const char *name = "name";
    const room_t type = START_ROOM;

    // When
    struct Room *room = new_room(name, type);

    // Then
    CuAssertPtrNotNull(tc, room);
    CuAssertStrEquals(tc, name, room->name);
    CuAssertIntEquals(tc, type, room->type);
    CuAssertIntEquals(tc, 0, room->num_connections);
    CuAssertPtrNotNull(tc, room->connections);

    // Clean up
    del_room(room);
}

void has_connection_available_when_num_connections_less_than_max_should_return_true(CuTest *tc) {
    // Given
    const char *name = "name";
    const room_t type = START_ROOM;
    struct Room *room = new_room(name, type);

    // When
    const bool actual = has_connection_available(room);

    // Then
    CuAssertIntEquals(tc, true, actual);

    // Clean up
    del_room(room);
}

void has_connection_available_when_num_connections_equals_max_should_return_false(CuTest *tc) {
    // Given
    const char *name = "name";
    const room_t type = START_ROOM;
    struct Room *room = new_room(name, type);
    room->num_connections = MAX_CONNECTIONS;

    // When
    const bool actual = has_connection_available(room);

    // Then
    CuAssertIntEquals(tc, false, actual);

    // Clean up
    del_room(room);
}

void has_connection_available_when_num_connections_greater_than_max_should_return_false(CuTest *tc) {
    // Given
    const char *name = "name";
    const room_t type = START_ROOM;
    struct Room *room = new_room(name, type);
    room->num_connections = MAX_CONNECTIONS + 1;

    // When
    const bool actual = has_connection_available(room);

    // Then
    CuAssertIntEquals(tc, false, actual);

    // Clean up
    del_room(room);
}

void add_connection_when_same_rooms_should_not_add_connection(CuTest *tc) {
    // Given
    const char *name = "name";
    const room_t type = START_ROOM;
    struct Room *room = new_room(name, type);

    // When
    const bool actual = add_connection(room, room);

    // Then
    CuAssertIntEquals(tc, false, actual);
    CuAssertIntEquals(tc, 0, room->num_connections);

    // Clean up
    del_room(room);
}

void add_connection_when_different_rooms_should_add_connection(CuTest *tc) {
    // Given
    const char *name1 = "name1";
    const room_t type1 = START_ROOM;
    struct Room *room1 = new_room(name1, type1);

    const char *name2 = "name2";
    const room_t type2 = END_ROOM;
    struct Room *room2 = new_room(name2, type2);

    // When
    const bool actual = add_connection(room1, room2);

    // Then
    CuAssertIntEquals(tc, true, actual);
    CuAssertIntEquals(tc, 1, room1->num_connections);
    CuAssertIntEquals(tc, 1, room2->num_connections);
    CuAssertPtrEquals(tc, room2, room1->connections[0]);
    CuAssertPtrEquals(tc, room1, room2->connections[0]);

    // Clean up
    del_room(room1);
    del_room(room2);
}

void add_connection_when_room1_has_connections_but_room2_doesnt_should_not_add_connection(CuTest *tc) {
    // Given
    const char *name1 = "name1";
    const room_t type1 = START_ROOM;
    struct Room *room1 = new_room(name1, type1);

    const char *name2 = "name2";
    const room_t type2 = END_ROOM;
    struct Room *room2 = new_room(name2, type2);
    room2->num_connections = MAX_CONNECTIONS;

    // When
    const bool actual = add_connection(room1, room2);

    // Then
    CuAssertIntEquals(tc, false, actual);
    CuAssertIntEquals(tc, 0, room1->num_connections);
    CuAssertIntEquals(tc, MAX_CONNECTIONS, room2->num_connections);

    // Clean up
    del_room(room1);
    del_room(room2);
}

void add_connection_when_room2_has_connections_but_room1_doesnt_should_not_add_connection(CuTest *tc) {
    // Given
    const char *name1 = "name1";
    const room_t type1 = START_ROOM;
    struct Room *room1 = new_room(name1, type1);
    room1->num_connections = MAX_CONNECTIONS;

    const char *name2 = "name2";
    const room_t type2 = END_ROOM;
    struct Room *room2 = new_room(name2, type2);

    // When
    const bool actual = add_connection(room1, room2);

    // Then
    CuAssertIntEquals(tc, false, actual);
    CuAssertIntEquals(tc, MAX_CONNECTIONS, room1->num_connections);
    CuAssertIntEquals(tc, 0, room2->num_connections);

    // Clean up
    del_room(room1);
    del_room(room2);
}

void find_connection_when_connection_doesnt_exist_should_return_null(CuTest *tc) {
    // Given
    const char *name1 = "name1";
    const room_t type1 = START_ROOM;
    struct Room *room1 = new_room(name1, type1);

    const char *name2 = "name2";
    const room_t type2 = END_ROOM;
    struct Room *room2 = new_room(name2, type2);

    // When
    struct Room *actual = find_connection(room1, room2->name);

    // Then
    CuAssertPtrEquals(tc, NULL, actual);

    // Clean up
    del_room(room1);
    del_room(room2);
}

void find_connection_when_connection_exists_should_return_connection(CuTest *tc) {
    // Given
    const char *name1 = "name1";
    const room_t type1 = START_ROOM;
    struct Room *room1 = new_room(name1, type1);

    const char *name2 = "name2";
    const room_t type2 = END_ROOM;
    struct Room *room2 = new_room(name2, type2);

    add_connection(room1, room2);

    // When
    struct Room *actual = find_connection(room1, room2->name);

    // Then
    CuAssertPtrEquals(tc, room2, actual);

    // Clean up
    del_room(room1);
    del_room(room2);
}

CuSuite *get_room_suite() {
    CuSuite *suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, new_room_should_create_new_room);
    SUITE_ADD_TEST(suite, has_connection_available_when_num_connections_less_than_max_should_return_true);
    SUITE_ADD_TEST(suite, has_connection_available_when_num_connections_equals_max_should_return_false);
    SUITE_ADD_TEST(suite, has_connection_available_when_num_connections_greater_than_max_should_return_false);
    SUITE_ADD_TEST(suite, add_connection_when_same_rooms_should_not_add_connection);
    SUITE_ADD_TEST(suite, add_connection_when_different_rooms_should_add_connection);
    SUITE_ADD_TEST(suite, add_connection_when_room1_has_connections_but_room2_doesnt_should_not_add_connection);
    SUITE_ADD_TEST(suite, add_connection_when_room2_has_connections_but_room1_doesnt_should_not_add_connection);
    SUITE_ADD_TEST(suite, find_connection_when_connection_doesnt_exist_should_return_null);
    SUITE_ADD_TEST(suite, find_connection_when_connection_exists_should_return_connection);

    return suite;
}
