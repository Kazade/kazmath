#ifndef KAZTEST_H
#define KAZTEST_H

#include <vector>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>

#define assert_equal(expected, actual) _assert_equal((expected), (actual), __FILE__, __LINE__)
#define assert_not_equal(expected, actual) _assert_not_equal((expected), (actual), __FILE__, __LINE__)
#define assert_false(actual) _assert_false((actual), __FILE__, __LINE__)
#define assert_true(actual) _assert_true((actual), __FILE__, __LINE__)
#define assert_close(expected, actual, difference) _assert_close((expected), (actual), (difference), __FILE__, __LINE__)
#define assert_is_null(actual) _assert_is_null((actual), __FILE__, __LINE__)
#define assert_is_not_null(actual) _assert_is_not_null((actual), __FILE__, __LINE__)
#define assert_raises(exception, func) _assert_raises<exception>((func), __FILE__, __LINE__)
#define assert_items_equal(expected, actual) _assert_items_equal((actual), (expected), __FILE__, __LINE__)
#define not_implemented() _not_implemented(__FILE__, __LINE__)


class StringFormatter {
public:
    StringFormatter(const std::string& templ):
        templ_(templ) { }

    struct Counter {
        Counter(uint32_t c): c(c) {}
        uint32_t c;
    };

    template<typename T>
    std::string format(T value) {
        std::stringstream ss;
        ss << value;
        return _do_format(0, ss.str());
    }

    template<typename T>
    std::string format(Counter count, T value) {
        std::stringstream ss;
        ss << value;
        return _do_format(count.c, ss.str());
    }

    template<typename T, typename... Args>
    std::string format(T value, const Args&... args) {
        std::stringstream ss;
        ss << value;
        return StringFormatter(_do_format(0, ss.str())).format(Counter(1), args...);
    }

    template<typename T, typename... Args>
    std::string format(Counter count, T value, const Args&... args) {
        std::stringstream ss;
        ss << value;
        return StringFormatter(_do_format(count.c, ss.str())).format(Counter(count.c + 1), args...);
    }

    std::string _do_format(uint32_t counter, const std::string& value) {
        const std::string to_replace = "{" + std::to_string(counter) + "}";
        std::string output = templ_;

        auto replace = [](std::string& str, const std::string& from, const std::string& to) -> bool {
            size_t start_pos = str.find(from);
            if(start_pos == std::string::npos)
                return false;
            str.replace(start_pos, from.length(), to);
            return true;
        };

        replace(output, to_replace, value);
        return output;
    }

private:
    std::string templ_;
};

class StringSplitter {
public:
    StringSplitter(const std::string& str):
        str_(str) {

    }

    std::vector<std::string> split() {
        std::vector<std::string> result;
        std::string buffer;

        for(auto c: str_) {
            if(c == '\n') {
                if(!buffer.empty()) {
                    result.push_back(buffer);
                    buffer.clear();
                }
            } else {
                buffer.push_back(c);
            }
        }

        if(!buffer.empty()) {
            result.push_back(buffer);
        }

        return result;
    }

private:
    std::string str_;
};

#define _F StringFormatter

namespace kaztest {

class AssertionError : public std::logic_error {
public:
    AssertionError(const std::string& what):
        std::logic_error(what),
        file(""),
        line(-1) {
    }

    AssertionError(const std::pair<std::string, int> file_and_line, const std::string& what):
        std::logic_error(what),
        file(file_and_line.first),
        line(file_and_line.second) {

    }

    std::string file;
    int line;
};

class NotImplementedError: public std::logic_error {
public:
    NotImplementedError(const std::string& file, int line):
        std::logic_error(_F("Not implemented at {0}:{1}").format(file, line)) {}
};

}

class TestCase {
public:
    virtual ~TestCase() {}

    virtual void set_up() {}
    virtual void tear_down() {}

    template<typename T, typename U>
    void _assert_equal(T expected, U actual, std::string file, int line) {
        if(expected != (T) actual) {
            auto file_and_line = std::make_pair(file, line);
            throw kaztest::AssertionError(file_and_line, _F("{0} does not match {1}").format(actual, expected));
        }
    }

    template<typename T, typename U>
    void _assert_not_equal(T lhs, U rhs, std::string file, int line) {
        if(lhs == (T) rhs) {
            auto file_and_line = std::make_pair(file, line);
            throw kaztest::AssertionError(file_and_line, _F("{0} should not match {1}").format(lhs, rhs));
        }
    }

    template<typename T>
    void _assert_true(T actual, std::string file, int line) {
        if(!bool(actual)) {
            auto file_and_line = std::make_pair(file, line);
            throw kaztest::AssertionError(file_and_line, _F("{0} is not true").format(bool(actual) ? "true" : "false"));
        }
    }

    template<typename T>
    void _assert_false(T actual, std::string file, int line) {
        if(bool(actual)) {
            auto file_and_line = std::make_pair(file, line);
            throw kaztest::AssertionError(file_and_line, _F("{0} is not false").format(bool(actual) ? "true" : "false"));
        }
    }

    template<typename T, typename U, typename V>
    void _assert_close(T expected, U actual, V difference, std::string file, int line) {
        if(actual < expected - difference ||
           actual > expected + difference) {
            auto file_and_line = std::make_pair(file, line);
            throw kaztest::AssertionError(file_and_line, _F("{0} is not close enough to {1}").format(actual, expected));
        }
    }

    template<typename T>
    void _assert_is_null(T* thing, std::string file, int line) {
        if(thing != nullptr) {
            auto file_and_line = std::make_pair(file, line);
            throw kaztest::AssertionError(file_and_line, "Pointer was not NULL");
        }
    }

    template<typename T>
    void _assert_is_not_null(T* thing, std::string file, int line) {
        if(thing == nullptr) {
            auto file_and_line = std::make_pair(file, line);
            throw kaztest::AssertionError(file_and_line, "Pointer was unexpectedly NULL");
        }
    }

    template<typename T, typename Func>
    void _assert_raises(Func func, std::string file, int line) {
        try {
            func();
            auto file_and_line = std::make_pair(file, line);
            throw kaztest::AssertionError(file_and_line, _F("Expected exception ({0}) was not thrown").format(typeid(T).name()));
        } catch(T& e) {}
    }

    template<typename T, typename U>
    void _assert_items_equal(const T& lhs, const U& rhs, std::string file, int line) {
        auto file_and_line = std::make_pair(file, line);

        if(lhs.size() != rhs.size()) {
            throw kaztest::AssertionError(file_and_line, "Containers are not the same length");
        }

        for(auto item: lhs) {
            if(std::find(rhs.begin(), rhs.end(), item) == rhs.end()) {
                throw kaztest::AssertionError(file_and_line, _F("Container does not contain {0}").format(item));
            }
        }
    }

    void _not_implemented(std::string file, int line) {
        throw kaztest::NotImplementedError(file, line);
    }
};

class TestRunner {
public:
    template<typename T, typename U>
    void register_case(std::vector<U> methods, std::vector<std::string> names) {
        std::shared_ptr<TestCase> instance = std::make_shared<T>();

        instances_.push_back(instance); //Hold on to it

        for(std::string name: names) {
            names_.push_back(name);
        }

        for(U& method: methods) {
            std::function<void()> func = std::bind(method, dynamic_cast<T*>(instance.get()));
            tests_.push_back([=]() {
                instance->set_up();
                func();
                instance->tear_down();
            });
        }
    }

    int32_t run(const std::string& test_case) {
        int failed = 0;
        int skipped = 0;
        int ran = 0;
        int crashed = 0;

        auto new_tests = tests_;
        auto new_names = names_;

        if(!test_case.empty()) {
            new_tests.clear();
            new_names.clear();

            for(uint32_t i = 0; i < names_.size(); ++i) {
                if(names_[i].find(test_case) == 0) {
                    new_tests.push_back(tests_[i]);
                    new_names.push_back(names_[i]);
                }
            }
        }

        std::cout << std::endl << "Running " << new_tests.size() << " tests" << std::endl << std::endl;

        for(std::function<void ()> test: new_tests) {
            try {
                std::string output = "    " + new_names[ran];

                for(int i = output.length(); i < 76; ++i) {
                    output += " ";
                }

                std::cout << output;
                test();
                std::cout << "\033[32m" << "   OK   " << "\033[0m" << std::endl;
            } catch(kaztest::NotImplementedError& e) {
                std::cout << "\033[34m" << " SKIPPED" << "\033[0m" << std::endl;
                ++skipped;
            } catch(kaztest::AssertionError& e) {
                std::cout << "\033[33m" << " FAILED " << "\033[0m" << std::endl;
                std::cout << "        " << e.what() << std::endl;
                if(!e.file.empty()) {
                    std::cout << "        " << e.file << ":" << e.line << std::endl;

                    std::ifstream ifs(e.file);
                    if(ifs.good()) {
                        std::string buffer((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
                        auto lines = StringSplitter(buffer).split();

                        int line_count = lines.size();
                        if(line_count && e.line <= line_count) {
                            std::cout << lines.at(e.line - 1) << std::endl << std::endl;
                        }
                    }
                }
                ++failed;
            } catch(std::exception& e) {
                std::cout << "\033[31m" << " EXCEPT " << std::endl;
                std::cout << "        " << e.what() << "\033[0m" << std::endl;
                ++crashed;
            }
            std::cout << "\033[0m";
            ++ran;
        }

        std::cout << "-----------------------" << std::endl;
        if(!failed && !crashed && !skipped) {
            std::cout << "All tests passed" << std::endl << std::endl;
        } else {
            if(skipped) {
                std::cout << skipped << " tests skipped";
            }

            if(failed) {
                if(skipped) {
                    std::cout << ", ";
                }
                std::cout << failed << " tests failed";
            }

            if(crashed) {
                if(failed) {
                    std::cout << ", ";
                }
                std::cout << crashed << " tests crashed";
            }
            std::cout << std::endl << std::endl;
        }

        return failed;
    }

private:
    std::vector<std::shared_ptr<TestCase>> instances_;
    std::vector<std::function<void()> > tests_;
    std::vector<std::string> names_;
};


#endif // KAZTEST_H
