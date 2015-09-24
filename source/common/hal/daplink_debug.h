/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#ifndef DAPLINK_DEBUG_H
#define DAPLINK_DEBUG_H

#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "RTL.h"
#include "rl_usb.h"

#ifdef __cplusplus
 extern "C" {
#endif

#ifndef MSC_DEBUG
//#define MSC_DEBUG
#endif

#if defined (MSC_DEBUG)

static inline uint32_t daplink_debug(uint8_t *buf, uint32_t size)
{
    uint32_t size_sent = 0;
    while (1) {
        size_sent += USBD_CDC_ACM_DataSend(buf + size_sent, size - size_sent);
        if (size_sent >= size) {
            break;
        }
        os_dly_wait(1);
    }
    return size_sent;
}

char buf[128] = {0};
static inline uint32_t daplink_debug_print(const char* format, ...)
{
    uint32_t ret;
    int32_t r = 0;
    va_list arg;
    ret = 1;
    
    va_start(arg, format);
    r = vsnprintf(buf, sizeof(buf), format, arg);
    if(r >= sizeof(buf)) {
        r = snprintf(buf, sizeof(buf), "<Error - string length %i exceeds print buffer>\r\n", r);
        ret = 0;
    }
    va_end(arg);
    daplink_debug((uint8_t*)buf, r);
    
    return ret;
}

#else

static inline uint32_t daplink_debug_print(const char* format, ...)
{
    return 1;
}

static inline uint32_t daplink_debug(uint8_t *data, uint32_t size)
{
    return 1;
}

#endif

#define debug_msg(fmt, args...) daplink_debug_print(fmt, ## args);
#define debug_data(buf, size) daplink_debug(buf, size);

#ifdef __cplusplus
 }
#endif

#endif
