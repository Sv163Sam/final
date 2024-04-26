#ifndef FINAL_STUDENT_H
#define FINAL_STUDENT_H
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <thread>
#include <mutex>

struct Student {
    int id;
    std::string name;
    int age;
};

class StudentDatabase {
private:
    std::vector<std::shared_ptr<Student>> students;
    std::mutex mtx;

public:
    void addStudent(int id, const std::string& name, int age) {
        std::lock_guard<std::mutex> lock(mtx);
        students.push_back(std::make_shared<Student>(Student{id, name, age}));
    }

    void removeStudent(int id) {
        std::lock_guard<std::mutex> lock(mtx);
        students.erase(std::remove_if(students.begin(), students.end(), [id](const std::shared_ptr<Student>& student) {
            return student->id == id;
        }), students.end());
    }

    std::shared_ptr<Student> getStudent(int id) {
        std::lock_guard<std::mutex> lock(mtx);
        for (const auto& student : students) {
            if (student->id == id) {
                return student;
            }
        }
        return nullptr;
    }
};

void writerFunction(StudentDatabase& db) {
    for (int i = 0; i < 10; ++i) {
        db.addStudent(i, "Student" + std::to_string(i), 20 + i);
        std::cout << "Was write student - " << db.getStudent(i)->id << "with name: "<< db.getStudent(i)->name << " is " << db.getStudent(i)->age << " years old." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void readerFunction(StudentDatabase& db) {
    for (int i = 0; i < 10; ++i) {
        auto student = db.getStudent(i);
        if (student != nullptr) {
            std::cout << "Was read student - " << student->name << " is " << student->age << " years old." << std::endl;
        } else {
            std::cout << "Student with ID " << i << " not found." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void call_student()
{
    StudentDatabase db;

    std::thread writerThread(writerFunction, std::ref(db));
    std::thread readerThread(readerFunction, std::ref(db));

    writerThread.join();
    readerThread.join();
}

#endif //FINAL_STUDENT_H
