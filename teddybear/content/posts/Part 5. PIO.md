---
title: "Part 5. PIO Introduction"
date: 2022-12-29T14:46:38-05:00
weight: 6
---
### Explaination 
This part is an explanation of the PIO part of the code, and it also explains why this makes rp2040 special.

### Why Does PIO Make RP2040 Special?

There are 2 identical PIO blocks in RP2040. Each PIO block has dedicated connections to the bus fabric, GPIO and interrupt controller. The diagram for a single PIO block is show below：

{{< figure src="/img/pio.png" width="800" align="center">}}

The programmable input/output block (PIO) is a versatile hardware interface, which supports a variety of IO standards. It is programmable in the same sense as a processor. There are two PIO blocks with four state machines each, that can independently execute sequential programs to manipulate GPIOs and transfer data. Unlike a general-purpose processor, PIO state machines are highly specialized for IO, with a focus on determinism, precise timing, and close integration with fixed-function hardware. 

Moreover, PIO state machines can be configured and reconfigured dynamically to implement numerous different interfaces. Making state machines programmable in a software-like manner, rather than a fully configurable logic fabric like a CPLD, allows more hardware interfaces to be offered in the same cost and power envelope. This also presents a more familiar programming model, and simpler tool flow, to those who wish to exploit PIO’s full flexibility by programming it directly, rather than using a premade interface from the PIO library.

State machines' inputs and outputs are mapped to up to 32 GPIOs (limited to 30 GPIOs for RP2040), and all state machines have independent, simultaneous access to any GPIO. The amount of freedom available depends on how exactly a given PIO program chooses to use PIO’s pin mapping resources, but at the minimum, an interface can be freely shifted up or down by some number of GPIOs.


### PIO Applied In Our Project

In this project, the PIO module is applied to drive the camera to capture the environmental gray value in real time. The data transferred between RP2040 and HM01b0 is through i2c. The PIO code is showing below:

```
.program image
.wrap_target

	wait 1 pin 9 // wait for hsync
	wait 1 pin 8 // wait for rising pclk
	in pins 1
	wait 0 pin 8
.wrap

% c-sdk {
void image_program_init(PIO pio, uint sm, uint offset, uint pin_base) {
	pio_sm_set_consecutive_pindirs(pio, sm, pin_base, 1, false);

	pio_sm_config c = image_program_get_default_config(offset);
	sm_config_set_in_pins(&c, pin_base);
	sm_config_set_in_shift(&c, false, true, 8);
	sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_RX);
	pio_sm_init(pio, sm, offset, &c);
	//pio_sm_set_enabled(pio, sm, true);
}
%}
```