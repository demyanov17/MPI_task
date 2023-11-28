#include <iostream>
#include <mpi.h>
#include <vector>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Define data to be sent by each process
    int send_data = rank + 1;

    // Calculate displacements and counts for MPI_Allgatherv
    std::vector<int> send_counts(size);  // Number of elements to send from each process
    std::vector<int> displacements(size); // Displacement where each segment is placed
    for (int i = 0; i < size; ++i)
    {
        send_counts[i] = i + 1;
        displacements[i] = (i > 0) ? displacements[i - 1] + send_counts[i - 1] : 0;
    }

    // Calculate total size for the receive buffer
    int total_size = displacements[size - 1] + send_counts[size - 1];

    // Allocate receive buffer
    std::vector<int> recv_data(total_size);

    // Perform MPI_Allgatherv
    MPI_Allgatherv(&send_data, 1, MPI_INT, recv_data.data(), send_counts.data(), displacements.data(), MPI_INT, MPI_COMM_WORLD);

    // Print received data
    if (rank == 0)
    {
        std::cout << "Received data: ";
        for (int i = 0; i < total_size; ++i)
            std::cout << recv_data[i] << " ";
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}
