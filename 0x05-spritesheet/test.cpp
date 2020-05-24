#include <iostream>
#include <vector>

struct RectStruct
{
    int x, y;
    int w, h;
};

class RectClass
{
public:
    int x, y;
    int w, h;

    RectClass() {}
    ~RectClass() {}

    virtual void say()
    {
        printf("RectClass\n\n");
    }
};

class SubRectClass : public RectClass
{
public:
    SubRectClass() {}
    ~SubRectClass() {}

    virtual void say()
    {
        printf("SubRectClass\n\n");
    }
};

int main()
{
    // Rect rects[3];
    // int len = *(&rects + 1) - rects;
    // std::cout << "Hello: " << len << "!" << std::endl;

    std::vector<RectStruct> rStructs;
    rStructs.push_back({1, 2, 3, 4});
    rStructs.push_back({5, 6, 7, 8});
    rStructs.push_back({9, 9, 9, 9});

    std::cout << "Structs: " << rStructs.size() << std::endl;

    for (int i = 0; i < rStructs.size(); i++)
    {
        std::cout << "INDEX " << i << std::endl;
        std::cout << "x: " << rStructs.at(i).x << std::endl;
        std::cout << "y: " << rStructs.at(i).y << std::endl;
        std::cout << "w: " << rStructs.at(i).w << std::endl;
        std::cout << "h: " << rStructs.at(i).h << std::endl;
    }

    std::vector<RectClass *> rClasses;
    rClasses.push_back(new RectClass());
    rClasses.push_back(new SubRectClass());
    rClasses.push_back(new RectClass());

    std::cout << "\n\nClasses: " << rStructs.size() << std::endl;

    for (int i = 0; i < rClasses.size(); i++)
    {
        std::cout << "INDEX " << i << std::endl;
        std::cout << "x: " << rClasses.at(i)->x << std::endl;
        std::cout << "y: " << rClasses.at(i)->y << std::endl;
        std::cout << "w: " << rClasses.at(i)->w << std::endl;
        std::cout << "h: " << rClasses.at(i)->h << std::endl;
        rClasses.at(i)->say();
    }

    RectClass *test = rClasses.at(1);
    for (int i = 0; i < rClasses.size(); i++)
    {
        delete rClasses.at(i);
    }
    rClasses.clear();

    // this will freeze, because the object has been deleted
    // test->say();

    return 0;
}