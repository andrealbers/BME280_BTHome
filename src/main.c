/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// Von bme280
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>

// von BTHome
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/uuid.h>

// von battery
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "battery.h"

// von BTHome
#define SERVICE_DATA_LEN 12
#define SERVICE_UUID 0xfcd2 /* BTHome service UUID */

#define IDX_TEMPL 4 /* Index of lo byte of temp in service data*/
#define IDX_TEMPH 5 /* Index of hi byte of temp in service data*/

#define IDX_HUML 7 // Index lo byte fuer Luftfeuchtigkeit
#define IDX_HUMH 8 // Index hi byte fuer Luftfeuchtigkeit

// #define IDX_PRESSL 10 // Index lo byte fuer Luftdruck
// #define IDX_PRESSM 11 // Index mittleres byte fuer Luftdruck
// #define IDX_PRESSH 12 // Index hi byte fuer Luftdruck

#define IDX_VOLTAGEL 10
#define IDX_VOLTAGEH 11

#define ADV_PARAM BT_LE_ADV_PARAM(BT_LE_ADV_OPT_CONNECTABLE, \
								  BT_GAP_ADV_SLOW_INT_MIN,   \
								  BT_GAP_ADV_SLOW_INT_MAX, NULL)

static uint8_t service_data[SERVICE_DATA_LEN] = {
	BT_UUID_16_ENCODE(SERVICE_UUID),
	0x40,

	0x02, /* Temperature */
	0x00, /* Low byte */
	0x00, /* High byte */

	0x03, /* Humidity */
	0x00, /* 50.55%  low byte*/
	0x00, /* 50.55%  high byte*/

	// 0x04, // Luftdruck
	// 0x00, // Low byte
	// 0x00, // Mittleres Byte
	// 0x00,  // High Byte

	0x4A, // Batteriespannung
	0x00, // Low byte
	0x00  // High Byte
};

static struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR),
	BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME) - 1),
	BT_DATA(BT_DATA_SVC_DATA16, service_data, ARRAY_SIZE(service_data))};

static void bt_ready(int err)
{
	if (err)
	{
		return;
	}

	/* Start advertising */
	err = bt_le_adv_start(ADV_PARAM, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err)
	{
		return;
	}
}

static const struct device *get_bme280_device(void)
{
	const struct device *const dev = DEVICE_DT_GET_ANY(bosch_bme280);

	if (dev == NULL)
	{
		return NULL;
	}

	if (!device_is_ready(dev))
	{
		return NULL;
	}

	return dev;
}

int main(void)
{
	// BME280
	const struct device *dev = get_bme280_device();

	if (dev == NULL)
	{
		return 0;
	}

	int err;
	int temp_data = 0;
	int hum_data = 0;
	// int press_data = 0;

	// Battery
	int rc = battery_measure_enable(true);

	if (rc != 0)
	{
		return 0;
	}

	// Loop
	for (;;)
	{
		// BME280 lesen
		struct sensor_value temp, press, humidity;

		sensor_sample_fetch(dev);
		sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
		// sensor_channel_get(dev, SENSOR_CHAN_PRESS, &press);
		sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &humidity);

		temp_data = temp.val1 * 100 + temp.val2 / 10000;
		hum_data = humidity.val1 * 100 + humidity.val2 / 10000;
		// press_data = press.val1 * 1000 + press.val2 / 100000; // Druck wird in kPa gemessen, statt hPa

		// Battery lesen
		int batt_mV = battery_sample();
		int batt_2digits = (batt_mV + 50) / 100;

		service_data[IDX_TEMPL] = temp_data & 0xff;
		service_data[IDX_TEMPH] = temp_data >> 8;

		service_data[IDX_HUML] = hum_data & 0xff;
		service_data[IDX_HUMH] = hum_data >> 8;

		// service_data[IDX_PRESSL] = press_data & 0xff;
		// service_data[IDX_PRESSM] = (press_data >> 8) & 0xff;
		// service_data[IDX_PRESSH] = (press_data >> 16) & 0xff;

		service_data[IDX_VOLTAGEL] = batt_2digits & 0xff;
		service_data[IDX_VOLTAGEH] = (batt_2digits >> 8) & 0xff;

		err = bt_enable(bt_ready);
		k_sleep(K_SECONDS(30));

		err = bt_le_adv_update_data(ad, ARRAY_SIZE(ad), NULL, 0);
		k_sleep(K_SECONDS(30));

		err = bt_le_adv_stop();
		k_sleep(K_SECONDS(30));

		err = bt_disable();
		k_sleep(K_MSEC(900000));
		
	}
	return 0;
}
