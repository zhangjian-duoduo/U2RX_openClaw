/**
 * Copyright (c) 2015-2019 Shanghai Fullhan Microelectronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-01-24     tangyh    the first version
 *
 */

#ifndef __FH_CLK_H__
#define __FH_CLK_H__

#if defined(CONFIG_CHIP_FH8862)
#include "fh8862/clock_config.h"
#endif

#if defined(CONFIG_CHIP_MC632X)
#include "mc632x/clock_config.h"
#endif

#if defined(CONFIG_CHIP_FH8856V500)
#include "fh8856v500/clock_config.h"
#endif

#if defined(CONFIG_CHIP_FH8857V500)
#include "fh8857v500/clock_config.h"
#endif

#if defined(CONFIG_CHIP_FH8626V300)
#include "fh8626v300/clock_config.h"
#endif

#endif /* FH_CLK_H_ */
