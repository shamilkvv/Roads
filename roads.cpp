#include <iostream>
#include <vector>
#include <mutex>
#include <chrono>
#include <thread>

std::mutex mtx;

struct Vehicle {
    int id;
    int roadId;
    int direction;
    int arrivalTime;
};

void changeLight(int* lightState) {
    *lightState = (*lightState == 1) ? 2 : 1;
}

void vehicleCrossed(const Vehicle& vehicle, int* lightState) {
    std::lock_guard<std::mutex> lock(mtx);
    const char* road = (vehicle.roadId == 1) ? "A" : "B";
    std::cout << "Vehicle " << vehicle.id << " has crossed path " << road << " in direction " << vehicle.direction << "\n";
}

int main() {
    std::vector<Vehicle> vehicles = {
        {1, 1, 2, 10},
        {3, 1, 1, 20},
        {5, 1, 2, 30},
        {2, 2, 4, 40},
        {4, 2, 3, 50}
    };

    int lightState = 1;
    for (size_t i = 0; i < vehicles.size(); ++i) {
        if (vehicles[i].direction <= 2 && lightState != 1) {
            lightState = 1;
        }
        else if (vehicles[i].direction > 2 && lightState != 2) {
            lightState = 2;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(vehicles[i].arrivalTime));

        vehicleCrossed(vehicles[i], &lightState);

        if (i + 1 < vehicles.size()) {
            if (lightState == 1 && vehicles[i + 1].direction > 2) {
                std::cout << "Traffic light on path B is Green\n";
            }
            else if (lightState == 2 && vehicles[i + 1].direction <= 2) {
                std::cout << "Traffic light on path A is Green\n";
            }
        }
    }

    return 0;
}
