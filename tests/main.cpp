//
// Created by xobtah on 06/02/17.
//

#include <string>
#include <iostream>

#include "../DataBase.hpp"

struct Test
{
    int x;
    std::string y;
};

int main()
{
    Basium::DataBase<Test>  db;
    Test                    test;
    unsigned int            id;

    test.x = 42;
    test.y = "lol";
    std::cout << db.Insert(test) << std::endl;
    test.x = 24;
    test.y = "kek";
    std::cout << (id = db.Insert(test)) << std::endl;
    std::cout << db.FindOne(id).x << std::endl;
    std::cout << db.FindOne([](Test const &t) { return (t.x == 42); }).x << std::endl;
    return (0);
}