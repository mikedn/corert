// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

extern "C" void CoreLibNative_GetRandomBytes(uint8_t* buffer, int32_t bufferLength)
{
    assert(buffer != NULL);

    int rand_des;
    char buf;
    long num = 0;

    static bool sMissingDevRandom;
    static bool sMissingDevURandom;
    static bool sInitializedMRand;

    int i = 0;

    if (i < bufferLength && !sMissingDevRandom)
    {
        // request non-blocking access to avoid hangs if the /dev/random is exhausted
        // or just simply broken
        if ((rand_des = open("/dev/random", O_RDONLY | O_NONBLOCK)) == -1)
        {
            if (errno == ENOENT)
            {
                sMissingDevRandom = true;
            }

            // Back off and try /dev/urandom.
        }
        else
        {
            for( ; i < bufferLength; i++)
            {
                if (read(rand_des, &buf, 1) < 1)
                {
                    // the /dev/random pool has been exhausted.  Fall back
                    // to /dev/urandom for the remainder of the buffer.
                    break;
                }

                *(buffer + i) ^= buf;
            }

            close(rand_des);
        }
    }
 
    if (i < bufferLength && !sMissingDevURandom)
    {
        if ((rand_des = open("/dev/urandom", O_RDONLY, 0)) == -1)
        {
            if (errno == ENOENT)
            {                
                sMissingDevURandom = true;
            }

            // Back off and try mrand48.
        }
        else
        {
            for( ; i < bufferLength; i++)
            {
                if (read(rand_des, &buf, 1) < 1)
                {
                    // Fall back to srand48 for the remainder of the buffer.
                    break;
                }

                *(buffer + i) ^= buf;
            }

            close(rand_des);
        }
    }

    if (!sInitializedMRand)
    {
        srand48(time(NULL));
        sInitializedMRand = true;
    }

    // always xor srand48 over the whole buffer to get some randomness
    // in case /dev/random is not really random

    for(i = 0; i < bufferLength; i++)
    {
        if (i % sizeof(long) == 0)
        {
            num = mrand48();
        }

        *(buffer + i) ^= num;
        num >>= 8;
    }
}
