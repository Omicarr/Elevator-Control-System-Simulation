#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_FLOORS 10

typedef struct
{
    int floor;
    float timestamp;
} Request;

typedef struct
{
    Request requests[MAX_FLOORS];
    int count;
} PriorityDeque;

PriorityDeque elevatorDeque;
int currentFloor = 0;
int destinationFloor = 0;
int highestRequestedFloorDown = 0;
int lastVisitedFloor = -1; // Added variable to track the last visited floor

// Function declaration for compareIntegers
int compareIntegers(const void *a, const void *b);

void initPriorityDeque(PriorityDeque *priorityDeque)
{
    priorityDeque->count = 0;
}

void pushPriority(PriorityDeque *priorityDeque, Request req)
{
    if (priorityDeque->count >= MAX_FLOORS)
    {
        printf("Priority Deque is full. Request at floor %d cannot be added.\n", req.floor);
        return;
    }

    priorityDeque->requests[priorityDeque->count++] = req;
    qsort(priorityDeque->requests, priorityDeque->count, sizeof(Request), compareIntegers);
}

void popPriority(PriorityDeque *priorityDeque)
{
    if (priorityDeque->count == 0)
    {
        printf("Priority Deque is empty. No request to dequeue.\n");
        return;
    }
    //qsort()
    qsort(priorityDeque->requests + 1, priorityDeque->count - 1, sizeof(Request), compareIntegers);
    priorityDeque->count--;
}


void goToFloor(int floor)
{
    int travelTime = 1* abs(floor - currentFloor);
    printf("Moving from floor %d to floor %d...\n", currentFloor, floor);
    sleep(travelTime); //
    currentFloor = floor;
    lastVisitedFloor = floor; // Update last visited floor
    printf("Arrived at floor %d\n", floor);
}

int compareIntegers(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

void processRequestsUp(PriorityDeque *priorityDeque)
{
    qsort(priorityDeque->requests, priorityDeque->count, sizeof(Request), compareIntegers);

    for (int i = 0; i < priorityDeque->count; i++)
    {
        destinationFloor = priorityDeque->requests[i].floor;
        if (destinationFloor != lastVisitedFloor)
        {
            goToFloor(destinationFloor);
            // No displayPriorityStatus here
        }
    }
}

bool isFloorVisited(int floor, int *visitedFloors, int visitedCount)
{
    for (int i = 0; i < visitedCount; i++)
    {
        if (visitedFloors[i] == floor)
        {
            return true; // Floor has been visited
        }
    }
    return false; // Floor has not been visited
}

void processRequestsDownHoldHigher(PriorityDeque *priorityDeque)
{
    qsort(priorityDeque->requests, priorityDeque->count, sizeof(Request), compareIntegers);

    int visitedFloors[MAX_FLOORS];
    int visitedCount = 0;

    for (int i = priorityDeque->count - 1; i >= 0; i--)   //descending order 
    {
        destinationFloor = priorityDeque->requests[i].floor;

        // Skip the floor if it has already been visited
        if (isFloorVisited(destinationFloor, visitedFloors, visitedCount))
        {
            continue;
        }

        goToFloor(destinationFloor);

        // Update visitedFloors
        visitedFloors[visitedCount++] = destinationFloor;

        // Update highestRequestedFloorDown only if the current floor is higher
        if (destinationFloor > highestRequestedFloorDown)
        {
            highestRequestedFloorDown = destinationFloor;
        }
    }

    // Go to the highest requested floor without stopping at intermediate floors
    destinationFloor = highestRequestedFloorDown;
    if (!isFloorVisited(destinationFloor, visitedFloors, visitedCount))
    {
        goToFloor(destinationFloor);
    }
}

void processRequestsMenu() //handling the movement of the elevator based on the requests and inputs 
{
    printf("Enter the starting floor of the elevator: ");
    scanf("%d", &currentFloor);

    char moreRequests = 'y';

    while (moreRequests == 'y' || moreRequests == 'Y')
    {
        printf("Select direction:\n");
        printf("1. Go Up\n");
        printf("2. Go Down\n");
        printf("Enter your choice: ");

        int choice;
        scanf("%d", &choice);
  
        int direction = (choice == 1) ? 1 : -1;  //if else condition 

        if ((direction == 1 && currentFloor == MAX_FLOORS - 1) ||  //cant give another request after the array is full or  or down from ground 
            (direction == -1 && currentFloor == 0))
        {
            printf("Cannot go in that direction from the current floor. Please choose another direction.\n");
            continue;
        }

        PriorityDeque requestPriorityDeque; //to store and prioritise the entered requests 
        initPriorityDeque(&requestPriorityDeque);

        while (1)
        {
            int floor;
            printf("Enter the floor you want to go to (-1 to stop): ");
            scanf("%d", &floor);

            if (floor == -1)
            {
                break;
            }

            Request req = {floor, 0};  //request object 
            pushPriority(&requestPriorityDeque, req);
        }

        if (requestPriorityDeque.count > 0)
        {
            if (direction == 1)
            {
                processRequestsUp(&requestPriorityDeque);
            }
            else
            {
                processRequestsDownHoldHigher(&requestPriorityDeque);
            }
        }

        destinationFloor = -1;
        lastVisitedFloor = -1; // Reset last visited floor

        printf("Do you want to continue? (y/n): ");
        scanf(" %c", &moreRequests);
    }
}

int main()
{
    initPriorityDeque(&elevatorDeque);

    processRequestsMenu();

    return 0;
}