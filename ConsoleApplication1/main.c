#include <stdlib.h>
#include "logger.h"
#include "WAV_PARCER.h"
#include "test_wav.h"
/**************************************
*Included files in which converts and then tests data.
*Included libraries which needs for work.
***************************************/
#define MODULE_LOG_LVL      LOG_LVL_INFO        //mode of displayed info
#define LOG_TAG             "[main] "

typedef enum {
    APP_MODE_PARSER,
    APP_MODE_TEST
} app_mode_t;

int main(int argc, char* argv[])
{
    if (argc == 1) {
        LOG_INFO("HELP");
        LOG_INFO("For working with this program you need write 4 arguments");
        LOG_INFO("First: Name of your WAV file");
        LOG_INFO("Second: Name of CSV file");
        LOG_INFO("Third: Size of buffer in bytes for reading information. It must be even.");
        LOG_INFO("Fourth: Mode of working: 0 is reading mode, 1 is testing mode.");
        return WAV_PARSER_OPEN_FAILED_ERROR;
    }
    /****************************************************
     * @brief function in what we chose file names     
     * and send to other module to convertt information.
     * @return 0 if everything is good, or -1 if meets problem.
     *Number and name of problem consist in .h files. 
    ****************************************************/
    if (argc != 5) {
        LOG_ERROR("Write only 4 arguments");
        return WAV_PARSER_OPEN_FAILED_ERROR;
    }
    /*****************************************************
     * @brief  when program startsyou must write:
     *1  name of WAV file from what you read (example.wav);
     *2  name of  CSV file in what you want to write information (example.csv);
     *3 size of reading buffer ( from 1 to max sizze of your RAM, must be couple);
     *4 Mode of work: 0 for Read mode(must be only WAV file);  1 for test mode(needs WAV and CSV files);
    */

    char* wavfilename = argv[1];
    char* csvfilename = argv[2];
    char* max_buffer_len_str = argv[3];
    char* test = argv[4];

    if ((uint32_t)atoi(test) != APP_MODE_PARSER && (uint32_t)atoi(test) != APP_MODE_TEST) {
        LOG_ERROR("Fourth argument must be 0 or 1");
        return WAV_PARSER_OPEN_FAILED_ERROR;
    }
    /**************************************************
     *checks if writen information was not text.
    **************************************************/
    if ((uint32_t)atoi(max_buffer_len_str) == 0) {
        LOG_ERROR ("Third argument must be only numbers");
        return WAV_PARSER_OPEN_FAILED_ERROR;
    }

    /**************************************************
    Chosing mode of work, 1 for Test and 0 fro Reading.
    **************************************************/
    if ((uint32_t)atoi(test) == APP_MODE_TEST) {
        test_wav_ret_t result =  test_wav(wavfilename, csvfilename);
        return result == WAV_TEST_OK ? 0 : -1;
        /**************************************************
         Sending names of files
        **************************************************/
    }
    else if ((uint32_t)atoi(test) == APP_MODE_PARSER) {
        wav_parser_ret_t result = wav_parser(wavfilename, csvfilename, (uint32_t)atoi(max_buffer_len_str));
        return result == WAV_PARSER_OK ? 0 : -1;
        /**************************************************
        Sending names of files amd size of reading buffer.
        **************************************************/
    }
    else {
        LOG_ERROR("Wrong mode. Must be only 1(test) or 0(read).");
        return -1;
    }
}