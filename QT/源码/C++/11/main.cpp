#include <iostream>

using namespace std;

class Dog
{
public:
    void getWeight(string color, int weight) {
        cout << "int类型狗的体重：" << weight << endl;
    }

    double getWeight(double weight) {
        cout << "double类型狗的体重：" << weight << endl;
        return weight;
    }
};

int main()
{
    Dog dog1;
    dog1.getWeight("黑色", 10);
    dog1.getWeight(10.2);
    return 0;
}
