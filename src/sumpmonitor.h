#ifndef SUMPMONITOR
#define SUMPMONITOR

namespace Priority{
    const int Lowest = -2;
    const int Low = -1;
    const int Standard = 0;
    const int High = 1;
    const int Emergency = 2;
};

namespace Emergency{
    const int Retry = 600;
    const int Expire = 86400;
};

#endif //SUMPMONITOR
