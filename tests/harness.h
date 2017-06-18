#ifndef CAMLIB_TEST_H_INCLUDED
#define CAMLIB_TEST_H_INCLUDED

#include "out.h"
#include <vector>
#include <string>
#include <memory>


class TestHarness
{
public:
    TestHarness(std::string t)
    {
        title = t;
    }

    bool run()
    {
        out << "BEGIN TEST: " << title.c_str() << "\n\n";
        std::vector< std::shared_ptr<TestModule> >::iterator it = tm_.begin();
        while(it != tm_.end())
        {
            if(!(*it)->run())
            {
                out << "Aborting test\n";
                return false;
            }
            ++it;
        }

        out << "\nEND TEST\n";
        return true;
    }

    void add_module(bool (*test)(std::string& subtest), std::string t)
    {
        std::shared_ptr<TestModule> ptm = std::make_shared<TestModule>(test, t);
        tm_.push_back(ptm);
    }

private:
    class TestModule
    {
    public:
        TestModule(bool (*tes)(std::string& sub_test), std::string t)
        {
            title = t;
            test = tes;
        }

        bool run()
        {
            out << title.c_str();
            std::string subtest;
            if(test(subtest))
                out << "\t\t-\t\tpass\n";
            else
            {
                out << "\t\t-\t\tfailed\n\n\t" << subtest.c_str() << "\n\n";
                return false;
            }

            return true;
        }


    private:
        std::string title;
        bool (*test)(std::string& subtest);
    };

    std::string title;
    std::vector<std::shared_ptr<TestModule> > tm_;
};


#endif // TEST_H_INCLUDED
