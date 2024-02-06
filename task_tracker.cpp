#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
    NEW = 0,          // новая
    IN_PROGRESS,  // в разработке
    TESTING,      // на тестировании
    DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
public:
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const string& person) const
    {
        return task_count_by_person_.at(person);
    }

    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const string& person)
    {
        task_count_by_person_[person][TaskStatus::NEW]++;
    }

    // Обновить статусы по данному количеству задач конкретного разработчика,
    // подробности см. ниже
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count)
    {
        TasksInfo updated_tasks;
        TasksInfo not_updated_tasks;
        int temp;
        
        TasksInfo current_person_tasks = task_count_by_person_.at(person);  
        for (TaskStatus task_status = (TaskStatus)0; task_status < TaskStatus::DONE;
                                    task_status = static_cast<TaskStatus>((size_t)task_status + 1)){
            int quantity = task_count_by_person_.at(person)[task_status];
            if(task_count > quantity){
                current_person_tasks[task_status] -= quantity;
                TaskStatus new_status = static_cast<TaskStatus>((size_t)task_status+1);
                current_person_tasks[new_status] += quantity;
                updated_tasks[new_status] = quantity;
                task_count -= quantity;
            }else{
                if(task_count != quantity)
                {
                    not_updated_tasks[task_status] = quantity - task_count;
                }
                temp = current_person_tasks[task_status];
                current_person_tasks[task_status]-= task_count;
                temp = current_person_tasks[task_status];
                temp -= task_count;
                TaskStatus new_status = static_cast<TaskStatus>((size_t)task_status+1);
                current_person_tasks[new_status] += task_count;
                updated_tasks[new_status] = task_count;
                task_count = 0;
            }

        }

        for(auto [task,num]:current_person_tasks)
        {
            task_count_by_person_.at(person)[task] = num;
        }
        

        return make_tuple(updated_tasks,not_updated_tasks);
    }
private:
    map<string,TasksInfo> task_count_by_person_;
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь.
void PrintTasksInfo(TasksInfo tasks_info) {
    cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
        ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
        ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
        ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}

void PrintStat(TasksInfo& updated_tasks, TasksInfo& untouched_tasks, TeamTasks& tasks, int count_tasks) {
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, count_tasks);
    cout << "Ivan's tasks: "s;
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"s));
    cout << "Updated Ivan's tasks: "s;
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: "s;
    PrintTasksInfo(untouched_tasks);
    cout <<  "======================================================\n"s;
}

int main() {
    TeamTasks tasks;
    tasks.AddNewTask("Ivan"s);
    cout << "Ivan's tasks: "s;
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"s));
    cout <<  "======================================================\n"s;
    TasksInfo updated_tasks, untouched_tasks;

    for (int i = 0; i < 4; ++i) {
        PrintStat(updated_tasks, untouched_tasks, tasks, 1000);
    }

    tasks.AddNewTask("Ivan"s);
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"s));
    cout <<  "======================================================\n"s;
    PrintStat(updated_tasks, untouched_tasks, tasks, 0);
    PrintStat(updated_tasks, untouched_tasks, tasks, 1);
    tasks.AddNewTask("Ivan"s);
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"s));
    PrintStat(updated_tasks, untouched_tasks, tasks, 1);
}