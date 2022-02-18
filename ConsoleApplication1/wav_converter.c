#include <stdlib.h>
#include "WAV_PARCER.h"

#define MODULE_LOG_LVL      LOG_LVL_INFO
#define LOG_TAG             "[main] "
#include "logger.h"

int main(int argc, char* argv[])
{
    start:
    if (argc != 4) {
        LOG_ERROR("Write only 3 arguments \n");
        return WAV_PARSER_OPEN_FAILED_ERROR;
    }

    char* wavfilename = argv[1];
    char* csvfilename = argv[2];
    char* max_buffer_len_str = argv[3];
    
    LOG_INFO ("%i \n", (uint32_t)atoi(max_buffer_len_str));
    if ((uint32_t)atoi(max_buffer_len_str) <= 0) {
        LOG_ERROR ("Third argument must be only numbers\n");
        return WAV_PARSER_OPEN_FAILED_ERROR;
        goto start;
    }
    wav_parser_ret_t result = wav_parser(wavfilename, csvfilename, (uint32_t)atoi(max_buffer_len_str));

    return result == WAV_PARSER_OK ? 0 : -1;
}