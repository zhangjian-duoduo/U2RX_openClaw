/**
 * Copyright (c) 2015-2019 Shanghai Fullhan Microelectronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-04-09     songyh    the first version
 *
 */

#ifndef __PLATFORM_DEF_H__
#define __PLATFORM_DEF_H__

#ifdef CONFIG_CHIP_FH8862
#include "fh8862/fh8862_board_def.h"
#endif

#ifdef CONFIG_CHIP_MC632X
#include "mc632x/mc632x_board_def.h"
#endif

#ifdef CONFIG_CHIP_FH8856V500
#include "fh8856v500/fh8856v500_board_def.h"
#endif

#ifdef CONFIG_CHIP_FH8857V500
#include "fh8857v500/fh8857v500_board_def.h"
#endif

#ifdef CONFIG_CHIP_FH8626V300
#include "fh8626v300/fh8626v300_board_def.h"
#endif

#endif /* TEST_H_ */
