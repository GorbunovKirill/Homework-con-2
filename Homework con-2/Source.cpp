#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <iomanip>

std::mutex print_mutex;


void move_cursor_to(int x, int y) {
    std::cout << "\033[" << y << ";" << x << "H";
}


void thread_function(int thread_num, int progress_length, int y_position) {
    auto start_time = std::chrono::high_resolution_clock::now();
    std::thread::id thread_id = std::this_thread::get_id();

    for (int i = 0; i <= progress_length; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
       
        {
            std::lock_guard<std::mutex> lock(print_mutex);
            move_cursor_to(0, y_position); 

           
            int filled_length = i * (50 / progress_length); 
            int total_length = 50; 
            std::cout << "Поток " << thread_num << " (ID: " << thread_id << "): [";
            std::cout << std::string(filled_length, '#') << std::string(total_length - filled_length, '-') << "] "
                << std::setw(4) << (i * 100 / progress_length) << "%" << std::flush;
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;

    
    {
        std::lock_guard<std::mutex> lock(print_mutex);
        move_cursor_to(0, y_position);
        std::cout << "Поток " << thread_num << " завершил работу. Время: " << elapsed_time.count() << " сек" << std::endl;
    }
}

int main() {

    setlocale(LC_ALL, "Russian");

    int num_threads = 5; 
    int progress_length = 20;  

    std::vector<std::thread> threads;

    std::cout << "\033[2J";

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(thread_function, i + 1, progress_length, i + 2);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "\nВсе потоки завершили работу." << std::endl;

    return 0;
}
