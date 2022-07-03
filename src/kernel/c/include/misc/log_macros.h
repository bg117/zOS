/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#define KLOG(msg, ...) log_append_format_string("[%s]: " msg, __func__, ##__VA_ARGS__)
