#include "test_wav.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <tchar.h>
#include <conio.h>

#define MODULE_LOG_LVL      LOG_LVL_INFO
#define LOG_TAG             "[main] "
#include "logger.h"

#define STR 1024
#define HEAD_SIZE 44
#define BYTES_IN_SAMPLE 2
#pragma warning(disable : 4996)

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

/**************************************************
Function that implements sequential reading of CSV file.
**************************************************/

static bool Headercheck() {
    if (sizeof(wav_header_t) == HEAD_SIZE) return  true;
    else  return false;
}

static const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ";");
        tok && *tok;
        tok = strtok(line, "\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

test_wav_ret_t test_wav(char* wavfilename, char* csvfilename) {
    LOG_INFO ("Test mode");
    /**************************************************
    Âeclare variables and assign them initial values.
    **************************************************/
    test_wav_ret_t res = WAV_TEST_OK;
    FILE* File_In = NULL;
    FILE* File_WAV = NULL;
    FILE* File_Data = NULL;
    uint32_t str_cntr = 0;
    uint16_t  ChanelNumber = 2;
    uint32_t WAV_s = 0;
    uint32_t* buffer_total = NULL;
    uint32_t* buffer = NULL;
    uint32_t* WAV_buf = NULL;
    uint32_t i = 0;
    size_t ret = 0;
    uint16_t cntr = 0;
    uint32_t p = 0;
    uint16_t n = 0;
    char line[STR];
    wav_header_t header;
    uint8_t fseek_ret = 0;

    // Call the size check function of header.
    if (Headercheck() == false) {
        LOG_ERROR("Innvalid header");
        res = WAV_TEST_WRONG_HEADER;
        goto exit;
    }

    if (!wavfilename || !csvfilename) {
        LOG_ERROR("filename NULL");
        res = WAV_TEST_NULL_ERROR;
        goto exit;
    }

    File_In = fopen(csvfilename, "r");
    File_WAV = fopen(wavfilename, "rb");
    File_Data = fopen(csvfilename, "r");
    /**************************************************
    Files open check.
    **************************************************/
    if (!File_In) {
        LOG_ERROR("failed to open file %s", csvfilename);
        res = WAV_TEST_OPEN_FAILED_ERROR;
        goto exit;
    }
    if (!File_WAV) {
        LOG_ERROR("failed to open file %s", wavfilename);
        res = WAV_TEST_OPEN_FAILED_ERROR; 
        goto exit;
    } 
    if (!File_Data) {
        LOG_ERROR("failed to open file %s", csvfilename);
        res = WAV_TEST_OPEN_FAILED_ERROR;
        goto exit;
    }
    /**************************************************   
    Calculate WAV file size.
    **************************************************/
    fseek_ret = fseek(File_WAV, 0, SEEK_END);
    if (fseek_ret != 0) {
        LOG_ERROR("Failed to check file size");
        res = WAV_TEST_FSEEK_ERROR;
        goto exit;
    }
    int file_size = ftell(File_WAV);
    fseek_ret = fseek(File_WAV, 0, SEEK_SET);
    if (fseek_ret != 0) {
        LOG_ERROR("Failed to check file size");
        res = WAV_TEST_FSEEK_ERROR;
        goto exit;
    }
    LOG_DEBUG("file_size %d", file_size);
    /**************************************************
    Calculate number of lines in CSV file.
    **************************************************/
    while (!feof(File_In))
    {
        if (fgetc(File_In) == '\n') {
            str_cntr++;
        }
    }

    WAV_s = str_cntr * BYTES_IN_SAMPLE * ChanelNumber * sizeof(uint16_t) + sizeof(wav_header_t);   //calculate size of read buffer
    LOG_DEBUG("WAV_s = %d\n", WAV_s);
    buffer_total = (uint32_t*)malloc(WAV_s);                                        //creat reading buffer for CSV file
    buffer = buffer_total + (sizeof(wav_header_t) / sizeof(uint32_t));              //We make a special buffer with an offset inside the main buffer
    LOG_DEBUG("%p %p\n", buffer_total, buffer);
    WAV_buf = (uint32_t*)malloc(file_size);                                         //creat buffer for the reading of  WAV file 
    LOG_DEBUG("WAV file size %s %d", wavfilename, file_size);
    if (WAV_buf == NULL) {
        LOG_ERROR("No memory");
        res = WAV_TEST_MEMORY_END;
        goto exit;
    }
    if (buffer_total == NULL) {
        LOG_ERROR("No memory");
        res = WAV_TEST_MEMORY_END;
        goto exit;
    }
    /**************************************************
    Reading information from CSV file to the the buffer
    **************************************************/
    while (fgets(line, str_cntr, File_Data)) {
        if (strstr(line, "Chanel")) {
            continue;
        }
        
        for (uint32_t ch = 0; ch < ChanelNumber; ch++) {
            char* tmp = strdup(line);
            const char* field = getfield(line, ch + 1);
            //LOG_ERROR("%d", i);
            if (!field) {
                LOG_ERROR("field error");
                return -1;
            }
            buffer[i] = (uint16_t)atoi(field);
            LOG_DEBUG("buf[%d] = %d", i, buffer[i]);
            free(tmp);
            i++;
        }
    }
    
    /**************************************************
    Reading information from WAV file to the the buffer
    **************************************************/
    ret = fread_s(WAV_buf, file_size, 1, file_size, File_WAV);

    if (ret != file_size) {
        LOG_ERROR("Reading error %d", ret);
        res = WAV_TEST_READING_ERROR;
        goto exit;
    }
    /**************************************************
    Copy header information 
    **************************************************/
    memcpy(buffer_total, WAV_buf, file_size);
    /**************************************************
    *Displays information which contains inside buffer.
    *Can be used for finding wrong byte
    **************************************************/
    for (p < file_size / sizeof(uint16_t); p++;) {
        LOG_DEBUG("%d", cntr++);
        LOG_DEBUG("%d             ", buffer_total[p]);
        LOG_DEBUG("%d", WAV_buf[p]);
    }
    /**************************************************
    *Compares all information which contains inside buffers.
     * @return 0 if everything coincides, 1 or -1 if one buffers bigger than another.
    **************************************************/

    n = memcmp(WAV_buf, buffer_total, file_size);
    if (n != 0) {
        LOG_INFO("Test is not successful");
    }
    if (n == 0) {
        LOG_INFO("Test is successful");
    }
    LOG_DEBUG("%d", n);
exit:
    if (File_In) {
        fclose(File_In);
    }
    if (File_WAV) {
        fclose(File_WAV);
    }
    if (buffer_total) {
        free(buffer_total);
    }
    if (WAV_buf) {
        free(WAV_buf);
    }
    system("pause");
    return res;
}