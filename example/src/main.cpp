#include <cstdlib>
#include <iostream>

#include <function_extractor/exceptions.hpp>
#include <function_extractor/library_loader.hpp>

static int print_error_and_exit(const std::exception & error)
{
    std::cerr << error.what() << std::endl;

    return EXIT_FAILURE;
}

int main()
{
    namespace function_extractor = burda::function_extractor;

    try
    {
        function_extractor::library_loader library{ "./demo-library" };

        // get procedures at runtime from the shared library
        //const auto func_void_no_params = library.get_procedure<void()>("foo");
        //const auto func_void_int_param = library.get_procedure<void(int)>("bar");

        // don't have to check for call-ability, otherwise the "function_does_not_exist" would be thrown
        //func_void_no_params();
        //func_void_int_param(99);
    }
    catch (const function_extractor::exceptions::library_load_failed & error)
    {
        return print_error_and_exit(error);
    }
    catch (const function_extractor::exceptions::function_does_not_exist & error)
    {
        return print_error_and_exit(error);
    }

    // library object will go out of scope, thus it's going to free all resources and unloads the library handle

    return EXIT_SUCCESS;
}
