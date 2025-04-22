#include <data_structures/DataBuffer.hpp> // Assuming your DataBuffer is defined in this header
#include <iostream>
#include <string>
#include <exception>

class TestObject {
public:
    int x;
    float y;

    friend DataBuffer& operator<<(DataBuffer& p_buffer, const TestObject& p_object) {
        p_buffer << p_object.x << p_object.y;
        return p_buffer;
    }

    friend DataBuffer& operator>>(DataBuffer& p_buffer, TestObject& p_object) {
        p_buffer >> p_object.x >> p_object.y;
        return p_buffer;
    }
};

int main() {
    DataBuffer myBuffer;

    TestObject obj1;
    obj1.x = 42;
    obj1.y = 19.0;

    TestObject obj2;
    obj2.x = 99;
    obj2.y = 45.0;

    myBuffer << obj1 << obj2;

    TestObject deserializedObj1, deserializedObj2, deserializedObj3;

    try {
        myBuffer >> deserializedObj1 >> deserializedObj2;
        std::cout << "Deserialized obj1: x = " << deserializedObj1.x << ", y = " << deserializedObj1.y << std::endl;
        std::cout << "Deserialized obj2: x = " << deserializedObj2.x << ", y = " << deserializedObj2.y << std::endl;
        if (deserializedObj1.x != 42)
            return 1;
        if (deserializedObj1.y != 19.0)
            return 1;
        if (deserializedObj2.x != 99)
            return 1;
        if (deserializedObj2.y != 45.0)
            return 1;
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    try {
        myBuffer >> deserializedObj3;
        return 1;
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    return 0;
}
