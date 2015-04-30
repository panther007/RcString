#include "RcString.hpp"

/*
 * To test the move operations, compile with -fno-elide-constructors.
 */

int
main() {

    cs540::RcString s1; // Initialize to "".

    // After the below, s2 and s1 should point to the same data object.
    cs540::RcString s2(s1);

    cs540::RcString s3;
    
    s3 = s1; // After this, also point to same data object.
    
    cs540::RcString s4("hello");

    cs540::RcString s5(std::string("hello"));

    // Output.
    std::cout << s5 << std::endl;

    // Concatentation.
    {
        cs540::RcString s1("hello ");
        cs540::RcString s2("goodbye");
        DEBUG_STM("Check");
        cs540::RcString s3(s1 + s2); // Invoke move ctor.
        DEBUG_STM("1 : "<<s1<<", 2 : "<<s2<<", 3 : "<<s3);
        DEBUG_STM("End");
        
        std::cout << s3 << std::endl;

        cs540::RcString s4;
        s4 = s3 + " forever."; // Move assignment.
        std::cout << s4 << std::endl;
        
    }
}
