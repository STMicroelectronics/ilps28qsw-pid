/*
 ******************************************************************************
 * @file    ilps28qsw_reg.c
 * @author  Sensors Software Solution Team
 * @brief   ILPS28QSW driver file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "ilps28qsw_reg.h"

/**
  * @defgroup    ILPS28QSW
  * @brief       This file provides a set of functions needed to drive the
  *              ilps28qsw nano pressure sensor.
  * @{
  *
  */

/**
  * @defgroup    Interfaces_Functions
  * @brief       This section provide a set of functions used to read and
  *              write a generic register of the device.
  *              MANDATORY: return 0 -> no Error.
  * @{
  *
  */

/**
  * @brief  Read generic device register
  *
  * @param  ctx   read / write interface definitions(ptr)
  * @param  reg   register to read
  * @param  data  pointer to buffer that store the data read(ptr)
  * @param  len   number of consecutive register to read
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t __weak ilps28qsw_read_reg(const stmdev_ctx_t *ctx, uint8_t reg, uint8_t *data,
                                  uint16_t len)
{
  int32_t ret;

  if (ctx == NULL)
  {
    return -1;
  }

  ret = ctx->read_reg(ctx->handle, reg, data, len);

  return ret;
}

/**
  * @brief  Write generic device register
  *
  * @param  ctx   read / write interface definitions(ptr)
  * @param  reg   register to write
  * @param  data  pointer to data to write in register reg(ptr)
  * @param  len   number of consecutive register to write
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t __weak ilps28qsw_write_reg(const stmdev_ctx_t *ctx, uint8_t reg, uint8_t *data,
                                   uint16_t len)
{
  int32_t ret;

  if (ctx == NULL)
  {
    return -1;
  }

  ret = ctx->write_reg(ctx->handle, reg, data, len);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  Private_functions
  * @brief     Section collect all the utility functions needed by APIs.
  * @{
  *
  */

static void bytecpy(uint8_t *target, uint8_t *source)
{
  if ((target != NULL) && (source != NULL))
  {
    *target = *source;
  }
}

/**
  * @}
  *
  */

/**
  * @defgroup    Sensitivity
  * @brief       These functions convert raw-data into engineering units.
  * @{
  *
  */

float_t ilps28qsw_from_fs1260_to_hPa(int32_t lsb)
{
  return ((float_t)lsb / 1048576.0f);   /* 4096.0f * 256 */
}

float_t ilps28qsw_from_fs4060_to_hPa(int32_t lsb)
{
  return ((float_t)lsb /  524288.0f);   /* 2048.0f * 256 */
}

float_t ilps28qsw_from_lsb_to_celsius(int16_t lsb)
{
  return ((float_t)lsb / 100.0f);
}

float_t ilps28qsw_from_lsb_to_mv(int32_t lsb)
{
  return ((float_t)lsb) / 426000.0f;
}

/**
  * @}
  *
  */

/**
  * @defgroup    Basic functions
  * @brief       This section groups all the functions concerning device basic
  *              configuration.
  * @{
  *
  */

/**
  * @brief  Device "Who am I".[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   ID values.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_id_get(const stmdev_ctx_t *ctx, ilps28qsw_id_t *val)
{
  uint8_t reg;
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_WHO_AM_I, &reg, 1);
  val->whoami = reg;

  return ret;
}

/**
  * @brief  Configures the bus operating mode.[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   configures the bus operating mode.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_bus_mode_set(const stmdev_ctx_t *ctx, ilps28qsw_bus_mode_t *val)
{
  ilps28qsw_i3c_if_ctrl_t i3c_if_ctrl;
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_I3C_IF_CTRL,
                           (uint8_t *)&i3c_if_ctrl, 1);
  if (ret == 0)
  {
    i3c_if_ctrl.asf_on = (uint8_t)val->filter & 0x01U;
    i3c_if_ctrl.I3C_Bus_Avb_Sel = (uint8_t)val->bus_avb_time & 0x03U;
    ret = ilps28qsw_write_reg(ctx, ILPS28QSW_I3C_IF_CTRL,
                              (uint8_t *)&i3c_if_ctrl, 1);
  }

  return ret;
}

/**
  * @brief  Configures the bus operating mode.[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   configures the bus operating mode.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_bus_mode_get(const stmdev_ctx_t *ctx, ilps28qsw_bus_mode_t *val)
{
  ilps28qsw_i3c_if_ctrl_t i3c_if_ctrl;
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_I3C_IF_CTRL, (uint8_t *)&i3c_if_ctrl, 1);
  if (ret == 0)
  {
    switch (i3c_if_ctrl.asf_on)
    {
      case ILPS28QSW_AUTO:
        val->filter = ILPS28QSW_AUTO;
        break;
      case ILPS28QSW_ALWAYS_ON:
        val->filter = ILPS28QSW_ALWAYS_ON;
        break;
      default:
        val->filter = ILPS28QSW_AUTO;
        break;
    }

    switch (i3c_if_ctrl.I3C_Bus_Avb_Sel)
    {
      case ILPS28QSW_BUS_AVB_TIME_50us:
        val->bus_avb_time = ILPS28QSW_BUS_AVB_TIME_50us;
        break;
      case ILPS28QSW_BUS_AVB_TIME_2us:
        val->bus_avb_time = ILPS28QSW_BUS_AVB_TIME_2us;
        break;
      case ILPS28QSW_BUS_AVB_TIME_1ms:
        val->bus_avb_time = ILPS28QSW_BUS_AVB_TIME_1ms;
        break;
      case ILPS28QSW_BUS_AVB_TIME_25ms:
        val->bus_avb_time = ILPS28QSW_BUS_AVB_TIME_25ms;
        break;
      default:
        val->bus_avb_time = ILPS28QSW_BUS_AVB_TIME_50us;
        break;
    }
  }

  return ret;
}

/**
  * @brief  Configures the bus operating mode.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   configures the bus operating mode.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_init_set(const stmdev_ctx_t *ctx, ilps28qsw_init_t val)
{
  ilps28qsw_ctrl_reg2_t ctrl_reg2;
  ilps28qsw_ctrl_reg3_t ctrl_reg3;
  uint8_t reg[2];
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_CTRL_REG2, reg, 2);
  if (ret == 0)
  {
    bytecpy((uint8_t *)&ctrl_reg2, &reg[0]);
    bytecpy((uint8_t *)&ctrl_reg3, &reg[1]);

    switch (val)
    {
      case ILPS28QSW_BOOT:
        ctrl_reg2.boot = PROPERTY_ENABLE;
        ret = ilps28qsw_write_reg(ctx, ILPS28QSW_CTRL_REG2,
                                  (uint8_t *)&ctrl_reg2, 1);
        break;
      case ILPS28QSW_RESET:
        ctrl_reg2.swreset = PROPERTY_ENABLE;
        ret = ilps28qsw_write_reg(ctx, ILPS28QSW_CTRL_REG2,
                                  (uint8_t *)&ctrl_reg2, 1);
        break;
      case ILPS28QSW_DRV_RDY:
        ctrl_reg2.bdu = PROPERTY_ENABLE;
        ctrl_reg3.if_add_inc = PROPERTY_ENABLE;
        bytecpy(&reg[0], (uint8_t *)&ctrl_reg2);
        bytecpy(&reg[1], (uint8_t *)&ctrl_reg3);
        ret = ilps28qsw_write_reg(ctx, ILPS28QSW_CTRL_REG2, reg, 2);
        break;
      default:
        ctrl_reg2.swreset = PROPERTY_ENABLE;
        ret = ilps28qsw_write_reg(ctx, ILPS28QSW_CTRL_REG2,
                                  (uint8_t *)&ctrl_reg2, 1);
        break;
    }
  }
  return ret;
}

/**
  * @brief  Get the status of the device.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   the status of the device.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_status_get(const stmdev_ctx_t *ctx, ilps28qsw_stat_t *val)
{
  ilps28qsw_interrupt_cfg_t interrupt_cfg;
  ilps28qsw_int_source_t int_source;
  ilps28qsw_ctrl_reg2_t ctrl_reg2;
  ilps28qsw_status_t status;
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_CTRL_REG2,
                           (uint8_t *)&ctrl_reg2, 1);
  if (ret == 0)
  {
    ret = ilps28qsw_read_reg(ctx, ILPS28QSW_INT_SOURCE, (uint8_t *)&int_source, 1);
  }
  if (ret == 0)
  {
    ret = ilps28qsw_read_reg(ctx, ILPS28QSW_STATUS, (uint8_t *)&status, 1);
  }
  if (ret == 0)
  {
    ret = ilps28qsw_read_reg(ctx, ILPS28QSW_INTERRUPT_CFG,
                             (uint8_t *)&interrupt_cfg, 1);
  }
  val->sw_reset  = ctrl_reg2.swreset;
  val->boot      = int_source.boot_on;
  val->drdy_pres = status.p_da;
  val->drdy_temp = status.t_da;
  val->ovr_pres  = status.p_or;
  val->ovr_temp  = status.t_or;
  val->end_meas  = ~ctrl_reg2.oneshot;
  val->ref_done = ~interrupt_cfg.autozero;

  return ret;
}

/**
  * @brief  Electrical pin configuration.[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   the electrical settings for the configurable pins.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_pin_conf_set(const stmdev_ctx_t *ctx, ilps28qsw_pin_conf_t *val)
{
  ilps28qsw_if_ctrl_t if_ctrl;
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_IF_CTRL, (uint8_t *)&if_ctrl, 1);

  if (ret == 0)
  {
    if_ctrl.sda_pu_en = val->sda_pull_up;
    ret = ilps28qsw_write_reg(ctx, ILPS28QSW_IF_CTRL, (uint8_t *)&if_ctrl, 1);
  }

  return ret;
}

/**
  * @brief  Electrical pin configuration.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   the electrical settings for the configurable pins.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_pin_conf_get(const stmdev_ctx_t *ctx, ilps28qsw_pin_conf_t *val)
{
  ilps28qsw_if_ctrl_t if_ctrl;
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_IF_CTRL, (uint8_t *)&if_ctrl, 1);
  val->sda_pull_up  = if_ctrl.sda_pu_en;

  return ret;
}

/**
  * @brief  Get the status of all the interrupt sources.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   the status of all the interrupt sources.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_all_sources_get(const stmdev_ctx_t *ctx,
                                  ilps28qsw_all_sources_t *val)
{
  ilps28qsw_fifo_status2_t fifo_status2;
  ilps28qsw_int_source_t int_source;
  ilps28qsw_status_t status;
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_STATUS, (uint8_t *)&status, 1);
  if (ret == 0)
  {
    ret = ilps28qsw_read_reg(ctx, ILPS28QSW_INT_SOURCE,
                             (uint8_t *)&int_source, 1);
  }
  if (ret == 0)
  {
    ret = ilps28qsw_read_reg(ctx, ILPS28QSW_FIFO_STATUS2,
                             (uint8_t *)&fifo_status2, 1);
  }

  val->drdy_pres        = status.p_da;
  val->drdy_temp        = status.t_da;
  val->over_pres        = int_source.ph;
  val->under_pres       = int_source.pl;
  val->thrsld_pres      = int_source.ia;
  val->fifo_full        = fifo_status2.fifo_full_ia;
  val->fifo_ovr         = fifo_status2.fifo_ovr_ia;
  val->fifo_th          = fifo_status2.fifo_wtm_ia;

  return ret;
}


/**
  * @brief  Sensor conversion parameters selection.[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   set the sensor conversion parameters.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_mode_set(const stmdev_ctx_t *ctx, ilps28qsw_md_t *val)
{
  ilps28qsw_ctrl_reg1_t ctrl_reg1;
  ilps28qsw_ctrl_reg2_t ctrl_reg2;
  ilps28qsw_ctrl_reg3_t ctrl_reg3;
  ilps28qsw_fifo_ctrl_t fifo_ctrl;
  uint8_t odr_save = 0, ah_qvar_en_save = 0;
  uint8_t reg[3];
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_CTRL_REG1, reg, 3);

  if (ret == 0)
  {
    bytecpy((uint8_t *)&ctrl_reg1, &reg[0]);
    bytecpy((uint8_t *)&ctrl_reg2, &reg[1]);
    bytecpy((uint8_t *)&ctrl_reg3, &reg[2]);

    /* handle interleaved mode setting */
    if (ctrl_reg1.odr != 0x0U)
    {
      /* power-down */
      odr_save = ctrl_reg1.odr;
      ctrl_reg1.odr = 0x0U;
      ret += ilps28qsw_write_reg(ctx, ILPS28QSW_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
    }

    if (ctrl_reg3.ah_qvar_en != 0U)
    {
      /* disable QVAR */
      ah_qvar_en_save = ctrl_reg3.ah_qvar_en;
      ctrl_reg3.ah_qvar_en = 0;
      ret += ilps28qsw_write_reg(ctx, ILPS28QSW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
    }

    /* set interleaved mode (0 or 1) */
    ctrl_reg3.ah_qvar_p_auto_en = val->interleaved_mode;
    ret += ilps28qsw_write_reg(ctx, ILPS28QSW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

    /* set FIFO interleaved mode (0 or 1) */
    ret += ilps28qsw_read_reg(ctx, ILPS28QSW_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);
    fifo_ctrl.ah_qvar_p_fifo_en = val->interleaved_mode;
    ret += ilps28qsw_write_reg(ctx, ILPS28QSW_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);

    if (ah_qvar_en_save != 0U)
    {
      /* restore ah_qvar_en back to previous setting */
      ctrl_reg3.ah_qvar_en = ah_qvar_en_save;
    }

    if (odr_save != 0U)
    {
      /* restore odr back to previous setting */
      ctrl_reg1.odr = odr_save;
    }

    ctrl_reg1.odr = (uint8_t)val->odr;
    ctrl_reg1.avg = (uint8_t)val->avg;
    ctrl_reg2.en_lpfp = (uint8_t)val->lpf & 0x01U;
    ctrl_reg2.lfpf_cfg = ((uint8_t)val->lpf & 0x02U) >> 1;
    ctrl_reg2.fs_mode = (uint8_t)val->fs;

    bytecpy(&reg[0], (uint8_t *)&ctrl_reg1);
    bytecpy(&reg[1], (uint8_t *)&ctrl_reg2);
    bytecpy(&reg[2], (uint8_t *)&ctrl_reg3);
    ret += ilps28qsw_write_reg(ctx, ILPS28QSW_CTRL_REG1, reg, 3);
  }

  return ret;
}

/**
  * @brief  Sensor conversion parameters selection.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   get the sensor conversion parameters.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_mode_get(const stmdev_ctx_t *ctx, ilps28qsw_md_t *val)
{
  ilps28qsw_ctrl_reg1_t ctrl_reg1;
  ilps28qsw_ctrl_reg2_t ctrl_reg2;
  ilps28qsw_ctrl_reg3_t ctrl_reg3;
  uint8_t reg[3];
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_CTRL_REG1, reg, 3);

  if (ret == 0)
  {
    bytecpy((uint8_t *)&ctrl_reg1, &reg[0]);
    bytecpy((uint8_t *)&ctrl_reg2, &reg[1]);
    bytecpy((uint8_t *)&ctrl_reg3, &reg[2]);

    switch (ctrl_reg2.fs_mode)
    {
      case ILPS28QSW_1260hPa:
        val->fs = ILPS28QSW_1260hPa;
        break;
      case ILPS28QSW_4060hPa:
        val->fs = ILPS28QSW_4060hPa;
        break;
      default:
        val->fs = ILPS28QSW_1260hPa;
        break;
    }

    switch (ctrl_reg1.odr)
    {
      case ILPS28QSW_ONE_SHOT:
        val->odr = ILPS28QSW_ONE_SHOT;
        break;
      case ILPS28QSW_1Hz:
        val->odr = ILPS28QSW_1Hz;
        break;
      case ILPS28QSW_4Hz:
        val->odr = ILPS28QSW_4Hz;
        break;
      case ILPS28QSW_10Hz:
        val->odr = ILPS28QSW_10Hz;
        break;
      case ILPS28QSW_25Hz:
        val->odr = ILPS28QSW_25Hz;
        break;
      case ILPS28QSW_50Hz:
        val->odr = ILPS28QSW_50Hz;
        break;
      case ILPS28QSW_75Hz:
        val->odr = ILPS28QSW_75Hz;
        break;
      case ILPS28QSW_100Hz:
        val->odr = ILPS28QSW_100Hz;
        break;
      case ILPS28QSW_200Hz:
        val->odr = ILPS28QSW_200Hz;
        break;
      default:
        val->odr = ILPS28QSW_ONE_SHOT;
        break;
    }

    switch (ctrl_reg1.avg)
    {
      case ILPS28QSW_4_AVG:
        val->avg = ILPS28QSW_4_AVG;
        break;
      case ILPS28QSW_8_AVG:
        val->avg = ILPS28QSW_8_AVG;
        break;
      case ILPS28QSW_16_AVG:
        val->avg = ILPS28QSW_16_AVG;
        break;
      case ILPS28QSW_32_AVG:
        val->avg = ILPS28QSW_32_AVG;
        break;
      case ILPS28QSW_64_AVG:
        val->avg = ILPS28QSW_64_AVG;
        break;
      case ILPS28QSW_128_AVG:
        val->avg = ILPS28QSW_128_AVG;
        break;
      case ILPS28QSW_256_AVG:
        val->avg = ILPS28QSW_256_AVG;
        break;
      case ILPS28QSW_512_AVG:
        val->avg = ILPS28QSW_512_AVG;
        break;
      default:
        val->avg = ILPS28QSW_4_AVG;
        break;
    }

    switch ((ctrl_reg2.lfpf_cfg << 2) | ctrl_reg2.en_lpfp)
    {
      case ILPS28QSW_LPF_DISABLE:
        val->lpf = ILPS28QSW_LPF_DISABLE;
        break;
      case ILPS28QSW_LPF_ODR_DIV_4:
        val->lpf = ILPS28QSW_LPF_ODR_DIV_4;
        break;
      case ILPS28QSW_LPF_ODR_DIV_9:
        val->lpf = ILPS28QSW_LPF_ODR_DIV_9;
        break;
      default:
        val->lpf = ILPS28QSW_LPF_DISABLE;
        break;
    }

    val->interleaved_mode = ctrl_reg3.ah_qvar_p_auto_en;
  }
  return ret;
}

/**
  * @brief  Software trigger for One-Shot.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  md    the sensor conversion parameters.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_trigger_sw(const stmdev_ctx_t *ctx, ilps28qsw_md_t *md)
{
  ilps28qsw_ctrl_reg2_t ctrl_reg2;
  int32_t ret = 0;

  if (md->odr == ILPS28QSW_ONE_SHOT)
  {
    ret = ilps28qsw_read_reg(ctx, ILPS28QSW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
    ctrl_reg2.oneshot = PROPERTY_ENABLE;
    if (ret == 0)
    {
      ret = ilps28qsw_write_reg(ctx, ILPS28QSW_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
    }
  }
  return ret;
}

/**
  * @brief  AH/QVAR function enable.[set]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Change the value of ah_qvar_en in reg CTRL_REG3
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t ilps28qsw_ah_qvar_en_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  ilps28qsw_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.ah_qvar_en = val;
    ret = ilps28qsw_write_reg(ctx, ILPS28QSW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief  AH/QVAR function enable.[get]
  *
  * @param  ctx    Read / write interface definitions
  * @param  val    Return the value of ah_qvar_en in reg CTRL_REG3
  * @retval        Interface status (MANDATORY: return 0 -> no Error).
  *
  */
int32_t ilps28qsw_ah_qvar_en_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  ilps28qsw_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  *val = ctrl_reg3.ah_qvar_en;

  return ret;
}

/**
  * @brief  Retrieve sensor data.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  md    the sensor conversion parameters.(ptr)
  * @param  data  data retrived from the sensor.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_data_get(const stmdev_ctx_t *ctx, ilps28qsw_md_t *md,
                           ilps28qsw_data_t *data)
{
  uint8_t buff[5];
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_PRESS_OUT_XL, buff, 5);

  /* pressure conversion */
  data->pressure.raw = (int32_t)buff[2];
  data->pressure.raw = (data->pressure.raw * 256) + (int32_t) buff[1];
  data->pressure.raw = (data->pressure.raw * 256) + (int32_t) buff[0];
  data->pressure.raw = data->pressure.raw * 256;

  if (md->interleaved_mode == 1U)
  {
    if ((buff[0] & 0x1U) == 0U)
    {
      /* data is a pressure sample */
      switch (md->fs)
      {
        case ILPS28QSW_1260hPa:
          data->pressure.hpa = ilps28qsw_from_fs1260_to_hPa(data->pressure.raw);
          break;
        case ILPS28QSW_4060hPa:
          data->pressure.hpa = ilps28qsw_from_fs4060_to_hPa(data->pressure.raw);
          break;
        default:
          data->pressure.hpa = 0.0f;
          break;
      }
      data->ah_qvar.lsb = 0;
    }
    else
    {
      /* data is a AH_QVAR sample */
      data->ah_qvar.lsb = (data->pressure.raw / 256); /* shift 8bit left */
      data->pressure.hpa = 0.0f;
    }
  }
  else
  {
    switch (md->fs)
    {
      case ILPS28QSW_1260hPa:
        data->pressure.hpa = ilps28qsw_from_fs1260_to_hPa(data->pressure.raw);
        break;
      case ILPS28QSW_4060hPa:
        data->pressure.hpa = ilps28qsw_from_fs4060_to_hPa(data->pressure.raw);
        break;
      default:
        data->pressure.hpa = 0.0f;
        break;
    }
    data->ah_qvar.lsb = 0;
  }

  /* temperature conversion */
  data->heat.raw = (int16_t)buff[4];
  data->heat.raw = (data->heat.raw * 256) + (int16_t) buff[3];
  data->heat.deg_c = ilps28qsw_from_lsb_to_celsius(data->heat.raw);

  return ret;
}

/**
  * @brief  Pressure output value.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  buff     buffer that stores data read
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_pressure_raw_get(const stmdev_ctx_t *ctx, uint32_t *buff)
{
  int32_t ret;
  uint8_t reg[3];

  ret =  ilps28qsw_read_reg(ctx, ILPS28QSW_PRESS_OUT_XL, reg, 3);
  *buff = reg[2];
  *buff = (*buff * 256U) + reg[1];
  *buff = (*buff * 256U) + reg[0];
  *buff *= 256U;

  return ret;
}

/**
  * @brief  Temperature output value.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  buff     buffer that stores data read
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_temperature_raw_get(const stmdev_ctx_t *ctx, int16_t *buff)
{
  int32_t ret;
  uint8_t reg[2];

  ret =  ilps28qsw_read_reg(ctx, ILPS28QSW_TEMP_OUT_L, reg, 2);
  *buff = (int16_t)reg[1];
  *buff = (*buff * 256) + (int16_t)reg[0];

  return ret;
}

/**
  * @brief  AH/QVAR data read.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  md    the sensor conversion parameters.(ptr)
  * @param  data  data retrived from the sensor.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_ah_qvar_data_get(const stmdev_ctx_t *ctx,
                                   ilps28qsw_ah_qvar_data_t *data)
{
  uint8_t buff[5];
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_PRESS_OUT_XL, buff, 3);

  /* QVAR conversion */
  data->raw = (int32_t)buff[2];
  data->raw = (data->raw * 256) + (int32_t) buff[1];
  data->raw = (data->raw * 256) + (int32_t) buff[0];
  data->raw = (data->raw * 256);
  data->lsb = (data->raw / 256); /* shift 8bit left */

  data->mv = ilps28qsw_from_lsb_to_mv(data->lsb);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup     FIFO functions
  * @brief        This section groups all the functions concerning the
  *               management of FIFO.
  * @{
  *
  */

/**
  * @brief  FIFO operation mode selection.[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   set the FIFO operation mode.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_fifo_mode_set(const stmdev_ctx_t *ctx, ilps28qsw_fifo_md_t *val)
{
  ilps28qsw_fifo_ctrl_t fifo_ctrl;
  ilps28qsw_fifo_wtm_t fifo_wtm;
  uint8_t reg[2];
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_FIFO_CTRL, reg, 2);
  if (ret == 0)
  {
    bytecpy((uint8_t *)&fifo_ctrl, &reg[0]);
    bytecpy((uint8_t *)&fifo_wtm, &reg[1]);

    fifo_ctrl.f_mode = (uint8_t)val->operation & 0x03U;
    fifo_ctrl.trig_modes = ((uint8_t)val->operation & 0x04U) >> 2;

    if (val->watermark != 0x00U)
    {
      fifo_ctrl.stop_on_wtm = PROPERTY_ENABLE;
    }
    else
    {
      fifo_ctrl.stop_on_wtm = PROPERTY_DISABLE;
    }

    fifo_wtm.wtm = val->watermark;

    bytecpy(&reg[0], (uint8_t *)&fifo_ctrl);
    bytecpy(&reg[1], (uint8_t *)&fifo_wtm);

    ret = ilps28qsw_write_reg(ctx, ILPS28QSW_FIFO_CTRL, reg, 2);
  }
  return ret;
}

/**
  * @brief  FIFO operation mode selection.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   get the FIFO operation mode.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_fifo_mode_get(const stmdev_ctx_t *ctx, ilps28qsw_fifo_md_t *val)
{
  ilps28qsw_fifo_ctrl_t fifo_ctrl;
  ilps28qsw_fifo_wtm_t fifo_wtm;
  uint8_t reg[2];
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_FIFO_CTRL, reg, 2);

  bytecpy((uint8_t *)&fifo_ctrl, &reg[0]);
  bytecpy((uint8_t *)&fifo_wtm, &reg[1]);

  switch ((fifo_ctrl.trig_modes << 2) | fifo_ctrl.f_mode)
  {
    case ILPS28QSW_BYPASS:
      val->operation = ILPS28QSW_BYPASS;
      break;
    case ILPS28QSW_FIFO:
      val->operation = ILPS28QSW_FIFO;
      break;
    case ILPS28QSW_STREAM:
      val->operation = ILPS28QSW_STREAM;
      break;
    case ILPS28QSW_STREAM_TO_FIFO:
      val->operation = ILPS28QSW_STREAM_TO_FIFO;
      break;
    case ILPS28QSW_BYPASS_TO_STREAM:
      val->operation = ILPS28QSW_BYPASS_TO_STREAM;
      break;
    case ILPS28QSW_BYPASS_TO_FIFO:
      val->operation = ILPS28QSW_BYPASS_TO_FIFO;
      break;
    default:
      val->operation = ILPS28QSW_BYPASS;
      break;
  }

  val->watermark = fifo_wtm.wtm;

  return ret;
}

/**
  * @brief  Get the number of samples stored in FIFO.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  md    the sensor conversion parameters.(ptr)
  * @param  val   number of samples stored in FIFO.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_fifo_level_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  ilps28qsw_fifo_status1_t fifo_status1;
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_FIFO_STATUS1,
                           (uint8_t *)&fifo_status1, 1);

  *val = fifo_status1.fss;

  return ret;
}

/**
  * @brief  Software trigger for One-Shot.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  md    the sensor conversion parameters.(ptr)
  * @param  fmd   get the FIFO operation mode.(ptr)
  * @param  samp  number of samples stored in FIFO.(ptr)
  * @param  data  data retrived from FIFO.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_fifo_data_get(const stmdev_ctx_t *ctx, uint8_t samp,
                                ilps28qsw_md_t *md, ilps28qsw_fifo_data_t *data)
{
  uint8_t fifo_data[3];
  uint8_t i;
  int32_t ret = 0;

  for (i = 0U; i < samp; i++)
  {
    ret = ilps28qsw_read_reg(ctx, ILPS28QSW_FIFO_DATA_OUT_PRESS_XL, fifo_data, 3);
    data[i].raw = (int32_t)fifo_data[2];
    data[i].raw = (data[i].raw * 256) + (int32_t)fifo_data[1];
    data[i].raw = (data[i].raw * 256) + (int32_t)fifo_data[0];
    data[i].raw = (data[i].raw * 256);

    if (md->interleaved_mode == 1U)
    {
      if ((fifo_data[0] & 0x1U) == 0U)
      {
        /* data is a pressure sample */
        switch (md->fs)
        {
          case ILPS28QSW_1260hPa:
            data[i].hpa = ilps28qsw_from_fs1260_to_hPa(data[i].raw);
            break;
          case ILPS28QSW_4060hPa:
            data[i].hpa = ilps28qsw_from_fs4060_to_hPa(data[i].raw);
            break;
          default:
            data[i].hpa = 0.0f;
            break;
        }
        data[i].lsb = 0;
      }
      else
      {
        /* data is a AH_QVAR sample */
        data[i].lsb = (data[i].raw / 256); /* shift 8bit left */
        data[i].hpa = 0.0f;
      }
    }
    else
    {
      switch (md->fs)
      {
        case ILPS28QSW_1260hPa:
          data[i].hpa = ilps28qsw_from_fs1260_to_hPa(data[i].raw);
          break;
        case ILPS28QSW_4060hPa:
          data[i].hpa = ilps28qsw_from_fs4060_to_hPa(data[i].raw);
          break;
        default:
          data[i].hpa = 0.0f;
          break;
      }
      data[i].lsb = 0;
    }
  }

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup     Interrupt signals
  * @brief        This section groups all the functions concerning
  *               the management of interrupt signals.
  * @{
  *
  */

/**
  * @brief  Interrupt pins hardware signal configuration.[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   the pins hardware signal settings.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_interrupt_mode_set(const stmdev_ctx_t *ctx,
                                     ilps28qsw_int_mode_t *val)
{
  ilps28qsw_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_INTERRUPT_CFG,
                           (uint8_t *)&interrupt_cfg, 1);
  if (ret == 0)
  {
    interrupt_cfg.lir = val->int_latched ;
    ret = ilps28qsw_write_reg(ctx, ILPS28QSW_INTERRUPT_CFG,
                              (uint8_t *)&interrupt_cfg, 1);
  }
  return ret;
}

/**
  * @brief  Interrupt pins hardware signal configuration.[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   the pins hardware signal settings.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_interrupt_mode_get(const stmdev_ctx_t *ctx,
                                     ilps28qsw_int_mode_t *val)
{
  ilps28qsw_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_INTERRUPT_CFG,
                           (uint8_t *)&interrupt_cfg, 1);
  val->int_latched = interrupt_cfg.lir;

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup     Interrupt on threshold functions
  * @brief        This section groups all the functions concerning
  *               the wake up functionality.
  * @{
  *
  */

/**
  * @brief  Configuration of Wake-up and Wake-up to Sleep .[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   parameters of configuration.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_int_on_threshold_mode_set(const stmdev_ctx_t *ctx,
                                            ilps28qsw_int_th_md_t *val)
{
  ilps28qsw_interrupt_cfg_t interrupt_cfg;
  ilps28qsw_ths_p_l_t ths_p_l;
  ilps28qsw_ths_p_h_t ths_p_h;
  uint8_t reg[3];
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_INTERRUPT_CFG, reg, 3);
  if (ret == 0)
  {
    bytecpy((uint8_t *)&interrupt_cfg, &reg[0]);
    bytecpy((uint8_t *)&ths_p_l, &reg[1]);
    bytecpy((uint8_t *)&ths_p_h, &reg[2]);

    interrupt_cfg.phe = val->over_th;
    interrupt_cfg.ple = val->under_th;
    ths_p_h.ths = (uint8_t)(val->threshold / 256U);
    ths_p_l.ths = (uint8_t)(val->threshold - (ths_p_h.ths * 256U));

    bytecpy(&reg[0], (uint8_t *)&interrupt_cfg);
    bytecpy(&reg[1], (uint8_t *)&ths_p_l);
    bytecpy(&reg[2], (uint8_t *)&ths_p_h);

    ret = ilps28qsw_write_reg(ctx, ILPS28QSW_INTERRUPT_CFG, reg, 3);
  }
  return ret;
}

/**
  * @brief  Configuration of Wake-up and Wake-up to Sleep .[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   parameters of configuration.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_int_on_threshold_mode_get(const stmdev_ctx_t *ctx,
                                            ilps28qsw_int_th_md_t *val)
{
  ilps28qsw_interrupt_cfg_t interrupt_cfg;
  ilps28qsw_ths_p_l_t ths_p_l;
  ilps28qsw_ths_p_h_t ths_p_h;
  uint8_t reg[3];
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_INTERRUPT_CFG, reg, 3);

  bytecpy((uint8_t *)&interrupt_cfg, &reg[0]);
  bytecpy((uint8_t *)&ths_p_l, &reg[1]);
  bytecpy((uint8_t *)&ths_p_h, &reg[2]);

  val->over_th = interrupt_cfg.phe;
  val->under_th = interrupt_cfg.ple;
  val->threshold = ths_p_h.ths;
  val->threshold = (val->threshold * 256U)  + ths_p_l.ths;

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup     Reference value of pressure
  * @brief        This section groups all the functions concerning
  *               the wake up functionality.
  * @{
  *
  */

/**
  * @brief  Configuration of Wake-up and Wake-up to Sleep .[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   parameters of configuration.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_reference_mode_set(const stmdev_ctx_t *ctx, ilps28qsw_ref_md_t *val)
{
  ilps28qsw_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_INTERRUPT_CFG,
                           (uint8_t *)&interrupt_cfg, 1);
  if (ret == 0)
  {

    interrupt_cfg.autozero = val->get_ref;
    interrupt_cfg.autorefp = (uint8_t)val->apply_ref & 0x01U;

    interrupt_cfg.reset_az  = ((uint8_t)val->apply_ref & 0x02U) >> 1;
    interrupt_cfg.reset_arp = ((uint8_t)val->apply_ref & 0x02U) >> 1;

    ret = ilps28qsw_write_reg(ctx, ILPS28QSW_INTERRUPT_CFG,
                              (uint8_t *)&interrupt_cfg, 1);
  }
  return ret;
}

/**
  * @brief  Configuration of Wake-up and Wake-up to Sleep .[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   parameters of configuration.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_reference_mode_get(const stmdev_ctx_t *ctx, ilps28qsw_ref_md_t *val)
{
  ilps28qsw_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_INTERRUPT_CFG,
                           (uint8_t *)&interrupt_cfg, 1);

  switch ((interrupt_cfg.reset_az << 1) |
          interrupt_cfg.autorefp)
  {
    case ILPS28QSW_OUT_AND_INTERRUPT:
      val->apply_ref = ILPS28QSW_OUT_AND_INTERRUPT;
      break;
    case ILPS28QSW_ONLY_INTERRUPT:
      val->apply_ref = ILPS28QSW_ONLY_INTERRUPT;
      break;
    default:
      val->apply_ref = ILPS28QSW_RST_REFS;
      break;
  }
  val->get_ref = interrupt_cfg.autozero;

  return ret;
}

/**
  * @brief  Reference Pressure LSB data .[get]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   parameters of configuration.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_refp_get(const stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t reg[2];
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_REF_P_L, reg, 2);

  *val = (int16_t)reg[1];
  *val = *val * 256 + (int16_t)reg[0];

  return ret;
}

/**
  * @brief  Configuration of Wake-up and Wake-up to Sleep .[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   parameters of configuration.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_opc_set(const stmdev_ctx_t *ctx, int16_t val)
{
  uint8_t reg[2];
  int32_t ret;

  reg[1] = (uint8_t)(((uint16_t)val & 0xFF00U) / 256U);
  reg[0] = (uint8_t)((uint16_t)val & 0x00FFU);

  ret = ilps28qsw_write_reg(ctx, ILPS28QSW_RPDS_L, reg, 2);

  return ret;
}

/**
  * @brief  Configuration of Wake-up and Wake-up to Sleep .[set]
  *
  * @param  ctx   communication interface handler.(ptr)
  * @param  val   parameters of configuration.(ptr)
  * @retval       interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t ilps28qsw_opc_get(const stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t reg[2];
  int32_t ret;

  ret = ilps28qsw_read_reg(ctx, ILPS28QSW_RPDS_L, reg, 2);

  *val = (int16_t)reg[1];
  *val = *val * 256 + (int16_t)reg[0];

  return ret;
}


/**
  * @}
  *
  */

/**
  * @}
  *
  */
