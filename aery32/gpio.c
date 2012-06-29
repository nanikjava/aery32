/*   _____             ___ ___   |
    |  _  |___ ___ _ _|_  |_  |  |  Teh framework for 32-bit AVRs
    |     | -_|  _| | |_  |  _|  |  
    |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
                  |___|          |

    Copyright (c) 2012, Muiku Oy
    All rights reserved.

    LICENSE: This source file is subject to the new BSD license that is
    bundled with this package in the file LICENSE.txt. If you did not
    receive a copy of the license and are unable to obtain it through
    the world-wide-web, please send an email to contact@muiku.com so
    we can send you a copy.
*/

#include <inttypes.h>
#include <avr32/io.h>
#include "aery32/gpio.h"

#ifdef AERY_SHORTCUTS
	volatile avr32_gpio_port_t *porta = &AVR32_GPIO.port[0];
	volatile avr32_gpio_port_t *portb = &AVR32_GPIO.port[1];
	volatile avr32_gpio_port_t *portc = &AVR32_GPIO.port[2];

	volatile avr32_gpio_local_port_t *lporta = &AVR32_GPIO_LOCAL.port[0];
	volatile avr32_gpio_local_port_t *lportb = &AVR32_GPIO_LOCAL.port[1];
	volatile avr32_gpio_local_port_t *lportc = &AVR32_GPIO_LOCAL.port[2];
#endif

void
aery_gpio_init_pins(volatile avr32_gpio_port_t *pport, uint32_t pinmask,
                    int options)
{
	/* By default define pin to be controlled by the GPIO with output
	 * drivers disabled. */
	pport->gpers = pinmask;
	pport->oderc = pinmask;

	switch (options & 0007) {
	case 0002:		/* Output */
		pport->oders = pinmask;
		pport->ovrc = pinmask;
		break;
	case 0003:		/* Output high */
		pport->oders = pinmask;
		pport->ovrs = pinmask;
		break;

	/* Before the pin peripheral function can be assigned, the pin has to
	 * be controlled by the GPIO with output drivers disabled, as has been
	 * done by default. The pin can then be assured to be tri-stated while
	 * changing the Peripheral Mux Registers.
	 */
	case 0004:		/* Peripheral Function A */
		pport->pmr1c = pinmask;
		pport->pmr0c = pinmask;
		pport->gperc = pinmask;
		break;
	case 0005:		/* Peripheral Function B */
		pport->pmr1c = pinmask;
		pport->pmr0s = pinmask;
		pport->gperc = pinmask;
		break;
	case 0006:		/* Peripheral Function C */
		pport->pmr1s = pinmask;
		pport->pmr0c = pinmask;
		pport->gperc = pinmask;
		break;
	case 0007:		/* Peripheral Function D */
		pport->pmr1s = pinmask;
		pport->pmr0s = pinmask;
		pport->gperc = pinmask;
		break;
	}

	if (options & 0040) {
		/* Interrupt can be enabled on a pin, regardless of the configuration
		 * the I/O line, i.e. controlled by the GPIO or assigned to a
		 * peripheral function. [ds, p.173]
		 */
		switch (options & 0030) {
		case 0000:
			pport->imr0c = pinmask;
			pport->imr1c = pinmask;
			break;
		case 0010:
			pport->imr0s = pinmask;
			pport->imr1c = pinmask;
			break;
		case 0020:
			pport->imr0c = pinmask;
			pport->imr1s = pinmask;
			break;
		}
		pport->iers = pinmask;
	} else {
		pport->ierc = pinmask;
	}

	if (options & 0100) {
		/* Control of the pull-up resistor is possible whether an I/O line
		 * is controlled by a peripheral or the GPIO. */
		pport->puers = pinmask;
	} else {
		pport->puerc = pinmask;
	}

	if (options & 0200) {
		/* The open drain mode can be selected whether the I/O line is
		 * controlled by the GPIO or assigned to a peripheral function. */
		pport->odmers = pinmask;
	} else {
		pport->odmerc = pinmask;
	}

	if (options & 0400) {
		pport->gfers = pinmask;
	} else {
		pport->gferc = pinmask;
	}
}

void
aery_gpio_init_pin(uint8_t pinnum, int options)
{
	aery_gpio_init_pins(&AVR32_GPIO.port[GPIO_NUM2PORT(pinnum)],
		(1UL << GPIO_NUM2PIN(pinnum)), options);
}