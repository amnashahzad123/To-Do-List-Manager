
#include <iostream>
#include <fstream>
#include <sstream>// taka phrase can be input as one stream

using namespace std;

const int maxtask = 100;

class Task {
protected:
    string title, description, deadline;
    bool complete;

public:
    Task(string t = "", string d = "", string dl = "", bool c = false)
        : title(t), description(d), deadline(dl), complete(c) {}

    virtual void display(int index) const = 0;
    virtual void type()  = 0;

    virtual void printSerialized(ostream& out)  = 0;


    virtual void edit(const string& t, const string& d, const string& dl) {
        title = t;
        description = d;
        deadline = dl;
    }

    void markCompleted() {
        complete = true;
    }

    bool isCompleted() const {
        return complete;
    }

    virtual ~Task() {}
};

class SimpleTask : public Task {
public:
    SimpleTask(string t = "", string d = "", string dl = "", bool c = false)
        : Task(t, d, dl, c) {}

    void display(int index) const override {
        cout << index + 1 << " Simple-";

        if (complete) {
            cout << "Done";
        }
        else {
            cout << "Not done ";
        }

        cout << "- " << title << " (Due: " << deadline << ")" << endl
            << "   Description: " << description << endl;
    }

    void printSerialized(ostream& out)  override {
        out << "Simple|" << title << "|" << description << "|" << deadline << "|";

        if (complete) {
            out << "Done";
        }
        else {
            out << "Not done";
        }

        out << endl;
    }

    void type()  override {
        cout << "Simple" << endl;
    }
};

class RecurringTask : public Task {
private:
    string recurrence;

public:
    RecurringTask(string t = "", string d = "", string dl = "", string r = "", bool c = false)
        : Task(t, d, dl, c), recurrence(r) {}

    void display(int index) const override {
        cout << index + 1 << ". Recurring-";

        if (complete) {
            cout << "Done";
        }
        else {
            cout << "Not done ";
        }

        cout << "- " << title << " (Due: " << deadline << ", Every: " << recurrence << ")" << endl
            << "   Description: " << description << endl;
    }


    void printSerialized(ostream& out)  override {
        out << "Recurring|" << title << "|" << description << "|" << deadline << "|" << recurrence << "|";

        if (complete) {
            out << "Done";
        }
        else {
            out << "Not done";
        }

        out << endl;
    }

    void type()  override {
        cout << "Recurring" << endl;
    }

    void edit(const string& t, const string& d, const string& dl) override {
        Task::edit(t, d, dl);
    }
};

class TaskManager {
private:
    Task* tasks[maxtask];// base class pointer banaya 
    int taskCount;

public:
    TaskManager() : taskCount(0) {}

    void addTask(Task* task) {
        if (taskCount < maxtask) {
            tasks[taskCount++] = task;
            cout << "Task added." << endl;
        }
        else {
            cout << "Task list is full." << endl;
        }
    }

    void viewTasks() const {
        if (taskCount == 0) {
            cout << "No tasks to show." << endl;
            return;
        }
        for (int i = 0; i < taskCount; i++) {
            tasks[i]->display(i);
        }
    }

    void markTaskCompleted(int index) {
        if (index >= 0 && index < taskCount) {
            tasks[index]->markCompleted();
            cout << "Task marked as completed." << endl;
        }
        else {
            cout << "Invalid index." << endl;
        }
    }

    void deleteTask(int index) {
        if (index >= 0 && index < taskCount) {
            delete tasks[index];
            for (int i = index; i < taskCount - 1; i++) {
                tasks[i] = tasks[i + 1];
            }
            --taskCount;
            cout << "Task deleted." << endl;
        }
        else {
            cout << "Invalid index." << endl;
        }
    }

    void editTask(int index, const string& title, const string& desc, const string& deadline) {
        if (index >= 0 && index < taskCount) {
            tasks[index]->edit(title, desc, deadline);
            cout << "Task updated." << endl;
        }
        else {
            cout << "Invalid index." << endl;
        }
    }


    void saveToFile(const string& filename) const {
        ofstream file(filename);
        for (int i = 0; i < taskCount; ++i) {
            tasks[i]->printSerialized(file); // now writing to file
        }
        file.close();
        cout << "Tasks saved to file." << endl;
    }


    void loadFromFile(const string& filename) {// task load karna ka lia
        ifstream file(filename);
        string line;
        taskCount = 0;

        while (getline(file, line) && taskCount < maxtask) {
            istringstream iss(line);// to read the whole line
            string type;
            getline(iss, type, '|');

            if (type == "Simple") {
                string t, d, dl, c;
                getline(iss, t, '|'); // iss se kia hay for splitting
                getline(iss, d, '|');
                getline(iss, dl, '|');
                getline(iss, c, '|');
                tasks[taskCount++] = new SimpleTask(t, d, dl, c == "Done");//ye store kar raah pointer to a simple class object
            }
            else if (type == "Recurring") {
                string t, d, dl, r, c;
                getline(iss, t, '|');
                getline(iss, d, '|');
                getline(iss, dl, '|');
                getline(iss, r, '|');
                getline(iss, c, '|');
                tasks[taskCount++] = new RecurringTask(t, d, dl, r, c == "Done");
            }
        }

        file.close();
        cout << "Tasks loaded from file." << endl;
    }

    ~TaskManager() {
        for (int i = 0; i < taskCount; ++i) {
            delete tasks[i];
        }
    }
};



int main() {
    TaskManager manager;
    int choice;

    while (true) {//chalta raha till uuser selects 9

        cout << endl << "--- To-Do List Manager ---" << endl
            << "1. Add Simple Task" << endl
            << "2. Add Recurring Task" << endl
            << "3. View Tasks" << endl
            << "4. Mark Task as Completed" << endl
            << "5. Delete Task" << endl
            << "6. Edit Task" << endl
            << "7. Save Tasks" << endl
            << "8. Load Tasks" << endl
            << "9. Exit" << endl
            << "Choose an option: ";

        cin >> choice;
        cin.ignore();// remove anything previosly buffer ka andar

        if (choice == 1) {
            string title, desc, deadline;
            cout << "Enter title: "; getline(cin, title);//getline taka szze define na karna para
            cout << "Enter description: "; getline(cin, desc);
            cout << "Enter deadline: "; getline(cin, deadline);
            manager.addTask(new SimpleTask(title, desc, deadline));
        }
        else if (choice == 2) {
            string title, desc, deadline, recurrence;
            cout << "Enter title: "; getline(cin, title);
            cout << "Enter description: "; getline(cin, desc);
            cout << "Enter deadline: "; getline(cin, deadline);
            cout << "Enter recurrence (e.g., daily, weekly): "; getline(cin, recurrence);
            manager.addTask(new RecurringTask(title, desc, deadline, recurrence));
        }
        else if (choice == 3) {
            manager.viewTasks();
        }
        else if (choice == 4) {
            int index;
            cout << "Enter task number to mark as completed: ";
            cin >> index;
            manager.markTaskCompleted(index - 1);
        }
        else if (choice == 5) {
            int index;
            cout << "Enter task number to delete: ";
            cin >> index;
            manager.deleteTask(index - 1);
        }
        else if (choice == 6) {
            int index;
            string title, desc, deadline;
            cout << "Enter task number to edit: "; cin >> index;
            cin.ignore();
            cout << "Enter new title: "; getline(cin, title);
            cout << "Enter new description: "; getline(cin, desc);
            cout << "Enter new deadline: "; getline(cin, deadline);
            manager.editTask(index - 1, title, desc, deadline);
        }
        else if (choice == 7) {
            manager.saveToFile("tasks.txt");
        }
        else if (choice == 8) {
            manager.loadFromFile("tasks.txt");
        }
        else if (choice == 9) {
            cout << "Exiting" << endl;
            break;
        }
        else {
            cout << "Invalid option. Please try again." << endl;
        }
    }

    return 0;
}