#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EVENTS 50

// Structure to store events
typedef struct {
    int day;
    int month;
    int year;
    char event[50];
} Event;

Event events[MAX_EVENTS];
int eventCount = 0;

// Function to check if a year is a leap year
int isLeapYear(int year) {
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

// Function to get the number of days in a month
int getDaysInMonth(int year, int month) {
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year))
        return 29;
    return daysInMonth[month - 1];
}

// Function to get the starting day of a given month and year
int getStartDay(int year, int month) {
    int day = 1, y;
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    y = year;
    if (month < 3) y -= 1;
    return (y + y/4 - y/100 + y/400 + t[month-1] + day) % 7;
}

// Function to add an event
void addEvent(int day, int month, int year, char event[]) {
    if (eventCount < MAX_EVENTS) {
        events[eventCount].day = day;
        events[eventCount].month = month;
        events[eventCount].year = year;
        strcpy(events[eventCount].event, event);
        eventCount++;
    }
}

// Function to display events for a given month
void displayEvents(int year, int month) {
    printf("\nEvents for %d-%02d:\n", year, month);
    int found = 0;
    for (int i = 0; i < eventCount; i++) {
        if (events[i].month == month && events[i].year <= year) {
            printf("  - %02d: %s\n", events[i].day, events[i].event);
            found = 1;
        }
    }
    if (!found) {
        printf("  No events found.\n");
    }
}

// Function to print the calendar with events highlighted
void printCalendar(int year, int month) {
    int days = getDaysInMonth(year, month);
    int startDay = getStartDay(year, month);
    
    printf("\n============================================\n");
    printf("           Calendar for %d - %02d\n", year, month);
    printf("============================================\n");
    printf("  Sun   Mon   Tue   Wed   Thu   Fri   Sat\n");
    printf("--------------------------------------------\n");
    
    int day, position = startDay;
    for (int i = 0; i < startDay; i++)
        printf("      ");
    
    for (day = 1; day <= days; day++) {
        int isEvent = 0;
        for (int i = 0; i < eventCount; i++) {
            if (events[i].day == day && events[i].month == month && events[i].year <= year) {
                printf(" \033[1;31m%4d\033[0m ", day); // Red color for event dates
                isEvent = 1;
                break;
            }
        }
        if (!isEvent)
            printf("%4d  ", day);
        
        position++;
        if (position % 7 == 0)
            printf("\n");
    }
    printf("\n============================================\n");
    displayEvents(year, month);
}

// Function to print the full year calendar
void printYearCalendar(int year) {
    for (int month = 1; month <= 12; month++) {
        printCalendar(year, month);
    }
}

int main() {
    int year, month, choice;
    while (1) {
        printf("\n============================================\n");
        printf("        Welcome to the Calendar Generator  \n");
        printf("============================================\n");
        
        printf(" Choose an option:\n");
        printf(" 1. View a specific month\n");
        printf(" 2. View the full year calendar\n");
        printf(" 3. Exit\n");
        printf(" Enter your choice: ");
        scanf("%d", &choice);
        
        if (choice == 1) {
            printf(" Enter year (YYYY): ");
            scanf("%d", &year);
            
            printf(" Enter month (MM): ");
            scanf("%d", &month);
            if (month < 1 || month > 12) {
                printf(" Invalid month! Please enter a value between 1 and 12.\n");
                continue;
            }
            printCalendar(year, month);
        } else if (choice == 2) {
            printf(" Enter year (YYYY): ");
            scanf("%d", &year);
            printYearCalendar(year);
        } else if (choice == 3) {
            break;
        } else {
            printf(" Invalid choice!\n");
            continue;
        }
        
        while (1) {
            printf(" Do you want to add an event? (1 for Yes, 0 for No): ");
            int addEventChoice;
            scanf("%d", &addEventChoice);
            
            if (addEventChoice == 0)
                break;
            
            if (addEventChoice == 1 && eventCount < MAX_EVENTS) {
                printf(" Enter event month (MM): ");
                scanf("%d", &month);
                if (month < 1 || month > 12) {
                    printf(" Invalid month! Enter a value between 1 and 12.\n");
                    continue;
                }
                
                int eventDay;
                char eventDesc[50];
                printf(" Enter event day: ");
                scanf("%d", &eventDay);
                
                if (eventDay < 1 || eventDay > getDaysInMonth(year, month)) {
                    printf(" Invalid day! Enter a valid day for this month.\n");
                    continue;
                }
                
                getchar(); // Clear newline from buffer
                printf(" Enter event description: ");
                fgets(eventDesc, sizeof(eventDesc), stdin);
                eventDesc[strcspn(eventDesc, "\n")] = 0; // Remove newline character
                
                addEvent(eventDay, month, year, eventDesc);
                printf(" Event added!\n");
            } else {
                printf(" Invalid choice!\n");
            }
        }
    }
    return 0;
}
