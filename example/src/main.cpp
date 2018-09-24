#include <cstdlib>
#include <iostream>

#include <function_extractor/exceptions.hpp>
#include <function_extractor/function_loader.hpp>

static int print_error_and_exit(const std::exception & error)
{
    std::cerr << error.what() << std::endl;

    return EXIT_FAILURE;
}

static int show_usage()
{
    namespace function_extractor = burda::function_extractor;

    try
    {
        function_extractor::function_loader loader{ "./demo-library.dll" };

        // get procedures at runtime from the shared library
        // see "demo-library.hpp" and "demo-library.cpp" in the "demo-library" directory
        const auto func_void_no_params = loader.get_procedure<void()>("function_with_no_params");
        const auto func_with_return_value_and_params = loader.get_procedure<int(float, const char *)>("function_with_return_value_and_params");

        // don't have to check for call-ability, otherwise the "function_does_not_exist" would be thrown
        func_void_no_params();
        std::clog << "func_with_return_value_and_params returned " << func_with_return_value_and_params(99.0, "foo");
    }
    catch (const function_extractor::exceptions::library_load_failed & error)
    {
        return print_error_and_exit(error);
    }
    catch (const function_extractor::exceptions::function_does_not_exist & error)
    {
        return print_error_and_exit(error);
    }

    // "loader" object will go out of scope, thus it's going to free all resources and unloads the library handle

    return EXIT_SUCCESS;
}

int main(int /*argc*/, char ** /*argv*/)
{
    return show_usage();
}
