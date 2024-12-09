#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hotel.h"

Client* clients_head = NULL;
Reservation* reservations_head = NULL;
Room* rooms_head = NULL;

void init_system() {
    // Initialize some rooms
    Room* room;
    for (int i = 1; i <= 10; i++) {
        room = (Room*)malloc(sizeof(Room));
        sprintf(room->room_number, "%d", i);
        room->capacity = (i % 3) + 1;  // 1-3 persons
        room->price_per_night = 100.0 * room->capacity;
        room->is_available = 1;
        room->next = rooms_head;
        rooms_head = room;
    }
    load_data();
}

void add_client() {
    Client* new_client = (Client*)malloc(sizeof(Client));

    printf("Enter client ID: ");
    fgets(new_client->client_id, MAX_STRING, stdin);
    new_client->client_id[strcspn(new_client->client_id, "\n")] = 0;

    printf("Enter name: ");
    fgets(new_client->name, MAX_STRING, stdin);
    new_client->name[strcspn(new_client->name, "\n")] = 0;

    printf("Enter surname: ");
    fgets(new_client->surname, MAX_STRING, stdin);
    new_client->surname[strcspn(new_client->surname, "\n")] = 0;

    printf("Enter address: ");
    fgets(new_client->address, MAX_STRING, stdin);
    new_client->address[strcspn(new_client->address, "\n")] = 0;

    printf("Enter email: ");
    fgets(new_client->email, MAX_STRING, stdin);
    new_client->email[strcspn(new_client->email, "\n")] = 0;

    printf("Enter phone: ");
    fgets(new_client->phone, MAX_STRING, stdin);
    new_client->phone[strcspn(new_client->phone, "\n")] = 0;

    new_client->next = clients_head;
    clients_head = new_client;

    printf("Client added successfully!\n");
}

void add_reservation() {
    Reservation* new_reservation = (Reservation*)malloc(sizeof(Reservation));

    printf("Enter client ID: ");
    fgets(new_reservation->client_id, MAX_STRING, stdin);
    new_reservation->client_id[strcspn(new_reservation->client_id, "\n")] = 0;

    if (!find_client(new_reservation->client_id)) {
        printf("Client not found!\n");
        free(new_reservation);
        return;
    }

    printf("Enter reservation ID: ");
    fgets(new_reservation->reservation_id, MAX_STRING, stdin);
    new_reservation->reservation_id[strcspn(new_reservation->reservation_id, "\n")] = 0;

    printf("Enter room number: ");
    fgets(new_reservation->room_number, MAX_STRING, stdin);
    new_reservation->room_number[strcspn(new_reservation->room_number, "\n")] = 0;

    Room* room = find_room(new_reservation->room_number);
    if (!room || !room->is_available) {
        printf("Room not available!\n");
        free(new_reservation);
        return;
    }

    printf("Enter start date (YYYY-MM-DD): ");
    fgets(new_reservation->date_debut, MAX_STRING, stdin);
    new_reservation->date_debut[strcspn(new_reservation->date_debut, "\n")] = 0;

    printf("Enter end date (YYYY-MM-DD): ");
    fgets(new_reservation->date_fin, MAX_STRING, stdin);
    new_reservation->date_fin[strcspn(new_reservation->date_fin, "\n")] = 0;

    printf("Enter number of persons: ");
    scanf("%d", &new_reservation->nb_personnes);
    getchar();

    if (new_reservation->nb_personnes > room->capacity) {
        printf("Room capacity exceeded!\n");
        free(new_reservation);
        return;
    }

    new_reservation->prix_total = room->price_per_night;  // Per night
    room->is_available = 0;

    new_reservation->next = reservations_head;
    reservations_head = new_reservation;

    printf("Reservation added successfully!\n");
}

void show_all_clients() {
    Client* current = clients_head;
    if (!current) {
        printf("No clients found.\n");
        return;
    }

    printf("\n=== Client List ===\n");
    while (current) {
        printf("ID: %s\n", current->client_id);
        printf("Name: %s %s\n", current->name, current->surname);
        printf("Email: %s\n", current->email);
        printf("Phone: %s\n", current->phone);
        printf("---------------\n");
        current = current->next;
    }
}

void show_all_reservations() {
    Reservation* current = reservations_head;
    if (!current) {
        printf("No reservations found.\n");
        return;
    }

    printf("\n=== Reservation List ===\n");
    while (current) {
        printf("Reservation ID: %s\n", current->reservation_id);
        printf("Client ID: %s\n", current->client_id);
        printf("Room: %s\n", current->room_number);
        printf("Dates: %s to %s\n", current->date_debut, current->date_fin);
        printf("Persons: %d\n", current->nb_personnes);
        printf("Total Price: %.2f\n", current->prix_total);
        printf("---------------\n");
        current = current->next;
    }
}

void show_available_rooms() {
    Room* current = rooms_head;
    printf("\n=== Available Rooms ===\n");
    while (current) {
        if (current->is_available) {
            printf("Room %s (Capacity: %d, Price: %.2f)\n",
                   current->room_number, current->capacity, current->price_per_night);
        }
        current = current->next;
    }
}

void delete_reservation() {
    char reservation_id[MAX_STRING];
    printf("Enter reservation ID to delete: ");
    fgets(reservation_id, MAX_STRING, stdin);
    reservation_id[strcspn(reservation_id, "\n")] = 0;

    Reservation* current = reservations_head;
    Reservation* prev = NULL;

    while (current) {
        if (strcmp(current->reservation_id, reservation_id) == 0) {
            Room* room = find_room(current->room_number);
            if (room) room->is_available = 1;

            if (prev) prev->next = current->next;
            else reservations_head = current->next;

            free(current);
            printf("Reservation deleted successfully!\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Reservation not found!\n");
}

void modify_reservation() {
    char reservation_id[MAX_STRING];
    printf("Enter reservation ID to modify: ");
    fgets(reservation_id, MAX_STRING, stdin);
    reservation_id[strcspn(reservation_id, "\n")] = 0;

    Reservation* current = reservations_head;
    while (current) {
        if (strcmp(current->reservation_id, reservation_id) == 0) {
            printf("Enter new start date (YYYY-MM-DD): ");
            fgets(current->date_debut, MAX_STRING, stdin);
            current->date_debut[strcspn(current->date_debut, "\n")] = 0;

            printf("Enter new end date (YYYY-MM-DD): ");
            fgets(current->date_fin, MAX_STRING, stdin);
            current->date_fin[strcspn(current->date_fin, "\n")] = 0;

            printf("Enter new number of persons: ");
            scanf("%d", &current->nb_personnes);
            getchar();

            Room* room = find_room(current->room_number);
            if (room && current->nb_personnes > room->capacity) {
                printf("Warning: Number of persons exceeds room capacity!\n");
            }

            printf("Reservation modified successfully!\n");
            return;
        }
        current = current->next;
    }
    printf("Reservation not found!\n");
}

Client* find_client(const char* client_id) {
    Client* current = clients_head;
    while (current) {
        if (strcmp(current->client_id, client_id) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

Room* find_room(const char* room_number) {
    Room* current = rooms_head;
    while (current) {
        if (strcmp(current->room_number, room_number) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void save_data() {
    FILE* file = fopen("clients.txt", "w");
    if (file) {
        Client* current_client = clients_head;
        while (current_client) {
            fprintf(file, "%s,%s,%s,%s,%s,%s\n",
                   current_client->client_id,
                   current_client->name,
                   current_client->surname,
                   current_client->address,
                   current_client->email,
                   current_client->phone);
            current_client = current_client->next;
        }
        fclose(file);
    }

    file = fopen("reservations.txt", "w");
    if (file) {
        Reservation* current_res = reservations_head;
        while (current_res) {
            fprintf(file, "%s,%s,%s,%s,%s,%d,%.2f\n",
                   current_res->reservation_id,
                   current_res->client_id,
                   current_res->room_number,
                   current_res->date_debut,
                   current_res->date_fin,
                   current_res->nb_personnes,
                   current_res->prix_total);
            current_res = current_res->next;
        }
        fclose(file);
    }
}

void load_data() {
    FILE* file = fopen("clients.txt", "r");
    if (file) {
        char line[MAX_STRING * 6];
        while (fgets(line, sizeof(line), file)) {
            Client* client = (Client*)malloc(sizeof(Client));
            sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]",
                   client->client_id,
                   client->name,
                   client->surname,
                   client->address,
                   client->email,
                   client->phone);
            client->next = clients_head;
            clients_head = client;
        }
        fclose(file);
    }

    file = fopen("reservations.txt", "r");
    if (file) {
        char line[MAX_STRING * 7];
        while (fgets(line, sizeof(line), file)) {
            Reservation* res = (Reservation*)malloc(sizeof(Reservation));
            sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%f",
                   res->reservation_id,
                   res->client_id,
                   res->room_number,
                   res->date_debut,
                   res->date_fin,
                   &res->nb_personnes,
                   &res->prix_total);
            res->next = reservations_head;
            reservations_head = res;

            Room* room = find_room(res->room_number);
            if (room) room->is_available = 0;
        }
        fclose(file);
    }
}

