#include "room.h"

int main(int argc, char *argv[]) {
    struct Room *eastern_palace = new_room("Eastern Palace", START_ROOM);
    struct Room *house_of_gales = new_room("House of Gales", END_ROOM);

    add_connection(eastern_palace, house_of_gales);

    print_room(eastern_palace);
    print_room(house_of_gales);

    del_room(eastern_palace);
    del_room(house_of_gales);

    return 0;
}
