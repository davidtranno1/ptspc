//
// Created by alberto on 17/05/19.
//

#ifndef STOCHASTIC_TSP_DISABLE_STREAM_H
#define STOCHASTIC_TSP_DISABLE_STREAM_H

// For output redirection on Linux:
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <fcntl.h>

#ifndef DISABLE_STREAM
#define DISABLE_STREAM(stream, fno)                     \
        fflush(stream);                                 \
        int bak_##stream = dup(fno);                    \
        int new_##stream = open("/dev/null", O_WRONLY); \
        dup2(new_##stream, fno);                        \
        close(new_##stream);
#endif

#ifndef REENABLE_STREAM
#define REENABLE_STREAM(stream, fno)                   \
        fflush(stream);                                \
        dup2(bak_##stream, fno);                       \
        close(bak_##stream);
#endif

#endif //STOCHASTIC_TSP_DISABLE_STREAM_H
