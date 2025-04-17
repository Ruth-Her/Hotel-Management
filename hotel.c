#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROOMS 100

typedef struct {
    int room_number;
    int capacity;
    int is_booked;
    float price_per_night;
} Room;

Room rooms[MAX_ROOMS];
int room_count = 0;

// Function prototypes
void load_rooms();
void showAvailableRooms();
void make_reservation();

void load_rooms() {
    FILE *file = fopen("rooms.csv", "r");
    if (!file) {
        printf("Error: Could not open rooms.csv\n");
        return;
    }

    while (fscanf(file, "%d,%d,%d,%f\n", 
            &rooms[room_count].room_number, 
            &rooms[room_count].capacity,
            &rooms[room_count].is_booked,
            &rooms[room_count].price_per_night) != EOF) {
        room_count++;
    }
    fclose(file);
}

void showAvailableRooms() {
    printf("\nAvailable Rooms:\n");
    printf("Room\tCapacity\tPrice\n");

    for (int i = 0; i < room_count; i++) {
        if (rooms[i].is_booked == 0) {
            printf("%d\t%d\t\t%.2f\n", 
                   rooms[i].room_number, 
                   rooms[i].capacity, 
                   rooms[i].price_per_night);
        }
    }
}

void make_reservation() {
    int room_number, nights;
    char guest_name[100];
    FILE *rooms_file = fopen("rooms.csv", "r");
    FILE *temp_file = fopen("temp.csv", "w");
    FILE *res_file = fopen("reservations.csv", "a");

    if (!rooms_file || !temp_file || !res_file) {
        printf("Error accessing files!\n");
        return;
    }

    printf("Enter your name: ");
    scanf(" %[^\n]", guest_name);
    printf("Enter room number: ");
    scanf("%d", &room_number);
    printf("Enter number of nights: ");
    scanf("%d", &nights);

    Room room;
    int found = 0;

    while (fscanf(rooms_file, "%d,%d,%d,%f\n", 
                  &room.room_number, 
                  &room.capacity, 
                  &room.is_booked, 
                  &room.price_per_night) != EOF) {

        if (room.room_number == room_number && room.is_booked == 0) {
            room.is_booked = 1;
            float total = nights * room.price_per_night;
            fprintf(res_file, "%s,%d,%d,%.2f\n", guest_name, room_number, nights, total);
            printf("Reservation complete! Total bill: KES %.2f\n", total);
            found = 1;
        }

        fprintf(temp_file, "%d,%d,%d,%.2f\n", 
                room.room_number, 
                room.capacity, 
                room.is_booked, 
                room.price_per_night);
    }

    fclose(rooms_file);
    fclose(temp_file);
    fclose(res_file);

    remove("rooms.csv");
    rename("temp.csv", "rooms.csv");

    if (!found) {
        printf("Room not available or doesn't exist.\n");
    }
}

int main() {
    int choice;
    load_rooms();  // Load room data at the start

    do {
        printf("\n--- Hotel Management System ---\n");
        printf("1. Show Available Rooms\n");
        printf("2. Make Reservation\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                showAvailableRooms();
                break;
            case 2:
                make_reservation();
                break;
            case 0:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 0);

    return 0;
}
