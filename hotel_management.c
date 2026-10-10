#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "hotel_data.dat"

struct Room {
    int roomNumber;
    char guestName[50];
    char phone[20];
    int days;
    float rate;
    int isOccupied;
};

void displayMenu();
void checkIn();
void viewRooms();
void checkOut();
void calculateBill();

int main() {
    int choice;

    while (1) {
        displayMenu();
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while(getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                checkIn();
                break;
            case 2:
                viewRooms();
                break;
            case 3:
                checkOut();
                break;
            case 4:
                calculateBill();
                break;
            case 5:
                printf("\nThank you for using Hotel Management System. Goodbye!\n");
                exit(0);
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }

    return 0;
}

void displayMenu() {
    printf("\nHotel Management System Menu\n");
    printf("1. Check-In (Book Room)\n");
    printf("2. View All Rooms Status\n");
    printf("3. Check-Out\n");
    printf("4. Calculate Bill\n");
    printf("5. Exit\n");
}

void checkIn() {
    struct Room r;
    FILE *file = fopen(FILENAME, "r+b");
    
    if (file == NULL) {
        file = fopen(FILENAME, "w+b");
        if (file == NULL) {
            printf("Error opening file!\n");
            return;
        }
    }

    printf("\nEnter Room Number (101 - 110): ");
    scanf("%d", &r.roomNumber);

    int found = 0;
    struct Room temp;
    while (fread(&temp, sizeof(struct Room), 1, file)) {
        if (temp.roomNumber == r.roomNumber && temp.isOccupied == 1) {
            found = 1;
            break;
        }
    }

    if (found) {
        printf("Room %d is already occupied.\n", r.roomNumber);
        fclose(file);
        return;
    }

    printf("Enter Guest Name: ");
    scanf(" %[^\n]s", r.guestName);
    printf("Enter Phone Number: ");
    scanf("%s", r.phone);
    printf("Enter Number of Days of Stay: ");
    scanf("%d", &r.days);
    printf("Enter Daily Room Rate: ");
    scanf("%f", &r.rate);
    r.isOccupied = 1;

    fseek(file, 0, SEEK_END);
    fwrite(&r, sizeof(struct Room), 1, file);
    fclose(file);

    printf("Check-in successful for %s in Room %d.\n", r.guestName, r.roomNumber);
}

void viewRooms() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("\nNo room records found yet.\n");
        return;
    }

    struct Room r;
    printf("\nHotel Room Status:\n");

    while (fread(&r, sizeof(struct Room), 1, file)) {
        if (r.isOccupied) {
            printf("Room Number: %d | Guest Name: %s | Phone: %s | Status: Occupied\n", r.roomNumber, r.guestName, r.phone);
        }
    }
    fclose(file);
}

void calculateBill() {
    int roomNo;
    printf("\nEnter Room Number to calculate bill: ");
    scanf("%d", &roomNo);

    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("No records found!\n");
        return;
    }

    struct Room r;
    int found = 0;
    while (fread(&r, sizeof(struct Room), 1, file)) {
        if (r.roomNumber == roomNo && r.isOccupied == 1) {
            float totalBill = r.days * r.rate;
            printf("\nBill Details:\n");
            printf("Guest Name: %s\n", r.guestName);
            printf("Room Number: %d\n", r.roomNumber);
            printf("Days Stayed: %d\n", r.days);
            printf("Daily Rate: %.2f\n", r.rate);
            printf("Total Bill: BDT %.2f\n", totalBill);
            found = 1;
            break;
        }
    }
    fclose(file);

    if (!found) {
        printf("No active booking found for Room %d.\n", roomNo);
    }
}

void checkOut() {
    int roomNo;
    printf("\nEnter Room Number for Check-Out: ");
    scanf("%d", &roomNo);

    FILE *file = fopen(FILENAME, "r+b");
    if (file == NULL) {
        printf("No records found!\n");
        return;
    }

    struct Room r;
    int found = 0;
    while (fread(&r, sizeof(struct Room), 1, file)) {
        if (r.roomNumber == roomNo && r.isOccupied == 1) {
            r.isOccupied = 0;
            fseek(file, -sizeof(struct Room), SEEK_CUR);
            fwrite(&r, sizeof(struct Room), 1, file);
            found = 1;
            break;
        }
    }
    fclose(file);

    if (found) {
        printf("Check-out successful. Room %d is now vacant.\n", roomNo);
    } else {
        printf("Room %d is not currently occupied or does not exist.\n", roomNo);
    }
}
