#include <stdint.h>
#include <tm4c123gh6pm.h>

int main()
{
	/* Enable ports A and F */
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0 | SYSCTL_RCGCGPIO_R5;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA | SYSCTL_RCGC2_GPIOF;
	/* Enable SSI0 */
	SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R0;
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_SSI0;
	/* Wait for the ports to come online, and set them to digital mode */
	while ((SYSCTL_PRGPIO_R & (SYSCTL_PRGPIO_R0 | SYSCTL_PRGPIO_R5)) != (SYSCTL_PRGPIO_R0 | SYSCTL_PRGPIO_R5));
	GPIO_PORTF_DEN_R |= 0x0E;
	/* Wait for SSI0 to come online */
	while ((SYSCTL_PRSSI_R & SYSCTL_PRSSI_R0) != SYSCTL_PRSSI_R0);

	/* Enable the LED pin outputs */
	GPIO_PORTF_DIR_R |= 0x0E;
	/* Set the blue LED on */
	GPIO_PORTF_DATA_BITS_R[0x0E] = 0x04;

	//GPIO_PORTA_AFSEL_R;

	GPIO_PORTA_PCTL_R |= GPIO_PCTL_PA5_SSI0TX | GPIO_PCTL_PA4_SSI0RX |
		GPIO_PCTL_PA3_SSI0FSS | GPIO_PCTL_PA2_SSI0CLK;
	GPIO_PORTA_DEN_R;
	/* Set Freescale SPI, SPO = 0, SPH = 1 */
	SSI0_CR0_R = SSI_CR0_SPH | SSI_CR0_FRF_MOTO | SSI_CR0_DSS_8;
	SSI0_CR1_R = SSI_CR1_SSE;
	SSI0_SR_R;
	SSI0_CC_R;
	SSI0_CPSR_R;

	while (1);
	return 0;
}

