/*
 * SPDX-FileCopyrightText: 2019 Benjamin Valentin
 * SPDX-License-Identifier: LGPL-2.1-only
 */

/**
 * @ingroup     boards_common_weact-f4x1cx
 * @{
 *
 * @file
 * @brief       Board initialization code for the WeAct-F4x1Cx board.
 *
 * @author      Benjamin Valentin <benpicco@googlemail.com>
 *
 * @}
 */

#include "board.h"
#include "cpu.h"
#include "mtd.h"
#include "mtd_spi_nor.h"
#include "periph/gpio.h"
#include "timex.h"

#ifdef MODULE_MTD
/* AT25SF041 */
static const mtd_spi_nor_params_t _weact_nor_params = {
    .opcode = &mtd_spi_nor_opcode_default,
    .wait_chip_erase   = 4800LU * US_PER_MS,
    .wait_32k_erase    = 300LU * US_PER_MS,
    .wait_sector_erase = 70LU * US_PER_MS,
    .wait_chip_wake_up = 1LU * US_PER_MS,
    .clk  = WEACT_4X1CX_NOR_SPI_CLK,
    .flag = WEACT_4X1CX_NOR_FLAGS,
    .spi  = WEACT_4X1CX_NOR_SPI_DEV,
    .mode = WEACT_4X1CX_NOR_SPI_MODE,
    .cs   = WEACT_4X1CX_NOR_SPI_CS,
    .wp   = GPIO_UNDEF,
    .hold = GPIO_UNDEF,
};

static mtd_spi_nor_t weact_nor_dev = {
    .base = {
        .driver = &mtd_spi_nor_driver,
        .page_size = WEACT_4X1CX_NOR_PAGE_SIZE,
        .pages_per_sector = WEACT_4X1CX_NOR_PAGES_PER_SECTOR,
    },
    .params = &_weact_nor_params,
};

MTD_XFA_ADD(weact_nor_dev, 0);

#ifdef MODULE_VFS_DEFAULT
#include "vfs_default.h"
VFS_AUTO_MOUNT(littlefs2, VFS_MTD(weact_nor_dev), VFS_DEFAULT_NVM(0), 0);
#endif
#endif /* MODULE_MTD */
