//
// Created by dmerkushov on 03.05.22.
//

#ifndef NANOSERVICES2_TESTING_H
#define NANOSERVICES2_TESTING_H

#include <functional>
#include <map>
#include <memory>
#include <string>

#define NSTEST_ADD(func) static int __nanoservices2_nstest_var_##func = ns_testing_context::main_instance()->add_test(#func, func);

#define NSTEST_RUN ns_testing_context::main_instance()->run_tests();

class ns_testing_context {
public:
    ns_testing_context() noexcept;

    ~ns_testing_context() noexcept;

    template<typename T>
    int add_test(std::string funcname, T func) {
        static_assert(std::is_same_v<T, std::function<bool()>>, "Test functions can be only std::function<bool()>");
        _tests.insert({funcname, (std::function<bool()>) func});
        return 0;
    }

    void run_tests();

    static std::shared_ptr<ns_testing_context> main_instance() {
        static std::shared_ptr<ns_testing_context> instance = std::make_shared<ns_testing_context>();
        return instance;
    }

private:
    std::map<std::string, std::function<bool()>> _tests;
};

#endif // NANOSERVICES2_TESTING_H
