#include <iostream>

class Singleton {
public:
    static Singleton* GetInstance();
    static Singleton* DelInstance();
private:
    Singleton() = default;
    static Singleton* _instance;
};

Singleton* Singleton::_instance = nullptr;

Singleton* Singleton::GetInstance() { 
    if (!_instance)
        _instance = new Singleton;
    return _instance;
}

Singleton* Singleton::DelInstance() { 
    std::cout << "deleted" << std::endl;
    if (_instance)
        delete _instance;
}

void testSing(Singleton* inst) {
    inst = Singleton::GetInstance();
    std::cout << "Singleton instance created: " << inst << std::endl;
    Singleton::DelInstance();
    std::cout << "Singleton instance deleted: " << inst << std::endl;

}

int main() {

    Singleton* inst;

    testSing(inst);

}