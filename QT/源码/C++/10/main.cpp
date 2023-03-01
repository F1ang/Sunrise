#include <iostream>

using namespace std;

// 这个就是基类，也叫父类
class Animal
{
public:
    string name;
    int age;
    void run() {
        cout << "狗的年龄：" << age << endl;
    }
private:
    double weight;

protected:
    string color;
};

// 这个类是派生类，也叫子类
class Dog : private Animal
{
public:
    void eat() {
        this->name = "旺财";
        this->color = "黑色";
    }
};

int main()
{
    Dog dog1;
    return 0;
}
