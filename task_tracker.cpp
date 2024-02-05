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
        
        static TasksInfo current_person_tasks = task_count_by_person_.at(person);  
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
                current_person_tasks[task_status]-= task_count;
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
    cout << tasks_info[TaskStatus::NEW] << " new tasks"s
         << ", "s << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress"s
         << ", "s << tasks_info[TaskStatus::TESTING] << " tasks are being tested"s
         << ", "s << tasks_info[TaskStatus::DONE] << " tasks are done"s << endl;
}

int main() {
    TeamTasks tasks;
    tasks.AddNewTask("Ilia"s);
    for (int i = 0; i < 1; ++i) {
        tasks.AddNewTask("Ivan"s);
    }
    cout << "Ilia's tasks: "s;
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"s));
    cout << "Ivan's tasks: "s;
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"s));

    TasksInfo updated_tasks, untouched_tasks;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 0);
    cout << "Updated Ivan's tasks: "s;
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: "s;
    PrintTasksInfo(untouched_tasks);

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 2);
    cout << "Updated Ivan's tasks: "s;
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: "s;
    PrintTasksInfo(untouched_tasks);

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 2);
    cout << "Updated Ivan's tasks: "s;
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: "s;
    PrintTasksInfo(untouched_tasks);

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 1000);
    cout << "Updated Ivan's tasks: "s;
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: "s;
    PrintTasksInfo(untouched_tasks);


    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 1000);
    cout << "Updated Ivan's tasks: "s;
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: "s;
    PrintTasksInfo(untouched_tasks);
    
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 1000);
    cout << "Updated Ivan's tasks: "s;
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: "s;
    PrintTasksInfo(untouched_tasks);



}