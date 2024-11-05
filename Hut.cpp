#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <cctype> 
#include <string>
#include <sstream> 

class Seat {
public:
    bool booked;
    int x, y;

    Seat(int xPos, int yPos) : booked(false), x(xPos), y(yPos) {}

    void draw() {
        setcolor(WHITE);
        rectangle(x - 20, y - 20, x + 20, y + 20);  // Draw a 40x40 seat
        if (booked) {
            setfillstyle(SOLID_FILL, RED);
        } else {
            setfillstyle(SOLID_FILL, GREEN);
        }
        floodfill(x, y, WHITE);
    }
};

class CinemaHall {
private:
    std::vector<Seat> seats;

public:
    CinemaHall() {
        // Rows A and B (22 seats per row)
        int colsA_B = 22;
        for (int i = 0; i < 2; ++i) { // For rows A and B
            for (int j = 0; j < colsA_B; ++j) {
                seats.push_back(Seat(80 + j * 40, 100 + i * 60));  // Spacing between seats in rows A and B
            }
        }

        // Rows C to G (16 seats per row)
        int colsC_G = 16;
        for (int i = 2; i < 7; ++i) { // For rows C to G
            for (int j = 0; j < colsC_G; ++j) {
                int xPos;
                if (j < 2) {
                    xPos = 80 + j * 40; // Left block (columns 1-2)
                } else if (j < 14) {
                    xPos = 280 + (j - 2) * 40; // Middle block (columns 3-14)
                } else {
                    xPos = 880 + (j - 14) * 40; // Right block (columns 15-16)
                }
                seats.push_back(Seat(xPos, 220 + (i - 2) * 60));  // Adjust Y position for rows C-G
            }
        }
    }

    void drawSeats() {
        // Draw row labels (A to G)
        for (int i = 0; i < 7; ++i) {
            char rowLabel = 'A' + i;
            outtextxy(50, 100 + i * 60, &rowLabel);
        }

        // Draw column headers for rows A and B (1 to 22)
        for (int j = 0; j < 22; ++j) {
            char colLabel[3];
            sprintf(colLabel, "%d", j + 1);
            outtextxy(80 + j * 40, 70, colLabel); // Adjust X position
        }

        // Draw seats for rows A and B
        for (int i = 0; i < 2 * 22; ++i) {
            seats[i].draw();
        }

        // Draw column headers for rows C to G (1 to 16, with gaps)
        for (int j = 0; j < 16; ++j) {
            char colLabel[3];
            sprintf(colLabel, "%d", j + 1);
            int xPos;
            if (j < 2) {
                xPos = 80 + j * 40; // Left block
            } else if (j < 14) {
                xPos = 280 + (j - 2) * 40; // Middle block
            } else {
                xPos = 880 + (j - 14) * 40; // Right block
            }
            outtextxy(xPos, 190, colLabel); // Column header for rows C to G
        }

        // Draw seats for rows C to G
        for (int i = 2 * 22; i < seats.size(); ++i) {
            seats[i].draw();
        }

        // Draw the screen below row G
        drawScreen();
    }

    void drawScreen() {
        // Position the screen in front of row G
        setcolor(WHITE);
        rectangle(100, 530, 900, 550);  // The screen size
        outtextxy(500 - textwidth("SCREEN") / 2, 510, "SCREEN");  // Center the text horizontally
    }

    void bookSeat(int index) {
        if (!seats[index].booked) {
            seats[index].booked = true;
            seats[index].draw();
            std::cout << "Ticket successfully booked for seat " << getSeatLabel(index) << "!\n";
        } else {
            std::cout << "Seat " << getSeatLabel(index) << " is already booked. Please choose another seat.\n";
        }
    }

    void cancelSeat(int index) {
        if (seats[index].booked) {
            seats[index].booked = false;
            seats[index].draw();
            std::cout << "Ticket for seat " << getSeatLabel(index) << " successfully canceled.\n";
        } else {
            std::cout << "Seat " << getSeatLabel(index) << " is not booked, cannot cancel.\n";
        }
    }

    int getSeatIndex(char row, int col) {
        row = std::toupper(row);
        int rowIndex = row - 'A';
        if (rowIndex < 0 || rowIndex >= 7 || col < 1 || (rowIndex < 2 && col > 22) || (rowIndex >= 2 && col > 16)) {
            return -1;
        }
        if (rowIndex < 2) {
            return rowIndex * 22 + (col - 1);
        }
        return 44 + (rowIndex - 2) * 16 + (col - 1);
    }

    std::string getSeatLabel(int index) {
        char label[4]; // Enough space for a row label and column number
        if (index < 44) {
            int row = index / 22;
            int col = index % 22;
            sprintf(label, "%c%d", 'A' + row, col + 1);
        } else {
            int row = (index - 44) / 16 + 2;
            int col = (index - 44) % 16;
            sprintf(label, "%c%d", 'A' + row, col + 1);
        }
        return std::string(label);
    }

    void showSeatingArrangement() {
        std::cout << "Seating Arrangement:\n";
        for (int i = 0; i < 7; ++i) {
            for (int j = 0; j < (i < 2 ? 22 : 16); ++j) {
                std::cout << (seats[(i < 2 ? i * 22 + j : 44 + (i - 2) * 16 + j)].booked ? "X" : "O") << " ";
            }
            std::cout << std::endl;
        }
    }

    bool isSeatBooked(int index) {
        return seats[index].booked;
    }
};

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, ""); // Initializes the graphics system
    initwindow(1200, 800); // Adjust the window size to fit the seats

    CinemaHall hall;
    hall.drawSeats();
    
    hall.showSeatingArrangement();

    int choice;
    do {
        std::cout << "\nMenu:\n1. Book a ticket\n2. Cancel a ticket\n3. View seating arrangement\n0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (choice == 1) {
            int numberOfTickets;
            std::cout << "Enter the number of tickets you want to book: ";
            std::cin >> numberOfTickets;

            for (int i = 0; i < numberOfTickets; ++i) {
                char row;
                int col;
                std::cout << "Select row (A to G) and column (1 to " << (row <= 'B' ? 22 : 16) << ") for ticket " << (i + 1) << ": ";
                std::cin >> row >> col;

                int seatIndex = hall.getSeatIndex(row, col);
                if (seatIndex != -1) {
                    if (!hall.isSeatBooked(seatIndex)) {
                        hall.bookSeat(seatIndex);
                    } else {
                        std::cout << "Seat " << hall.getSeatLabel(seatIndex) << " is already booked. Please choose another seat.\n";
                        i--; // Repeat this iteration
                    }
                } else {
                    std::cout << "Invalid seat selection. Please try again.\n";
                    i--; // Repeat this iteration
                }
            }
        } else if (choice == 2) {
            char row;
            int col;
            std::cout << "Select row (A to G) and column (1 to " << (row <= 'B' ? 22 : 16) << ") to cancel booking: ";
            std::cin >> row >> col;

            int seatIndex = hall.getSeatIndex(row, col);
            if (seatIndex != -1) {
                hall.cancelSeat(seatIndex);
            } else {
                std::cout << "Invalid seat selection. Please try again.\n";
            }
        } else if (choice == 3) {
            hall.showSeatingArrangement();
        }
    } while (choice != 0);


    getch();
    closegraph();
    return 0;
}

