/*
 * Copyright (C) 2022 Gunar Schorcht
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#pragma once

/**
 * @ingroup     cpu_esp32
 * @{
 *
 * @file
 * @brief       Wrapper for source code compatibility of ESP-IDF log with RIOT's log module
 *
 * @author      Gunar Schorcht <gunar@schorcht.net>
 * @}
 */

#ifndef DOXYGEN     /* Hide implementation details from doxygen */

#ifdef __cplusplus
extern "C" {
#endif

#include "log.h"
#include_next "esp_log.h"

#if defined(RIOT_VERSION)

#include "esp_common.h"

#ifndef LOG_LOCAL_LEVEL
#define LOG_LOCAL_LEVEL LOG_LEVEL
#endif

#define ESP_LOG_LEVEL(level, tag, format, ...) \
            do { \
                if ((esp_log_level_t)level==ESP_LOG_ERROR ) { \
                    ESP_LOGE(tag, format, ##__VA_ARGS__);  \
                } \
                else if ((esp_log_level_t)level==ESP_LOG_WARN ) { \
                    ESP_LOGW(tag, format, ##__VA_ARGS__);  \
                } \
                else if ((esp_log_level_t)level==ESP_LOG_INFO ) { \
                    ESP_LOGI(tag, format, ##__VA_ARGS__);  \
                } \
                else if ((esp_log_level_t)level==ESP_LOG_DEBUG ) { \
                    ESP_LOGD(tag, format, ##__VA_ARGS__);  \
                } \
                else if ((esp_log_level_t)level==ESP_LOG_VERBOSE ) { \
                    ESP_LOGV(tag, format, ##__VA_ARGS__);  \
                } \
            } while (0)

#define ESP_LOG_LEVEL_LOCAL(level, tag, format, ...) \
            do { \
                if ( LOG_LOCAL_LEVEL >= level ) { \
                    ESP_LOG_LEVEL(level, tag, format, ##__VA_ARGS__); \
                } \
            } while (0)

#define ESP_LOGE(tag, format, ...) esp_log_write((esp_log_level_t)LOG_ERROR  , tag, format "\n", ##__VA_ARGS__)
#define ESP_LOGW(tag, format, ...) esp_log_write((esp_log_level_t)LOG_WARNING, tag, format "\n", ##__VA_ARGS__)
#define ESP_LOGI(tag, format, ...) esp_log_write((esp_log_level_t)LOG_INFO   , tag, format "\n", ##__VA_ARGS__)
#define ESP_LOGD(tag, format, ...) esp_log_write((esp_log_level_t)LOG_DEBUG  , tag, format "\n", ##__VA_ARGS__)
#define ESP_LOGV(tag, format, ...) esp_log_write((esp_log_level_t)LOG_ALL    , tag, format "\n", ##__VA_ARGS__)

#if MODULE_ESP_LOG_TAGGED

#define ESP_DRAM_LOG_LEVEL(level, letter, tag, format, ...) \
            do { \
                if ((esp_log_level_t)LOG_LOCAL_LEVEL >= level ) { \
                    esp_rom_printf(DRAM_STR(LOG_FORMAT(letter, format)), \
                                   system_get_time_ms(), ##__VA_ARGS__); \
                }\
            } while (0U)

#else

#define ESP_DRAM_LOG_LEVEL(level, letter, tag, format, ...) \
            do { \
                if ((esp_log_level_t)LOG_LOCAL_LEVEL >= level ) { \
                    esp_rom_printf(DRAM_STR(LOG_FORMAT(letter, format)), \
                                   ##__VA_ARGS__); \
                }\
            } while (0U)

#endif

#define ESP_DRAM_LOGE(tag, format, ...) ESP_DRAM_LOG_LEVEL(LOG_ERROR  , E, tag, format "\n", ##__VA_ARGS__)
#define ESP_DRAM_LOGW(tag, format, ...) ESP_DRAM_LOG_LEVEL(LOG_WARNING, W, tag, format "\n", ##__VA_ARGS__)
#define ESP_DRAM_LOGI(tag, format, ...) ESP_DRAM_LOG_LEVEL(LOG_INFO   , I, tag, format "\n", ##__VA_ARGS__)
#define ESP_DRAM_LOGD(tag, format, ...) ESP_DRAM_LOG_LEVEL(LOG_DEBUG  , D, tag, format "\n", ##__VA_ARGS__)
#define ESP_DRAM_LOGV(tag, format, ...) ESP_DRAM_LOG_LEVEL(LOG_ALL    , V, tag, format "\n", ##__VA_ARGS__)

#endif /* defined(RIOT_VERSION) */

#ifdef __cplusplus
}
#endif

#endif /* DOXYGEN */
