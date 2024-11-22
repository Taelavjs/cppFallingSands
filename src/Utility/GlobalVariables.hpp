#include <string>
#ifndef GLOBALVARIABLES

#define GLOBALVARIABLES

class GlobalVariables {
public:
    const static inline int fps{60};
    const static inline int chunkSize{48 * 4};
    const static inline int rendererScale{3};
    const static inline std::string title{"NoitaB"};

    static inline float getDeltaTime(){
        return deltaTime;
    }

    static inline void setDeltaTime(float time){
        deltaTime = time;
    }


private:
    static inline float deltaTime{0};
    
};

#endif /* */
