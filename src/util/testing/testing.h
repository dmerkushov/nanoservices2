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
    ns_testing_context() noexcept = default;

    ~ns_testing_context() noexcept = default;

    template<typename T>
    int add_test(std::string const &funcname, T func) {
        static_assert(std::is_same_v<T, std::function<bool()>>, "Test functions can be only std::function<bool()>");
        _tests.try_emplace(funcname, (std::function<bool()>) func);
        return 0;
    }

    void run_tests();

    static std::shared_ptr<ns_testing_context> main_instance() {
        static auto instance = std::make_shared<ns_testing_context>();
        return instance;
    }

private:
    std::map<std::string, std::function<bool()>, std::less<>> _tests;
};

#endif // NANOSERVICES2_TESTING_H
