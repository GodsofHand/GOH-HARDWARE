#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <cstring>
#include <sstream> // Include for std::istringstream

struct SharedData {
    int X,Y,S;
};

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        const char *args[] = {"python3", "joystickReader.py", NULL};  // Use const char* for string literals
        execvp("python3", const_cast<char**>(args));
        exit(1);  // execvp only returns on error
    } else if (pid > 0) {
        // Parent process
        const char* fifo_path = "/tmp/mpu_fifo2";
        std::ifstream fifo(fifo_path);

        // Set up shared memory
        int shm_fd = shm_open("/SharedMemoryJS", O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, sizeof(SharedData));
        SharedData* shared_data = static_cast<SharedData*>(
            mmap(nullptr, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)
        );

        std::string line;
        while (getline(fifo, line)) {
            std::istringstream iss(line);
            if (!(iss >> shared_data->X >> shared_data->Y >> shared_data->S)) {
                // std::cerr << "Error parsing line: " << line << std::endl;
            } else {
                 std::cout << line << std::endl;
            }
        }

        // Cleanup
        munmap(shared_data, sizeof(SharedData));
        close(shm_fd);
        shm_unlink("/SharedMemoryJS");
        wait(NULL);  // Wait for child process to finish
    } else {
        std::cerr << "Failed to fork." << std::endl;
    }
    return 0;
}
