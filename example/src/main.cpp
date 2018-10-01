#include <cstdlib>
#include <iostream>

#include <function_loader/exceptions.hpp>
#include <function_loader/function_loader.hpp>

static int print_error_and_exit(const std::exception & error)
{
    std::cerr << error.what() << std::endl;

    return EXIT_FAILURE;
}

static int show_usage()
{
    namespace function_loader = burda::function_loader;

    try
    {
        function_loader::function_loader loader{ "./demo-library.dll" };

        // get procedures at runtime from the shared library
        // see "demo-library.hpp" and "demo-library.cpp" in the "demo-library" directory
        const auto func_void_no_params = loader.get_function<void()>("function_with_no_params");
        const auto func_with_return_value_and_params = loader.get_function<int(float, const char *)>("function_with_return_value_and_params");

        // don't have to check for call-ability, otherwise the "function_does_not_exist" would be thrown
        func_void_no_params();
        std::clog << "func_with_return_value_and_params returned " << func_with_return_value_and_params(99.0, "foo");

        // "loader" object will go out of scope, thus it's going to free all resources and unloads the library handle
    }
    catch (const function_loader::exceptions::library_load_failed & error)
    {
        return print_error_and_exit(error);
    }
    catch (const function_loader::exceptions::function_does_not_exist & error)
    {
        return print_error_and_exit(error);
    }

    return EXIT_SUCCESS;
}

int main(int /*argc*/, char ** /*argv*/)
{
    return show_usage();
}
