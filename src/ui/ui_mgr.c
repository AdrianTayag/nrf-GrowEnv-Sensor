/*
 * ui_mgr.c
 * Copyright (c) 2024 Adrian Tayag
 *
 * SPDX-License-Identifier: MIT
 */

#include "ui_mgr.h"

void uiMgr_start(void)
{
    printk("UI Mgr Started\n");
    for(;;)
    {
        k_msleep(1000);
    }
}