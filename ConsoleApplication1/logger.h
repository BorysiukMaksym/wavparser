#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define LOG_LVL_DEBUG           0
#define LOG_LVL_INFO            1
#define LOG_LVL_WARNING         2
#define LOG_LVL_ERROR           3

#define SYS_LOG_LVL             LOG_LVL_INFO

//#define SYS_LOG_COLOR_EN

#define TEXT_COLOR_DEFAULT     "\033[0m"
#define TEXT_COLOR_RED           "\033[31m"
#define TEXT_COLOR_GREEN       "\033[32m"
#define TEXT_COLOR_YELLOW      "\033[33m"
#define TEXT_COLOR_BLUE          "\033[34m"
#define TEXT_COLOR_MAGENTA       "\033[35m"
#define TEXT_COLOR_CYAN         "\033[36m"
#define TEXT_COLOR_GRAY         "\033[37m"

#ifdef SYS_LOG_COLOR_EN
#define LOG_COLOR_DEBUG         TEXT_COLOR_DEFAULT
#define LOG_COLOR_INFO          TEXT_COLOR_GREEN
#define LOG_COLOR_WARNING       TEXT_COLOR_YELLOW
#define LOG_COLOR_ERROR         TEXT_COLOR_RED
#define LOG_COLOR_RESET         TEXT_COLOR_DEFAULT
#else /* SYS_LOG_COLOR_EN */
#define LOG_COLOR_DEBUG
#define LOG_COLOR_INFO
#define LOG_COLOR_WARNING
#define LOG_COLOR_ERROR
#define LOG_COLOR_RESET
#endif /* SYS_LOG_COLOR_EN */


#ifndef LOG_TAG
#define LOG_TAG               "app " 
#endif /* LOG_TAG */

#ifndef MODULE_LOG_LVL
#define MODULE_LOG_LVL           LOG_LVL_INFO
#endif /* MODULE_LOG_LVL */

#define DEBUG_STR               "[debug]"
#define INFO_STR                "[info]"
#define WARNING_STR             "[WARN]"
#define ERROR_STR               "[ERROR]"

#if (MODULE_LOG_LVL == LOG_LVL_DEBUG)
#define LOG_INTERNAL_DEBUG(FORMAT, ...)     printf(LOG_COLOR_DEBUG DEBUG_STR LOG_TAG FORMAT "\r\n" LOG_COLOR_RESET, ##__VA_ARGS__)
#define LOG_INTERNAL_INFO(FORMAT, ...)      printf(LOG_COLOR_INFO INFO_STR LOG_TAG FORMAT "\r\n" LOG_COLOR_RESET, ##__VA_ARGS__)
#define LOG_INTERNAL_WARNING(FORMAT, ...)   printf(LOG_COLOR_WARNING WARNING_STR LOG_TAG FORMAT "\r\n" LOG_COLOR_RESET, ##__VA_ARGS__)
#define LOG_INTERNAL_ERROR(FORMAT, ...)     printf(LOG_COLOR_ERROR ERROR_STR LOG_TAG FORMAT "\r\n" LOG_COLOR_RESET, ##__VA_ARGS__)
#elif (MODULE_LOG_LVL == LOG_LVL_INFO)
#define LOG_INTERNAL_DEBUG(FORMAT, ...)
#define LOG_INTERNAL_INFO(FORMAT, ...)      printf(LOG_COLOR_INFO INFO_STR LOG_TAG FORMAT "\r\n" LOG_COLOR_RESET, ##__VA_ARGS__)
#define LOG_INTERNAL_WARNING(FORMAT, ...)   printf(LOG_COLOR_WARNING WARNING_STR LOG_TAG FORMAT "\r\n" LOG_COLOR_RESET, ##__VA_ARGS__)
#define LOG_INTERNAL_ERROR(FORMAT, ...)     printf(LOG_COLOR_ERROR ERROR_STR LOG_TAG FORMAT "\r\n" LOG_COLOR_RESET, ##__VA_ARGS__)
#elif (MODULE_LOG_LVL == LOG_LVL_WARNING)
#define LOG_INTERNAL_DEBUG(FORMAT, ...)
#define LOG_INTERNAL_INFO(FORMAT, ...)
#define LOG_INTERNAL_WARNING(FORMAT, ...)   printf(LOG_COLOR_WARNING WARNING_STR LOG_TAG FORMAT "\r\n" LOG_COLOR_RESET, ##__VA_ARGS__)
#define LOG_INTERNAL_ERROR(FORMAT, ...)     printf(LOG_COLOR_ERROR ERROR_STR LOG_TAG FORMAT "\r\n" LOG_COLOR_RESET, ##__VA_ARGS__)
#elif (MODULE_LOG_LVL == LOG_LVL_ERROR)
#define LOG_INTERNAL_DEBUG(FORMAT, ...)
#define LOG_INTERNAL_INFO(FORMAT, ...)
#define LOG_INTERNAL_WARNING(FORMAT, ...)
#define LOG_INTERNAL_ERROR(FORMAT, ...)     printf(LOG_COLOR_ERROR ERROR_STR LOG_TAG FORMAT "\r\n" LOG_COLOR_RESET, ##__VA_ARGS__)
#endif /* MODULE_LOG_LVL */

#if (MODULE_LOG_LVL == LOG_LVL_DEBUG)
#define LOG_HEX_DUMP_INTERNAL_DEBUG(p_data, len)    printf(LOG_COLOR_DEBUG DEBUG_STR LOG_TAG); print_buf_hex(p_data, len)
#define LOG_HEX_DUMP_INTERNAL_INFO(p_data, len)     printf(LOG_COLOR_INFO INFO_STR LOG_TAG); print_buf_hex(p_data, len)
#define LOG_HEX_DUMP_INTERNAL_WARNING(p_data, len)  printf(LOG_COLOR_WARNING WARNING_STR LOG_TAG); print_buf_hex(p_data, len)
#define LOG_HEX_DUMP_INTERNAL_ERROR(p_data, len)    printf(LOG_COLOR_ERROR ERROR_STR LOG_TAG); print_buf_hex(p_data, len)
#elif (MODULE_LOG_LVL == LOG_LVL_INFO)
#define LOG_HEX_DUMP_INTERNAL_DEBUG(p_data, len)
#define LOG_HEX_DUMP_INTERNAL_INFO(p_data, len)     printf(LOG_COLOR_INFO INFO_STR LOG_TAG); print_buf_hex(p_data, len)
#define LOG_HEX_DUMP_INTERNAL_WARNING(p_data, len)  printf(LOG_COLOR_WARNING WARNING_STR LOG_TAG); print_buf_hex(p_data, len)
#define LOG_HEX_DUMP_INTERNAL_ERROR(p_data, len)    printf(LOG_COLOR_ERROR ERROR_STR LOG_TAG); print_buf_hex(p_data, len)
#elif (MODULE_LOG_LVL == LOG_LVL_WARNING)
#define LOG_HEX_DUMP_INTERNAL_DEBUG(p_data, len)
#define LOG_HEX_DUMP_INTERNAL_INFO(p_data, len)
#define LOG_HEX_DUMP_INTERNAL_WARNING(p_data, len)  printf(LOG_COLOR_WARNING WARNING_STR LOG_TAG); print_buf_hex(p_data, len)
#define LOG_HEX_DUMP_INTERNAL_ERROR(p_data, len)    printf(LOG_COLOR_ERROR ERROR_STR LOG_TAG); print_buf_hex(p_data, len)
#elif (MODULE_LOG_LVL == LOG_LVL_ERROR)
#define LOG_HEX_DUMP_INTERNAL_DEBUG(p_data, len)
#define LOG_HEX_DUMP_INTERNAL_INFO(p_data, len)
#define LOG_HEX_DUMP_INTERNAL_WARNING(p_data, len)
#define LOG_HEX_DUMP_INTERNAL_ERROR(p_data, len)    printf(LOG_COLOR_ERROR ERROR_STR LOG_TAG); print_buf_hex(p_data, len)
#endif /* MODULE_LOG_LVL */

#define LOG_DEBUG(FORMAT, ...)              LOG_INTERNAL_DEBUG(FORMAT, ##__VA_ARGS__)
#define LOG_INFO(FORMAT, ...)               LOG_INTERNAL_INFO(FORMAT, ##__VA_ARGS__)
#define LOG_WARNING(FORMAT, ...)            LOG_INTERNAL_WARNING(FORMAT, ##__VA_ARGS__)
#define LOG_ERROR(FORMAT, ...)              LOG_INTERNAL_ERROR(FORMAT, ##__VA_ARGS__)

#define LOG_HEX_DUMP_DEBUG(p_data, len)     LOG_HEX_DUMP_INTERNAL_DEBUG(p_data, len)
#define LOG_HEX_DUMP_INFO(p_data, len)      LOG_HEX_DUMP_INTERNAL_INFO(p_data, len)
#define LOG_HEX_DUMP_WARNING(p_data, len)   LOG_HEX_DUMP_INTERNAL_WARNING(p_data, len)
#define LOG_HEX_DUMP_ERROR(p_data, len)     LOG_HEX_DUMP_INTERNAL_ERROR(p_data, len)


void print_buf_hex(uint8_t *buf, uint32_t datalen);

#endif /* LOGGER_H_ */
