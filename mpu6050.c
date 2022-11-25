/*_____________________________________________________________________________
 │                                                                            |
 │ COPYRIGHT (C) 2022 Mihai Baneu                                             |
 │                                                                            |
 | Permission is hereby  granted,  free of charge,  to any person obtaining a |
 | copy of this software and associated documentation files (the "Software"), |
 | to deal in the Software without restriction,  including without limitation |
 | the rights to  use, copy, modify, merge, publish, distribute,  sublicense, |
 | and/or sell copies  of  the Software, and to permit  persons to  whom  the |
 | Software is furnished to do so, subject to the following conditions:       |
 |                                                                            |
 | The above  copyright notice  and this permission notice  shall be included |
 | in all copies or substantial portions of the Software.                     |
 |                                                                            |
 | THE SOFTWARE IS PROVIDED  "AS IS",  WITHOUT WARRANTY OF ANY KIND,  EXPRESS |
 | OR   IMPLIED,   INCLUDING   BUT   NOT   LIMITED   TO   THE  WARRANTIES  OF |
 | MERCHANTABILITY,  FITNESS FOR  A  PARTICULAR  PURPOSE AND NONINFRINGEMENT. |
 | IN NO  EVENT SHALL  THE AUTHORS  OR  COPYRIGHT  HOLDERS  BE LIABLE FOR ANY |
 | CLAIM, DAMAGES OR OTHER LIABILITY,  WHETHER IN AN ACTION OF CONTRACT, TORT |
 | OR OTHERWISE, ARISING FROM,  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR  |
 | THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                 |
 |____________________________________________________________________________|
 |                                                                            |
 |  Author: Mihai Baneu                           Last modified: 26.Oct.2022  |
 |                                                                            |
 |___________________________________________________________________________*/


#include <stdint.h>
#include "mpu6050.h"

static mpu6050_hw_control_t hw;

static void write_register(const uint8_t reg, const uint8_t value)
{
    const uint8_t data[2] = { reg, value };
    hw.data_wr(hw.address, data, sizeof(data));
}

static void read(uint8_t reg, uint8_t *data, uint16_t size)
{
    hw.data_wr(hw.address, &reg, 1);
    hw.data_rd(hw.address, data, size);
}

void mpu6050_init(mpu6050_hw_control_t hw_control)
{
    hw = hw_control;

    write_register(MPU6050_RA_GYRO_CONFIG,  0x00);  // ± 250 °/s
    write_register(MPU6050_RA_ACCEL_CONFIG, 0x00); // ± 2g
    write_register(MPU6050_RA_PWR_MGMT_1,   0x01);   // PLL with X axis gyroscope reference
}

mpu6050_data_t mpu6050_get_motion()
{
    mpu6050_data_t motion;
    uint8_t buffer[14];
    
    read(MPU6050_RA_ACCEL_XOUT_H, buffer, sizeof(buffer));

    motion.ax   = ((int16_t)(((uint16_t)buffer[ 0] << 8) | (uint16_t)buffer[ 1]))/16384.0F;
    motion.ay   = ((int16_t)(((uint16_t)buffer[ 2] << 8) | (uint16_t)buffer[ 3]))/16384.0F;
    motion.az   = ((int16_t)(((uint16_t)buffer[ 4] << 8) | (uint16_t)buffer[ 5]))/16384.0F;
    motion.temp = ((int16_t)(((uint16_t)buffer[ 6] << 8) | (uint16_t)buffer[ 5]))/340.0F + 36.53F;
    motion.gx   = ((int16_t)(((uint16_t)buffer[ 8] << 8) | (uint16_t)buffer[ 9]))/131.0F;
    motion.gy   = ((int16_t)(((uint16_t)buffer[10] << 8) | (uint16_t)buffer[11]))/131.0F;
    motion.gz   = ((int16_t)(((uint16_t)buffer[12] << 8) | (uint16_t)buffer[13]))/131.0F;

    return motion;
}
