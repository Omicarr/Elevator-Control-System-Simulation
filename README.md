Elevator Control System Simulation

This project is a C-based simulation of an elevator control system designed for a building with up to 10 floors. It models how elevators handle floor requests in both upward and downward directions, ensuring efficient movement by prioritizing requests.

 🚀 Features

- Handles requests to move **up** or **down**.
- Uses a **priority deque** to sort floor requests automatically.
- Simulates elevator movement with sleep delays based on floor distance.
- Prevents duplicate or invalid floor requests.
- Optimizes downward requests by holding for the highest requested floor.

🧠 How It Works

1. User inputs the **current floor** of the elevator.
2. User chooses the direction of travel:
   - `1` for Up
   - `2` for Down
3. User enters one or more **target floors**.
4. The system processes the requests in:
   - **Ascending order** for up.
   - **Descending order** for down.
5. After servicing all requests, the user is asked whether they want to continue.

🛠️ Code Structure

- `Request' struct: Represents a floor request with a timestamp.
- `PriorityDeque`: Stores and sorts incoming floor requests.
- `processRequestsUp()`: Handles upward movement requests.
- `processRequestsDownHoldHigher()`: Handles downward movement and moves directly to the highest requested floor.
- `goToFloor()`: Simulates elevator movement and floor arrival.

📂 Files

- `elevator_simulation.c`: Main program source code.

🧪 How to Run

On Linux/macOS:

gcc elevator_simulation.c -o elevator
./elevator

On Windows (using MinGW or similar):

gcc elevator_simulation.c -o elevator.exe
elevator.exe


Sample Interaction

 
Enter the starting floor of the elevator: 3
Select direction:
1. Go Up
2. Go Down
Enter your choice: 1
Enter the floor you want to go to (-1 to stop): 5
Enter the floor you want to go to (-1 to stop): 7
Enter the floor you want to go to (-1 to stop): -1
Moving from floor 3 to floor 5...
Arrived at floor 5
Moving from floor 5 to floor 7...
Arrived at floor 7
Do you want to continue? (y/n):
