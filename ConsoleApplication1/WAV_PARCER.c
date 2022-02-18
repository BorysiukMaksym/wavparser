#include "WAV_PARCER.h"
#include <stdlib.h>
#include <tchar.h>
#include <conio.h>
#include <stdio.h>

#define MODULE_LOG_LVL      LOG_LVL_INFO
#define LOG_TAG             "[main] "
#include "logger.h"

#define HEAD_SIZE 44						/** Number of header bits. Uses for first check/**/
#define FILE_DIFFERENCE 8					/**Number of diferet bits between real file size and size in header.**/
#define CHANK_ID_STR        "RIFF"			/**Audio format name. Ussing in last check and allow work onle with WAV files.**/
#define READING_SHIFT 44					/**Marks the cursor to the specified number of characters**/
#define READING_SIZE 88						/**Using for reading file by parts**/
#define DATA_SPACE 4						/**Using for corect number of output information**/

#define SAMPLE_LEN		2


typedef struct 
{
	char           chunkId[4];		         /**Including "RIFF" symbols(8 bits)*/
	unsigned long  chunkSize;		         /**Size of the rest file witiut first 8 biits. **/
	char           format[4];			     /**Including "WAVE" symbols**/
	char           subchunk1Id[4];			 /**Including "fmt" symbols**/
	unsigned long  subchunk1Size;			 /**Size of the rest file**/
	unsigned short audioFormat;				 /**Audio format of special compresion**/
	unsigned short numChannels;				 /**Number of ausio chanels. Mono = 1, Stereo > 2**/
	unsigned long  sampleRate;				 /**Sampling frequency**/
	unsigned long  byteRate;				 /**The number of bytes per sample**/
	unsigned short blockAlign;				 /**Size of 1 sample**/
	unsigned short bitsPerSample;			 /**Bits per sample**/
	char           subchunk2Id[4];			 /**Including "data" symbols **/
	unsigned long  subchunk2Size;			 /**Size of data**/
} wav_header_t;

/******************************************************************
*\param [in] id_int equal to the header->chunkId what have size of all file.
*\param [in] CHANK_ID_STR the name of the structure inherent in the WAV file
*Check if information from header equal to the allowed symbolls "RIFF".
*If it's matches then value take True and proram go to the next step.
*Else program stops and show message about eror.
********************************************************************/
static bool WAVcheñk(char* id_str) {
	return memcmp(id_str, CHANK_ID_STR, 4) != 0 ? false : true;
}

/******************************************************************
*\param [in] FILE_DIFFERENCE contain standart  8 biits which makr diference.
*\param [in] id_int equal to the header->chunkSize what have size of all file.
*\param [in] file_size size of file what count in the program.
*\return true or false.
*Check if read file size matches with the information in chunkSize in header.
*Also add first 8 bits which are not taken into account.
*If it's matches then value take True and proram go to the next step.
*Else program stops and show message about eror.
********************************************************************/
static bool Sizecheck(int id_int, int fsize) {
	if (fsize == id_int + FILE_DIFFERENCE) return true;
	else return false;
}

/******************************************************************
*\param [in] HEAD_SIZE contain standart  44 biits
*\param [in] sizeof(wav_header_t) Ñounts the number of bits in the structure
*\return true or false
*Check if read file header size matches with the standart 44 bits.
*If it's matches then value take True and proram go to the next step.
*Else program stops and show message about eror.
********************************************************************/
static bool Headercheñk() {
	if (sizeof(wav_header_t) == HEAD_SIZE) return  true;
	else  return false;
}

static void Display(char* file_name, wav_header_t* header) {
	LOG_INFO("#Name: %s", file_name);
	LOG_INFO("#Format info: %c%c%c%c", header->chunkId[0], header->chunkId[1], header->chunkId[2], header->chunkId[3]);
	LOG_INFO("#Data format : %c%c%c", header->format[0], header->format[1], header->format[2]);
	LOG_INFO("#Size: %d", header->chunkSize);
	LOG_INFO("#Channels: %d", header->numChannels);
	LOG_INFO("#Discrit frequency : %d", header->sampleRate);
	LOG_INFO("#Bits per sample: %d", header->bitsPerSample);
	LOG_INFO("#Samples:   %d", header->subchunk2Size / 2);
}
wav_parser_ret_t wav_parser(char* wavfilename, char* csvfilename, uint32_t buf_len)
{
	wav_parser_ret_t res = WAV_PARSER_OK;
	FILE* File_Out = NULL;
	FILE* File_In = NULL;
	uint8_t* buf_header = NULL;
	uint8_t* buf_data = NULL;
	uint16_t* buf_data_samples = NULL;
	size_t ret = 0;
	float fDurationSeconds = 0.0;
	int iDurationMinutes = 0;
	int d = 0;
	wav_header_t* header = NULL;
	uint8_t* data = NULL;
	uint16_t* data_samples = NULL;
	uint8_t ChanelNumber = 0;
	uint32_t samples_lost = 0;
	uint32_t data_pieces = 0;
	uint32_t bytes_to_read = 0;
	uint32_t bytes_lost = 0;
	uint16_t bytes_per_sample = 0;

	if (buf_len % 2 == 1) {
		LOG_ERROR("3 argument must be even number");
		res = WAV_PARSER_WRONG_HEADER;
		goto exit;
	}

	// Call the size check function of header.
	if (Headercheñk() == false) {
		LOG_ERROR("Innvalid header");
		res = WAV_PARSER_WRONG_HEADER;
		goto exit;
	}

	// Open files and show eror if there some problems what related with files.
	File_In = fopen(wavfilename, "rb");
	File_Out = fopen(csvfilename, "wb");
	if (File_In == NULL) {
		LOG_ERROR("File can't open");
		res = WAV_PARSER_OPEN_FAILED_ERROR;
		goto exit;
	}
	if (File_Out == NULL) {
		LOG_ERROR("Can`t create data file");
		res = WAV_PARSER_CREATE_ERROR;
		goto exit;
	}

	//The size of the open file is being calculated
	fseek(File_In, 0, SEEK_END);
	int file_size = ftell(File_In);
	fseek(File_In, 0, SEEK_SET);

	//Create buffer for data
	buf_data = (uint8_t*)malloc(buf_len);
	if (buf_data == NULL) {
		LOG_ERROR("No memory");
		res = WAV_PARSER_MEMORY_END;
		goto exit;
	}
	buf_data_samples = (uint16_t *)buf_data;


	//Create buffer for header
	buf_header = (uint8_t*)malloc(HEAD_SIZE);
	if (buf_header == NULL) {
		LOG_ERROR("No memory");
		res = WAV_PARSER_MEMORY_END;
		goto exit;
	}
	header = (wav_header_t*)buf_header;
	data = buf_header + sizeof(wav_header_t);
	data_samples = (uint16_t*)data;

	//Read iformation from header. Shows eror if return sum incorect.
	ret = fread_s(buf_header, HEAD_SIZE, HEAD_SIZE, 1, File_In);
	if (ret != 1) {
		LOG_ERROR("Reading error");
		res = WAV_PARSER_HEADER_READING_ERROR;
		goto exit;
	}

	//Check bits per sample
	if (header->bitsPerSample == 16) {
		bytes_per_sample = 2;
	}
	if (header->bitsPerSample == 8) {
		bytes_per_sample = 1;
	}
	if (header->bitsPerSample > 16) {
		LOG_ERROR("Only 16 or 8 bits per sample");
		res = WAV_PARSER_WRONG_BITS_PER_SECOND;
		goto exit;
	}

	//Call the size check function of file.Shows eror if size of file not match with information in header.
	if (Sizecheck(header->chunkSize, file_size) != true) {
		LOG_ERROR("Innvalid size");
		res = WAV_PARSER_INVAILD_SIZE;
		goto exit;
	}

	//Call the check function of file. Checks if this is a WAV file type.
	//Shows eror if size of file not match with information in header.
	if (WAVcheñk(header->chunkId) == false) {
		LOG_ERROR("Innvalid format");
		res = WAV_PARSER_INVAILD_FORMAT;
		goto exit;
	}

	/************************************************************
	*Counting duration of the audio file.
	*\param [in] header->subchunk2Size
	*\param [in] header->bitsPerSample
	*\param [in] header->numChannels
	*\param [in] header->sampleRate
	*\param [in] FILE_DIFFERENCE
	*/
	fDurationSeconds = 1.f * header->subchunk2Size / (header->bitsPerSample / FILE_DIFFERENCE) / header->numChannels / header->sampleRate;
	iDurationMinutes = (int)fDurationSeconds / 60;
	fDurationSeconds = fDurationSeconds - (iDurationMinutes * 60);

	//Show ifromation in console
	Display(wavfilename, header);

	ChanelNumber = (uint8_t)header->numChannels;
	d = 1;
	samples_lost = header->subchunk2Size / bytes_per_sample;
	bytes_lost = samples_lost * SAMPLE_LEN;
	data_pieces = 0;

	//Data reading and writing
	while (bytes_lost > 0) {

		fseek(File_In, HEAD_SIZE + (buf_len * data_pieces), SEEK_SET);

		bytes_to_read = bytes_lost > buf_len ? buf_len : bytes_lost;
		size_t ret = fread_s(buf_data, (size_t)buf_len, (size_t)bytes_to_read, 1, File_In);

		if (ret != 1) {
			LOG_ERROR("Reading error");
			res = WAV_PARSER_FILE_READING_ERROR;
			goto exit;
		}

		if (ChanelNumber == 1) {
			while (d == 1) {
				fprintf(File_Out, "Chanel# 1 \n");
				d++;
			}
			for (uint32_t i = 0; i < bytes_to_read / bytes_per_sample; i++)
			{
				fprintf(File_Out, "%d\n", buf_data_samples[i]);
			}
		}

		if (ChanelNumber > 1)
		{
			while (d == 1) {
				for (uint8_t i = 1; i <= ChanelNumber; i++) {
					fprintf(File_Out, "Chanel # %d;", i);
					d++;
				}
				fprintf(File_Out, "\n");
			}
			for (uint32_t i = 0; i < bytes_to_read / bytes_per_sample; i++)
			{
				if (i % ChanelNumber == 0) {
					fprintf(File_Out, "%d; \n", buf_data_samples[i]);
					i++;
				}
				fprintf(File_Out, "%d;", buf_data_samples[i]);
			}
		}

		data_pieces++;
		bytes_lost -= bytes_to_read;

	}

exit:
	if (File_In) {
		fclose(File_In);
	}

	if (File_Out) {
		fclose(File_Out);
	}

	if (buf_header) {
		free(buf_header);
	}

	return res;
}