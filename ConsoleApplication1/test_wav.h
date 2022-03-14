/**************************************************************************
* Module uses for testing.
* It reading raw data from WAV file and data from CSV file that we have from main module WAV_PARCER.
* If this files are same, we seen text in console "Test is successful", either text be "Test is not successful".
***************************************************************************/

#ifndef TEST_WAV_H
#define TEST_WAV_H

#include <stdint.h>

typedef enum {
	WAV_TEST_OK = 0,
	WAV_TEST_NULL_ERROR,
	WAV_TEST_OPEN_FAILED_ERROR,
	WAV_TEST_MEMORY_END,
	WAV_TEST_READING_ERROR,
	WAV_TEST_WRONG_HEADER,
	WAV_TEST_FSEEK_ERROR,
}   test_wav_ret_t;
/******************************************************************
*@brief Start of the test function which opens WAV file, and  CSV file from which 
*reads all information in special buffers. Which then checks for coincidence. 
*Here we send 2 parametrs: first and second is names of file which must bee open.
*@param [in] wavfilename name of WAV file from what data reads. (File_In)
*@param [in] csvfilename name of CSV file from what data reads. (File_Out), (File_Data)
*@return	WAV_TEST_OK - if everthing is good,
*			other value - error
*******************************************************************/
test_wav_ret_t test_wav(char* wavfilename, char* csvfilename);

#endif