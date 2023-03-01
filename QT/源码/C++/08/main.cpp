#include <iostream>

using namespace std;

class Dog
{
    // 不写关键字的时候，就是private属性的
public:
    string name;
    int age;
    void run() {
    cout << "狗会跑！" << endl;
    }
protected:
    double weight;
};

int main()
{
    // 从栈中实例化对象
    Dog dog1;
    dog1.name = "旺财";
    cout << dog1.name << endl;
    dog1.run();
    // 从堆中实例化对象,要用delete删除
    Dog *dog2 = new Dog;
    dog2->age = 3;
    cout << dog2->age << endl;
    delete dog2;
    return 0;
}
