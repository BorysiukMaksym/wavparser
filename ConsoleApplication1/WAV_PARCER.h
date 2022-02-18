/**************************************************************************
*Module uses for reading WAV file and writting data to the CSV file.
*There are 3 stages check for correct parcing of file.
*First of all it's cheсk  header size is it equal to 44 bits.
*Next check is file size. Checks if  real file size corresponds to the
*data from header of this file.
Last check corect format of the file. Because this module corect work
*only with WAV files.
*After thees checks information from header writes to the console, data wtites to the CSV file.
***************************************************************************/
#ifndef WAV_PARCER_H
#define WAV_PARCER_H

#include <stdint.h>

typedef enum {
	WAV_PARSER_OK = 0,
	WAV_PARSER_WRONG_HEADER = 1,
	WAV_PARSER_ARGYMENTS_ERROR = 2,
	WAV_PARSER_OPEN_FAILED_ERROR = 3,
	WAV_PARSER_CREATE_ERROR = 4,
	WAV_PARSER_MEMORY_END = 5,
	WAV_PARSER_HEADER_READING_ERROR = 6,
	WAV_PARSER_INVAILD_SIZE = 7,
	WAV_PARSER_INVAILD_FORMAT = 8,
	WAV_PARSER_FILE_READING_ERROR = 9,
	WAV_PARSER_BUFFER_ODD_NUMBER = 10,
	WAV_PARSER_WRONG_BITS_PER_SECOND = 11,
} wav_parser_ret_t;

/******************************************************************
*@beif Start of the main function which opens WAV file, check it's parametrs,
*what shows in console, opens data in special CSV file where we can see
*information in each chanel of audio file. Also it contains 3 stages check.
*Here we send 3 parametrs: first and second is names of file which must bee open, and third is size of buffer for data.
*@param [in] wavfilename name of WAV file from what data reads. (File_In)
*@param [in] csvfilename name of CSV file in what we write data. (File_Out)
*@param [in] buf_len size of buffer for data. It must be even.
*@return
*******************************************************************/
wav_parser_ret_t wav_parser(char* wavfilename, char* csvfilename, uint32_t buf_len);

#endif