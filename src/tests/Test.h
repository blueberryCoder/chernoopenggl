//
// Created by blueberry on 2022/12/3.
//

#pragma once

namespace test {
    class Test {
    public :
        Test() {}

        virtual ~Test() {}

        virtual void OnUpdate(float deltaTime) {}

        virtual void OnRender() {}

        virtual void OnImGuiRender() {}
    };
}
