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
    float f1, f2, f3, f4, f5;
    float qw,qx,qy,qz;
    float yaw, pitch, roll;
};

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        const char *args[] = {"python3", "mpuReader.py", NULL};  // Use const char* for string literals
        execvp("python3", const_cast<char**>(args));
        exit(1);  // execvp only returns on error
    } else if (pid > 0) {
        // Parent process
        const char* fifo_path = "/tmp/mpu_fifo";
        std::ifstream fifo(fifo_path);

        // Set up shared memory
        int shm_fd = shm_open("/SharedMemoryMPU", O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, sizeof(SharedData));
        SharedData* shared_data = static_cast<SharedData*>(
            mmap(nullptr, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)
        );

        std::string line;
        while (getline(fifo, line)) {
            std::istringstream iss(line);
            if (!(iss >>shared_data->yaw >> 
                  shared_data->pitch >> shared_data->roll >>shared_data->f5 >> shared_data->f4 >> shared_data->f3 >> 
                  shared_data->f2 >> shared_data->f1 >> shared_data->qw>> shared_data->qx>> shared_data->qy>> shared_data->qz )) {
                // std::cerr << "Error parsing line: " << line << std::endl;
            } else {
                 std::cout << line << std::endl;
            }
        }

        // Cleanup
        munmap(shared_data, sizeof(SharedData));
        close(shm_fd);
        shm_unlink("/SharedMemoryMPU");
        wait(NULL);  // Wait for child process to finish
    } else {
        std::cerr << "Failed to fork." << std::endl;
    }
    return 0;
}
