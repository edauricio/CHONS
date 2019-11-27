// Class to manage all resource requests from the application
// (both memory and thread) (should we include external I/O resources too?)

#include <map>
#include <string>

namespace CHONS {

class ResourceDirector {
    public:
        ResourceDirector() = delete;

        class Memory {
            public:
                template <typename R>
                static R* GetResource();

            private:
        };

        template <typename R, typename... Args>
        static R* GetResource(R, Args...);

    private:

};


// class MemoryDirector {
//     friend ResourceDirector<MemoryDirector>;

//     public:
//         MemoryDirector() = delete;

//         static MemoryDirector* GetInstance();

//         template <typename R, typename... Args>
//         static R* GetResource(R, Args...);

//     private:



// };

} // end of CHONS namespace